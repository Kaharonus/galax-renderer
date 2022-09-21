//
// Created by tomas on 20.9.22.
//

#pragma once

#include "../SceneObject.h"
#include "Texture.h"


#include <memory>
#include <vector>

namespace Galax::Renderer::SceneObjects {
    class FrameBuffer : public SceneObject {
    public:
        FrameBuffer();
        explicit FrameBuffer(const std::string& name);

        void bind(bool clear = true);
        void unbind();
        void addOutputTexture(std::shared_ptr<Texture> texture);

        int getWidth() const;
        int getHeight() const;

        void copyTo(unsigned int target);
        void copyFrom(unsigned int target);
        void resize(int width, int height);


        uint getId() override;


    private:
        void init();

        int width;
        int height;
        std::vector<std::shared_ptr<Texture>> outTextures;


        unsigned int id = 0;
        unsigned int rbo = 0;

        unsigned int maxTextures = 0;

        void create();
    };

}
