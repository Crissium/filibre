#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>
#include "imdbscraper.h"

namespace Ui {
class OptionsDialog;
}

class OptionsDialog : public QDialog
{
	Q_OBJECT

public:
	explicit OptionsDialog(QWidget *parent = nullptr);
	~OptionsDialog();

private:
	Ui::OptionsDialog *ui;
};

#endif // OPTIONSDIALOG_H
