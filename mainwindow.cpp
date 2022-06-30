#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSettings>
#include <QDebug>
#include <QFileDialog>
#include <QDir>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, currentlySelectedFilm(nullptr)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	createActions();
	createMenus();
	//createLeftList();

	readSettings();
}

MainWindow::~MainWindow()
{
	delete collection;
	delete ui;
}

void MainWindow::createActions()
{
	newCollectionAction = new QAction("&New collection", this);
	connect(newCollectionAction, &QAction::triggered, this, &MainWindow::createNewCollection);

	openCollectionAction = new QAction("&Open collection", this);
	connect(openCollectionAction, &QAction::triggered, this, &MainWindow::openExistingCollection);

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

void MainWindow::createLeftList()
{
	leftList = new LeftList(this);
}

void MainWindow::readSettings()
{
	QSettings settings("FilibreTeam", "filibre");

	settings.beginGroup("mainwindow");
	resize(settings.value("size").toSize());
	if (settings.value("fullscreen").toBool())
		showFullScreen();
	settings.endGroup();

	settings.beginGroup("library");
	currentlyUsedCollectionXmlFilePath = settings.value("xmlpath").toString();
	if (!currentlyUsedCollectionXmlFilePath.isEmpty())
		loadCollectionFromXml();
	else
		createNewCollection(); // on first run, create new collection
	settings.endGroup();
}

void MainWindow::writeSettings()
{
	QSettings settings("FilibreTeam", "filibre");

	settings.beginGroup("mainwindow");
	settings.setValue("size", size());
	settings.setValue("fullScreen", isFullScreen());
	settings.endGroup();

	settings.beginGroup("library");
	settings.setValue("xmlpath", currentlyUsedCollectionXmlFilePath);
	settings.endGroup();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	try {
		collection->writeToXmlFile(currentlyUsedCollectionXmlFilePath.toStdString());
	}  catch (const char * msg) {
		qDebug() << "Saving to " << currentlyUsedCollectionXmlFilePath << " failed!";
		auto newFileName = QFileDialog::getSaveFileName(this, "Saving failed, please select a new location", "XML (*.xml)");
		try {
			collection->writeToXmlFile(newFileName.toStdString());
		}  catch (const char * stillFailedMsg) {
			qDebug() << "Still failed! I think this should be impossible!";
		}
		writeSettings();
		event->accept();
	}

	writeSettings();
	event->accept();
}

void MainWindow::loadCollectionFromXml()
{
	try {
		collection = new FilmCollection(currentlyUsedCollectionXmlFilePath.toStdString());
	}  catch (const char * msg) {
		qDebug() << "XML file is invalid, loading failed!\n";
		currentlyUsedCollectionXmlFilePath.clear();
		throw "Load failure!\n";
	}

	currentlyDisplayedFilms = collection->all();
}

void MainWindow::createNewCollection()
{
	currentlyUsedCollectionXmlFilePath = QFileDialog::getSaveFileName(this, "Create new collection", QDir::homePath() + "/new_collection.xml", "XML (*.xml)");

	while (currentlyUsedCollectionXmlFilePath.isEmpty())
	{
		currentlyUsedCollectionXmlFilePath = QFileDialog::getSaveFileName(this, "Create new collection", QDir::homePath() + "/new_collection.xml", "XML (*.xml)");
	}
	currentlyDisplayedFilms.clear();

	collection = new FilmCollection;
}

void MainWindow::openExistingCollection()
{
	currentlyUsedCollectionXmlFilePath = QFileDialog::getOpenFileName(this, "Select collection", QDir::homePath(), "XML (*.xml)");

	if (!currentlyUsedCollectionXmlFilePath.isEmpty())
	{
		try {
			loadCollectionFromXml();
		}  catch (const char * msg) {
			qDebug() << msg;
		}
	}
}
