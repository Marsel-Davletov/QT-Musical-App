#include "mainwindow.h"
#include "musicplayer.h"
#include "searchmusicwindow.h"
#include "navigationtabwidget.h"

#include <QVBoxLayout>


MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    this->resize(1000, 600);
    this->setStyleSheet("background-color: rgb(20, 20, 20); padding: 0px; margin: 0xp;");
    this->setLayout(new QVBoxLayout);
    this->layout()->setContentsMargins(0, 0, 0, 0);

//    QMap<QString, QString> MyMusicButtons;
    MusicButtonsWidgetMap->insert("mymusic", new QMap<QString, MusicButton*>);
    MusicButtonsWidgetMap->insert("searchmusic", new QMap<QString, MusicButton*>);

    buttongroup = new QButtonGroup;

    MusicPlayer *musicplayer = new MusicPlayer(this); // создаём виджет плеера
    musicplayer->setServerUrl("http://127.0.0.1:5000/getMusicUrl/"); // задаём url сервера, для получения url музыки
//    player->setPlaylist(&myMusicPlaylist);

    mymusicwindow = new MusicWindow(this); // окно с лайкнутой пользователем музыки
    mymusicwindow->setMusicPlayer(musicplayer);
    mymusicwindow->setObjectName("mymusic"); // решить вопрос как это убрать, у меня и так есть playlist name
    mymusicwindow->setPlaylistName("mymusic");
    mymusicwindow->setPlaylist(MyMusicPlaylist, "mymusic");

//    QThread *thread = new QThread; // создаём поток для того чтобы поиск музыки не замораживал всё приложение
    searchmusicwindow = new SearchMusicWindow(this); // окно поиска музыки
    searchmusicwindow->setMusicPlayer(musicplayer);
//    searchmusicwindow->moveToThread(thread); // устанвливаем поток окну поиска музыки
//    connect(thread, SIGNAL(started()), searchmusicwindow, SLOT(loadMusic()));
//    thread->start();

    searchmusicwindow->setUrl("http://127.0.0.1:5000/"); // устанавливакем url сервера, для получения списка музыки

    NavigationTabWidget *tabs = new NavigationTabWidget;
    tabs->addTab(searchmusicwindow, "Найти музыку");
    tabs->setTabIcon(0, QIcon(":/images/icons/search_icon.png"));
    tabs->addTab(mymusicwindow, "Моя музыка");
    tabs->setTabIcon(1, QIcon(":/images/icons/music.png"));

    this->layout()->addWidget(tabs);
    this->layout()->addWidget(musicplayer);
}

void MainWindow::addMusicInMyMusicList(QString cover, QString title, QString url, QString musicID, QString duraton, bool isLiked) {
    mymusicwindow->addMusic(cover, title, url, musicID, duraton, isLiked);
}

void MainWindow::removeMusicInMyMusicList(QString musicId) {
    mymusicwindow->removeMusic(musicId);
}

SearchMusicWindow* MainWindow::getSearchMusicWindow() {
    return searchmusicwindow;
}

QMap<QString, QList<QString>>* MainWindow::getMyMusicPlaylist() {
    return MyMusicPlaylist;
}

QButtonGroup* MainWindow::getButtonGroup() {
    return buttongroup;
}

QMap<QString, QMap<QString, MusicButton*>*>* MainWindow::getMusicButtonsWidgetMap() {
    return MusicButtonsWidgetMap;
}



