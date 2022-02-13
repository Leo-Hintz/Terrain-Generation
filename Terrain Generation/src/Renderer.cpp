#include "Renderer.hpp"
#include "stb_image.h"

Texture::Texture(const char* fileDirectory)
{
	unsigned char* data = stbi_load(fileDirectory, &m_Width, &m_Height, &m_NumberOfChannels, 0);

	glGenTextures(1, &m_TextureID);
	glBindTexture(GL_TEXTURE_2D, m_TextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	if (m_NumberOfChannels == 4)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
	else if (m_NumberOfChannels == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(data);
}

void Shader::AttachVertexShader(const char* filePath)
{
	FILE* file;
	file = fopen(filePath, "r");
	if (!file)
	{
		std::cout << filePath << " is an invalid file name\n";
		return;
	}
	char* str = (char*)_alloca(2048);
	int i = 0;
	while (true)
	{
		str[i] = fgetc(file);
		if (str[i] == EOF)
		{
			str[i] = '\0';
			break;
		}
		++i;
	}
	glDetachShader(m_Program, m_VertexShader);
	m_VertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(m_VertexShader, 1, &str, 0);
	glCompileShader(m_VertexShader);
	int status;
	glGetShaderiv(m_VertexShader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		int messageLength;
		glGetShaderiv(m_VertexShader, GL_INFO_LOG_LENGTH, &messageLength);
		char* message = (char*)_alloca(messageLength);
		glGetShaderInfoLog(m_VertexShader, messageLength, 0, message);
		std::cout << message;
	}
	glAttachShader(m_Program, m_VertexShader);
	glDeleteShader(m_VertexShader);
}

void Shader::AttachFragmentShader(const char* filePath)
{
	FILE* file;
	file = fopen(filePath, "r");
	if (!file)
	{
		std::cout << filePath << " is an invalid file name\n";
		return;
	}
	char* str = (char*)_alloca(2048);
	int i = 0;
	while (true)
	{
		str[i] = fgetc(file);
		if (str[i] == EOF)
		{
			str[i] = '\0';
			break;
		}
		++i;
	}
	glDetachShader(m_Program, m_FragmentShader);
	m_FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(m_FragmentShader, 1, &str, 0);
	glCompileShader(m_FragmentShader);
	int status;
	glGetShaderiv(m_FragmentShader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		int messageLength;
		glGetShaderiv(m_VertexShader, GL_INFO_LOG_LENGTH, &messageLength);
		char* message = (char*)_alloca(messageLength);
		glGetShaderInfoLog(m_VertexShader, messageLength, 0, message);
		std::cout << message << "\n";
	}
	glAttachShader(m_Program, m_FragmentShader);
	glDeleteShader(m_FragmentShader);
}

void Shader::LinkShaders()
{
	int status;
	glLinkProgram(m_Program);
	glGetProgramiv(m_Program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		char message[128];
		glGetProgramInfoLog(m_Program, 128, 0, message);
	}
	glValidateProgram(m_Program);
	glGetProgramiv(m_Program, GL_VALIDATE_STATUS, &status);
	if (status == GL_FALSE)
	{
		char message[128];
		glGetProgramInfoLog(m_Program, 128, 0, message);
	}
}

void Shader::Use()
{
	glUseProgram(m_Program);
}

void CameraController::Use()
{
	glUniformMatrix4fv(1, 1, GL_FALSE, glm::value_ptr(m_ViewMatrix));
	glUniformMatrix4fv(0, 1, GL_FALSE, glm::value_ptr(m_ProjectionMatrix));
}

void CameraController::Move(float x, float y, float z)
{
	
}

void CameraController::OnUpdate(Window* window)
{
	if (glfwGetKey(window->GetWindow(), GLFW_KEY_ESCAPE))
	{
		glfwSetInputMode(window->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		m_CursorDisabled = false;
	}
	if (glfwGetMouseButton(window->GetWindow(), 0))
	{
		glfwSetInputMode(window->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		m_CursorDisabled = true;
	}
	if (!m_CursorDisabled)
		return;
	if (glfwGetKey(window->GetWindow(), GLFW_KEY_W))
	{
		m_Pos += normalize(glm::vec3(m_Dir.x, 0, m_Dir.z));
	}
	if (glfwGetKey(window->GetWindow(), GLFW_KEY_S))
	{
		m_Pos -= normalize(glm::vec3(m_Dir.x, 0, m_Dir.z));
	}
	if (glfwGetKey(window->GetWindow(), GLFW_KEY_A))
	{
		m_Pos -= m_Right;
	}
	if (glfwGetKey(window->GetWindow(), GLFW_KEY_D))
	{
		m_Pos += m_Right;
	}
	if (glfwGetKey(window->GetWindow(), GLFW_KEY_LEFT_CONTROL))
	{
		--m_Pos.y;
	}
	if (glfwGetKey(window->GetWindow(), GLFW_KEY_SPACE))
	{
		++m_Pos.y;
	}
	if (!(window->GetData().cursorPos == window->GetData().oldCursorPos))
	{


		yaw += window->GetData().cursorPos.x - window->GetData().oldCursorPos.x;
		pitch += window->GetData().oldCursorPos.y - window->GetData().cursorPos.y;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
		m_Dir = glm::normalize(glm::vec3(glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch)), glm::sin(glm::radians(pitch)), glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch))));
		m_Right = glm::normalize(glm::cross(m_Dir, glm::vec3(0, 1, 0)));
		m_Up = glm::normalize(glm::cross(m_Right, m_Dir));
	}
	m_ViewMatrix = glm::lookAt(m_Pos, m_Dir + m_Pos, m_Up);
}


