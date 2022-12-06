#include "musicbutton.h"
#include "changeiconbutton.h"
#include "iconlabel.h"
#include <QPushButton>
#include <QHBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QMenu>
#include <QGraphicsEffect>
#include <QRect>
#include <QEvent>

const int DurationLabelMaxWIDTH = 50;
const int MusicButtonMinimumHeight = 60;
const int AnimationDuration = 100;
const int SPACING = 10;
const QSize ICON_SIZE = QSize(45, 45);

const QString noLikedDefaultIcon = "";
const QString noLikedChangeIcon = ":/images/icons/line-heart.png";
const QString LikedDefaultIcon = ":/images/icons/grey-heart.png";
const QString LikedChangeIcon = ":/images/icons/red-heart.png";

MusicButton::MusicButton(QWidget *parent, QString coverUrl, QString musicTitle, QString duration, bool isliked) : QPushButton(parent)
{
    this->setCheckable(true);
    isLiked = isliked;
    QString style = QString("\
            QPushButton {\
                text-align:left;\
                background-color: rgb(24,24,24);\
                border-style: outset;\
                font: bold 14px;}\
            QPushButton:hover {\
                background: %1;\
            }\
            QPushButton:checked {\
                background: %1;\
            }\
            QPushButton:pressed {\
                background-color: rgb(98,97,97);\
            };").arg("qlineargradient(x1:0, y1:0, x2:1, y2:1\
                                    stop:0 rgb(66, 66, 66), stop:1 rgb(42, 42, 42))");

    this->setStyleSheet(style);
    this->setMinimumHeight(MusicButtonMinimumHeight);
    this->setLayout(new QHBoxLayout);
    this->layout()->setSpacing(SPACING);

    IconLabel *icon_label = new IconLabel(this, coverUrl, ICON_SIZE);

    this->layout()->addWidget(icon_label);

    //
    QLabel *textlabel = new QLabel(musicTitle);
    textlabel->setStyleSheet("background-color: transparent; color: white; text-align: left;");
    textlabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    textlabel->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    this->layout()->addWidget(textlabel);

    QString hearticon;
    QString changehearticon;

    // создаём кнопку лайка
    like = new ChangeIconButton(this, "", "rgb(220, 220, 220)");
    setLikeIcons(); // устанаваливает иконки кнопки лайка, взависимости от того, добавлена ли музыка в "моя музыка"
    connect(like, SIGNAL(clicked()), SLOT(clickLike()));
    this->layout()->addWidget(like);
    //

    // надпись длительности музыки
    QLabel *durationLabel = new QLabel(duration);
    durationLabel->setStyleSheet("background-color: transparent; color: white;");
    durationLabel->setMaximumWidth(DurationLabelMaxWIDTH);
    this->layout()->addWidget(durationLabel);
    //

    // анимация при нажатии
    _animation = new QPropertyAnimation(this, "geometry");
    connect(_animation, SIGNAL(finished()), SLOT(changeAnimationState()));
    _animation->setDuration(AnimationDuration);
    _animation->setEasingCurve(QEasingCurve::OutCubic); // кривая изменения значения с течением времени

//    connect(this, SIGNAL(pressed()), SLOT(startPressedAnimation()));
//    connect(this, SIGNAL(clicked()), SLOT(startClickAnimation()));
    //
}

MusicButton::~MusicButton()
{

}

// устанаваливает иконки кнопки лайка, взависимости от того, добавлена ли музыка в "моя музыка"
void MusicButton::setLikeIcons() {
    if (isLiked) {
        like->setChangeIcon(LikedChangeIcon);
        like->setDefaultIcon(LikedDefaultIcon);
    }
    else {
        like->setChangeIcon(noLikedChangeIcon);
        like->setDefaultIcon(noLikedDefaultIcon);
    }
}

// событие нажатия на музыку
void MusicButton::clickMusic() {
    startClickAnimation(); // запуск анимации нажатия
}

// событие нажатия на кнопку лайка
void MusicButton::clickLike() {
    isLiked = !isLiked; // меняем статус кнопки
    setLikeIcons(); // устанавливаем новые иконки
    like->changeIcon(like->changeIconPath); // чтобы сразу менялась иконка после нажатия
}

void MusicButton::setMenuStyle() {
    menu = new QMenu;
    menu->setWindowFlag(Qt::FramelessWindowHint);
    menu->setAttribute(Qt::WA_TranslucentBackground);
    menu->setStyleSheet("\
                        QMenu {\
                            background-color: rgb(30, 30, 30);\
                            color: white;\
                            font-size: 13px;\
                            border-radius: 4px;\
                            border: 1px solid rgb(110, 110, 110);\
                            min-width: 200px;\
                            padding-top: 3px;\
                        }\
                        QMenu::item {\
                            background: transparent;\
                        }\
                        QMenu::item:selected {\
                            background: qlineargradient(x1:0, y1:0, x2:1, y1:1,\
                                        stop:0 rgb(75,75,75), stop:1 rgb(53,53,53));\
                        }");
    parametrs->setMenu(menu);
}

void MusicButton::addActionToMenu(QString iconpath, QString actionname) {
    QAction *action = new QAction(actionname);
    action->setIcon(QIcon(iconpath));
    menu->addAction(action);
}


// анимации при нажатии
void MusicButton::startPressedAnimation() {
    _animation->stop();
    _animation->setStartValue(QRectF(start_animation_x, start_animation_y, start_animation_width, start_animation_height));
    _animation->setEndValue(QRectF(start_animation_x + 10, start_animation_y, start_animation_width - 20, start_animation_height));
    is_animation_going = true;
    _animation->start();

}

void MusicButton::startClickAnimation() {
    _animation->stop();
    _animation->setStartValue(QRectF(start_animation_x + 10, start_animation_y, start_animation_width - 20, start_animation_height));
    _animation->setEndValue(QRectF(start_animation_x, start_animation_y, start_animation_width, start_animation_height));
    is_animation_going = true;
    _animation->start();
}

// при изменении размеров MusicButton стартовая позиция анимации изменяется
void MusicButton::update_rect() {
//    qDebug() << "БЫЛА___________";
//    qDebug() << start_animation_x;
//    qDebug() << start_animation_y;
//    qDebug() << start_animation_width;
//    qDebug() << start_animation_height;
//    qDebug() << "_____________";
    start_animation_x = this->pos().x();
    start_animation_y = this->pos().y();
    start_animation_width = this->size().width();
    start_animation_height = this->size().height();
//    qDebug() << "СТАЛО___________";
//    qDebug() << start_animation_x;
//    qDebug() << start_animation_y;
//    qDebug() << start_animation_width;
//    qDebug() << start_animation_height;
//    qDebug() << "_____________";
}

bool MusicButton::event(QEvent *event) {
   if ((event->type() == QEvent::Resize) & !is_animation_going) {
        update_rect();
    }
   if (event->type() == QEvent::HoverEnter) {
        like->changeIcon(like->changeIconPath);
   }
   if (event->type() == QEvent::HoverLeave) {
        like->changeIcon(like->defaultIconPath);
   }
   return QWidget::event(event);
}


