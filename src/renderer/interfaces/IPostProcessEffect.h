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
        IPostProcessEffect() : SceneObject() {};
        IPostProcessEffect(const std::string& name) : SceneObject(name) {};
        virtual ~IPostProcessEffect() = default;
        virtual std::vector<std::shared_ptr<ITexture>> getInputTextures() const = 0;
        virtual std::vector<std::shared_ptr<ITexture>> getOutputTextures() const = 0;
        virtual std::shared_ptr<IShader> getShader() const = 0;
        virtual void render() = 0;
        virtual void resize(int width, int height) = 0;
    };
}