std::queue<Mesh*> Renderer::m_Meshlist = {}; 
std::queue<Terrain*> Renderer::m_Terrainlist = {}; 
void Renderer::Init()
{
	glewInit();
	glClearColor(0.1f, .24f, .74f, 1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
}

void Renderer::Render(Terrain* terrainMesh, Shader* shader, CameraController* cameraController, Texture* texture)
{
	shader->Use();
	terrainMesh->Use();
	shader->SetMat4("viewMatrix", glm::value_ptr(cameraController->GetViewMatrix()));
	shader->SetMat4("projectionMatrix", glm::value_ptr(cameraController->GetProjectionMatrix()));
	shader->BindTex2D0(texture->GetTexID());
	for (int i = 0; i < terrainMesh->GetHeight() - 1; ++i)
	{
		glDrawElements(GL_TRIANGLE_STRIP, terrainMesh->GetWidth() * 2, GL_UNSIGNED_INT, (void*)(sizeof(uint32_t) * terrainMesh->GetWidth() * 2 * i));
	}
	m_Terrainlist.pop();
}

void Renderer::Render(Mesh* mesh, Shader* shader, CameraController* cameraController, Texture* texture)
{
	shader->Use();
	mesh->Use();
	shader->SetMat4("viewMatrix", glm::value_ptr(cameraController->GetViewMatrix()));
	shader->SetMat4("projectionMatrix", glm::value_ptr(cameraController->GetProjectionMatrix()));
	shader->BindTex2D0(texture->GetTexID());
	glDrawElements(GL_TRIANGLES, mesh->GetIndexCount(), GL_UNSIGNED_INT, 0);
	m_Meshlist.pop();
}

void Renderer::Submit(Mesh* mesh)
{
	m_Meshlist.push(mesh);
}

void Renderer::Submit(Terrain* terrainMesh)
{
	m_Terrainlist.push(terrainMesh);
}

Terrain* TerrainGenerator::GenerateTerrainMap(const char* imageDirectory)
{
	int channels, x, y;
	unsigned char* data = stbi_load(imageDirectory, &x, &y, &channels, 0);
	Vertex* vertices = (Vertex*)malloc(sizeof(Vertex) * x * y);
	for (int i = 0; i < y; i++)
	{
		for (int j = 0; j < x; j++)
		{
			vertices[(x * i) + j] = { {-y / 2 + i ,(data[(j + x * i) * channels]) / 7, -x / 2 + j, 1}, {float(x-i)/x, float(y-j)/y}, {} };
		}
	}
	uint32_t* indices = (uint32_t*)malloc(sizeof(uint32_t) * x * y * 2 - 1);
	int n = 0;
	for (uint32_t i = 0; i < y - 1; i++)
	{
		for (uint32_t j = 0; j < x; j++)
		{
			for (uint32_t k = 0; k < 2; k++)
			{
				indices[n] = (j + x * (i + k));
				n++;
			}
		}
	}
	Terrain* mesh = Terrain::CreateTerrain(vertices, x, indices, y);
	stbi_image_free(data);
	return mesh;
}