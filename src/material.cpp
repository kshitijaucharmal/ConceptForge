#include "material.hpp"

Material::Material(ShaderType type) {
    InitWhiteTexture();
    // Only load the specified type
    InitShader(type);
}

void Material::SetDrawMode(DrawMode mode) {
    shader->SetDrawMode(mode);
}

void Material::InitShader(ShaderType type){
    switch(type){
        case ShaderType::Lit:
            InitDefaultLit();
            break;
        case ShaderType::Unlit:
            InitDefaultUnlit();
            break;
        case ShaderType::Light:
            InitDefaultLight();
            break;
    }
}

void Material::ApplyProjection(Projection& projection) {
    projection.UpdateShader(*shader);
}

void Material::InitDefaultLit(){
    // Lit Shader
    std::shared_ptr<ShaderProgram> litShader = std::make_shared<ShaderProgram>();
    litShader->Init(DrawMode::FILLED, Const::litVert, Const::litFrag);

    litShader->Use();
    // unsigned int container2Tex = litShader->LoadTexture(TEXTURE_DIR "/container2.png", "material.diffuseMap", 0, true);
    // unsigned int container2_specularTex = litShader->LoadTexture(TEXTURE_DIR "/container2_specular.png", "material.specularMap", 1, false);
    litShader->setFloat("material.shininess", 32.0f);

    shader = std::move(litShader);
}

void Material::InitDefaultUnlit(){
    // Unlit Shader
    std::shared_ptr<ShaderProgram> unlitShader = std::make_shared<ShaderProgram>();
    unlitShader->Init(DrawMode::FILLED, Const::unlitVert, Const::unlitFrag);

    shader = std::move(unlitShader);
}

void Material::InitDefaultLight(){
    // Light Shader (Shader for a light emitting object)
    std::shared_ptr<ShaderProgram> lightShader = std::make_shared<ShaderProgram>();
    lightShader->Init(DrawMode::FILLED, Const::lightVert, Const::lightFrag);
    shader = std::move(lightShader);
}


void Material::InitWhiteTexture() {
    // Binding white Texture
    glGenTextures(1, &whiteTexture);
    glBindTexture(GL_TEXTURE_2D, whiteTexture);

    unsigned char whitePixel[3] = {125, 125, 125};
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, whitePixel);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Use
    // glActiveTexture(GL_TEXTURE0);
    // glBindTexture(GL_TEXTURE_2D, whiteTexture);
    // glUniform1i(glGetUniformLocation(shader.shaderID, "material.diffuseMap"), 0);
}

