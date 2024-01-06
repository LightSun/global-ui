#pragma once

#include "view/View.h"

namespace h7_qt {

class LinearLayout: public ViewGroup
{
public:
    LinearLayout();

    FUNC_NEW_SHARED(LinearLayout);

    void setVertical(bool vertical){
        m_vertical = vertical;
    }
    void setSpace(int space){
        m_space = space;
    }

protected:
    virtual void onMeasure(int& w, int& h) override;
    virtual void onLayout(CRect) override;

private:
    bool m_vertical {true};
    int m_space {0};
};


}
