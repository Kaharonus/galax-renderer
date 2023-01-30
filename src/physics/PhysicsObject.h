//
// Created by tomas on 8.11.22.
//


#pragma once

#include <reactphysics3d/reactphysics3d.h>

namespace Galax::Physics{
    class PhysicsObject {
    public:
        PhysicsObject() = default;
        explicit PhysicsObject(const std::string& name)  {
            this->name = name;
        }

        std::string getName() const {
            return name;
        }

    protected:
        static rp3d::PhysicsWorld* createWorld();
        static rp3d::SphereShape* createSphereShape(float radius);
        static void deleteSphereShape(rp3d::SphereShape* shape);

        std::string name;
    private:
        static rp3d::PhysicsCommon common;
    };

}

