QMAKE_CXXFLAGS += -std=c++11

QT += widgets network testlib

SOURCES += \
    main.cpp \
    ConnectionWindow.cpp \
    ../../parser.cpp \
    RoomWindow.cpp \
    MainWindow.cpp

HEADERS += \
    ConnectionWindow.h \
    ../../parser.h \
    RoomWindow.h \
    MainWindow.h
