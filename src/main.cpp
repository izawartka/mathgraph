#include "rzuf3/rzuf.h"
#include "mathgraph/mathgraph.h"

RZUF3_Game* game = nullptr;

#ifndef _DEBUG
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif
int main(int argc, char* argv[]) {
    spdlog::set_level(spdlog::level::debug);

    game = new RZUF3_Game();
    game->addConfigFile(g_defaultConfigDef);
    game->setLanguageFromConfigFile("assets/", "lang");
    game->initWindow(WINDOW_MIN_WIDTH, WINDOW_MIN_HEIGHT, false);
    game->setWindowTitle("window_title", true);
    game->setWindowIcon("assets/icon.png");
    game->setWindowMinimumSize(WINDOW_MIN_WIDTH, WINDOW_MIN_HEIGHT);
    game->setWindowResizable(true);

    RZUF3_SceneDefinition* sceneDef = MainScene::getSceneDef();

    game->setScene(sceneDef);
    game->startGameLoop();

    delete game;
    MainScene::destroy();
    SettingsScene::destroy();

    return 0;
}
