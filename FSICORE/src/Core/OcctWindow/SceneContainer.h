#pragma once
#include "Core/Layer.h"
#include "IfcReader/FSIScene.h"
#include "IfcReader/IfcReader.h"

namespace fsicore
{

	class SceneContainer : virtual public Layer
	{
	public:
		SceneContainer();
		virtual fsi::FSIScenePtr GetScenePtr() { return pScene; }

		virtual std::vector<fsi::ObjectPtr> LoadShapes(const std::string& filepath);
		virtual bool IsLoading() const { return load; }
		virtual bool IsRemoved() const { return removed; };
		virtual void SetRemovedStatus(bool flag) { removed = flag; }
		virtual void SetLoadingStatus(bool flag) { load = flag; }
		virtual void ClearAllObjects() { 
			m_shapes.clear();
			m_shapes.resize(0);
			m_objects.clear(); 
			m_objects.resize(0);
			removed = true; }
	private:
		std::vector<TopoDS_Shape> m_shapes;
		fsi::FSIScenePtr pScene;
		std::vector<fsi::ObjectPtr> m_objects;
		bool load = false;
		bool removed = false;
	};
}
