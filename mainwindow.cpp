#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	createActions();
	createMenus();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::createActions()
{
	newCollectionAction = new QAction("&New collection", this);

	openCollectionAction = new QAction("&Open collection", this);

	addItemAction = new QAction("&Add item", this);

	exportAction = new QAction("&Export collection", this);

	quitAction = new QAction("&Quit", this);
	connect(quitAction, &QAction::triggered, QApplication::instance(), &QApplication::quit);

	editMetadataAction = new QAction("&Edit metadata", this);

	deleteItemAction = new QAction("&Delete item", this);

	playAction = new QAction("&Play", this);

	for (size_t i = 0; i < Film::NumAttributes; ++i)
	{
		sortActions[i] = new QAction(Film::NamesAttributes[i].c_str(), this);
	}

	aboutAction = new QAction("&About filibre");
}

void MainWindow::createMenus()
{
	fileMenu = menuBar()->addMenu("&File");
	fileMenu->addAction(newCollectionAction);
	fileMenu->addAction(openCollectionAction);
	fileMenu->addAction(addItemAction);
	fileMenu->addAction(exportAction);
	fileMenu->addAction(quitAction);

	editMenu = menuBar()->addMenu("&Edit");
	editMenu->addAction(editMetadataAction);
	editMenu->addAction(deleteItemAction);
	editMenu->addAction(playAction);

	sortMenu = menuBar()->addMenu("&Sort by");
	for (size_t i = 0; i < Film::NumAttributes; ++i)
	{
		sortMenu->addAction(sortActions[i]);
	}

	aboutMenu = menuBar()->addMenu("&About");
	aboutMenu->addAction(aboutAction);
}
