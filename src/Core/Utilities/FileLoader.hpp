#pragma once

#include <fstream>
#include <iostream>
#include <vector>
#include <string>

class FileLoader {
public:
    FileLoader(const std::string& filename);
    const char* getData() const;

private:
    std::vector<char> m_data;
};
