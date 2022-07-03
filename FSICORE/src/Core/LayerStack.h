#pragma once

#include "Core/Core.h"
#include "Layer.h"

#include <vector>

namespace fsicore {

	class FSI_API LayerStack
	{
	public:
		LayerStack();
		~LayerStack();

		//Regular layer that is pushed first
		void PushLayer(Layer* layer);
		
		//Overlay is always on top of stack and cover up what's beneath
		//For example gui is an overlay as it appears last in rendering cover up
		//Entire screen
		void PushOverlay(Layer* overlay);
		void PopLayer(Layer* layer);
		void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_Layers.end(); }
	private:
		std::vector<Layer*> m_Layers;
		std::vector<Layer*>::iterator m_LayerInsert;
	};

}