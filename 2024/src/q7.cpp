#include <algorithm>
#include <iostream>
#include <fstream>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

const std::vector<std::string> TRACK = {
  "S-=++=-==++=++=-=+=-=+=+=--=-=++=-==++=-+=-=+=-=+=+=++=-+==++=++=-=-=--",
  "-                                                                     -",
  "=                                                                     =",
  "+                                                                     +",
  "=                                                                     +",
  "+                                                                     =",
  "=                                                                     =",
  "-                                                                     -",
  "--==++++==+=+++-=+=-=+=-+-=+-=+-=+=-=+=--=+++=++=+++==++==--=+=++==+++-"
};

const std::vector<std::string> TRACK_2 = {
  "S+= +=-== +=++=     =+=+=--=    =-= ++=     +=-  =+=++=-+==+ =++=-=-=--",
  "- + +   + =   =     =      =   == = - -     - =  =         =-=        -",
  "= + + +-- =-= ==-==-= --++ +  == == = +     - =  =    ==++=    =++=-=++",
  "+ + + =     +         =  + + == == ++ =     = =  ==   =   = =++=       ",
  "= = + + +== +==     =++ == =+=  =  +  +==-=++ =   =++ --= + =          ",
  "+ ==- = + =   = =+= =   =       ++--          +     =   = = =--= ==++==",
  "=     ==- ==+-- = = = ++= +=--      ==+ ==--= +--+=-= ==- ==   =+=    =",
  "-               = = = =   +  +  ==+ = = +   =        ++    =          -",
  "-               = + + =   +  -  = + = = +   =        +     =          -",
  "--==++++==+=+++-= =-= =-+-=  =+-= =-= =--   +=++=+++==     -=+=++==+++-"
};
                                                                  
typedef enum {
  ADD,
  SUB,
  EQ
} Ops;

typedef struct {
  std::vector<Ops> ops;
} Plan;

const std::vector<std::pair<int, int>> DIRS = {
  {-1, 0},
  {0, 1},
  {1, 0},
  {0, -1}
};

bool is_in_bounds(int i, int j, int height, int width) {
  return i >= 0 && i < height && j >= 0 && j < width;
}

std::string parse_track(std::vector<std::string> track) {
  int height = static_cast<int>(track.size());
  int width = static_cast<int>(track[0].size());

  std::string flat_track;
  std::unordered_set<std::string> visited;
  visited.insert("0-0");
  std::queue<std::pair<int, int>> to_be_visited;
  to_be_visited.push({0, 1});

  while(!to_be_visited.empty()) {
    std::pair<int, int> pos = to_be_visited.front();
    to_be_visited.pop();

    std::string hash = std::to_string(pos.first) + "-" + std::to_string(pos.second);
    visited.insert(hash);
    
    flat_track += track[pos.first][pos.second];
    for(auto &dir : DIRS) {
      int next_i = pos.first + dir.first;
      int next_j = pos.second + dir.second;
      if(!is_in_bounds(next_i, next_j, height, width)) { continue; }
      if(track[next_i][next_j] == ' ') { continue; }
      std::string next_hash = std::to_string(next_i) + "-" + std::to_string(next_j);
      if(visited.find(next_hash) != visited.end()) { continue; }
      to_be_visited.push({next_i, next_j});
    }
  }
  flat_track += 'S';
  return flat_track;
}

std::unordered_map<std::string, Plan> parse_plans(std::ifstream &input) {
  std::unordered_map<std::string, Plan> plans;
  std::string line;
  while(std::getline(input ,line)) {
    std::stringstream ss(line);
    std::string name;
    std::getline(ss, name, ':');

    std::vector<Ops> ops;
    std::string op;
    while(std::getline(ss, op, ',')) {
      if(op == "+") {
        ops.push_back(Ops::ADD);
      } else if(op == "-") {
        ops.push_back(Ops::SUB);
      } else {
        ops.push_back(Ops::EQ);
      }
    }
    plans.insert({name, Plan{ops}});
  }
  return plans;
}

int get_score(const Plan &plan) {
  int score = 0;
  int curr_val = 10;
  size_t index = 0;
  for(int _ = 0; _ < 10; _++) {
    switch(plan.ops[index]) {
      case Ops::ADD:
        curr_val++;
        break;
      case Ops::SUB:
        curr_val--;
        break;
      case Ops::EQ:
        break;
    }
    score += curr_val;
    index++;
    if(index == plan.ops.size()) { index = 0; }
  }
  return score;
}

