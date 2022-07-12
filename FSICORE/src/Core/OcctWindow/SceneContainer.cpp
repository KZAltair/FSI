#include "fsipch.h"
#include "SceneContainer.h"

namespace fsicore
{
	SceneContainer::SceneContainer()
		:Layer("SceneModelsHolder")
	{

	}
	std::vector<fsi::ObjectPtr> SceneContainer::LoadShapes(const std::string& filepath)
	{
		if (!filepath.empty())
		{
			pScene = fsi::IfcReader::read(filepath);
		}
		if (pScene)
		{
			return pScene->getObjects();
		}
		return std::vector<fsi::ObjectPtr>();
	}
}