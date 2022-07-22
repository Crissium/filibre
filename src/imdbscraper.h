#ifndef IMDBSCRAPER_H
#define IMDBSCRAPER_H

#include <curl/curl.h>
#include <string>

struct ScrapedData
{
	std::string plot;
	std::string director;
	std::string awards;
	std::string stars;
	std::string genres;
	std::string languages;
	int rating;
};

class ImdbScraper
{
private:
	CURL * curl;

	std::string title;
	std::string year;
	std::string imageFileName;

	std::string imdbId;
	std::string imageUrl;
	ScrapedData metadata;

	std::string stripDoubleQuotes(const std::string &) const;
	std::string escapeSpaces(const std::string &) const;

	void searchForImdbIdAndImageUrl();
	void downloadImage() const;
	void downloadMetadata();

public:
	static void GlobalInitialise();
	static void GlobalCleanup();

	static std::string ApiKey;
	static std::string ProxyAddress;

	ImdbScraper();
	ImdbScraper(const std::string & _title, const std::string & _year, const std::string & imageFile);
	~ImdbScraper();

	explicit operator ScrapedData();
};

#endif // IMDBSCRAPER_H
