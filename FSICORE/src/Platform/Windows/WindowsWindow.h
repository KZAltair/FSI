#pragma once

#include "Core/Window.h"
#include <GLFW/glfw3.h>

namespace fsicore 
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		// Window attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVSync() const override;
		inline virtual void* GetNativeWindow() const override { return m_Window; }
		virtual void SetOcctShowHideEvent(bool flag) override;
		virtual void SetOcctShowHideSingleObjectEvent(bool flag) override;
		virtual void SetOcctEmptyScene(bool flag) override;
		virtual void SetOcctLoadScene(bool flag) override;
		virtual void SetWindowClosed() override;
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;

		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};

		WindowData m_Data;
	};

}