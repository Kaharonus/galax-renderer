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

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        shouldGenerate = false;
        generatorProgram->bind();

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) 0);

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *) (3 * sizeof(float)));

        generatorProgram->setUniform(camera->getViewUniform());
        generatorProgram->setUniform(camera->getProjectionUniform());
        generatorProgram->setUniform(camera->getPositionUniform());
        generatorProgram->setUniform(camera->getRotationUniform());
        generatorProgram->setUniform(this->modelMatrixUniform);

        selectLOD(50);
        mesh->bind();
        if (feedbackSizeQuery == 0) {
            glGenQueries(1, &feedbackSizeQuery);
        }
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, generatorProgram->getTransformFeedbackId());
        glBeginTransformFeedback(GL_TRIANGLES);
        glDrawElements(GL_TRIANGLES, (int) mesh->size(), GL_UNSIGNED_INT, nullptr);
        glEndTransformFeedback();
        glEnableVertexAttribArray(0);
        glFlush();
        glBindVertexArray(0);


    } else {
        program->bind();
        useCamera();

        //glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void *) 0);
        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, generatorProgram->getFeedbackBufferId());
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, generatorProgram->getTransformFeedbackId());
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
