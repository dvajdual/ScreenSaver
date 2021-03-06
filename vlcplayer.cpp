#include "vlcplayer.h"
#include "ui_vlcplayer.h"
#if defined(WIN32) || defined(WIN64)
#include <windows.h>
#endif
#include <QDesktopWidget>
#include <QMetaEnum>
#include <QDebug>
#include <QMouseEvent>
#include <QProcess>
vlcPlayer::vlcPlayer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::vlcPlayer)
{
    ui->setupUi(this);
    _instance = new VlcInstance(VlcCommon::args(), this);
    _player = new VlcMediaPlayer(_instance);
    _player->setVideoWidget(ui->VlcWidget);
    ui->VlcWidget->setMediaPlayer(_player);
    ui->VlcWidget->setMouseTracking(true);

    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
#if defined(WIN32) || defined(WIN64)
    showMaximized();
#endif
#ifdef __APPLE__
    showFullScreen();
#endif
    setCursor(Qt::BlankCursor);
    setMouseTracking(true);
    QRect rec = QApplication::desktop()->screenGeometry();
    auto height = rec.height();
    auto width = rec.width();
    this->setMaximumHeight(height);
    this->setMinimumHeight(height);
    this->setMaximumWidth(width);
    this->setMinimumWidth(width);
#if defined(WIN32) || defined(WIN64)
    {
        HWND hwnd = FindWindow(L"Shell_traywnd", L"");
        SetWindowPos(hwnd,0,0,0,0,0,SWP_HIDEWINDOW);
        HWND hwndButton = FindWindowEx(hwnd, 0, L"Button", L"");
        if(hwndButton)
            SetWindowPos(hwndButton,0,0,0,0,0,SWP_HIDEWINDOW);
    }
    {
        HWND hwnd = FindWindow(L"Shell_SecondaryTrayWnd", L"");
        SetWindowPos(hwnd,0,0,0,0,0,SWP_HIDEWINDOW);
        HWND hwndButton = FindWindowEx(hwnd, 0, L"Button", L"");
        if(hwndButton)
            SetWindowPos(hwndButton,0,0,0,0,0,SWP_HIDEWINDOW);
    }
#endif
#ifdef __APPLE__
    QProcess proc;
    QString str_skript = "defaults write com.apple.dock autohide -bool true && defaults write com.apple.dock autohide-delay -float 0 && defaults write com.apple.dock autohide-time-modifier -float 0 && killall Dock";
    proc.start("sh", QStringList() << "-c" << str_skript);
#endif
    connect(_player, &VlcMediaPlayer::end, this, &vlcPlayer::slotEnded);
    setFocusPolicy(Qt::WheelFocus);
}

void vlcPlayer::startPulse()
{
    bPulse = true;
}

void vlcPlayer::slotEnded()
{
    _player->open(_media);
    if(bPulse)
    {
        _player->open(_media1);
        bPulse = false;
    }
#if 0
    if(currentMedia == 0)
    {
        if(repeat == 1)
        {
            _player->open(_media1);
            currentMedia = 1;
            repeat = 0;
        }
        else
        {
            repeat = 1;
            _player->open(_media);
        }
    }
    else
    {
        _player->open(_media);
        currentMedia = 0;
    }
#endif
}


void vlcPlayer::timerEvent(QTimerEvent *event)
{
    if(idFocusTimer == event->timerId())
    {
#if defined(WIN32) || defined(WIN64)
        POINT pt;
        HWND hwnd;
        GetCursorPos(&pt);
            //получаем дескриптор окна
        hwnd = WindowFromPoint(pt);

        SetFocus(hwnd);
        //killTimer(idFocusTimer);
#endif
    }
}

void vlcPlayer::openMedia(QStringList files)
{
    file = new QFile(files[0]);
    file->open(QIODevice::ReadOnly);
    auto mmap = file->map(0, file->size());
    qDebug() <<"vlcPlayer::openMedia " << mmap;
    _media = new VlcMedia(files[0], true, _instance);
    _media1 = new VlcMedia(files[1], true, _instance);
    _player->open(_media);
    idFocusTimer = startTimer(100);
}

vlcPlayer::~vlcPlayer()
{
#if defined(WIN32) || defined(WIN64)
    {
        HWND hwnd = FindWindow(L"Shell_traywnd", L"");
        if(hwnd)
            SetWindowPos(hwnd,0,0,0,0,0,SWP_SHOWWINDOW);
        HWND hwndButton = FindWindowEx(hwnd, 0, L"Button", L"");
        if(hwndButton)
            SetWindowPos(hwndButton,0,0,0,0,0,SWP_SHOWWINDOW);
    }
    {
        HWND hwnd = FindWindow(L"Shell_SecondaryTrayWnd", L"");
        if(hwnd)
            SetWindowPos(hwnd,0,0,0,0,0,SWP_SHOWWINDOW);
        HWND hwndButton = FindWindowEx(hwnd, 0, L"Button", L"");
        if(hwndButton)
            SetWindowPos(hwndButton,0,0,0,0,0,SWP_SHOWWINDOW);
    }
#endif
#ifdef __APPLE__
    QProcess proc;
    QString str_skript = "defaults delete com.apple.dock autohide && defaults delete com.apple.dock autohide-delay && defaults delete com.apple.dock autohide-time-modifier && killall Dock";
    proc.start("sh", QStringList() << "-c" << str_skript);
    proc.waitForFinished(10*60*1000);
#endif
    delete ui;
}
