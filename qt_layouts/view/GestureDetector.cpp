#include "GestureDetector.h"
#include <math.h>
#include "ViewConfiguration.h"
#include "MotionEvent.h"

#include "input/VelocityTracker.h"
#include "handler-os/HandlerOS.h"
#include "handler-os/Message.h"
#include "core/h7_common.h"

using namespace h7;
using namespace h7_qt;
using namespace h7_handler_os;
using SPHandler = std::shared_ptr<h7_handler_os::Handler>;

static int LONGPRESS_TIMEOUT = ViewConfiguration::getLongPressTimeout();
static int TAP_TIMEOUT = ViewConfiguration::getTapTimeout();
static int DOUBLE_TAP_TIMEOUT = ViewConfiguration::getDoubleTapTimeout();

enum{
    SHOW_PRESS = 1,
    LONG_PRESS = 2,
    TAP = 3
};

struct GestureDetectorCompatImpl {
    virtual boolean isLongpressEnabled() = 0;
    virtual boolean onTouchEvent(MotionEventPtr ev) = 0;
    virtual void setIsLongpressEnabled(boolean enabled) = 0;
    virtual void setOnDoubleTapListener(OnDoubleTapListener* listener) = 0;
};

namespace h7_qt {

class GestureDetectorImpl: public GestureDetectorCompatImpl, public HandlerCallbackI{

private:
    int mTouchSlopSquare;
    int mDoubleTapSlopSquare;
    int mMinimumFlingVelocity;
    int mMaximumFlingVelocity;

private:
    SPHandler mHandler;
    OnGestureListener* mListener {nullptr};
    OnDoubleTapListener* mDoubleTapListener {nullptr};

    boolean mStillDown {false};
    boolean mDeferConfirmSingleTap  {false};
    boolean mInLongPress  {false};
    boolean mAlwaysInTapRegion {false};
    boolean mAlwaysInBiggerTapRegion {false};

    MotionEventPtr mCurrentDownEvent {nullptr};
    MotionEventPtr mPreviousUpEvent {nullptr};
    /**
    * True when the user is still touching for the second tap (down, move, and
    * up events). Can only be true if there is a double tap listener attached.
    */
    boolean mIsDoubleTapping {false};

    float mLastFocusX {0};
    float mLastFocusY {0};
    float mDownFocusX {0};
    float mDownFocusY {0};
    boolean mIsLongpressEnabled {false};

    VelocityTracker* mVelocityTracker {nullptr};
public:
    GestureDetectorImpl(OnGestureListener* listener,
                                  Looper* looper){
        ASSERT_FMT(listener, "OnGestureListener can't be null");
        ASSERT_FMT(looper, "looper can't be null");
        mHandler = std::make_shared<Handler>(looper, std::make_shared<HandlerCallback>(this));
        mListener = listener;
        init();
    }
    virtual ~GestureDetectorImpl(){
        if(mVelocityTracker){
            mVelocityTracker->recycle();
            mVelocityTracker = nullptr;
        }
        if(mCurrentDownEvent){
            mCurrentDownEvent->recycle();
            mCurrentDownEvent = nullptr;
        }
        if(mPreviousUpEvent){
            mPreviousUpEvent->recycle();
            mPreviousUpEvent = nullptr;
        }
    }

