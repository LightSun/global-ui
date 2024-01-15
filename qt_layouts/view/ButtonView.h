#pragma once

#include "view/TextView.h"


namespace h7_qt {

class ButtonView: public TextView
{
public:
    ButtonView();

    FUNC_NEW_SHARED(ButtonView);

protected:
    //virtual void onDraw(Canvas* c) override;
};

}

