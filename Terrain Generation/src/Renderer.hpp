#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <queue>

#include "Window.h"
#include "Mesh.h"
#pragma warning(disable:4996) //Disable warning in order to be able to use C-style file system


class Texture
{
public:
	Texture(const char* fileDirectory);
	const GLuint& GetTexID() { return m_TextureID; }
private:
	GLuint m_TextureID;
	int32_t m_Width, m_Height, m_NumberOfChannels;
};

class CubeMap
{
public:
};


class Shader
{
public:
	Shader()
	{
		m_Program = glCreateProgram();
	}
	void AttachVertexShader(const char* filePath);
	void AttachFragmentShader(const char* filePath);
	void LinkShaders();
	void Use();
	void SetFloat(const char* name, float& value) const { glUniform1f(glGetUniformLocation(m_Program, name), value); }
	void SetVec2(const char* name, float& x, float& y) const { glUniform2f(glGetUniformLocation(m_Program, name), x, y); }
	void SetVec3(const char* name, float& x, float& y, float& z) const { glUniform3f(glGetUniformLocation(m_Program, name), x, y, z); }
	void SetVec4(const char* name, float& x, float& y, float& z, float& w) const { glUniform4f(glGetUniformLocation(m_Program, name), x, y, z, w); }
	void SetMat4(const char* name, const float* value) const { glUniformMatrix4fv(glGetUniformLocation(m_Program, name), 1, GL_FALSE, value); }
	void SetMat3(const char* name, const float* value) const { glUniformMatrix3fv(glGetUniformLocation(m_Program, name), 1, GL_FALSE, value); }
	void BindTex2D0(const GLuint& id) { glActiveTexture(GL_TEXTURE0); glBindTexture(GL_TEXTURE_2D, id); }
	void BindTex2D1(const GLuint& id) { glActiveTexture(GL_TEXTURE1); glBindTexture(GL_TEXTURE_2D, id); }
	void BindTex2D2(const GLuint& id) { glActiveTexture(GL_TEXTURE2); glBindTexture(GL_TEXTURE_2D, id); }
	void BindTex2D3(const GLuint& id) { glActiveTexture(GL_TEXTURE3); glBindTexture(GL_TEXTURE_2D, id); }
	void BindTex2D4(const GLuint& id) { glActiveTexture(GL_TEXTURE4); glBindTexture(GL_TEXTURE_2D, id); }
	void BindTex2D5(const GLuint& id) { glActiveTexture(GL_TEXTURE5); glBindTexture(GL_TEXTURE_2D, id); }
	void BindTex2D6(const GLuint& id) { glActiveTexture(GL_TEXTURE6); glBindTexture(GL_TEXTURE_2D, id); }
	void BindTex2D7(const GLuint& id) { glActiveTexture(GL_TEXTURE7); glBindTexture(GL_TEXTURE_2D, id); }
	void BindTex2D8(const GLuint& id) { glActiveTexture(GL_TEXTURE8); glBindTexture(GL_TEXTURE_2D, id); }
private:
	GLuint m_Program;
	GLuint m_VertexShader;
	GLuint m_FragmentShader;
};

class CameraController
{
public:
	CameraController(float width, float height)
	{
		m_ProjectionMatrix = glm::perspective(90.0f, width / height, 0.1f, 800.0f);
		m_ViewMatrix = glm::lookAt(glm::vec3(0, 0, 0), glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));
	}
	const glm::mat4& GetViewMatrix() { return m_ViewMatrix; }
	const glm::mat4& GetProjectionMatrix() { return m_ProjectionMatrix; }
	void Use();
	void Move(float x, float y, float z);
	void Rotate(int x, int y);
	void OnUpdate(Window* window);
private:
	glm::mat4 m_ProjectionMatrix;
	glm::mat4 m_ViewMatrix;
	bool m_CursorDisabled = false;
	float yaw = 0, pitch = 0;
	glm::vec3 m_Pos{}, m_Dir{0, 0,-1}, m_Right{1, 0, 0}, m_Up{0, 1, 0};
};

class Renderer
{
public:
	static void Init();
	static void Render(Terrain* terrainMesh, Shader* shader, CameraController* cameraControlller, Texture* texture);
	static void Render(Mesh* mesh, Shader* shader, CameraController* cameraController, Texture* texture);
	static void Submit(Mesh* mesh);
	static void Submit(Terrain* terrainMesh);
	static std::queue<Mesh*> m_Meshlist;
	static std::queue <Terrain*> m_Terrainlist;
};

class TerrainGenerator
{
public:
	static Terrain* GenerateTerrainMap(const char* imageDirectory);
};