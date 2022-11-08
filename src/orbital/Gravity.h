//
// Created by tomas on 8.11.22.
//


#pragma once

#include <physics/impl/Force.h>
#include <renderer/interfaces/IUniform.h>

namespace Galax::Orbital {
    using namespace Galax::Renderer;
    class Gravity : public Physics::Force {
    public:
        Gravity();


        void setCenter(std::shared_ptr<IUniform> center);
        void setCenterMass(float centerMass);

        void setPosition(std::shared_ptr<IUniform> center);
        void setMass(float centerMass);



        rp3d::Vector3 getDirection() override;


    private:

        std::shared_ptr<Galax::Renderer::IUniform> center;
        float centerMass = 0;

        std::shared_ptr<Galax::Renderer::IUniform> position;
        float mass = 0;

        double G = 6.67408e-11;
    };
}

