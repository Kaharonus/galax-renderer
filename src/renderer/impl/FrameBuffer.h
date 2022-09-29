//
// Created by tomas on 20.9.22.
//

#pragma once

#include "../SceneObject.h"
#include "Texture.h"
#include "../interfaces/IFrameBuffer.h"


#include <memory>
#include <vector>

namespace Galax::Renderer::SceneObjects {
    class FrameBuffer : public IFrameBuffer {
    public:
        FrameBuffer();
        explicit FrameBuffer(const std::string& name);

        void bind(bool clear) override;
        void unbind() override;
        void addOutputTexture(std::shared_ptr<ITexture> texture);

        [[nodiscard]] int getWidth() const override;
        [[nodiscard]] int getHeight() const override;

        void copyTo(unsigned int target) const;
        void copyFrom(unsigned int target) const;
        void resize(int width, int height) override;


        uint getId() override;


    private:
        void init();

        int width;
        int height;
        std::vector<std::shared_ptr<ITexture>> outTextures;


        unsigned int id = 0;
        unsigned int rbo = 0;

        unsigned int maxTextures = 0;

        void create();
    };

}
