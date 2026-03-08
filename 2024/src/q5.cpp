#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

typedef unsigned long ul;

class Dance {
  public:
    Dance(std::vector<std::vector<int>> cols) : _index(0), _cols(cols) {}

    std::string simulate_round() {
      size_t to_be_moved = static_cast<size_t>(_cols[_index].front());
      _cols[_index].erase(_cols[_index].begin());

      _index++;
      if(_index == _cols.size()) { _index = 0; }

      size_t insert_index = (to_be_moved - 1) % (_cols[_index].size() * 2);
      if(insert_index > _cols[_index].size()) {
        insert_index = ((2 * _cols[_index].size()) - insert_index) ;
      }

      _cols[_index].insert(_cols[_index].begin() + insert_index, to_be_moved);

      std::stringstream ss;
      for(auto &col : _cols) {
        ss << col.front();
      }
      return ss.str();
    }
    
    std::string get_state() {
      std::stringstream ss;
      for(auto &col : _cols) {
        for(auto &val : col) {
          ss << val << ",";
        }
        ss << ",";
      }
      return ss.str();
    }

  private:
    size_t _index;
    std::vector<std::vector<int>> _cols;
};

Dance parse_dance(std::ifstream &input) {
  std::string line;
  std::vector<std::vector<int>> cols;
  bool first = true;

  while(std::getline(input, line)) {
    std::stringstream ss(line);
    std::string val_str;
    int index = 0;
    while(std::getline(ss, val_str, ' ')) {
      if(first) {
        cols.push_back({std::stoi(val_str)});
      } else {
        cols[index++].push_back(std::stoi(val_str));
      }
    }
    if(first) { first = false; }
  }

  Dance dance(cols);
  return dance;
}

std::string simulate_ten_rounds(Dance &dance) {
  std::string call;
  for(int _ = 0; _ < 10; _++) {
    call = dance.simulate_round();
  }
  return call;
}

ul simulate_until_2024_calls(Dance &dance) {
  ul rounds = 0;
  std::string call;
  std::unordered_map<std::string, ul> call_freq;

  while(true) {
    call = dance.simulate_round();
    rounds++;
    call_freq[call]++;
    if(call_freq[call] == 2024) { break; }
  }

  return std::stoul(call) * rounds;
}

ul simulate_and_get_largest_call(Dance &dance) {
  std::unordered_set<std::string> states_seen;
  std::string call;
  ul highest_call = 0;
  while(true) {
    call = dance.simulate_round();
    highest_call = std::max(highest_call, std::stoul(call));
    std::string state = dance.get_state();
    if(states_seen.find(state) != states_seen.end()) { break; }
    states_seen.insert(state);
  }
  return highest_call;
}

int main() {
  std::ifstream input_1("./data/q5_1.txt");
  std::ifstream input_2("./data/q5_2.txt");
  std::ifstream input_3("./data/q5_3.txt");

  Dance dance = parse_dance(input_1);
  Dance dance_2 = parse_dance(input_2);
  Dance dance_3 = parse_dance(input_3);

  std::cout << "Answer to part 1: " << simulate_ten_rounds(dance) << std::endl;
  std::cout << "Answer to part 2: " << simulate_until_2024_calls(dance_2) << std::endl;
  std::cout << "Answer to part 3: " << simulate_and_get_largest_call(dance_3) << std::endl;
  return 0;
}