#pragma once

#include "View.h"

namespace h7_qt {

typedef struct _TextView_ctx _TextView_ctx;

class TextView: public View
{
public:
    enum {
        kFlag_BOLD        = 0x0001,
        kFlag_DELETE_LINE = 0x0002,
        kFlag_UNDER_LINE  = 0x0004,
        kFlag_OVER_LINE   = 0x0008,
    };
    TextView();
    ~TextView();

    FUNC_NEW_SHARED(TextView);

    void setText(CString text);
    void setTextColor(int qt_global_val);
    void setTextSize(float size){m_textSize = size;}
    void setBold(bool bold){
        if(bold) addTextFlags(kFlag_BOLD);
        else removeTextFlags(kFlag_BOLD);
    }
    void setUnderline(bool underline){
        if(underline) addTextFlags(kFlag_UNDER_LINE);
        else removeTextFlags(kFlag_UNDER_LINE);
    }
    //
    int getTextFlags(){return m_textFlags;}
    void setTextFlags(int flags){m_textFlags = flags;}
    void addTextFlags(int flags){m_textFlags |= flags;}
    void removeTextFlags(int flags){m_textFlags &= ~flags;}
    bool hasTextFlag(int flags){return (m_textFlags & flags) == flags;}

protected:
    virtual void onDraw(Canvas* c) override;
    virtual void onMeasure(int& parentW, int& parentH) override;

private:
    void applyToFont(QFont& font);

private:
    _TextView_ctx* m_ptr;
    float m_textSize {14};
    int m_textFlags{0};
};

}
