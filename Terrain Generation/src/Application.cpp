#include "Application.h"
#include "Renderer.hpp"
#include "Window.h"
#include "Mesh.h"
#include "Timer.h"

constexpr int WIDTH = 1280, HEIGHT = 720;

void Application::Run()
{
	
	Window::Init();
	Window* window = Window::CreateWindow(WIDTH, HEIGHT, "Window");
	
	Renderer::Init();
	Shader shader;
	shader.AttachVertexShader("Shaders/shader.vert");
	shader.AttachFragmentShader("Shaders/shader.frag");
	shader.LinkShaders();
	Shader skyboxShader;
	skyboxShader.AttachVertexShader("Shaders/Skyboxshader.vert");
 	skyboxShader.AttachFragmentShader("Shaders/Skyboxshader.frag");
	skyboxShader.LinkShaders();
	
	CameraController cameraController = CameraController(WIDTH, HEIGHT);
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

	std::array<const char*, 6> skyboxTextures
	{
		"Textures/skybox/right.jpg",		
		"Textures/skybox/left.jpg",
		"Textures/skybox/top.jpg",
		"Textures/skybox/bottom.jpg",
		"Textures/skybox/front.jpg",
		"Textures/skybox/back.jpg",
		
	};
	Terrain* terrain = TerrainGenerator::GenerateTerrainMap("Textures/Heightmap.jpg");
	Mesh* mesh = Mesh::CreateMesh(vertices, 8, indices, 36);
	Cube* skybox = Cube::CreateSkybox();
	Texture tex = Texture("Textures/Default.jpg");
	CubeMap cubeMap = CubeMap(skyboxTextures);

	//Main loop
	while (!window->ShouldBeClosed())
	{
		terrain->Draw();
		mesh->Draw();
		Renderer::RenderSkybox(skybox, &skyboxShader, &cameraController, &cubeMap);
		for(int i = 0; i < Renderer::m_Meshlist.size(); ++i)
		Renderer::Render(Renderer::m_Meshlist.front(), &shader, &cameraController, &tex);
		for (int i = 0; i < Renderer::m_Terrainlist.size(); ++i)
			Renderer::Render(Renderer::m_Terrainlist.front(), &shader, &cameraController, &tex);

		cameraController.OnUpdate(window);
		window->OnUpdate();
		Time::OnUpdate();
	}
	delete window;
	Window::Terminate();
}