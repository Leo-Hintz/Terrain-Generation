#pragma once
#include <GLFW/glfw3.h>
#include <memory>
#include <iostream>

struct WindowData
{
	bool closed;
	uint32_t x;
	uint32_t y;
};

class Window
{
public:
	static void Init()
	{
		glfwInit();
	}
	static void Terminate()
	{
		glfwTerminate();
	}
	static Window* CreateWindow(uint32_t width, uint32_t height, const char* title)
	{
		//Construct instance of window
		Window* window = (Window*)malloc(sizeof(Window));
		window->m_Window = glfwCreateWindow(width, height, title, nullptr, nullptr);
		window->m_Data.closed = false;
		window->m_Data.x = width;
		window->m_Data.y = height;

		//Set up relationship to glfw
		glfwSetWindowUserPointer(window->m_Window, (void*)&window->m_Data);
		glfwSetWindowCloseCallback(window->m_Window, [](GLFWwindow* window)
			{
				WindowData* windowData = (WindowData*)glfwGetWindowUserPointer(window);
				windowData->closed = true;
			});
		glfwMakeContextCurrent(window->m_Window);

		return window;
	}
	bool ShouldBeClosed()
	{
		return m_Data.closed;
	}
	void OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
		glClear(GL_COLOR_BUFFER_BIT);
	}
private:
	Window(uint32_t width, uint32_t height, const char* title)
	{
		m_Window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	}
	GLFWwindow* m_Window;
	WindowData m_Data;
};