#pragma once

#include <QLayout>
#include <QRect>
#include <QWidgetItem>
#include <QList>
#include <QLayoutItem>
#include <QLayout>

template <typename T,typename P>
class AbstractLayout : public QLayout
{
public:
    enum SizeType { MinSize, MaxSize, SizeHint };

    AbstractLayout(QWidget* p = nullptr):QLayout(p){}

    ~AbstractLayout(){
        foreach (T* t, mItems) {
            destroyItem(t);
        }
        mItems.clear();
    }
    void addItem(QLayoutItem *item, const P& p){
        mItems.push_back(newItemWrapper(item, p));
    }
    void addWidget(QWidget *widget, const P& p){
        addItem(new QWidgetItem(widget), p);
    }
    void addItem(QLayoutItem *item){
        mItems.push_back(newItemWrapper(item, P()));
    }
    void addWidget(QWidget *widget){
        addItem(new QWidgetItem(widget), P());
    }
    //
    Qt::Orientations expandingDirections() const{
        return Qt::Orientation::Vertical | Qt::Orientation::Horizontal;
    }
    bool hasHeightForWidth() const{
        return false;
    }
    int count() const{
        return mItems.size();
    }
    QLayoutItem *itemAt(int index) const{
        auto wrapper = mItems.value(index);
        return wrapper ? getItem(wrapper) : nullptr;
    }
    QLayoutItem *takeAt(int index){
        if(index >= mItems.size()){
            return nullptr;
        }
        auto wrap = mItems.takeAt(index);
        auto ret = getItem(wrap);
        delete wrap;
        return ret;
    }
    QLayoutItem* replaceAt(int index, QLayoutItem *newitem){
        if(index >= mItems.size()){
            return nullptr;
        }
        QLayoutItem* old = mItems.value(index);
        mItems.replace(index, newitem);
        return old;
    }

    QSize minimumSize() const{
        return calculateSize(MinSize);
    }
    QSize sizeHint() const{
        return calculateSize(SizeHint);
    }
    QSize maximumSize() const{
        return calculateSize(MaxSize);
    }
    void setGeometry(const QRect &){};
    QSize calculateSize(SizeType)const{};

public:
    QList<T*> mItems;

protected:
    T* newItemWrapper(QLayoutItem *item, const P& p){
        return new T(item, p);
    }
    QLayoutItem* getItem(T *t)const{
        return t->item;
    }
    void destroyItem(T *t)const{
        auto ret = getItem(t);
        delete t;
        delete ret;
    }
};
