//
// Created by tomas on 14.9.22.
//


#pragma once

#include "SceneObject.h"
#include "scene_objects/Shader.h"
#include "scene_objects/Texture.h"
#include "scene_objects/Camera.h"
#include "scene_objects/Light.h"
#include "scene_objects/Program.h"
#include "GBuffer.h"

#include <array>
#include <memory>
#include <vector>

namespace Galax::Renderer {

    using namespace Galax::Renderer::SceneObjects;

    class LightingModel : public SceneObject {
    public:
        LightingModel();
        ~LightingModel();

        uint getId() override;

        void init();

        void draw();

        void unbind();

        void setLightningShader(std::shared_ptr<Shader> shader);

        void addTexture(std::shared_ptr<Texture> texture);

        void addUniform(std::shared_ptr<Uniform> uniform);

    private:
        unsigned int quadVAO = 0;
        unsigned int quadVBO = 0;

        std::shared_ptr<Shader> vertexShader;
        std::shared_ptr<Shader> lightingShader;
        std::shared_ptr<Program> lightingProgram;
        std::vector<std::shared_ptr<Texture>> textures;
        std::vector<std::shared_ptr<Uniform>> uniforms;

        std::array<float, 16> vertices = {
                -1.0f, -1.0f, 0.0f,
                1.0f, -1.0f, 0.0f,
                -1.0f, 1.0f, 0.0f,
                1.0f, 1.0f, 0.0f
        };


        std::string vertexShaderSource = R"(
        #version 430 core
        layout(location = 0) in vec3 position;

        out vec2 texCoords;

        void main() {
            gl_Position = vec4(position, 1.0);
            texCoords = vec2(position.x + 1, position.y + 1) / 2.0;
        })";

    };

}
