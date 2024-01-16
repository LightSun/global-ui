
#include "view/ContainerWidget.h"
#include "view/LinearLayout.h"
#include "view/FrameLayout.h"

#include "view/TextView.h"
#include "view/ImageView.h"
#include "view/ButtonView.h"
#include <QApplication>

using namespace h7_qt;

static void test_LinearLayout(ContainerWidget* v);
static void test_FrameLayout(ContainerWidget* v);

int test_qt_view(int argc, char* argv[]){
    QApplication app(argc, argv);

    ContainerWidget w;
    w.resize(600, 800);
    //test_LinearLayout(&w);
    test_FrameLayout(&w);

    //
    w.measure();
    w.layout();
    w.show();

    return app.exec();
}

static inline std::shared_ptr<TextView> createTextView(int gravity){
    //auto v = TextView::New();
    auto v = ButtonView::New();
    v->setText("Hello Heaven7! Good luck!");
    v->setTextColor(Qt::red);
    v->setTextSize(18);
    v->setMargin({5,5,5,5});
    v->setPadding({10, 10, 10, 10});
    v->getLayoutParams()->gravity = gravity;
    // Image img;
    // img.setQColor(Qt::blue);
    // v->setBackground(std::move(img));
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

void test_FrameLayout(ContainerWidget* v){
    auto ll = FrameLayout::New();
    ll->addView(createTextView(kGravity_HCENTER|kGravity_TOP));
    ll->addView(createTextView(kGravity_LEFT|kGravity_BOTTOM));
    ll->addView(createTextView(kGravity_RIGHT));
    v->setViewGroup(ll);
}

void test_LinearLayout(ContainerWidget* v){
    auto ll = LinearLayout::New();
    //ll->setSpace(10);
    ll->addView(createTextView(kGravity_HCENTER));
    ll->addView(createTextView(kGravity_LEFT));
    ll->addView(createTextView(kGravity_RIGHT));
    ll->addView(createImageView());
    v->setViewGroup(ll);
}
