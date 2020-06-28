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


using namespace std;
namespace fs = filesystem;


char* AV_API_KEY = getenv("AV_API_KEY");
fs::path AV_DOWNLOAD_DIR = getenv("AV_DOWNLOAD_DIR");

vector<string> split_string (const string& str, const char delim) {
  vector<string> output;
  istringstream is (str);
  string token;
  while(getline(is, token, delim)) {
    output.push_back(token);
  }

  return output;
}

vector<fs::path> filter_by_symbol (const vector<fs::path>& filepaths,
                                   const string& symbol) {
  vector<fs::path> filtered;
  for (const auto& f : filepaths) {
    if (split_string(f.filename(), '_').at(0) == symbol) {
      filtered.push_back(f);
    }
  }

  return filtered;
}

int main (int argc, char** argv) {
  map<string, int> commands { { "download", 1},
                              { "list", 2},
                              { "clean", 3 },
                              { "concat", 4 } };

  char* api_key = getenv("AV_API_KEY");
  fs::path data_dir = getenv("AV_DOWNLOAD_DIR");

  cxxopts::Options options ("Mike's Quant Tools",
                            "Some basic tools for messing around with finance data.");

  options.add_options()
    ("c,command", "Command to execute", cxxopts::value<string>())
    ("a,all", "all", cxxopts::value<bool>())
    ("p,parse", "after download, parse results to stdout", cxxopts::value<string>())
    ("f,filename", "Filename String", cxxopts::value<string>())
    ("s,symbol", "Symbol String", cxxopts::value<string>())
    ("o,old", "Old files only", cxxopts::value<bool>())
    ("v,verbose", "Verbose output", cxxopts::value<bool>());

  options.parse_positional({ "command" });

  auto result = options.parse(argc, argv);

  if (api_key == NULL) {
    cout << "api key not set" << endl;
    return 1;
  }

  int command  = commands.at(result["command"].as<string>());
  bool verbose = result["verbose"].as<bool>();

  switch (command) {
  case 1: {
    // download data for symbol given with -s
    string symbol = result["symbol"].as<string>();
    fs::path filepath = avtools::download_data(symbol);

    if (fs::exists(filepath)) {
      cout << filepath.string() << '\n';
    }

    break;
  }
  case 2: {
    // list all data files in AV_DOWNLOAD_DIR
    vector<string> filepaths;

    for (auto i : fs::directory_iterator(data_dir)) {
      filepaths.push_back(i.path().filename());
    }

    sort(filepaths.begin(), filepaths.end());

    for (auto& f : filepaths) {
      istringstream is (f);
      string token;
      cout << boolalpha;
      while (getline(is, token, '_')) {
        if (token.find('.') == 10) {
          cout << token.substr(0, 10) << '\n';
        } else {
          cout << token << '\t';
        }
      }
    }
    cout << flush;

    break;
  }
  case 3: {
    // clean the AV_DOWNLOAD_DIR
    vector<fs::path> all_files;
    vector<string> symbol_vec;
    for (const auto& p : fs::directory_iterator(data_dir)) {
      all_files.push_back(p);
    }

    if (result.count("symbol")) {
      string symbols = result["symbol"].as<string>();
      symbol_vec = split_string(symbols, ',');
      cout << "removing..." << '\n';
      for (const auto& s : symbol_vec) {
        vector<fs::path> filtered = filter_by_symbol(all_files, s);
        if (result.count("old")) {
          sort(filtered.begin(),
               filtered.end(),
               [](fs::path& f1, fs::path& f2) {
                      return f1.filename() > f2.filename();
                    });
          for (auto it = filtered.begin()+1; it != filtered.end(); ++it) {
            cout << (*it).string() << '\n';
            fs::remove(*it);
          }
        } else {
          for (const auto& p : filtered) {
          cout << p.string() << '\n';
          fs::remove(p);
        }
        cout << flush;
        }
      }
    }

    if (result.count("all")) {
      cout << "removing..." << '\n';
      for (const auto& f : all_files) {
        cout << f.string() << '\n';
        fs::remove(f);
      }
    }

    break;
  }
  default: {
    cout << "please enter a valid command" << endl;
    break;
  }
  }

  return 0;
}
