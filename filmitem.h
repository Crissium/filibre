#ifndef FILMITEM_H
#define FILMITEM_H

#include <QGraphicsItem>
#include <QImage>
#include "film.h"

class FilmItem : public QGraphicsItem
{
	Q_OBJECT
private:
	Film * film;
	QImage * poster;


public:
	FilmItem();
	FilmItem(Film * );
	~FilmItem();
};

#endif // FILMITEM_H
