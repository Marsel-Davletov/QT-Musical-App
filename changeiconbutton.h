#ifndef CHANGEICONBUTTON_H
#define CHANGEICONBUTTON_H

#include "includes.h"


class ChangeIconButton : public QPushButton
{
    Q_OBJECT

public:
    QString defaultIconPath;
    QString changeIconPath;

   explicit ChangeIconButton(QWidget *parent = nullptr, QString iconpath = "", QString hover_color = "white");
   ~ChangeIconButton();

public slots:
   void setDefaultIcon(QString iconpath);
   void setChangeIcon(QString iconpath);
   void changeIcon(QString iconpath);
};

#endif // CHANGEICONBUTTON_H
