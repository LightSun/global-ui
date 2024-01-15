#pragma once

#include <QWidget>
#include <memory>

namespace h7_qt {
    class ViewGroup;
}

class ContainerWidget: public QWidget
{
public:
    using SPViewGroup = std::shared_ptr<h7_qt::ViewGroup>;
    ContainerWidget(QWidget* parent = nullptr);

    void setViewGroup(SPViewGroup vg){m_vg = vg;}

    void measure();

    void layout();

private:
    void installEvent();
    void uninstallEvent();

protected:
    //bool event(QEvent *event) override;
    //void resizeEvent(QResizeEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

//    void mouseDoubleClickEvent(QMouseEvent *event) override;
//    void wheelEvent(QWheelEvent *event) override;

//    void keyPressEvent(QKeyEvent *event) override;
//    void keyReleaseEvent(QKeyEvent *event) override;
//    void focusInEvent(QFocusEvent *event) override;
//    void focusOutEvent(QFocusEvent *event) override;
//    void enterEvent(QEvent *event) override;
//    void leaveEvent(QEvent *event) override;
//    void moveEvent(QMoveEvent *event) override;

//    void resizeEvent(QResizeEvent *event) override;
//    void closeEvent(QCloseEvent *event) override;

private:
    SPViewGroup m_vg;
};

