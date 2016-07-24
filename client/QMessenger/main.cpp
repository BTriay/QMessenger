#include <QApplication>
#include <iterator>
#include <QMessageBox>
#include <string>

#include "MainWindow.h"
#include "RoomWindow.h"

int main(int argc, char* argv[]) {

    QApplication app(argc, argv);

    MainWindow m;
    m.show();

    return app.exec();
}

