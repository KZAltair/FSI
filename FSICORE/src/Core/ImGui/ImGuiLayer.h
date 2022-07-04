#pragma once

#include "Core/Layer.h"

#include "Core/Events/ApplicationEvent.h"
#include "Core/Events/KeyEvent.h"
#include "Core/Events/MouseEvent.h"

namespace fsicore {

	class FSI_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;
		virtual void OnEvent(Event& e) override;
		void BlockEvents(bool block) { m_BlockEvents = block; }

		void Begin();
		void End();

	private:
		float m_Time = 0.0f;

		bool m_BlockEvents = true;
	};

}