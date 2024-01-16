#include "TextView.h"
#include <QFontMetrics>

using namespace h7_qt;

namespace h7_qt {
struct _TextView_ctx{
    QString text;
    QColor textColor {Qt::black};

    void setText(CString t){
        text = QString::fromStdString(t);
    }
    void setTextColor(const QColor& c){
        textColor = c;
    }
};
}

TextView::TextView()
{
    m_ptr = new _TextView_ctx();
}
TextView::~TextView()
{
    if(m_ptr){
        delete m_ptr;
        m_ptr = nullptr;
    }
}

void TextView::setText(CString text){
    m_ptr->setText(text);
}
void TextView::setTextColor(int qt_global_val){
    QColor c((Qt::GlobalColor)qt_global_val);
    m_ptr->setTextColor(c);
}
void TextView::onDraw(Canvas* c){
    QFont font;
    applyToFont(font);
    c->setFont(font);
    c->setColor(m_ptr->textColor);
    c->applyStyle();
    c->drawText(m_ptr->text, contentRect(), m_layoutP->gravity);
}
void TextView::onMeasure(int& parentW, int& parentH){
    QFont font;
    applyToFont(font);
    QFontMetrics fm(font);
    parentW = fm.horizontalAdvance(m_ptr->text);
    parentH = fm.height();
}
void TextView::applyToFont(QFont& font){
    font.setPixelSize(m_textSize);
    if(hasFlag(kFlag_BOLD)){
        font.setBold(true);
    }
    if(hasFlag(kFlag_DELETE_LINE)){
        font.setStrikeOut(true);
    }
    if(hasFlag(kFlag_UNDER_LINE)){
        font.setUnderline(true);
    }
    if(hasFlag(kFlag_OVER_LINE)){
        font.setOverline(true);
    }
}
