#ifndef __AVT_DOWNLOAD
#define __AVT_DOWNLOAD

#include <iostream>
#include <iomanip>
#include <filesystem>
#include <ctime>
#include <string>
#include <sstream>
#include <stdexcept>
#include <cstdlib>
#include <curl/curl.h>


namespace fs = std::filesystem;

namespace avtools {

  std::string& download_data (const std::string& symbol) {
    fs::path filepath (std::getenv("AV_DOWNLOAD_DIR"));

    std::stringstream filename;
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    filename << symbol << "_";
    filename << std::put_time(&tm, "%Y-%m-%d");
    filename << ".json";

    filepath += fs::path(filename.str());

    std::cout << filepath.c_str() << std::endl;

    FILE* data = fopen(filepath.c_str(), "w");

    std::stringstream URL;
    URL << "https://www.alphavantage.co/query?function=TIME_SERIES_DAILY";
    URL << "&symbol=" << symbol;
    URL << "&outputsize=full";
    URL << "&datatype=json";
    URL << "&apikey=" << "7I8BNLLQRWXUNHXJ";

    // set up curl
    CURL* curl = curl_easy_init();
    CURLcode response;

    char errbuf[512];

    try {
      curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);
      curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
      curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
      curl_easy_setopt(curl, CURLOPT_WRITEDATA, data);
      curl_easy_setopt(curl, CURLOPT_URL, URL.str().c_str());

      response = curl_easy_perform(curl);

    } catch (const std::exception& e) {
      std::cout << "something went wrong: " << e.what() << std::endl;
      return false;
    }

    if (response != CURLE_OK) {
      std::cout << curl_easy_strerror(response) << '\n';
      std::cout << errbuf << std::endl;
      return false;
    }

    fclose(data);
    curl_easy_cleanup(curl);
    return filepath;
  }
}

#endif
