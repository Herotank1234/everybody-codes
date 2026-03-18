#include <cassert>
#include <iostream>
#include <fstream>
#include <limits>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

enum Direction {
  UP,
  DOWN,
  LEFT,
  RIGHT,
  FORWARD,
  BACK
};

typedef struct {
  Direction dir;
  int steps;
} Move;

typedef struct Point {
  int x;
  int y;
  int z;

  bool operator==(const Point &other) const {
    return x == other.x && y == other.y && z == other.z;
  }
} Point;

const std::vector<Point> DIRS = {
  Point{-1, 0, 0},
  Point{0, -1, 0},
  Point{0, 0, -1},
  Point{1, 0, 0},
  Point{0, 1, 0},
  Point{0, 0, 1}
};

namespace std {
  template <> struct hash<Point> {
    size_t operator()(const Point &p) const {
      std::stringstream ss;
      ss << p.x << "-" << p.y << "-" << p.z;
      std::hash<string> hasher;
      return hasher(ss.str());
    }
  };
}

class Plant {
  public:
    Plant(const std::vector<Move> &moves) {
      _curr_pos = Point{0, 0, 0};
      _max_y = _curr_pos.y;
      _build_branch(moves);
    }

    Plant(const std::vector<std::vector<Move>> &moves_lists) {
      _curr_pos = Point{0, 0, 0};
      _max_y = _curr_pos.y;
      for(const auto &moves_list : moves_lists) {
        _build_branch(moves_list);
      }
    }

    int get_max_y() {
      return _max_y;
    }

    int get_unique_points() {
      return static_cast<int>(_unique_points.size());
    }

    int get_min_murkiness() {
      int min_murkiness = std::numeric_limits<int>::max();
      for(const Point &p : _unique_points) {
        if(p.x == 0 && p.z == 0) {
          int curr_murkiness = 0;
          for(const Point &leaf : _leaves) {
            curr_murkiness += _get_min_dist(leaf, p);
          }
          min_murkiness = std::min(min_murkiness, curr_murkiness);
        }
      }
      return min_murkiness;
    }

  private:
    void _build_branch(const std::vector<Move> &moves) {
      _curr_pos = Point{0, 0, 0};
      for(const Move &move : moves) {
        switch(move.dir) {
          case Direction::UP:
            for(int i = 0; i < move.steps; i++) {
              _curr_pos.y++;
              _unique_points.insert(_curr_pos);
            }
            _max_y = std::max(_max_y, _curr_pos.y);
            break;
          case Direction::DOWN:
            for(int i = 0; i < move.steps; i++) {
              _curr_pos.y--;
              _unique_points.insert(_curr_pos);
            }
            break;
          case Direction::RIGHT:
            for(int i = 0; i < move.steps; i++) {
              _curr_pos.x++;
              _unique_points.insert(_curr_pos);
            }
            break;
          case Direction::LEFT:
            for(int i = 0; i < move.steps; i++) {
              _curr_pos.x--;
              _unique_points.insert(_curr_pos);
            }
            break;
          case Direction::FORWARD:
            for(int i = 0; i < move.steps; i++) {
              _curr_pos.z++;
              _unique_points.insert(_curr_pos);
            }
            break;
          case Direction::BACK:
            for(int i = 0; i < move.steps; i++) {
              _curr_pos.z--;
              _unique_points.insert(_curr_pos);
            }
            break;
        }
      }
      _leaves.push_back(_curr_pos);
    }

    int _get_min_dist(const Point& leaf, const Point &trunk) {
      std::unordered_set<Point> visited;
      std::queue<std::pair<Point, int>> to_be_visited;
      to_be_visited.push({leaf, 0});

      while(!to_be_visited.empty()) {
        const std::pair<Point, int> &state = to_be_visited.front();
        to_be_visited.pop();

        const Point &position = state.first;
        const int &steps = state.second;

        if(position == trunk) { return steps; }

        if(visited.find(position) != visited.end()) { continue; }
        visited.insert(position);
        
        for(const Point &dir : DIRS) {
          int next_x = position.x + dir.x;
          int next_y = position.y + dir.y;
          int next_z = position.z + dir.z;
          Point next_position = Point{next_x, next_y, next_z};
          if(_unique_points.find(next_position) == _unique_points.end()) { continue; }
          to_be_visited.push({next_position, steps + 1});
        }
      }

      assert(false);
      return -1;
    }

    Point _curr_pos;
    std::unordered_set<Point> _unique_points;
    std::vector<Point> _leaves;
    int _max_y;
};

std::vector<Move> parse_line(const std::string &line) {
  std::vector<Move> moves;
  std::stringstream ss(line);
  std::string move;
  while(std::getline(ss, move, ',')) {
    char dir = move[0];
    int steps = std::stoi(move.substr(1));
    Direction d;
    switch(dir) {
      case 'U':
        d = Direction::UP;
        break;
      case 'D':
        d = Direction::DOWN;
        break;
      case 'L':
        d = Direction::LEFT;
        break;
      case 'R':
        d = Direction::RIGHT;
        break;
      case 'F':
        d = Direction::FORWARD;
        break;
      case 'B':
        d = Direction::BACK;
        break;
    }
    moves.push_back(Move{d, steps});
  }
  return moves;
}

std::vector<Move> parse_moves(std::ifstream &input) {
  std::string line;
  std::getline(input, line);
  std::vector<Move> moves = parse_line(line);
  return moves;
}

std::vector<std::vector<Move>> parse_moves_lists(std::ifstream &input) {
  std::vector<std::vector<Move>> moves_lists;
  std::string line;
  while(std::getline(input, line)) {
    moves_lists.push_back(parse_line(line));
  }
  return moves_lists;
}

int get_max_height(const std::vector<Move> &moves) {
  Plant p(moves);
  return p.get_max_y();
}

int count_unique_points(const std::vector<std::vector<Move>> &moves_lists) {
  Plant p(moves_lists);
  return p.get_unique_points();
}

int get_min_murkiness(const std::vector<std::vector<Move>> &moves_lists) {
  Plant p(moves_lists);
  return p.get_min_murkiness();
}

int main() {
  std::ifstream input_1("./data/q14_1.txt");
  std::ifstream input_2("./data/q14_2.txt");
  std::ifstream input_3("./data/q14_3.txt");

  std::vector<Move> moves = parse_moves(input_1);
  std::vector<std::vector<Move>> moves_lists = parse_moves_lists(input_2);
  std::vector<std::vector<Move>> moves_lists_2 = parse_moves_lists(input_3);
  
  std::cout << "Answer to part 1: " << get_max_height(moves) << std::endl;
  std::cout << "Answer to part 2: " << count_unique_points(moves_lists) << std::endl;
  std::cout << "Answer to part 3: " << get_min_murkiness(moves_lists_2) << std::endl;
  return 0;
}