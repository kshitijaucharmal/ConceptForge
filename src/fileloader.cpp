#include "fileloader.hpp"

FileLoader::FileLoader(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    file.seekg(0, std::ios::end);
    size_t size = file.tellg();
    file.seekg(0, std::ios::beg);

    m_data.resize(size + 1); // +1 for null-terminator
    file.read(m_data.data(), size);
    m_data[size] = '\0'; // null-terminate
}

const char* FileLoader::getData() const {
    return m_data.empty() ? nullptr : m_data.data();
}

