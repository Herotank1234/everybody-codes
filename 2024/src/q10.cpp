#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <unordered_set>
#include <unordered_map>
#include <vector>

const std::vector<size_t> BORDERS = {0, 1, 6, 7};

std::vector<std::vector<char>> parse_grid(std::ifstream &input) {
  std::vector<std::vector<char>> grid;
  std::string line;
  while(std::getline(input, line)) {
    std::vector<char> row(line.begin(), line.end());
    grid.push_back(row);
  }
  return grid;
}

std::vector<std::vector<std::vector<char>>> parse_grids(std::ifstream &input) {
  std::vector<std::vector<std::vector<char>>> grids;
  std::string line;

  std::vector<std::vector<std::vector<char>>> curr_grids;
  bool first = true;
  while(std::getline(input, line)) {
    if(line == "") {
      grids.insert(grids.end(), curr_grids.begin(), curr_grids.end());
      curr_grids.clear();
      first = true;
    } else {
      std::stringstream ss(line);
      std::string row_str;
      size_t index = 0;
      while(std::getline(ss, row_str, ' ')) {
        std::vector<char> row(row_str.begin(), row_str.end());
        if(first) {
          std::vector<std::vector<char>> grid;
          grid.push_back(row);
          curr_grids.push_back(grid);
        } else {
          curr_grids[index++].push_back(row);
        }
      }
      if(first) { first = false; }
    }
  }
  grids.insert(grids.end(), curr_grids.begin(), curr_grids.end());
  return grids;
}

std::string get_runic_word(const std::vector<std::vector<char>> &grid) {
  std::vector<std::unordered_set<char>> row_symbols;
  std::vector<std::unordered_set<char>> col_symbols;

  for(size_t i = 0; i < grid.size(); i++) {
    std::unordered_set<char> row_symbol;
    for(size_t j = 0; j < grid[0].size(); j++) {
      if(grid[i][j] != '.' && grid[i][j] != '*') {
        row_symbol.insert(grid[i][j]);
      }
    }
    row_symbols.push_back(row_symbol);
  }

  for(size_t j = 0; j < grid[0].size(); j++) {
    std::unordered_set<char> col_symbol;
      for(size_t i = 0; i < grid.size(); i++) {
      if(grid[i][j] != '.' && grid[i][j] != '*') {
        col_symbol.insert(grid[i][j]);
      }
    }
    col_symbols.push_back(col_symbol);
  }

  std::string runic_word;
  for(size_t i = 0; i < grid.size(); i++) {
    for(size_t j = 0; j < grid[0].size(); j++) {
      if(grid[i][j] == '.') {
        for(const char &symbol : row_symbols[i]) {
          if(col_symbols[j].find(symbol) != col_symbols[j].end()) {
            runic_word += symbol;
            break;
          }
        }
      }
    }
  }

  return runic_word;
}

int sum_runic_power(const std::vector<std::vector<std::vector<char>>> &grids) {
  int total_power = 0;
  for(const auto &grid : grids) {
    std::string runic_word = get_runic_word(grid);
    int curr_power = 0;
    for(size_t i = 0; i < runic_word.size(); i++) {
      curr_power += (i + 1) * (runic_word[i] - 'A' + 1);
    }
    total_power += curr_power;
  }
  return total_power;
}

bool try_solve_runic_word(std::vector<std::vector<char>> &grid, size_t start_i, size_t start_j) {
  std::vector<std::unordered_set<char>> row_symbols;
  std::vector<std::unordered_set<char>> col_symbols;

  for(size_t i = start_i + 2; i < start_i + 6; i++) {
    std::unordered_set<char> row_symbol;
    for(const size_t &dj : BORDERS) {
      if(grid[i][start_j + dj] != '?') { row_symbol.insert(grid[i][start_j + dj]); }
    }
    row_symbols.push_back(row_symbol);
  }

  for(size_t j = start_j + 2; j < start_j + 6; j++) {
    std::unordered_set<char> col_symbol;
    for(const size_t &di : BORDERS) {
      if(grid[start_i + di][j] != '?') { col_symbol.insert(grid[start_i + di][j]); }
    }
    col_symbols.push_back(col_symbol);
  }

  bool changed = false;
  for(size_t i = 0; i < 4; i++) {
    for(size_t j = 0; j < 4; j++) {
      if(grid[start_i + 2 + i][start_j + 2 + j] == '.') {
        for(const char &symbol : row_symbols[i]) {
          if(col_symbols[j].find(symbol) != col_symbols[j].end()) {
            grid[start_i + 2 + i][start_j + 2 + j] = symbol;
            changed = true;
            break;
          }
        }
      }
    }
  }

  return changed;
}

