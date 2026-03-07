#include <iostream>
#include <fstream>
#include <string>
#include <vector>

const std::vector<std::pair<int, int>> DIRS = {
  {-1, 0},
  {0, 1},
  {1, 0},
  {0, -1}
};

const std::vector<std::pair<int, int>> DIRS_WITH_DIAGS = {
  {-1, -1},
  {-1, 0},
  {-1, 1},
  {0, -1},
  {0, 1},
  {1, -1},
  {1, 0},
  {1, 1}
};

bool is_in_bounds(int &i, int &j, int &height, int &width) {
  return i >= 0 && i < height && j >= 0 && j < width;
}

int count_blocks_dug(std::vector<std::vector<char>> &grid, bool use_diags) {
  std::vector<std::vector<char>> curr_grid = grid;
  int blocks_dug = 0;
  char curr_level = '1';
  for(auto &row : curr_grid) {
    for(auto &col : row) {
      if(col == '#') {
        blocks_dug++;
        col = curr_level;
      }
    }
  }

  int height = static_cast<int>(curr_grid.size());
  int width = static_cast<int>(curr_grid[0].size());
  std::vector<std::pair<int, int>> to_be_dug;
  do {
    to_be_dug.clear();
    for(int i = 0; i < height; i++) {
      for(int j = 0; j < width; j++) {
        if(curr_grid[i][j] == curr_level) {
          bool is_diggable = true;
          for(auto &dir : (use_diags ? DIRS_WITH_DIAGS : DIRS)) {
            int next_i = i + dir.first;
            int next_j = j + dir.second;
            if(!is_in_bounds(next_i, next_j, height, width)) {
              is_diggable = false;
              break;
            }
            if(curr_grid[next_i][next_j] != curr_level) {
              is_diggable = false;
              break;
            }
          }

          if(is_diggable) { to_be_dug.push_back({i, j}); }
        }
      }
    }

    curr_level++;
    blocks_dug += static_cast<int>(to_be_dug.size());
    for(const std::pair<int, int> &coords : to_be_dug) {
      curr_grid[coords.first][coords.second] = curr_level;
    }
  } while(to_be_dug.size() > 0);

  return blocks_dug;
}

int main() {
  std::ifstream input_1("./data/q3_1.txt");
  std::ifstream input_2("./data/q3_2.txt");
  std::ifstream input_3("./data/q3_3.txt");
  std::string line;

  std::vector<std::vector<char>> grid;
  while(std::getline(input_1, line)) {
    std::vector<char> row(line.begin(), line.end());
    grid.push_back(row);
  }
  
  std::vector<std::vector<char>> grid_2;
  while(std::getline(input_2, line)) {
    std::vector<char> row(line.begin(), line.end());
    grid_2.push_back(row);
  }

  std::vector<std::vector<char>> grid_3;
  while(std::getline(input_3, line)) {
    std::vector<char> row(line.begin(), line.end());
    grid_3.push_back(row);
  }

  std::cout << "Answer to part 1: " << count_blocks_dug(grid, false) << std::endl;
  std::cout << "Answer to part 2: " << count_blocks_dug(grid_2, false) << std::endl;
  std::cout << "Answer to part 3: " << count_blocks_dug(grid_3, true) << std::endl;
  return 0;
}