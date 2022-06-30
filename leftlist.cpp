#include "leftlist.h"

static const int Size_w = 100;
static const int Size_h = 20;
std::list<QPushButton *> attriButton;
LeftList::LeftList(QWidget *parent)
	: QWidget{parent},
	  showFavouritesOnly(true),
	  chosenAttribute(Film::NumAttributes)
{
	//View = new QVBoxLayout(this);
	for (size_t attri = 0; attri < Film::NumAttributes; ++attri)
	{
		QPushButton * thisButton = new QPushButton(this);
		attriButton.push_back(thisButton);
		thisButton->setText(Film::NamesAttributes[attri].c_str());
		//thisButton->setGeometry(Size_w,Size_h * attri + Size_h,Size_w,Size_h);
		//thisButton->move(thisButton->size().width(),thisButton->size().height() * (attri + 1));
		thisButton->move(0,15);
		thisButton->show();
		//View->addWidget(attriButton);
	}
}
