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
    RZUF3_ObjectDefinition objBackButton;
    objBackButton.name = "back_btn";
    objBackButton.parentName = "ui_root";
    objBackButton.scripts = {
        new RZUF3_TextButton(MAIN_FONT, "back_btn"),
        new SceneSwitchButton(MainScene::getSceneDef()),
        new RZUF3_WindowAnchor({0, 0, 16, 16})
    };
    m_sceneDef->objects.push_back(objBackButton);

    // settings title //
    RZUF3_TextRendererOptions titleOptions;
    titleOptions.style.fontFilepath = MAIN_FONT;
    titleOptions.style.size = 32;
    titleOptions.text = "settings_title";

    RZUF3_ObjectDefinition objTitle;
    objTitle.name = "settings_title";
    objTitle.parentName = "ui_root";
    objTitle.pos = { 32, 64 };
    objTitle.scripts = {
		new RZUF3_TextRenderer(titleOptions)
	};
    m_sceneDef->objects.push_back(objTitle);

    // labels //
    RZUF3_TextRendererOptions labelOptions;
    labelOptions.style.fontFilepath = MAIN_FONT;
    labelOptions.alignment = RZUF3_Align_TopRight;

    // dropdowns //
    RZUF3_DropDownOptions dropDownOptions;
    dropDownOptions.style.setFontFilepath(MAIN_FONT);
    dropDownOptions.style.defaultColors();
    dropDownOptions.sendValueAsString = true;

    // checkboxes //
    RZUF3_CheckboxOptions checkboxOptions;
    checkboxOptions.style.textStyle.fontFilepath = MAIN_FONT;
    checkboxOptions.style.defaultColors();

    // language label //
    labelOptions.text = "lang_label";

    RZUF3_ObjectDefinition objLanguageLabel;
    objLanguageLabel.name = "lang_label";
    objLanguageLabel.parentName = "ui_root";
    objLanguageLabel.pos = { SETTINGS_LABELS_X, SETTINGS_ENTRY_Y(0) + 4 };
    objLanguageLabel.scripts = {
        new RZUF3_TextRenderer(labelOptions)
    };
    m_sceneDef->objects.push_back(objLanguageLabel);

    // language //
    dropDownOptions.items = {
		{"lang_en.txt", "lang_opt_en"},
		{"lang_pl.txt", "lang_opt_pl"}
	};

    RZUF3_ObjectDefinition objLanguage;
    objLanguage.name = "language";
    objLanguage.parentName = "ui_root";
    objLanguage.pos = { SETTINGS_COMPONENTS_X, SETTINGS_ENTRY_Y(0) };
    objLanguage.scripts = {
        new RZUF3_DropDown(dropDownOptions),
        new RZUF3_UIConfigSync(MAIN_CONFIG, "lang")
    };
    m_sceneDef->objects.push_back(objLanguage);

    // x scale label //
    labelOptions.text = "x_scale_label";

    RZUF3_ObjectDefinition objXScaleLabel;
    objXScaleLabel.name = "x_scale_label";
    objXScaleLabel.parentName = "ui_root";
    objXScaleLabel.pos = { SETTINGS_LABELS_X, SETTINGS_ENTRY_Y(1) + 4 };
    objXScaleLabel.scripts = {
		new RZUF3_TextRenderer(labelOptions)
	};
    m_sceneDef->objects.push_back(objXScaleLabel);

    // x scale //
    dropDownOptions.items = {
        {"decimal", "scale_opt_decimal"},
        {"log", "scale_opt_log"},
        {"pi_based", "scale_opt_pi_based"}
    };

    RZUF3_ObjectDefinition objXScale;
    objXScale.name = "x_scale";
    objXScale.parentName = "ui_root";
    objXScale.pos = { SETTINGS_COMPONENTS_X, SETTINGS_ENTRY_Y(1) };
    objXScale.scripts = {
		new RZUF3_DropDown(dropDownOptions),
		new RZUF3_UIConfigSync(MAIN_CONFIG, "x_scale")
	};
    m_sceneDef->objects.push_back(objXScale);

    // y scale label //
    labelOptions.text = "y_scale_label";

    RZUF3_ObjectDefinition objYScaleLabel;
    objYScaleLabel.name = "y_scale_label";
    objYScaleLabel.parentName = "ui_root";
    objYScaleLabel.pos = { SETTINGS_LABELS_X, SETTINGS_ENTRY_Y(2) + 4 };
    objYScaleLabel.scripts = {
        new RZUF3_TextRenderer(labelOptions)
    };
    m_sceneDef->objects.push_back(objYScaleLabel);

    // y scale //
    RZUF3_ObjectDefinition objYScale;
    objYScale.name = "y_scale";
    objYScale.parentName = "ui_root";
    objYScale.pos = { SETTINGS_COMPONENTS_X, SETTINGS_ENTRY_Y(2) };
    objYScale.scripts = {
		new RZUF3_DropDown(dropDownOptions),
		new RZUF3_UIConfigSync(MAIN_CONFIG, "y_scale")
	};
    m_sceneDef->objects.push_back(objYScale);

    // show grid //
    checkboxOptions.labelText = "show_grid_label";
    
    RZUF3_ObjectDefinition objShowGrid;
    objShowGrid.name = "show_grid";
    objShowGrid.parentName = "ui_root";
    objShowGrid.pos = { SETTINGS_COMPONENTS_X, SETTINGS_ENTRY_YS(3, 1) };
    objShowGrid.scripts = {
		new RZUF3_Checkbox(checkboxOptions),
		new RZUF3_UIConfigSync(MAIN_CONFIG, "show_grid")
	};
    m_sceneDef->objects.push_back(objShowGrid);

    // show point //
    checkboxOptions.labelText = "show_point_label";

    RZUF3_ObjectDefinition objShowPoint;
    objShowPoint.name = "show_point";
    objShowPoint.parentName = "ui_root";
    objShowPoint.pos = { SETTINGS_COMPONENTS_X, SETTINGS_ENTRY_YS(4, 1) };
    objShowPoint.scripts = {
        new RZUF3_Checkbox(checkboxOptions),
        new RZUF3_UIConfigSync(MAIN_CONFIG, "show_point")
    };
    m_sceneDef->objects.push_back(objShowPoint);

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
