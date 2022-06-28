#include "filmcollection.h"
#include <pugixml.hpp>
#include <string>

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

		film.id = item.attribute("id").as_uint();

		for (size_t i = 0; i < Film::NumAttributes; ++i)
		{
			film.attributes[Film::NamesAttributes[i]] = item.attribute(Film::NamesAttributes[i].c_str()).as_string();
		}

		push_back(film);
	}
}

void FilmCollection::writeToXmlFile(const std::string &xmlFileName) const
{
	pugi::xml_document doc;
	auto filmList = doc.append_child("collection");

	for (auto const & film : *this)
	{
		auto item = filmList.append_child("item");

		item.append_attribute("ID").set_value(film.id);

		for (size_t i = 0; i < Film::NumAttributes; ++i)
		{
			item.append_attribute(Film::NamesAttributes[i].c_str()).set_value(film.attributes.at(Film::NamesAttributes[i]).c_str());
		}
	}

	if (!doc.save_file(xmlFileName.c_str(), "\t", pugi::format_default, pugi::encoding_utf8))
		throw "File saving error\n";
}
