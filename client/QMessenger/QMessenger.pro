DEFINES += CLIENT

QMAKE_CXXFLAGS += -std=c++11

#http://forum.qt.io/topic/8996/solved-linking-to-some-functions-of-windows-sockets-api/2
win32:LIBS += -lsetupapi -lws2_32

QT += widgets network testlib

SOURCES += \
    main.cpp \
    ConnectionWindow.cpp \
    ../../parser.cpp \
    ../../User.cpp \
    RoomWindow.cpp \
    MainWindow.cpp \
    Socket.cpp \
    Matrix.cpp \
    FriendshipWindow.cpp \
    JoinRoom.cpp

HEADERS += \
    ConnectionWindow.h \
    ../../parser.h \
    ../../parserConst.h \
    ../../User.h \
    RoomWindow.h \
    MainWindow.h \
    Socket.h \
    Matrix.h \
    FriendshipWindow.h \
    JoinRoom.h
