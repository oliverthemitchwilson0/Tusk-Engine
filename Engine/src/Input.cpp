#include <Tusk/Input.h>
#include <Tusk/Logging.h>

#include <SDL3/SDL.h>

//* Users should be able to bind keys with the name of their actions, like how Godot does.
//* The engine should then update the state of each action with SDL key states.
//* Users could then check for input by getting their actions state.

namespace Tusk
{
	void Input::GetKeyDown(Keys key)
	{
		auto* keystate = SDL_GetKeyboardState(nullptr);

		if(keystate[key] == true)
		{
			Logging::Log("Key: %d", key);
		}
	}
}