//
// Created by tomas on 30.9.22.
//


#pragma once

#include "../interfaces/ISSBO.h"

namespace Galax::Renderer::SceneObjects{
    class SSBO : public ISSBO {
    public:
        SSBO();
        SSBO(const std::string &name);
        ~SSBO() override;
        void setData(const std::vector<char>& data) override;
        template<typename T> void addData(const T& data);
        template<typename T> void setDataAt(const T& data, unsigned int index);
        void bind() override;
        void unbind() override;
        uint getId() override;
    private:
        void init();

        std::vector<char> data;
        bool updated = true;

        unsigned int id = 0;

    };

}
