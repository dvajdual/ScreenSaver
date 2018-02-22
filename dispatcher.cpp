#include "dispatcher.h"
#include "ui_dispatcher.h"
#include <QDesktopWidget>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFileDialog>
#include <QMetaEnum>
Dispatcher::Dispatcher(QWidget *parent) :
    QWidget(parent),
    settings("shmuleyStudio", "ScreenSaver"),
    ui(new Ui::Dispatcher)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_DontShowOnScreen);
    //connect signals/slots
    {
        connect(ui->pushButtonStart, &QPushButton::clicked,
                this, &Dispatcher::slotClickStart);
    }

    nam = new QNetworkAccessManager(this);
    //connect signals/slots
    {
        connect(nam, &QNetworkAccessManager::finished,
                this, &Dispatcher::slotReplyFinished);

        connect(nam, &QNetworkAccessManager::sslErrors,
                this, &Dispatcher::slotSslErrors);

        connect(ui->pushButtonAddingNewBlock, &QPushButton::clicked,
                this, &Dispatcher::slotFindFileAddNewBlock);
        connect(ui->pushButtonNoNewBlock, &QPushButton::clicked,
                this, &Dispatcher::slotFindFileNoBlock);
    }
    //load settings
    {
        iTimeNewBlock = settings.value("TimeNewBlock").toLongLong();
    }
    //Test
    slotClickStart();
    //hide();
}

void Dispatcher::sendReply()
{
    QNetworkRequest request;
    request.setUrl(QUrl("https://blockchain.info/ru/latestblock"));
    auto reply = nam->get(request);
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(slotReplyErrored(QNetworkReply::NetworkError)));
}


void Dispatcher::slotReplyFinished(QNetworkReply *reply)
{
    auto out = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(out);
    auto obj = doc.object();
    if(iTimeNewBlock != obj.value("time").toDouble())
    {
        iTimeNewBlock = obj.value("time").toDouble();
    }
    idNewBlockTimer = startTimer(defStepNewBlock);
}

void Dispatcher::slotFindFileAddNewBlock()
{
    QString file =
            QFileDialog::getOpenFileName(this, tr("Open file"),
                                         QDir::homePath(),
                                         tr("Multimedia files(*)"));
    if (file.isEmpty())
        return;

    ui->lineEditVideoAddingBlock->setText(file);
}

void Dispatcher::slotFindFileNoBlock()
{
    QString file =
            QFileDialog::getOpenFileName(this, tr("Open file"),
                                         QDir::homePath(),
                                         tr("Multimedia files(*)"));
    if (file.isEmpty())
        return;

    ui->lineEditVideoNoBlock->setText(file);

}


void Dispatcher::slotReplyErrored(QNetworkReply::NetworkError code)
{
    qDebug() <<QMetaEnum::fromType<QNetworkReply::NetworkError>().valueToKey(code);
}

void Dispatcher::slotSslErrors(QNetworkReply *reply, const QList<QSslError> &errors)
{
    foreach(auto error, errors)
    {
        qDebug() <<error.errorString();
    }
}

void Dispatcher::showNewBlockSaverScreen()
{
    createPlayer();
    _vlcPlayer->startPulse();
}

void Dispatcher::createPlayer()
{
    if(_vlcPlayer.isNull())
    {
        _vlcPlayer = new vlcPlayer(nullptr);
        _vlcPlayer->windowHandle()->setGeometry(qApp->primaryScreen()->virtualGeometry());
        _vlcPlayer->openMedia(QStringList() << ui->lineEditVideoNoBlock->text()
                              << ui->lineEditVideoAddingBlock->text());
        timeStart = QTime::currentTime();
    }
}


void Dispatcher::slotClickStart()
{
    createPlayer();
    idNewBlockTimer = startTimer(defStepNewBlock);
}

void Dispatcher::timerEvent(QTimerEvent *event)
{
    if(idNewBlockTimer == event->timerId())
    {
        sendReply();
        killTimer(idNewBlockTimer);
        idNewBlockTimer = 0;
    }
}

void Dispatcher::slotHideScreenSaver()
{
    if(!_vlcPlayer.isNull() && timeStart.msecsTo(QTime::currentTime()) > 1000)
    {
        qDebug() << "slotHideScreenSaver";
        _vlcPlayer->close();
        _vlcPlayer->deleteLater();
    }
}

Dispatcher::~Dispatcher()
{
    delete ui;
}
