//
// Created by tomas on 14.9.22.
//


#pragma once

#include <memory>

#include "SceneObject.h"
#include "scene_objects/Texture.h"

namespace Galax::Renderer {
    using namespace Galax::Renderer::SceneObjects;

    class GBuffer : public SceneObject {
    public:
        GBuffer();

        uint getId() override;
        void init();
        void bind();
        void unbind();

        int getWidth(){
            return width;
        }

        int getHeight(){
            return height;
        }

        void resize(int width, int height);
        void copyToScreen();

        std::shared_ptr<Texture> getAlbedoTexture();
        std::shared_ptr<Texture> getNormalTexture();
        std::shared_ptr<Texture> getPositionTexture();


    private:
        int width = 1;
        int height = 1;

        unsigned int gBuffer = 0;

        unsigned int rbo = 0;
        std::shared_ptr<Texture> albedo;
        std::shared_ptr<Texture> position;
        std::shared_ptr<Texture> normal;


        void initTextures();
    };

}

