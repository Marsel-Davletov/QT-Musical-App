#include "searchmusicwindow.h"
#include "musiclist.h"
#include "mainwindow.h"
#include <QLineEdit>
#include <QVBoxLayout>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QThread>

extern void addMusicInMyMusic(QString musicId, QString cover, QString title, QString duraton, QString url);

const QString LineEditPlaceHolderText = "Поиск новой музыки";

SearchMusicWindow::SearchMusicWindow(QWidget *par)
    : QWidget{par}
{
//    QThread *thread = new QThread; // создаём поток для того чтобы поиск музыки не замораживал всё приложение
//    this->moveToThread(thread); // устанвливаем поток окну поиска музыки
//    connect(thread, SIGNAL(started()), SLOT(process()));
//    connect(this, SIGNAL(finished()), thread, SLOT(quit()));
//    thread->start();
    parent = par;
    networkManager = new QNetworkAccessManager;
    connect(networkManager, &QNetworkAccessManager::finished, this, &SearchMusicWindow::onResult);;

    this->setLayout(new QVBoxLayout);
    this->layout()->setContentsMargins(0, 10, 0, 0);

    searchline = new QLineEdit("");
    connect(searchline, &QLineEdit::textChanged, [=]{ style()->polish(searchline); });

    searchline->setPlaceholderText(LineEditPlaceHolderText);
    connect(searchline, SIGNAL(returnPressed()), SLOT(searchMusic()));
    searchline->setStyleSheet("\
                            QLineEdit {\
                                color: white;\
                                min-height: 25px;\
                                margin-left: 100px;\
                                margin-right: 100px;\
                                border: 0px;\
                                border-bottom: 1px solid rgb(78, 78, 78);\
                             }\
                            QLineEdit::focus {\
                                border-bottom: 1px solid rgb(210, 210, 210);\
                            }\
                            QLineEdit[text=\"\"]{\
                                 color: gray;\
                             }\
                            ");
    this->layout()->addWidget(searchline);

    musiclist = new MusicWindow(parent);
    musiclist->setPlaylistName("searchmusic"); // сам хз зачем это здесь
    musiclist->setObjectName("searchmusic");
    this->layout()->addWidget(musiclist);

}


void SearchMusicWindow::searchMusic() {
//    QLineEdit *searchline = qobject_cast<QLineEdit*>(sender());
    QString query = searchline->text();
    QString url_with_query = url + query + "/" + search_limit;
    qDebug() << url_with_query;
    networkManager->get(QNetworkRequest(QUrl(url_with_query)));
}

void SearchMusicWindow::onResult(QNetworkReply *reply) {
    if (!reply->error()) {
        QJsonDocument jsondocument = QJsonDocument::fromJson(reply->readAll()); // считываем весь результат запроса в json документ
        jsonobject = jsondocument.object(); // забираем из документа корневой объект

//        thread->start(); // loadMusic();
        loadMusic();

    }
    else {
        qDebug() << "Ошибка";
    }
}

void SearchMusicWindow::loadMusic() {
//    for (int i = 0; i < 1000000000000000000; i++) {
//        qDebug() << i;
//    }
    int result_len = search_limit.toInt(); // кол-во песен, которые будут выведенны
    bool isLiked; // лайкнута ли музыка
    if (jsonobject.count() < result_len) { // если вдруг нашлось меньше чем предел поиска, то выводим столько сколько нашлось
        result_len = jsonobject.count();
    }
    if (!jsonobject.isEmpty()) {
        musiclist->removeAllMusics();
        findmusicplaylist->clear();
        for (int i = 0; i < result_len; i++) {
            QJsonArray musicInfo = jsonobject.value(jsonobject.keys().at(i)).toArray();

            QString musicCover = musicInfo.at(0).toString();
            QString musicTitle = musicInfo.at(1).toString();
            QString duration = musicInfo.at(2).toString();
            QString url = musicInfo.at(3).toString();
            QString musicId = jsonobject.keys().at(i);

            isLiked = false;
            // музыка проверяется есть ли она в базе данных, если она лайкнута, то isLiked = true
            MainWindow *mainwindowparent = qobject_cast<MainWindow*>(parent);
            QList<QString> musicInfoList = {musicCover, musicTitle, duration, "_"};
            if (mainwindowparent->getMyMusicPlaylist()->key(musicInfoList) == musicId) {
                isLiked = true;
            }
            //
            MusicButton *musicbutton = musiclist->addMusic(musicCover, musicTitle, url, musicId, duration, isLiked);
//            mainwindowparent->getMusicButtonsWidgetMap()->value(playlistname)->insert()
//            findmusicplaylist[jsonobject.keys().at(i)] = {musicCover, musicTitle, duration, "_"};
            findmusicplaylist->insert(jsonobject.keys().at(i), musicInfoList);
        }
        musiclist->setPlaylist(findmusicplaylist, "searchmusic");
    }
}

// устанваливает url сервера для получения музыки
void SearchMusicWindow::setUrl(QString server_url) {
    url = server_url;
}

// устанавливаем лимит поиска - максимальное кол-во найденных песен
void SearchMusicWindow::setSearchLimit(QString limit) {
    search_limit = limit;
}

// устанавливаем виджет медиа плеера
void SearchMusicWindow::setMusicPlayer(MusicPlayer *player) {
    musiclist->setMusicPlayer(player);
}

// убрать лайк с музыки из списка музыки
void SearchMusicWindow::removeLikeFromMusic(QString musicId) {
    musiclist->removeLikeFromMusic(musicId);
}
