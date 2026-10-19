#include <Tusk/Logging.h>
#include <SDL3/SDL_log.h>

namespace Tusk
{
	namespace Logging
	{
		void Log()
		{
			SDL_Log("Work");
		}
	}
}