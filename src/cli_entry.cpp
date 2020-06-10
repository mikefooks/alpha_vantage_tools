#include <iostream>
#include <string>
#include <exception>
#include <filesystem>
#include <cstdlib>
#include <cxxopts.hpp>
#include <cstdlib>
#include "download_data.h"
#include "parse_prices.h"


namespace fs = std::filesystem;

int main (int argc, char** argv) {
  cxxopts::Options options ("Mike's Quant Tools",
                            "Some basic tools for messing around with finance data.");

  options.add_options()
    ("c,command", "Command to execute", cxxopts::value<std::string>())
    ("f,filename", "Filename String", cxxopts::value<std::string>())
    ("s,symbol", "Symbol String", cxxopts::value<std::string>());

  options.parse_positional({ "command" });

  auto result = options.parse(argc, argv);

  char* api_key = std::getenv("AV_API_KEY");
  std::cout << api_key << '\n';

  std::string command  = result["command"].as<std::string>();
  std::string symbol = result["symbol"].as<std::string>();

  std::cout << "Downloading " << symbol << " data..." << '\n';

  bool ok = download_data(symbol);

  if (ok) {
    std::cout << "Success!" << std::endl;
  }

  /*
  try {
    std::string command = result["command"].as<std::string>();
    fs::path filepath = result["f"].as<std::string>();
  } catch (std::exception& e) {
    std::cout << e.what() << std::endl;
    return 1;
  }
  */

  return 0;
}
