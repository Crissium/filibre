#include "collectiondisplay.h"
#include <QGraphicsGridLayout>

CollectionDisplay::CollectionDisplay()
{
}

CollectionDisplay::CollectionDisplay(FilmList list, int areaWidth, QGraphicsScene * scene, QGraphicsItem * parent, Qt::WindowFlags wFlags)
	: QGraphicsWidget(parent, wFlags)
{
	// Automatically adjust row size according to width
	int numColsPerRow = areaWidth / (FilmItem::BorderWidth + FilmItem::PosterWidth + FilmItem::BorderWidth);

	auto layout = new QGraphicsGridLayout;

	if (list.empty())
	{
		auto item = new FilmItem;
		layout->addItem(item, 0, 0, Qt::AlignLeft);
	}
	else
	{
		size_t subscript = 1; // for some mysterious reason, cannot declare in for-loop
		for (auto it = list.begin(); it != list.end(); ++it)
		{
			auto item = new FilmItem(*it, this);
			scene->addItem(item);

			// Calculate item position
			if (subscript % numColsPerRow) // then would not be at a row's end
			{
				layout->addItem(item, subscript / numColsPerRow, subscript % numColsPerRow - 1, Qt::AlignLeft);
			}
			else
			{
				layout->addItem(item, subscript / numColsPerRow - 1, numColsPerRow, Qt::AlignLeft);
			}
			++subscript;

			connect(item, &FilmItem::itemSelected, this, [this] (Film * f) {emit itemSelected(f);});
		}
	}

	setLayout(layout);
}
