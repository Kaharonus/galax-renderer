//
// Created by tomas on 14.9.22.
//


#pragma once

#include <renderer/SceneObject.h>
#include <renderer/impl/Shader.h>
#include <renderer/impl/Texture.h>
#include <renderer/impl/Camera.h>
#include <renderer/impl/Light.h>
#include <renderer/impl/Program.h>
#include <renderer/impl/Quad.h>
#include <renderer/impl/SSBO.h>

#include <renderer/GBuffer.h>

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

        void setLightningShader(std::shared_ptr<IShader> shader);

		std::shared_ptr<FrameBuffer> getFrameBuffer() const;

        void addTexture(std::shared_ptr<ITexture> texture);

        void addUniform(std::shared_ptr<IUniform> uniform);

        std::vector<std::shared_ptr<IUniform>> getUniforms();
        std::vector<std::shared_ptr<ITexture>> getTextures();

        std::shared_ptr<IShader> getLightingShader();

        void addOutputTexture(std::shared_ptr<ITexture> texture);
        void addLight(std::shared_ptr<ILight> light);

        void resize(int width, int height);

		std::shared_ptr<IUniform> getLightCountUniform() const;
		std::shared_ptr<ISSBO> getLightSSBO() const;



    private:
        int width = 1;
        int height = 1;

        std::shared_ptr<IShader> lightingShader;
        std::shared_ptr<Quad> quad;
        std::vector<std::shared_ptr<ITexture>> textures;
        std::vector<std::shared_ptr<IUniform>> uniforms;
        std::vector<std::shared_ptr<ILight>> lights;
        std::shared_ptr<Uniform> lightCountUniform;
        std::shared_ptr<SSBO> lightSSBO;

        std::vector<std::shared_ptr<ITexture>> outputTextures;
        std::shared_ptr<FrameBuffer> outputFrameBuffer;

    };

}
