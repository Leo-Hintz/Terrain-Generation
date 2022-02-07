#pragma once
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <stb_image.h>

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>

#pragma warning(disable:4996)

struct Vertex
{
	glm::vec4 pos;
	glm::vec3 norm;
	glm::vec2 tex;
};

class Mesh
{
public:
	static Mesh* CreateMesh(Vertex* vertices, uint32_t vertexCount, uint32_t* indices, uint32_t indexCount) //Creation method enforces heap allocation, needed for large amounts of data
	{
		Mesh* mesh = (Mesh*)malloc(sizeof(Mesh));
		mesh->m_Vertices = vertices;
		mesh->m_Indices = indices;
		mesh->m_IndexCount = indexCount;
		mesh->m_IndexCount = vertexCount;

		glGenVertexArrays(1, &mesh->m_VertexArray);
		glBindVertexArray(mesh->m_VertexArray);

		glGenBuffers(1, &mesh->m_VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->m_VertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		glEnableVertexAttribArray(0);


		glGenBuffers(1, &mesh->m_IndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->m_IndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(uint32_t), indices, GL_STATIC_DRAW);
		return mesh;
	}
	inline Vertex* GetVertices()
	{
		return m_Vertices;
	}
	inline uint32_t& GetVertexCount()
	{
		return m_VertexCount;
	}
	inline void SetVertices(Vertex* newVertices)
	{
		m_Vertices = newVertices;
	}
	inline uint32_t* GetIndices()
	{
		return m_Indices;
	}
	inline uint32_t& GetIndexCount()
	{
		return m_IndexCount;
	}

	void Use()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBuffer);
	}
private:
	Vertex* m_Vertices;
	uint32_t* m_Indices;
	uint32_t m_IndexCount;
	uint32_t m_VertexCount;
	GLuint m_VertexArray;
	GLuint m_VertexBuffer;
	GLuint m_IndexBuffer;
};

class Shader
{
public:
	Shader()
	{
		m_Program = glCreateProgram();
	}
	void AttachVertexShader(const char* filePath)
	{
		FILE* file;
		file = fopen(filePath, "r");
		if (!file)
		{
			std::cout << filePath << " is an invalid file name\n";
			return;
		}
		char* str = (char*)alloca(512);
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
		std::cout << str << "\n";
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
			char* message = (char*)alloca(messageLength);
			glGetShaderInfoLog(m_VertexShader, messageLength, 0, message);
			std::cout << message;
		}
		glAttachShader(m_Program, m_VertexShader);
		glDeleteShader(m_VertexShader);
	}
	void AttachFragmentShader(const char* filePath)
	{
		FILE* file;
		file = fopen(filePath, "r");
		if (!file)
		{
			std::cout << filePath << " is an invalid file name\n";
			return;
		}
		char* str = (char*)alloca(512);
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
		std::cout << str << "\n";
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
				char* message = (char*)alloca(messageLength);
				glGetShaderInfoLog(m_VertexShader, messageLength, 0, message);
				std::cout << message << "\n";
			}
		glAttachShader(m_Program, m_FragmentShader);
		glDeleteShader(m_FragmentShader);
	}
	void LinkShaders()
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
	void Use()
	{
		glUseProgram(m_Program);
	}

private:
	GLuint m_Program;
	GLuint m_VertexShader;
	GLuint m_FragmentShader;
};
class Renderer
{
public:
	static void Init()
	{
		glewInit();
		glClearColor(0.1f, .24f, .74f, 1);
	}
	static void Render(Mesh* mesh, Shader* shader)
	{
		shader->Use();
		mesh->Use();
		for (int i = 0; i < 256; ++i)
		{
			glDrawElements(GL_TRIANGLE_STRIP, 257 * 2, GL_UNSIGNED_INT, (void*)(sizeof(uint32_t) * 257 * 2 * i));
		}
	}
};

class TerrainGenerator
{
public:
	static Mesh* GenerateTerrainMap(const char* imageDirectory)
	{
		int x, y, channels;
		unsigned char* data = stbi_load(imageDirectory, &x, &y, &channels, 0);
		Vertex* vertices = (Vertex*)malloc(sizeof(Vertex) * x * y);
		for (int i = 0; i < x; i++)
		{
			for (int j = 0; j < y; j++)
			{
				vertices[(y * i) + j] = {{-x/2 + i ,data[(y*i) + j] * channels, -y/2 + j, 1}, {}, {} };
			}
		}
		uint32_t* indices = (uint32_t*)malloc(sizeof(uint32_t) * x * y * 2 - 1);
		int n = 0;
			for(uint32_t i = 0; i < y-1; i++)
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
		Mesh* mesh = Mesh::CreateMesh(vertices, x * y, indices, x * y * 2 - 1);
		return mesh;
	}
};

class CameraController
{
public:
	void Use()
	{

	}
	void Move()
	{

	}
	void Rotate()
	{

	}
	void OnUpdate()
	{

	}
private:
	glm::mat4 m_ProjectionMatrix;
	glm::mat4 m_ViewMatrix;
};