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

/*
Sample HTML Inventory:
<!DOCTYPE html>
<html>

<head>
	<meta charset="utf-8">
	<title>Film Collection Inventory</title>
	<style>
		body {
			margin: 0 10%;
			font-family: serif;
		}

		h1 {
			text-align: center;
			font-family: serif;
			font-weight: bold;
			border-bottom: 1px solid sienna;
			margin-top: 30px;
		}

		h2 {
			text-align: center;
			font-family: serif;
			font-weight: bold;
			margin-top: 30px;
			font-size: 3em;
		}

	</style>
</head>

<body>
	<h1><br>Film Collection Inventory</h1>

	<h2>El Cid</h2>
	<img src="/run/media/ellis/Data/Video/Films/El Cid (1961) [BluRay] [1080p] [YTS.AM]/Poster.jpg" alt="Poster" align="right">
	<dl>
		<dt>Director(s)</dt>
		<dd>Anthony Mann</dd>

		<dt>Leading Actor(s)</dt>
		<dd>Charlton Heston</dd>

		<dt>Year</dt>
		<dd>1961</dd>

		<dt>Genre</dt>
		<dd>Epic</dd>

		<dt>Language</dt>
		<dd>English</dd>

		<dt>Accolades</dt>
		<dd>British Society of Cinematographers</dd>
	</dl>

	<h3>Synopsis</h3>
	<p>The fabled Spanish hero Rodrigo Diaz de Vivar (a.k.a. El Cid) overcomes a family vendetta and court intrigue to defend Christian Spain against the Moors.</p>
	<h3>Description</h3>
	<p>El Cid is a 1961 epic historical drama film directed by Anthony Mann and produced by Samuel Bronston. The film is loosely based on the life of the 11th century Castilian warlord Rodrigo Díaz de Vivar, called "El Cid" (from the Arabic as-sidi, meaning "The Lord"). The film stars Charlton Heston in the title role and Sophia Loren as Doña Ximena. The screenplay is credited to Fredric M. Frank, Philip Yordan, and Ben Barzman with uncredited contributions by Bernard Gordon.</p>

</body>

</html>
*/

public:
	FilmCollection();
	FilmCollection(const std::string & xmlFileName);

	void writeToXmlFile(const std::string & xmlFileName);
	void writeToHtmlInventory(const std::string & inventoryFileName);
	void writeToCsvFile(const std::string & csvFileName);

	std::set<std::string> allValuesOfAttribute(/*Film::Attribute*/int) const;
	FilmList filmsWithAttributeValue(Film::Attribute, const std::string & value);
	FilmList filmsWithAttributeValue(const std::string & attr, const std::string & value);
	FilmList searchFor(Film::Attribute, const std::string & value);
	FilmList searchFor(const std::string & attribute, const std::string value);
	FilmList favourites();
	FilmList all();
};

#endif // FILMCOLLECTION_H
