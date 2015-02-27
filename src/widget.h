#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QCloseEvent>
#include <QAction>
#include <QAudioRecorder>
#include <QAudioProbe>
#include <QAudioBuffer>
#include <QButtonGroup>
#include <QPushButton>

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
    void onStateChanged(QMediaRecorder::State state);
    void toggleRecord(QAbstractButton *button);
    void updateProgress(qint64 duration);
    void processBuffer(const QAudioBuffer &buffer);

private:
    void createTrayIcon();
    void createAudioRecorder();
    void createDeviceList();
    void createRecordButtonsGroup();

    Ui::Widget *ui;

    QSystemTrayIcon *trayIcon;
    QMenu *trayMenu;
    QAudioRecorder *audioRecorder;
    QAudioProbe *probe;
    QButtonGroup *recordButtons;
    QPushButton *whichPressed;

    QAction *quitAction;
    QAction *pauseAction;
    QVector<quint16> buff;
};

#endif // WIDGET_H
