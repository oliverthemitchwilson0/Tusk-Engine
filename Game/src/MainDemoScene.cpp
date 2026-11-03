#include <TuskGame/MainDemoScene.h>

#include <Tusk/Logging.h>

namespace TuskGame
{
	bool MainDemoScene::Start()
	{
		Tusk::Logging::Log("Main Demo Scene Start");
		return true;
	}

	void MainDemoScene::FixedUpdate(Tusk::Input& input, float fixedDeltaTime)
	{
		Tusk::Logging::Log("Main Demo Scene Fixed Update");
	}

	void MainDemoScene::Render(Tusk::Renderer& renderer)
	{
		Tusk::Logging::Log("Main Demo Scene Render");
	}

	void MainDemoScene::Stop()
	{
		
	}
}