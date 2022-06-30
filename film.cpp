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
