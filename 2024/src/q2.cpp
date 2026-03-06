#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

std::vector<std::string> parse_words(std::string line) {
  line = line.substr(line.find(':') + 1);
  std::vector<std::string> words;

  size_t pos;
  while((pos = line.find(',')) != std::string::npos) {
    words.push_back(line.substr(0, pos));
    line = line.substr(pos + 1);
  }
  words.push_back(line);
  return words;
}

int count_matches(const std::string &word, std::string line) {
  size_t pos = 0;
  int count = 0;
  while((pos = line.find(word)) != std::string::npos) {
    count++;
    line = line.substr(pos + 1);
  }
  return count;
}

int count_word_occurences(const std::vector<std::string> &words, const std::string &line) {
  int count = 0;
  for(const std::string &word : words) {
    std::string curr_line = line;
    count += count_matches(word, curr_line);
  }
  return count;
}

void count_symbol_matches(const std::string &word, std::string line, std::vector<bool> &is_word) {
  size_t index = 0;
  size_t pos = 0;
  while((pos = line.find(word)) != std::string::npos) {
    index += pos;
    for(size_t i = 0; i < word.size(); i++) {
      is_word[index + i] = true;
    }
    line = line.substr(pos + 1);
    index++;
  }
}

int count_symbol_occurances(const std::vector<std::string> &words, const std::vector<std::string> &lines) {
  int count = 0;
  for(const std::string &line : lines) {
    std::vector<bool> is_word(line.size(), false);
    for(const std::string &word : words) {
      std::string curr_line = line;
      count_symbol_matches(word, curr_line, is_word);
    }

    std::reverse(is_word.begin(), is_word.end());
    for(const std::string &word : words) {
      std::string curr_line = line;
      std::reverse(curr_line.begin(), curr_line.end());
      count_symbol_matches(word, curr_line, is_word);
    }

    for(bool b : is_word) {
      if(b) { count += 1; }
    }
  }
  return count;
}

void check_up(const std::vector<std::string> &words, const std::vector<std::vector<char>> & lines,
  std::vector<std::vector<bool>> &is_word_grid, int i, int j)
{
  for(const std::string &word : words) {
    if(word.size() - 1 > static_cast<size_t>(i)) { continue; }
    std::string curr_word;
    for(int di = 0; di < static_cast<int>(word.size()); di++) {
      curr_word += lines[i - di][j];
    }
    if(word == curr_word) {
      for(int di = 0; di < static_cast<int>(word.size()); di++) {
        is_word_grid[i - di][j] = true;
      }
    }
  }
}

void check_down(const std::vector<std::string> &words, const std::vector<std::vector<char>> & lines,
  std::vector<std::vector<bool>> &is_word_grid, int i, int j)
{
  for(const std::string &word : words) {
    if(word.size() - 1 > lines.size() - 1 - static_cast<size_t>(i)) { continue; }
    std::string curr_word;
    for(int di = 0; di < static_cast<int>(word.size()); di++) {
      curr_word += lines[i + di][j];
    }
    if(word == curr_word) {
      for(int di = 0; di < static_cast<int>(word.size()); di++) {
        is_word_grid[i + di][j] = true;
      }
    }
  }
}

void check_left(const std::vector<std::string> &words, const std::vector<std::vector<char>> & lines,
  std::vector<std::vector<bool>> &is_word_grid, int i, int j)
{
  for(const std::string &word : words) {
    std::string curr_word;
    for(int dj = 0; dj < static_cast<int>(word.size()); dj++) {
      int next_j = j - dj;
      if(next_j < 0) { next_j += lines[0].size(); }
      curr_word += lines[i][next_j];
    }
    if(word == curr_word) {
      for(int dj = 0; dj < static_cast<int>(word.size()); dj++) {
        int next_j = j - dj;
        if(next_j < 0) { next_j += lines[0].size(); }
        is_word_grid[i][next_j] = true;
      }
    }
  }
}

void check_right(const std::vector<std::string> &words, const std::vector<std::vector<char>> & lines,
  std::vector<std::vector<bool>> &is_word_grid, int i, int j)
{
  for(const std::string &word : words) {
    std::string curr_word;
    for(int dj = 0; dj < static_cast<int>(word.size()); dj++) {
      curr_word += lines[i][(j + dj) % lines[0].size()];
    }
    if(word == curr_word) {
      for(int dj = 0; dj < static_cast<int>(word.size()); dj++) {
        is_word_grid[i][(j + dj) % lines[0].size()] = true;
      }
    }
  }
}


int count_symbol_occurances_grid(
  const std::vector<std::string> &words, 
  const std::vector<std::vector<char>> &lines)
{
  int count = 0;
  int height = static_cast<int>(lines.size());
  int width = static_cast<int>(lines[0].size());
  std::vector<std::vector<bool>> is_word_grid(height, std::vector<bool>(width, false));

  for(int i = 0; i < height; i++) {
    for(int j = 0; j < width; j++) {
      check_up(words, lines, is_word_grid, i, j);
      check_left(words, lines, is_word_grid, i, j);
      check_down(words, lines, is_word_grid, i, j);
      check_right(words, lines, is_word_grid, i, j);
    }
  }

  for(auto row : is_word_grid) {
    for(auto col : row) {
      if(col) { count++; }
    }
  }

  return count;
}


int main() {
  std::ifstream input_1("./data/q2_1.txt");
  std::ifstream input_2("./data/q2_2.txt");
  std::ifstream input_3("./data/q2_3.txt");
  std::string line;

  std::getline(input_1, line);
  std::vector<std::string> words = parse_words(line);
  std::getline(input_1, line);
  std::string line_1;
  std::getline(input_1, line_1);

  std::getline(input_2, line);
  std::vector<std::string> words_2 = parse_words(line);
  std::getline(input_2, line);
  std::vector<std::string> lines;
  while(std::getline(input_2, line)) {
    lines.push_back(line);
  }

  std::getline(input_3, line);
  std::vector<std::string> words_3 = parse_words(line);
  std::getline(input_3, line);
  std::vector<std::vector<char>> lines_2;
  while(std::getline(input_3, line)) {
    lines_2.push_back(std::vector<char>(line.begin(), line.end()));
  }

  std::cout << "Answer to part 1: " << count_word_occurences(words, line_1) << std::endl;
  std::cout << "Answer to part 2: " << count_symbol_occurances(words_2, lines) << std::endl;
  std::cout << "Answer to part 3: " << count_symbol_occurances_grid(words_3, lines_2) << std::endl;
  return 0;
}