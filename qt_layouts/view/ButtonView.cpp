#include "ButtonView.h"
#include "view/Canvas.h"

using namespace h7_qt;

ButtonView::ButtonView()
{
    Image img;
    img.setBuilder([](Canvas* c, int w, int h){
        c->setColor(Qt::black);
        c->applyStyle();
        c->drawRect(QRect(0, 0, w, h), h / 2.0f, h / 2.0f);
    });
    setBackground(img);
}

//void ButtonView::onDraw(Canvas* c){

//}
