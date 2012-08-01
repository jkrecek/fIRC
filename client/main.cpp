#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QTextCodec>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setApplicationName("tcpclient");

	{
		QTextCodec* unicode = QTextCodec::codecForName("UTF-8");
		QTextCodec::setCodecForCStrings(unicode);
		QTextCodec::setCodecForTr(unicode);
	}

	QTranslator tr;
	tr.load(a.applicationName() + "_" + QLocale::system().name());
	a.installTranslator(&tr);

	MainWindow w;
    w.setWindowTitle("Client");
	w.show();

	return a.exec();
}
