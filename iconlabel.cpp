#include "iconlabel.h"
#include "includes.h"

IconLabel::IconLabel(QWidget *parent, QString url, QSize s) : QLabel(parent)
{
    size = s;
    this->setStyleSheet("background-color: transparent;");
    this->setMaximumWidth(100);
    map = new QBitmap(size); // растровое 1-битное пресдтавление, имеет два цвета color0 и color1
    map->fill(Qt::color0);
    QPainter painter(map); // пэинтер для рисования по map
    painter.setBrush(Qt::color1);
    painter.drawRoundedRect(QRectF(0, 0, size.width(), size.height()), 10, 10); // рисуем квадрат с круглыми углами
    cover.setMask(*map);
    loadPicture(url);
}

void IconLabel::setCover(QByteArray data) {
    cover.loadFromData(data);
    cover = cover.scaled(size, Qt::IgnoreAspectRatio, Qt::SmoothTransformation); // Qt::KeepAspectRatio
    cover.setMask(*map);
    this->setPixmap(cover);
}

void IconLabel::loadPicture(QString url) {
    qDebug() << url;
    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    manager->get(QNetworkRequest(QUrl(url)));
    connect(manager, &QNetworkAccessManager::finished, this, &IconLabel::loadPictureData);
}

void IconLabel::loadPictureData(QNetworkReply *reply) {
    if (!reply->error()) {
        QByteArray data(reply->readAll());
        if (!data.isEmpty()) {
            setCover(data);
        }
        else {
            qDebug() << "Error. Data is empty";
        }
    }
    else {
        qDebug() << "Network Error";
    }

}
