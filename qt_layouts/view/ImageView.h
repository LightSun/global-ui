#pragma once

#include "view/View.h"

namespace h7_qt {

class ImageView: public View
{
public:
    ImageView();

    FUNC_NEW_SHARED(ImageView);

    void setImage(CImage img){
        m_image = img;
        if(m_size.w > 0 && m_size.h > 0){
            auto& c = m_size;
            m_image.prepareDraw(c.w - m_padding.left - m_padding.right,
                                c.h - m_padding.top - m_padding.bottom);
        }
    }

    void setSize(CSize c) override{
        View::setSize(c);
        if(m_image.isValid()){
            m_image.prepareDraw(c.w - m_padding.left - m_padding.right,
                                c.h - m_padding.top - m_padding.bottom);
        }
    }

protected:
    virtual void onDraw(Canvas* c) override;
    virtual void onMeasure(int& parentW, int& parentH) override;

private:
    Image m_image;
};

}
