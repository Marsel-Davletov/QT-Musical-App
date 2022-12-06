#include "musicplayer.h"
#include "changeiconbutton.h"
#include "clickedchangeiconbutton.h"
#include "hoverchangeiconbutton.h"
#include "iconlabel.h"
#include "mainwindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSlider>
#include <QPushButton>
#include <QAction>
#include <QAudioOutput>
#include <QMediaMetaData>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>

float DefaultVolume = 50;

MusicPlayer::MusicPlayer(QWidget *par)
    : QWidget(par)
{
    parent = par;
    this->setStyleSheet("background-color: transparent;");

    mediaplayer = new QMediaPlayer(this);
    connect(mediaplayer, SIGNAL(positionChanged(qint64)), SLOT(positionMusicDuration(qint64)));
    connect(mediaplayer, SIGNAL(durationChanged(qint64)), SLOT(durationMusicDuration(qint64)));
    connect(mediaplayer, SIGNAL(errorChanged()), SLOT(catchError()));
    audioOutput = new QAudioOutput;
    mediaplayer->setAudioOutput(audioOutput);
    audioOutput->setVolume(DefaultVolume / 100);
    mediaplayer->play();

    QVBoxLayout *mainlayout =  new QVBoxLayout(this);
    this->setLayout(mainlayout);
//    mainlayout->setContentsMargins(5, 0, 5, 25);

    QHBoxLayout *navigationlayout = new QHBoxLayout;
    navigationlayout->setAlignment(Qt::AlignLeft);

    ChangeIconButton *to_left = new ChangeIconButton(this, ":/images/icons/left.png");
    navigationlayout->addWidget(to_left);
    connect(to_left, SIGNAL(clicked()), SLOT(previousMusic()));

    stop_play = new ChangeIconButton(this, ":/images/icons/play.png");
    connect(stop_play, SIGNAL(clicked()), SLOT(clickPlayMusic()));
    stop_play->setChangeIcon(":/images/icons/stop.png");
    navigationlayout->addWidget(stop_play);

    ChangeIconButton *to_right = new ChangeIconButton(this, ":/images/icons/right.png");
    navigationlayout->addWidget(to_right);
    connect(to_right, SIGNAL(clicked()), SLOT(nextMusic()));

    QSize size = QSize(45, 45);

    // описание музыки - обложка и название
    QHBoxLayout *descriptionlayout = new QHBoxLayout;
    descriptionlayout->setSpacing(20);
    descriptionlayout->setContentsMargins(50, 0, 0, 0);
    descriptionlayout->setAlignment(Qt::AlignHCenter);
    navigationlayout->addLayout(descriptionlayout);

    icon_label = new IconLabel(this, ":/images/icons/defaul_cover.png", size);
    descriptionlayout->addWidget(icon_label);

    textlabel = new QLabel("Музыка не выбрана", this);
    textlabel->setStyleSheet("color: white; text-align: left;");
    textlabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    textlabel->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    descriptionlayout->addWidget(textlabel);
    //

    // ползунок громкости
    volumechange = new QSlider;
    volumechange->setToolTip(QString::number(DefaultVolume));
    volumechange->setOrientation(Qt::Orientation::Horizontal);
    volumechange->setStyleSheet("\
                            QSlider::groove:horizontal {\
                                height: 4px;\
                                margin: 0px;\
                                border-radius: 1px;\
                                background: rgb(121, 121, 121);\
                            }\
                            QSlider::handle:horizontal {\
                                background: rgb(34, 34, 34);\
                                border: 3px solid #E3DEE2;\
                                width: 8px;\
                                margin: -5px 0;\
                                border-radius: 6px;\
                            }\
                            QSlider::sub-page:qlineargradient {\
                                background: white;\
                                border-radius: 5px;\
                            }");
    connect(volumechange, SIGNAL(sliderMoved(int)), SLOT(changeVolume(int)));
    volumechange->setMaximum(100);
    volumechange->setValue(DefaultVolume);
    volumechange->setMaximumWidth(150);
    navigationlayout->addWidget(volumechange);
    //

    musicroleslider = new QSlider;

    musicroleslider->setToolTip("0:0");
    musicroleslider->setOrientation(Qt::Orientation::Horizontal);
    musicroleslider->setStyleSheet("\
                            QSlider::groove:horizontal {\
                                height: 4px;\
                                margin: 0px;\
                                border-radius: 1px;\
                                background: rgb(121, 121, 121);\
                            }\
                            QSlider::handle:horizontal {\
                                background: rgb(34, 34, 34);\
                                border: 3px solid #E3DEE2;\
                                width: 8px;\
                                margin: -5px 0;\
                                border-radius: 6px;\
                            }\
                            QSlider::sub-page:qlineargradient {\
                                background: white;\
                                border-radius: 5px;\
                            }");
    connect(musicroleslider, SIGNAL(sliderPressed()), SLOT(musicSliderPressed()));
    connect(musicroleslider, SIGNAL(sliderMoved(int)), SLOT(musicSliderMoved(int)));
    connect(musicroleslider, SIGNAL(sliderReleased()), SLOT(musicSliderReleased()));

    mainlayout->addWidget(musicroleslider);
    mainlayout->addLayout(navigationlayout);

    networkManager = new QNetworkAccessManager;
    connect(networkManager, &QNetworkAccessManager::finished, this, &MusicPlayer::updadeMusicUrl);

}

MusicPlayer::~MusicPlayer()
{
}

void MusicPlayer::setPlaylist(QMap<QString, QList<QString>> *playlist, QString playlistname) {
    currentPlaylist = playlist;
    currentPlaylistName = playlistname;
    currentMusicIndex = 0;
}


