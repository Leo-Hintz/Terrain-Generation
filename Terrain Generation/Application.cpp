#include "Application.h"

void Application::Run()
{
	Window::Init();
	Window* window = Window::CreateWindow(1920, 1080, "Window");
	
	//Main loop
	while (!window->ShouldBeClosed())
	{
		window->OnUpdate();
	}

	delete window;
}
