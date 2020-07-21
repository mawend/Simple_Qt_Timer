#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QKeyEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    isPaused = false;
    ui->setupUi(this);

    timer = new QTimer(this);
    zeroTime = new QTime(0,0,0);
    player = new QMediaPlayer(this);
    player->setMedia(QUrl("qrc:/res/beep_beep.mp3"));
    this->setPlayerVolume(ui->sldVolume->value());
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setIcon(QIcon("qrc:/res/trayIcon.png"));
    trayIcon->show();
    ui->timeEdit->setFocus();

    connect(ui->btnStart, SIGNAL(clicked()), this, SLOT(start()));
    connect(ui->btnStop, SIGNAL(clicked()), this, SLOT(stop()));
    connect(ui->btnPause, SIGNAL(clicked()), this, SLOT(pause()));
    connect(ui->sldVolume, SIGNAL(valueChanged(int)), this, SLOT(setPlayerVolume(int)));
    connect(ui->sldVolume, SIGNAL(sliderReleased()), player, SLOT(play()));
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTimer()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete timer;
    delete zeroTime;
    delete player;
    delete trayIcon;
}

void MainWindow::updateTimer()
{
    ellapsedSeconds--;
    ui->timeEdit->setTime(startTime.addSecs(ellapsedSeconds));
    ui->progressBar->setValue(ui->progressBar->value()+1);
    ui->progressBar->repaint();
    if(!(ui->timeEdit->time().secsTo(*zeroTime))) {
        player->play();
        trayIcon->showMessage("Countdown", "Time ellapsed.");
        if(ui->cboxRepeat->isChecked()) {
            emit(restart());
        } else {
            emit(stop());
        }
    }
}

void MainWindow::start()
{
    if(!isPaused)
    {
        startTime = ui->timeEdit->time();
        ellapsedSeconds = 0;
        int prBarVal = ui->timeEdit->time().secsTo(*zeroTime);
        ui->progressBar->setRange(prBarVal, 0);
        ui->progressBar->setValue(prBarVal);
    }
    isPaused = false;
    ui->timeEdit->setEnabled(false);
    ui->btnStart->setEnabled(false);
    ui->btnStop->setEnabled(true);
    ui->btnPause->setEnabled(true);
    ui->statusBar->showMessage("Running...");
    timer->start(1000);
}

void MainWindow::stop()
{
    timer->stop();
    isPaused = false;
    ui->timeEdit->setTime(startTime);
    ui->progressBar->reset();
    ui->timeEdit->setEnabled(true);
    ui->btnStart->setEnabled(true);
    ui->btnPause->setEnabled(false);
    ui->statusBar->showMessage("Stopped");
    ui->btnStart->setFocus();
}

void MainWindow::pause()
{
    timer->stop();
    isPaused = true;
    ui->btnStart->setEnabled(true);
    ui->btnPause->setEnabled(false);
    ui->statusBar->showMessage("Paused");
}

void MainWindow::restart()
{
    ellapsedSeconds = 0;
    ui->progressBar->reset();
    ui->progressBar->setValue(ui->progressBar->value()+1);
}

void MainWindow::setPlayerVolume(int volume)
{
    QString lblString;
    lblString.sprintf("%d %%", volume);
    ui->lblVolume->setText(lblString);

    qreal linearVolume = QAudio::convertVolume(volume / qreal(100.0), QAudio::LogarithmicVolumeScale, QAudio::LinearVolumeScale);
    player->setVolume(qRound(linearVolume * 100));
}
