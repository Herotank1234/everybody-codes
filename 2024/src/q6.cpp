#include <iostream>
#include <fstream>
#include <limits>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

const std::string ROOT = "RR";

typedef enum {
  FULL,
  FIRST_LETTER
} Format;

typedef struct {
  bool has_apple;
  std::vector<std::string> children;
} Node;

std::unordered_map<std::string, Node> parse_nodes(std::ifstream &input) {
  std::unordered_map<std::string, Node> nodes;
  std::string line;
  while(std::getline(input, line)) {
    std::stringstream ss(line);
    std::string name;
    bool has_apple = false;
    std::string child;
    std::vector<std::string> children;

    std::getline(ss, name, ':');
    while(std::getline(ss, child, ',')) {
      if(child == "@") {
        has_apple = true;
      } else {
        children.push_back(child);
      }
    }
    nodes[name] =  Node{has_apple, children};
  }
  return nodes;
}

std::vector<std::vector<std::string>> get_paths(const std::unordered_map<std::string, Node> &nodes,
  std::string node_name, std::vector<std::string> curr_path)
{
  curr_path.push_back(node_name);

  std::vector<std::vector<std::string>> paths;
  auto curr_node_it = nodes.find(node_name);
  if(curr_node_it == nodes.end()) {
    return paths;
  }
  const Node &curr_node = curr_node_it->second;

  if(curr_node.has_apple) { 
    auto path = curr_path;
    path.push_back("@");
    paths.push_back(path);
  }

  for(auto &child : curr_node.children) {
    if(child == "BUG") { continue; }
    if(child == "ANT") { continue; }
    std::vector<std::vector<std::string>> child_paths = get_paths(nodes, child, curr_path);
    paths.insert(paths.end(), child_paths.begin(), child_paths.end());
  }

  return paths;
}

std::string get_unique_length_path(const std::unordered_map<std::string, Node> &nodes, Format format) {
  std::vector<std::vector<std::string>> paths = get_paths(nodes, ROOT, {});
  std::unordered_map<size_t, int> length_freq;

  for(std::vector<std::string> &path : paths) {
    length_freq[path.size()]++;
  }

  size_t unique_length = 0;
  for(auto &kv : length_freq) {
    if(kv.second == 1) {
      unique_length = kv.first;
      break;
    }
  }

  std::vector<std::string> unique_path;
  for(auto &path : paths) {
    if(path.size() == unique_length) {
      unique_path = path;
      break;
    }
  }

  std::stringstream ss;
  switch(format) {
    case Format::FULL:
      for(auto &node : unique_path) {
        ss << node;
      }
      break;
    case Format::FIRST_LETTER:
      for(auto &node : unique_path) {
        ss << node[0];
      }
      break;
  }
  return ss.str();
}

int main() {
  std::ifstream input_2("./data/q6_2.txt");
  std::ifstream input_1("./data/q6_1.txt");
  std::ifstream input_3("./data/q6_3.txt");
  
  std::unordered_map<std::string, Node> nodes = parse_nodes(input_1);
  std::unordered_map<std::string, Node> nodes_2 = parse_nodes(input_2);
  std::unordered_map<std::string, Node> nodes_3 = parse_nodes(input_3);
  
  std::cout << "Answer to part 1: " << get_unique_length_path(nodes, Format::FULL) << std::endl;
  std::cout << "Answer to part 2: " << get_unique_length_path(nodes_2, Format::FIRST_LETTER) << std::endl;
  std::cout << "Answer to part 3: " << get_unique_length_path(nodes_3, Format::FIRST_LETTER) << std::endl;
  return 0;
}