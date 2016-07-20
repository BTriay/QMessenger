#include <QApplication>
#include <iterator>
#include <QMessageBox>

#include "../../parser.h"
#include "StartWindow.h"

int main(int argc, char* argv[]) {

    QApplication app(argc, argv);

    std::string filename = "profile";
    std::vector<std::string> tokens;
    std::vector<std::string>::iterator it;
    QString username;
    QString server;

    if (configFileReader(filename, tokens)) {
        for (it = tokens.begin(); it != tokens.end(); it++) {
            if (*it == "username") {
                it++;
                username = QString::fromStdString(*it);
            }
            else if (*it == "domain") {
                it++;
                server = QString::fromStdString(*it);
            }
        }
    }
    else {
        QMessageBox::warning(NULL, "QMessenger", "There is no config file");
    }

    StartWindow w(username, server);
    w.show();

    return app.exec();
}

