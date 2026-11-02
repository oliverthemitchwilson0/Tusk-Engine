#pragma once
#include <Tusk/Logging.h>

namespace Tusk 
{
	class Scene
	{
	public:
		Scene() = default;
		virtual ~Scene() = default;

		virtual bool Start() { Logging::Log("Scene Started."); return true; };
		virtual void FixedUpdate() { Logging::Log("Scene Updated. (Fixed)"); };
		virtual void Render() { Logging::Log("Scene Rendered."); };
		virtual void Stop() { Logging::Log("Scene Stopped."); };
	private:
		//
	};
}