#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "mpvplayerdialog.h"
#include "searchdialog.h"
#include "editmetadatadialog.h"
#include "additemdialog.h"
#include <QSettings>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QDesktopServices>
#include <QUrl>
#include <algorithm>
#include <iostream>
#include <QDebug>

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
	updateInfoPanel();
	createStatusBar();

	connect(this, &MainWindow::displayListChanged, this, &MainWindow::updateCollectionView);
	connect(this, &MainWindow::collectionChanged, this, &MainWindow::updateLeftPanel);
	connect(this, &MainWindow::currentlySelectedFilmChanged, this, &MainWindow::updateInfoPanel);
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
	connect(addItemAction, &QAction::triggered, this, [this] ()
	{
		auto addDialog = new AddItemDialog(collection, this);
		addDialog->show();

		connect(addDialog, &AddItemDialog::filmAdded, this, [this] ()
		{
			emit collectionChanged();

			currentlySelectedFilm = &*(collection->rbegin());
			emit currentlySelectedFilmChanged();

			currentlyDisplayedFilms.push_front(currentlySelectedFilm);
			emit displayListChanged();
		});
	});

	showFavouritesAction = new QAction("Display &favourites");
	connect(showFavouritesAction, &QAction::triggered, this, [this] ()
	{
		currentlyDisplayedFilms = collection->favourites();
		emit displayListChanged();

		currentlySelectedFilm = nullptr;
		emit currentlySelectedFilmChanged();
	});

	searchAction = new QAction("&Search");
	connect(searchAction, &QAction::triggered, this, [this] ()
	{
		auto searchDialog = new SearchDialog(collection, this);
		searchDialog->show();
		connect(searchDialog, &SearchDialog::searchResult, this, [this] (const FilmList & l)
		{
			currentlyDisplayedFilms = l;
			emit this->displayListChanged();

			currentlySelectedFilm = nullptr;
			emit this->currentlySelectedFilmChanged();
		});
	});

	exportAction = new QAction("&Export inventory", this);
	connect(exportAction, &QAction::triggered, this, [this] ()
	{
		auto htmlFileName = QFileDialog::getSaveFileName(this, "Export collection inventory to", QDir::homePath() + "/inventory.html", "HTML (*.html)");

		if (!htmlFileName.isEmpty())
		{
			collection->writeToHtmlInventory(htmlFileName.toStdString());
		}
	});

	quitAction = new QAction("&Quit", this);
	connect(quitAction, &QAction::triggered, QApplication::instance(), &QApplication::quit);

	editMetadataAction = new QAction("&Edit metadata", this);
	connect(editMetadataAction, &QAction::triggered, this, [this] ()
	{
		auto editDialog = new EditMetadataDialog(currentlySelectedFilm, this);
		editDialog->show();
		connect(editDialog, &EditMetadataDialog::filmEdited, this, [this] (const Film f)
		{
			*currentlySelectedFilm = f;
			emit currentlySelectedFilmChanged();
		});
	});

	deleteItemAction = new QAction("&Delete item", this);
	connect(deleteItemAction, &QAction::triggered, this, [this] ()
	{
		collection->remove(*currentlySelectedFilm);
		currentlyDisplayedFilms = collection->all();

		currentlySelectedFilm = nullptr;

		emit collectionChanged();
		emit currentlySelectedFilmChanged();
		emit displayListChanged();
	});

	playAction = new QAction("&Play", this);
	connect(playAction, &QAction::triggered, this, [this] ()
	{
		auto player = new MpvPlayerDialog(currentlySelectedFilm->attributes.at(Film::NamesAttributes[Film::Path]), this);
		player->show();
	});

	externalPlayAction = new QAction("Play with &external player");
	connect(externalPlayAction, &QAction::triggered, this, [this] ()
	{
		QDesktopServices::openUrl(QUrl::fromLocalFile(this->currentlySelectedFilm->attributes.at(Film::NamesAttributes[Film::Path]).c_str()));
	});

	openPosterAction = new QAction("Open &poster", this);
	connect(openPosterAction, &QAction::triggered, this, [this] ()
	{QDesktopServices::openUrl(QUrl::fromLocalFile(this->currentlySelectedFilm->attributes.at(Film::NamesAttributes[Film::PosterPath]).c_str()));});


	for (size_t i = 0, j = 0; i < Film::NumAttributes;)
	{
		if (i == Film::Description || i == Film::Path || i == Film::PosterPath || i == Film::Synopsis)
		{
			++i;
			continue;
		}

		sortActions[j] = new QAction(Film::NamesAttributes[i].c_str(), this);
		connect(sortActions[j], &QAction::triggered, this, [this, i] ()
		{
			this->currentlyDisplayedFilms.sort([i] (const Film * lhs, const Film * rhs)
			{return lhs->attributes.at(Film::NamesAttributes[i]) < rhs->attributes.at(Film::NamesAttributes[i]);});

			emit displayListChanged();
		});
		++i, ++j;
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
	fileMenu->addAction(showFavouritesAction);
	fileMenu->addAction(searchAction);
	fileMenu->addAction(exportAction);
	fileMenu->addAction(quitAction);

	editMenu = menuBar()->addMenu("&Edit");
	editMenu->addAction(editMetadataAction);
	editMenu->addAction(deleteItemAction);
	editMenu->addAction(playAction);
	editMenu->addAction(externalPlayAction);
	editMenu->addAction(openPosterAction);

	editMenu->setDisabled(true); // disable on startup
	connect(this, &MainWindow::currentlySelectedFilmChanged, editMenu, [this] ()
	{editMenu->setDisabled(!currentlySelectedFilm);});

	sortMenu = menuBar()->addMenu("&Sort by");
	for (size_t i = 0; i < Film::NumAttributes - 4; ++i)
	{
		sortMenu->addAction(sortActions[i]);
	}

	helpMenu = menuBar()->addMenu("&Help");
	helpMenu->addAction(aboutAction);
	helpMenu->addAction(aboutQtAction);
}

