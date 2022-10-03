//
// Created by tomas on 30.9.22.
//


#pragma once

#include "../interfaces/ISSBO.h"
#include <cstring>

namespace Galax::Renderer::SceneObjects{
    class SSBO : public ISSBO {
    public:
        SSBO();
        SSBO(const std::string &name);
        ~SSBO() override;
        void setData(const std::vector<char>& data) override;
        template<typename T> void addData(const T& data){
            auto sizeT = sizeof(T);
            this->data.resize(this->data.size() + sizeT);
            memcpy(&this->data[this->data.size() - sizeT], &data, sizeT);
            updated = true;
        };
        template<typename T> void setDataAt(const T& data, unsigned int index){
            auto sizeT = sizeof(T);
            if(index * sizeT > this->data.size()){
                this->data.resize(index * sizeT);
            }
            memcpy(&this->data[index * sizeT], &data, sizeT);
            updated = true;
        }
        void bind(unsigned int location) override;
        void unbind() override;
        uint getId() override;
    private:
        void init();

        std::vector<char> data;
        bool updated = true;

        unsigned int id = 0;

    };

}
