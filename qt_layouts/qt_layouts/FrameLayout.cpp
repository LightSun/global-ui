#include "FrameLayout.h"
#include <QDebug>
#include <QWidget>

FrameLayout::FrameLayout(QWidget* p): AbstractLayout(p)
{
    setSpacing(0);
    setMargin(0);
   // setContentsMargins(0, 0, 0, 0);
}
FrameLayout::~FrameLayout(){

}

void FrameLayout::setGeometry(const QRect &rect){
    qDebug() << "setGeometry >> ";
    auto margins = contentsMargins();
    //int space = spacing();
    int fx = rect.x() + margins.left();
    int fy = rect.y() + margins.top();
    int fw = rect.width() - margins.left() - margins.right();
    int fh = rect.height() - margins.top() - margins.bottom();
    for (int i = 0; i < mItems.size(); ++i) {
        ItemWrapper *wrapper = mItems.at(i);
        QLayoutItem *item = wrapper->item;
        int w, h;
        int left_offset = 0,top_offset = 0;
        QMargins tcm;
        if(item->widget()){
            tcm = item->widget()->contentsMargins();
            // item->widget()->height()= item->sizeHint().height()
            //int th = item->widget()->height();
            //qDebug() << tw << ", " << th;

            //why horizontal need + left. vertical not ?
            left_offset = tcm.left();
        }else if(item->layout()){
            item->layout()->setGeometry(QRect(fx, fy, fw, fh));
        }else{
            continue;
        }
        int h_margin = tcm.left() + tcm.right();
        int v_margin = tcm.top() + tcm.bottom();
        w = item->sizeHint().width() + h_margin;
        h = item->sizeHint().height() + v_margin;
        //
        if(wrapper->hasFlags(Qt::AlignLeft)){
            //ignore
        }else if(wrapper->hasFlags(Qt::AlignRight)){
            left_offset += fw - w;
        }else if(wrapper->hasFlags(Qt::AlignHCenter)){
            left_offset += (fw - w)/2;
        }
        if(wrapper->hasFlags(Qt::AlignTop)){
           //ignore
        }else if(wrapper->hasFlags(Qt::AlignBottom)){
            top_offset += fh - h;
        }else if(wrapper->hasFlags(Qt::AlignVCenter)){
            top_offset += (fh - h)/2;
        }
        //qDebug() << "w = " << w << ",h = " << h;
        item->setGeometry(QRect(fx + left_offset,
                                fy + top_offset,
                          w,
                          h));
    }
}
QSize FrameLayout::calculateSize(SizeType sizeType) const{
    qDebug() << "calculateSize >> ";
    QSize totalSize;
    for (int i = 0; i < mItems.size(); ++i) {
        ItemWrapper *wrapper = mItems.at(i);
        //wrapper->item->widget()->contentsMargins();
        QSize itemSize;
        switch (sizeType) {
        case MinSize: {
            itemSize = wrapper->item->minimumSize();
        }break;
        case MaxSize: {
            itemSize = wrapper->item->maximumSize();
        }break;
        case SizeHint: {
            itemSize = wrapper->item->sizeHint();
        }break;
        }
        if(itemSize.width() > totalSize.width()){
            totalSize.setWidth(itemSize.width());
        }
        if(itemSize.height() > totalSize.height()){
            totalSize.setHeight(itemSize.height());
        }
    }
    return totalSize;
}
