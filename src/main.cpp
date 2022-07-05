#include "mainwindow.h"
#include "runguard.h"
#include <ctime>
#include <QApplication>
#include <QSplashScreen>

int main(int argc, char *argv[])
{
	RunGuard guard("Magic!");
	if (!guard.tryToRun())
		return 0;

	QApplication a(argc, argv);

	time_t start = time(nullptr);

	QSplashScreen splash(QPixmap(":/images/splash.png"));
	splash.show();

	while (time(nullptr) - start < 1);

	MainWindow w;
	w.show();
	splash.finish(&w);
	return a.exec();
}
