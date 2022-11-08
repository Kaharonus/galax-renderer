//
// Created by tomas on 7.9.22.
//

#pragma once

#include <physics/impl/PhysicalNode.h>
#include <orbital/FeedbackProgram.h>

namespace Galax::Orbital {

    using namespace Galax::Renderer::SceneObjects;


class Planet : public Physics::PhysicalNode {


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

    [[nodiscard]] std::vector<std::shared_ptr<Renderer::IProgram>> getPrograms() const override;

    protected:


        std::shared_ptr<FeedbackProgram> generatorProgram;

        bool shouldGenerate = true;


        Type type;
        uint prims;

        void generatePlanet();

        void drawPlanet();
    private:
        uint sizeQuery = 0;

        void startSizeQuery();
        int getSizeQueryResult();
    };

}
