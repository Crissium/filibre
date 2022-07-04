#ifndef ADDITEMDIALOG_H
#define ADDITEMDIALOG_H

#include <QDialog>
#include "filmcollection.h"

namespace Ui {
class AddItemDialog;
}

class AddItemDialog : public QDialog
{
	Q_OBJECT

public:
	explicit AddItemDialog(FilmCollection *, QWidget * parent = nullptr);
	~AddItemDialog();

private:
	Ui::AddItemDialog *ui;

	Film film;

signals:
	void filmAdded();
};

#endif // ADDITEMDIALOG_H
