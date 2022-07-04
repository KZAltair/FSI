#include "fsipch.h"
#include "OcctWindowLayer.h"
#include "Core/Application.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>
#include "glad/glad.h"


namespace fsicore
{
    OcctWindow::OcctWindow(void* win, int theWidth, int theHeight, const TCollection_AsciiString& theTitle)
        :
        glWindow(win),
        myXLeft(0),
        myYTop(0),
        myXRight(0),
        myYBottom(0)
    {

        if (glWindow != nullptr)
        {
            int aWidth = 0, aHeight = 0;
            glfwGetWindowPos((GLFWwindow*)glWindow, &myXLeft, &myYTop);
            glfwGetWindowSize((GLFWwindow*)glWindow, &aWidth, &aHeight);
            myXRight = myXLeft + aWidth;
            myYBottom = myYTop + aHeight;
        }
    }

    Aspect_Drawable OcctWindow::NativeHandle() const
    {
        return (Aspect_Drawable)glfwGetWin32Window((GLFWwindow*)glWindow);
    }
    Aspect_RenderingContext OcctWindow::NativeGlContext() const
    {
        return glfwGetWGLContext((GLFWwindow*)glWindow);
    }

    Standard_Boolean OcctWindow::IsMapped() const
    {
        return glfwGetWindowAttrib((GLFWwindow*)glWindow, GLFW_VISIBLE) != 0;
    }

    void OcctWindow::Map() const
    {
        glfwShowWindow((GLFWwindow*)glWindow);
    }

    void OcctWindow::Unmap() const
    {
        glfwHideWindow((GLFWwindow*)glWindow);
    }

    Aspect_TypeOfResize OcctWindow::DoResize()
    {
        if (glfwGetWindowAttrib((GLFWwindow*)glWindow, GLFW_VISIBLE) == 1)
        {
            int anXPos = 0, anYPos = 0, aWidth = 0, aHeight = 0;
            glfwGetWindowPos((GLFWwindow*)glWindow, &anXPos, &anYPos);
            glfwGetWindowSize((GLFWwindow*)glWindow, &aWidth, &aHeight);
            myXLeft = anXPos;
            myXRight = anXPos + aWidth;
            myYTop = anYPos;
            myYBottom = anYPos + aHeight;
        }
        return Aspect_TOR_UNKNOWN;
    }

    Graphic3d_Vec2i OcctWindow::CursorPosition() const
    {
        Graphic3d_Vec2d aPos;
        glfwGetCursorPos((GLFWwindow*)glWindow, &aPos.x(), &aPos.y());
        return Graphic3d_Vec2i((int)aPos.x(), (int)aPos.y());
    }

    //--Occt window layer setup
    OcctRenderLayer::OcctRenderLayer()
        :Layer("OcctWindowLayer")
    {
    }

    OcctRenderLayer::~OcctRenderLayer()
    {
    }

    void OcctRenderLayer::OnAttach()
    {
        Application& app = Application::Get();
        void* window = app.GetWindow().GetNativeWindow();

        //Critial to init occt window before all callbacks
        h_occtWindow = new OcctWindow(window, app.GetWindow().GetWidth(), app.GetWindow().GetHeight(), "FSI Viewer");
        rawGlContext = ::wglGetCurrentContext();

        occtWinWidth = app.GetWindow().GetWidth();
        occtWinHeight = app.GetWindow().GetHeight();

        Handle(Aspect_DisplayConnection) h_Display;
        Handle(OpenGl_GraphicDriver) aGraphicDriver = new OpenGl_GraphicDriver(h_Display, false);

        //Important to not flush 3D buffer after rendering to render gui on top and then flush with OpenGL swap buffers
        aGraphicDriver->SetBuffersNoSwap(true);
        Handle(V3d_Viewer) aViewer = new V3d_Viewer(aGraphicDriver);

        aViewer->SetDefaultLights();
        aViewer->SetLightOn();
        aViewer->SetDefaultTypeOfView(V3d_PERSPECTIVE);
        aViewer->ActivateGrid(Aspect_GT_Rectangular, Aspect_GDM_Lines);
        mainView = aViewer->CreateView();
        mainView->SetImmediateUpdate(false);
        mainView->SetWindow(h_occtWindow, h_occtWindow->NativeGlContext());
        mainView->ChangeRenderingParams().ToShowStats = true;
        h_aisInteractor = new AIS_InteractiveContext(aViewer);
        h_occtGLcontext = new OpenGl_Context();
        h_occtGLcontext->Init((HWND)(mainView->Window()->NativeHandle()), ::GetDC((HWND)(mainView->Window()->NativeHandle())), rawGlContext);
        //h_occtGLcontext->ResizeViewport(size);

    }

    void OcctRenderLayer::OnDetach()
    {
        if (!t.IsNull())
        {
            t->Release(h_occtGLcontext.get());
        }
        if (!mainView.IsNull())
        {
            mainView->Remove();
        }

        //TODO check window destruction
    }

    void OcctRenderLayer::OnOcctWindowRender()
    {
        if (!t.IsNull())
        {
            t->Release(h_occtGLcontext.get());
        }
        t = new OpenGl_Texture();
        OpenGl_TextureFormat p = OpenGl_TextureFormat::FindSizedFormat(h_occtGLcontext, GL_RGBA8);
        if (!mainView->ToPixMap(anImage, occtWinWidth, occtWinHeight))
        {
            FSI_CORE_ERROR("View dump failed");
        }
        //t->Init(m_GLcontext, p, Graphic3d_Vec2i(w, h), Graphic3d_TypeOfTexture::Graphic3d_TypeOfTexture_2D, &anImage);
        t->Init(h_occtGLcontext, anImage, Graphic3d_TypeOfTexture_2D, true);
        mainView->Invalidate();
        FlushViewEvents(h_aisInteractor, mainView, true);
    }
    unsigned int OcctRenderLayer::GetTexID() const
    {
        return t->TextureId();
    }
}


