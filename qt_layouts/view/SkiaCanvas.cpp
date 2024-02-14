#include <optional>
#include "SkiaCanvas.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkSurface.h"
#include "include/core/SkData.h"
#include "include/core/SkStream.h"

using namespace h7_qt;

struct AutoSaveRestore{
    SkCanvas* canvas;
    int sc;
    AutoSaveRestore(SkCanvas* canvas, std::optional<Rect> rect): canvas(canvas){
        sc = canvas->save();
        canvas->clipRect(SkRect::MakeLTRB(rect->left, rect->top, rect->right, rect->bottom), false);
    }
    ~AutoSaveRestore(){
        canvas->restoreToCount(sc);
    }
};

SkiaCanvas::SkiaCanvas(int w, int h){
    m_buffer.resize(w * h);
    SkImageInfo info = SkImageInfo::Make(w, h, kRGBA_8888_SkColorType, kOpaque_SkAlphaType);
    m_surface = SkSurface::MakeRasterDirect(info, m_buffer.data(), info.minRowBytes());
    m_canvas = m_surface->getCanvas();
}
SkiaCanvas::~SkiaCanvas(){
}

void SkiaCanvas::drawLine(int x1, int y1 ,int x2, int y2){
    m_canvas->drawLine(x1, y1, x2, y2, m_paint);
}
void SkiaCanvas::drawRect(const SkRect& r){
    m_canvas->drawRect(r, m_paint);
}
void SkiaCanvas::drawIRect(const SkIRect& r){
    m_canvas->drawIRect(r, m_paint);
}
void SkiaCanvas::drawOval(const SkRect& r){
    m_canvas->drawOval(r, m_paint);
}
void SkiaCanvas::drawCircle(SkScalar cx, SkScalar cy, SkScalar radius){
    m_canvas->drawCircle(cx, cy, radius, m_paint);
}
void SkiaCanvas::drawArc(const SkRect& oval, SkScalar startAngle, SkScalar sweepAngle, bool useCenter){
    m_canvas->drawArc(oval, startAngle, sweepAngle, useCenter, m_paint);
}
void SkiaCanvas::drawRoundRect(const SkRect& rect, SkScalar rx, SkScalar ry){
    m_canvas->drawRoundRect(rect, rx, ry, m_paint);
}
void SkiaCanvas::drawPath(const SkPath& path){
    m_canvas->drawPath(path, m_paint);
}

void SkiaCanvas::drawText(const SkString& text, CRect rect){
    auto& r = (Rect&)rect;
    SkRect skr;
    auto w = m_font.measureText(text.c_str(), text.size(), SkTextEncoding::kUTF8, &skr);
    Gravity::center(r, w, skr.height());
    m_canvas->drawString(text, r.left, r.top, m_font, m_paint);
}
void SkiaCanvas::drawText(const SkString& text, Scala x, Scala y){
    m_canvas->drawString(text, x, y, m_font, m_paint);
}
void SkiaCanvas::drawImage(sk_sp<SkImage> img, Scala x, Scala y, SkPaint* p){
    m_canvas->drawImage(img, x, y, p);
}
void SkiaCanvas::saveImage(const SkString& path){
    auto img = m_surface->makeImageSnapshot();
    sk_sp<SkData> png(img->encodeToData());
    SkFILEWStream fos(path.c_str());
    fos.write(png->data(), png->size());
}
sk_sp<SkImage> SkiaCanvas::toImage(){
    //SkBitmap::Make
    return m_surface->makeImageSnapshot();
}
