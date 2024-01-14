#include "MotionEvent.h"
#include "core/System.h"
#include "core/SynchronizedPool.h"

using namespace h7_qt;

static h7::SynchronizedPool<MotionEvent> sPool;

MotionEvent::MotionEvent()
{

}
MotionEvent::MotionEvent(const MotionEvent& e){
    *this = e;
}
MotionEvent* MotionEvent::obtain(){
    return sPool.acquire();
}
MotionEvent* MotionEvent::obtain(MotionEvent* me){
    auto ptr = obtain();
    *ptr = *me;
    return ptr;
}
MotionEvent* MotionEvent::obtain(int action, int pointerId, int x, int y){
    auto ptr = obtain();
    ptr->add(pointerId, x, y);
    ptr->setAction(action, 0);
    return ptr;
}

void MotionEvent::recycle(){
    clear();
    sPool.release(this);
}
void MotionEvent::clear(){
    m_infoMap.clear();
    m_action = 0;
    m_downTime = 0;
    m_lastPointerId = -1;
}
void MotionEvent::copyTo(MotionEvent* e){
   *e = *this;
}
MotionEvent& MotionEvent::operator=(const MotionEvent& e){
    this->m_infoMap = e.m_infoMap;
    this->m_action = e.m_action;
    this->m_downTime = e.m_downTime;
    this->m_lastPointerId = e.m_lastPointerId;
    return *this;
}

void MotionEvent::add(int pointerId, int x, int y){
    add(pointerId, x, y, System::currentMills());
}
void MotionEvent::add(int pointerId, int x, int y, LL timeMs){
    auto it = m_infoMap.find(pointerId);
    if(it != m_infoMap.end()){
        it->second.x = x;
        it->second.y = y;
        it->second.time = timeMs;
    }else{
        PointerInfo info;
        info.id = pointerId;
        info.x = x;
        info.y = y;
        info.time = timeMs;
        m_infoMap[pointerId] = std::move(info);
    }
    if(m_downTime == 0){
        m_downTime = timeMs;
    }
    m_lastPointerId = pointerId;
}
int MotionEvent::getPointerId(int index){
    if(m_infoMap.size() > 0){
        auto it = m_infoMap.begin();
        for(int i = index ; i > 0; --i){
            ++it;
            if(it == m_infoMap.end()){
                return -1;
            }
        }
        return it->second.id;
    }
    return -1;
}
MotionEvent::LL MotionEvent::getEventTime(){
    auto it = m_infoMap.find(m_lastPointerId);
    return it != m_infoMap.end() ? it->second.time : 0;
}
int MotionEvent::findPointerIndex(int pointerId){
    auto it = m_infoMap.begin();
    int idx = 0;
    while (it != m_infoMap.end()) {
        if(it->second.id == pointerId){
            return idx;
        }
        idx ++;
        ++it;
    }
    return -1;
}
int MotionEvent::getX(int index){
    //use latest
    if(index < 0){
        auto it = m_infoMap.find(m_lastPointerId);
        return it != m_infoMap.end() ? it->second.x : 0;
    }
    auto it = m_infoMap.begin();
    for(int i = index ; i > 0; --i){
        ++it;
        if(it == m_infoMap.end()){
            return -1;
        }
    }
    return it->second.x;
}
int MotionEvent::getY(int index){
    //use latest
    if(index < 0){
        auto it = m_infoMap.find(m_lastPointerId);
        return it != m_infoMap.end() ? it->second.y : 0;
    }
    auto it = m_infoMap.begin();
    for(int i = index ; i > 0; --i){
        ++it;
        if(it == m_infoMap.end()){
            return -1;
        }
    }
    return it->second.y;
}
