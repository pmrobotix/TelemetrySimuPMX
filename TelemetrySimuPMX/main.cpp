#include <QApplication>
#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QStringList>
#ifndef QT_NO_OPENGL
#include <QGLFormat>
#endif

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Q_INIT_RESOURCE(telemetrysimupmx);

//    QCommandLineParser parser;
//    parser.setApplicationDescription("Qt SVG Viewer");
//    parser.addHelpOption();
//    parser.addVersionOption();
//    parser.addPositionalArgument("file", "The file to open.");
//    parser.process(app);

    MainWindow mainWin;
//    if (!mainWin.loadFile(parser.positionalArguments().value(0, QLatin1String(":/images/table2020.svg"))))
//        return -1;
    mainWin.show();
    return app.exec();
}
