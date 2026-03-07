#include <algorithm>
#include <iostream>
#include <fstream>
#include <limits>
#include <string>
#include <vector>

int get_nail_strikes_to_min(const std::vector<int> &nails) {
  int min_length = std::numeric_limits<int>::max();
  for(const int &nail : nails) {
    min_length = std::min(min_length, nail);
  }

  int strikes = 0;
  for(const int &nail : nails) {
    strikes += nail - min_length;
  }

  return strikes;
}

int get_min_nail_strikes(std::vector<int> &nails) {
  std::sort(nails.begin(), nails.end());
  size_t median_index = nails.size() / 2;
  int median = nails[median_index];

  int strikes = 0;
  for(const int &nail : nails) {
    strikes += std::abs(nail - median);
  }
  return strikes;
}

int main() {
  std::ifstream input_1("./data/q4_1.txt");
  std::ifstream input_2("./data/q4_2.txt");
  std::ifstream input_3("./data/q4_3.txt");
  std::string line;
  
  std::vector<int> nails;
  while(std::getline(input_1, line)) {
    nails.push_back(std::stoi(line));
  }

  std::vector<int> nails_2;
  while(std::getline(input_2, line)) {
    nails_2.push_back(std::stoi(line));
  }

  std::vector<int> nails_3;
  while(std::getline(input_3, line)) {
    nails_3.push_back(std::stoi(line));
  }

  std::cout << "Answer to part 1: " << get_nail_strikes_to_min(nails) << std::endl;
  std::cout << "Answer to part 2: " << get_nail_strikes_to_min(nails_2) << std::endl;
  std::cout << "Answer to part 3: " << get_min_nail_strikes(nails_3) << std::endl;
  return 0;
}