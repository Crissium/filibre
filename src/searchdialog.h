#ifndef SEARCHDIALOG_H
#define SEARCHDIALOG_H

#include <QDialog>
#include "filmcollection.h"

namespace Ui {
class SearchDialog;
}

class SearchDialog : public QDialog
{
	Q_OBJECT

public:
	explicit SearchDialog(FilmCollection *, QWidget *parent = nullptr);
	~SearchDialog();

signals:
	void searchResult(const FilmList &);

private:
	Ui::SearchDialog *ui;
};

#endif // SEARCHDIALOG_H
