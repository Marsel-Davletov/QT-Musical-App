#include "musiclist.h"
#include "mainwindow.h"
#include "musicbutton.h"
#include "musicplayer.h"
#include <QScrollArea>
#include <QGridLayout>

extern void addMusicInMyMusic(QString musicId, QString cover, QString title, QString duraton, QString url);
extern void removeMusicInMyMusic(QString musicID, QString musicWindowObjectName);

MusicWindow::MusicWindow(QWidget *par) : QMainWindow(par)
{
    this->setStyleSheet("background-color: rgb(24, 24, 24); padding: 0px; margin: 0xp;");
    parent = par;
    // заполняем правую часть экрана - с музыкой
    QScrollArea *area = new QScrollArea(this); // создаём сам виджет прокрутки
    area->setStyleSheet("border: 0px;");
    area->setWidgetResizable(true); // возможность изменять размер виджетов
    this->setCentralWidget(area); // установить виджет прокрутки как центральный виджет

    QWidget *area_widget = new QWidget; // виджет - контейнер
    area->setWidget(area_widget); // устанавливаем виджет - контейнер
    layout = new QGridLayout(area_widget);
    //layout->setContentsMargins(0, 0, 0, 0);
    layout->setAlignment(Qt::AlignTop); // Чтобы виджеты появлялись не по центру а наверху
}

MusicButton* MusicWindow::addMusic(QString musicCover, QString musicTitle, QString url, QString musicId, QString duration, bool isLiked) {
    MusicButton *musicbutton = new MusicButton(this, musicCover, musicTitle, duration, isLiked);
    musicbutton->setObjectName(musicId);
    connect(musicbutton, SIGNAL(clicked()), this, SLOT(clickOnMusic()));
    connect(musicbutton->like, SIGNAL(clicked()), this, SLOT(clickOnLike()));
    layout->addWidget(musicbutton);

    // получаем mainwindow, чтобы получить доступ как единой группе кнопок
    MainWindow *mainwindowparent = qobject_cast<MainWindow*>(parent);
    mainwindowparent->getButtonGroup()->addButton(musicbutton);
    qDebug() << "PLAYLSIT: " << playlistname;

    QMap<QString, MusicButton*> *musicButtonsMap = mainwindowparent->getMusicButtonsWidgetMap()->value(playlistname);
    if (musicButtonsMap) {
        musicButtonsMap->insert(musicId, musicbutton);
    }
    else {
        qDebug() << "ERROR. Cant find music in QMap";
    }

    return musicbutton;
}

// полностью удалить виджет музыкальной кнопки
void MusicWindow::deleteMusicWidget(MusicButton* musicbutton) {

    // получаем mainwindow, чтобы получить доступ как единой группе кнопок
    MainWindow *mainwindowparent = qobject_cast<MainWindow*>(parent);
    //

    QMap<QString, MusicButton*> *musicButtonsMap = mainwindowparent->getMusicButtonsWidgetMap()->value(playlistname);
    if (musicButtonsMap) {
        musicButtonsMap->remove(musicbutton->objectName());
        mainwindowparent->getButtonGroup()->removeButton(musicbutton);
        layout->removeWidget(musicbutton);
        delete musicbutton;
    }
    else {
        qDebug() << "ERROR. Cant find music in QMap";
    }
}

// удалить всю музыку
void MusicWindow::removeAllMusics() {
    QList<MusicButton*> allMusicButtons = this->findChildren<MusicButton*>();
    for (int i = 0; i < allMusicButtons.count(); i++) {
        deleteMusicWidget(allMusicButtons[i]);
    }
}

// удалить определённую музыку
void MusicWindow::removeMusic(QString musicId) {
    QList<MusicButton*> allMusicButtons = this->findChildren<MusicButton*>();
    for (int i = 0; i < allMusicButtons.count(); i++) {
        if (allMusicButtons[i]->objectName() == musicId) {
            deleteMusicWidget(allMusicButtons[i]);
        }
    }
}

// убрать лайк с музыки
void MusicWindow::removeLikeFromMusic(QString musicId) {
    QList<MusicButton*> allMusicButtons = this->findChildren<MusicButton*>();
    for (int i = 0; i < allMusicButtons.count(); i++) {
        if (allMusicButtons[i]->objectName() == musicId) {
            allMusicButtons[i]->clickLike();
        }
    }
}

// получить индекс музыки в плайлисте
int indexOfMusicInPlaylsit(QMap<QString, QList<QString>> *playlist, QString musicID) {
    for (int i = 0; i < playlist->count(); i++) {
        if (playlist->keys().at(i) == musicID) {
            return i;
        }
    }
    return -1;
}

void MusicWindow::clickOnMusic(){
    qDebug() << "playlist name: " << playlistname;
    qDebug() << "playlist: " << playlist->count();
    if (musicplayer->getPlaylistName() != playlistname) {
        musicplayer->setPlaylist(playlist, playlistname);
        qDebug() << "запускаю музыку из другого плейлиста";
    }
    QList<QString> musicInfo = playlist->value(sender()->objectName());
    QString musicCover = musicInfo[0];
    QString musicTitle = musicInfo[1];
    QString url = musicInfo[3];
    QString musicId = sender()->objectName();

    qDebug() << "music URL" << url;

    int musicIndex = indexOfMusicInPlaylsit(playlist, musicId);
    if (musicIndex != -1) {
        musicplayer->playMusic(musicCover, musicTitle, musicId, url);
        musicplayer->setCurrentMusicIndex(musicIndex);
    }
    else {
        qDebug() << "ERROR. Music didn't find in playlist.";
    }
}

void MusicWindow::clickOnLike() {
    MusicButton *musicbutton = qobject_cast<MusicButton*>(sender()->parent());
    QString musicId = musicbutton->objectName();
    if (musicbutton->isLiked) {
        QList<QString> musicInfo = playlist->value(musicId);
        QString cover = musicInfo[0];
        QString title = musicInfo[1];
        QString duration = musicInfo[2];
        QString url = musicInfo[3];
        addMusicInMyMusic(musicId, cover, title, duration, url);
    }
    else {
        int musicButtonIndex = indexOfMusicInPlaylsit(playlist, musicId);
        removeMusicInMyMusic(musicId, this->objectName());
//        updateMusicButtonsRects(musicButtonIndex);
    }
}

void MusicWindow::updateMusicButtonsRects(int sinceIndex) {
    QList<MusicButton*> allMusicButtons = this->findChildren<MusicButton*>();
    for (int i = 0; i < allMusicButtons.count(); i++) {
        allMusicButtons[i]->update_rect();
    }
}

void MusicWindow::setMusicPlayer(MusicPlayer *player) {
    musicplayer = player;
}

void MusicWindow::setPlaylistName(QString name) {
    playlistname = name;
}

void MusicWindow::setPlaylist(QMap<QString, QList<QString>> *plist, QString name) {
    playlist = plist;
    playlistname = name;
    musicplayer->setPlaylist(playlist, name);
}


