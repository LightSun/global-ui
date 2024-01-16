#include "FrameLayout.h"


using namespace h7_qt;

FrameLayout::FrameLayout() {

}

void FrameLayout::onMeasure(int& w, int& h){
    int tw = 0;
    int th = 0;
    int vsize = m_views.size();
    for(int i = 0 ; i < vsize ; ++i){
        auto& v = m_views[i];
        v->measure(w, h);
        auto size = v->getBoxSize();
        if(size.width() > tw){
            tw = size.width();
        }
        if(size.height() > th){
            th = size.height();
        }
    }
    w = tw;
    h = th;
}
void FrameLayout::onLayout(CRect r){
    int top = r.top + m_padding.top;
    int left = r.left + m_padding.left;
    //
    int vsize = m_views.size();
    int x_offset;
    int y_offset;
    for(int i = 0 ; i < vsize ; ++i){
        auto& v = m_views[i];
        auto size = v->getBoxSize();
        x_offset = Gravity::computeXOffsetWithMargin(r.width(), size.width(),
                                                     v->getMargin(), v->getLayoutParams()->gravity);
        y_offset = Gravity::computeYOffsetWithMargin(r.height(), size.height(),
                                                         v->getMargin(), v->getLayoutParams()->gravity);
        v->setPosition(Point(left + x_offset , top + y_offset));
    }
}
