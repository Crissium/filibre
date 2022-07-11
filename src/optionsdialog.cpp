#include "optionsdialog.h"
#include "ui_optionsdialog.h"

OptionsDialog::OptionsDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::OptionsDialog)
{
	ui->setupUi(this);

	ui->apiEdit->setText(ImdbScraper::ApiKey.c_str());
	ui->proxyEdit->setText(ImdbScraper::ProxyAddress.c_str());

	connect(ui->buttonBox, &QDialogButtonBox::accepted, this, [this] ()
	{
		ImdbScraper::ApiKey = ui->apiEdit->text().toStdString();
		ImdbScraper::ProxyAddress = ui->proxyEdit->text().toStdString();

		this->accept();
	});
}

OptionsDialog::~OptionsDialog()
{
	delete ui;
}
