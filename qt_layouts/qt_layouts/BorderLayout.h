
#ifndef BORDERLAYOUT_H
#define BORDERLAYOUT_H

#include <QLayout>
#include <QRect>
#include <QWidgetItem>

class BorderLayout : public QLayout
{
public:
  enum Position { West, North, South, East, Center };

  BorderLayout(QWidget *parent, int margin = 0, int spacing = -1);
  BorderLayout(int spacing = -1);
  ~BorderLayout();

  void addItem(QLayoutItem *item);
  void addWidget(QWidget *widget, Position position);
  Qt::Orientations expandingDirections() const;
  bool hasHeightForWidth() const;
  int count() const;
  QLayoutItem *itemAt(int index) const;
  QSize minimumSize() const;
  void setGeometry(const QRect &rect);
  QSize sizeHint() const;
  QLayoutItem *takeAt(int index);

  void add(QLayoutItem *item, Position position);

private:
  struct ItemWrapper
  {
    ItemWrapper(QLayoutItem *i, Position p) {
      item = i;
      position = p;
    }

    QLayoutItem *item;
    Position position;
  };

  enum SizeType { MinimumSize, SizeHint };
  QSize calculateSize(SizeType sizeType) const;

  QList<ItemWrapper *> list;
};

#endif
