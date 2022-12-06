#ifndef ICONLABEL_H
#define ICONLABEL_H

#include "includes.h"

class IconLabel : public QLabel
{
    Q_OBJECT

private:
    QPixmap cover;

public:
    QBitmap *map;
    QSize size;
    IconLabel(QWidget *parent = nullptr, QString url = "", QSize size = QSize(30, 30));

public slots:
    void setCover(QByteArray data);
    void loadPicture(QString url);
    void loadPictureData(QNetworkReply *reply);
};

#endif // ICONLABEL_H
