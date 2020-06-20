#include "download_data.h"
#include "parse_prices.h"
#include <cxxopts.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <functional>
#include <algorithm>
#include <filesystem>


namespace fs = std::filesystem;

char* AV_API_KEY = std::getenv("AV_API_KEY");
fs::path AV_DOWNLOAD_DIR = std::getenv("AV_DOWNLOAD_DIR");

std::vector<std::string> split_string (const std::string& str, const char delim) {
  std::vector<std::string> output;
  std::istringstream is (str);
  std::string token;
  while(std::getline(is, token, delim)) {
    output.push_back(token);
  }

  return output;
}

std::vector<fs::path> filter_by_symbol (const std::string& symbol) {
  std::vector<fs::path> filtered;
  std::cout << symbol << '\n';
  for (auto& f : fs::directory_iterator(AV_DOWNLOAD_DIR)) {
    if (split_string(f.path().filename(), '_').at(0) == symbol) {
      filtered.push_back(f.path());
    }
  }

  for (auto& f : filtered) {
    std::cout << f.filename() << '\n';
  }

  return filtered;
}

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
    ("o,old", "Old files only", cxxopts::value<bool>())
    ("v,verbose", "Verbose output", cxxopts::value<bool>());

  options.parse_positional({ "command" });

  auto result = options.parse(argc, argv);

  if (api_key == NULL) {
    std::cout << "api key not set" << std::endl;
    return 1;
  }

  int command  = commands.at(result["command"].as<std::string>());
  bool verbose = result["verbose"].as<bool>();

  switch (command) {
  case 1: {
    // download data for symbol given with -s
    std::string symbol = result["symbol"].as<std::string>();
    bool ok = avtools::download_data(symbol);

    break;
  }
  case 2: {
    // list all data files in AV_DOWNLOAD_DIR
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
  case 3: {
    // clean the AV_DOWNLOAD_DIR
    std::vector<fs::path> filepaths;
    std::vector<std::string> symbol_vec;
    for (auto& p : fs::directory_iterator(data_dir)) {
      filepaths.push_back(fs::path(p));
    }

    std::sort(filepaths.begin(),
              filepaths.end(),
              [](fs::path& f1, fs::path& f2) {
                return f1.filename() > f2.filename();
              });

    if (result.count("symbol")) {
      std::string symbols = result["symbol"].as<std::string>();
      symbol_vec = split_string(symbols, ',');
      for (auto& s : symbol_vec) {
        std::vector<fs::path> filtered = filter_by_symbol(s);
        for (auto& p : filtered) {
          std::cout << p << '\n';
        }
      }
    }
    /*
    if (result.count("symbol")) {
      // clean only specified symbols
      std::string symbols = result["symbol"].as<std::string>();
      symbol_vec = split_string(symbols, ',');

      std::vector<std::string> selected;
      for (auto& f : filepaths) {
        std::string symbol = split_string(f.filename(), '_').at(0);
        if (std::find(symbol_vec.begin(), symbol_vec.end(), symbol) != symbol_vec.end()) {
          if (verbose) {
            std::cout << "removing " << f.string() << '\n';
          }
          fs::remove(f);
        }
      }
    }
    */

    /*else {
      // clean the entire directory
      for (auto& f : filepaths) {
        if (verbose) {
          std::cout << "removing " << f.string() << '\n';
        }
        fs::remove(f);
      }
    }
    */
    std::cout << std::flush;

    break;
  }
  default:
    std::cout << "please enter a valid command" << std::endl;
    break;
  }

  return 0;
}
