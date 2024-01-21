#include "View.h"
#include "Canvas.h"
#include "MotionEvent.h"

using namespace h7_qt;

View::View(){
    m_layoutP = std::make_shared<LayoutParams>();
    //setDrawingCacheEnabled(true);
}

View::~View(){
    if(m_cacheImg){
        delete m_cacheImg;
        m_cacheImg = nullptr;
    }
}
void View::setDrawingCacheEnabled(bool enable){
    m_drawCached = enable;
    if(m_cacheImg){
        delete m_cacheImg;
        m_cacheImg = nullptr;
    }
    if(enable){
        m_cacheImg = new Image();
        m_cacheImg->setBuilder([this](Canvas* c,int w, int h){
            c->drawImage(&m_bg, 0, 0);
            onDraw(c);
        });
        if(m_size.w != 0 && m_size.h != 0){
             m_cacheImg->prepareDraw(m_size.w, m_size.h);
        }
    }
}
void View::onSizeChanged(CSize s){
    if(m_drawCached){
        m_cacheImg->prepareDraw(s.w, s.h);
    }
}
void View::drawDirect(Canvas* c){
    //c->clipRect(Rect::ofXYWH(m_position.x, m_position.y, m_size.w, m_size.h));
    c->save();
    c->setCompositionModeDst();
    int& offsetX = m_position.x;
    int& offsetY = m_position.y;
    c->translate(offsetX, offsetY);
    c->drawImage(&m_bg, 0, 0);
    onDraw(c);
    c->translate(-offsetX, -offsetY);
    c->restore();
}

//TODO have bug? with 'setDrawingCacheEnabled'
void View::drawInCached(Canvas* c){
    if(m_cacheImg){
        PRINTLN("drawInCached: p.x,y = %d, %d", m_position.x, m_position.y);
        int& offsetX = m_position.x;
        int& offsetY = m_position.y;
        c->translate(offsetX, offsetY);
        c->drawImage(m_cacheImg, m_padding.left, m_padding.top);
        c->translate(-offsetX, -offsetY);
    }
}
bool View::hitTest(CPoint p){
    // e->x
    return Rect::ofXYWH(m_position.x, m_position.y, m_size.width(), m_size.height())
        .isPointIn(p.x, p.y);
}

//----------------------------------
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

