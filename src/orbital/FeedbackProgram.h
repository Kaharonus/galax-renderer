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

        void addFeedbackVariable(const std::string& name);

        std::vector<std::string> getFeedbackVariables();

        void clearFeedbackVariables();

        uint getFeedbackBufferId() const;

        void bind() override;

    protected:
        std::vector<std::string> feedbackVars;
        uint feedbackBuffer = 0;
        uint feedbackQuery = 0;

        bool compile() override;

        void createFeedbackBuffer();

        void createFeedbackSizeQuery();
    };
} // Orital
