
#include "dispatcher.h"

#include <QApplication>
#include <QMetaEnum>
#include "eventsdebugger.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    EventsDebugger evDebug;
    app.installEventFilter(&evDebug);

    //remove current screen saver
    {
        auto regPath = "\\HKEY_CURRENT_USER\\Control Panel\\Desktop";
        QSettings settings_registr(regPath, QSettings::NativeFormat);
        settings_registr.remove("SCRNSAVE.EXE");
        settings_registr.setValue("ScreenSaveActive", 0);
        settings_registr.sync();
    }

    Dispatcher dispatcher;
    dispatcher.show();
    app.connect(&evDebug, &EventsDebugger::closeScreenSaver,
                &dispatcher, &Dispatcher::slotHideScreenSaver);
    return app.exec();
}
