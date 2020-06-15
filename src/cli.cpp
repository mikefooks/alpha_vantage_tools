#include "download_data.h"
#include "parse_prices.h"
#include <cxxopts.hpp>
#include <iostream>
#include <string>
#include <map>
#include <exception>
#include <filesystem>
#include <cstdlib>


namespace fs = std::filesystem;

int main (int argc, char** argv) {
  std::map<std::string, int> commands { { "download", 1},
                                        { "list-data", 2},
                                        { "clean-data-dir", 3 },
                                        { "stats", 4 } };

  cxxopts::Options options ("Mike's Quant Tools",
                            "Some basic tools for messing around with finance data.");

  options.add_options()
    ("c,command", "Command to execute", cxxopts::value<std::string>())
    ("p,parse", "after download, parse results to stdout", cxxopts::value<std::string>())
    ("f,filename", "Filename String", cxxopts::value<std::string>())
    ("s,symbol", "Symbol String", cxxopts::value<std::string>());

  options.parse_positional({ "command" });

  auto result = options.parse(argc, argv);

  char* api_key = std::getenv("AV_API_KEY");

  if (api_key == NULL) {
    std::cout << "api key not set" << std::endl;
    return 1;
  }

  int command  = commands.at(result["command"].as<std::string>());

  try {
    switch (command) {
    case 1:
      std::string symbol = result["symbol"].as<std::string>();

      bool ok = avtools::download_data(symbol);
      if (ok) {
        std::cout << "success!" << std::endl;
      }
    } e
       } catch (std::exception& e) {
    std::cerr << "something went wrong: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
