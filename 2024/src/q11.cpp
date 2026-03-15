#include <iostream>
#include <fstream>
#include <limits>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

typedef unsigned long ul;

std::unordered_map<std::string, std::vector<std::string>> parse_rules(std::ifstream &input) {
  std::unordered_map<std::string, std::vector<std::string>> rules;
  std::string line;
  while(std::getline(input, line)) {
    std::stringstream ss(line);

    std::string name;
    std::getline(ss, name, ':');

    std::vector<std::string> children;
    std::string child;
    while(std::getline(ss, child, ',')) {
      children.push_back(child);
    }
    rules.insert({name, children});
  }

  return rules;
}

ul simulate_rounds(const std::unordered_map<std::string, std::vector<std::string>> rules, int rounds,
  std::string start_termite) 
{
  std::unordered_map<std::string, ul> population;
  population[start_termite] = 1;

  for(int _ = 0; _ < rounds; _++) {
    std::unordered_map<std::string, ul> next_population;
    for(const auto &breed : population) {
      const std::vector<std::string> &evolutions = rules.find(breed.first)->second;
      for(const auto &evolution : evolutions) {
        next_population[evolution] += breed.second;
      }
    }
    population = next_population;
  }

  ul total_population = 0;
  for(const auto &breed : population) {
    total_population += breed.second;
  }
  return total_population;
}

ul get_biggest_population_diff(const std::unordered_map<std::string, std::vector<std::string>> &rules) {
  ul max_pop = 0;
  ul min_pop = std::numeric_limits<ul>::max();
  for(const auto &rule : rules) {
    ul population = simulate_rounds(rules, 20, rule.first);
    max_pop = std::max(max_pop, population);
    min_pop = std::min(min_pop, population);
  }
  return max_pop - min_pop;
}

int main() {
  std::ifstream input_1("./data/q11_1.txt");
  std::ifstream input_2("./data/q11_2.txt");
  std::ifstream input_3("./data/q11_3.txt");
  std::string line;

  std::unordered_map<std::string, std::vector<std::string>> rules = parse_rules(input_1);
  std::unordered_map<std::string, std::vector<std::string>> rules_2 = parse_rules(input_2);
  std::unordered_map<std::string, std::vector<std::string>> rules_3 = parse_rules(input_3);
  
  std::cout << "Answer to part 1: " << simulate_rounds(rules, 4, "A") << std::endl;
  std::cout << "Answer to part 2: " << simulate_rounds(rules_2, 10, "Z") << std::endl;
  std::cout << "Answer to part 3: " << get_biggest_population_diff(rules_3) << std::endl;
  return 0;
}