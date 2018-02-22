#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <QWidget>
#include <QDir>
#include <QWindow>
#include <QScreen>
#if defined(WIN32) || defined(WIN64)
#include <windows.h>
#endif
#include <QThread>
#include <QPointer>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QSettings>
#include "vlcplayer.h"
#define defStepNewBlock 100

namespace Ui {
class Dispatcher;
}

class Dispatcher : public QWidget
{
    Q_OBJECT

public:
    explicit Dispatcher(QWidget *parent = 0);
    ~Dispatcher();
public slots:
   void slotHideScreenSaver();
private:
    Ui::Dispatcher *ui;
    QPointer<vlcPlayer> _vlcPlayer = nullptr;

    QNetworkAccessManager * nam;
    int idNewBlockTimer {0};
    int idStopAddingBlock {0};
    int idStartWait {0};
    QSettings settings;
    void sendReply();
    qint64 iTimeNewBlock {0};
    void showNewBlockSaverScreen();
    void createPlayer();
    QTime timeStart;
protected:
    void timerEvent(QTimerEvent *event);
private slots:
    void slotClickStart();
    void slotReplyFinished(QNetworkReply *reply);
    void slotReplyErrored(QNetworkReply::NetworkError code);
    void slotSslErrors(QNetworkReply *reply, const QList<QSslError> &errors);
    void slotFindFileAddNewBlock();
    void slotFindFileNoBlock();
};


#endif // DISPATCHER_H
