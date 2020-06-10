#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <cstdlib>
#include <memory>
#include <curl/curl.h>


bool download_data (const std::string& symbol) {
  std::stringstream output_filename;
  output_filename << symbol << "_recent.json";

  FILE* data = fopen(output_filename.str().c_str(), "w");

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
  return true;
}
