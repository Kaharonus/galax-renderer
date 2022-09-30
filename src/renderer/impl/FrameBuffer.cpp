//
// Created by tomas on 20.9.22.
//
#include <glbinding/gl/gl.h>

#include "FrameBuffer.h"
#include "../../Extensions.h"

using namespace gl;
using namespace Galax::Renderer::SceneObjects;

FrameBuffer::FrameBuffer() : IFrameBuffer() {
    init();
}

FrameBuffer::FrameBuffer(const std::string &name) : IFrameBuffer(name) {
    init();
}

void FrameBuffer::init() {
    // Query the maximum number of color attachments
    GLenum result;
    glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &result);
    maxTextures = (int) result;
    this->width = 1;
    this->height = 1;
    create();

}

uint FrameBuffer::getId() {
    return id;
}

void FrameBuffer::create() {
    glGenFramebuffers(1, &this->id);
    // create the textures
    resize(width, height);
}



void FrameBuffer::addOutputTexture(std::shared_ptr<ITexture> texture) {
    outTextures.push_back(texture);
}

void FrameBuffer::bind(bool clear) {
    if (id == 0) {
        create();
    }
    glBindFramebuffer(GL_FRAMEBUFFER, id);
    if(clear){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
    lastBoundFrameBuffer = id;
}

void FrameBuffer::unbind() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::copyTo(unsigned int target) const {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, id);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, target);
    glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer((gl::GLenum) GL_FRAMEBUFFER, 0);

}


void FrameBuffer::copyFrom(unsigned int target) const {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, target);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, id);
    glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer((gl::GLenum) GL_FRAMEBUFFER, 0);
}


void FrameBuffer::resize(int width, int height) {
    this->width = width;
    this->height = height;

    glBindFramebuffer(GL_FRAMEBUFFER, id);

    glViewport(0, 0, width, height);
    checkError(true);

    std::vector<GLenum> attachments;
    for (auto [i, texture]: enumerate(outTextures)) {
        checkError(true);
        texture->setDimensions(width, height);
        texture->upload();
        glBindTexture((GLenum) texture->getType(), texture->getId());
        checkError(true);

        switch (texture->getType()) {
            case Texture::TYPE_2D:
                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, texture->getId(), 0);
                break;
            case Texture::TYPE_3D:
                glFramebufferTexture3D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_3D, texture->getId(), 0, 0);
                break;
            default:
                throw std::runtime_error("Unsupported texture type");
        }
        checkError(true);

        attachments.push_back(GL_COLOR_ATTACHMENT0 + i);
    }

    glDrawBuffers((GLsizei) attachments.size(), attachments.data());

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

int FrameBuffer::getWidth() const {
    return width;
}

int FrameBuffer::getHeight() const {
    return height;
}

void FrameBuffer::clearOutputTextures() {
    outTextures.clear();
}
