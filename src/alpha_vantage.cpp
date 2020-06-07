#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <cstdlib>
#include <memory>
#include <curl/curl.h>


int main (int argc, char** argv) {

  // build the URL
  const char* API_KEY = std::getenv("AV_API_KEY");
  FILE* data = fopen("FB_recent.json", "w");

  std::stringstream URL;

  URL << "https://www.alphavantage.co/query?function=TIME_SERIES_DAILY";
  URL << "&symbol=FB";
  URL << "&outputsize=full";
  URL << "&datatype=json";
  URL << "&apikey=" << API_KEY;

  // set up curl
  CURL* curl = curl_easy_init();
  CURLcode response;

  char errbuf[512];

  // curl_global_init(CURL_GLOBAL_SSL);

  // perform request
  try {
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errbuf);

    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, data);
    curl_easy_setopt(curl, CURLOPT_URL, URL.str().c_str());

    response = curl_easy_perform(curl);

  } catch (const std::exception& e) {
    std::cout << "something went wrong: " << e.what() << std::endl;
  }

  if (response != CURLE_OK) {
    std::cout << curl_easy_strerror(response) << '\n';
    std::cout << errbuf << std::endl;
  }

  fclose(data);
  curl_easy_cleanup(curl);

  return 0;
}
