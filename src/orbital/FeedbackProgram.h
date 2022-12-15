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

        bool shadersUpdated();

		bool compile() override;


		void bind() override;

    protected:
        uint feedbackBufferSize = 30'000'000;

        std::vector<std::string> feedbackVars;
        std::vector<int> feedbackSizes;
        uint feedbackBufferId = 0;
        uint feedbackId = 0;
        uint feedbackVaoId = 0;


        void prepareFeedback();

    };
} // Orital
