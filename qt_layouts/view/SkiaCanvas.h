#pragma once

#include "view_common.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkFont.h"
#include "include/core/SkTextBlob.h"
#include "view/Gravity.h"

namespace h7_qt {

using CSkImagePtr = const SkImage*;

class SkiaCanvas
{
public:
    SkiaCanvas(int w, int h);
    ~SkiaCanvas();

    SkCanvas& getCanvas()const{return *m_canvas;}
    SkImageInfo getImageInfo()const{return m_canvas->imageInfo();}

    void translate(Scala dx, Scala dy){m_canvas->translate(dx, dy);}
    void rotate(Scala degrees){m_canvas->rotate(degrees);}
    void scale(Scala sx, Scala sy){m_canvas->scale(sx, sy);}

    void save(){m_canvas->save();}
    void restore(){m_canvas->restore();}

    void resetFont(){m_font = SkFont();}
    void resetPaint(){m_paint.reset();}
    void setFont(const SkFont& f){m_font = f;}
    void setPaint(const SkPaint& p){m_paint = p;}
    SkFont& getFont(){return m_font;}
    SkPaint& getPaint(){return m_paint;}

    //with current paint
    void drawLine(int x1, int y1 ,int x2, int y2);
    void drawRect(const SkRect& r);
    void drawIRect(const SkIRect& r);
    void drawOval(const SkRect& r);
    void drawCircle(SkScalar cx, SkScalar cy, SkScalar radius);
    void drawArc(const SkRect& oval, SkScalar startAngle, SkScalar sweepAngle, bool useCenter);
    void drawRoundRect(const SkRect& rect, SkScalar rx, SkScalar ry);
    void drawPath(const SkPath& path);
    //with current paint and font
    void drawText(const SkString& text, const SkRect& rect);
    void drawText(const SkString& text, Scala left, Scala top);
    //SkPaint containing SkBlendMode, SkColorFilter, SkImageFilter,
    //      and so on; or nullptr
    void drawImage(sk_sp<SkImage> img, Scala left, Scala top, SkPaint* p = nullptr);
    void drawImage(CSkImagePtr img, Scala left, Scala top, SkPaint* p = nullptr);

    void drawImageRect(CSkImagePtr, const SkRect& src, const SkRect& dst,
                       const SkPaint* paint, bool strict);
    void drawImageRect(sk_sp<SkImage> img, const SkRect& src, const SkRect& dst,
                       const SkPaint* paint, bool strict);

    //png
    void saveImage(const SkString& path);
    sk_sp<SkImage> toImage();


private:
    std::vector<unsigned int> m_buffer;
    sk_sp<SkSurface> m_surface;
    SkCanvas* m_canvas;
    SkFont m_font;
    SkPaint m_paint;
};


}

