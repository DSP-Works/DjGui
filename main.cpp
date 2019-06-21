#include <QApplication>

#include "widgets/mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow mainWindow;
    mainWindow.resize(1366, 768);
    mainWindow.show();

    return a.exec();
}
