// 
//  TestInventoryScreen.h
//  Seed Of Andromeda
//
//  Created by Matthew Marshall on 09 Nov 2015
//  Copyright 2014 Regrowth Studios
//  All Rights Reserved
//  
//  This file provides a testing screen for 
//  the inventory system.
//

#pragma once

#ifndef TESTINVENTORYSCREEN_H_
#define TESTINVENTORYSCREEN_H_

#include <Vorb/ui/IGameScreen.h>
#include <Vorb/graphics/SpriteFont.h>

#include "CommonState.h"
#include "InventoryComponentRenderer.h"
#include "SoaController.h"

class TestInventoryScreen : public vui::IAppScreen<App> {
public:
    TestInventoryScreen(const App* app, CommonState* state);
    /************************************************************************/
    /* IGameScreen functionality                                            */
    /************************************************************************/
    virtual i32 getNextScreen() const override;
    virtual i32 getPreviousScreen() const override;
    virtual void build() override;
    virtual void destroy(const vui::GameTime& gameTime) override;
    virtual void onEntry(const vui::GameTime& gameTime) override;
    virtual void onExit(const vui::GameTime& gameTime) override;
    virtual void update(const vui::GameTime& gameTime) override;
    virtual void draw(const vui::GameTime& gameTime) override;

    void initUI();
    void reloadUI();

private:
    InputMapper* m_inputMapper = nullptr;

    InventoryComponentRenderer m_renderer; ///< This handles all rendering for the inventory
    InventoryScriptedUI* m_ui; ///< The UI form
    vg::SpriteFont m_formFont; ///< The UI font

    CommonState* m_commonState;
    SoaState* m_soaState;

    SoaController controller;

    bool m_shouldReloadUI = false;
};

#endif // TESTINVENTORYSCREEN_H_
