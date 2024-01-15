#include "ButtonView.h"
#include "view/Canvas.h"

using namespace h7_qt;

ButtonView::ButtonView()
{
    Image img;
    img.setBuilder([](Canvas* c, int w, int h){
        c->setColor(Qt::black);
        c->setStrokeSize(1);
        c->applyStyle();
        c->drawRect(QRect(1, 1, w - 2, h - 2), (h - 2) / 2.0f, (h - 2) / 2.0f);
    });
    setBackground(img);
}

//void ButtonView::onDraw(Canvas* c){

//}
