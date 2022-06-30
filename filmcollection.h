#ifndef FILMCOLLECTION_H
#define FILMCOLLECTION_H

#include <list>
#include <set>
#include "film.h"

class FilmList : public std::list<Film *>
{
public:
};

/**
 * @brief Film collection stored in memory
 *
 * At a particular time we should only have one copy of the film collection.
 * This class is the full collection stored concretely in memory.
 * I think, tentatively, all operations should be performed with pointers.
 */
class FilmCollection : public std::list<Film>
{
/*
 * Sample XML used:
 * <?xml version="1.0" encoding="UTF-8"?>
<collection>
	<item Favourite="true" ID="0" Title="Braveheart" Director="Mel Gibson" Lead="Mel Gibson" Year="1995" Genre="whatever" Poster="/home/ellis/Pictures/unnamed.png" Language="English" Synopsis="emm" Rating="8" Path="/path/to/film.mkv" Description="emm" Accolades="Oscar"/>
</collection>
 */

public:
	FilmCollection();
	FilmCollection(const std::string & xmlFileName);

	void writeToXmlFile(const std::string & xmlFileName);

	std::set<std::string> allValuesOfAttribute(Film::Attribute) const;
	FilmList filmsWithAttributeValue(Film::Attribute, const std::string & value);
	FilmList favourites();
	FilmList all();
};

#endif // FILMCOLLECTION_H
