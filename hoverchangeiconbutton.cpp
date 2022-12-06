#include "hoverchangeiconbutton.h"

HoverChangeIconButton::HoverChangeIconButton(QWidget *parent, QString iconpath, QString hover_color) : ChangeIconButton(parent, iconpath, hover_color)
{

}


void HoverChangeIconButton::hoverEnter(QHoverEvent*) {
    changeIcon(changeIconPath);
}

void HoverChangeIconButton::hoverLeave(QHoverEvent*) {
    changeIcon(defaultIconPath);
}

bool HoverChangeIconButton::event(QEvent *event) {
    switch (event->type())
    {
    case QEvent::HoverEnter:
        hoverEnter(static_cast<QHoverEvent*>(event));
        return true;
        break;
    case QEvent::HoverLeave:
        hoverLeave(static_cast<QHoverEvent*>(event));
        return true;
        break;
    default:
        break;
    }
    return QWidget::event(event);
}
