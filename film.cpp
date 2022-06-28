#include "film.h"

const std::array<const std::string, Film::NumAttributes> Film::NamesAttributes({"Title","Director","Lead","Year","Genre","Poster","Language","Synopsis","Rating","Path","Description","Accolades"});

Film::Film(): attributes(NumAttributes)
{
}

bool Film::operator==(const Film &other) const
{
	return id == other.id;
}

bool Film::operator<(const Film &other) const
{
	return id < other.id;
}

std::function<bool(const Film &, const Film &)> lessPredicateWithAttr(Film::Attribute attr)
{
	return [=] (const Film & lhs, const Film & rhs) -> bool
	{
		return lhs.attributes.at(Film::NamesAttributes[attr]) < rhs.attributes.at(Film::NamesAttributes[attr]);
	};
}
