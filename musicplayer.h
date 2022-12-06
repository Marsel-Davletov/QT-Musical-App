#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include "iconlabel.h"
#include "clickedchangeiconbutton.h"
#include "musicbutton.h"
#include "includes.h"

class MusicPlayer : public QWidget
{
    Q_OBJECT

public:
    int currentMusicIndex;

    MusicPlayer(QWidget *parent = nullptr);
    ~MusicPlayer();

protected:
    QNetworkAccessManager *networkManager;
    QMediaPlayer *mediaplayer;
    QAudioOutput *audioOutput;

    QSlider *musicroleslider;
    QSlider *volumechange;

    ChangeIconButton *stop_play;
    qint64 startMusicPosition = 0;

    QString currentMusicUrl = ""; // url играющей музыки
    QString currentPlaylistName = ""; // название установленного плейлиста
    QString currentMusicId = ""; // youtube id играющей музыки
    MusicButton *currentMusicButton; // youtube id играющей музыки
    bool isMusicPlay = false;
    bool isSliderPressed = false;

    QString serverUrl = "";

private:
    QMap<QString, QList<QString>> *currentPlaylist = new QMap<QString, QList<QString>>;
    IconLabel *icon_label;
    QLabel *textlabel;
    QWidget *parent;

public slots:
    void setMusic(QString musicTitle, QString url);
    void playMusic(QString coverPath, QString musicTitle, QString musicId, QString url);
    void positionMusicDuration(qint64 duration);
    void durationMusicDuration(qint64 duration);
    void musicSliderPressed();
    void musicSliderReleased();
    void musicSliderMoved(int value);
    void changeVolume(int value);
    void clickPlayMusic();
    void setPlaylist(QMap<QString, QList<QString>> *playlist, QString playlistname);

    // управление музыкой
    void StopMediaPlayer();
    void PlayMediaPlayer();
    void nextMusic();
    void previousMusic();
    //

    void setCurrentMusicIndex(int newIndex);
    void playMusicFromPlaylist(int index);

    //
    void catchError();
    void setServerUrl(QString url);
    void updadeMusicUrl(QNetworkReply *reply);

    QString getPlaylistName();
};
#endif // MUSICPLAYER_H
