#include "filmcollection.h"
#include <pugixml.hpp>
#include <algorithm>

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

std::set<std::string> FilmCollection::allValuesOfAttribute(Film::Attribute attr) const
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
