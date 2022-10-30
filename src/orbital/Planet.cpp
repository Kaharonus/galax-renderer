//
// Created by tomas on 7.9.22.
//

#include <glbinding/gl/gl.h>
#include <iostream>

#include "Planet.h"

using namespace Galax::Orbital;
using namespace gl;

Planet::Planet(const std::string &name, Planet::Type type) : Node(name) {
    this->type = type;
}

void Planet::draw() {
    if (shouldGenerate) {

        shouldGenerate = false;
        generatorProgram->bind();

        selectLOD(50); //TODO rework to something more sensible
        mesh->bind();

        checkError(true);
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, generatorProgram->getTransformFeedbackId());
        glBeginTransformFeedback(GL_TRIANGLES);
        checkError(true);

        glDrawElements(GL_TRIANGLES, (int) mesh->size(), GL_UNSIGNED_INT, nullptr);
        checkError(true);

        glEndTransformFeedback();
        checkError(true);

        glFlush();
        checkError(true);

        glBindVertexArray(0);
        checkError(true);

    } else {
        program->bind();
        useCamera();
        checkError(true);

        glBindVertexArray(generatorProgram->getFeedbackVaoId());
        checkError(true);

        glBindBuffer(GL_ARRAY_BUFFER, generatorProgram->getFeedbackBufferId());
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, generatorProgram->getTransformFeedbackId());
        checkError(true);

        //glDrawTransformFeedback(GL_TRIANGLES, generatorProgram->getTransformFeedbackId());
        glDrawArrays(GL_TRIANGLES, 0, 15360);

    }


}


std::shared_ptr<Galax::Renderer::IProgram> Planet::getProgram() const {
    return generatorProgram;
}

void Planet::setGeneratorProgram(const std::shared_ptr<FeedbackProgram> &program) {
    generatorProgram = program;
}
