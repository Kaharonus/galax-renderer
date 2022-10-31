//
// Created by tomas on 7.9.22.
//

#include <glbinding/gl/gl.h>
#include <iostream>

#include "Planet.h"
#include "../Extensions.h"

using namespace Galax::Orbital;
using namespace gl;

Planet::Planet(const std::string &name, Planet::Type type) : Node(name) {
    this->type = type;
}

void Planet::generatePlanet(){
    generatorProgram->bind();

    mesh->bind();


    startSizeQuery();

    //Prepare for transform feedback rendering - disable rasterization and begin the transform feedback
    glEnable(GL_RASTERIZER_DISCARD);
    glBeginTransformFeedback(GL_TRIANGLES);

    //Normal mesh drawing
    auto drawMode = GL_TRIANGLES;
    if (generatorProgram->hasTesslation()) {
        glPatchParameteri(GL_PATCH_VERTICES, 3);
        drawMode = GL_PATCHES;
    }
    glDrawElements(drawMode, (int) mesh->size(), GL_UNSIGNED_INT, nullptr);

    //End transform feedback and re-enable rasterization
    glEndTransformFeedback();
    glDisable(GL_RASTERIZER_DISCARD);

    // Get query result
    prims = getSizeQueryResult();


    glBindVertexArray(0);

}

void Planet::drawPlanet(){
    program->bind();

    for(auto &uniform: uniforms){
        program->setUniform(uniform);
    }

    auto texCount = program->getTextureCount();
    for (auto [i, texture]: enumerate(textures)) {
        program->setTexture(texture, texCount + i);
    }
    updateAnimations();


    useCamera();
    glBindVertexArray(generatorProgram->getFeedbackVaoId());
    glBindBuffer(GL_ARRAY_BUFFER, generatorProgram->getFeedbackBufferId());
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, generatorProgram->getTransformFeedbackId());

    if(wireframe){
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    //glDrawTransformFeedback(GL_TRIANGLES, generatorProgram->getTransformFeedbackId());
    glDrawArrays(GL_TRIANGLES, 0, prims*3);

    if(wireframe){
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}


void Planet::draw() {
    if (shouldGenerate || generatorProgram->shadersUpdated()) {
        generatePlanet();
        shouldGenerate = false;
    } else {
          drawPlanet();
    }
}


std::vector<std::shared_ptr<Galax::Renderer::IProgram>> Planet::getPrograms() const {
    return {program, generatorProgram};
}


void Planet::setGeneratorProgram(const std::shared_ptr<FeedbackProgram> &program) {
    generatorProgram = program;
}

void Planet::startSizeQuery() {
    if(sizeQuery == 0){
        glGenQueries(1, &sizeQuery);
    }
    glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, sizeQuery);
}

int Planet::getSizeQueryResult() {
    glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);
    GLuint64 result = 0;
    glGetQueryObjectui64v(sizeQuery, GL_QUERY_RESULT, &result);
    return result;

}
