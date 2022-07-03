#include "filmitem.h"
#include <QPainter>
#include <iostream>

QImage FilmItem::FallBackImage(QImage(":/images/film-image-fallback.png").scaled(PosterWidth, PosterHeight, Qt::KeepAspectRatio));

FilmItem::FilmItem(QGraphicsLayoutItem * parent, bool isLayout): QGraphicsLayoutItem(parent, isLayout), film(nullptr), poster(&FallBackImage)
{
}

FilmItem::FilmItem(Film * f, QGraphicsLayoutItem * parent, bool isLayout)
	: QGraphicsLayoutItem(parent, isLayout)
	, film(f)
	, poster(((film->attributes.at(Film::NamesAttributes[Film::PosterPath]).empty()) ? (&FallBackImage) : (new QImage(QImage(film->attributes.at(Film::NamesAttributes[Film::PosterPath]).c_str()).scaled(PosterWidth, PosterHeight, Qt::KeepAspectRatio)))))
{
	setFlag(ItemIsFocusable, false);
}

FilmItem::~FilmItem()
{
	if (poster != &FallBackImage)
		delete poster;
}

QRectF FilmItem::boundingRect() const
{
	return QRectF(0, 0, BorderWidth + PosterWidth + BorderWidth, BorderWidth + PosterHeight + BorderWidth);
}

void FilmItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * item, QWidget * widget)
{
	Q_UNUSED(widget);
	Q_UNUSED(item);

	painter->drawImage(QPoint(BorderWidth, BorderWidth), *poster);
}

void FilmItem::setGeometry(const QRectF & geom)
{
	prepareGeometryChange();
	QGraphicsLayoutItem::setGeometry(geom);
	setPos(geom.topLeft());
}

void FilmItem::mousePressEvent(QGraphicsSceneMouseEvent * event)
{
	emit itemSelected(film);

	QGraphicsItem::mousePressEvent(event);
	update();
}

void FilmItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
{
	emit itemDoubleClicked(film);

	QGraphicsItem::mouseDoubleClickEvent(event);
	update();
}

QSizeF FilmItem::sizeHint(Qt::SizeHint which, const QSizeF & constraint) const
{
	switch (which)
	{
	case Qt::MinimumSize:
	case Qt::PreferredSize:
	case Qt::MaximumSize:
	case Qt::MinimumDescent:
		return QSizeF(BorderWidth + PosterWidth + BorderWidth, BorderWidth + PosterHeight + BorderWidth);
	default:
		break;
	}
	return constraint;
}
