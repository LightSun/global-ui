#pragma once

#include "view/view_common.h"

namespace h7_qt {

enum {
    kGravity_LEFT    = 0x0001,
    kGravity_RIGHT   = 0x0002,
    kGravity_HCENTER = 0x0004,

    kGravity_TOP     = 0x0020,
    kGravity_BOTTOM  = 0x0040,
    kGravity_VCENTER = 0x0080,

    kGravity_HMASK   = 0x000f,
    kGravity_VMASK   = 0x00f0,
};

class Gravity{
public:

    static Scala computeXOffset(Scala total, Scala sub, int gravity){
        //PRINTLN("computeXOffset: total = %lu, sub = %lu", total, sub);
        switch (gravity & kGravity_HMASK) {
        case kGravity_LEFT:
            return 0;

        case kGravity_RIGHT:
            return total - sub;

        case kGravity_HCENTER:
            return (total - sub) / 2;

        default:
            break;
        }
        return 0;
    }
    //sub: often include margin
    static Scala computeXOffsetWithMargin(Scala total, Scala sub, CRect margin,int gravity){
        return computeXOffset(total, sub, gravity) + margin.left;
    }
    static Scala computeYOffsetWithMargin(Scala total, Scala sub, CRect margin,int gravity){
        return computeYOffset(total, sub, gravity) + margin.top;
    }
    static Scala computeYOffset(Scala total, Scala sub, int gravity){
        switch (gravity & kGravity_VMASK) {
        case kGravity_TOP:
            return 0;

        case kGravity_BOTTOM:
            return total - sub;

        case kGravity_VCENTER:
            return (total - sub) / 2;

        default:
            break;
        }
        return 0;
    }

    static void center(SkRect& in_out, Scala w, Scala h){
        Scala xOffset = (in_out.width() - w)/2;
        Scala yOffset = (in_out.height() - h)/2;
        in_out.inset(xOffset, yOffset);
    }
};
}
