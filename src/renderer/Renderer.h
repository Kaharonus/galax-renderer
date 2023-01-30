//
// Created by tomas on 27.12.22.
//

#pragma once

#include <renderer/Scene.h>
#include <renderer/LightingModel.h>
#include <renderer/interfaces/IPostProcessEffect.h>

namespace Galax::Renderer{
	class Renderer {
	public:
        Renderer();
        ~Renderer();

		void setScene(std::shared_ptr<Scene> scene);
		void setLightingModel(std::shared_ptr<LightingModel> lightingModel);
		void addPostProcess(std::shared_ptr<IPostProcessEffect> postProcess);
		void setInputHandler(std::shared_ptr<InputHandler> handler);
		void setLastFrameTime(double frameTime);

		std::shared_ptr<Scene> getScene(){
			return scene;
		}


		void init();
		void resize(int width, int height);
        void draw();
	private:

		void prepareRender();

		void drawOpaqueGeometry();

		void drawTransparentGeometry();

		void drawLighting();

		void drawPostProcess();

		void finishRender();


		double frameTime = 0.0;
		bool frameTimeSet = false;

		float width = 1;
		float height = 1;
		bool requestedResize = false;
		std::shared_ptr<Scene> scene;
		std::shared_ptr<LightingModel> lightingModel;
		std::vector<std::shared_ptr<IPostProcessEffect>> postProcesses;
		std::shared_ptr<GBuffer> gBuffer;
		std::shared_ptr<InputHandler> input;



	};

}
