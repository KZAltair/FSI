#pragma once
#include "PrimaryIncludes.h"
#include "IfcReader/FSIScene.h"
#include "IfcReader/IfcReader.h"
#include "Core/Application.h"

class GuiInterface : public fsicore::Layer
{
public:
	GuiInterface();
	void OnUpdate() override;
	virtual void OnImGuiRender() override;
	virtual void OnImGuiDrawWidget() override;
	virtual void OnEvent(fsicore::Event& e) override;
	bool OnMouseScrolled(fsicore::MouseScrolledEvent& e);
	bool OnMouseMoveEvent(fsicore::MouseMovedEvent& e);

	void AddShape(const TopoDS_Shape& shape);
	void ProcessShape();
	//void OnEvent(fsicore::Event& event) override;
	void set_mesh_load_callback(const std::function<void(const std::string&)>& callback)
	{
		mMeshLoadCallback = callback;
	}
	fsi::FSIScenePtr load(const std::string& filepath)
	{
		return fsi::IfcReader::read(filepath);
	}
	// create a file browser instance
private:
	fsicore::OcctRenderLayer* occtLayer;
	ImGui::FileBrowser mFileDialog;
	
	std::function<void(const std::string&)> mMeshLoadCallback;

	std::string mCurrentFile;
	fsi::FSIScenePtr pScene;
	std::vector<TopoDS_Shape> m_shapes; //!< Shapes to visualize.
	std::vector<fsi::ObjectPtr> m_objects;
};