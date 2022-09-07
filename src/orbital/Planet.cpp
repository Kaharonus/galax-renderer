//
// Created by tomas on 7.9.22.
//

#include "Planet.h"

using namespace Galax::Orbital;

Planet::Planet(const std::string& name, Planet::Type type) : Node(name) {
    this->type = type;
}
