//
// Created by tomas on 20.9.22.
//

#pragma once


#include <array>
#include "../SceneObject.h"
#include "Shader.h"
#include "Program.h"

namespace Galax::Renderer::SceneObjects{
    /**
     * @brief A simple class that provides a full screen quad mesh for rendering purposes.
     */
    class Quad : public SceneObject {
    public:
        Quad();
        explicit Quad(const std::string& name);
        ~Quad();

        unsigned int getId() override;

        void setFragShader(std::shared_ptr<IShader> shader);
        [[nodiscard]] std::shared_ptr<Program> getProgram() const;
        void draw();

    private:

        std::shared_ptr<IShader> fragShader;
        std::shared_ptr<Program> program;

        inline static std::shared_ptr<Shader> vertexShader = nullptr;
        inline static unsigned int quadVAO = 0;
        inline static unsigned int quadVBO = 0;
        inline static bool initialized = false;

        static void init();

        static inline std::array<float, 16> vertices = {
                -1.0f, -1.0f, 0.0f,
                1.0f, -1.0f, 0.0f,
                -1.0f, 1.0f, 0.0f,
                1.0f, 1.0f, 0.0f
        };

        static inline std::string vertexShaderSource = R"(
        #version 430 core
        layout(location = 0) in vec3 position;

        out vec2 texCoords;

        void main() {
            gl_Position = vec4(position, 1.0);
            texCoords = vec2(position.x + 1, position.y + 1) / 2.0;
        })";
    };
}
