//
// Created by tomas on 13.11.22.
//


#pragma once

#include <renderer/impl/PostProcessEffect.h>

namespace Galax::Effects{
    using namespace Galax::Renderer;
    class Outline :  public PostProcessEffect {
    public:

        Outline();

        void draw() override;

        explicit Outline(const std::string& name);

    private:

    };
}