std::string get_plan_ranking(const std::unordered_map<std::string, Plan> &plans) {
  std::vector<std::pair<std::string, int>> scores;
  for(auto &plan : plans) {
    int score = get_score(plan.second);
    scores.push_back({plan.first, score});
  }
  std::sort(scores.begin(), scores.end(), []
    (std::pair<std::string, int> a, std::pair<std::string, int> b) { 
      return a.second > b.second; 
    }
  );
  std::stringstream ss;
  for(auto &score : scores) {
    ss << score.first;
  }
  return ss.str();
}

int get_score_with_track(const Plan &plan, const std::string &track, int laps) {
  int score = 0;
  int curr_val = 10;
  int curr_laps = 0;
  size_t index = 0;
  size_t track_index = 0;
  while(curr_laps != laps) {
    switch(track[track_index]) {
      case '+':
        curr_val++;
        break;
      case '-':
        curr_val--;
        break;
      case '=':
      case 'S':
        switch(plan.ops[index]) {
          case Ops::ADD:
            curr_val++;
            break;
          case Ops::SUB:
            curr_val--;
            break;
          case Ops::EQ:
            break;
        }
        break;
    }
    score += curr_val;
    index++;
    track_index++;
    if(index == plan.ops.size()) { index = 0; }
    if(track_index == track.size()) {
      track_index = 0;
      curr_laps++;
    }
  }
  return score;
}


std::string get_plan_ranking_with_track(const std::unordered_map<std::string, Plan> &plans,
  const std::string &track) 
{
  std::vector<std::pair<std::string, int>> scores;
  for(auto &plan : plans) {
    int score = get_score_with_track(plan.second, track, 10);
    scores.push_back({plan.first, score});
  }
  std::sort(scores.begin(), scores.end(), []
    (std::pair<std::string, int> a, std::pair<std::string, int> b) { 
      return a.second > b.second; 
    }
  );
  std::stringstream ss;
  for(auto &score : scores) {
    ss << score.first;
  }
  return ss.str();
}

std::vector<Plan> get_unique_permutations(int plus, int sub, int eq, std::vector<Ops> curr_perm) {
  if(plus == 0 && sub == 0 && eq == 0) { return {Plan{curr_perm}}; }

  std::vector<Plan> perms;
  if(plus > 0) {
    std::vector<Ops> next_perm = curr_perm;
    next_perm.push_back(Ops::ADD);
    std::vector<Plan> child_perms = get_unique_permutations(plus - 1, sub, eq, next_perm);
    perms.insert(perms.end(), child_perms.begin(), child_perms.end());
  }

  if(sub > 0) {
    std::vector<Ops> next_perm = curr_perm;
    next_perm.push_back(Ops::SUB);
    std::vector<Plan> child_perms = get_unique_permutations(plus, sub - 1, eq, next_perm);
    perms.insert(perms.end(), child_perms.begin(), child_perms.end());
  }

  if(eq > 0) {
    std::vector<Ops> next_perm = curr_perm;
    next_perm.push_back(Ops::EQ);
    std::vector<Plan> child_perms = get_unique_permutations(plus, sub, eq - 1, next_perm);
    perms.insert(perms.end(), child_perms.begin(), child_perms.end());
  }
  return perms;
}

int count_winning_plans(const std::unordered_map<std::string, Plan> &plans,
  const std::string &track)
{
  const Plan &rival_plan = plans.find("A")->second;
  /* Can test with 11 laps instead of 2024, since the score increases by same ammount every 11 laps */
  int baseline_score = get_score_with_track(rival_plan, track, 11);
  std::vector<Plan> permutations = get_unique_permutations(5, 3, 3, {});
  int winning_plans = 0;
  for(Plan &plan : permutations) {
    winning_plans += (get_score_with_track(plan, track, 11) > baseline_score);
  }
  return winning_plans;
}

int main() {
  std::ifstream input_1("./data/q7_1.txt");
  std::ifstream input_2("./data/q7_2.txt");
  std::ifstream input_3("./data/q7_3.txt");

  std::unordered_map<std::string, Plan> plans = parse_plans(input_1);
  std::unordered_map<std::string, Plan> plans_2 = parse_plans(input_2);
  std::unordered_map<std::string, Plan> plans_3 = parse_plans(input_3);

  std::string flat_track = parse_track(TRACK);
  std::string flat_track_2 = parse_track(TRACK_2);
  
  std::cout << "Answer to part 1: " << get_plan_ranking(plans) << std::endl;
  std::cout << "Answer to part 2: " << get_plan_ranking_with_track(plans_2, flat_track) << std::endl;
  std::cout << "Answer to part 3: " << count_winning_plans(plans_3, flat_track_2) << std::endl;
  return 0;
}