    bool handleMessage(Message* m)override{
        switch (m->what) {
            case SHOW_PRESS:{
                mListener->onShowPress(mCurrentDownEvent);
                return true;
            }break;

            case LONG_PRESS:{
                dispatchLongPress();
                return true;
            }break;
            case TAP:{
                // If the user's finger is still down, do not count it as a tap
                if (mDoubleTapListener) {
                    if (!mStillDown) {
                        mDoubleTapListener->onSingleTapConfirmed(mCurrentDownEvent);
                    } else {
                        mDeferConfirmSingleTap = true;
                    }
                    return true;
                }
            }break;
        }
        return false;
    }
    boolean onTouchEvent(MotionEventPtr e) override{
        auto& ev = *e;
        int action = ev.getActionMasked();
        if(mVelocityTracker == nullptr){
            mVelocityTracker = VelocityTracker::obtain();
        }
        mVelocityTracker->add(ev.getLastPointerId(), ev.getX(), ev.getY(), ev.getEventTime());
        //
        boolean pointerUp = action == MotionEvent::ACTION_POINTER_UP;
        int skipIndex = pointerUp ? ev.getActionIndex() : -1;
        // Determine focal point
       float sumX = 0, sumY = 0;
       int count = ev.getPointerCount();
       for (int i = 0; i < count; i++) {
           if (skipIndex == i) continue;
           sumX += ev.getX(i);
           sumY += ev.getY(i);
       }
       int div = pointerUp ? count - 1 : count;
       float focusX = sumX / div;
       float focusY = sumY / div;

       boolean handled = false;
       switch (action) {
           case MotionEvent::ACTION_POINTER_DOWN:{
               mDownFocusX = mLastFocusX = focusX;
               mDownFocusY = mLastFocusY = focusY;
               // Cancel long press and taps
               cancelTaps();
               break;
            }

           case MotionEvent::ACTION_POINTER_UP:{
               mDownFocusX = mLastFocusX = focusX;
               mDownFocusY = mLastFocusY = focusY;

               // Check the dot product of current velocities.
               // If the pointer that left was opposing another velocity vector, clear.
               mVelocityTracker->computeCurrentVelocity(1000, mMaximumFlingVelocity);
               int upIndex = ev.getActionIndex();
               int id1 = ev.getPointerId(upIndex);
               float x1 = mVelocityTracker->getXVelocity(id1);
               float y1 = mVelocityTracker->getYVelocity(id1);
               for (int i = 0; i < count; i++) {
                   if (i == upIndex) continue;

                   int id2 = ev.getPointerId(i);
                   float x = x1 * mVelocityTracker->getXVelocity(id2);
                   float y = y1 * mVelocityTracker->getYVelocity(id2);

                   float dot = x + y;
                   if (dot < 0) {
                       mVelocityTracker->clear();
                       break;
                   }
               }
               break;
         }

       case MotionEvent::ACTION_DOWN:{
               if (mDoubleTapListener != nullptr) {
                   boolean hadTapMessage = mHandler->hasMessages(TAP);
                   if (hadTapMessage) mHandler->removeMessages(TAP);
                   if ((mCurrentDownEvent != nullptr) && (mPreviousUpEvent != nullptr)
                           && hadTapMessage && isConsideredDoubleTap(
                                   mCurrentDownEvent, mPreviousUpEvent, e)) {
                       // This is a second tap
                       mIsDoubleTapping = true;
                       // Give a callback with the first tap of the double-tap
                       handled |= mDoubleTapListener->onDoubleTap(mCurrentDownEvent);
                       // Give a callback with down event of the double-tap
                       handled |= mDoubleTapListener->onDoubleTapEvent(e);
                   } else {
                       // This is a first tap
                       mHandler->sendEmptyMessageDelayed(TAP, DOUBLE_TAP_TIMEOUT);
                   }
               }

               mDownFocusX = mLastFocusX = focusX;
               mDownFocusY = mLastFocusY = focusY;
               if (mCurrentDownEvent != nullptr) {
                   mCurrentDownEvent->recycle();
               }
               mCurrentDownEvent = MotionEvent::obtain(e);
               mAlwaysInTapRegion = true;
               mAlwaysInBiggerTapRegion = true;
               mStillDown = true;
               mInLongPress = false;
               mDeferConfirmSingleTap = false;

               if (mIsLongpressEnabled) {
                   mHandler->removeMessages(LONG_PRESS);
                   mHandler->sendEmptyMessageAtTime(LONG_PRESS, mCurrentDownEvent->getDownTime()
                           + TAP_TIMEOUT + LONGPRESS_TIMEOUT);
               }
               mHandler->sendEmptyMessageAtTime(SHOW_PRESS,
                       mCurrentDownEvent->getDownTime() + TAP_TIMEOUT);
               handled |= mListener->onDown(e);
               break;
       }

       case MotionEvent::ACTION_MOVE:{
               if (mInLongPress) {
                   break;
               }
               float scrollX = mLastFocusX - focusX;
               float scrollY = mLastFocusY - focusY;
               if (mIsDoubleTapping) {
                   // Give the move events of the double-tap
                   handled |= mDoubleTapListener->onDoubleTapEvent(e);
               } else if (mAlwaysInTapRegion) {
                   int deltaX = (int) (focusX - mDownFocusX);
                   int deltaY = (int) (focusY - mDownFocusY);
                   int distance = (deltaX * deltaX) + (deltaY * deltaY);
                   if (distance > mTouchSlopSquare) {
                       handled = mListener->onScroll(mCurrentDownEvent, e, scrollX, scrollY);
                       mLastFocusX = focusX;
                       mLastFocusY = focusY;
                       mAlwaysInTapRegion = false;
                       mHandler->removeMessages(TAP);
                       mHandler->removeMessages(SHOW_PRESS);
                       mHandler->removeMessages(LONG_PRESS);
                   }
                   if (distance > mTouchSlopSquare) {
                       mAlwaysInBiggerTapRegion = false;
                   }
               } else if ((std::abs(scrollX) >= 1) || (std::abs(scrollY) >= 1)) {
                   handled = mListener->onScroll(mCurrentDownEvent, e, scrollX, scrollY);
                   mLastFocusX = focusX;
                   mLastFocusY = focusY;
               }
               break;
       }

       case MotionEvent::ACTION_UP:{
               mStillDown = false;
               auto currentUpEvent = MotionEvent::obtain(e);
               if (mIsDoubleTapping) {
                   // Finally, give the up event of the double-tap
                   handled |= mDoubleTapListener->onDoubleTapEvent(e);
               } else if (mInLongPress) {
                   mHandler->removeMessages(TAP);
                   mInLongPress = false;
               } else if (mAlwaysInTapRegion) {
                   handled = mListener->onSingleTapUp(e);
                   if (mDeferConfirmSingleTap && mDoubleTapListener != nullptr) {
                       mDoubleTapListener->onSingleTapConfirmed(e);
                   }
               } else {
                   // A fling must travel the minimum tap distance
                   auto velocityTracker = mVelocityTracker;
                   int pointerId = ev.getPointerId(0);
                   velocityTracker->computeCurrentVelocity(1000, mMaximumFlingVelocity);
                   float velocityY = velocityTracker->getYVelocity(pointerId);
                   float velocityX = velocityTracker->getXVelocity(pointerId);

                   if ((std::abs(velocityY) > mMinimumFlingVelocity)
                           || (std::abs(velocityX) > mMinimumFlingVelocity)) {
                       handled = mListener->onFling(
                               mCurrentDownEvent, e, velocityX, velocityY);
                   }
               }
               if (mPreviousUpEvent != nullptr) {
                   mPreviousUpEvent->recycle();
               }
               // Hold the event we obtained above - listeners may have changed the original.
               mPreviousUpEvent = currentUpEvent;
               if (mVelocityTracker != nullptr) {
                   // This may have been cleared when we called out to the
                   // application above.
                   mVelocityTracker->recycle();
                   mVelocityTracker = nullptr;
               }
               mIsDoubleTapping = false;
               mDeferConfirmSingleTap = false;
               mHandler->removeMessages(SHOW_PRESS);
               mHandler->removeMessages(LONG_PRESS);
               break;
       }

           case MotionEvent::ACTION_CANCEL:
               cancel();
               break;
        }

        return handled;
    }

