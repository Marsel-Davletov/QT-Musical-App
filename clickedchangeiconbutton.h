#ifndef CLICKEDCHANGEICONBUTTON_H
#define CLICKEDCHANGEICONBUTTON_H
#include "changeiconbutton.h"
#include "includes.h"


class ClickedChangeIconButton : public ChangeIconButton
{
    Q_OBJECT
public:
   explicit ClickedChangeIconButton(QWidget *parent = nullptr, QString iconpath = "", QString hover_color = "white");
};

#endif // CLICKEDCHANGEICONBUTTON_H
