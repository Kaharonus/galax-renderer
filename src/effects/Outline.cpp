//
// Created by tomas on 13.11.22.
//

#include <effects/Outline.h>
#include <Extensions.h>

using namespace Galax::Effects;


Outline::Outline() {

}


Outline::Outline(const std::string &name) : PostProcessEffect(name) {

}

void Outline::draw() {
    if (!initialized) {
        init();
    }
    setDrawSize(width, height);
    frameBuffer->bind(true);
    auto program = quad->getProgram();
    program->bind();
    for (auto [i, texture]: enumerate(inputTextures)) {
        program->setTexture(texture, i);
    }
    for (auto &uniform: uniforms) {
        program->setUniform(uniform);
    }
    quad->draw();
    frameBuffer->unbind();
}

