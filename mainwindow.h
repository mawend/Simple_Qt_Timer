#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QTime>
#include <QMediaPlayer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QTimer *timer;
    QTime startTime;
    QTime *zeroTime;
    QSystemTrayIcon *trayIcon;
    QMediaPlayer *player;

    int ellapsedSeconds;
    bool isPaused;


private slots:
    void updateTimer();
    void start();
    void stop();
    void pause();
    void restart();
    void setPlayerVolume(int volume);
    void setAlwaysOnTop(int value);
};
#endif // MAINWINDOW_H
