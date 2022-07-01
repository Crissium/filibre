#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSettings>
#include <QDebug>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, currentlySelectedFilm(nullptr)
	, ui(new Ui::MainWindow)
{
	ui->setupUi(this);

	readSettings();

	createActions();
	createMenus();

	createLeft();
	createCollectionView();
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

	openPosterAction = new QAction("Open &poster", this);
	connect(openPosterAction, &QAction::triggered, this, [this] ()
	{QDesktopServices::openUrl(QUrl::fromLocalFile(this->currentlySelectedFilm->attributes.at(Film::NamesAttributes[Film::PosterPath]).c_str()));});


	for (size_t i = 0; i < Film::NumAttributes; ++i)
	{
		sortActions[i] = new QAction(Film::NamesAttributes[i].c_str(), this);
		connect(sortActions[i], &QAction::triggered, this, [this, i] ()
		{
			this->currentlyDisplayedFilms.sort([i] (const Film * lhs, const Film * rhs)
			{return lhs->attributes.at(Film::NamesAttributes[i]) < rhs->attributes.at(Film::NamesAttributes[i]);});
		});
	}

	aboutAction = new QAction("About filibre");
	connect(aboutAction, &QAction::triggered, this, [this] () {QMessageBox::about(this, "About filibre", "Filibre is a calibre-like film management tool.\nAuthors:\nXing Yi\nLin Duoming\nZhao Kehan");});

	aboutQtAction = new QAction("About Qt");
	connect(aboutQtAction, &QAction::triggered, this, [this] () {QMessageBox::aboutQt(this, "About Qt");});
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
	editMenu->addAction(openPosterAction);

	sortMenu = menuBar()->addMenu("&Sort by");
	for (size_t i = 0; i < Film::NumAttributes; ++i)
	{
		sortMenu->addAction(sortActions[i]);
	}

	helpMenu = menuBar()->addMenu("&Help");
	helpMenu->addAction(aboutAction);
	helpMenu->addAction(aboutQtAction);
}

void MainWindow::createLeft()
{
	leftList = new QStandardItemModel;
	Director = leftList->invisibleRootItem();
	for (size_t attr = Film::Title; attr < Film::NumAttributes; ++attr)
	{
		if (attr == Film::Path || attr == Film::PosterPath || attr == Film::Description || attr == Film::Synopsis)
			continue;
		QStandardItem * thisAttr = new QStandardItem;
		thisAttr->setText(tr(Film::NamesAttributes[attr].c_str()));
		std::set<std::string> const typeAttr = collection->allValuesOfAttribute(attr);
		for(auto const & typical : typeAttr)
		{
			QStandardItem * thisType = new QStandardItem;
			thisType->setText(tr(typical.c_str()));
			thisAttr->appendRow(thisType);
		}
		Director->appendRow(thisAttr);
	}
	ui->leftNavigationTree->setModel(leftList);
	ui->leftNavigationTree->setMinimumWidth(280);
	ui->leftNavigationTree->setMaximumWidth(280);
}

void MainWindow::createCollectionView()
{
	scene = new QGraphicsScene(ui->collectionView);

	display = new CollectionDisplay(currentlyDisplayedFilms, size().width() - 500, scene);
	scene->addItem(display);
	ui->collectionView->setScene(scene);
	ui->collectionView->setMinimumSize(QSize(size().width() - 500, size().height()));
	ui->collectionView->show();
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
			QMessageBox msgBox(this);
			msgBox.setText("Please select a valid collection!");
			msgBox.exec();
		}
	}
}