// установить обложку музыки и ее название в плейер
void MusicPlayer::setMusic(QString musicTitle, QString url) {
    textlabel->setText(musicTitle);
    icon_label->loadPicture(url);
}

// включить музыку
void MusicPlayer::playMusic(QString coverPath, QString musicTitle, QString musicId, QString url) {
    if (currentMusicUrl == url && isMusicPlay) {
        StopMediaPlayer();
    }
    else {
        if (!(currentMusicUrl == url && !isMusicPlay)){
            startMusicPosition = 0;
            currentMusicUrl = url;
            currentMusicId = musicId;
            setMusic(musicTitle, coverPath);
            mediaplayer->setSource(QUrl(url));
            qDebug() << "currentMusicUrl: " << currentMusicUrl;
        }
        PlayMediaPlayer();
    }
}

QString get_music_position_string(qint64 position) {
    int minutes = position / 1000 / 60;
    int seconds = (position - minutes * 1000 * 60) / 1000;
    QString musicTime = QString::number(minutes) + ":" + QString::number(seconds);
    return musicTime;
}

void MusicPlayer::positionMusicDuration(qint64 position) {
    musicroleslider->setToolTip(get_music_position_string(position));
    if (!isSliderPressed && isMusicPlay) {
        musicroleslider->setValue(position);
    }
    if (position == mediaplayer->duration()) {
        nextMusic();
    }
}

void MusicPlayer::durationMusicDuration(qint64 duration) {
    musicroleslider->setMaximum(duration);
}

void MusicPlayer::musicSliderPressed() {
    isSliderPressed = true;
}

void MusicPlayer::musicSliderReleased() {
    isSliderPressed = false;
}

void MusicPlayer::musicSliderMoved(int value) {
//    musicroleslider->setValue(value);
    mediaplayer->setPosition(value);
}

void MusicPlayer::clickPlayMusic() {
    if (!isMusicPlay) {
        PlayMediaPlayer();
    }
    else {
        StopMediaPlayer();
    }
}

void MusicPlayer::StopMediaPlayer() {
    startMusicPosition = mediaplayer->position();
    isMusicPlay = false;
    stop_play->changeIcon(stop_play->defaultIconPath);
    mediaplayer->stop();
}

void MusicPlayer::PlayMediaPlayer() {
    qDebug() << "play url";
    mediaplayer->setPosition(startMusicPosition);
    isMusicPlay = true;
    stop_play->changeIcon(stop_play->changeIconPath);
    mediaplayer->play();
    qDebug() << mediaplayer->errorString();
}

void MusicPlayer::changeVolume(int value) {
    volumechange->setToolTip(QString::number(value));
    audioOutput->setVolume(float(value) / 100);
}

void MusicPlayer::setCurrentMusicIndex(int newIndex) {
    currentMusicIndex = newIndex;
}

// играется музыка из плейлиста по индексу, функция исп в nextMusic и в previousMusic
void MusicPlayer::playMusicFromPlaylist(int index) {    
    QString musicId = currentPlaylist->keys().at(index);
    QList<QString> musicInfo = currentPlaylist->value(musicId);
    QString coverPath = musicInfo[0];
    QString musicTitle = musicInfo[1];
    QString url = musicInfo[3];
    playMusic(coverPath, musicTitle, musicId, url);


    // получаем mainwindow, чтобы получить доступ как единой группе кнопок
    MainWindow *mainwindowparent = qobject_cast<MainWindow*>(parent);
    // устанавливаем setChecked true новой музыке, потому что она будет воспроизводится
    QMap<QString, MusicButton*> *musicButtonsMap = mainwindowparent->getMusicButtonsWidgetMap()->value(currentPlaylistName);
    if (musicButtonsMap) {
        qDebug() << musicButtonsMap->value(musicId);
        musicButtonsMap->value(musicId)->setChecked(true);
    }
    else {
        qDebug() << "ERROR. Cant find music in QMap";
    }
}

// включается следуюущая музыка из установленного плейлиста
void MusicPlayer::nextMusic() {
    if (currentMusicIndex < currentPlaylist->count() - 1) {
        currentMusicIndex++;
        playMusicFromPlaylist(currentMusicIndex);
    }
}

// включается предыдущая музыка из установленного плейлиста
void MusicPlayer::previousMusic() {
    if (currentMusicIndex > 0 && ((mediaplayer->position() <= 2000 && isMusicPlay) || (!isMusicPlay && startMusicPosition <= 2000))) {
        currentMusicIndex--;
        playMusicFromPlaylist(currentMusicIndex);
    }
    else {
        startMusicPosition = 0;
        mediaplayer->setPosition(0);
        musicroleslider->setValue(0);
    }
}

void MusicPlayer::catchError() {
    if (mediaplayer->error() == QMediaPlayer::Error::ResourceError) {
        networkManager->get(QNetworkRequest(QUrl(serverUrl + currentMusicId)));
    }
}

void MusicPlayer::updadeMusicUrl(QNetworkReply *reply) {
    qDebug() << "updadeMusicUrl";
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());
    QJsonObject jsonobject = document.object();

    if (currentPlaylistName != "mymusicplaylist") {
        QList<QString> musicInfo;
        musicInfo = currentPlaylist->value(currentPlaylist->keys().at(currentMusicIndex));
        QString url = jsonobject.value("url").toString(); // получа url из json файла
        currentPlaylist->insert(currentPlaylist->keys().at(currentMusicIndex), {musicInfo[0], musicInfo[1], musicInfo[2], url}); // меняю в плейлисте url с пустого или неакуального на новый
        playMusic(musicInfo[0], musicInfo[1], musicInfo[2], url); // воспроизвожу эту музыку
    }
}

void MusicPlayer::setServerUrl(QString url) {
    serverUrl = url;
}

QString MusicPlayer::getPlaylistName() {
    return currentPlaylistName;
}



