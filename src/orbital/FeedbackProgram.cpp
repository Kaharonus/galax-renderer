//
// Created by tomas on 30.10.22.
//

#include <glbinding/gl/gl.h>
#include <stdexcept>

#include "FeedbackProgram.h"

using namespace Galax::Orbital;
using namespace gl;

bool FeedbackProgram::compile() {
    createProgram();
    compileAndLinkShaders();

    // C Style char*... what can you do
    std::vector<char*> cstrings;
    cstrings.reserve(feedbackVars.size());

    for(size_t i = 0; i < feedbackVars.size(); ++i){
        cstrings.push_back(const_cast<char*>(feedbackVars[i].c_str()));
    }
    glTransformFeedbackVaryings(this->id, feedbackVars.size(), cstrings.data(), GL_INTERLEAVED_ATTRIBS);

    glLinkProgram(this->id);
    glUseProgram(this->id);

    checkError(true);

    compiled = checkStatus();
    if(!compiled){
        return false;
    }

    readUniforms();

    createFeedbackBuffer();
    checkError(true);


    return true;

}

void FeedbackProgram::bind() {
    if(!this->compiled || this->shadersUpdated()){
        compile();
        checkError(true);
    }

    glBindBuffer(GL_ARRAY_BUFFER, feedbackBuffer);
    glUseProgram(this->id);


}

void FeedbackProgram::clearFeedbackVariables() {
    feedbackVars.clear();
}

void FeedbackProgram::addFeedbackVariable(const std::string &name) {
    feedbackVars.push_back(name);
}

std::vector<std::string> FeedbackProgram::getFeedbackVariables() {
    return feedbackVars;
}

void FeedbackProgram::createFeedbackBuffer() {
    if(feedbackBuffer != 0){
        glDeleteBuffers(1, &feedbackBuffer);
    }
    if(transformFeedback != 0){
        glDeleteTransformFeedbacks(1, &feedbackBuffer);
    }
    glCreateTransformFeedbacks(1, &transformFeedback);
    glCreateBuffers(1, &feedbackBuffer);

    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, transformFeedback);

    glBindBuffer(GL_ARRAY_BUFFER, feedbackBuffer);
    // Arbitrary large number - in this case I hope I'm not getting over 250MB per planet
    // 250 MB works out to around 3.5M tris, however I can double that if I would calculate normals in a geom shader
    auto size = 250'000'000;
    // Dynamic draw is used since I can change the tessalation level at any time to achieve a higher/lower detail level.
    // TODO actually change it according to the distance of the object to the camera
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, feedbackBuffer);

}

uint FeedbackProgram::getFeedbackBufferId() const {
    return feedbackBuffer;
}

uint FeedbackProgram::getTransformFeedbackId() const {
    return transformFeedback;
}



