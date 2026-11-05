#include <Tusk/Engine.h>
#include <Tusk/Logging.h>

#include <SDL3/SDL.h>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlgpu3.h>
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
		bool windowResized = false;

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
				ImGui_ImplSDL3_ProcessEvent(&event); //pass event data to ImGui impl.
				if (event.type == SDL_EVENT_QUIT || event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
					quit = true;
				}
				else if(event.type == SDL_EVENT_WINDOW_RESIZED)
				{
					windowResized = true;
				}
			}
		}
	};

	Engine::Engine() : impl(std::make_unique<Impl>()) {}
	Engine::~Engine() = default;

	bool ImGuiInitilise(SDL_Window* window)
	{
		//ImGui init.
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls

		// Setup Dear ImGui style
		ImGuiStyle& style = ImGui::GetStyle();
		ImGui::StyleColorsDark();
		style.FrameRounding = 3.0f;
		style.GrabRounding = 3.0f;

		// Setup scaling
		auto mainScale = SDL_GetWindowDisplayScale(window);
		style.ScaleAllSizes(mainScale);        // Bake a fixed style scale. (until we have a solution for dynamic style scaling, changing this requires resetting Style + calling this again)
		style.FontScaleDpi = mainScale;        // Set initial font scale. (in docking branch: using io.ConfigDpiScaleFonts=true automatically overrides this for every window depending on the current monitor)
	
		return true;
	}

	//This is mostly SDL boilerplate. Don't worry about it too much.
	bool Engine::Initialise(const char* title, int width, int height) 
	{
		if (impl->sdlInitialised || !title || width <= 0 || height <= 0) //Early exit if SDL is already initilised or if any window arguments are invalid.
			return false;

		if (!SDL_Init(SDL_INIT_VIDEO)) 
		{
			Logging::Log("SDL Failed to Initlise: %s", SDL_GetError());
			return false; //Return false if SDL fails to init.
		}
		impl->sdlInitialised = true;

		auto windowFlags = SDL_WINDOW_RESIZABLE;
		impl->window.reset(SDL_CreateWindow(title, width, height, windowFlags)); //Create new window.
		if (!impl->window) 
		{
			Logging::Log("SDL Failed to Create Window: %s", SDL_GetError());
			impl->Shutdown();
			return false; //Return false if SDL window fails to create.
		}

		if(!ImGuiInitilise(impl->window.get()))
		{
			Logging::Log("ImGui Failed to initilise");
			impl->Shutdown();
			return false;
		}
		
		bool debugActive = true;
		if(!mRenderer.Initilise(impl->window.get(), debugActive))
		{
			impl->Shutdown();
			return false;
		}

		return true;
	}
	 
	int Engine::Run(Application& application)
	{
		impl->quit = false;
		using Clock = std::chrono::steady_clock;
		auto previousTime = Clock::now();
		double accumulator = 0.0;
		int result = 0;

		try 
		{
			if (!application.Start(mSceneManager)) 
			{
				std::fputs("Application start failed\n", stderr);
				result = 1;
			}

			//After game Start(), upload any imported meshses and textures to the renderer.
			Asset::PushAssetsToRenderer(GetRenderer()); // This will have to change since scenes can load anytime.

			//Core engine loop
			while (result == 0 && !impl->quit)
			{
				impl->PollEvents();

				mInput.ProcessInput();

				//Input Test
				if (mInput.GetKeyPressed(Tusk::ESCAPE))
					impl->quit = true;

				if(mInput.GetKeyPressed(Tusk::BACKSPACE))
					SDL_SetWindowFullscreen(impl->window.get(), true);

				if (mInput.GetKeyPressed(Tusk::G))
					mSceneManager.SetScene("Second");
				else if (mInput.GetKeyPressed(Tusk::H))
					mSceneManager.SetScene("Main");
				

				if (impl->quit) break; //break loop if application has been quit.

				const auto now = Clock::now();
				const double elapsed = std::chrono::duration<double>(now - previousTime).count();
				previousTime = now;

				accumulator += elapsed;
				while (accumulator >= fixedStepTime && !impl->quit) {
					application.FixedUpdate(GetInput(), static_cast<float>(fixedStepTime)); //Call FixedUpdate once the time waiting has accumulated to the fixedStepTime.
					mSceneManager.FixedUpdate(GetInput(), static_cast<float>(fixedStepTime));
					accumulator -= fixedStepTime;
				}
				if (impl->quit) break;

				if(impl->windowResized)
				{
					impl->windowResized = false;
					GetRenderer().ResizeDepthTexture();
				}

				//ImGui Test - This should get tidied away somewhere.
				ImGui_ImplSDLGPU3_NewFrame();
				ImGui_ImplSDL3_NewFrame();
				ImGui::NewFrame();

				ImGui::ShowDemoWindow();

				ImGui::Render();

				mRenderer.Clear();
				mSceneManager.Render(GetRenderer());
				mRenderer.Present();

				//bool result = SDL_RenderPresent(impl_->renderer.get());
				//renderer.RenderPresent();

				//If we're not using vsync, we need to manually delay to avoid a busy loop that maxes CPU usage.
				if (!impl->vsyncEnabled)
					SDL_Delay(1);
			}
		}
		catch (const std::exception& error) {
			Logging::Log("Engine run failed: %s", error.what());
			result = 1;
		}



		// Stop also runs after a failed Start so partial application setup can unwind.
		try {
			application.Stop(mSceneManager);
		}
		catch (const std::exception& error) {
			Logging::Log("Application stop failed: %s", error.what());
			result = 1;
		}
		return result;
	}

	void Engine::Quit() noexcept { impl->quit = true; }

	Renderer& Engine::GetRenderer()
	{
		return mRenderer;
	}

	Input& Engine::GetInput()
	{
		return mInput;
	}
}


