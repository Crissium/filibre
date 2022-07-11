#include "imdbscraper.h"
#include <nlohmann/json.hpp>
#include <cmath>

void ImdbScraper::GlobalInitialise()
{
	curl_global_init(CURL_GLOBAL_ALL);
}

void ImdbScraper::GlobalCleanup()
{
	curl_global_cleanup();
}

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	((std::string *)userp)->append((char *)contents, size * nmemb);
	return size * nmemb;
}

static size_t WriteToBinary(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	size_t written = fwrite(ptr, size, nmemb, stream);
	return written;
}

std::string ImdbScraper::stripDoubleQuotes(const std::string & str) const
{
	size_t len = str.size();
	if ((str[0] == '"') && (str[len - 1] == '"'))
	{
		return str.substr(1, len - 2);
	}
	else
	{
		return str;
	}
}

std::string ImdbScraper::escapeSpaces(const std::string & str) const
{
	size_t pos = str.find(' ');
	std::string escaped(str);

	while (pos != std::string::npos)
	{
		escaped.replace(pos, 1, "%20");
		pos = escaped.find(' ');
	}

	return escaped;
}

void ImdbScraper::searchForImdbIdAndImageUrl()
{
	std::string searchKey = escapeSpaces(title + " " + year);
	std::string rawJson;

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &rawJson);
	curl_easy_setopt(curl, CURLOPT_URL, ("https://imdb-api.com/en/API/Search/k_6z1a4g8g/" + searchKey).c_str());
	curl_easy_perform(curl);

	nlohmann::json jsonInfo = nlohmann::json::parse(rawJson);

	jsonInfo.at("results")[0].at("id").get_to(imdbId);
	jsonInfo.at("results")[0].at("image").get_to(imageUrl);
}

void ImdbScraper::downloadImage() const
{
	FILE * image = fopen(imageFileName.c_str(), "wb");

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteToBinary);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, image);
	curl_easy_setopt(curl, CURLOPT_URL, imageUrl.c_str());
	curl_easy_perform(curl);

	fclose(image);
}

void ImdbScraper::downloadMetadata()
{
	std::string rawJson;

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &rawJson);
	curl_easy_setopt(curl, CURLOPT_URL, ("https://imdb-api.com/en/API/Title/k_6z1a4g8g/" + imdbId + "/FullActor,Ratings,").c_str());
	curl_easy_perform(curl);

	nlohmann::json jsonInfo = nlohmann::json::parse(rawJson);

	jsonInfo.at("plot").get_to(metadata.plot);
	jsonInfo.at("directorList")[0].at("name").get_to(metadata.director);
	jsonInfo.at("awards").get_to(metadata.awards);
	jsonInfo.at("starList")[0].at("name").get_to(metadata.stars);
	jsonInfo.at("genreList")[0].at("key").get_to(metadata.genres);
	jsonInfo.at("languageList")[0].at("key").get_to(metadata.languages);

	std::string originalRating;
	jsonInfo.at("ratings").at("imDb").get_to(originalRating);
	metadata.rating = std::round(std::stof(originalRating));
}

ImdbScraper::ImdbScraper()
{
}

ImdbScraper::ImdbScraper(const std::string & _title, const std::string & _year, const std::string & imageFile)
	: curl(curl_easy_init())
	, title(_title)
	, year(_year)
	, imageFileName(imageFile)
{
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "GET");
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(curl, CURLOPT_DEFAULT_PROTOCOL, "https");
	curl_easy_setopt(curl, CURLOPT_PROXY, "socks5://127.0.0.1:1081");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, nullptr);
}

ImdbScraper::~ImdbScraper()
{
	curl_easy_cleanup(curl);
}

ScrapedData ImdbScraper::getMetadata()
{
	searchForImdbIdAndImageUrl();
	downloadImage();
	downloadMetadata();

	return metadata;
}
