TEMPLATE = app
TARGET = QT_UI

QT = core gui

INCLUDEPATH =  /usr/local/include ../../TMM_CTRL/src $INCLUDEPATH

LIBS += -L"/usr/local/lib" -lboost_program_options

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

SOURCES += \
    main.cpp \
    radio.cpp \
    shoutdown.cpp \
    domain.cpp \
    constants.cpp \
    ../../TMM_CTRL/src/media_source.cpp \
    ../../TMM_CTRL/src/multicast_group.cpp \
    ../../TMM_CTRL/src/TMM_StreamCtrl.cpp

HEADERS += \
    radio.h \
    global_defs.h \
    sharedmemory.h \
    shoutdown.h \
    domain.h \
    ../../TMM_CTRL/src/media_source.h \
    ../../TMM_CTRL/src/multicast_group.h \
    ../../TMM_CTRL/src/TMM_StreamCtrl.h
