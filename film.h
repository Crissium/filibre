#ifndef FILM_H
#define FILM_H

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
	bool favourite;
	std::unordered_map<std::string, std::string> attributes;

	Film();

	inline bool operator==(const Film & other) const;
    bool operator<(const Film & other) const;
};

#endif // FILM_H
