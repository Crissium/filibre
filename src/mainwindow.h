#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QCloseEvent>
#include <QResizeEvent>
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
	void resizeEvent(QResizeEvent * event) override;

signals:
	void collectionChanged();
	void displayListChanged();
	void currentlySelectedFilmChanged();

private slots:
	void createNewCollection();
	void openExistingCollection();
	void updateLeftPanel();
	void updateCollectionView();
	void updateSelectedFilm(Film *);
	void updateInfoPanel();

private:
	Ui::MainWindow *ui;

	QMenu * fileMenu;
	QMenu * editMenu;
	QMenu * sortMenu;
	QMenu * helpMenu;

	QAction * newCollectionAction;
	QAction * openCollectionAction;
	QAction * addItemAction;
	QAction * showFavouritesAction;
	QAction * searchAction;
	QAction * exportHtmlAction;
	QAction * exportCsvAction;
	QAction * quitAction;

	QAction * editMetadataAction;
	QAction * deleteItemAction;
	QAction * playAction;
	QAction * externalPlayAction;
	QAction * openPosterAction;

	QAction * sortActions[Film::NumAttributes - 4];

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

	QString infoOf(const Film *) const;

	void readSettings();
	void writeSettings();

	void loadCollectionFromXml();
};


#endif // MAINWINDOW_H
