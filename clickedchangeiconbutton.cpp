#include "clickedchangeiconbutton.h"

ClickedChangeIconButton::ClickedChangeIconButton(QWidget *parent, QString iconpath, QString hover_color) : ChangeIconButton(parent, iconpath, hover_color)
{
    connect(this, SIGNAL(clicked()), SLOT(changeIcon()));
}
