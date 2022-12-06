#ifndef SEARCHMUSICWINDOW_H
#define SEARCHMUSICWINDOW_H

#include "musiclist.h"
#include "includes.h"
class SearchMusicWindow : public QWidget
{
    Q_OBJECT
public:
    explicit SearchMusicWindow(QWidget *parent = nullptr);

private:
    QWidget *parent;
    QString url = ""; // url сервера для получения музыки
    QString search_limit = "10";
    QString playlistname = "searchmusics";
    QNetworkAccessManager *networkManager; // класс отвечающий за механизм запросов на сервер
    QLineEdit *searchline; // строка поиска музыки
    MusicWindow *musiclist; // виджет со списком музыки
    QJsonObject jsonobject; // json файл с результатом поиска
    QMap<QString, QList<QString>> *findmusicplaylist = new QMap<QString, QList<QString>>;
//    QThread *thread;
//    QThread *thread = new QThread;

public slots:
    void removeLikeFromMusic(QString musicId); // убрать лайк с музыки
    void setMusicPlayer(MusicPlayer *player); // устанавливает media player
    void setUrl(QString server_url); // устанваливает url сервера для получения музыки
    void searchMusic(); // функция поиска музыки, отправляет запрос на сервер
    void onResult(QNetworkReply *reply); // после получения результата запроса, функция обрабатывает json файл
    void setSearchLimit(QString limit); // устанавливаем лимит поиска - максимальное кол-во найденных песен
    void loadMusic(); // выводит всю музыку из jsonobject
    void process() {
        loadMusic();
    }
//    void addAllMusic(QJsonObject jsonobject); // отображение всей

signals:
};

#endif // SEARCHMUSICWINDOW_H
