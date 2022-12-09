//
// Created by tomas on 29.9.22.
//


#pragma once

#include <renderer/interfaces/IPostProcessEffect.h>
#include <renderer/SceneObject.h>
#include <renderer/interfaces/IShader.h>
#include <renderer/interfaces/ITexture.h>
#include <renderer/impl/FrameBuffer.h>
#include <renderer/impl/Shader.h>
#include <renderer/impl/Quad.h>


namespace Galax::Effects {
        using namespace Galax::Renderer;
        using namespace Galax::Renderer::SceneObjects;
        class Bloom : public IPostProcessEffect {
        public:
            Bloom();
            explicit Bloom(const std::string& name);
            void draw() override;
            void resize(int width, int height) override;
            unsigned int getId() override;
            void addInputTexture(std::shared_ptr<ITexture> texture) override;
            void addOutputTexture(std::shared_ptr<ITexture> texture) override;
            void setShader(std::shared_ptr<IShader> shader);
            int getPasses() const;
            [[nodiscard]] std::vector<std::shared_ptr<ITexture>> getInputTextures() const override;
            [[nodiscard]] std::vector<std::shared_ptr<ITexture>> getOutputTextures() const override;
            [[nodiscard]] std::shared_ptr<IShader> getShader() const override;

            void requestGBufferTextures(GBufferTexture texture) override;

            GBufferTexture getRequestedGBufferTextures() override;

        private:
            int passes = 6;
            std::shared_ptr<IShader> shader;
            std::shared_ptr<ITexture> inputTexture;
            std::shared_ptr<ITexture> outputTexture;
            std::array<std::shared_ptr<ITexture>, 2> textures;
            std::array<std::shared_ptr<FrameBuffer>, 2> buffers;
            std::shared_ptr<Quad> quad;
            std::shared_ptr<Program> program;
            std::shared_ptr<Uniform> directionUniform;

            int width, height = 0;
            void init();
        };

}
