//
// Created by tomas on 7.9.22.
//

#pragma once

#include <physics/impl/PhysicalNode.h>
#include <orbital/FeedbackProgram.h>
#include <physics/impl/Force.h>

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

        Planet(const std::string &name, Planet::Type type);

        void setGeneratorProgram(const std::shared_ptr<FeedbackProgram> &program);

        void draw(glm::mat4 parentModel) override;

        void setScale(const glm::vec3 &scale) override;

        void setIsMouseOver(bool isMouseOver) override;

        [[nodiscard]] std::vector<std::shared_ptr<Renderer::IProgram>> getPrograms() const override;

    protected:
        std::shared_ptr<FeedbackProgram> generatorProgram;
        bool shouldGenerate = true;
        bool previousMouseOver = false;
        Type type;
        uint prims;

        void generatePlanet();

        void drawPlanet(glm::mat4 mat);

    private:
        std::shared_ptr<Uniform> colorOffsetUniform;
        std::shared_ptr<Animation> lightenAnimation;
        std::shared_ptr<Animation> darkenAnimation;

        uint sizeQuery = 0;

        void startSizeQuery();

        int getSizeQueryResult();

    };

}
