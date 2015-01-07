#include <QIcon>

#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    createActions();
    createTrayIcon();

    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    trayIcon->show();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::closeEvent(QCloseEvent *event)
{
    if (trayIcon->isVisible()) {
        hide();
        trayIcon->showMessage("Whistle My Mouse", "This is program is running "
                              "in the background");
        event->ignore();
    }
}

void Widget::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::DoubleClick) {
        show();
    }
}

void Widget::createTrayIcon()
{
    if (QSystemTrayIcon::isSystemTrayAvailable()) {
        trayMenu = new QMenu();
        trayMenu->addAction(quitAction);

        trayIcon = new QSystemTrayIcon(QIcon("icon.jpg"));
        trayIcon->setContextMenu(trayMenu);
        trayIcon->setToolTip("Whistle My Mouse");
    }
}

void Widget::createActions()
{
    quitAction = new QAction("Quit", this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}
