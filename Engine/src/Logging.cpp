#include <Tusk/Logging.h>
#include <SDL3/SDL_log.h>

namespace Tusk
{
	namespace Logging
	{
		//Variadac Functions: https://www.youtube.com/watch?v=oDC208zvsdg
		void Log(const char* format, ...)
		{
			va_list args;
			va_start(args, format);

			//SDL_Log() - Logs messages with the category and priority below, However cannot take va_list.
			SDL_LogMessageV(SDL_LOG_CATEGORY_APPLICATION, SDL_LOG_PRIORITY_INFO, format, args);

			va_end(args);
		}
	}
}