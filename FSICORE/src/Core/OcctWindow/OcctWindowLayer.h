#pragma once
#include "Core/Core.h"
#include "Core/Layer.h"
#include "Core/Events/MouseEvent.h"
#include "Core/Events/ApplicationEvent.h"

//Opencascade includes
#include <Aspect_DisplayConnection.hxx>
#include <Aspect_RenderingContext.hxx>
#include <Aspect_Window.hxx>
#include <TCollection_AsciiString.hxx>
#include <V3d_View.hxx>
#include <AIS_ViewController.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <Image_PixMap.hxx>
#include <OpenGl_Texture.hxx>
#include <Graphic3d_CView.hxx>

namespace fsicore
{
    //! GLFWwindow wrapper implementing Aspect_Window interface.
    class FSI_API OcctWindow : public Aspect_Window
    {
        DEFINE_STANDARD_RTTI_INLINE(OcctWindow, Aspect_Window)
    public:
        //! Main constructor.
        OcctWindow(void* win, int theWidth, int theHeight, const TCollection_AsciiString& theTitle);

        //! Close the window.
        virtual ~OcctWindow() {};

        //! Return X Display connection.
        const Handle(Aspect_DisplayConnection)& GetDisplay() const { return myDisplay; }

        //! Return GLFW window.
        void* getGlfwWindow() { return glWindow; }

        //! Return native OpenGL context.
        Aspect_RenderingContext NativeGlContext() const;

        //! Return cursor position.
        Graphic3d_Vec2i CursorPosition() const;

    public:

        //! Returns native Window handle
        virtual Aspect_Drawable NativeHandle() const Standard_OVERRIDE;

        //! Returns parent of native Window handle.
        virtual Aspect_Drawable NativeParentHandle() const Standard_OVERRIDE { return 0; }

        //! Applies the resizing to the window <me>
        virtual Aspect_TypeOfResize DoResize() Standard_OVERRIDE;

        //! Returns True if the window <me> is opened and False if the window is closed.
        virtual Standard_Boolean IsMapped() const Standard_OVERRIDE;

        //! Apply the mapping change to the window <me> and returns TRUE if the window is mapped at screen.
        virtual Standard_Boolean DoMapping() const Standard_OVERRIDE { return Standard_True; }

        //! Opens the window <me>.
        virtual void Map() const Standard_OVERRIDE;

        //! Closes the window <me>.
        virtual void Unmap() const Standard_OVERRIDE;

        virtual void Position(Standard_Integer& theX1, Standard_Integer& theY1,
            Standard_Integer& theX2, Standard_Integer& theY2) const Standard_OVERRIDE
        {
            theX1 = myXLeft;
            theX2 = myXRight;
            theY1 = myYTop;
            theY2 = myYBottom;
        }

        //! Returns The Window RATIO equal to the physical WIDTH/HEIGHT dimensions.
        virtual Standard_Real Ratio() const Standard_OVERRIDE
        {
            return Standard_Real(myXRight - myXLeft) / Standard_Real(myYBottom - myYTop);
        }

        //! Return window size.
        virtual void Size(Standard_Integer& theWidth, Standard_Integer& theHeight) const Standard_OVERRIDE
        {
            theWidth = myXRight - myXLeft;
            theHeight = myYBottom - myYTop;
        }

        virtual Aspect_FBConfig NativeFBConfig() const Standard_OVERRIDE { return NULL; }

    protected:
        Handle(Aspect_DisplayConnection) myDisplay;
        void* glWindow;
        Standard_Integer myXLeft;
        Standard_Integer myYTop;
        Standard_Integer myXRight;
        Standard_Integer myYBottom;
    };

    class FSI_API OcctRenderLayer : virtual public Layer, public AIS_ViewController
    {
    public:
        OcctRenderLayer();
        virtual ~OcctRenderLayer();
        //-----------------Layer overrides --------------------------//
        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnOcctWindowRender() override;
        virtual void OnEvent(Event& e) override;
        bool OnChangeSizeOcct(WindowResizeEvent& e);

        Handle(V3d_View) GetView() const { return mainView;  }
        Handle(V3d_Viewer) GetViewer() const { return aViewer; }
        Handle(OcctWindow) GetOcctWindow() const { return h_occtWindow; }
        Handle_OpenGl_Context GetOcctGLContext() const { return h_occtGLcontext; }
    protected:
        
    private:
        //Occt vars
        Handle(V3d_Viewer) aViewer;
        Handle(V3d_View) mainView;
        Handle(OcctWindow) h_occtWindow;
        Handle(Graphic3d_CView) viewport;
        Handle_OpenGl_Context h_occtGLcontext;
        void* rawGlContext;
        int occtWinWidth = 0;
        int occtWinHeight = 0;

        float m_ZoomLevel = 1.0f;
        float mouseX = 0.0f;
        float mouseY = 0.0f;

        
    };
}