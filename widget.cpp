#include <QIcon>

#include "include/qfouriertransformer.h"

#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    createTrayIcon();
    createAudioRecorder();
    createDeviceList();
    createRecordButtonsGroup();

    connect(audioRecorder, SIGNAL(stateChanged(QMediaRecorder::State)),
            this, SLOT(onStateChanged(QMediaRecorder::State, QAbstractButton*)));
    connect(audioRecorder, SIGNAL(durationChanged(qint64)),
            this, SLOT(updateProgress(qint64)));
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
            this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
    connect(recordButtons, SIGNAL(buttonClicked(QAbstractButton*)),
            this, SLOT(toggleRecord()));
    connect(probe, SIGNAL(audioBufferProbed(QAudioBuffer)),
            this, SLOT(processBuffer(QAudioBuffer)));
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

void Widget::onStateChanged(QMediaRecorder::State state, QAbstractButton *button)
{
    switch (state) {
        case QMediaRecorder::RecordingState:
            button->setText("Stop");
            break;
        case QMediaRecorder::PausedState:
            button->setText("Pause");
            break;
        case QMediaRecorder::StoppedState:
            button->setText("Pause");
            break;
    }
}

void Widget::createTrayIcon()
{
    if (QSystemTrayIcon::isSystemTrayAvailable()) {
        quitAction = new QAction("Quit", this);
        connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

        trayMenu = new QMenu();
        trayMenu->addAction(quitAction);

        trayIcon = new QSystemTrayIcon(QIcon("icon.jpg"));
        trayIcon->setContextMenu(trayMenu);
        trayIcon->setToolTip("Whistle My Mouse");

        trayIcon->show();
    }
}

void Widget::createAudioRecorder()
{
    audioRecorder = new QAudioRecorder();
    probe = new QAudioProbe;
    probe->setSource(audioRecorder);
}

void Widget::createDeviceList()
{
    ui->audioDeviceBox->addItem("Default", QVariant(QString()));
    foreach (const QString &device, audioRecorder->audioInputs()) {
        ui->audioDeviceBox->addItem(device, QVariant(device));
    }
}

void Widget::createRecordButtonsGroup()
{
    recordButtons = new QButtonGroup();
    recordButtons->addButton(ui->buttonRecordUp);
    recordButtons->addButton(ui->buttonRecordDown);
    recordButtons->addButton(ui->buttonRecordLeft);
    recordButtons->addButton(ui->buttonRecordRight);
}

static QVariant boxValue(const QComboBox *box)
{
    int idx = box->currentIndex();
    if (idx == -1)
        return QVariant();

    return box->itemData(idx);
}

void Widget::toggleRecord()
{
    if (audioRecorder->state() == QMediaRecorder::StoppedState) {
        audioRecorder->setAudioInput(boxValue(ui->audioDeviceBox).toString());

        QAudioEncoderSettings settings;

        //audioRecorder->setEncodingSettings(settings, QVideoEncoderSettings(), container);
        audioRecorder->record();
    }
    else {
        audioRecorder->stop();
    }
}

void Widget::updateProgress(qint64 duration)
{
    if (audioRecorder->error() != QMediaRecorder::NoError || duration < 2000)
        return;

    ui->statusbar->setText(tr("Recorded %1 seconds").arg(duration / 1000));
    audioRecorder->stop();
}
