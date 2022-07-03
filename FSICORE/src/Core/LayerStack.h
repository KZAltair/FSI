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
		std::vector<Layer*>::reverse_iterator rbegin() { return m_Layers.rbegin(); }
		std::vector<Layer*>::reverse_iterator rend() { return m_Layers.rend(); }

		std::vector<Layer*>::const_iterator begin() const { return m_Layers.begin(); }
		std::vector<Layer*>::const_iterator end()	const { return m_Layers.end(); }
		std::vector<Layer*>::const_reverse_iterator rbegin() const { return m_Layers.rbegin(); }
		std::vector<Layer*>::const_reverse_iterator rend() const { return m_Layers.rend(); }
	private:
		std::vector<Layer*> m_Layers;

		unsigned int m_LayerInsertIndex = 0;
	};

}