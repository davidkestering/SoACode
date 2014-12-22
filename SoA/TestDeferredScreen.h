///
/// TestDeferredScreen.h
/// Seed of Andromeda
///
/// Created by Cristian Zaloj on 16 Dec 2014
/// Copyright 2014 Regrowth Studios
/// All Rights Reserved
///
/// Summary:
/// Development test for deferred rendering
///

#pragma once

#ifndef TestDeferredScreen_h__
#define TestDeferredScreen_h__

#include <DeferredShaders.h>
#include <Events.hpp>
#include <FullQuadVBO.h>
#include <GBuffer.h>
#include <GLProgram.h>
#include <gtypes.h>
#include <IGameScreen.h>
#include <SpriteBatch.h>

class TestDeferredScreen : public IGameScreen {
public:
    /// 
    TestDeferredScreen();

    /************************************************************************/
    /* IGameScreen functionality                                            */
    /************************************************************************/
    virtual i32 getNextScreen() const override;
    virtual i32 getPreviousScreen() const override;
    virtual void build() override;
    virtual void destroy(const GameTime& gameTime) override;
    virtual void onEntry(const GameTime& gameTime) override;
    virtual void onExit(const GameTime& gameTime) override;
    virtual void onEvent(const SDL_Event& e) override;
    virtual void update(const GameTime& gameTime) override;
    virtual void draw(const GameTime& gameTime) override;
private:
    void buildGeometry();
    void buildLightMaps();

    VGVertexBuffer m_verts; ///< Sphere's vertex buffer (of positions)
    VGIndexBuffer m_inds; ///< Sphere's index buffer
    ui32 m_indexCount; ///< Number of indices for sphere
    vg::DeferredShaders m_deferredPrograms; ///< Basic rendering programs
    vg::FullQuadVBO m_quad; ///< Used for GBuffer clearing operations
    vg::GBuffer m_gbuffer; ///< Geometry buffer of deferred rendering
    SpriteBatch m_sb; ///< Debug SpriteBatch
    AutoDelegatePool m_hooks; ///< Input hooks reservoir
    VGTexture m_envMap; ///< Environment map
    f32 m_roughness, m_reflectance, m_metalness; ///< Temp test values
};

#endif // TestDeferredScreen_h__