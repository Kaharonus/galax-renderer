
#pragma once

#include <memory>
#include <chrono>
#include <string>
#include <unordered_set>
#include <glm/vec2.hpp>
#include <unordered_map>
#include <mutex>

namespace Galax::Renderer {
#define NOT_IMPLEMENTED throw std::runtime_error("Not implemented");

    class SceneObject {
    public:
        static void init();

        SceneObject();
        SceneObject(const std::string &name, bool enableUnsafeNaming = false);
        ~SceneObject();
        std::string getName();
        uint getNameHash();

        void setName(std::string name);

        static void setFrameTime(float frameTime);


        static unsigned int getLastFrameBuffer();

        inline static bool initialized = false;
        virtual uint getId() = 0;

        static float getFrameTime();

		static float getCurrentTime();

		static void addUniqueContext(const std::string& name);

		static void selectAndLockContext(const std::string& name);

		static void setLastBoundFBO(uint fbo);

		static void unlockContext();

    protected:
        std::string name;
		int drawWidth = 0;
		int drawHeight = 0;
        void setDrawSize(int width, int height);
        void checkError(bool shouldThrow);
		std::string getNextDefaultName();

    private:
		static uint selectedContext;
		static std::unordered_map<std::string, uint> contexts;
		static std::unordered_map<uint, unsigned int> lastBoundFrameBuffer;
		static std::unordered_map<uint, float> frameTime;
		static std::unordered_map<uint, float> currentTime;
		static std::unordered_map<uint, glm::vec2> screenSize;
		static std::unordered_map<uint,unsigned long> startTime;
		static std::mutex mutex;

		static std::unordered_map<uint,std::unordered_set<unsigned int>> usedNames;

        uint id = 0;

        std::string matchError(int error);

        virtual void ignorethisshitrighthere(){}; // A hack to make the compiler happy :)

        inline static unsigned int defaultNameCounter = 0;

        void checkName();
    };
} // namespace MapGenerator::Renderer::SceneObjects
