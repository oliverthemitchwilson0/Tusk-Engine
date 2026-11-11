#include <Tusk/Engine.h>
#include <Tusk/Logging.h>

#include <SDL3/SDL.h>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlgpu3.h>
#include <chrono>

//To-Do (Stretch)
//* Create a queue of GPU uploads instead of submiting each upload one at a time.
//* Change the Asset Class.

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
		bool debugMenu = false;
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
		void PollEvents(Input& mInput) {
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
				else if(event.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
				{
					mInput.PushMouseEvent(MouseEvent{
						.button = event.button.button,
						.down = event.button.down,
						}
					);
				}
				else if (event.type == SDL_EVENT_MOUSE_BUTTON_UP)
				{
					mInput.PushMouseEvent(MouseEvent{
						.button = event.button.button,
						.down = event.button.down,
						}
					);
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
		/*style.FrameRounding = 3.0f;
		style.GrabRounding = 3.0f;*/

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

			//Core engine loop
			while (result == 0 && !impl->quit)
			{
				impl->PollEvents(mInput);

				mSceneManager.Process();

				mInput.Process();

				//Input Test
				if (mInput.GetKeyPressed(Tusk::ESCAPE))
					impl->quit = true;

				if(mInput.GetKeyPressed(Tusk::F11))
					SDL_SetWindowFullscreen(impl->window.get(), true);

				if (mInput.GetKeyPressed(Tusk::F3))
					impl->debugMenu = !impl->debugMenu;

				if (impl->quit) break; //break loop if application has been quit.

				const auto now = Clock::now();
				const double elapsed = std::chrono::duration<double>(now - previousTime).count();
				previousTime = now;

				if (Asset::AssetsFreshlyLoaded())
				{
					Logging::Log("Pushing new assets to renderer.");
					Asset::PushAssetsToRenderer(GetRenderer());
				}

				mSceneManager.HandleInput(GetInput());

				accumulator += elapsed;
				while (accumulator >= fixedStepTime && !impl->quit) {
					mSceneManager.FixedUpdate(static_cast<float>(fixedStepTime));
					accumulator -= fixedStepTime;
				}
				if (impl->quit) break;

				if(impl->windowResized)
				{
					impl->windowResized = false;
					GetRenderer().ResizeDepthTexture();
				}
				
				mRenderer.Clear();
				mSceneManager.Render(GetRenderer());
				ProcessDebugUI();
				mRenderer.Present();

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

	void Engine::ProcessDebugUI()
	{
		ImGui_ImplSDLGPU3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();

		if(impl->debugMenu)
		{
			ImGui::Begin("Tusk Debug Menu");

			ImGui::Text("Framerate: %.1f", ImGui::GetIO().Framerate);
			ImGui::Text("GPU Driver: %s", mRenderer.GetDeviceDriverName().c_str());

			if (ImGui::CollapsingHeader("Scenes"))
			{
				auto sceneCount = mSceneManager.GetSceneCount();

				for (int i = 0; i < sceneCount; i++)
				{
					std::string buttonName = std::string("Scene ") + std::to_string(i);
					if (ImGui::Button(buttonName.c_str()))
					{
						mSceneManager.SetScene(i);
					}
				}
			}
			ImGui::End();
		}

		ImGui::Render();
	}
}


