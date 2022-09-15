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
    checkError(true);


    // create framebuffer object
    glGenFramebuffers(1, &gBuffer);
    // create the textures
    initTextures();


}

void GBuffer::initTextures() {
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

    if (!position) {
        this->position = std::make_shared<Texture>("gPosition", Texture::TYPE_2D, Texture::RGBA, Texture::FLOAT,
                                                   Texture::REPEAT, Texture::NEAREST);
    }
    this->position->setDimensions(width, height);
    this->position->upload();
    glBindTexture(GL_TEXTURE_2D, position->getId());
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, position->getId(), 0);

    if (!normal) {
        this->normal = std::make_shared<Texture>("gNormal", Texture::TYPE_2D, Texture::RGBA, Texture::FLOAT,
                                                 Texture::REPEAT, Texture::NEAREST);
    }
    this->normal->setDimensions(width, height);
    this->normal->upload();
    glBindTexture(GL_TEXTURE_2D, normal->getId());
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normal->getId(), 0);
    if (!albedo) {
        this->albedo = std::make_shared<Texture>("gAlbedo", Texture::TYPE_2D, Texture::RGBA, Texture::UNSIGNED_BYTE,
                                                 Texture::REPEAT, Texture::NEAREST);
    }
    this->albedo->setDimensions(width, height);
    this->albedo->upload();
    glBindTexture(GL_TEXTURE_2D, albedo->getId());
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, albedo->getId(), 0);

    GLenum attachments[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
    glDrawBuffers(3, attachments);

    // create render buffer object

    if (rbo == 0) {
        glGenRenderbuffers(1, &rbo);
    }
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
    auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "Framebuffer not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void GBuffer::resize(int w, int h) {
    this->width = w;
    this->height = h;
    initTextures();
}

uint GBuffer::getId() {
    return gBuffer;
}

void GBuffer::bind() {
    if (gBuffer == 0) {
        init();
    }

    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
    glViewport(0, 0, width, height);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GBuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
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

void GBuffer::copyToScreen() {
    glBindFramebuffer((gl::GLenum) GL_READ_FRAMEBUFFER, gBuffer);
    glBindFramebuffer((gl::GLenum) GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer((gl::GLenum) GL_FRAMEBUFFER, 0);
}

