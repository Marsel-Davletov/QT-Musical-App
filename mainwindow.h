#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "includes.h"
#include "searchmusicwindow.h"


class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

private:
    QButtonGroup *buttongroup;
    MusicWindow *mymusicwindow;
    SearchMusicWindow *searchmusicwindow;
    QMap<QString, QList<QString>> *MyMusicPlaylist = new QMap<QString, QList<QString>>;
    QMap<QString, QMap<QString, MusicButton*>*> *MusicButtonsWidgetMap = new QMap<QString, QMap<QString, MusicButton*>*>;

public slots:
    void addMusicInMyMusicList(QString cover, QString title, QString url, QString musicID, QString duraton, bool isLiked);
    void removeMusicInMyMusicList(QString musicId);

    // обращение к элементам класса
    SearchMusicWindow *getSearchMusicWindow();
    QButtonGroup *getButtonGroup();
    QMap<QString, QList<QString>> *getMyMusicPlaylist();
    QMap<QString, QMap<QString, MusicButton*>*> *getMusicButtonsWidgetMap();
};


#endif // MAINWINDOW_H
