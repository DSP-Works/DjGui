QT = core gui widgets multimedia sql

CONFIG += c++17

DEFINES += QT_DEPRECATED_WARNINGS QT_DISABLE_DEPRECATED_BEFORE=0x060000

SOURCES += main.cpp \
    audiogenerator.cpp \
    audiolimiter.cpp \
    audiomixer.cpp \
    audiosource.cpp \
    audiovolumecontrol.cpp \
    databasemodel.cpp \
    streamdevice.cpp \
    widgets/browserwidget.cpp \
    widgets/decks/remixdeck.cpp \
    widgets/decks/synthesizerdeck.cpp \
    widgets/decks/trackdeck.cpp \
    widgets/mainwindow.cpp

HEADERS += \
    audiogenerator.h \
    audiolimiter.h \
    audiomixer.h \
    audiosource.h \
    audiovolumecontrol.h \
    databasemodel.h \
    streamdevice.h \
    widgets/browserwidget.h \
    widgets/decks/remixdeck.h \
    widgets/decks/synthesizerdeck.h \
    widgets/decks/trackdeck.h \
    widgets/mainwindow.h