bool try_infer_symbols(std::vector<std::vector<char>> &grid, size_t start_i, size_t start_j) {
  bool changed = false;

  for(size_t i = start_i + 2; i < start_i + 6; i++) {
    std::unordered_set<char> border_symbols;
    int question_marks = 0;
    for(const size_t &dj : BORDERS) {
      if(grid[i][start_j + dj] != '?') {
        border_symbols.insert(grid[i][start_j + dj]);
      } else {
        question_marks++;
      }
    }

    std::unordered_set<char> row_runic_symbols;
    std::vector<std::pair<size_t, size_t>> dots;
    for(size_t j = start_j + 2; j < start_j + 6; j++) {
      if(grid[i][j] == '.') {
        dots.push_back({i, j});
      } else {
        row_runic_symbols.insert(grid[i][j]);
      }
    }

    if(!(dots.size() == 1 && question_marks == 0)) { continue; }

    for(const char &symbol : row_runic_symbols) {
      border_symbols.erase(symbol);
    }
    if(border_symbols.size() != 1) { continue; } 
    changed = true;
    grid[dots[0].first][dots[0].second] = *border_symbols.begin();
  }

  for(size_t j = start_j + 2; j < start_j + 6; j++) {
    std::unordered_set<char> border_symbols;
    int question_marks = 0;
    for(const size_t &di : BORDERS) {
      if(grid[start_i + di][j] != '?') {
        border_symbols.insert(grid[start_i + di][j]);
      } else {
        question_marks++;
      }
    }

    std::unordered_set<char> col_runic_symbols;
    std::vector<std::pair<size_t, size_t>> dots;
    for(size_t i = start_i + 2; i < start_i + 6; i++) {
      if(grid[i][j] == '.') {
        dots.push_back({i, j});
      } else {
        col_runic_symbols.insert(grid[i][j]);
      }
    }

    if(!(dots.size() == 1 && question_marks == 0)) { continue; }

    for(const char &symbol : col_runic_symbols) {
      border_symbols.erase(symbol);
    }

    if(border_symbols.size() != 1) { continue; }
    changed = true;
    grid[dots[0].first][dots[0].second] = *border_symbols.begin();
  }

  return changed;
}

bool try_solve_question_mark(std::vector<std::vector<char>> &grid, size_t start_i, size_t start_j) {
  bool changed = false;

  for(size_t i = start_i + 2; i < start_i + 6; i++) {
    std::vector<std::pair<size_t, size_t>> question_marks;
    std::unordered_map<char, int> border_symbols;
    for(const size_t &dj : BORDERS) {
      if(grid[i][start_j + dj] == '?') {
        question_marks.push_back({i, start_j + dj});
      } else {
        border_symbols[grid[i][start_j + dj]]++;
      }
    }

    std::unordered_map<char, int> row_runic_symbols;
    int dots = 0;
    for(size_t j = start_j + 2; j < start_j + 6; j++) {
      if(grid[i][j] != '.') { 
        row_runic_symbols[grid[i][j]]++;
      } else {
        dots++;
      }
    }

    if(!(question_marks.size() == 1 && dots == 0)) { continue; }
    for(const auto &kv : row_runic_symbols) {
      if(border_symbols.find(kv.first) == border_symbols.end() ||
        border_symbols[kv.first] != kv.second)
      {
        changed = true;
        grid[question_marks[0].first][question_marks[0].second] = kv.first;
        break;
      }
    }
  }

  for(size_t j = start_j + 2; j < start_j + 6; j++) {
    std::vector<std::pair<size_t, size_t>> question_marks;
    std::unordered_map<char, int> border_symbols;
    for(const size_t &di : BORDERS) {
      if(grid[start_i + di][j] == '?') {
        question_marks.push_back({start_i + di, j});
      } else {
        border_symbols[grid[start_i + di][j]]++;
      }
    }

    std::unordered_map<char, int> col_runic_symbols;
    int dots = 0;
    for(size_t i = start_i + 2; i < start_i + 6; i++) {
      if(grid[i][j] != '.') {
        col_runic_symbols[grid[i][j]]++;
      } else {
        dots++;
      }
    }

    if(!(question_marks.size() == 1 && dots == 0)) { continue; }
    for(const auto &kv : col_runic_symbols) {
      if(border_symbols.find(kv.first) == border_symbols.end() ||
        border_symbols[kv.first] != kv.second) 
      {
        changed = true;
        grid[question_marks[0].first][question_marks[0].second] = kv.first;
        break;
      }
    }
  }

  return changed;
}

int sum_big_runic_power(std::vector<std::vector<char>> &grid) {
  bool changed;
  do {
    changed = false;
    for(size_t i = 0; i < grid.size() - 2; i += 6) {
      for(size_t j = 0; j < grid[0].size() - 2; j += 6) {
        changed |= try_solve_runic_word(grid, i, j);
        changed |= try_infer_symbols(grid, i, j);
        changed |= try_solve_question_mark(grid, i, j);
      }
    }
  } while(changed);

  int total_power = 0;
  for(size_t i = 0; i < grid.size() - 2; i += 6) {
    for(size_t j = 0; j < grid[0].size() - 2; j += 6) {
      std::string runic_word;

      for(size_t di = i + 2; di < i + 6; di++) {
        for(size_t dj = j + 2; dj < j + 6; dj++) {
          if(grid[di][dj] != '.') { runic_word += grid[di][dj]; }
        }
      }
      if(runic_word.size() != 16) { continue; }

      int curr_power = 0;
      for(size_t c = 0; c < runic_word.size(); c++) {
        curr_power += (c + 1) * (runic_word[c] - 'A' + 1);
      }
      total_power += curr_power;
    }
  }

  return total_power;
}

int main() {
  std::ifstream input_1("./data/q10_1.txt");
  std::ifstream input_2("./data/q10_2.txt");
  std::ifstream input_3("./data/q10_3.txt");
  
  std::vector<std::vector<char>> grid = parse_grid(input_1);
  std::vector<std::vector<std::vector<char>>> grids = parse_grids(input_2);
  std::vector<std::vector<char>> big_grid = parse_grid(input_3);

  std::cout << "Answer to part 1: " << get_runic_word(grid) << std::endl;
  std::cout << "Answer to part 2: " << sum_runic_power(grids) << std::endl;
  std::cout << "Answer to part 3: " << sum_big_runic_power(big_grid) << std::endl;
  return 0;
}