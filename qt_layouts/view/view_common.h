#pragma once

#include "core/h7_common.h"

class QImage;
class QColor;
class QString;

namespace h7_qt {

typedef bool boolean ;

using String = std::string;
using CString = const std::string&;
using CQColor = const QColor&;

class Image;
class Canvas;

struct Rect;
struct Point;
struct Size;

using CRect = const Rect&;
using CSize = const Size&;
using CPoint = const Point&;
using CImage = const Image&;

struct Rect{
    int left;
    int top;
    int right;
    int bottom;

    Rect():left(0),top(0),right(0),bottom(0){}
    Rect(int left, int top, int right, int bottom)
        :left(left),top(top),right(right),bottom(bottom){}

    static Rect ofXYWH(int x, int y, int w, int h){
        return Rect(x, y, x + w, y + h);
    }
    int width()const{
        return right - left;
    }
    int height()const{
        return bottom - top;
    }
    bool isPointIn(int x, int y)const{
        return x >= left && x <= right && y >= top && y <= bottom;
    }
};

struct Point{
    int x;
    int y;
    Point():x(0), y(0){}
    Point(int x, int y):x(x), y(y){}
};

struct Size{
    int w;
    int h;
    Size():w(0), h(0){}
    Size(int w, int h):w(w), h(h){}

    void setWidth(int w){this->w = w;}
    void setHeight(int h){this->h = h;}
    int width()const{return w;}
    int height()const{return h;}
};

//
typedef struct _Image_ctx _Image_ctx;
class Image{
public:
    Image();
    ~Image();

    bool isValid()const;
    int width()const;
    int height()const;

    bool loadFromFile(CString file, const char *format = nullptr);
    bool loadFromBuffer(const char* data, int len, const char *format = nullptr);
    //w,h: 0 means no scale. w >0 && h = 0 means fit width, w >0 && h > 0 fit height.
    void fitWH(int w, int h);

    void setImage(QImage* img);
    void setColor(QColor* c);
    void setQColor(int enum_val);

    void prepareDraw(int w, int h);
    void draw(Canvas* c, int x, int y);
    Image& operator=(const Image& img);

private:
    _Image_ctx* m_ptr;
};

}