void MainWindow::createStatusBar()
{
	selectionLabel = new QLabel("Selected none");
	statusBar()->addPermanentWidget(selectionLabel);

	connect(this, &MainWindow::currentlySelectedFilmChanged, this, [this] ()
	{
		currentlySelectedFilm ? (selectionLabel->setText(QString("Selected ") + currentlySelectedFilm->attributes.at(Film::NamesAttributes[Film::Title]).c_str())) : (selectionLabel->setText("Selected none"));
	});
}

void MainWindow::createLeft()
{
	leftList = new QStandardItemModel;
	Director = leftList->invisibleRootItem();
	for (size_t attr = Film::Title; attr < Film::NumAttributes; ++attr)
	{
		if (attr == Film::Title || attr == Film::Path || attr == Film::PosterPath || attr == Film::Description || attr == Film::Synopsis)
			continue;
		QStandardItem * thisAttr = new QStandardItem;
		thisAttr->setText(tr(Film::NamesAttributes[attr].c_str()));
		thisAttr->setEditable(false);
		std::set<std::string> const typeAttr = collection->allValuesOfAttribute(attr);
		for(auto const & typical : typeAttr)
		{
			QStandardItem * thisType = new QStandardItem;
			thisType->setText(tr(typical.c_str()));
			thisType->setEditable(false);
			thisAttr->appendRow(thisType);
		}
		Director->appendRow(thisAttr);
	}
	ui->leftNavigationTree->setModel(leftList);

	connect(ui->leftNavigationTree, &QTreeView::clicked, this, [this] (auto arg)
	{
		auto item = leftList->itemFromIndex(arg);

		if (std::find(Film::NamesAttributes.cbegin(), Film::NamesAttributes.cend(), item->text().toStdString()) == Film::NamesAttributes.cend())
		{
			std::cout << "Damn it" << std::endl;
			currentlySelectedFilm = nullptr;
			currentlyDisplayedFilms = collection->filmsWithAttributeValue(item->parent()->text().toStdString(), item->text().toStdString());

			emit displayListChanged();
			emit currentlySelectedFilmChanged();
		}
	});

	ui->leftNavigationTree->setMinimumWidth(280);
	ui->leftNavigationTree->setMaximumWidth(280);
}

