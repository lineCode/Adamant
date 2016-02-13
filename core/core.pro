DESTDIR       = ../bin

TEMPLATE = lib
CONFIG += qt c++14
QT += core network gui widgets script scripttools
TARGET = adamant

DEFINES += CORE_INTERNAL

HEADERS += \
    core.h \
    interfaces/adamantplugin.h \
    ui/commandbutton.h \
    ui/mainwindow.h \
    ui/setupdialog.h \
    ui/ui.h \
    session/session.h \
    scripting/scriptsandbox.h \
    adamantplugininfo.h \
    pluginmanager.h \
    core_global.h \
    session/imagecache.h \
    items/itemmanager.h \
    items/itemlocation.h \
    items/stashitemlocation.h \
    items/item.h \
    ui/pages/pluginpage.h \
    ui/widgets/pluginitem.h \
    ui/widgets/pluginitemdelegate.h \
    session/sessionrequest.h \
    items/characteritemlocation.h

SOURCES += \
    core.cpp \
    ui/mainwindow.cpp \
    ui/setupdialog.cpp \
    ui/ui.cpp \
    session/session.cpp \
    scripting/scriptsandbox.cpp \
    pluginmanager.cpp \
    session/imagecache.cpp \
    items/itemmanager.cpp \
    items/stashitemlocation.cpp \
    items/item.cpp \
    ui/pages/pluginpage.cpp \
    ui/widgets/pluginitem.cpp \
    ui/widgets/pluginitemdelegate.cpp \
    session/sessionrequest.cpp \
    items/characteritemlocation.cpp

RESOURCES += \
    ui/res.qrc

FORMS += \
    ui/mainwindow.ui \
    ui/setupdialog.ui \
    ui/pages/pluginpage.ui \
    ui/widgets/pluginitem.ui
