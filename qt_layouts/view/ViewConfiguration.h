#pragma once

namespace h7_qt {

//see ViewConfiguration.java
    class ViewConfiguration{
    public:
        enum{
            SCROLL_BAR_SIZE = 4,
            SCROLL_BAR_FADE_DURATION = 250,
            SCROLL_BAR_DEFAULT_DELAY = 300,
            FADING_EDGE_LENGTH = 12,
            PRESSED_STATE_DURATION = 64,
            DEFAULT_LONG_PRESS_TIMEOUT = 500,
            DEFAULT_MULTI_PRESS_TIMEOUT = 300,
            KEY_REPEAT_DELAY = 50,
            GLOBAL_ACTIONS_KEY_TIMEOUT = 500,

            TAP_TIMEOUT = 100,
            JUMP_TAP_TIMEOUT = 500,
            DOUBLE_TAP_TIMEOUT = 300,
            DOUBLE_TAP_MIN_TIME = 40,
            ZOOM_CONTROLS_TIMEOUT = 3000,
            EDGE_SLOP = 12,
            TOUCH_SLOP = 8,

            MIN_SCROLLBAR_TOUCH_TARGET = 48,
            DOUBLE_TAP_TOUCH_SLOP = TOUCH_SLOP, //in dip?
            PAGING_TOUCH_SLOP = TOUCH_SLOP * 2,
            DOUBLE_TAP_SLOP = 100,
            WINDOW_TOUCH_SLOP = 16,
            MINIMUM_FLING_VELOCITY = 50,
            MAXIMUM_FLING_VELOCITY = 8000,
            SCROLL_FRICTION = 15000, //0.015f. 15000/1000000
            OVERSCROLL_DISTANCE = 0,
            OVERFLING_DISTANCE = 6,
            HORIZONTAL_SCROLL_FACTOR = 64,
            VERTICAL_SCROLL_FACTOR = 64,
        };
        //latter make it can be config.
        static int getLongPressTimeout(){
            return DEFAULT_LONG_PRESS_TIMEOUT;
        }
        static int getTapTimeout() {
            return TAP_TIMEOUT;
        }
        static int getDoubleTapTimeout() {
            return DOUBLE_TAP_TIMEOUT;
        }
        static int getScaledTouchSlop() {
            return TOUCH_SLOP;
        }
        static int getScaledDoubleTapSlop() {
            return DOUBLE_TAP_SLOP;
        }
        static int getScaledMinimumFlingVelocity() {
            return MINIMUM_FLING_VELOCITY;
        }
        static int getScaledMaximumFlingVelocity() {
            return MAXIMUM_FLING_VELOCITY;
        }
    };

}
