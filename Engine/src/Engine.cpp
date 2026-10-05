//#include <Tusk/Engine.h>

#include <Tusk/Engine.h>

#include <SDL3/SDL.h>
#include <chrono>

namespace Tusk
{
	namespace
	{
		constexpr double fixedStepTime = 1.0 / 60.0;
	}

	//Private Engine Implementation that holds SDL window lifetime.
	struct Engine::Impl
	{
		struct WindowDeleter //Struct that call Destroy Window.
		{
			void operator()(SDL_Window* window) const noexcept { SDL_DestroyWindow(window); }
		};

		std::unique_ptr<SDL_Window, WindowDeleter> window;
		bool sdlInitialised = false;
		bool quit = false;
		bool vsyncEnabled = false;

		~Impl() { Shutdown(); }

		//Shuts-down Main Engine Implementation.
		void Shutdown() noexcept {
			window.reset();
			if (sdlInitialised)
				SDL_Quit();
			sdlInitialised = false;
		}

		//Listens to SDL events.
		void PollEvents() {
			SDL_Event event;
			while (SDL_PollEvent(&event)) {
				if (event.type == SDL_EVENT_QUIT || event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
					quit = true;
				}
			}
		}
	};

	Engine::Engine() : impl(std::make_unique<Impl>()) {}
	Engine::~Engine() = default;

	//This is mostly SDL boilerplate. Don't worry about it too much.
	bool Engine::Initialise(const char* title, int width, int height) 
	{
		if (impl->sdlInitialised || !title || width <= 0 || height <= 0) //Early exit if SDL is already initilised or if any window arguments are invalid.
			return false;

		if (!SDL_Init(SDL_INIT_VIDEO)) 
		{
			std::fprintf(stderr, "SDL_Init: %s\n", SDL_GetError());
			return false; //Return false if SDL fails to init.
		}
		impl->sdlInitialised = true;

		impl->window.reset(SDL_CreateWindow(title, width, height, 0)); //Create new window.
		if (!impl->window) 
		{
			std::fprintf(stderr, "SDL_CreateWindow: %s\n", SDL_GetError());
			impl->Shutdown();
			return false; //Return false if SDL window fails to create.
		}

		return true;
	}
	 
	int Engine::Run(Application& application)
	{
		/*if (!renderer.IsActive()) return 1;*/
		impl->quit = false;
		using Clock = std::chrono::steady_clock;
		auto previousTime = Clock::now();
		double accumulator = 0.0;
		int result = 0;

		try 
		{
			if (!application.Start(*this)) 
			{
				std::fputs("Application start failed\n", stderr);
				result = 1;
			}

			//Core engine loop
			while (result == 0 && !impl->quit)
			{
				impl->PollEvents();

				if (impl->quit) break; //break loop if application has been quit.

				const auto now = Clock::now();
				const double elapsed = std::chrono::duration<double>(now - previousTime).count();
				previousTime = now;

				accumulator += elapsed;
				while (accumulator >= fixedStepTime && !impl->quit) {
					application.FixedUpdate(*this, static_cast<float>(fixedStepTime)); //Call FixedUpdate once the time waiting has accumulated to the fixedStepTime.
					accumulator -= fixedStepTime;
				}
				if (impl->quit) break;

				application.Render(*this);

				//bool result = SDL_RenderPresent(impl_->renderer.get());
				//renderer.RenderPresent();

				//If we're not using vsync, we need to manually delay to avoid a busy loop that maxes CPU usage.
				if (!impl->vsyncEnabled)
					SDL_Delay(1);
			}
		}
		catch (const std::exception& error) {
			std::fprintf(stderr, "Engine run failed: %s\n", error.what());
			result = 1;
		}



		// Stop also runs after a failed Start so partial application setup can unwind.
		try {
			application.Stop(*this);
		}
		catch (const std::exception& error) {
			std::fprintf(stderr, "Application stop failed: %s\n", error.what());
			result = 1;
		}
		return result;
	}

	void Engine::Quit() noexcept { impl->quit = true; }
}


