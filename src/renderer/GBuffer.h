//
// Created by tomas on 14.9.22.
//


#pragma once

#include <memory>

#include "SceneObject.h"
#include "scene_objects/Texture.h"
#include "scene_objects/FrameBuffer.h"

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

        std::shared_ptr<Texture> getAlbedoTexture();
        std::shared_ptr<Texture> getNormalTexture();
        std::shared_ptr<Texture> getPositionTexture();
        std::shared_ptr<Texture> getEmissionTexture();


    private:
        int width = 1;
        int height = 1;

        std::shared_ptr<Texture> albedo;
        std::shared_ptr<Texture> position;
        std::shared_ptr<Texture> normal;
        std::shared_ptr<Texture> emission;

        std::shared_ptr<FrameBuffer> frameBuffer;


    };

}

