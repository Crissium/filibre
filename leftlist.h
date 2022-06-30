#ifndef LEFTLIST_H
#define LEFTLIST_H

#include <QWidget>
#include <QTreeView>

#include "filmcollection.h"


class LeftList : QWidget
{
	Q_OBJECT
public:
	explicit LeftList(QWidget *parent = nullptr);
	~LeftList();
	void changeFilm_lessPredicateWithAttr();
	void showFavourites();
	void showAllFilm();

signals:
	void showThisFilm(Film *);

private:
	bool showFavouritesOnly;
	QTreeView * data;
};

#endif // LEFTLIST_H
