#ifndef MUSICBUTTON_H
#define MUSICBUTTON_H

#include "changeiconbutton.h"
#include "includes.h"

class MusicButton : public QPushButton
{
    Q_OBJECT
public:
    ChangeIconButton *like;
    bool isLiked;

    explicit MusicButton(QWidget *parent = nullptr, QString coverpath = "", QString musictitle = "", QString duration = "0:0", bool isLiked = false);
    ~MusicButton();


private:
    QMenu *menu;
    ChangeIconButton *parametrs;

    QPropertyAnimation *_animation;

    int start_animation_x;
    int start_animation_y;
    int start_animation_width;
    int start_animation_height;

    bool is_animation_going = false;


protected:
    bool event(QEvent *event);

public slots:
    void setLikeIcons(); // устанаваливает иконки кнопки лайка, взависимости от того, добавлена ли музыка в "моя музыка"
    void clickMusic();
    void clickLike();
    void setMenuStyle();
    void addActionToMenu(QString iconpath, QString actionname);

    void startClickAnimation();
    void startPressedAnimation();
    void update_rect();
};

#endif // MUSICBUTTON_H
