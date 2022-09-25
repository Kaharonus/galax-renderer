//
// Created by tomas on 20.9.22.
//

#pragma once

#include "SceneObject.h"
#include "scene_objects/Texture.h"
#include "scene_objects/Quad.h"
#include "scene_objects/Shader.h"
#include "scene_objects/FrameBuffer.h"

namespace Galax::Renderer {
    using namespace Galax::Renderer::SceneObjects;

    class PostProcessEffect : public SceneObject {
    public:
        PostProcessEffect();
        explicit PostProcessEffect(const std::string& name);

        unsigned int getId() override;
        void draw();
        [[nodiscard]] std::vector<std::shared_ptr<Texture>> getOutputTextures() const;
        [[nodiscard]] std::vector<std::shared_ptr<Texture>> getInputTextures() const;
        [[nodiscard]] std::shared_ptr<Shader> getShader() const;
        void addInputTexture(std::shared_ptr<Texture> texture);
        void addOutputTexture(std::shared_ptr <Texture> texture);
        void setShader(std::shared_ptr<Shader> shader);
        void resize(int width, int height);
        void addUniform(std::shared_ptr<Uniform> uniform);
        void setCallCount(int count);
        [[nodiscard]] int getCallCount() const;
        [[nodiscard]] std::shared_ptr<FrameBuffer> getFrameBuffer() const;

    private:
        void init();

        int callCount = 1;
        bool initialized = false;
        std::shared_ptr<Shader> shader;
        std::shared_ptr<Quad> quad;
        std::shared_ptr<FrameBuffer> frameBuffer;
        std::shared_ptr<FrameBuffer> secondFrameBuffer;
        std::vector<std::shared_ptr<Texture>> inputTextures;
        std::vector<std::shared_ptr<Texture>> outputTextures;
        std::vector<std::shared_ptr<Uniform>> uniforms;
        std::shared_ptr<Uniform> callCountUniform;

        std::unordered_map<std::shared_ptr<Texture>, std::tuple<std::shared_ptr<Texture>, std::shared_ptr<Texture>>> temporaryTextures;

        void drawMultiple();

        void drawSingle();
    };
}