#ifndef COLLECTIONDISPLAY_H
#define COLLECTIONDISPLAY_H

#include <QGraphicsScene>
#include <QGraphicsWidget>
#include "filmcollection.h"
#include "filmitem.h"

class CollectionDisplay : public QGraphicsWidget
{
	Q_OBJECT
public:
	CollectionDisplay();
	CollectionDisplay(FilmList, int areaWidth, QGraphicsScene * scene, QGraphicsItem * parent = nullptr, Qt::WindowFlags wFlags = Qt::WindowFlags());

signals:
	void itemSelected(Film *);
	void itemDoubleClicked(Film *);
};

#endif // COLLECTIONDISPLAY_H
