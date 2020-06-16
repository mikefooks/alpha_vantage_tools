#include "download_data.h"
#include "parse_prices.h"
#include <cxxopts.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <exception>
#include <filesystem>
#include <cstdlib>


namespace fs = std::filesystem;

int main (int argc, char** argv) {
  std::map<std::string, int> commands { { "download", 1},
                                        { "list", 2},
                                        { "clean", 3 } };

  char* api_key = std::getenv("AV_API_KEY");
  fs::path data_dir = std::getenv("AV_DOWNLOAD_DIR");

  cxxopts::Options options ("Mike's Quant Tools",
                            "Some basic tools for messing around with finance data.");

  options.add_options()
    ("c,command", "Command to execute", cxxopts::value<std::string>())
    ("p,parse", "after download, parse results to stdout", cxxopts::value<std::string>())
    ("f,filename", "Filename String", cxxopts::value<std::string>())
    ("s,symbol", "Symbol String", cxxopts::value<std::string>())
    ("o,old", "Old files only", cxxopts::value<bool>());

  options.parse_positional({ "command" });

  auto result = options.parse(argc, argv);

  if (api_key == NULL) {
    std::cout << "api key not set" << std::endl;
    return 1;
  }

  int command  = commands.at(result["command"].as<std::string>());

  switch (command) {
  case 1: {
    std::string symbol = result["symbol"].as<std::string>();
    bool ok = avtools::download_data(symbol);

    if (ok) {
      std::cout << "success!" << std::endl;
    }
    break;
  }
  case 2: {
    std::vector<std::string> filepaths;

    for (auto& i : fs::directory_iterator(data_dir)) {
      filepaths.push_back(i.path().filename());
    }

    std::sort(filepaths.begin(), filepaths.end());

    for (auto& f : filepaths) {
      std::istringstream is (f);
      std::string token;
      std::cout << std::boolalpha;
      while (std::getline(is, token, '_')) {
        if (token.find('.') == 10) {
          std::cout << token.substr(0, 10) << '\n';
        } else {
          std::cout << token << '\t';
        }
      }
    }
    std::cout << std::flush;

    break;
  }
  default:
    std::cout << "please enter a valid command" << std::endl;
    break;
  }


  return 0;
}
