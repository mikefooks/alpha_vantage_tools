#include <iostream>
#include <vector>
#include <chrono>
#include <sstream>
#include <filesystem>
#include <gsl/gsl_statistics_double.h>
#include <gsl/gsl_rng.h>
#include <gsl/gsl_randist.h>
#include "parse_prices.hpp"


using namespace std::chrono;

int main (int argc, char** argv) {
  std::stringstream filename;
  filename << "/home/m1ke/development/quantitative_finance_book/";
  filename << "data/FB_recent.json";

  milliseconds ms;
  ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
  unsigned long int seed = ms.count();

  const gsl_rng_type* T;
  gsl_rng* rng;

  gsl_rng_env_setup();

  T = gsl_rng_mt19937;
  rng = gsl_rng_alloc(T);
  gsl_rng_set(rng, seed);

  std::vector<Diff> movements = fetch_price_movements(filename.str());
  std::vector<double> c_movements;

  for (auto d : movements) {
    c_movements.push_back(d.corrected_diff);
  }

  double mu = gsl_stats_mean(c_movements.data(), 1, c_movements.size());
  double sigma = gsl_stats_sd(c_movements.data(), 1, c_movements.size());

  std::vector<double> generated;

  for (int i = 0; i < 255; ++i) {
    double new_val = gsl_ran_gaussian(rng, sigma);
    generated.push_back(mu + new_val);
  }

  for (auto d : generated) {
    std::cout << d << '\n';
  }

  gsl_rng_free(rng);

  return 0;
}
