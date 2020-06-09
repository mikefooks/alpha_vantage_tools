#include <iostream>
#include <exception>
#include <filesystem>
#include <cxxopts.hpp>


namespace fs = std::filesystem;

int main (int argc, char** argv) {
  cxxopts::Options options ("Mike's Quant Tools",
                            "Some basic tools for messing around with finance data.");

  options.add_options()
    ("f,filename", "Filename String", cxxopts::value<std::string>())
    ("s,symbols", "Symbols String", cxxopts::value<std::string>());

  auto result = options.parse(argc, argv);

  try {
    fs::path filepath = result["f"].as<std::string>();
    std::cout << fs::absolute(filepath) << std::endl;
  } catch (std::exception& e) {
    std::cout << e.what() << std::endl;
    return 1;
  }

  return 0;
}
