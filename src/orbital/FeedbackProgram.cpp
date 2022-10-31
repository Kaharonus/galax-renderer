//
// Created by tomas on 30.10.22.
//

#include <glbinding/gl/gl.h>
#include <stdexcept>
#include <numeric>

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

    prepareFeedback();
    checkError(true);


    return true;

}

void FeedbackProgram::bind() {
    if(!this->compiled || this->shadersUpdated()){
        compile();
        checkError(true);
    }

    glBindBuffer(GL_ARRAY_BUFFER, feedbackBufferId);
    glUseProgram(this->id);


}

void FeedbackProgram::clearFeedbackVariables() {
    feedbackVars.clear();
    feedbackSizes.clear();
}

void FeedbackProgram::addFeedbackVariable(const std::string &name, int size) {
    feedbackVars.push_back(name);
    feedbackSizes.push_back(size);
}

std::vector<std::tuple<std::string, int>> FeedbackProgram::getFeedbackVariables() {
    std::vector<std::tuple<std::string, int>> vars;
    for(size_t i = 0; i < feedbackVars.size(); ++i){
        vars.emplace_back(feedbackVars[i], feedbackSizes[i]);
    }
    return vars;
}

void FeedbackProgram::prepareFeedback() {
    if(feedbackBufferId != 0){
        glDeleteBuffers(1, &feedbackBufferId);
    }
    if(feedbackId != 0){
        glDeleteTransformFeedbacks(1, &feedbackBufferId);
    }
    if(feedbackVaoId != 0){
        glDeleteVertexArrays(1, &feedbackVaoId);
    }

    glCreateVertexArrays(1, &feedbackVaoId);
    glBindVertexArray(feedbackVaoId);

    glCreateTransformFeedbacks(1, &feedbackId);
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedbackId);

    glCreateBuffers(1, &feedbackBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, feedbackBufferId);

    glBufferData(GL_ARRAY_BUFFER, feedbackBufferSize, nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, feedbackBufferId);

    auto stride = std::accumulate(feedbackSizes.begin(), feedbackSizes.end(), 0);
    for(int i = 0; i < feedbackVars.size(); ++i){
        auto offset = std::accumulate(feedbackSizes.begin(), feedbackSizes.begin() + i, 0);
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, feedbackSizes[i], GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
    }
}

uint FeedbackProgram::getFeedbackBufferId() const {
    return feedbackBufferId;
}

uint FeedbackProgram::getTransformFeedbackId() const {
    return feedbackId;
}

uint FeedbackProgram::getFeedbackVaoId() const {
    return feedbackVaoId;
}

bool FeedbackProgram::shadersUpdated() {
    return Program::shadersUpdated();
}



