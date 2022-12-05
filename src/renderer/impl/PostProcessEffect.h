//
// Created by tomas on 20.9.22.
//

#pragma once

#include "../SceneObject.h"
#include "Texture.h"
#include "Quad.h"
#include "Shader.h"
#include "FrameBuffer.h"
#include "../interfaces/IPostProcessEffect.h"

namespace Galax::Renderer {
    using namespace Galax::Renderer::SceneObjects;

    class PostProcessEffect : public IPostProcessEffect {
    public:
        PostProcessEffect();
        explicit PostProcessEffect(const std::string& name);

        unsigned int getId() override;
        void draw() override;
        [[nodiscard]] std::vector<std::shared_ptr<ITexture>> getOutputTextures() const override;
        [[nodiscard]] std::vector<std::shared_ptr<ITexture>> getInputTextures() const override;
        [[nodiscard]] std::shared_ptr<IShader> getShader() const;
        void addInputTexture(std::shared_ptr<ITexture> texture) override;
        void addOutputTexture(std::shared_ptr <ITexture> texture) override;
        void setShader(std::shared_ptr<Shader> shader);
        void resize(int width, int height) override;
        void addUniform(std::shared_ptr<Uniform> uniform);
        [[nodiscard]] std::shared_ptr<FrameBuffer> getFrameBuffer() const;

        void requestGBufferTextures(GBufferTexture texture) override;

        GBufferTexture getRequestedGBufferTextures() override;

    protected:
        void init();

        bool initialized = false;
        std::shared_ptr<Shader> shader;
        std::shared_ptr<Quad> quad;
        std::shared_ptr<FrameBuffer> frameBuffer;
        std::vector<std::shared_ptr<ITexture>> inputTextures;
        std::vector<std::shared_ptr<ITexture>> outputTextures;
        std::vector<std::shared_ptr<Uniform>> uniforms;
		std::shared_ptr<Uniform> resolutionUniform;
        std::shared_ptr<Uniform> callCountUniform;
        int requestedGBufferTextures = 0;

        int height, width;
    };
}