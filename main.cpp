#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QtCore>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTranslator myappTranslator;
    QTranslator qtTranslator;

    qtTranslator.load (QString ("qt_%1").arg (QLocale::system().name()),
               QLibraryInfo::location (QLibraryInfo::TranslationsPath));
    app.installTranslator (&qtTranslator);
    myappTranslator.load (":/main_" + QLocale::system().name());
    app.installTranslator (&myappTranslator);

    MainWindow w;
    w.show();
    return app.exec();
}
