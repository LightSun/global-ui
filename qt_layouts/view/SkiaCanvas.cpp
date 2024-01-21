#include <optional>
#include "SkiaCanvas.h"
#include "include/core/SkCanvas.h"

using namespace h7_qt;

namespace h7_qt {

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

struct _SkiaCanvas_ctx{
    std::vector<unsigned int> m_buffer;
    std::unique_ptr<SkCanvas> m_canvas;
    SkPaint m_paint;

    _SkiaCanvas_ctx(int w, int h){
        m_buffer.resize(w * h);
        auto info = SkImageInfo::Make(w, h, kRGBA_8888_SkColorType, kOpaque_SkAlphaType);
        m_canvas = SkCanvas::MakeRasterDirect(info, m_buffer.data(), info.minRowBytes());
    }
    void begin(){
        m_paint = SkPaint();
    }
    void end(){
        //m_canvas->clipRect()
        m_canvas->drawPaint(m_paint);
    }
};
}
SkiaCanvas::SkiaCanvas(int w, int h){
    m_ptr = new _SkiaCanvas_ctx(w, h);
}
SkiaCanvas::~SkiaCanvas(){
    if(m_ptr){
        delete m_ptr;
        m_ptr = nullptr;
    }
}
void SkiaCanvas::translate(int dx, int dy){

}
