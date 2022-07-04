#ifndef EDITMETADATADIALOG_H
#define EDITMETADATADIALOG_H

#include <QDialog>
#include "film.h"

namespace Ui {
class EditMetadataDialog;
}

class EditMetadataDialog : public QDialog
{
	Q_OBJECT

public:
	explicit EditMetadataDialog(Film *, QWidget * parent = nullptr);
	~EditMetadataDialog();

private:
	Ui::EditMetadataDialog *ui;

	Film film;

	void fillWithDefaultValues();

signals:
	void filmEdited(const Film);
};

#endif // EDITMETADATADIALOG_H
