#include "LinearLayout.h"

using namespace h7_qt;

LinearLayout::LinearLayout()
{

}

void LinearLayout::onMeasure(int& w, int& h){
    int tw = 0;
    int th = 0;
    int vsize = m_views.size();
    if(m_vertical){
        for(int i = 0 ; i < vsize ; ++i){
            auto& v = m_views[i];
            v->measure(w, h);
            auto size = v->getBoxSize();
            if(size.width() > tw){
                tw = size.width();
            }
            th += size.height();
            if(i != vsize - 1){
                th += m_space;
            }
        }
    }else{
        for(int i = 0 ; i < vsize ; ++i){
            auto& v = m_views[i];
            v->measure(w, h);
            auto size = v->getBoxSize();
            if(size.height() > th){
                th = size.height();
            }
            tw += size.width();
            if(i != vsize - 1){
                tw += m_space;
            }
        }
    }
    w = tw;
    h = th;
}
void LinearLayout::onLayout(CRect r){
    int vsize = m_views.size();
    if(m_vertical){
        int top = r.top;
        for(int i = 0 ; i < vsize ; ++i){
            auto& v = m_views[i];
            auto size = v->getBoxSize();
            v->setPosition(Point(r.left + v->getMargin().left,
                                  top + v->getMargin().top));
            top += size.height();
            //space
            if(i != vsize - 1){
                top += m_space;
            }
        }
    }else{
        int left = r.left;
        for(int i = 0 ; i < vsize ; ++i){
            auto& v = m_views[i];
            auto size = v->getBoxSize();
            v->setPosition(Point(left + v->getMargin().left,
                                  r.top + v->getMargin().top));
            left += size.width();
            //space
            if(i != vsize - 1){
                left += m_space;
            }
        }
    }
}
