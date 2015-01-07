#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QCloseEvent>
#include <QAction>

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);

private:
    void createTrayIcon();
    void createActions();

    Ui::Widget *ui;

    QSystemTrayIcon *trayIcon;
    QMenu *trayMenu;

    QAction *quitAction;
};

#endif // WIDGET_H
