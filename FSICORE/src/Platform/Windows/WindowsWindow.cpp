#include "fsipch.h"
#include "WindowsWindow.h"

#include "Core/Events/ApplicationEvent.h"
#include "Core/Events/MouseEvent.h"
#include "Core/Events/KeyEvent.h"
#include "Core/Events/OcctEvents.h"
#include <Aspect_VKeyFlags.hxx>

#include <glad/glad.h>

namespace fsicore {

	static bool s_GLFWInitialized = false;
	static void GLFWErrorCallback(int error, const char* description)
	{
		FSI_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}
	namespace
	{
		//! Convert GLFW mouse button into Aspect_VKeyMouse.
		static Aspect_VKeyMouse mouseButtonFromGlfw(int theButton)
		{
			switch (theButton)
			{
			case GLFW_MOUSE_BUTTON_LEFT:   return Aspect_VKeyMouse_LeftButton;
			case GLFW_MOUSE_BUTTON_RIGHT:  return Aspect_VKeyMouse_RightButton;
			case GLFW_MOUSE_BUTTON_MIDDLE: return Aspect_VKeyMouse_MiddleButton;
			}
			return Aspect_VKeyMouse_NONE;
		}

		//! Convert GLFW key modifiers into Aspect_VKeyFlags.
		static Aspect_VKeyFlags keyFlagsFromGlfw(int theFlags)
		{
			Aspect_VKeyFlags aFlags = Aspect_VKeyFlags_NONE;
			if ((theFlags & GLFW_MOD_SHIFT) != 0)
			{
				aFlags |= Aspect_VKeyFlags_SHIFT;
			}
			if ((theFlags & GLFW_MOD_CONTROL) != 0)
			{
				aFlags |= Aspect_VKeyFlags_CTRL;
			}
			if ((theFlags & GLFW_MOD_ALT) != 0)
			{
				aFlags |= Aspect_VKeyFlags_ALT;
			}
			if ((theFlags & GLFW_MOD_SUPER) != 0)
			{
				aFlags |= Aspect_VKeyFlags_META;
			}
			return aFlags;
		}
	}
	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}



	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		FSI_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

		if (!s_GLFWInitialized)
		{
			// TODO: glfwTerminate on system shutdown
			int success = glfwInit();
			FSI_CORE_ASSERT(success, "Could not intialize GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(m_Window);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		FSI_CORE_ASSERT(status, "Failed to initialize GLAD!");
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		// Set GLFW callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				data.Width = width;
				data.Height = height;

				WindowResizeEvent event(width, height);
				data.EventCallback(event);
			});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent event;
				data.EventCallback(event);
			});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					data.EventCallback(event);
					break;
				}
				}
			});
		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				KeyTypedEvent event(keycode);
				data.EventCallback(event);
			});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button, mods);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button, mods);
					data.EventCallback(event);
					break;
				}
				}
			});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseScrolledEvent event((float)xOffset, (float)yOffset);
				data.EventCallback(event);
			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseMovedEvent event((float)xPos, (float)yPos);
				data.EventCallback(event);
			});
	}

	void WindowsWindow::Shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void WindowsWindow::SetVSync(bool enabled)
	{
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}
	void WindowsWindow::SetOcctShowHideEvent(bool flag)
	{
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(m_Window);

		OcctShowHideEvent event(flag);
		data.EventCallback(event);
	}
	void WindowsWindow::SetOcctShowHideSingleObjectEvent(bool flag)
	{
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(m_Window);

		OcctShowHideSignleObjectEvent event(flag);
		data.EventCallback(event);
	}
	void WindowsWindow::SetOcctEmptyScene(bool flag)
	{
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(m_Window);

		OcctEmptySceneEvent event(flag);
		data.EventCallback(event);
	}
	void WindowsWindow::SetOcctLoadScene(bool flag)
	{
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(m_Window);

		OcctLoadSceneEvent event(flag);
		data.EventCallback(event);
	}
	void WindowsWindow::SetWindowClosed()
	{
		WindowData& data = *(WindowData*)glfwGetWindowUserPointer(m_Window);
		WindowCloseEvent event;
		data.EventCallback(event);
	}
}
