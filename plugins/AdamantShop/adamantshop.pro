! include( ../plugin.pri ) {
    error( Couldn\'t find plugin.pri! )
}

DEFINES += ADAMANTSHOP_LIBRARY

adamantshop.depends = stashviewer core

QT += core gui widgets network printsupport
TARGET = adamant.shop

OTHER_FILES += adamantshop.json
HEADERS += adamantshopplugin.h \
    shopviewer.h \
    widgets/shopwidget.h \
    external/qcustomplot.h \
    shop/shop.h \
    external/mustache.h \
    shoptemplate.h \
    templateviewer.h \
    dialogs/pricedialog.h

win32: LIBS += -L$$OUT_PWD/../../bin/ -ladamant
else:unix: LIBS += -L$$OUT_PWD/../../bin/ -ladamant

INCLUDEPATH += $$PWD/../../core
DEPENDPATH += $$PWD/../../core

win32: LIBS += -L$$OUT_PWD/../../bin/plugins/ -ladamant.stashviewer
else:unix: LIBS += -L$$OUT_PWD/../../bin/plugins/ ladamant.stashviewer

INCLUDEPATH += $$PWD/../../plugins/StashViewer
DEPENDPATH += $$PWD/../../plugins/StashViewer

FORMS += \
    shopviewer.ui \
    widgets/shopwidget.ui \
    templateviewer.ui \
    dialogs/pricedialog.ui

SOURCES += \
    shopviewer.cpp \
    widgets/shopwidget.cpp \
    external/qcustomplot.cpp \
    adamantshopplugin.cpp \
    shop/shop.cpp \
    templateviewer.cpp \
    dialogs/pricedialog.cpp \
    external/mustache.cpp
