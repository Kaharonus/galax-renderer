//
// Created by tomas on 8.11.22.
//

#pragma once

#include <physics/interfaces/IForce.h>

namespace Galax::Physics {
    class Force : public IForce {
    public:
        Force() = default;
        explicit Force(rp3d::Vector3 direction){
            this->direction = direction;
        };

        void setDirection(rp3d::Vector3 direction) override {
            this->direction = direction;

        }

        rp3d::Vector3 getDirection() override {
            return direction;
        }

    protected:
        rp3d::Vector3 direction = {};
    };

}