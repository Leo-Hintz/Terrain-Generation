#include "Application.h"
#include "Renderer.h"
#include "Window.h"

#include <queue> //for creating a rendering queue

void Application::Run()
{
	
	Window::Init();
	Window* window = Window::CreateWindow(1920, 1080, "Window");
	
	Renderer::Init();
	Shader shader;
	shader.AttachVertexShader("Shaders/shader.vert");
	shader.AttachFragmentShader("Shaders/shader.frag");
	shader.LinkShaders();

	Mesh* terrain = TerrainGenerator::GenerateTerrainMap("Textures/Heightmap.png");
	

	//Main loop
	while (!window->ShouldBeClosed())
	{
		window->OnUpdate();
		Renderer::Render(terrain, &shader);
	}
	delete window;
	Window::Terminate();
}
