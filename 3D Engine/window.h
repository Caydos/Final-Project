#ifndef WINDOW_H
#define WINDOW_H
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Keys.h"
#include "Colors.h"

class Window
{
public:
	Window()
	{
		window = nullptr;
		lastX = .0f;
		lastY = .0f;
		firstMouse = true;
	}
	~Window()
	{
		if (window != nullptr)
		{
			glfwDestroyWindow(window);
		}
	}
	GLFWwindow* GetWindow() { return this->window; }
	void SetTitle(const char* _title)
	{
		glfwSetWindowTitle(this->window, _title);
	}
	void Create(const char* _name, unsigned int _width, unsigned int _height)
	{
		lastX = _width / 2.0f;
		lastY = _height / 2.0f;
		firstMouse = true;

		window = glfwCreateWindow(_width, _height, _name, NULL, NULL);
		if (window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return;
		}
		glfwMakeContextCurrent(window);
		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetScrollCallback(window, scroll_callback);

		// glad: load all OpenGL function pointers
		// ---------------------------------------
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return;
		}
	}

	glm::ivec2 GetDimensions()
	{
		glm::ivec2 dimensions;
		glfwGetWindowSize(window, &dimensions.x, &dimensions.y);
		return dimensions;
	}

	static void framebuffer_size_callback(GLFWwindow* window, int width, int height)
	{
		// make sure the viewport matches the new window dimensions; note that width and 
		// height will be significantly larger than specified on retina displays.
		glViewport(0, 0, width, height);
	}


	// glfw: whenever the mouse moves, this callback is called
	// -------------------------------------------------------
	static void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
	{
		float xpos = static_cast<float>(xposIn);
		float ypos = static_cast<float>(yposIn);
		Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(window));

		if (instance->firstMouse)
		{
			instance->lastX = xpos;
			instance->lastY = ypos;
			instance->firstMouse = false;
		}

		//std::cout << instance->lastX << std::endl;
		instance->xoffset = xpos - instance->lastX;
		instance->yoffset = instance->lastY - ypos; // reversed since y-coordinates go from bottom to top

		instance->lastX = xpos;
		instance->lastY = ypos;
	}

	// glfw: whenever the mouse scroll wheel scrolls, this callback is called
	// ----------------------------------------------------------------------
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(window));
		instance->xScroll = static_cast<float>(xoffset);
		instance->yScroll = static_cast<float>(yoffset);
	}

	void Clear(Colors::Color _color = Colors::LimedSpruce)
	{
		glClearColor(_color.values[0], _color.values[1], _color.values[2], _color.values[3]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	void Events()
	{
		xoffset = .0f;
		yoffset = .0f;
		xScroll = .0f;
		yScroll = .0f;
		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	bool IsActive()
	{
		return !glfwWindowShouldClose(window);
	}
	void Close(bool _closure = true)
	{
		glfwSetWindowShouldClose(window, _closure);
	}

	void Focus(bool _focusState = true)
	{
		focus = _focusState;
		if (focus)
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
	}
	bool IsFocused()
	{
		return focus;
	}
	bool IsKeyPressed(Keys::Keys _keyId)
	{
		return (glfwGetKey(window, _keyId) == GLFW_PRESS);
	}
	bool IsKeyPressed(Keys::MouseButtons _keyId)
	{
		return (glfwGetMouseButton(window, _keyId) == GLFW_PRESS);
	}

	glm::vec2 GetCursorPosition()
	{
		double cursorX, cursorY;
		glfwGetCursorPos(window, &cursorX, &cursorY);
		return glm::vec2(cursorX,cursorY);
	}

	float lastX;
	float lastY;
	bool firstMouse;
	float xoffset;
	float yoffset;

	float xScroll;
	float yScroll;
private:
	GLFWwindow* window;
	bool focus;

};


#endif // !WINDOW_H