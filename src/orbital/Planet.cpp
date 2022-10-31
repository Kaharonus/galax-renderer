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

void Planet::draw() {
    if (shouldGenerate) {

        shouldGenerate = false;
        generatorProgram->bind();

        selectLOD(50); //TODO rework to something more sensible
        mesh->bind();

        uint query = 0;
        glGenQueries(1, &query);
        glBeginQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN, query);

        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, generatorProgram->getTransformFeedbackId());
        glBeginTransformFeedback(GL_TRIANGLES);
        auto drawMode = GL_TRIANGLES;

        if (generatorProgram->hasTesslation()) {
            glPatchParameteri(GL_PATCH_VERTICES, 3);
            drawMode = GL_PATCHES;
        }
        glDrawElements(drawMode, (int) mesh->size(), GL_UNSIGNED_INT, nullptr);

        glEndTransformFeedback();
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
        glEndQuery(GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN);

        // Get query result

        GLuint64 result = 0;
        glGetQueryObjectui64v(query, GL_QUERY_RESULT, &result);
        std::cout << "Generated " << result << " vertices" << std::endl;
        this->prims = result;

        glBindVertexArray(0);

    } else {
        program->bind();

        for(auto &uniform: uniforms){
            program->setUniform(uniform);
        }

        auto texCount = program->getTextureCount();
        for (auto [i, texture]: enumerate(textures)) {
            program->setTexture(texture, texCount + i);
        }

        useCamera();
        checkError(true);

        glBindVertexArray(generatorProgram->getFeedbackVaoId());
        checkError(true);

        glBindBuffer(GL_ARRAY_BUFFER, generatorProgram->getFeedbackBufferId());
        //glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, generatorProgram->getTransformFeedbackId());
        glDrawArrays(GL_TRIANGLES, 0, prims*3);
        checkError(true);

        //glDrawTransformFeedback(GL_TRIANGLES, generatorProgram->getTransformFeedbackId());

    }


}


std::vector<std::shared_ptr<Galax::Renderer::IProgram>> Planet::getPrograms() const {
    return {program, generatorProgram};
}


void Planet::setGeneratorProgram(const std::shared_ptr<FeedbackProgram> &program) {
    generatorProgram = program;
}