    boolean isLongpressEnabled() override{
        return mIsLongpressEnabled;
    }
    void setIsLongpressEnabled(boolean enabled) override{
        mIsLongpressEnabled = enabled;
    }
    void setOnDoubleTapListener(OnDoubleTapListener* listener) override{
        mDoubleTapListener = listener;
    }

private:
    void init(){
        mIsLongpressEnabled = true;
        //final ViewConfiguration configuration = ViewConfiguration.get(context);
        int touchSlop = ViewConfiguration::getScaledTouchSlop();
        int doubleTapSlop = ViewConfiguration::getScaledDoubleTapSlop();
        mMinimumFlingVelocity = ViewConfiguration::getScaledMinimumFlingVelocity();
        mMaximumFlingVelocity = ViewConfiguration::getScaledMaximumFlingVelocity();

        mTouchSlopSquare = touchSlop * touchSlop;
        mDoubleTapSlopSquare = doubleTapSlop * doubleTapSlop;
    }
    boolean isConsideredDoubleTap(MotionEvent* firstDown, MotionEvent* firstUp,
            MotionEvent* secondDown) {
        if (!mAlwaysInBiggerTapRegion) {
            return false;
        }

        if (secondDown->getEventTime() - firstUp->getEventTime() > DOUBLE_TAP_TIMEOUT) {
            return false;
        }

        int deltaX = (int) firstDown->getX() - (int) secondDown->getX();
        int deltaY = (int) firstDown->getY() - (int) secondDown->getY();
        return (deltaX * deltaX + deltaY * deltaY < mDoubleTapSlopSquare);
    }
    void cancel(){
       auto& mHandler = *this->mHandler;
       mHandler.removeMessages(SHOW_PRESS);
       mHandler.removeMessages(LONG_PRESS);
       mHandler.removeMessages(TAP);
       if(mVelocityTracker){
           mVelocityTracker->recycle();
           mVelocityTracker = nullptr;
       }
       mIsDoubleTapping = false;
       mStillDown = false;
       mAlwaysInTapRegion = false;
       mAlwaysInBiggerTapRegion = false;
       mDeferConfirmSingleTap = false;
       if (mInLongPress) {
           mInLongPress = false;
       }
    }
    void cancelTaps(){
        auto& mHandler = *this->mHandler;
        mHandler.removeMessages(SHOW_PRESS);
        mHandler.removeMessages(LONG_PRESS);
        mHandler.removeMessages(TAP);
        mIsDoubleTapping = false;
        mAlwaysInTapRegion = false;
        mAlwaysInBiggerTapRegion = false;
        mDeferConfirmSingleTap = false;
        if (mInLongPress) {
            mInLongPress = false;
        }
    }
    void dispatchLongPress(){
       mHandler->removeMessages(TAP);
       mDeferConfirmSingleTap = false;
       mInLongPress = true;
       mListener->onLongPress(mCurrentDownEvent);
    }
};

}

GestureDetector::GestureDetector(OnGestureListener* l, Looper* looper)
{
    // if(looper == nullptr){
    //     looper = Looper::getMainLooper().get();
    // }
    m_impl = new GestureDetectorImpl(l, looper);
}
GestureDetector::~GestureDetector(){
    if(m_impl){
        delete m_impl;
        m_impl = nullptr;
    }
}
boolean GestureDetector::isLongpressEnabled(){
    return m_impl->isLongpressEnabled();
}

boolean GestureDetector::onTouchEvent(MotionEvent* event){
    return m_impl->onTouchEvent(event);
}
void GestureDetector::setIsLongpressEnabled(boolean enabled){
    m_impl->setIsLongpressEnabled(enabled);
}
void GestureDetector::setOnDoubleTapListener(OnDoubleTapListener* listener){
    m_impl->setOnDoubleTapListener(listener);
}