void MainWindow::updateLeftPanel()
{
	leftList = new QStandardItemModel;
	Director = leftList->invisibleRootItem();
	for (size_t attr = Film::Title; attr < Film::NumAttributes; ++attr)
	{
		if (attr == Film::Title || attr == Film::Path || attr == Film::PosterPath || attr == Film::Description || attr == Film::Synopsis)
			continue;
		QStandardItem * thisAttr = new QStandardItem;
		thisAttr->setText(tr(Film::NamesAttributes[attr].c_str()));
		thisAttr->setEditable(false);
		std::set<std::string> const typeAttr = collection->allValuesOfAttribute(attr);
		for(auto const & typical : typeAttr)
		{
			QStandardItem * thisType = new QStandardItem;
			thisType->setText(tr(typical.c_str()));
			thisType->setEditable(false);
			thisAttr->appendRow(thisType);
		}
		Director->appendRow(thisAttr);
	}
	ui->leftNavigationTree->setModel(leftList);

	connect(ui->leftNavigationTree, &QTreeView::clicked, this, [this] (auto arg)
	{
		auto item = leftList->itemFromIndex(arg);

		if (std::find(Film::NamesAttributes.cbegin(), Film::NamesAttributes.cend(), item->text().toStdString()) == Film::NamesAttributes.cend())
		{
			std::cout << "Damn it" << std::endl;
			currentlySelectedFilm = nullptr;
			currentlyDisplayedFilms = collection->filmsWithAttributeValue(item->parent()->text().toStdString(), item->text().toStdString());

			emit displayListChanged();
			emit currentlySelectedFilmChanged();
		}
	});

	ui->leftNavigationTree->setMinimumWidth(280);
	ui->leftNavigationTree->setMaximumWidth(280);
}

void MainWindow::createCollectionView()
{
	scene = new QGraphicsScene(ui->collectionView);

	display = new CollectionDisplay(currentlyDisplayedFilms, size().width() - 500, scene);
	scene->addItem(display);

	connect(display, &CollectionDisplay::itemSelected, this, &MainWindow::updateSelectedFilm);
	connect(display, &CollectionDisplay::itemDoubleClicked, this, [this] (Film * f)
	{
		auto player = new MpvPlayerDialog(f->attributes.at(Film::NamesAttributes[Film::Path]), this);
		player->show();
	});

	ui->collectionView->setScene(scene);
	ui->collectionView->setMinimumSize(QSize(size().width() - 500, size().height()));
	ui->collectionView->show();
}

void MainWindow::updateCollectionView()
{
	scene = new QGraphicsScene(ui->collectionView);

	display = new CollectionDisplay(currentlyDisplayedFilms, size().width() - 500, scene);
	scene->addItem(display);

	connect(display, &CollectionDisplay::itemSelected, this, &MainWindow::updateSelectedFilm);
	connect(display, &CollectionDisplay::itemDoubleClicked, this, [this] (Film * f)
	{
		auto player = new MpvPlayerDialog(f->attributes.at(Film::NamesAttributes[Film::Path]), this);
		player->show();
	});

	ui->collectionView->setScene(scene);
	ui->collectionView->setMinimumSize(QSize(size().width() - 500, size().height()));
	ui->collectionView->show();
}

void MainWindow::updateSelectedFilm(Film * f)
{
	currentlySelectedFilm = f;
	emit currentlySelectedFilmChanged();
}

QString MainWindow::infoOf(const Film * f) const
{
	return f ? (QString("%1 (%2)\n\nDirector(s): %3\n\nLeading Actor(s): %4\n\nGenre: %5\n\nLanguage: %6\n\nRating: %7\n\nAccolades: %8\n\nSynopsis: %9\n\nDescription: %10")
			   .arg(f->attributes.at(Film::NamesAttributes[Film::Title]).c_str())
			.arg(f->attributes.at(Film::NamesAttributes[Film::Year]).c_str())
			.arg(f->attributes.at(Film::NamesAttributes[Film::Director]).c_str())
			.arg(f->attributes.at(Film::NamesAttributes[Film::Lead]).c_str())
			.arg(f->attributes.at(Film::NamesAttributes[Film::Genre]).c_str())
			.arg(f->attributes.at(Film::NamesAttributes[Film::Language]).c_str())
			.arg(f->attributes.at(Film::NamesAttributes[Film::Rating]).c_str())
			.arg(f->attributes.at(Film::NamesAttributes[Film::Accolades]).c_str())
			.arg(f->attributes.at(Film::NamesAttributes[Film::Synopsis]).c_str())
			.arg(f->attributes.at(Film::NamesAttributes[Film::Description]).c_str()))
			: QString("None selected.");
}

void MainWindow::updateInfoPanel()
{
	ui->infoPanel->setPlainText(infoOf(currentlySelectedFilm));
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

void MainWindow::resizeEvent(QResizeEvent * event)
{
	emit displayListChanged();
	QMainWindow::resizeEvent(event);
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
	currentlySelectedFilm = nullptr;

	emit currentlySelectedFilmChanged();
	emit displayListChanged();
	emit collectionChanged();
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

	emit currentlySelectedFilmChanged();
	emit displayListChanged();
	emit collectionChanged();
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


