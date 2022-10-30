//
// Created by tomas on 30.10.22.
//

#pragma once

#include "../renderer/impl/Program.h"

namespace Galax::Orbital {

    using namespace Galax::Renderer;
    using namespace Galax::Renderer::SceneObjects;

    class FeedbackProgram : public Program {
    public:
        FeedbackProgram() : Program(){};
        FeedbackProgram(const std::string& name) : Program(name){};

        void addFeedbackVariable(const std::string& name, int size);

        std::vector<std::tuple<std::string, int>> getFeedbackVariables();

        void clearFeedbackVariables();

        uint getFeedbackBufferId() const;

        uint getTransformFeedbackId() const;

        uint getFeedbackVaoId() const;

        void bind() override;

    protected:
        // The size of the feedback buffer. The buffer is allocated per planet. The size is around 2M vertices, which is
        // enough for testing purposes, HOWEVER it definitely needs to be properly tested and changed in the future.
        uint feedbackBufferSize = 250'000'000;

        std::vector<std::string> feedbackVars;
        std::vector<int> feedbackSizes;
        uint feedbackBufferId = 0;
        uint feedbackId = 0;
        uint feedbackVaoId = 0;

        bool compile() override;

        void prepareFeedback();

    };
} // Orital
