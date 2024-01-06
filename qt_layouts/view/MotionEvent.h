#pragma once

namespace h7_qt {

class MotionEvent
{
public:
    using LL = long long;
    MotionEvent();

private:
    LL time;
    int x, y;
    int action;
};

}

