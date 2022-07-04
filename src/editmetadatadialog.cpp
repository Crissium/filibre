#include "editmetadatadialog.h"
#include "ui_editmetadatadialog.h"
#include <QFileDialog>

EditMetadataDialog::EditMetadataDialog(Film * f, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::EditMetadataDialog),
	film(*f)
{
	ui->setupUi(this);

	fillWithDefaultValues();

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

	connect(ui->buttonBox, &QDialogButtonBox::accepted, this, [this] ()
	{
		film.attributes.at(Film::NamesAttributes[Film::Title]) = ui->titleEdit->text().toStdString();
		film.attributes.at(Film::NamesAttributes[Film::Lead]) = ui->leadEdit->text().toStdString();
		film.attributes.at(Film::NamesAttributes[Film::Language]) = ui->languageEdit->text().toStdString();
		film.attributes.at(Film::NamesAttributes[Film::Rating]) = ui->ratingEdit->text().toStdString();
		film.attributes.at(Film::NamesAttributes[Film::Director]) = ui->directorEdit->text().toStdString();
		film.attributes.at(Film::NamesAttributes[Film::Year]) = ui->yearEdit->text().toStdString();
		film.attributes.at(Film::NamesAttributes[Film::Genre]) = ui->genreEdit->text().toStdString();
		film.attributes.at(Film::NamesAttributes[Film::Accolades]) = ui->accoladesEdit->text().toStdString();
		film.attributes.at(Film::NamesAttributes[Film::Path]) = ui->filmEdit->text().toStdString();
		film.attributes.at(Film::NamesAttributes[Film::PosterPath]) = ui->posterEdit->text().toStdString();
		film.attributes.at(Film::NamesAttributes[Film::Synopsis]) = ui->synopsisEdit->toPlainText().toStdString();
		film.attributes.at(Film::NamesAttributes[Film::Description]) = ui->descriptionEdit->toPlainText().toStdString();

		film.favourite = ui->favouriteCheck->isChecked();

		emit filmEdited(film);

		this->accept();
	});
}

EditMetadataDialog::~EditMetadataDialog()
{
	delete ui;
}

void EditMetadataDialog::fillWithDefaultValues()
{
	ui->titleEdit->setText(film.attributes.at(Film::NamesAttributes[Film::Title]).c_str());
	ui->leadEdit->setText(film.attributes.at(Film::NamesAttributes[Film::Lead]).c_str());
	ui->languageEdit->setText(film.attributes.at(Film::NamesAttributes[Film::Language]).c_str());
	ui->ratingEdit->setText(film.attributes.at(Film::NamesAttributes[Film::Rating]).c_str());
	ui->directorEdit->setText(film.attributes.at(Film::NamesAttributes[Film::Director]).c_str());
	ui->yearEdit->setText(film.attributes.at(Film::NamesAttributes[Film::Year]).c_str());
	ui->genreEdit->setText(film.attributes.at(Film::NamesAttributes[Film::Genre]).c_str());
	ui->accoladesEdit->setText(film.attributes.at(Film::NamesAttributes[Film::Accolades]).c_str());;
	ui->filmEdit->setText(film.attributes.at(Film::NamesAttributes[Film::Path]).c_str());
	ui->posterEdit->setText(film.attributes.at(Film::NamesAttributes[Film::PosterPath]).c_str());
	ui->synopsisEdit->setPlainText(film.attributes.at(Film::NamesAttributes[Film::Synopsis]).c_str());
	ui->descriptionEdit->setPlainText(film.attributes.at(Film::NamesAttributes[Film::Description]).c_str());

	ui->favouriteCheck->setChecked(film.favourite);
}
