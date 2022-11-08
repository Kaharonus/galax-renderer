//
// Created by tomas on 8.11.22.
//

#pragma once
#include <reactphysics3d/reactphysics3d.h>

namespace Galax::Physics{
    class IForce{
        public:
            virtual void setDirection(rp3d::Vector3 direction) = 0;
            virtual rp3d::Vector3 getDirection() = 0;
    };
}
