/// 
///  MainMenuRenderer.h
///  Seed of Andromeda
///
///  Created by Benjamin Arnold on 1 Nov 2014
///  Copyright 2014 Regrowth Studios
///  All Rights Reserved
///  
///  This file implements the rendering pipeline for the main menu screen.
///

#pragma once

#ifndef MainMenuRenderer_h__
#define MainMenuRenderer_h__

#include <Vorb/graphics/FullQuadVBO.h>
#include <Vorb/graphics/GLRenderTarget.h>
#include <Vorb/graphics/RTSwapChain.hpp>
#include <Vorb/VorbPreDecl.inl>
#include <Vorb/Events.hpp>
#include <Vorb/graphics/GBuffer.h>

#include "LoadContext.h"

/// Forward declarations
class Camera;
class MainMenuScreen;
class MainMenuScriptedUI;
class MainMenuSystemViewer;
class SpaceSystem;
struct CommonState;
struct CommonStateRenderStages;

struct SoaState;
DECL_VUI(struct WindowResizeEvent; class GameWindow);

class MainMenuRenderer {
public:
    /// Initializes the renderer and fills context with anticipated work
    void init(vui::GameWindow* window, StaticLoadContext& context,
              MainMenuScreen* mainMenuScreen, CommonState* commonState);

    void dispose(StaticLoadContext& context);

    /// Hooks render stages in to any dependencies
    void hook();

    /// Asynchronously loads each render stage.
    /// Will not be complete when load() returns.
    /// Check isLoaded() to see when it is finished.
    /// Context should be processed on an OpenGL thread.
    void load(StaticLoadContext& context);

    void render();

    void takeScreenshot() { m_shouldScreenshot = true; }

    void setShowUI(bool showUI) { m_showUI = showUI; }
    void toggleUI() { m_showUI = !m_showUI; }
    void toggleAR() { m_showAR = !m_showAR; }
    void toggleWireframe() { m_wireframe = !m_wireframe; }
    void cycleColorFilter() { m_colorFilter++; if (m_colorFilter > 3) m_colorFilter = 0; }
   
    const volatile bool& isLoaded() const { return m_isLoaded; }

    /************************************************************************/
    /* Events                                                               */
    /************************************************************************/
    void onWindowResize(Sender s, const vui::WindowResizeEvent& e);

private:
    void resize();
    void dumpScreenshot();

    vui::GameWindow* m_window;
    CommonState* m_commonState = nullptr;
    SoaState* m_state = nullptr;
    MainMenuScreen* m_mainMenuScreen = nullptr;
    CommonStateRenderStages* m_stages = nullptr;

    vg::GBuffer m_hdrTarget; ///< Framebuffer needed for the HDR rendering
    vg::RTSwapChain<2> m_swapChain; ///< Swap chain of framebuffers used for post-processing
    MainMenuScriptedUI* m_mainMenuUI; ///< The main menu UI

    std::thread* m_loadThread = nullptr;
    volatile bool m_isLoaded = false;

    ui32v4 m_viewport; ///< Viewport to draw to
    bool m_showUI = true;
    bool m_showAR = true;
    bool m_shouldScreenshot = false;
    bool m_shouldResize = false;
    bool m_wireframe = false;
    ui32v2 m_newDims;
    int m_colorFilter = 0;
};

#endif // MainMenuRenderer_h__
