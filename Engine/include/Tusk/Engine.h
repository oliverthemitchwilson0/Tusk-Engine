#pragma once

#include <Tusk/Renderer.h>
#include <memory>

namespace Tusk 
{
	class Engine;

	//Application is for the end developer to build with Tusk.
	//Lifetime managed by engine.
	class Application {
	public:
		virtual ~Application() = default;
		virtual bool Start(Engine& engine) = 0;
		virtual void FixedUpdate(Engine& engine, float dt) = 0;
		virtual void Render(Engine& engine) = 0;
		virtual void Stop(Engine& engine) = 0;
	};

	class Engine final
	{
	public:
		Engine();
		~Engine();

		//Delete Copy & Move Constructors to ensure single-instance resourses ramain unique (Such as SDL).
		Engine(const Engine&) = delete;
		Engine& operator=(const Engine&) = delete;
		Engine(Engine&&) = delete;
		Engine& operator=(Engine&&) = delete;

		bool    Initialise(const char* title, int width = 640, int height = 480);
		int     Run(Application& application);
		void    Quit() noexcept;

	private:
		struct Impl;
		std::unique_ptr<Impl> impl;

		Renderer renderer;
	};
}