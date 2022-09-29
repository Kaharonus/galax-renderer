//
// Created by tomas on 27.9.22.
//
#pragma once
namespace Galax::Renderer {
    class IFrameBuffer : public Galax::Renderer::SceneObject {
    public:
        IFrameBuffer() : SceneObject() {};

        IFrameBuffer(const std::string &name) : SceneObject(name) {};

        virtual ~IFrameBuffer() = default;

        virtual void bind(bool clear = true) = 0;

        virtual void unbind() = 0;

        virtual void resize(int width, int height) = 0;

        [[nodiscard]] virtual int getWidth() const = 0;

        [[nodiscard]] virtual int getHeight() const = 0;


    };
}