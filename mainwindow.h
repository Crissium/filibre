#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QCloseEvent>
#include <QTreeView>
#include <QStandardItemModel>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLabel>
#include "filmcollection.h"
#include "collectiondisplay.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT
private:
	FilmCollection * collection; // The only concrete info in memory, others are pointers
	FilmList currentlyDisplayedFilms;
	Film * currentlySelectedFilm;
	QString currentlyUsedCollectionXmlFilePath;

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

protected:
	void closeEvent(QCloseEvent *event) override;

signals:
	void displayListChanged();
	void currentlySelectedFilmChanged();

private slots:
	void createNewCollection();
	void openExistingCollection();
	void updateCollectionView();
	void updateSelectedFilm(Film *);
	void addItem();

private:
	Ui::MainWindow *ui;

	QMenu * fileMenu;
	QMenu * editMenu;
	QMenu * sortMenu;
	QMenu * helpMenu;

	QAction * newCollectionAction;
	QAction * openCollectionAction;
	QAction * addItemAction;
	QAction * exportAction;
	QAction * quitAction;

	QAction * editMetadataAction;
	QAction * deleteItemAction;
	QAction * playAction;
	QAction * openPosterAction;

	QAction * sortActions[Film::NumAttributes];

	QAction * aboutAction;
	QAction * aboutQtAction;

	void createActions();
	void createMenus();

	QLabel * selectionLabel;
	void createStatusBar();

	QStandardItemModel * leftList;
	QStandardItem * Director;
	void createLeft();

	QGraphicsScene * scene;
	CollectionDisplay * display;
	void createCollectionView();

	void readSettings();
	void writeSettings();

	void loadCollectionFromXml();
};


#endif // MAINWINDOW_H
