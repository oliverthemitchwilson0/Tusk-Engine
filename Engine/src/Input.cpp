#include <Tusk/Input.h>
#include <Tusk/Logging.h>

#include <SDL3/SDL.h>

//* Users should be able to bind keys with the name of their actions, like how Godot does.
//* The engine should then update the state of each action with SDL key states.
//* Users could then check for input by getting their actions state.

//https://dev.epicgames.com/documentation/unreal-engine/input-overview-in-unreal-engine

//New Note: After talking to Gareth, I'm just focusing on getting the "must have" sytems working. Leaving the above notes so I can come back to it later if I feel necessary.

//https://wiki.libsdl.org/SDL3/BestKeyboardPractices

namespace Tusk
{
	struct MouseCoordinates
	{
		float x, y;
	};

	struct Input::Impl
	{
		//Keyboard states
		bool currentKeyState[SDL_SCANCODE_COUNT];
		bool previousKeyState[SDL_SCANCODE_COUNT];

		//Mouse states
		MouseCoordinates currentMousePos;
		MouseCoordinates previousMousePos;
	};

	Input::Input() : impl(std::make_unique<Impl>()) {}

	Input::~Input() {}

	void Input::ProcessInput()
	{
		//Setup
		auto& currentKeyState = impl->currentKeyState;
		auto& previousKeyState = impl->previousKeyState;
		auto* sdlKeystate = SDL_GetKeyboardState(nullptr);

		auto& currentMousePos = impl->currentMousePos;
		auto& previousMousePos = impl->previousMousePos;

		//Process
		
		//Copy current key state into previous state.
		std::copy(std::begin(currentKeyState), std::end(currentKeyState), std::begin(previousKeyState));

		//Copy SDL keyboard state into current key state.
		std::copy(sdlKeystate, sdlKeystate + SDL_SCANCODE_COUNT, std::begin(currentKeyState));

		previousMousePos = currentMousePos;
		auto mouseState = SDL_GetMouseState(&currentMousePos.x, &currentMousePos.y);
	}

	bool Input::GetKeyDown(Keys key)
	{
		if(impl->currentKeyState[key])
		{
			//Logging::Log("Key: %d", key);
			return true;
		}

		return false;
	}

	bool Input::GetKeyPressed(Keys key)
	{
		if (impl->currentKeyState[key] && !impl->previousKeyState[key])
		{
			//Logging::Log("Key: %d", key);
			return true;
		}

		return false;
	}

	void Tusk::Input::GetMouseDelta(float* deltaX, float* deltaY)
	{
		*deltaX = std::abs(impl->currentMousePos.x - impl->previousMousePos.x);
		*deltaY = std::abs(impl->currentMousePos.y - impl->previousMousePos.y);
	}


}