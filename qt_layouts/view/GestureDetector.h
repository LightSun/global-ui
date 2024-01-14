#pragma once

#include "view_common.h"

namespace h7_handler_os {
    class Looper;
}

namespace h7_qt {

class GestureDetectorImpl;
class MotionEvent;
using MotionEventPtr = MotionEvent*;

class OnGestureListener{
public:
    virtual ~OnGestureListener(){}
    /**
     * Notified when a tap occurs with the down {@link MotionEvent}
     * that triggered it. This will be triggered immediately for
     * every down event. All other events should be preceded by this.
     *
     * @param e The down motion event.
     */
    virtual boolean onDown(MotionEventPtr e) = 0;

    /**
     * The user has performed a down {@link MotionEvent} and not performed
     * a move or up yet. This event is commonly used to provide visual
     * feedback to the user to let them know that their action has been
     * recognized i.e. highlight an element.
     *
     * @param e The down motion event
     */
    virtual void onShowPress(MotionEventPtr e) = 0;

    /**
     * Notified when a tap occurs with the up {@link MotionEvent}
     * that triggered it.
     *
     * @param e The up motion event that completed the first tap
     * @return true if the event is consumed, else false
     */
    virtual boolean onSingleTapUp(MotionEventPtr e) = 0;

    /**
     * Notified when a scroll occurs with the initial on down {@link MotionEvent} and the
     * current move {@link MotionEvent}. The distance in x and y is also supplied for
     * convenience.
     *
     * @param e1 The first down motion event that started the scrolling.
     * @param e2 The move motion event that triggered the current onScroll.
     * @param distanceX The distance along the X axis that has been scrolled since the last
     *              call to onScroll. This is NOT the distance between {@code e1}
     *              and {@code e2}.
     * @param distanceY The distance along the Y axis that has been scrolled since the last
     *              call to onScroll. This is NOT the distance between {@code e1}
     *              and {@code e2}.
     * @return true if the event is consumed, else false
     */
    virtual boolean onScroll(MotionEventPtr e1, MotionEventPtr e2, float distanceX, float distanceY) = 0;

    /**
     * Notified when a long press occurs with the initial on down {@link MotionEvent}
     * that trigged it.
     *
     * @param e The initial on down motion event that started the longpress.
     */
     virtual void onLongPress(MotionEventPtr e) = 0;

    /**
     * Notified of a fling event when it occurs with the initial on down {@link MotionEvent}
     * and the matching up {@link MotionEvent}. The calculated velocity is supplied along
     * the x and y axis in pixels per second.
     *
     * @param e1 The first down motion event that started the fling.
     * @param e2 The move motion event that triggered the current onFling.
     * @param velocityX The velocity of this fling measured in pixels per second
     *              along the x axis.
     * @param velocityY The velocity of this fling measured in pixels per second
     *              along the y axis.
     * @return true if the event is consumed, else false
     */
    virtual boolean onFling(MotionEventPtr e1, MotionEventPtr e2, float velocityX, float velocityY) = 0;
};

/**
* The listener that is used to notify when a double-tap or a confirmed
* single-tap occur.
*/
class OnDoubleTapListener {
public:
    virtual ~OnDoubleTapListener(){}
   /**
    * Notified when a single-tap occurs.
    * <p>
    * Unlike {@link OnGestureListener#onSingleTapUp(MotionEvent)}, this
    * will only be called after the detector is confident that the user's
    * first tap is not followed by a second tap leading to a double-tap
    * gesture.
    *
    * @param e The down motion event of the single-tap.
    * @return true if the event is consumed, else false
    */
   virtual boolean onSingleTapConfirmed(MotionEventPtr e) = 0;

   /**
    * Notified when a double-tap occurs.
    *
    * @param e The down motion event of the first tap of the double-tap.
    * @return true if the event is consumed, else false
    */
   virtual boolean onDoubleTap(MotionEventPtr e) = 0;

   /**
    * Notified when an event within a double-tap gesture occurs, including
    * the down, move, and up events.
    *
    * @param e The motion event that occurred during the double-tap gesture.
    * @return true if the event is consumed, else false
    */
   virtual boolean onDoubleTapEvent(MotionEventPtr e) = 0;
};

class SimpleOnGestureListener: public OnGestureListener, public OnDoubleTapListener{
public:
    virtual ~SimpleOnGestureListener(){}
    virtual boolean onSingleTapUp(MotionEventPtr e) override {
       return false;
   }

   virtual void onLongPress(MotionEventPtr e) override{
   }

   virtual boolean onScroll(MotionEventPtr e1, MotionEventPtr e2,
           float distanceX, float distanceY) override{
       return false;
   }

   virtual boolean onFling(MotionEventPtr e1, MotionEventPtr e2, float velocityX,
           float velocityY) override{
       return false;
   }

   virtual void onShowPress(MotionEventPtr e) override{
   }

   virtual boolean onDown(MotionEventPtr e) override{
       return false;
   }

   virtual boolean onDoubleTap(MotionEventPtr e) override{
       return false;
   }

   virtual boolean onDoubleTapEvent(MotionEventPtr e) override{
       return false;
   }

   virtual boolean onSingleTapConfirmed(MotionEventPtr e) override{
       return false;
   }
};

class GestureDetector
{
public:
    using Looper = h7_handler_os::Looper;
    GestureDetector(OnGestureListener* l, Looper* looper);
    ~GestureDetector();

    boolean isLongpressEnabled();
   /**
    * Analyzes the given motion event and if applicable triggers the
    * appropriate callbacks on the {@link OnGestureListener} supplied.
    *
    * @param event The current motion event.
    * @return true if the {@link OnGestureListener} consumed the event,
    *              else false.
    */
   boolean onTouchEvent(MotionEvent* event);

   /**
    * Set whether longpress is enabled, if this is enabled when a user
    * presses and holds down you get a longpress event and nothing further.
    * If it's disabled the user can press and hold down and then later
    * moved their finger and you will get scroll events. By default
    * longpress is enabled.
    *
    * @param enabled whether longpress should be enabled.
    */
   void setIsLongpressEnabled(boolean enabled);

   /**
    * Sets the listener which will be called for double-tap and related
    * gestures.
    *
    * @param listener the listener invoked for all the callbacks, or
    *        null to stop listening for double-tap gestures.
    */
   void setOnDoubleTapListener(OnDoubleTapListener* listener);

private:
    GestureDetectorImpl* m_impl;
};


}

