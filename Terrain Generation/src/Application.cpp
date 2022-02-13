#include "Application.h"
#include "Renderer.hpp"
#include "Window.h"
#include "Mesh.h"

void Application::Run()
{
	
	Window::Init();
	Window* window = Window::CreateWindow(1920, 1080, "Window");
	
	Renderer::Init();
	Shader shader;
	shader.AttachVertexShader("Shaders/shader.vert");
	shader.AttachFragmentShader("Shaders/shader.frag");
	shader.LinkShaders();

	CameraController cameraController = CameraController(1920, 1080);
	uint32_t indices[36] =
	{
		0, 1, 3, 3, 1, 2,
		1, 5, 2, 2, 5, 6,
		5, 4, 6, 6, 4, 7,
		4, 0, 7, 7, 0, 3,
		3, 2, 7, 7, 2, 6,
		4, 5, 0, 0, 5, 1
	};
	Vertex vertices[8] =
	{
		{{-1,-1,-1, 1},{0,0},{}},
		{{1,-1,-1,1},{1,0},{}},
		{{1,1,-1,1},{1,1},{}},
		{{-1,1,-1,1},{0,1},{}},
		{{-1,-1,1,1},{0,0},{}},
		{{1,-1,1,1},{1,0},{}},
		{{1,1,1,1},{1,1},{}},
		{{-1,1,1,1},{0,0},{}}
	};
	Terrain* terrain = TerrainGenerator::GenerateTerrainMap("Textures/Heightmap.jpg");
	Mesh* mesh = Mesh::CreateMesh(vertices, 8, indices, 36);
	Texture tex = Texture("Textures/Flag_of_Australia.svg.png");

	//Main loop
	while (!window->ShouldBeClosed())
	{
		terrain->Draw();
		mesh->Draw();

		for(int i = 0; i < Renderer::m_Meshlist.size(); ++i)
		Renderer::Render(Renderer::m_Meshlist.front(), &shader, &cameraController, &tex);
		for (int i = 0; i < Renderer::m_Terrainlist.size(); ++i)
			Renderer::Render(Renderer::m_Terrainlist.front(), &shader, &cameraController, &tex);

		cameraController.OnUpdate(window);
		window->OnUpdate();
	}
	delete window;
	Window::Terminate();
}