#ifndef FILM_H
#define FILM_H

#include <QString>
#include <string>
#include <array>
#include <unordered_map>

struct Film
{
	enum Attribute
	{
		Title,
		Director,
		Lead,
		Year,
		Genre,
		PosterPath,
		Language,
		Synopsis,
		Rating,
		Path,
		Description,
		Accolades,
		NumAttributes
	};

	static const std::array<const std::string, NumAttributes> NamesAttributes;

	size_t id;
	std::unordered_map<std::string, std::string> attributes;

	Film();

	inline bool operator==(const Film & other) const;
	inline bool operator<(const Film & other) const;
};

std::function<bool(const Film &, const Film &)> lessPredicateWithAttr(Film::Attribute);

#endif // FILM_H
