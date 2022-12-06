#ifndef HOVERCHANGEICONBUTTON_H
#define HOVERCHANGEICONBUTTON_H
#include "changeiconbutton.h"
#include "includes.h"


class HoverChangeIconButton : public ChangeIconButton
{
    Q_OBJECT
public:
    explicit HoverChangeIconButton(QWidget *parent = nullptr, QString iconpath = "", QString hover_color = "white");

protected:
   void hoverEnter(QHoverEvent *event);
   void hoverLeave(QHoverEvent *event);
   bool event(QEvent *event);
};

#endif // HOVERCHANGEICONBUTTON_H
