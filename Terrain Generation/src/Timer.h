#pragma once
#include <GLFW/glfw3.h>
class Time
{
	friend class Application;
public:
	static const float& GetDeltaTime() { return m_DeltaTime; }
	static const float& GetTimeElapsed() { return m_TotalTime; }
private:
	static void OnUpdate()
	{
		float previousTime = m_TotalTime;
		m_TotalTime = glfwGetTime();
		m_DeltaTime = m_TotalTime - previousTime;
	}
	static float m_DeltaTime, m_TotalTime;
};