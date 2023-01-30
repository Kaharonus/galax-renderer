//
// Created by tomas on 14.9.22.
//
#include <glbinding/gl/gl.h>

#include "GBuffer.h"
#include <iostream>

using namespace gl;
using namespace Galax::Renderer;
using namespace Galax::Renderer::SceneObjects;

GBuffer::GBuffer() : SceneObject("GBuffer") {

}

void GBuffer::init() {
    frameBuffer = std::make_shared<FrameBuffer>("gBuffer");


    this->position = std::make_shared<Texture>("gPosition", Texture::TYPE_2D, Texture::RGBA, Texture::FLOAT,
                                               Texture::REPEAT, Texture::NEAREST);
    this->frameBuffer->addOutputTexture(position);


    this->normal = std::make_shared<Texture>("gNormal", Texture::TYPE_2D, Texture::RGBA, Texture::FLOAT,
                                             Texture::REPEAT, Texture::NEAREST);
    this->frameBuffer->addOutputTexture(normal);


    this->albedo = std::make_shared<Texture>("gAlbedo", Texture::TYPE_2D, Texture::RGBA, Texture::UNSIGNED_BYTE,
                                             Texture::REPEAT, Texture::NEAREST);
    this->frameBuffer->addOutputTexture(albedo);

    this->emission = std::make_shared<Texture>("gEmission", Texture::TYPE_2D, Texture::RGBA, Texture::FLOAT,
                                               Texture::REPEAT, Texture::NEAREST);

    this->frameBuffer->addOutputTexture(emission);

    this->metadata = std::make_shared<Texture>("gMetadata", Texture::TYPE_2D, Texture::RGBA, Texture::FLOAT,
                                               Texture::REPEAT, Texture::NEAREST);
    this->frameBuffer->addOutputTexture(metadata);


    frameBuffer->resize(width, height);

}

void GBuffer::resize(int w, int h) {
    this->width = w;
    this->height = h;
    this->frameBuffer->resize(w, h);
}

uint GBuffer::getId() {
    return frameBuffer->getId();
}

void GBuffer::bind() {
    frameBuffer->bind(true);
    glViewport(0, 0, width, height);
}


void GBuffer::unbind() {
    frameBuffer->unbind();
}

std::shared_ptr<Texture> GBuffer::getPositionTexture() {
    return position;
}

std::shared_ptr<Texture> GBuffer::getNormalTexture() {
    return normal;
}

std::shared_ptr<Texture> GBuffer::getAlbedoTexture() {
    return albedo;
}

std::shared_ptr<Texture> GBuffer::getEmissionTexture() {
    return emission;
}

std::shared_ptr<Texture> GBuffer::getMetadataTexture() {
    return metadata;
}

std::shared_ptr<FrameBuffer> GBuffer::getFrameBuffer() const {
	return frameBuffer;
}
