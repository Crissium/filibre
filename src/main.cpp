#include "mainwindow.h"
#include "runguard.h"
#include <QApplication>

int main(int argc, char *argv[])
{
	RunGuard guard(RunGuard::RandomKey());
	if (!guard.tryToRun())
		return 0;

	QApplication a(argc, argv);
	MainWindow w;
	w.show();
	return a.exec();
}
