#pragma once
#include "PrimaryIncludes.h"
#include "Core/Application.h"
#include "Core/Events/OcctEvents.h"

class GuiInterface : public fsicore::Layer
{
public:
	GuiInterface();
	virtual ~GuiInterface();
	void OnUpdate() override;
	virtual void OnImGuiRender() override;
	virtual void OnImGuiDrawWidget() override;
	void OnEvent(fsicore::Event& event) override;
	void set_mesh_load_callback(const std::function<void(const std::string&)>& callback)
	{
		mMeshLoadCallback = callback;
	}
	void load(const std::string& filepath)
	{
		p_ModelsContainer->LoadShapes(filepath);
		app->GetWindow().SetOcctLoadScene(true);
	}

	void allobjects_showhide_callback(const std::function<void(bool)>& callback)
	{
		mObjectShowHideCallback = callback;
	}

	void showHideAllObjects(bool flag)
	{
		app->GetWindow().SetOcctShowHideSingleObjectEvent(flag);
	}
	void single_object_showhide_callback(const std::function<void(int, bool)>& callback)
	{
		mSingleObjectShowHideCallback = callback;
	}

	void showHideObject(int id, bool flag)
	{
		app->GetWindow().SetOcctShowHideEvent(flag);
		p_ModelsContainer->SetAisShapeID(id);
	}

	//Empty scene callback
	void occt_empty_scene_callback(const std::function<void(bool)>& callback)
	{
		fEmptySceneCallback = callback;
	}

	void occtEmptyScene(bool flag)
	{
		app->GetWindow().SetOcctEmptyScene(flag);
	}

	//This layer GUI functions
	void CreateNode(const char* prefix, int uid);
	void DrawPropertyPannel();
	// create a file browser instance
private:
	fsicore::Application* app;
	fsicore::OcctRenderLayer* occtLayer;
	ImGui::FileBrowser mFileDialog;
	std::function<void(const std::string&)> mMeshLoadCallback;
	std::function<void(bool)> mObjectShowHideCallback;
	std::function<void(int, bool)> mSingleObjectShowHideCallback;
	std::function<void(bool)> fEmptySceneCallback;
	std::string mCurrentFile;
	
	std::vector<TopoDS_Shape> m_shapes; //!< Shapes to visualize.
	
	fsicore::SceneContainer* p_ModelsContainer;
	bool m_ViewportFocused = false, m_ViewportHovered = false;
	std::vector<bool> checkBoxes;
	bool flag = false;
	bool showHideAllobjects = false;
	int mSelectionContext = -1;
	int mDefaultFloodId = 0;
	std::string label;
	std::vector<std::string> lables;
};