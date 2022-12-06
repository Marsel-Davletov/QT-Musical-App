#ifndef NAVIGATIONBUTTON_H
#define NAVIGATIONBUTTON_H

#include "includes.h"

class NavigationButton : public QPushButton
{
    Q_OBJECT
public:
    explicit NavigationButton(QWidget *parent = nullptr, QString buttonname = "", QString iconpath = "");
};

#endif // NAVIGATIONBUTTON_H
