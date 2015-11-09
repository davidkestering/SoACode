#include "stdafx.h"
#include "TestInventoryScreen.h"

#include "Inputs.h"

i32 TestInventoryScreen::getNextScreen() const {
    return SCREEN_INDEX_NO_SCREEN;
}

i32 TestInventoryScreen::getPreviousScreen() const {
    return SCREEN_INDEX_NO_SCREEN;
}

void TestInventoryScreen::build() {
    // Emptry
}

void TestInventoryScreen::destroy(const vui::GameTime& gameTime) {
    // Empty
}

void TestInventoryScreen::onEntry(const vui::GameTime& gameTime) {
    m_inputMapper = new InputMapper();
    initInputs(m_inputMapper);
    m_inputMapper->get(INPUT_RELOAD_UI).downEvent.addFunctor([&](Sender s, ui32 i) { m_shouldReloadUI = true; });
    m_inputMapper->startInput();

    m_formFont.init("Fonts/orbitron_bold-webfont.ttf", 32);

    initUI();

    m_renderer.init(m_ui);
}

void TestInventoryScreen::onExit(const vui::GameTime& gameTime) {
    m_formFont.dispose();
    m_ui->dispose();

    delete m_inputMapper;
}

void TestInventoryScreen::update(const vui::GameTime& gameTime) {
    if (m_shouldReloadUI) {
        reloadUI();
    }

    m_ui->update();
}

void TestInventoryScreen::draw(const vui::GameTime& gameTime) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_renderer.render();
}

void TestInventoryScreen::initUI() {
    m_ui = new InventoryScriptedUI();
    const ui32v2& vdims = m_game->getWindow().getViewportDims();
    m_ui->init("Data/UI/Forms/inventory.form.lua", this, &m_game->getWindow(), f32v4(0.0f, 0.0f, (f32)vdims.x, (f32)vdims.y), &m_formFont);
    m_ui->setInputMapper(m_inputMapper);
}

void TestInventoryScreen::reloadUI() {
    m_ui->dispose();
    initUI();

    m_shouldReloadUI = false;
    printf("UI was reloaded.\n");
}