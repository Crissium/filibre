#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include "filmcollection.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT
private:
	FilmCollection collection; // The only concrete info in memory, others are pointers
	FilmList currentlyDisplayedFilms;

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

private slots:


private:
	Ui::MainWindow *ui;

	QMenu * fileMenu;
	QMenu * editMenu;
	QMenu * sortMenu;
	QMenu * aboutMenu;

	QAction * newCollectionAction;
	QAction * openCollectionAction;
	QAction * addItemAction;
	QAction * exportAction;
	QAction * quitAction;

	QAction * editMetadataAction;
	QAction * deleteItemAction;
	QAction * playAction;

	QAction * sortActions[Film::NumAttributes];

	QAction * aboutAction;

	void createActions();
	void createMenus();
};
#endif // MAINWINDOW_H
