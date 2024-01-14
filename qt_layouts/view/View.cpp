#include "View.h"
#include "Canvas.h"
#include "MotionEvent.h"

using namespace h7_qt;


bool View::hitTest(CPoint p){
    // e->x
    return Rect::ofXYWH(m_position.x, m_position.y, m_size.width(), m_size.height()).isPointIn(p.x, p.y);
}
bool ViewGroup::dispatchTouchEvent(MotionEvent* ev){
    bool handled = false;
    if(ev->getActionMasked() != MotionEvent::ACTION_DOWN){
        handled = m_touched_view && m_touched_view->onTouchEvent(ev);
    }
    if(ev->getActionMasked() == MotionEvent::ACTION_UP){
        m_touched_view = nullptr;
    }
    if(handled) return true;
    //
    if(onTouchEvent(ev)){
        m_touched_view = this;
        return true;
    }
    Point p(ev->getX(), ev->getY());
    int size = m_views.size();
    for(int i = 0 ; i < size ; ++i){
        if(m_views[i]->hitTest(p)){
            m_touched_view = m_views[i].get();
            return true;
        }
    }
    return false;
}

