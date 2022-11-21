
#pragma once

#include <memory>
#include <chrono>
#include <string>
#include <unordered_set>
#include <glm/vec2.hpp>


namespace Galax::Renderer {
#define NOT_IMPLEMENTED throw std::runtime_error("Not implemented");

    class SceneObject {
    public:
        static void init();

        SceneObject();
        SceneObject(const std::string &name, bool enableUnsafeNaming = false);
        ~SceneObject();
        //virtual void render() = 0;
        std::string getName();
        uint getNameHash();

        void setName(std::string name);

        static void setFrameTime(float frameTime);


        static unsigned int getLastFrameBuffer();

        inline static bool initialized = false;
        virtual uint getId() = 0;

        static float getFrameTime();

    protected:

        std::string name;
        inline static unsigned int lastBoundFrameBuffer = 0;
        inline static float frameTime = 0.0f;
        inline static float currentTime;
		inline static glm::vec2 screenSize;

        void setDrawSize(int width, int height);

        std::string getNextDefaultName();

        void checkError(bool shouldThrow);

		int drawWidth = 0;
		int drawHeight = 0;

    private:
		inline static unsigned long startTime;


        static std::unordered_set<unsigned int> usedNames;

        uint id = 0;

        std::string matchError(int error);

        virtual void ignorethisshitrighthere(){}; // A hack to make the compiler happy :)

        inline static unsigned int defaultNameCounter = 0;

        void checkName();
    };
} // namespace MapGenerator::Renderer::SceneObjects
