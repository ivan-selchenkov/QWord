#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QtCore>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("Crazy Rabbit");
    QCoreApplication::setOrganizationDomain("crazyrabbit.com");
    QCoreApplication::setApplicationName("QWord");

    QTranslator myappTranslator;
    QTranslator qtTranslator;

    qtTranslator.load (QString ("qt_%1").arg (QLocale::system().name()),
               QLibraryInfo::location (QLibraryInfo::TranslationsPath));
    app.installTranslator (&qtTranslator);
    myappTranslator.load (":/main_" + QLocale::system().name());
    app.installTranslator (&myappTranslator);

    MainWindow w;

    QSettings settings;
    int x, y;
    if(settings.contains("X") && settings.contains("Y"))
    {
        x = settings.value("X").toInt();
        y = settings.value("Y").toInt();
        //w.move(x,y);
        w.setGeometry(x,y,w.width(), w.height());
    }
    Qt::WindowFlags flags = w.windowFlags();
    flags |= Qt::WindowStaysOnTopHint;
    w.setWindowFlags(flags);
    w.show();
    return app.exec();
}
