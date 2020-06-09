#include <iostream>
#include <string>
#include <gsl/gsl_movstat.h>
#include <cxxopts.hpp>
#include "parse_prices.hpp"


int main (int argc, char** argv) {
  cxxopts::Options options ("Alpha Vantage Tools",
                          "Command line tools for downloading, parsing, and anlyzing the Alpha Vantage API");

  options.add_options()
    ("h,hello_world", "say something", cxxopts::value<std::string>());

  auto result = options.parse(argc, argv);

  std::cout << result["h"].as<std::string>() << '\n';

  int window_size = 20;

  std::vector<Diff> prices = fetch_price_movements("../data/FB_recent.json");

  gsl_movstat_workspace* ws = gsl_movstat_alloc(window_size);
  gsl_vector* input = gsl_vector_alloc(prices.size());
  gsl_vector* output = gsl_vector_alloc(prices.size());

  for (long unsigned int i = 0; i < prices.size(); ++i) {
    gsl_vector_set(input, i, prices.at(i).close_price);
  }

  int success = gsl_movstat_mean(GSL_MOVSTAT_END_TRUNCATE,
                                 input,
                                 output,
                                 ws);

  std::vector<double> new_vec;
  for (long unsigned int i = 0; i < output->size; ++i) {
    new_vec.push_back(gsl_vector_get(output, i));
  }

  for (auto val : new_vec) {
    std::cout << val << '\n';
  }

  std::cout << result["hello_world"].as<std::string>() << std::endl;

  gsl_vector_free(input);
  gsl_vector_free(output);

  return 0;
}
