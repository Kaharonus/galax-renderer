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
        std::vector<std::shared_ptr<Texture>> getOutputTextures() const;
        std::vector<std::shared_ptr<Texture>> getInputTextures() const;
        std::shared_ptr<Shader> getShader() const;
        void addInputTexture(std::shared_ptr<Texture> texture);
        void addOutputTexture(std::shared_ptr <Texture> texture);
        void setShader(std::shared_ptr<Shader> shader);
        void resize(int width, int height);

    private:
        bool initialized = false;

        std::shared_ptr<Shader> shader;
        std::shared_ptr<Quad> quad;
        std::shared_ptr<FrameBuffer> frameBuffer;
        std::vector<std::shared_ptr<Texture>> inputTextures;
        std::vector<std::shared_ptr<Texture>> outputTextures;

        void init();
    };
}