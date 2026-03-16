#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

typedef struct {
  int x;
  int y;
} Position;

typedef struct {
  char name;
  Position position;
} Cannon;

enum TargetType {
  NORMAL,
  HARD
};

typedef struct {
  Position position;
  TargetType type;
} Target;

typedef struct {
  std::vector<Cannon> cannons;
  std::vector<Target> targets;
} Simulation;

Simulation parse_simulation(std::ifstream &input) {
  std::vector<std::vector<char>> grid;
  std::string line;
  while(std::getline(input, line))  {
    grid.push_back(std::vector<char>(line.begin(), line.end()));
  }

  std::vector<Cannon> cannons;
  std::vector<Target> targets;

  for(size_t i = 0; i < grid.size(); i++) {
    for(size_t j = 0; j < grid[0].size(); j++) {
      if(grid[i][j] == '.' || grid[i][j] == '=') { continue; }
      if(grid[i][j] == 'T') {
        targets.push_back({
          Position{static_cast<int>(j - 1), static_cast<int>(grid.size() - 2 - i)},
          TargetType::NORMAL
        });
      } else if(grid[i][j] == 'H') {
        targets.push_back({
          Position{static_cast<int>(j - 1), static_cast<int>(grid.size() - 2 - i)},
          TargetType::HARD
        });
      } else {
        cannons.push_back(Cannon{
          grid[i][j], 
          Position{static_cast<int>(j - 1), static_cast<int>(grid.size() - 2 - i)}
        });
      }
    }
  }

  return Simulation{cannons, targets};
}

Simulation parse_simulation_with_meteors(std::ifstream &input) {
  std::vector<Cannon> cannons = {
    Cannon{'A', Position{0, 0}},
    Cannon{'B', Position{0, 1}},
    Cannon{'C', Position{0, 2}}
  };

  std::vector<Target> targets;
  std::string line;
  while(std::getline(input, line)) {
    std::stringstream ss(line);
    std::string num_str;

    std::getline(ss, num_str, ' ');
    int x = std::stoi(num_str);
    std::getline(ss, num_str, ' ');
    int y = std::stoi(num_str);

    int xx = x / 2;
    int yy = y - x / 2 - x % 2;

    targets.push_back(Target{Position{xx, yy}, TargetType::NORMAL});
  }
  
  return Simulation{cannons, targets};
}

int get_ranking(const std::vector<Cannon> &cannons, const Target &target) {
  int ranking = 0;
  for(const auto &cannon : cannons) {
    int dy = target.position.y - cannon.position.y;
    int dx = target.position.x + dy;

    if(target.position.x < dy) { continue; }
    if(target.position.x < 2 * dy) {
      ranking = (cannon.name - 'A' + 1) * dy;
      break;
    }
    if(dx % 3 == 0) {
      ranking = (cannon.name - 'A' + 1) * (dx / 3);
    }
  }

  switch(target.type) {
    case TargetType::NORMAL:
      break;
    case TargetType::HARD:
      ranking *= 2;
  }
  return ranking;
}

int sum_ranking(const Simulation &simulation) {
  int total_ranking = 0;
  for(const Target &target : simulation.targets) {
    total_ranking += get_ranking(simulation.cannons, target);
  }
  return total_ranking;
}

int main() {
  std::ifstream input_1("./data/q12_1.txt");
  std::ifstream input_2("./data/q12_2.txt");
  std::ifstream input_3("./data/q12_3.txt");

  Simulation simulation = parse_simulation(input_1);
  Simulation simulation_2 = parse_simulation(input_2);
  Simulation simulation_3 = parse_simulation_with_meteors(input_3);
  
  std::cout << "Answer to part 1: " << sum_ranking(simulation) << std::endl;
  std::cout << "Answer to part 2: " << sum_ranking(simulation_2) << std::endl;
  std::cout << "Answer to part 3: " << sum_ranking(simulation_3) << std::endl;
  return 0;
}