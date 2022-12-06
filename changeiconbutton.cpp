#include "changeiconbutton.h"

ChangeIconButton::ChangeIconButton(QWidget *parent, QString iconpath, QString hover_color) : QPushButton(parent)
{
    changeIconPath = iconpath;
    defaultIconPath = iconpath;
    QString style = QString("\
        QPushButton {\
            background-color: transparent;\
            border-radius: 15px;\
            max-width: 32px;\
            max-height: 32px;\
            text-align: center;\
        }\
        QPushButton:hover {\
            border: 1px solid rgb(220, 220, 220);\
            background-color: rgb(45, 45, 45);\
        }\
        QPushButton::menu-indicator {\
            width: 0px;\
        }").arg(hover_color);

    this->setMouseTracking(true); // чтобы работали собитя с наведением мышки
    this->setStyleSheet(style);
    this->setDefaultIcon(defaultIconPath);
    this->setIconSize(QSize(25, 25));
}


ChangeIconButton::~ChangeIconButton()
{

}

void ChangeIconButton::changeIcon(QString iconpath) {
    this->setIcon(QIcon(iconpath));
}

void ChangeIconButton::setChangeIcon(QString iconpath) {
    changeIconPath = iconpath;
}

void ChangeIconButton::setDefaultIcon(QString iconpath) {
    defaultIconPath = iconpath;
    changeIcon(defaultIconPath);
}



