#include "ContainerWidget.h"
#include "View.h"
#include "Canvas.h"

ContainerWidget::ContainerWidget(QWidget* p):QWidget(p)
{

}

void ContainerWidget::measure(){
    if(m_vg){
        m_vg->measure(width(), height());
    }
}

void ContainerWidget::layout(){
    if(m_vg){
        m_vg->layout(h7_qt::Rect(0, 0, width(), height()));
    }
}

void ContainerWidget::paintEvent(QPaintEvent *){
    if(m_vg){
        h7_qt::Canvas c(this, QRect(0, 0, width(), height()));
        m_vg->draw(&c);
    }
}
void ContainerWidget::installEvent(){
    installEventFilter(this); //为窗体安装事件过滤器
    //实时跟踪鼠标, 不然默认上层子控件获取鼠标事件，窗体不处理鼠标移动事件
    setMouseTracking(true);
    //需要捕获的控件都要setMouseTracking(true)
    setMouseTracking(true);
}
void ContainerWidget::uninstallEvent(){
    setMouseTracking(false);
    setMouseTracking(false);
    removeEventFilter(this);
}
