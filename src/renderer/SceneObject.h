
#pragma once

#include <memory>
#include <chrono>
#include <string>
#include <unordered_set>


namespace Galax::Renderer {
#define NOT_IMPLEMENTED throw std::runtime_error("Not implemented");

    class SceneObject {
    public:
        static void init();

        SceneObject();
        SceneObject(const std::string &name);
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

        void setDrawSize(int width, int height);

        std::string getNextDefaultName();

        void checkError(bool shouldThrow);

    private:

        static std::unordered_set<unsigned int> usedNames;

        uint id = 0;

        std::string matchError(int error);

        virtual void ignorethisshitrighthere(){}; // A hack to make the compiler happy :)

        inline static unsigned int defaultNameCounter = 0;


        int drawWidth = 0;
        int drawHeight = 0;

        void checkName();
    };
} // namespace MapGenerator::Renderer::SceneObjects
