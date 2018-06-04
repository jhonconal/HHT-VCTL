#include "mainwindow.h"
#include <QApplication>
#include <QStringList>
int main(int argc, char *argv[])
{
#if (QT_VERSION >= QT_VERSION_CHECK(5,6,0))
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);//Hight DPI support
#endif
    QApplication a(argc, argv);
    MainWindow w;

    w.show();

    return a.exec();
}
