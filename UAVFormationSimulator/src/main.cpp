#include "MainStage.h"

class Simulator : public Runner
{
public:
	Simulator()
	{
		Settings s;
		s.window.title = "UAV Formation Simulator";
		s.window.width = 1000;
		s.window.height = 800;
		SetSettings(s);
	}

	void OnReady() override
	{
		RefPtr<MainStage> stage = new MainStage;
		Director::GetInstance().EnterStage(stage);
	}
};

int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
#ifdef KGE_DEBUG
	// Enable memory-leak reports
	_CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG));
#endif

	try
	{
		RefPtr<Simulator> simulator = new Simulator;
		Application::GetInstance().Run(simulator);
	}
	catch (std::exception & e)
	{
		::MessageBoxA(nullptr, e.what(), "An exception has occurred!", MB_ICONERROR | MB_OK);
	}
	return 0;
}
