
#pragma once

//#include <glbinding/gl/gl.h>
#include <memory>
#include <chrono>
#include <string>


namespace Galax::Renderer {
#define NOT_IMPLEMENTED throw std::runtime_error("Not implemented");

    class SceneObject {
    public:
        static void init();

        SceneObject();
        SceneObject(std::string name);
        ~SceneObject();
        //virtual void render() = 0;
        std::string getName();
        uint getNameHash();

        void setName(std::string name);

        static void setFrameTime(float frameTime);

        static unsigned int getLastFrameBuffer();

        inline static bool initialized = false;
        virtual uint getId() = 0;

    protected:

        std::string name;
        inline static unsigned int lastBoundFrameBuffer = 0;
        inline static float frameTime = 0.0f;
        inline static unsigned long startTime;
        inline static unsigned long currentTime;


        std::string getNextDefaultName();

        void checkError(bool shouldThrow);

    private:

        uint id = 0;

        std::string matchError(int error);

        virtual void ignorethisshitrighthere(){}; // A hack to make the compiler happy :)

        inline static unsigned int defaultNameCounter = 0;
    };
} // namespace MapGenerator::Renderer::SceneObjects
