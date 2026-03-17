#include <iostream>
#include <fstream>
#include <limits>
#include <queue>
#include <string>
#include <vector>

typedef struct {
  int i;
  int j;
} Position;

const std::vector<Position> DIRS = {
  Position{-1, 0},
  Position{0, 1},
  Position{1, 0},
  Position{0, -1}
};

typedef struct {
  std::vector<std::vector<char>> grid;
  Position start;
  Position end;
} Grid;

typedef struct {
  std::vector<std::vector<char>> grid;
  std::vector<Position> starts;
  Position end;
} GridManyStarts;

typedef struct {
  Position pos;
  int time;
} State;

Grid parse_grid(std::ifstream &input) {
  std::vector<std::vector<char>> grid;
  std::string line;
  while(std::getline(input, line)) {
    grid.push_back(std::vector<char>(line.begin(), line.end()));
  }

  Position start;
  Position end;
  for(size_t i = 0; i < grid.size(); i++) {
    for(size_t j = 0; j < grid[0].size(); j++) {
      if(grid[i][j] == 'S') {
        grid[i][j] = '0';
        start.i = i;
        start.j = j;
      }
      if(grid[i][j] == 'E') {
        grid[i][j] = '0';
        end.i = i;
        end.j = j;
      }
    }
  }

  return Grid{grid, start, end};
}

GridManyStarts parse_grid_with_many_starts(std::ifstream &input) {
  std::vector<std::vector<char>> grid;
  std::string line;
  while(std::getline(input, line)) {
    grid.push_back(std::vector<char>(line.begin(), line.end()));
  }

  std::vector<Position> starts;
  Position end;
  for(size_t i = 0; i < grid.size(); i++) {
    for(size_t j = 0; j < grid[0].size(); j++) {
      if(grid[i][j] == 'S') {
        grid[i][j] = '0';
        starts.push_back(Position{static_cast<int>(i), static_cast<int>(j)});
      }
      if(grid[i][j] == 'E') {
        grid[i][j] = '0';
        end.i = i;
        end.j = j;
      }
    }
  }

  return GridManyStarts{grid, starts, end};
}

bool is_in_bounds(int i, int j, int h, int w) {
  return i >= 0 && i < h && j >= 0 && j < w;
}

int get_optimal_time(Grid &grid) {
  int h = static_cast<int>(grid. grid.size());
  int w = static_cast<int>(grid.grid[0].size());
  std::vector<std::vector<bool>> visited(h, std::vector<bool>(w, false));

  auto cmp_state = [](State left, State right) { return left.time > right.time; };
  std::priority_queue<State, std::vector<State>, decltype(cmp_state)> to_be_visited(cmp_state);
  to_be_visited.push(State{grid.start, 0});

  while(!to_be_visited.empty()) {
    State state = to_be_visited.top();
    to_be_visited.pop();
    
    if(state.pos.i == grid.end.i && state.pos.j == grid.end.j) { return state.time; }

    if(visited[state.pos.i][state.pos.j]) { continue; }
    visited[state.pos.i][state.pos.j] = true;

    for(const Position& dir : DIRS) {
      int next_i = state.pos.i + dir.i;
      int next_j = state.pos.j + dir.j;

      if(!is_in_bounds(next_i, next_j, h, w)) { continue; }
      if(grid.grid[next_i][next_j] == '#') { continue; }

      int curr_level = grid.grid[state.pos.i][state.pos.j];
      int next_level = grid.grid[next_i][next_j];
      int change = std::max(next_level, curr_level) - std::min(next_level, curr_level);
      if(change > 5) { change = 10 - change; }
      to_be_visited.push(State{Position{next_i, next_j}, state.time + change + 1});
    }
  }

  return -1;
}

int get_most_optimal_time(const GridManyStarts &grid) {
  int optimal_time = std::numeric_limits<int>::max();
  for(const Position &start : grid.starts) {
    Grid curr_grid = {grid.grid, start, grid.end};
    optimal_time = std::min(optimal_time, get_optimal_time(curr_grid));
  }
  return optimal_time;
}

int main() {
  std::ifstream input_1("./data/q13_1.txt");
  std::ifstream input_2("./data/q13_2.txt");
  std::ifstream input_3("./data/q13_3.txt");

  Grid grid = parse_grid(input_1);
  Grid grid_2 = parse_grid(input_2);
  GridManyStarts grid_3 = parse_grid_with_many_starts(input_3);
  
  std::cout << "Answer to part 1: " << get_optimal_time(grid) << std::endl;
  std::cout << "Answer to part 2: " << get_optimal_time(grid_2) << std::endl;
  std::cout << "Answer to part 3: " << get_most_optimal_time(grid_3) << std::endl;
  return 0;
}