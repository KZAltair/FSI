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
		virtual void ClearAllObjects() { 
			pScene.reset();
			m_shapes.clear();
			m_shapes.resize(0);
			m_objects.clear();
		}
		virtual void SetAisShapeID(int id) { aisShapeId = id; }
		virtual int GetAisShapeID() const { return aisShapeId; }
	private:
		std::vector<TopoDS_Shape> m_shapes;
		fsi::FSIScenePtr pScene;
		std::vector<fsi::ObjectPtr> m_objects;
		int aisShapeId = 0;
	};
}
