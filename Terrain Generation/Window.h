#pragma once
#include <GLFW/glfw3.h>
#include <memory>

struct WindowData
{
	bool closed;
};

class Window
{
public:
	static void Init()
	{
		glfwInit();
	}
	static Window* CreateWindow(uint32_t width, uint32_t height, const char* title)
	{
		Window* window = (Window*)malloc(sizeof(Window));
		window->m_Window = glfwCreateWindow(width, height, title, nullptr, nullptr);
		window->m_Data.closed = false;
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
	}
private:
	Window(uint32_t width, uint32_t height, const char* title)
	{
		m_Window = glfwCreateWindow(width, height, title, nullptr, nullptr);
	}
	GLFWwindow* m_Window;
	WindowData m_Data;
};