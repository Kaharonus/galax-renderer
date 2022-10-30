//
// Created by tomas on 7.9.22.
//

#include <glbinding/gl/gl.h>
#include <iostream>

#include "Planet.h"

using namespace Galax::Orbital;
using namespace gl;
Planet::Planet(const std::string& name, Planet::Type type) : Node(name) {
    this->type = type;
}

void Planet::draw() {
    if(shouldGenerate){

        //shouldGenerate = false;
        generatorProgram->bind();
        checkError(true);
        //useCamera();

        generatorProgram->setUniform(camera->getViewUniform());
        generatorProgram->setUniform(camera->getProjectionUniform());
        generatorProgram->setUniform(camera->getPositionUniform());
        generatorProgram->setUniform(camera->getRotationUniform());
        generatorProgram->setUniform(this->modelMatrixUniform);

        selectLOD(50);
        mesh->bind();
        if(feedbackSizeQuery == 0){
            glGenQueries(1, &feedbackSizeQuery);
        }
        glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, feedbackSizeQuery);
        checkError(true);

        glBeginTransformFeedback(GL_TRIANGLES);
        checkError(true);
        glDrawElements(GL_TRIANGLES, (int) mesh->size(), GL_UNSIGNED_INT, nullptr);
        glEndTransformFeedback();
        glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);

        glFlush();
        checkError(true);
        GLuint primitives;
        glGetQueryObjectuiv(feedbackSizeQuery, GL_QUERY_RESULT, &primitives);
        std::cout << primitives << std::endl;
    }else{


    }


}



std::shared_ptr<Galax::Renderer::IProgram> Planet::getProgram() const {
    return generatorProgram;
}

void Planet::setGeneratorProgram(const std::shared_ptr<FeedbackProgram> &program) {
    generatorProgram = program;
}
