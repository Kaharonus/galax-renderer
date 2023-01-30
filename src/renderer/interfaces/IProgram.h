//
// Created by tomas on 27.9.22.
//

#pragma once

#include <vector>
#include <memory>
#include "IShader.h"
#include "ITexture.h"
#include <renderer/interfaces/ISSBO.h>

namespace Galax::Renderer {
    class IProgram : public Galax::Renderer::SceneObject {
    public:
        IProgram() : SceneObject() {};

        IProgram(const std::string &name) : SceneObject(name) {};

        virtual ~IProgram() = default;

        virtual void bind() = 0;

        virtual void unbind() = 0;

        virtual bool compile() = 0;

        virtual void addShader(std::shared_ptr<IShader> shader) = 0;

        virtual std::vector<std::shared_ptr<IShader>> getShaders() = 0;

        virtual std::shared_ptr<IShader> getShader(IShader::Type type) = 0;

		virtual void setSSBO(const std::shared_ptr<ISSBO>&) = 0;

        virtual void setUniform(const std::shared_ptr<IUniform> &uniform) = 0;

        virtual void setTexture(const std::shared_ptr<ITexture> &texture, int unit) = 0;

        virtual bool hasTesslation() = 0;

        virtual unsigned int getTexturePosition(const std::shared_ptr<ITexture> &texture) = 0;

        virtual unsigned int getTextureCount() = 0;


    };
}