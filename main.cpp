
#include "dispatcher.h"

#include <QApplication>
#include <QMetaEnum>
#include "eventsdebugger.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    EventsDebugger evDebug;
    app.installEventFilter(&evDebug);
    Dispatcher dispatcher;
    dispatcher.show();
    app.connect(&evDebug, &EventsDebugger::closeScreenSaver,
                &dispatcher, &Dispatcher::slotHideScreenSaver);
    return app.exec();
}
