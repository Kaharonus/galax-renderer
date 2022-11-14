//
// Created by tomas on 29.9.22.
//

#pragma once

#include <memory>
#include <vector>
#include "../SceneObject.h"
#include "ITexture.h"
#include "IShader.h"

namespace Galax::Renderer{
    class IPostProcessEffect : public Galax::Renderer::SceneObject{
    public:
        enum class GBufferTexture {
            NONE = 0,
            POSITION = 1,
            NORMAL = 2,
            ALBEDO = 4,
            EMISSION = 8,
            METADATA = 16,
        };

        IPostProcessEffect() : SceneObject() {};
        IPostProcessEffect(const std::string& name) : SceneObject(name) {};
        virtual ~IPostProcessEffect() = default;
        virtual void requestGBufferTextures(GBufferTexture texture) = 0;
        virtual GBufferTexture getRequestedGBufferTextures() = 0;
        virtual void addInputTexture(std::shared_ptr<ITexture> texture) = 0;
        virtual void addOutputTexture(std::shared_ptr<ITexture> texture) = 0;
        virtual std::vector<std::shared_ptr<ITexture>> getInputTextures() const = 0;
        virtual std::vector<std::shared_ptr<ITexture>> getOutputTextures() const = 0;
        virtual std::shared_ptr<IShader> getShader() const = 0;
        virtual void draw() = 0;
        virtual void resize(int width, int height) = 0;
    };
}
