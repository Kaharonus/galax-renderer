//
// Created by tomas on 7.9.22.
//

#pragma once

#include "../renderer/impl/Node.h"
#include "FeedbackProgram.h"

namespace Galax::Orbital {

    using namespace Galax::Renderer::SceneObjects;

    class Planet : public Renderer::SceneObjects::Node {


    public:
        enum class Type {
            ROCKY,
            GAS_GIANT,
            ICE,
            HOT,
            TEMPERATE,
            OCEAN
        };

        void setGeneratorProgram(const std::shared_ptr<FeedbackProgram> &program);

        Planet(const std::string& name, Planet::Type type);

        void draw() override;

        [[nodiscard]] std::shared_ptr<Renderer::IProgram> getProgram() const override;

    protected:

        uint vao;

        std::shared_ptr<FeedbackProgram> generatorProgram;

        bool shouldGenerate = true;

        Type type;
    };

}
