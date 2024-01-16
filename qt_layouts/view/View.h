#pragma once

#include "view/Canvas.h"
#include "view/view_common.h"
#include "view/Gravity.h"

namespace h7_qt {

enum{
    kLAYOUT_MATCH_PARENT = -1,
    kLAYOUT_WRAP_CONTENT = -2,
};

struct LayoutParams{
    int gravity = kGravity_HCENTER | kGravity_VCENTER;
    int width {kLAYOUT_WRAP_CONTENT};
    int height {kLAYOUT_WRAP_CONTENT};
};

class Canvas;
class ViewGroup;
struct MotionEvent;

class View{
public:
    using SPLayoutP = std::shared_ptr<LayoutParams>;
    using OnClickListener = std::function<void(View* v)>;

    enum{
        kFlags_IGNORE_BACKGROUND = 1
    };
    View();
    virtual ~View();

    void setParentView(ViewGroup* p){
        m_parent = p;
    }
    ViewGroup* getParentView(){
        return m_parent;
    }
    void setLayoutParams(SPLayoutP p){
        m_layoutP = p;
    }
    SPLayoutP getLayoutParams(){
        return m_layoutP;
    }
    void setFlags(int flags){m_flags = flags;}
    void addFlags(int flags){m_flags |= flags;}
    int getFlags(){return m_flags;}
    bool hasFlag(int f){return (m_flags & f) == f;}
    void ignoreBackGround(){m_flags |= kFlags_IGNORE_BACKGROUND;}

    void setPadding(CRect p){ m_padding = p;}
    void setMargin(CRect p){ m_margin = p;}
    Rect& getPadding(){return m_padding;}
    Rect& getMargin(){return m_margin;}

    void setBackground(CImage c){m_bg = c;}
    void setId(int id){m_id = id;}
    void setSize(int w, int h){
        setSize(Size(w, h));
    }
    virtual void setSize(CSize c){
        //if(!m_size.equals(c)){
        if(m_size != c){
            if(!hasFlag(kFlags_IGNORE_BACKGROUND)){
                m_bg.prepareDraw(c.width(), c.height());
            }
            m_size = c;
            onSizeChanged(c);
        }
        //PRINTLN("setSize: w,h = %d, %d", c.width(), c.height());
    }
    Size& getSize(){return m_size;}

    ///include margin.
    Size getBoxSize(){
        return Size(m_size.width() + m_margin.left + m_margin.right,
                     m_size.height() + m_margin.top + m_margin.bottom
                     );
    }
    Point& getPosition(){return m_position;}
    Rect contentRect(){
        return Rect::ofXYWH(m_padding.left, m_padding.top,
                     m_size.width() - m_padding.left - m_padding.right,
                     m_size.height() - m_padding.top - m_padding.bottom);
    }

    void setPosition(CPoint p){
        m_position = p;
        PRINTLN("setPosition: p.x,y = %d, %d", p.x, p.y);
    }
    void setDrawingCacheEnabled(bool enable);
    void draw(Canvas* c){
        if(m_drawCached){
            drawInCached(c);
        }else{
            drawDirect(c);
        }
    }
    void measure(int parentW, int parentH){
        int targetW = parentW;
        int targetH = parentH;
        onMeasure(targetW, targetH);
        //add padding
        targetW += m_padding.widthSpace();
        targetH += m_padding.heightSpace();
        if(m_layoutP->width == kLAYOUT_MATCH_PARENT){
            if(parentW > targetW){
                targetW = parentW;
            }
        }
        if(m_layoutP->height == kLAYOUT_MATCH_PARENT){
            if(parentH > targetH){
                targetH = parentH;
            }
        }
        setSize(Size(targetW, targetH));
    }
    bool hitTest(CPoint p);
    //return true if handled
    virtual bool onTouchEvent(MotionEvent* ){return false;}
protected:
    virtual void onDraw(Canvas* c){}
    //without this view's padding
    virtual void onMeasure(int& parentW, int& parentH){}

    virtual void onSizeChanged(CSize s);

private:
    void drawDirect(Canvas* c);
    void drawInCached(Canvas* c);

protected:
    Rect m_padding;
    Rect m_margin;
    Size m_size;
    Point m_position;

    SPLayoutP m_layoutP;
    Image m_bg;
    Image* m_cacheImg {nullptr};
    ViewGroup* m_parent{nullptr};
    OnClickListener m_func_click;
    int m_flags {0};
    int m_id {0};
    bool m_drawCached {false};
};

//------------------------

class ViewGroup: public View{
public:
    using SPView = std::shared_ptr<View>;

    void addView(SPView v){
        v->setParentView(this);
        m_views.append(v);
    }
    void addView(int index, SPView v){
        v->setParentView(this);
        m_views.insert(index, v);
    }
    void removeView(SPView v){
        v->setParentView(nullptr);
        m_views.removeOne(v);
    }
    void removeViewAt(int i){
        m_views[i]->setParentView(nullptr);
        m_views.removeAt(i);
    }
    SPView getChildAt(int index){return m_views[index];}
    int getChildCount(){return m_views.size();}

    virtual void layout(CRect r){onLayout(r);}

    virtual bool dispatchTouchEvent(MotionEvent* );

protected:
    virtual void onLayout(CRect){} //should setPosition.

    virtual void onMeasure(int& w, int& h) override{
//        for(SPView v: m_views){
//            v->onMeasure(w, h);
//        }
    }

    virtual void onDraw(Canvas* c)override{
        auto it = m_views.begin();
        for(; it != m_views.end(); ++it){
            (*it)->draw(c);
        }
    };

protected:
    QList<SPView> m_views;
    View* m_touched_view {nullptr};
};

}
