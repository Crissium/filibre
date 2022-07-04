#include "additemdialog.h"
#include "ui_additemdialog.h"
#include <QFileDialog>

AddItemDialog::AddItemDialog(FilmCollection * collection, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::AddItemDialog)
{
	ui->setupUi(this);

	connect(ui->filmSelect, &QPushButton::clicked, this, [this] ()
	{
		auto fileName = QFileDialog::getOpenFileName(this, "Select collection", QDir::homePath(), "Video (*.webm *.mkv *.flv *.avi *.wmv *.mp4 *.mpv *.m4v)");

		if (!fileName.isEmpty())
		{
			ui->filmEdit->setText(fileName);
		}
	});
	connect(ui->posterSelect, &QPushButton::clicked, this, [this] ()
	{
		auto fileName = QFileDialog::getOpenFileName(this, "Select collection", QDir::homePath(), "Image (*.jpg *.jpeg *.jpe *.png *.tiff *.tif *.bmp)");

		if (!fileName.isEmpty())
		{
			ui->posterEdit->setText(fileName);
		}
	});

	connect(ui->buttonBox, &QDialogButtonBox::accepted, this, [collection, this] ()
	{
		film.attributes[Film::NamesAttributes[Film::Title]] = ui->titleEdit->text().toStdString();
		film.attributes[Film::NamesAttributes[Film::Lead]] = ui->leadEdit->text().toStdString();
		film.attributes[Film::NamesAttributes[Film::Language]] = ui->languageEdit->text().toStdString();
		film.attributes[Film::NamesAttributes[Film::Rating]] = ui->ratingEdit->text().toStdString();
		film.attributes[Film::NamesAttributes[Film::Director]] = ui->directorEdit->text().toStdString();
		film.attributes[Film::NamesAttributes[Film::Year]] = ui->yearEdit->text().toStdString();
		film.attributes[Film::NamesAttributes[Film::Genre]] = ui->genreEdit->text().toStdString();
		film.attributes[Film::NamesAttributes[Film::Accolades]] = ui->accoladesEdit->text().toStdString();
		film.attributes[Film::NamesAttributes[Film::Path]] = ui->filmEdit->text().toStdString();
		film.attributes[Film::NamesAttributes[Film::PosterPath]] = ui->posterEdit->text().toStdString();
		film.attributes[Film::NamesAttributes[Film::Synopsis]] = ui->synopsisEdit->toPlainText().toStdString();
		film.attributes[Film::NamesAttributes[Film::Description]] = ui->descriptionEdit->toPlainText().toStdString();

		film.favourite = ui->favouriteCheck->isChecked();

		film.id = collection->crbegin()->id + 1;

		collection->push_back(film);

		emit filmAdded();

		this->accept();
	});
}

AddItemDialog::~AddItemDialog()
{
	delete ui;
}
