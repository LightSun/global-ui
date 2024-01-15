#pragma once

#include <QWidget>
#include <QRect>
#include <QPen>
#include <QPainterPath>

namespace h7_qt {

struct Rect;
class Image;

class Canvas
{
public:
    using SelfR = Canvas&;
    using CQColor = const QColor&;
    using CQRect = const QRect&;
    using CQString = const QString&;
    using CQFont = const QFont&;
    using CRect = const Rect&;
    using CImage = const Image&;

    Canvas(QWidget* w, const QRect& rect);
    Canvas(QImage* img);
    ~Canvas();

    void setViewport(CQRect c);
    void translate(int dx, int dy);

    void setFont(CQFont cf);

    SelfR setStrokeSize(float stroke){
        m_font.setPixelSize(stroke);
        m_pen.setWidthF(stroke);
        return *this;
    }
    SelfR setColor(CQColor c){
        m_pen.setColor(c);
        return *this;
    }
    ///apply pen, font.
    SelfR applyStyle();
    //self
    SelfR drawText(CQString text, Rect* rect);
    SelfR drawText(CQString text, Rect* rect, int alignFlags);
    SelfR drawImage(Image* pm, int left, int top);

    SelfR drawText(CQString text, CRect rect){
        return drawText(text, (Rect*)&rect);
    }
    SelfR drawText(CQString text, CRect rect, int alignFlags){
        return drawText(text, (Rect*)&rect, alignFlags);
    }
    SelfR drawImage(CImage pm, int left, int top){
        return drawImage((Image*)&pm, left, top);
    }
    //
    SelfR drawLine(int x1, int y1 ,int x2, int y2);
    SelfR drawText(CQString text, CQRect rect);
    SelfR drawText(CQString text, CQRect rect, int alignFlags);
    SelfR drawText(CQString text, int left, int top);
    //fill_color: null means just stroke
    //xRadius, yRadius: for round rect
    SelfR drawRect(CQRect rect, float xRadius = 0, float yRadius = 0,
                   QColor* fill_color = nullptr);
    SelfR drawEllipse(CQRect rect, QColor* fill_color = nullptr);
    SelfR drawPolygonF(const QPolygonF& pf, QColor* fill_color = nullptr);
    SelfR drawPixmap(const QPixmap& pm, int left, int top);

    SelfR addRect(CQRect rect, float xRadius = 0, float yRadius = 0);
    SelfR addPolygonF(const QPolygonF& pf);
    SelfR addText(const QPointF &point, const QFont &f, const QString &text);
   // SelfR addPolygonF(const QPolygonF& pf);
    QPainterPath& getPath(){return m_path;}
    void drawPath(QColor* fill_color = nullptr);

    void end();

private:
    QPainter* m_p;
    QWidget* m_w;
    QPen m_pen;
    QFont m_font;
    QPainterPath m_path;
};

}

