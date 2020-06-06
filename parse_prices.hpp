#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <numeric>
#include <json/reader.h>
#include <json/value.h>


struct Diff {
  double close_price;
  double difference;
  double corrected_diff;

  friend std::ostream& operator<< (std::ostream& os, Diff& diff) {
    os << "close: " << diff.close_price << '\t'
       << "difference: " << diff.difference << '\t'
       << "corrected: " << diff.corrected_diff;

    return os;
  }
};

std::vector<Diff> fetch_price_movements (const std::string& filename) {
  Json::CharReaderBuilder rbuilder;
  Json::Value root;
  std::string err;

  std::ifstream is (filename);

  bool ok = Json::parseFromStream(rbuilder, is, &root, &err);

  Json::Value time_series = root["Time Series (Daily)"];
  std::vector<double> prices;

  for (auto day = time_series.begin(); day != time_series.end(); ++day) {
    prices.push_back(std::atof(day->get("4. close", "0.0").asString().c_str()));
  }

  std::vector<Diff> diffs;
  for (auto p = prices.begin() + 1; p != prices.end(); ++p) {
    Diff d;
    d.close_price = *p;
    d.difference = *p - *(p - 1);
    d.corrected_diff = d.difference / d.close_price;
    diffs.push_back(d);
  }

  is.close();

  return diffs;
}
