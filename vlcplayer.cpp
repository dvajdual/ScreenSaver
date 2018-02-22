#include "vlcplayer.h"
#include "ui_vlcplayer.h"
#include <windows.h>
#include <QDesktopWidget>
#include <QMetaEnum>
#include <QDebug>
#include <QMouseEvent>
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
    showMaximized();
    setCursor(Qt::BlankCursor);
    setMouseTracking(true);
    QRect rec = QApplication::desktop()->screenGeometry();
    auto height = rec.height();
    auto width = rec.width();
    this->setMaximumHeight(height);
    this->setMinimumHeight(height);
    this->setMaximumWidth(width);
    this->setMinimumWidth(width);

    {
        HWND hwnd = FindWindow(L"Shell_traywnd", L"");
        SetWindowPos(hwnd,0,0,0,0,0,SWP_HIDEWINDOW);
    }
    {
        HWND hwnd = FindWindow(L"Shell_SecondaryTrayWnd", L"");
        SetWindowPos(hwnd,0,0,0,0,0,SWP_HIDEWINDOW);
    }
    connect(_player, &VlcMediaPlayer::end, this, &vlcPlayer::slotEnded);
    setFocusPolicy(Qt::WheelFocus);
    //ui->VlcWidget->installEventFilter(this);
    ui->VlcWidget->_video->installEventFilter(this);
}

bool vlcPlayer::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type() == QEvent::FocusIn
            ||
       event->type() == QEvent::FocusOut)
    {
        qDebug() << QMetaEnum::fromType<QEvent::Type>().valueToKey(event->type());
    }
    qDebug() << QMetaEnum::fromType<QEvent::Type>().valueToKey(event->type());
    return QWidget::eventFilter(watched, event);
}


void vlcPlayer::startPulse()
{
    _player->open(_media1);
}

void vlcPlayer::slotEnded()
{
    _player->open(_media);
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
        POINT pt;
        HWND hwnd;
        GetCursorPos(&pt);
            //получаем дескриптор окна
        hwnd = WindowFromPoint(pt);

        SetFocus(hwnd);
        killTimer(idFocusTimer);
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
    {
        HWND hwnd = FindWindow(L"Shell_traywnd", L"");
        if(hwnd)
            SetWindowPos(hwnd,0,0,0,0,0,SWP_SHOWWINDOW);
    }
    {
        HWND hwnd = FindWindow(L"Shell_SecondaryTrayWnd", L"");
        if(hwnd)
            SetWindowPos(hwnd,0,0,0,0,0,SWP_SHOWWINDOW);
    }
    delete ui;
}
