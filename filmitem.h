#ifndef FILMITEM_H
#define FILMITEM_H

#include <QGraphicsLayoutItem>
#include <QGraphicsObject>
#include <QImage>
#include "film.h"

class FilmItem : public QGraphicsObject, public QGraphicsLayoutItem
{
	Q_OBJECT
	Q_INTERFACES(QGraphicsLayoutItem);
public:
	enum
	{
		PosterWidth = 150,
		PosterHeight = 200,
		BorderWidth = 20
	};
private:
	static QImage FallBackImage;
	Film * film;
	QImage * poster;

public:
	FilmItem(QGraphicsLayoutItem * parent = nullptr, bool isLayout = false);
	FilmItem(Film *, QGraphicsLayoutItem * parent = nullptr, bool isLayout = false);
	~FilmItem();

	QRectF boundingRect() const override;
	void paint(QPainter * painter, const QStyleOptionGraphicsItem * item, QWidget * widget) override;
	void setGeometry(const QRectF &geom) override;

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent * event) override;
	QSizeF sizeHint(Qt::SizeHint which, const QSizeF & constraint = QSizeF()) const override;

signals:
	void itemSelected(Film *);
};

#endif // FILMITEM_H

