#pragma once
#include "PrimaryIncludes.h"
#include "Core/Application.h"

class GuiInterface : public fsicore::Layer
{
public:
	GuiInterface();
	void OnUpdate() override;
	virtual void OnImGuiRender() override;
	virtual void OnImGuiDrawWidget() override;
	//void OnEvent(fsicore::Event& event) override;
	void set_mesh_load_callback(const std::function<void(const std::string&)>& callback)
	{
		mMeshLoadCallback = callback;
	}
	void load(const std::string& filepath)
	{
		p_ModelsContainer->LoadShapes(filepath);
	}
	// create a file browser instance
private:
	
	fsicore::OcctRenderLayer* occtLayer;
	ImGui::FileBrowser mFileDialog;
	std::function<void(const std::string&)> mMeshLoadCallback;
	std::string mCurrentFile;
	
	std::vector<TopoDS_Shape> m_shapes; //!< Shapes to visualize.
	
	fsicore::SceneContainer* p_ModelsContainer;
	bool m_ViewportFocused = false, m_ViewportHovered = false;
};