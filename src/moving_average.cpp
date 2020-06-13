#include "parse_prices.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <gsl/gsl_movstat.h>
#include <cxxopts.hpp>


using DiffVec = std::vector<avtools::Diff>;

namespace avtools {
  
}



int main (int argc, char** argv) {
  int window_size = 20;

  std::vector<avtools::Diff> prices = fetch_price_movements("../data/FB_recent.json");

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
