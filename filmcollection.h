#ifndef FILMCOLLECTION_H
#define FILMCOLLECTION_H

#include <list>
#include "film.h"

class FilmCollection : public std::list<Film>
{
/*
 * Sample XML used:
 * <?xml version="1.0" encoding="UTF-8"?>
<collection>
	<item ID="0" Title="Braveheart" Director="Mel Gibson" Lead="Mel Gibson" Year="1995" Genre="whatever" Poster="/home/ellis/Pictures/unnamed.png" Language="English" Synopsis="emm" Rating="8" Path="/path/to/film.mkv" Description="emm" Accolades="Oscar"/>
</collection>
 */

public:
	FilmCollection();
	FilmCollection(const std::string & xmlFileName);

	void writeToXmlFile(const std::string & xmlFileName) const;
};

#endif // FILMCOLLECTION_H
