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
    void clipRect(CRect rect);
    void save();
    void restore();
    void setCompositionMode(int mode);
    void setCompositionModeDst();

    void setFont(CQFont cf);

    void setStrokeSize(float stroke){
        m_font.setPixelSize(stroke);
        m_pen.setWidthF(stroke);
    }
    void setColor(CQColor c){
        m_pen.setColor(c);
    }
    ///apply pen, font.
    void applyStyle();
    //self
    void drawText(CQString text, Rect* rect);
    void drawText(CQString text, Rect* rect, int alignFlags);
    void drawImage(Image* pm, int left, int top);

    void drawText(CQString text, CRect rect){
        drawText(text, (Rect*)&rect);
    }
    void drawText(CQString text, CRect rect, int alignFlags){
        drawText(text, (Rect*)&rect, alignFlags);
    }
    void drawImage(CImage pm, int left, int top){
        drawImage((Image*)&pm, left, top);
    }
    //
    void drawLine(int x1, int y1 ,int x2, int y2);
    void drawText(CQString text, CQRect rect);
    void drawText(CQString text, CQRect rect, int alignFlags);
    void drawText(CQString text, int left, int top);
    //fill_color: null means just stroke
    //xRadius, yRadius: for round rect
    void drawRect(CQRect rect, float xRadius = 0, float yRadius = 0,
                   QColor* fill_color = nullptr);
    void drawEllipse(CQRect rect, QColor* fill_color = nullptr);
    void drawPolygonF(const QPolygonF& pf, QColor* fill_color = nullptr);
    void drawPixmap(const QPixmap& pm, int left, int top);

    void addRect(CQRect rect, float xRadius = 0, float yRadius = 0);
    void addPolygonF(const QPolygonF& pf);
    void addText(const QPointF &point, const QFont &f, const QString &text);
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

