#include "verticaltabbar.h"

VerticalTabBar::VerticalTabBar(QTabBar *parent) : QTabBar(parent)
{
}


void VerticalTabBar::paintEvent(QPaintEvent *) {
    QStylePainter painter(this);
    QStyleOptionTab option; // настройки вкладки

    for (int i = 0; i < this->count(); i++) {
        this->initStyleOption(&option, i); // иницализируем стиль i по счету вкладки

        painter.drawControl(QStyle::CE_TabBarTabShape, option); // рисуем форму самой вкладки
        painter.save(); // сохраняем старые настройки

        QSize size = option.rect.size(); // получаем размер прямоуголника в который заключена вкладка
        size.transpose();
        QRect rect(QPoint(), size);
        rect.moveCenter(option.rect.center());
        option.rect = rect;

        QPoint centerpoint = this->tabRect(i).center();
        painter.translate(centerpoint);
        painter.rotate(90);
        painter.translate(-centerpoint);
        painter.drawControl(QStyle::CE_TabBarTabLabel, option); // рисуем название вкладки
        painter.restore(); // восстанавливаем старые настройки
    }
}
