#include <iostream>
#include <fstream>
#include <string>
#include <vector>

typedef unsigned long ul;

int build_pyramid(int blocks) {
  blocks--;
  int width = 1;
  int blocks_required = 1;
  while(blocks >= 0) {
    width += 2;
    blocks_required += 2;
    blocks -= blocks_required;
  }
  return width * std::abs(blocks);
}

int build_shrine(int priests) {
  int blocks = 20240000;
  int acolytes = 1111;

  blocks--;
  int width = 1;
  int thickness = 1;
  while(blocks >= 0) {
    width += 2;
    thickness = (thickness * priests) % acolytes;
    blocks -= (width * thickness);
  }
  return width * std::abs(blocks);
}

inline ul get_width(const std::vector<ul> &cols) {
  return ((static_cast<ul>(cols.size()) - 1) * 2) + 1;
}

ul build_shrine_with_space(ul high_priests) {
  ul blocks = 202400000;
  ul high_acolytes = 10;

  ul blocks_used = 1;
  std::vector<ul> cols = {1};
  ul thickness = 1;

  while(blocks >= blocks_used) {
    thickness = ((thickness * high_priests) % high_acolytes) + high_acolytes;
    for(ul &height : cols) {
      height += thickness;
    }
    cols.push_back(thickness);

    ul width = get_width(cols);
    ul blocks_removed = 0;
    for(size_t i = 0; i < cols.size() - 1; i++) {
      blocks_removed += (((width * high_priests) * cols[i]) % high_acolytes) * (i == 0 ? 1 : 2);
    }

    blocks_used = 0;
    for(size_t i = 0; i < cols.size(); i++) {
      blocks_used += cols[i] * (i == 0 ? 1 : 2);
    }
    blocks_used -= blocks_removed;
  }

  return blocks_used - blocks;
}

int main() {
  std::ifstream input_1("./data/q8_1.txt");
  std::ifstream input_2("./data/q8_2.txt");
  std::ifstream input_3("./data/q8_3.txt");
  std::string line;

  std::getline(input_1, line);
  int blocks = std::stoi(line);
  
  std::getline(input_2, line);
  int priests = std::stoi(line);

  std::getline(input_3, line);
  ul high_priests = std::stoul(line);

  std::cout << "Answer to part 1: " << build_pyramid(blocks) << std::endl;
  std::cout << "Answer to part 2: " << build_shrine(priests) << std::endl;
  std::cout << "Answer to part 3: " << build_shrine_with_space(high_priests) << std::endl;
  return 0;
}