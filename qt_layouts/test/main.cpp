
#include <QApplication>
#include <QWidget>
#include <QLabel>

#include "qt_layouts/BorderLayout.h"
#include "qt_layouts/FrameLayout.h"

static void test_border_layouts(QWidget* p);
static void test_frame_layouts(QWidget* p);
static void test_frame_layouts2(QWidget* p);

static int test1(int argc, char* argv[]);
extern int test_qt_view(int argc, char* argv[]);

int main(int argc, char* argv[]){
    setbuf(stdout, NULL);


    return test_qt_view(argc, argv);
    //return test1(argc, argv);
}

//--------------------------
int test1(int argc, char* argv[]){
    QApplication app(argc, argv);

    QWidget w;
    w.setFixedSize(500, 500);
    //test_border_layouts(&w);
    //test_frame_layouts(&w);
    test_frame_layouts2(&w);
    w.show();

    return app.exec();
}

static int _id = 0;
static QLabel* newLabe(QWidget* p){
    auto ids = QString("%1").arg(_id++);
    QLabel* l = new QLabel(p);
    l->setFixedSize(100, 50);
    l->setStyleSheet("background:#ff0000");
    l->setText("heaven7_" + ids);
    l->setContentsMargins(5,5,5,5);
    return l;
}
static QLayout* newLayout(QWidget* p){
    auto ids = QString("%1").arg(_id++);
    QLabel* l = new QLabel(p);
    l->setFixedSize(100, 50);
    l->setStyleSheet("background:#ff0000");
    l->setText("heaven7_" + ids);
    auto layout = new QVBoxLayout();
    //layout->setMargin(5);
    layout->setContentsMargins(5, 5, 5, 5);
    layout->setSpacing(0);
    layout->addWidget(l);
    return layout;
}
void test_border_layouts(QWidget* p){
    BorderLayout* bl = new BorderLayout();
    bl->addWidget(newLabe(p), BorderLayout::Position::East);
    bl->addWidget(newLabe(p), BorderLayout::Position::West);
    bl->addWidget(newLabe(p), BorderLayout::Position::North);
    bl->addWidget(newLabe(p), BorderLayout::Position::South);
    bl->addWidget(newLabe(p), BorderLayout::Position::Center);
    p->setLayout(bl);
}
void test_frame_layouts(QWidget* p){
    FrameLayout* bl = new FrameLayout();
    bl->addWidget(newLabe(p), Qt::AlignLeft | Qt::AlignTop);
    bl->addWidget(newLabe(p), Qt::AlignRight | Qt::AlignTop);
    bl->addWidget(newLabe(p), Qt::AlignLeft | Qt::AlignBottom);
    bl->addWidget(newLabe(p), Qt::AlignRight | Qt::AlignBottom);
    bl->addWidget(newLabe(p), Qt::AlignHCenter | Qt::AlignVCenter);
    p->setLayout(bl);
}
 void test_frame_layouts2(QWidget* p){
     FrameLayout* bl = new FrameLayout();
     bl->addItem(newLayout(p), Qt::AlignLeft | Qt::AlignTop);
     bl->addItem(newLayout(p), Qt::AlignRight | Qt::AlignTop);
     bl->addItem(newLayout(p), Qt::AlignLeft | Qt::AlignBottom);
     bl->addItem(newLayout(p), Qt::AlignRight | Qt::AlignBottom);
     bl->addItem(newLayout(p), Qt::AlignHCenter | Qt::AlignVCenter);
     p->setLayout(bl);
 }
