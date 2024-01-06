
#include "view/ContainerWidget.h"
#include "view/LinearLayout.h"
#include "view/TextView.h"
#include "view/ImageView.h"
#include <QApplication>

using namespace h7_qt;

static void test_LinearLayout(ContainerWidget* v);

int test_qt_view(int argc, char* argv[]){
    QApplication app(argc, argv);

    ContainerWidget w;
    w.resize(600, 800);
    test_LinearLayout(&w);
    w.measure();
    w.layout();
    w.show();

    return app.exec();
}

static inline std::shared_ptr<TextView> createTextView(){
    auto v = TextView::New();
    v->setText("Hello Heaven7! Good luck!");
    v->setTextColor(Qt::red);
    v->setTextSize(18);
    v->setMargin({5,5,5,5});
    v->setPadding({10, 10, 10, 10});
    Image img;
    img.setQColor(Qt::blue);
    v->setBackground(std::move(img));
    return v;
}

static inline std::shared_ptr<ImageView> createImageView(){
    auto v = ImageView::New();
    {
    Image img;
    img.setQColor(Qt::yellow);
    img.fitWH(30, 30);
    v->setImage(img);
    }
    v->setPadding({10, 10, 10, 10});
    {
    Image img2;
    img2.setQColor(Qt::gray);
    v->setBackground(std::move(img2));
    }
    return v;
}

void test_LinearLayout(ContainerWidget* v){
    auto ll = LinearLayout::New();
    //ll->setSpace(10);
    ll->addView(createTextView());
    ll->addView(createTextView());
    ll->addView(createTextView());
    ll->addView(createImageView());
    v->setViewGroup(ll);
}
