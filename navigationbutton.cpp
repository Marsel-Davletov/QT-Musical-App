#include "navigationbutton.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>

NavigationButton::NavigationButton(QWidget *parent, QString buttonname, QString iconpath) : QPushButton(parent)
{
    this->setStyleSheet("\
            QPushButton {\
                font-size: 15px;\
                color: white;\
                background-color: transparent;\
                margin: 0px;\
                min-height: 50px;\
                border: 0px;\
            }\
            QPushButton:hover {\
                background: qlineargradient(x1:0, y1:0, x2:1, y1:1,\
                    stop:0 rgb(75,75,75), stop:1 rgb(53,53,53));\
            }\
            QPushButton:pressed {\
                background-color: rgb(98,97,97)\
            }");
    this->setLayout(new QHBoxLayout);
    QLabel *iconlabel = new QLabel;
    iconlabel->setStyleSheet("background-color: transparent; max-width: 30px;");
    QPixmap icon(iconpath);
    icon = icon.scaled(20, 20, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
//    icon = icon.transformed(QTransform &QTransform::scale(10, 10), Qt::SmoothTransformation);
    iconlabel->setPixmap(icon);
    this->layout()->addWidget(iconlabel);

    QLabel *textlabel = new QLabel(buttonname);
    textlabel->setStyleSheet("color: white; font-size: 15px; text-align: center; background-color: transparent;");
    this->layout()->addWidget(textlabel);
}
