#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QTextCodec>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    a.setApplicationName("fIRC_client");
    a.setOrganizationDomain("Frca");

    {
        QTextCodec* unicode = QTextCodec::codecForName("UTF-8");
        QTextCodec::setCodecForCStrings(unicode);
        QTextCodec::setCodecForTr(unicode);
    }

    QTranslator tr;
    tr.load(a.applicationName() + "_" + QLocale::system().name());
    a.installTranslator(&tr);

    MainWindow w;
    //w.setWindowTitle("fIRC");
    w.show();

    return a.exec();
}
