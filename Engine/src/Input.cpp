#include <Tusk/Input.h>
#include <Tusk/Logging.h>

#include <SDL3/SDL.h>

namespace Tusk
{
	void Input::GetKeyDown()
	{
		auto* keystate = SDL_GetKeyboardState(nullptr);

		if(keystate[SDL_SCANCODE_W] == true)
		{
			Logging::Log("W");
		}
	}
}