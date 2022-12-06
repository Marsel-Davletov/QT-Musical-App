#include "mainwindow.h"

// Посмотреть видеоурок работы с файлам

// проблемы которые надо решить:
// когда нажимаю на закончившийся трэк он должен начинаться заново
// не меняется позиция трэка, когда он на паузе
// сделать чтобы играющая музыка была серой
// сделать расстояние между кнопками "поиск музыки" и "моя музыка"


//extern QMap<QString, QList<QString>> FindMusicPlaylist;
MainWindow *mainwindow;

extern void addMusicInMyMusic(QString musicID, QString cover, QString title, QString duraton, QString url) {
    mainwindow->getMyMusicPlaylist()->insert(musicID, {cover, title, duraton, url});
    mainwindow->addMusicInMyMusicList(cover, title, url, musicID, duraton, true);
}

extern void removeMusicInMyMusic(QString musicID, QString musicWindowObjectName) {
    mainwindow->getMyMusicPlaylist()->remove(musicID);
    mainwindow->removeMusicInMyMusicList(musicID);
    if (musicWindowObjectName == "mymusic") {
        mainwindow->getSearchMusicWindow()->removeLikeFromMusic(musicID); // убираем лайк с музыки на экране поиска музыки
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    mainwindow = new MainWindow;

    mainwindow->show();
    return a.exec();
}
