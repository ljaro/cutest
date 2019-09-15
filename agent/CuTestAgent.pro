QT       += gui core widgets quick testlib
CONFIG += c++11
TARGET = TestRobotAgent
TEMPLATE = lib

QMAKE_LFLAGS += -rdynamic # to make backtrace_symbols works correctly

DEFINES += TESTROBOTAGENT_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += \
$$[QT_INSTALL_HEADERS]/QtQml/$$[QT_VERSION]/QtQml \
$$[QT_INSTALL_HEADERS]/QtQml/$$[QT_VERSION] \
$$[QT_INSTALL_HEADERS]/QtCore/$$[QT_VERSION]/QtCore \
$$[QT_INSTALL_HEADERS]/QtCore/$$[QT_VERSION]

SOURCES += agent.cpp \
    actionresult.cpp \
    actions/click.cpp \
    actions/findsibling.cpp \
    actions/get.cpp \
    actions/highlight.cpp \
    actions/hitkey.cpp \
    actions/invoke.cpp \
    actions/modelset.cpp \
    actions/modelvalue.cpp \
    actions/mousemove.cpp \
    actions/mousepress.cpp \
    actions/mouserelease.cpp \
    actions/next.cpp \
    actions/null.cpp \
    actions/rolenames.cpp \
    actions/set.cpp \
    actions/tree.cpp \
    actions/waitfor.cpp \
    actions/waitforcheck.cpp \
    actions/waitforcheckchild.cpp \
    actions/waitforchild.cpp \
    actions/write.cpp \
    hook.cpp \
    spyspawner.cpp \
    spy.cpp \
    cache.cpp \
    inspector.cpp \
    testaction.cpp \
    testobject.cpp \
    dispatcher.cpp \
    param.cpp \
    params/paramexpr.cpp \
    params/paramprops.cpp \
    cachelookup.cpp \
    remotehandler.cpp \
    config.cpp \
    tcpserver.cpp \
    tcpclient.cpp \
    remoteclient.cpp \
    client.cpp \
    overlaypainter.cpp \
    testactionstatus.cpp

HEADERS += agent.h\
    actionresult.h \
    actions/click.h \
    actions/findsibling.h \
    actions/get.h \
    actions/highlight.h \
    actions/hitkey.h \
    actions/invoke.h \
    actions/modelset.h \
    actions/modelvalue.h \
    actions/mousemove.h \
    actions/mousepress.h \
    actions/mouserelease.h \
    actions/nextaction.h \
    actions/rolenames.h \
    actions/set.h \
    actions/tree.h \
    actions/waitfor.h \
    actions/waitforcheck.h \
    actions/waitforcheckchild.h \
    actions/waitforchild.h \
    actions/write.h \
    response.h \
    testrobotagent_global.h \
    hook.h \
    spyspawner.h \
    spy.h \
    cache.h \
    inspector.h \
    testaction.h \
    testobject.h \
    dispatcher.h \
    param.h \
    context.h \
    params/paramexpr.h \
    params/paramprops.h \
    cachelookup.h \
    remotehandler.h \
    config.h \
    tcpserver.h \
    tcpclient.h \
    remoteclient.h \
    client.h \
    actions/nullaction.h \
    overlaypainter.h \
    testactionstatus.h


unix {
    target.path = /usr/lib
    INSTALLS += target
}
