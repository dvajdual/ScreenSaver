TEMPLATE = app
TARGET = ScreenSaver
CONFIG += C++11
QT += network \
#      xml \
      multimedia \
#      multimediawidgets \
      widgets

HEADERS = \
    dispatcher.h \
    vlcplayer.h \
    eventsdebugger.h
SOURCES = main.cpp \
    dispatcher.cpp \
    vlcplayer.cpp

FORMS += \
    dispatcher.ui \
    vlcplayer.ui

install.path = $$OUT_PWD
install.files += *.mov
INSTALLS +=install

macx{
LIBS += -F$$PWD/../../Qt/5.5/clang_64/lib/ -framework VLCQtCore

INCLUDEPATH += $$PWD/../../Qt/5.5/clang_64/include
DEPENDPATH += $$PWD/../../Qt/5.5/clang_64/include
LIBS += -F$$PWD/../../Qt/5.5/clang_64/lib/ -framework VLCQtWidgets

INCLUDEPATH += $$PWD/../../Qt/5.5/clang_64/include
DEPENDPATH += $$PWD/../../Qt/5.5/clang_64/include
}

win32{
LIBS       += -lVLCQtCore -lVLCQtWidgets
}
