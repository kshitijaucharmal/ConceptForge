
#include "textureman.hpp"

// Texturepath is relative to TEXTURE_DIR for now
uint TextureManager::LoadTexture(
    const char* texturePath,
    bool flip
){
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    // set the textureID wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(flip);
    int width, height, nrChannels;
    unsigned char *data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
    if (data) {
        // If Alpha channel exists
        GLenum inputFormat = (nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, inputFormat, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else{
        std::cerr << "Failed to load textureID\n";
        return 0; // 0 is invalid for texture IDs
    }
    stbi_image_free(data);

    return textureID;
}

void TextureManager::BindTexture(uint shaderID, uint textureID, uint textureLoc, const char* textureShaderName ){
    if (textureLoc >= GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS) {
        std::cerr << "Texture unit " << textureLoc << " exceeds GPU limit.\n";
        return;
    }
    glUseProgram(shaderID);
    glActiveTexture(GL_TEXTURE0 + textureLoc);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glUniform1i(glGetUniformLocation(shaderID, textureShaderName), textureLoc);
}
