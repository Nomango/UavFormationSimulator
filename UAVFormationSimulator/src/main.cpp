#include "MainStage.h"

KGE_DECLARE_SMART_PTR(Simulator);
class Simulator : public Runner
{
public:
	Simulator()
	{
		WindowPtr window = Window::Create("UAV Formation Simulator", 1000, 800);
		SetMainWindow(window);
	}

	void OnReady() override
	{
		MainStagePtr stage = MainStage::Create();
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
		SimulatorPtr simulator = new Simulator;
		Application::GetInstance().Run(simulator);
	}
	catch (std::exception & e)
	{
		::MessageBoxA(nullptr, e.what(), "An exception has occurred!", MB_ICONERROR | MB_OK);
	}
	return 0;
}
