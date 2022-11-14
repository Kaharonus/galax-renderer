//
// Created by tomas on 7.9.22.
//

#include <glbinding/gl/gl.h>
#include <iostream>

#include "Planet.h"
#include <Extensions.h>
#include <physics/impl/SphereCollider.h>

using namespace Galax::Orbital;
using namespace gl;

Planet::Planet(const std::string &name, Planet::Type type) : Physics::PhysicalNode(name) {
    this->type = type;
    this->colorOffsetUniform = std::make_shared<Uniform>("colorOffset", Uniform::FLOAT, 1.0f);

    this->lightenAnimation = std::make_shared<Animation>("Lighten planet", Animation::Target::CUSTOM,
                                                         Animation::Repeat::ONCE);
    this->lightenAnimation->setLength(300);
    this->lightenAnimation->addKeyFrame(0, 1.0f);
    this->lightenAnimation->addKeyFrame(300, 1.5f);
    this->lightenAnimation->setStartType(Animation::StartType::MANUAL);
    this->lightenAnimation->setEase(Animation::Ease::QUADRATIC_IN);
    this->lightenAnimation->setCustomTarget(this->colorOffsetUniform);
    this->addAnimation(this->lightenAnimation);

    this->darkenAnimation = std::make_shared<Animation>("Darken planet", Animation::Target::CUSTOM,
                                                        Animation::Repeat::ONCE);
    this->darkenAnimation->setLength(300);
    this->darkenAnimation->addKeyFrame(0, 1.5f);
    this->darkenAnimation->addKeyFrame(300, 1.0f);
    this->darkenAnimation->setStartType(Animation::StartType::MANUAL);
    this->darkenAnimation->setEase(Animation::Ease::QUADRATIC_IN);
    this->darkenAnimation->setCustomTarget(this->colorOffsetUniform);
    this->addAnimation(this->darkenAnimation);

}

void Planet::generatePlanet(){
    generatorProgram->bind();

    mesh->bind();
    for(auto &uniform: uniforms){
        generatorProgram->setUniform(uniform);
    }

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
    program->setUniform(mouseOverUniform);


    auto texCount = program->getTextureCount();
    for (auto [i, texture]: enumerate(textures)) {
        program->setTexture(texture, texCount + i);
    }
    updateAnimations();


    useDefaultUniforms();
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

void Planet::setScale(const glm::vec3 &scale) {
    Node::setScale(scale);

    auto radius = scale.x;
    auto collider = std::make_shared<Physics::SphereCollider>(radius);
    Physics::RigidBody::clearColliders();
    Physics::RigidBody::addBodyCollider(collider);
}

void Planet::setIsMouseOver(bool isMouseOver) {
    PhysicalNode::setIsMouseOver(isMouseOver);

    if(previousMouseOver == isMouseOver){
        return;
    }
    previousMouseOver = isMouseOver;

    if(isMouseOver){
        darkenAnimation->stop();
        lightenAnimation->start();
    }else{
        lightenAnimation->stop();
        darkenAnimation->start();
    }

}
