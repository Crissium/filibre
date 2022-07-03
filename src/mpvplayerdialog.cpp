#include "mpvplayerdialog.h"
#include "ui_mpvplayerdialog.h"

MpvPlayerDialog::MpvPlayerDialog(const std::string & videoFileName, QWidget *parent) :
	QDialog(parent),
	ui(new Ui::MpvPlayerDialog)
{
	ui->setupUi(this);

	ui->mpv->command(QStringList() << "loadfile" << videoFileName.c_str());

	connect(ui->slider, &QSlider::sliderMoved, this, &MpvPlayerDialog::seek);
	connect(ui->playButton, &QPushButton::clicked, this, &MpvPlayerDialog::pauseResume);
	connect(ui->mpv, &MpvWidget::positionChanged, ui->slider, &QSlider::setValue);
	connect(ui->mpv, &MpvWidget::durationChanged, this, &MpvPlayerDialog::setSliderRange);
}

MpvPlayerDialog::~MpvPlayerDialog()
{
	delete ui;
}

void MpvPlayerDialog::seek(int pos)
{
	ui->mpv->command(QVariantList() << "seek" << pos << "absolute");
}

void MpvPlayerDialog::pauseResume()
{
	ui->mpv->setProperty("pause", !(ui->mpv->getProperty("pause").toBool()));
}

void MpvPlayerDialog::setSliderRange(int duration)
{
	ui->slider->setRange(0, duration);
}
