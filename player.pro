TEMPLATE = app
TARGET = player

QT += network \
      xml \
      multimedia \
      multimediawidgets \
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

LIBS       += -lVLCQtCore -lVLCQtWidgets
