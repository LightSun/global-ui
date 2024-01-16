#include "view_common.h"
#include "Canvas.h"

#include <QImage>
#include <QPixmap>

using namespace h7_qt;

#define _QT_DEF_IMG_FMT QImage::Format_RGBA8888

namespace h7_qt {
struct _Image_ctx{
    QImage image;
    QPixmap pixmap;
    std::function<void(Canvas* c,int w, int h)> builder;
    bool dirty {true};

    bool loadFromFile(CString file, const char* fmt){
        if(!image.load(QString::fromStdString(file), fmt)){
            return false;
        }
        dirty = true;
        return true;
    }
    bool loadFromBuffer(const char* data, int len, const char* fmt){
        if(!image.loadFromData((uchar*)data, len, fmt)){
            return false;
        }
        dirty = true;
        return true;
    }
    void fitWH(int w, int h){
        if(builder){
            image = QImage(w, h, _QT_DEF_IMG_FMT);
            image.fill(0);
            Canvas c(&image);
            c.setViewport(QRect(0, 0, w, h));
            builder(&c, w ,h);
            c.end();
            return;
        }
        if(image.isNull()){
            return;
        }
        if(w > 0){
            if(h > 0){
                image = image.scaled(w, h, Qt::IgnoreAspectRatio);
            }else{
                image = image.scaledToWidth(w);
            }
        }else{
            if(h > 0){
                image = image.scaledToHeight(h);
            }
        }
        dirty = true;
    }

   void prepareDraw(int w, int h){
        if(w <= 0 || h <= 0){
            pixmap = QPixmap();
            return;
        }
        if(builder){
            image = QImage(w, h, _QT_DEF_IMG_FMT);
            image.fill(0);
            {
                Canvas c(&image);
                c.setViewport(QRect(0, 0, w, h));
                builder(&c, w, h);
                c.end();
            }
            pixmap = QPixmap::fromImage(image);
            return;
        }
        if(image.isNull()){
            return;
        }
        if(dirty || pixmap.width() != w || pixmap.height() != h){
            auto im = image.scaled(w, h, Qt::IgnoreAspectRatio);
            //PRINTLN("prepareDraw >> req: w,h = %d, %d, act: w,h = %d, %d",
            //        w, h, im.width(), im.height());
            pixmap = QPixmap::fromImage(im);
            dirty = false;
        }
    }
   _Image_ctx& operator=(const _Image_ctx& c){
       image = QImage(c.image);
       pixmap = QPixmap(c.pixmap);
       dirty = c.dirty;
       builder = c.builder;
       return *this;
   }
   void setImage(QImage* i){
       image = *i;
       dirty = true;
   }
   void setColor(CQColor c){
       if(image.isNull()){
          image = QImage(10, 10, _QT_DEF_IMG_FMT);
       }
       image.fill(c);
       dirty = true;
   }
   int width(){
       if(!pixmap.isNull()){
           return pixmap.width();
       }
       if(!image.isNull()){
           return image.width();
       }
       return 0;
   }
   int height(){
       if(!pixmap.isNull()){
           return pixmap.height();
       }
       if(!image.isNull()){
           return image.height();
       }
       return 0;
   }
};
}

Image::Image(){
    m_ptr = new _Image_ctx();
}
Image::~Image(){
    if(m_ptr){
        delete m_ptr;
        m_ptr = nullptr;
    }
}
bool Image::isValid()const{
    return !m_ptr->image.isNull();
}
int Image::width()const{
    return m_ptr->width();
}
int Image::height()const{
    return m_ptr->height();
}
void Image::fitWH(int w, int h){
    m_ptr->fitWH(w, h);
}

void Image::setBuilder(std::function<void(Canvas* c,int w, int h)> func){
    m_ptr->builder = func;
}
bool Image::loadFromFile(CString file, const char* fmt){
    return m_ptr->loadFromFile(file, fmt);
}
bool Image::loadFromBuffer(const char* data, int len, const char* fmt){
    return m_ptr->loadFromBuffer(data, len, fmt);
}
void Image::setImage(QImage* img){
    m_ptr->setImage(img);
}
void Image::setColor(QColor* c){
    m_ptr->setColor(*c);
}
void Image::setQColor(int enum_val){
    QColor c((Qt::GlobalColor)enum_val);
    m_ptr->setColor(c);
}
void Image::prepareDraw(int w, int h){
    m_ptr->prepareDraw(w, h);
}
void Image::draw(Canvas* c, int x, int y){
    if(!m_ptr->pixmap.isNull()){
        //m_ptr->pixmap.setDevicePixelRatio(0.8);
        c->drawPixmap(m_ptr->pixmap, x, y);
    }
}
Image& Image::operator=(const Image& img){
    *m_ptr = *img.m_ptr;
    return *this;
}
