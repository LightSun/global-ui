#pragma once

#include "view_common.h"

namespace h7_qt {

typedef struct _SkiaCanvas_ctx _SkiaCanvas_ctx;

class Paint;
using SPPaint = std::shared_ptr<Paint>;

class Paint{
public:
    enum Style{
        kFill_Style,          //!< set to fill geometry
        kStroke_Style,        //!< set to stroke geometry
        kStrokeAndFill_Style, //!< sets to stroke and fill geometry
    };
    enum Cap {
        kButt_Cap,                  //!< no stroke extension
        kRound_Cap,                 //!< adds circle
        kSquare_Cap,                //!< adds square
        kLast_Cap    = kSquare_Cap, //!< largest Cap value
        kDefault_Cap = kButt_Cap,   //!< equivalent to kButt_Cap
    };
    enum Join : uint8_t {
        kMiter_Join,                 //!< extends to miter limit
        kRound_Join,                 //!< adds circle
        kBevel_Join,                 //!< connects outside edges
        kLast_Join    = kBevel_Join, //!< equivalent to the largest value for Join
        kDefault_Join = kMiter_Join, //!< equivalent to kMiter_Join
    };
    void setColor(int rgba);
    void setARGB(uint8 a, uint8 r, uint8 g, uint8 b);
    void setStrokeWidth(float width);
    void setAntiAlias(bool aa);
    void setDither(bool dither);
    void setStyle(Style style);
    void setStroke(bool); //strokr or fill

    void setAlphaf(float a);
    void setAlpha(uint8 a);
    void setStrokeMiter(float miter);
    void setStrokeCap(Cap cap);
    void setStrokeJoin(Join join);
};

class SkiaCanvas
{
public:
    SkiaCanvas(int w, int h);
    ~SkiaCanvas();

    void translate(int dx, int dy);

private:
    _SkiaCanvas_ctx* m_ptr;
};


}

