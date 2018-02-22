
#include "player.h"
#include "videowidget.h"

#include <QMediaService>
#include <QMediaPlaylist>
#include <QtWidgets>
#include <windows.h>


Player::Player(QWidget *parent)
    : QWidget(parent)
{
    m_player = new QMediaPlayer(this);
    m_player->setAudioRole(QAudio::VideoRole);
    qInfo() << "Supported audio roles:";
    for (QAudio::Role role : m_player->supportedAudioRoles())
        qInfo() << "    " << role;
    // owned by PlaylistModel
    m_playlist = new QMediaPlaylist();
    m_player->setPlaylist(m_playlist);
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

    m_videoWidget = new VideoWidget(this);
    m_player->setVideoOutput(m_videoWidget);
    m_videoWidget->setMouseTracking(true);
    m_videoWidget->installEventFilter(this);

    QBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(m_videoWidget);
    layout->setSpacing(0);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);
    timeStart = QTime::currentTime();

    {
        HWND hwnd = FindWindow(L"Shell_traywnd", L"");
        if(hwnd)
            SetWindowPos(hwnd,0,0,0,0,0,SWP_HIDEWINDOW);
    }
    {
        HWND hwnd = FindWindow(L"Shell_SecondaryTrayWnd", L"");
        if(hwnd)
            SetWindowPos(hwnd,0,0,0,0,0,SWP_HIDEWINDOW);
    }

//    connect(m_playlist, &QMediaPlaylist::currentIndexChanged,
//            this, &Player::indexChanged);
//    connect(m_player, &QMediaPlayer::positionChanged,
//            this, &Player::slotPositionChanged);
}

void Player::slotPositionChanged(int pos)
{
    if(pos >= m_player->duration()-1
            &&
       m_player->playlist()->currentIndex() == 0)
    {
        m_player->playlist()->setCurrentIndex(1);
        m_player->play();
        qWarning("setPosition(0)");
    }
    if(pos >= m_player->duration()-100
            &&
       m_player->playlist()->currentIndex() == 1)
    {
        m_player->playlist()->setCurrentIndex(0);
        m_player->play();
        qWarning("setPosition(1)");
    }
}

void Player::play(int index)
{
    m_player->playlist()->setCurrentIndex(index);
    m_player->play();
}

//bool Player::eventFilter(QObject *obj, QEvent *event)
//{
//    qDebug() <<QMetaEnum::fromType<QEvent::Type>().valueToKey(event->type());
//    if(event->type() == QEvent::MouseMove
//           &&
//       timeStart.msecsTo(QTime::currentTime()) > 1000)
//    {
//        close();
//        deleteLater();
//        return true;
//    }
//    return QWidget::eventFilter(obj, event);
//}

void Player::addToPlaylist(const QList<QUrl> &urls)
{
    foreach(auto url, urls)
    {
        m_playlist->addMedia(url);
    }
    //m_playlist->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
    m_player->setPlaylist(m_playlist);
}


void Player::keyPressEvent(QKeyEvent *event)
{
    if(timeStart.msecsTo(QTime::currentTime()) > 1000)
    {
        close();
        deleteLater();
    }
    QWidget::keyPressEvent(event);
}

//void Player::mouseMoveEvent(QMouseEvent *event)
//{
//    if(timeStart.msecsTo(QTime::currentTime()) > 1000)
//    {
//        close();
//        deleteLater();
//    }
//    QWidget::mouseMoveEvent(event);
//}

//void Player::mousePressEvent(QMouseEvent *event)
//{
//    if(timeStart.msecsTo(QTime::currentTime()) > 1000)
//    {
//        close();
//        deleteLater();
//    }
//    QWidget::mousePressEvent(event);
//}

void Player::closeEvent(QCloseEvent *event)
{
    emit closing();
    QWidget::closeEvent(event);
}

Player::~Player()
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
}
