#pragma once

//Platform independent window wrapper that should be defined per each OS

#include "fsipch.h"

#include "Core/Core.h"
#include "Core/Events/Event.h"

namespace fsicore {

	struct WindowProps
	{
		std::string Title;
		unsigned int Width;
		unsigned int Height;

		WindowProps(const std::string& title = "FSI Engine",
			unsigned int width = 1280,
			unsigned int height = 720)
			: Title(title), Width(width), Height(height)
		{
		}
	};

	// Interface representing a desktop system based Window
	class FSI_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		//Controll events
		virtual void SetOcctShowHideEvent(bool flag) = 0;
		virtual void SetOcctShowHideSingleObjectEvent(bool flag) = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};

}