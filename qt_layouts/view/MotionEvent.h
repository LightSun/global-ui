#pragma once

#include <map>
#include <vector>
#include "view_common.h"

namespace h7_qt {
struct MotionEvent;
using SPMotionEvent = std::shared_ptr<MotionEvent>;

struct PointerInfo{
    int id;
    int x, y;
    long long time;
};

struct MotionEvent
{
    enum{
        ACTION_DOWN = 0,
        ACTION_UP = 1,
        ACTION_MOVE = 2,
        ACTION_CANCEL = 3,
        ACTION_POINTER_DOWN = 5,
        ACTION_POINTER_UP = 6,

        ACTION_MASK = 0xff,
        ACTION_POINTER_INDEX_MASK = 0xff00,
        ACTION_POINTER_INDEX_SHIFT = 8,
    };

    using LL = long long;
    using Map = std::map<int,PointerInfo>; //<id,>

    MotionEvent();
    MotionEvent(const MotionEvent& e);

    static MotionEvent* obtain();
    static MotionEvent* obtain(MotionEvent* me);
    static MotionEvent* obtain(int action, int pointerId, int x, int y);
    void recycle();
    void clear();
    void cancel(){setAction(ACTION_CANCEL);}

    void add(int pointerId, int x, int y, LL timeMs);
    void add(int pointerId, int x, int y);

    int findPointerIndex(int pointerId);
    int getPointerId(int index);
    int getPointerCount(){return m_infoMap.size();}
    LL getDownTime(){return m_downTime;}
    LL getEventTime();

    int getActionMasked(){
        return m_action & ACTION_MASK;
    }
    int getActionIndex(){
        return (m_action & ACTION_POINTER_INDEX_MASK)
                >> ACTION_POINTER_INDEX_SHIFT;
    }
    void setAction(int action, int index = 0){
        m_action = (index << ACTION_POINTER_INDEX_SHIFT) | action;
    }
    int getLastPointerId(){return m_lastPointerId;}
    int getX(int index = -1);
    int getY(int index = -1);
    //
    SPMotionEvent copy(){
        return std::make_shared<MotionEvent>(*this);
    }
    void copyTo(MotionEvent* e);
    MotionEvent& operator=(const MotionEvent& e);

private:
    Map m_infoMap;
    int m_action;
    int m_lastPointerId {-1};
    LL m_downTime {0};
};

using MotionEventPtr = MotionEvent*;

}

