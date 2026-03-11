#include <iostream>
#include <fstream>
#include <limits>
#include <string>
#include <unordered_map>
#include <vector>

const std::vector<int> SIZES = {1, 3, 5, 10};
const std::vector<int> SIZES_2 = {1, 3, 5, 10, 15, 16, 20, 24, 25, 30};
const std::vector<int> SIZES_3 = {1, 3, 5, 10, 15, 16, 20, 24, 25, 30, 37, 38, 49, 50, 74, 75, 100, 101};

std::vector<int> parse_brightnesses(std::ifstream &input) {
  std::vector<int> brightnesses;
  std::string line;
  while(std::getline(input, line)) {
    brightnesses.push_back(std::stoi(line));
  }
  return brightnesses;
}

int get_min_beetles_needed(int brightness, const std::vector<int> &sizes, 
  std::unordered_map<int, int> &memo) 
{
  if(brightness == 0) { return 0; }
  if(memo.find(brightness) != memo.end()) { return memo[brightness]; }

  int min_beetles = std::numeric_limits<int>::max();
  for(auto it = sizes.rbegin(); it != sizes.rend(); it++) {
    if(brightness >= *it) {
      min_beetles = std::min(min_beetles, 1 + get_min_beetles_needed(brightness - *it, sizes, memo));
    }
  }
  memo.insert({brightness, min_beetles});
  return min_beetles;
}

int sum_beetles_needed(const std::vector<int> &brightnesses, const std::vector<int> &sizes) {
  std::unordered_map<int, int> memo;
  int total_beetles = 0;
  for(int brightness : brightnesses) {
    total_beetles += get_min_beetles_needed(brightness, sizes, memo);
  }
  return total_beetles;
}

int sum_beetles_needed_pairs(const std::vector<int> &brightnesses, const std::vector<int> &sizes) {
  std::unordered_map<int, int> memo;
  int total_beetles = 0;
  for(int brightness : brightnesses) {
    int left = brightness / 2;
    int right = left + (brightness & 1);
    int min_beetles = std::numeric_limits<int>::max();
    for(int i = 0; i <= 50 - (brightness & 1); i++) {
      min_beetles = std::min(min_beetles, get_min_beetles_needed(left - i, sizes, memo) +
        get_min_beetles_needed(right + i, sizes, memo));
    }
    total_beetles += min_beetles;
  }
  return total_beetles;
}

int main() {
  std::ifstream input_1("./data/q9_1.txt");
  std::ifstream input_2("./data/q9_2.txt");
  std::ifstream input_3("./data/q9_3.txt");

  std::vector<int> brightnesses = parse_brightnesses(input_1);
  std::vector<int> brightnesses_2 = parse_brightnesses(input_2);
  std::vector<int> brightnesses_3 = parse_brightnesses(input_3);
  
  std::cout << "Answer to part 1: " << sum_beetles_needed(brightnesses, SIZES) << std::endl;
  std::cout << "Answer to part 2: " << sum_beetles_needed(brightnesses_2, SIZES_2) << std::endl;
  std::cout << "Answer to part 3: " << sum_beetles_needed_pairs(brightnesses_3, SIZES_3) << std::endl;
  return 0;
}