#ifndef LEFTLIST_H
#define LEFTLIST_H

#include <QWidget>
#include <QPushButton>
#include <list>

#include "filmcollection.h"

class LeftList : public QWidget
{
	Q_OBJECT
public:
	explicit LeftList(QWidget *parent = nullptr);
	void changeFilm_lessPredicateWithAttr();
	void showFavourites();
	void showAllFilm();

signals:
	void showThisFilm(Film::Attribute, std::string);

private:
	bool showFavouritesOnly;
	int chosenAttribute;
	std::string chosenAttriVal;
};

#endif // LEFTLIST_H
