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
#include "scene_objects/Quad.h"
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

        void setLightningShader(std::shared_ptr<Shader> shader);

        void addTexture(std::shared_ptr<Texture> texture);

        void addUniform(std::shared_ptr<Uniform> uniform);

        std::vector<std::shared_ptr<Uniform>> getUniforms();
        std::vector<std::shared_ptr<Texture>> getTextures();

        std::shared_ptr<Shader> getLightingShader();

        void addOutputTexture(std::shared_ptr<Texture> texture);

        void resize(int width, int height);



    private:
        int width = 1;
        int height = 1;

        std::shared_ptr<Shader> lightingShader;
        std::shared_ptr<Quad> quad;
        std::vector<std::shared_ptr<Texture>> textures;
        std::vector<std::shared_ptr<Uniform>> uniforms;

        std::vector<std::shared_ptr<Texture>> outputTextures;
        std::shared_ptr<FrameBuffer> outputFrameBuffer;

    };

}
