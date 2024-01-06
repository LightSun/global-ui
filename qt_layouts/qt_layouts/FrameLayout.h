#pragma once

#include "AbstractLayout.h"

struct FrameLayout_ItemWrapper
{
  FrameLayout_ItemWrapper(QLayoutItem *i, int g) {
     this->item = i;
     this->gravity = g;
  }
  ~FrameLayout_ItemWrapper(){
  }
  QLayoutItem *item;
  int gravity;

  void destroyAll(){
      if(item){
          delete item;
          item = nullptr;
      }
      delete this;
  }

  bool hasFlags(int flags){
      return (gravity & flags) != 0;
  }
};

class FrameLayout : public AbstractLayout<FrameLayout_ItemWrapper, int>
{
    Q_OBJECT
public:
    using ItemWrapper = FrameLayout_ItemWrapper;
    FrameLayout(QWidget* parent = nullptr);
    ~FrameLayout();

    void setGeometry(const QRect &rect);
    QSize calculateSize(SizeType sizeType) const;

private:
};

