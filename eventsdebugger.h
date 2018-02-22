#ifndef EVENTSDEBUGGER_H
#define EVENTSDEBUGGER_H
#include <QObject>
#include <QMetaEnum>
#include <QDebug>
#include <QEvent>
class EventsDebugger: public QObject
{
    Q_OBJECT

public:
    explicit EventsDebugger(QObject *parent = 0): QObject(parent){}
   ~EventsDebugger(){}
protected:
    bool eventFilter(QObject *obj, QEvent *event)
    {
        if(/*event->type() == QEvent::MouseMove
           ||*/
           event->type() == QEvent::KeyPress
/*           ||
           event->type() == QEvent::MouseButtonPress*/)
        {
            emit closeScreenSaver();
        }
        return QObject::eventFilter(obj, event);
    }
signals:
    void closeScreenSaver();
};
#endif // EVENTSDEBUGGER_H
