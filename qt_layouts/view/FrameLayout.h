#pragma once

#include "view/View.h"

namespace h7_qt {

class FrameLayout: public ViewGroup
{
public:
    FrameLayout();

    FUNC_NEW_SHARED(FrameLayout);

protected:
    virtual void onMeasure(int& w, int& h) override;
    virtual void onLayout(CRect) override;
};

}
