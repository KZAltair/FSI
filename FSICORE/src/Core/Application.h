#pragma once

#include "Core.h"

#include "Window.h"
#include "Core/LayerStack.h"
#include "Core/Events/Event.h"
#include "Core/Events/ApplicationEvent.h"

#include "Core/ImGui/ImGuiLayer.h"
#include "Core/OcctWindow/OcctWindowLayer.h"
#include "Core/OcctWindow/SceneContainer.h"

namespace fsicore
{
	class FSI_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Run();
		void OnEvent(Event& e);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);
		inline Window& GetWindow() { return *m_Window; }
		inline static Application& Get() { return *s_Instance; }
		OcctRenderLayer* GetOcctLayer() { return m_occtRenderLayer; }
		ImGuiLayer* GetImGuiLayer() const { return m_ImGuiLayer; }
		SceneContainer* GetSceneContainer() const { return m_SceneContainer; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
	private:
		std::unique_ptr<Window> m_Window;
		
		
		bool m_Running = true;
		LayerStack m_LayerStack;
	private:
		static Application* s_Instance;

	protected:
		OcctRenderLayer* m_occtRenderLayer;
		ImGuiLayer* m_ImGuiLayer;
		SceneContainer* m_SceneContainer;
	};

	//To be defined at client side
	Application* CreateApplication();
}

