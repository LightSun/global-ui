#include "ImageView.h"

using namespace h7_qt;

ImageView::ImageView()
{

}

void ImageView::onMeasure(int& parentW, int& parentH){
    parentW = m_image.width() + m_padding.left + m_padding.right;
    parentH = m_image.height() + m_padding.top + m_padding.bottom;
    //PRINTLN("ImageView::onMeasure >> w,h = %d, %d", parentW, parentH);
}

void ImageView::onDraw(Canvas* c){
    c->drawImage(&m_image, m_padding.left - 1, m_padding.top);
}
