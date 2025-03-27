#include <fstream>
#include <iostream>
#include <string>

class File {
  std::ofstream file;

public:
  File(const std::string &filename) {
    file.open(filename);
    if (!file.is_open()) {
      throw std::runtime_error("Failed to open file");
    }
  }

  void write(const std::string &text) { file << text; }

  ~File() {
    if (file.is_open()) {
      file.close();
      std::cout << "File closed\n";
    }
  }
};

int main() {
  try {
    File f("example.txt");
    f.write("Hello, RAII!");
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
  }
  return 0;
}
