#include "asset_browser.hpp"

namespace fs = std::filesystem;

using namespace Editor;

// Store texture ID per file path
std::unordered_map<std::string, GLuint> g_ThumbnailCache;

// WARNING Not used, was for showing thumbnails
GLuint AssetBrowser::LoadTexture(const std::string& filePath) {
    int width, height, channels;
    unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &channels, 4);
    if (!data) return 0;

    // Save previous texture binding
    GLint prevTexture;
    glGetIntegerv(GL_TEXTURE_BINDING_2D, &prevTexture);

    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(data);

    // Restore previous binding
    glBindTexture(GL_TEXTURE_2D, prevTexture);

    return texID;
}

AssetBrowser::AssetBrowser(){
    // empty constructor
}

// Show
void AssetBrowser::Show(std::string& currentDirectory) {
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(Const::assetBrowserWidth, Const::HEIGHT - Const::consoleHeight), ImGuiCond_Always);
    ImGui::Begin("Asset Browser", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    // Static char buffer (resizable with std::vector)
    static std::vector<char> pathBuffer(currentDirectory.begin(), currentDirectory.end());
    pathBuffer.push_back('\0'); // Null-terminate once

    // Sync buffer if path was changed externally
    if (pathBuffer.data() != currentDirectory.c_str()) {
        pathBuffer.assign(currentDirectory.begin(), currentDirectory.end());
        pathBuffer.push_back('\0');
    }

    // Path Input
    if (ImGui::InputText("Path", pathBuffer.data(), pathBuffer.size(), ImGuiInputTextFlags_EnterReturnsTrue)) {
        std::string newPath(pathBuffer.data());
        if (fs::exists(newPath) && fs::is_directory(newPath)) {
            currentDirectory = newPath;
        }
    }

    ImGui::Separator();

    // Up one directory
    if (ImGui::Button(".. (Up)")) {
        fs::path parent = fs::path(currentDirectory).parent_path();
        if (!parent.empty() && fs::exists(parent)) {
            currentDirectory = parent.string();
            pathBuffer.assign(currentDirectory.begin(), currentDirectory.end());
            pathBuffer.push_back('\0');
        }
    }

    ImGui::Separator();

    // Collect entries
    std::vector<fs::directory_entry> entries;
    for (const auto& entry : fs::directory_iterator(currentDirectory)) {
        // Skip hidden files (dotfiles)
        const std::string filename = entry.path().filename().string();
        if (!filename.empty() && filename[0] == '.') continue;
        entries.push_back(entry);
    }

    // Sort: folders first, then files, both alphabetically
    std::sort(entries.begin(), entries.end(), [](const fs::directory_entry& a, const fs::directory_entry& b) {
        if (a.is_directory() != b.is_directory())
            return a.is_directory() > b.is_directory(); // folders before files
            return a.path().filename().string() < b.path().filename().string(); // alphabetical
    });

    // Draw entries
    int index = 0;
    for (const auto& entry : entries) {
        const fs::path& path = entry.path();
        const std::string name = path.filename().string();

        // Alternating background color
        ImVec2 itemStart = ImGui::GetCursorScreenPos();
        ImVec2 itemEnd = ImVec2(itemStart.x + ImGui::GetContentRegionAvail().x, itemStart.y + ImGui::GetTextLineHeightWithSpacing());
        ImU32 bgColor = (index % 2 == 0) ? IM_COL32(150, 150, 150, 50) : IM_COL32(255, 255, 255, 50);
        ImGui::GetWindowDrawList()->AddRectFilled(itemStart, itemEnd, bgColor);

        ImGui::PushID(name.c_str());

        if (entry.is_directory()) {
            if (ImGui::Selectable((std::string(ICON_FA_FOLDER) + " " + name).c_str(), false, 0, ImVec2(0, ImGui::GetTextLineHeightWithSpacing()))) {
                currentDirectory = path.string();
                pathBuffer.assign(currentDirectory.begin(), currentDirectory.end());
                pathBuffer.push_back('\0');
            }
        } else {
            ImGui::Selectable((std::string(ICON_FA_FILE) + " " + name).c_str(), false, 0, ImVec2(0, ImGui::GetTextLineHeightWithSpacing()));
        }

        ImGui::PopID();
        index++;
    }

    ImGui::End();
}
