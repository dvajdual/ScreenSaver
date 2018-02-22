#ifndef VLCPLAYER_H
#define VLCPLAYER_H

#include <QWidget>
#include <VLCQtCore/Common.h>
#include <VLCQtCore/Instance.h>
#include <VLCQtCore/Media.h>
#include <VLCQtCore/MediaPlayer.h>
#include <QFile>
#include <QTime>

namespace Ui {
class vlcPlayer;
}

class vlcPlayer : public QWidget
{
    Q_OBJECT

public:
    explicit vlcPlayer(QWidget *parent = 0);
    ~vlcPlayer();
    void openMedia(QStringList files);
    void startPulse();
    int idFocusTimer{0};
private:
    Ui::vlcPlayer *ui;
    VlcInstance *_instance;
    VlcMedia *_media;
    VlcMedia *_media1;
    QFile * file;
    QFile * file1;
    VlcMediaPlayer *_player;
    int currentMedia = 0;
    int repeat = 0;
protected:
    bool eventFilter(QObject *watched, QEvent *event);
    void timerEvent(QTimerEvent *event);
private slots:
    void slotEnded();
};

#endif // VLCPLAYER_H
