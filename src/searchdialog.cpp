#include "searchdialog.h"
#include "ui_searchdialog.h"

SearchDialog::SearchDialog(FilmCollection * collection, QWidget * parent) :
	QDialog(parent),
	ui(new Ui::SearchDialog)
{
	ui->setupUi(this);

	for (int i = Film::Title; i < Film::NumAttributes;)
	{
		if (i == Film::Year || i == Film::Genre || i == Film::PosterPath || i == Film::Language || i == Film::Rating || i == Film::Path)
		{
			++i;
			continue;
		}

		ui->fieldSelection->addItem(Film::NamesAttributes[i].c_str());
		++i;
	}

	connect(ui->buttonBox, &QDialogButtonBox::accepted, this, [collection, this] ()
	{
		emit searchResult(collection->searchFor(ui->fieldSelection->currentText().toStdString(), ui->keywordsEdit->text().toStdString()));
		this->accept();
	});
}

SearchDialog::~SearchDialog()
{
	delete ui;
}
