#include "navigationtabwidget.h"
#include "verticaltabbar.h"

NavigationTabWidget::NavigationTabWidget()
{
    this->setTabBar(new VerticalTabBar);
    this->setTabPosition(QTabWidget::West);

    this->setStyleSheet("\
                        QTabWidget::pane {\
                            border: 0;\
                        }\
                        QTabBar::tab {\
                            background-color: rgb(20, 20, 20);\
                            color: white;\
                        }\
                        QTabBar::tab:hover {\
                            background: qlineargradient(x1:0, y1:0, x2:1, y1:1,\
                                stop:0 rgb(75,75,75), stop:1 rgb(53,53,53));\
                        }\
                        QTabBar::tab:selected {\
                            background-color: rgb(98,97,97);\
                        }\
                        QTabWidget::tab-bar {\
                            background-color: rgb(98,97,97);\
                            min-width: 50px;\
                            min-height: 100px;\
                            alignment: center;\
                        };\
                        ");
}
