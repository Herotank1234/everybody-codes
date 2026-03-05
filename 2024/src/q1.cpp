#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>

const std::unordered_map<char, int> enemies_to_potions = {
  {'A', 0},
  {'B', 1},
  {'C', 3},
  {'D', 5},
  {'x', 0}
};

int get_potion_count(const std::vector<char> &enemies) {
  int potion_count = 0;
  for(char e : enemies) {
    potion_count += enemies_to_potions.find(e)->second;
  }
  return potion_count;
}

int get_potion_count_pair(const std::vector<char> &enemies) {
  int potion_count = 0;
  for(size_t i = 0; i < enemies.size(); i += 2) {
    int curr_count = 0;
    int enemy_count = 0;

    curr_count += enemies_to_potions.find(enemies[i])->second;
    curr_count += enemies_to_potions.find(enemies[i + 1])->second;

    enemy_count += enemies[i] == 'x' ? 0 : 1;
    enemy_count += enemies[i + 1] == 'x' ? 0 : 1;

    if(enemy_count == 2) { curr_count += 2; }
    potion_count += curr_count;
  }
  return potion_count;
}

int get_potion_count_triple(const std::vector<char> &enemies) {
  int potion_count = 0;
  for(size_t i = 0; i < enemies.size(); i += 3) {
    int curr_count = 0;
    int enemy_count = 0;

    curr_count += enemies_to_potions.find(enemies[i])->second;
    curr_count += enemies_to_potions.find(enemies[i + 1])->second;
    curr_count += enemies_to_potions.find(enemies[i + 2])->second;

    enemy_count += enemies[i] == 'x' ? 0 : 1;
    enemy_count += enemies[i + 1] == 'x' ? 0 : 1;
    enemy_count += enemies[i + 2] == 'x' ? 0 : 1;

    if(enemy_count == 2) {
      curr_count += 2;
    } else if(enemy_count == 3) {
      curr_count += 6;
    }
    potion_count += curr_count;
  }
  return potion_count;
}

int main() {
  std::ifstream input_1("./data/q1_1.txt");
  std::ifstream input_2("./data/q1_2.txt");
  std::ifstream input_3("./data/q1_3.txt");
  std::string line;
  std::getline(input_1, line);
  std::vector<char> enemies_1(line.begin(), line.end());

  std::getline(input_2, line);
  std::vector<char> enemies_2(line.begin(), line.end());

  std::getline(input_3, line);
  std::vector<char> enemies_3(line.begin(), line.end());

  std::cout << "Answer to part 1: " << get_potion_count(enemies_1) << std::endl;
  std::cout << "Answer to part 2: " << get_potion_count_pair(enemies_2) << std::endl;
  std::cout << "Answer to part 3: " << get_potion_count_triple(enemies_3) << std::endl;
  return 0;
}