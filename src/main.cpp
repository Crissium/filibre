#include "mainwindow.h"
#include "runguard.h"

#include <QApplication>
#include <QSplashScreen>

int main(int argc, char *argv[])
{
	RunGuard guard("Magic!");
	if (!guard.tryToRun())
		return 0;

	QApplication a(argc, argv);

	QSplashScreen splash(QPixmap(":/images/splash.png"));
	splash.show();

	MainWindow w;
	w.show();
	splash.finish(&w);
	return a.exec();
}
