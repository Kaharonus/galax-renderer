#pragma once

#include <tuple>
#include <glm/vec2.hpp>
#include <functional>
#include <chrono>
#include <memory>
#include <any>
#include <SDL.h>
#include <Qt>

namespace Galax::Renderer {
	class InputHandler {
	public:
		InputHandler();

		~InputHandler();

		enum Key {
			KEY_UNKNOWN = 0,
			KEY_SHIFT = 16,
			KEY_CONTROL = 27,
			KEY_SPACE = 32,
			KEY_APOSTROPHE = 39,
			KEY_COMMA = 44,
			KEY_MINUS = 45,
			KEY_PERIOD = 46,
			KEY_SLASH = 47,
			KEY_0 = 48,
			KEY_1 = 49,
			KEY_2 = 50,
			KEY_3 = 51,
			KEY_4 = 52,
			KEY_5 = 53,
			KEY_6 = 54,
			KEY_7 = 55,
			KEY_8 = 56,
			KEY_9 = 57,
			KEY_SEMICOLON = 59,
			KEY_EQUAL = 61,
			KEY_A = 65,
			KEY_B = 66,
			KEY_C = 67,
			KEY_D = 68,
			KEY_E = 69,
			KEY_F = 70,
			KEY_G = 71,
			KEY_H = 72,
			KEY_I = 73,
			KEY_J = 74,
			KEY_K = 75,
			KEY_L = 76,
			KEY_M = 77,
			KEY_N = 78,
			KEY_O = 79,
			KEY_P = 80,
			KEY_Q = 81,
			KEY_R = 82,
			KEY_S = 83,
			KEY_T = 84,
			KEY_U = 85,
			KEY_V = 86,
			KEY_W = 87,
			KEY_X = 88,
			KEY_Y = 89,
			KEY_Z = 90,
			KEY_LEFT_BRACKET = 91,
			KEY_BACKSLASH = 92,
			KEY_RIGHT_BRACKET = 93,

			KEY_ESCAPE = 256,
			KEY_ENTER = 257,
			KEY_TAB = 258,
			KEY_BACKSPACE = 259,
			KEY_INSERT = 260,
		};

		enum MouseButton {
			MOUSE_BUTTON_UNKNOWN = 0,
			MOUSE_LEFT = 1,
			MOUSE_RIGHT = 2,
			MOUSE_MIDDLE = 4,
		};

		void mouseMove(float x, float y);

		void mousePress(MouseButton button, float x, float y);

		void mouseRelease(MouseButton button, float x, float y);

		void mouseWheel(float x, float y);

		void keyPress(Key key);

		void keyRelease(Key key);

		void registerMouseClickCallback(std::function<void(MouseButton, float, float)> callback);

		void setClickOnDetection(std::function<std::any(MouseButton, float, float)> fn);

		void registerMouseClickOnCallback(std::function<void(std::any, MouseButton)> fn);

		bool isKeyPressed(Key key);

		bool isKeyUp(Key key);

		bool isMouseButtonPressed(MouseButton button);

		bool isMouseButtonUp(MouseButton button);

		glm::vec2 getPositionDelta();

		glm::vec2 getAbsolutePosition();

		glm::vec2 getRendererPosition();

		void setRendererSize(int width, int height);

		static Key fromQtKey(int key);

		static Key fromSDLKey(int key);

		glm::vec2 getScrollDelta();

	private:
		std::vector<std::function<void(MouseButton, float, float)>> mouseClickCallbacks;
		std::function<std::any(MouseButton, float, float)> clickedOn;
		std::vector<std::function<void(std::any, MouseButton)>> mouseClickedOnCallbacks;


		bool keys[512];
		bool mouseButtons[3];
		std::chrono::milliseconds mousePressTime[3];
		float deltaX = 0;
		float deltaY = 0;
		float absoluteX = 0;
		float absoluteY = 0;
		glm::vec2 scrollDelta = {0, 0};
		bool scrollDeltaSet = false;

		float rendererHeight;
		float rendererWidth;


		inline static const std::unordered_map<SDL_Keycode, Qt::Key> sdlToQt = {
				{SDLK_0, Qt::Key_0},
				{SDLK_1, Qt::Key_1},
				{SDLK_2, Qt::Key_2},
				{SDLK_3, Qt::Key_3},
				{SDLK_4, Qt::Key_4},
				{SDLK_5, Qt::Key_5},
				{SDLK_6, Qt::Key_6},
				{SDLK_7, Qt::Key_7},
				{SDLK_8, Qt::Key_8},
				{SDLK_9, Qt::Key_9},
				{SDLK_a, Qt::Key_A},
				{SDLK_b, Qt::Key_B},
				{SDLK_c, Qt::Key_C},
				{SDLK_d, Qt::Key_D},
				{SDLK_e, Qt::Key_E},
				{SDLK_f, Qt::Key_F},
				{SDLK_g, Qt::Key_G},
				{SDLK_h, Qt::Key_H},
				{SDLK_i, Qt::Key_I},
				{SDLK_j, Qt::Key_J},
				{SDLK_k, Qt::Key_K},
				{SDLK_l, Qt::Key_L},
				{SDLK_m, Qt::Key_M},
				{SDLK_n, Qt::Key_N},
				{SDLK_o, Qt::Key_O},
				{SDLK_p, Qt::Key_P},
				{SDLK_q, Qt::Key_Q},
				{SDLK_r, Qt::Key_R},
				{SDLK_s, Qt::Key_S},
				{SDLK_t, Qt::Key_T},
				{SDLK_u, Qt::Key_U},
				{SDLK_v, Qt::Key_V},
				{SDLK_w, Qt::Key_W},
				{SDLK_x, Qt::Key_X},
				{SDLK_y, Qt::Key_Y},
				{SDLK_z, Qt::Key_Z},
		};

	};
} // namespace MapGenerator::Renderer
