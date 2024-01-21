#include "Canvas.h"
#include <QPainter>
#include <QPainterPath>

#include "view/view_common.h"

using namespace h7_qt;

using Self = Canvas;
using SelfR = Canvas&;

Canvas::Canvas(QWidget* w, const QRect& rect): m_w(w)
{
    m_p = new QPainter(w);
    m_p->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    m_p->setViewport(rect);
    //
    m_font = w->font();
    m_pen.setStyle(Qt::SolidLine);
    m_pen.setCapStyle(Qt::RoundCap);
    m_pen.setJoinStyle(Qt::RoundJoin);
}
Canvas::Canvas(QImage* img){
     m_p = new QPainter(img);
     m_p->setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

     m_pen.setStyle(Qt::SolidLine);
     m_pen.setCapStyle(Qt::RoundCap);
     m_pen.setJoinStyle(Qt::RoundJoin);
}
Canvas::~Canvas(){
    if(m_p){
        delete m_p;
        m_p = nullptr;
    }
}
void Self::setViewport(CQRect c){
    m_p->setViewport(c);
}
void Self::translate(int x, int y){
    m_p->translate(x, y);
}
void Self::clipRect(CRect rect){
    m_p->setClipRect(QRect(rect.left, rect.top, rect.width(), rect.height()), Qt::ReplaceClip);
}
void Self::save(){
    m_p->save();
}
void Self::restore(){
    m_p->restore();
}
void Self::setCompositionMode(int mode){
    m_p->setCompositionMode((QPainter::CompositionMode)mode);
}
void Self::setCompositionModeDst(){
    m_p->setCompositionMode(QPainter::CompositionMode_DestinationOver);
}
void Self::setFont(CQFont cf){
    m_font = cf;
}
void Self::applyStyle(){
    m_p->setBrush(Qt::NoBrush);
    m_p->setPen(m_pen);
    m_p->setFont(m_font);
}
void Self::drawLine(int x1, int y1 ,int x2, int y2){
    m_p->drawLine(x1, y1, x2, y2);
}
void Self::drawText(CQString text, CQRect rect){
    m_p->drawText(rect, Qt::AlignHCenter|Qt::AlignVCenter, text);
}
void Self::drawText(CQString text, CQRect rect, int align){
    m_p->drawText(rect, align, text);
}
void Self::drawText(CQString text, Rect* rect, int alignFlags){
    m_p->drawText(QRect(rect->left, rect->top, rect->width(), rect->height()),
                  alignFlags, text);
}
void Self::drawText(CQString text, Rect* rect){
    m_p->drawText(QRect(rect->left, rect->top, rect->width(), rect->height()),
                  text);
}
void Self::drawImage(Image* pm, int left, int top){
    pm->draw(this, left, top);
}

void Self::drawText(CQString text, int left, int top){
    int h = m_p->fontMetrics().height();
    int w = m_p->fontMetrics().horizontalAdvance(text);
    m_p->drawText(QRect(left, top, w, h),
                  Qt::AlignHCenter|Qt::AlignVCenter,
                  text);
}
void Self::drawRect(CQRect rect, float xRadius, float yRadius,
                     QColor* fill_color){
    QPainterPath path;
    path.addRoundedRect(rect, xRadius, yRadius);
    if(fill_color){
        //m_p->setPen(Qt::NoPen);
        m_p->setBrush(QBrush(*fill_color, Qt::SolidPattern));
        m_p->fillPath(path, *fill_color);
        m_p->drawPath(path);
        m_p->setBrush(Qt::NoBrush);
    }else{
        m_p->drawPath(path);
    }
}
void Self::drawEllipse(CQRect rect, QColor* fill_color){
    QPainterPath path;
    path.addEllipse(rect);
    if(fill_color){
        //m_p->setPen(Qt::NoPen);
        m_p->setBrush(QBrush(*fill_color, Qt::SolidPattern));
        m_p->fillPath(path, *fill_color);
        m_p->drawPath(path);
        m_p->setBrush(Qt::NoBrush);
    }else{
        m_p->drawPath(path);
    }
}
void Self::drawPolygonF(const QPolygonF& pf, QColor* fill_color){
    QPainterPath path;
    path.addPolygon(pf);
    if(fill_color){
        //m_p->setPen(Qt::NoPen);
        m_p->setBrush(QBrush(*fill_color, Qt::SolidPattern));
        m_p->fillPath(path, *fill_color);
        m_p->drawPath(path);
        m_p->setBrush(Qt::NoBrush);
    }else{
        m_p->drawPath(path);
    }
}
void Self::drawPixmap(const QPixmap& pm,int left, int top){
    m_p->drawPixmap(left, top, pm);
}
void Self::addRect(CQRect rect, float xRadius, float yRadius){
    m_path.addRoundedRect(rect, xRadius, yRadius);
}
void Self::addPolygonF(const QPolygonF& pf){
    m_path.addPolygon(pf);
}
void Self::addText(const QPointF &point, const QFont &f, const QString &text){
    m_path.addText(point, f, text);
}
void Self::drawPath(QColor* fill_color){
    if(fill_color){
        //m_p->setPen(Qt::NoPen);
        m_p->setBrush(QBrush(*fill_color, Qt::SolidPattern));
        m_p->fillPath(m_path, *fill_color);
        m_p->drawPath(m_path);
        m_p->setBrush(Qt::NoBrush);
    }else{
        m_p->drawPath(m_path);
    }
    m_path = QPainterPath();
}
void Self::end(){
    m_p->end();
}
