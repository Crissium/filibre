#include "filmcollection.h"
#include <pugixml.hpp>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>

FilmCollection::FilmCollection()
{
}

FilmCollection::FilmCollection(const std::string & xmlFileName)
{
	/*
	 * The XML document used here must have such a structure:
	 * the only child of the document is collection
	 * collection has an arbitrary number of items,
	 * whose attributes are specified in the Film struct
	 */

	pugi::xml_document doc;
	std::cout << "Opening collection " << xmlFileName << '\n';
	if (!doc.load_file(xmlFileName.c_str()))
		throw "parsing error\n";

	auto filmList = doc.child("collection");
	for (auto const & item : filmList.children())
	{
		Film film;

		film.favourite = item.attribute("Favourite").as_bool();
		film.id = item.attribute("ID").as_uint();

		for (auto const & attr : Film::NamesAttributes)
		{
			film.attributes[attr] = item.attribute(attr.c_str()).as_string();
		}

		push_back(film);
	}
}

void FilmCollection::writeToXmlFile(const std::string &xmlFileName)
{
	sort();

	pugi::xml_document doc;
	auto filmList = doc.append_child("collection");

	for (auto const & film : *this)
	{
		auto item = filmList.append_child("item");

		item.append_attribute("Favourite").set_value(film.favourite);
		item.append_attribute("ID").set_value(film.id);

		for (auto const & attr : Film::NamesAttributes)
		{
			item.append_attribute(attr.c_str()).set_value(film.attributes.at(attr).c_str());
		}
	}

	if (!doc.save_file(xmlFileName.c_str(), "\t", pugi::format_default, pugi::encoding_utf8))
		throw "File saving error\n";
}

void FilmCollection::writeToHtmlInventory(const std::string & inventoryFileName)
{
	sort();

	std::ofstream inventory(inventoryFileName);

	inventory << "<!DOCTYPE html>\n<html>\n<head>\n<meta charset=\"utf-8\">\n<title>Film Collection Inventory</title>\n<style>\nbody {margin: 0 10%;font-family: serif;}\nh1 {text-align: center;font-family: serif;font-weight: bold;border-bottom: 1px solid sienna;margin-top: 30px;}\nh2 {text-align: center;font-family: serif;font-weight: bold;margin-top: auto;margin-bottom: auto;font-size: 3em;}\nimg {height: 300px;width: auto;}\n</style>\n</head>\n<body>\n<h1><br>Film Collection Inventory</h1>\n";

	for (auto const & film : *this)
	{
		inventory << "<h2>" << film.attributes.at(Film::NamesAttributes[Film::Title]) << "</h2>\n"
		<< "<img src=\"" << film.attributes.at(Film::NamesAttributes[Film::PosterPath]) << "\" alt=\"Poster\" align=\"right\">\n"
		<< "<dl>\n" << "<dt>Director(s)</dt><dd>" << film.attributes.at(Film::NamesAttributes[Film::Director]) << "</dd>\n"
		<< "<dt>Leading Actor(s)</dt><dd>" << film.attributes.at(Film::NamesAttributes[Film::Lead]) << "</dd>\n"
		<< "<dt>Year</dt><dd>" << film.attributes.at(Film::NamesAttributes[Film::Year]) << "</dd>\n"
		<< "<dt>Genre</dt><dd>" << film.attributes.at(Film::NamesAttributes[Film::Genre]) << "</dd>\n"
		<< "<dt>Language</dt><dd>" << film.attributes.at(Film::NamesAttributes[Film::Language]) << "</dd>\n"
		<< "<dt>Accolades</dt><dd>" << film.attributes.at(Film::NamesAttributes[Film::Accolades]) << "</dd>\n"
		<< "</dl>\n"
		<< "<h3>Synopsis</h3>\n<p>" << film.attributes.at(Film::NamesAttributes[Film::Synopsis]) << "</p>\n"
		<< "<h3>Description</h3>\n<p>" << film.attributes.at(Film::NamesAttributes[Film::Description]) << "</p>\n";
	}

	inventory << "</body>\n</html>\n";
	inventory.close();
}

void FilmCollection::writeToCsvFile(const std::string & csvFileName)
{
	sort();

	std::ofstream csv(csvFileName);

	for (size_t i = Film::Title; i < Film::NumAttributes; ++i)
	{
		if (i == Film::PosterPath || i == Film::Path || i == Film::Synopsis || i == Film::Description)
			continue;
		csv << Film::NamesAttributes[i] << ",\n"[i == Film::NumAttributes - 1];
	}

	for (auto const & film : *this)
	{
		for (size_t i = Film::Title; i < Film::NumAttributes; ++i)
		{
			if (i == Film::PosterPath || i == Film::Path || i == Film::Synopsis || i == Film::Description)
				continue;
			csv << film.attributes.at(Film::NamesAttributes[i]) << ",\n"[i == Film::NumAttributes - 1];
		}
	}

	csv.close();
}

std::set<std::string> FilmCollection::allValuesOfAttribute(int attr) const
{
	std::set<std::string> valueSet;

	std::transform(cbegin(), cend(), std::inserter(valueSet, valueSet.begin()), [attr] (const Film & f)
	{return f.attributes.at(Film::NamesAttributes[attr]);});

	return valueSet;
}

FilmList FilmCollection::filmsWithAttributeValue(Film::Attribute attr, const std::string & value)
{
	FilmList l;

	for (auto & film : *this)
	{
		if (film.attributes.at(Film::NamesAttributes[attr]) == value)
			l.push_back(&film);
	}

	return l;
}

FilmList FilmCollection::filmsWithAttributeValue(const std::string & attr, const std::string & value)
{
	FilmList l;

	for (auto & film : *this)
	{
		if (film.attributes.at(attr) == value)
			l.push_back(&film);
	}

	return l;
}

FilmList FilmCollection::searchFor(Film::Attribute attr, const std::string & value)
{
	std::istringstream input(value);


	std::vector<std::string> words; // split value into separate words
	{
		std::string word;
		while (input >> word)
		{
			words.push_back(word);
		}
	}

	FilmList l;

	for (auto & film : *this)
	{
		bool filmContainsAttrValue = true;

		for (auto const & word : words)
		{
			if (film.attributes.at(Film::NamesAttributes[attr]).find(word) == std::string::npos)
			{
				filmContainsAttrValue = false;
				break;
			}
		}
		if (filmContainsAttrValue)
			l.push_back(&film);
	}

	return l;
}

FilmList FilmCollection::searchFor(const std::string & attribute, const std::string value)
{
	size_t indexAttribute = Film::Title;
	while (Film::NamesAttributes[indexAttribute] != attribute)
		indexAttribute++;
	return searchFor((Film::Attribute) indexAttribute, value);
}

FilmList FilmCollection::favourites()
{
	FilmList l;

	for (auto & film : *this)
	{
		if (film.favourite)
			l.push_back(&film);
	}

	return l;
}

FilmList FilmCollection::all()
{
	FilmList l;

	for (auto & film : *this)
	{
		l.push_back(&film);
	}

	return l;
}
