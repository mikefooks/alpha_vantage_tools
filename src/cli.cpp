#include "download_data.h"
#include "parse_prices.h"
#include <cxxopts.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <tuple>
#include <exception>
#include <filesystem>


namespace fs = std::filesystem;

std::vector<std::string> split_string (const std::string& str, const char delim) {
  std::vector<std::string> output;
  std::istringstream is (str);
  std::string token;
  while(std::getline(is, token, delim)) {
    output.push_back(token);
  }

  return output;
}

struct data_filepath {
  fs::path _path;
  std::string _basepath;
  std::string _symbol;
  std::string _date;
  std::string _ext;

  data_filepath (const fs::path& path) : _path(path) {
    std::vector<std::string> stem = split_string(_path.stem(), '_');
    _basepath = _path.root_path();
    _symbol = stem.at(0);
    _date = stem.at(1);
    _ext = _path.extension();
  }

  friend std::ostream& operator<< (std::ostream& os, const data_filepath& df) {
    os << df._path;
    return os;
  }
};

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
    // download data for symbol given with -s
    std::string symbol = result["symbol"].as<std::string>();
    bool ok = avtools::download_data(symbol);

    if (ok) {
      std::cout << "success!" << std::endl;
    }
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
    std::vector<std::string> symbol_vec;
    bool old_only;

    if (result.count("symbol")) {
      std::string symbols = result["symbol"].as<std::string>();
      symbol_vec = split_string(symbols, ',');
    }

    if (result.count("old")) {
      old_only = result["old"].as<bool>();
    }

    std::vector<data_filepath> filepaths;
    for (auto& p : fs::directory_iterator(data_dir)) {
      filepaths.push_back(data_filepath(p));
    }

    for (auto& f : filepaths) {
      std::cout << f << '\n';
    }

    std::cout << std::endl;
    /*
    if (symbols != "") {
      for (auto& s : split_string(symbols, ',')) {
        symbol_vec.push_back(s);
      }
    }

    /
    std::cout << std::endl;
    for (auto& p : fs::directory_iterator(data_dir)) {
      fs::remove(p);
    }
    */
    break;
  }
  default:
    std::cout << "please enter a valid command" << std::endl;
    break;
  }

  return 0;
}
