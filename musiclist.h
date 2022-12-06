#ifndef MUSICLIST_H
#define MUSICLIST_H

#include "includes.h"
#include "musicplayer.h"

class MusicWindow : public QMainWindow
{
    Q_OBJECT
public:
    MusicWindow(QWidget *parent = nullptr);

private:
    QMap<QString, QList<QString>> *playlist = new QMap<QString, QList<QString>>; // словарь с музыкой списка музыки, ключи - music_id
    MusicPlayer *musicplayer;
    QString playlistname;
    QGridLayout *layout;
    QWidget *parent;


public slots:
    MusicButton* addMusic(QString musicCover, QString musicTitle, QString url, QString musicId, QString duration, bool isLiked);
    void deleteMusicWidget(MusicButton* musicbutton); // полностью удалить виджет музыкальной кнопки
    void removeAllMusics(); // удалить всю муызыку
    void removeMusic(QString musicId); // удалить определённую музыку
    void removeLikeFromMusic(QString musicId); // убрать лайк с музыки
    void updateMusicButtonsRects(int sinceIndex/*индекс с которого обновить*/); // обновляю координаты кнопок для анимации

    void clickOnMusic();
    void clickOnLike();
    void setPlaylist(QMap<QString, QList<QString>> *playlist, QString playlistname);
    void setMusicPlayer(MusicPlayer *player);
    void setPlaylistName(QString name);
};

#endif // MUSICLIST_H
