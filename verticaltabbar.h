#ifndef VERTICALTABBAR_H
#define VERTICALTABBAR_H

#include "includes.h"

class VerticalTabBar : public QTabBar
{
    Q_OBJECT
public:
    VerticalTabBar(QTabBar *parent = nullptr);
    QSize tabSizeHint(int index) const{
        QSize s = QTabBar::tabSizeHint(index);
        s.transpose();
        return s;
    }

protected:
    void paintEvent(QPaintEvent *event);
};

#endif // VERTICALTABBAR_H
