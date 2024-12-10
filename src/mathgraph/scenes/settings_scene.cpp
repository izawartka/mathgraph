#include "settings_scene.h"
#include "../mathgraph.h"

RZUF3_SceneDefinition* SettingsScene::m_sceneDef = nullptr;

RZUF3_SceneDefinition* SettingsScene::getSceneDef()
{
    if (m_sceneDef != nullptr) return m_sceneDef;

    m_sceneDef = new RZUF3_SceneDefinition();
    m_sceneDef->name = "settings";

    // UI root //
    RZUF3_ObjectDefinition objUIRoot;
    objUIRoot.name = "ui_root";
    m_sceneDef->objects.push_back(objUIRoot);

    // back button //
    RZUF3_TextButtonOptions backButtonOptions;
    backButtonOptions.text = "settings_back";
    backButtonOptions.useLangFile = true;
    backButtonOptions.alignment = RZUF3_Align_TopLeft;
    backButtonOptions.styles[0].horizontalPadding = 16;
    backButtonOptions.styles[0].textStyle.fontFilepath = "assets/fonts/roboto-regular.ttf";

    RZUF3_ObjectDefinition objBackButton;
    objBackButton.name = "back";
    objBackButton.parentName = "ui_root";
    objBackButton.scripts = {
        new RZUF3_TextButton(backButtonOptions),
        new SceneSwitchButton(MainScene::getSceneDef()),
        new RZUF3_WindowAnchor({0, 0, 16, 16})
    };
    m_sceneDef->objects.push_back(objBackButton);

    return m_sceneDef;
}

void SettingsScene::destroy()
{
    if (m_sceneDef == nullptr) return;

    for (auto& obj : m_sceneDef->objects)
    {
        for (auto& script : obj.scripts)
        {
			delete script;
		}
	}

	delete m_sceneDef;
	m_sceneDef = nullptr;
}
