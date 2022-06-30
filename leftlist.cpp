#include "leftlist.h"

LeftList::LeftList(QWidget *parent)
{
	data = new QTreeView(parent);
	showFavouritesOnly = true;
	for (size_t attri = 0; attri < Film::NumAttributes; ++attri)
	{
	}
}
