#include "main_scene.h"
#include "mathgraph.h"

RZUF3_SceneDefinition* MainScene::m_sceneDef = nullptr;

RZUF3_SceneDefinition* MainScene::getSceneDef()
{
    if (m_sceneDef != nullptr) return m_sceneDef;

    m_sceneDef = new RZUF3_SceneDefinition();
    m_sceneDef->name = "mathgraph";

    // UI root //
    RZUF3_ObjectDefinition objUIRoot;
    objUIRoot.name = "ui_root";
    m_sceneDef->objects.push_back(objUIRoot);

    // equation prompt //
    RZUF3_TextInputOptions equationPromptOptions;
    equationPromptOptions.fontFilepath = "assets/fonts/roboto-regular.ttf";
    equationPromptOptions.text = "x^2";
    equationPromptOptions.style.rect = { 16, 16, 256, 0 };
    equationPromptOptions.style.borderColor = { 128, 128, 128, 255 };
    equationPromptOptions.focusedStyle = equationPromptOptions.style;
    equationPromptOptions.focusedStyle.borderColor = { 255, 255, 255, 255 };

    RZUF3_ObjectDefinition objEquationPrompt;
    objEquationPrompt.name = "equation_prompt";
    objEquationPrompt.parentName = "ui_root";
    //objEquationPrompt.pos = { 17, 54, 2.3, 4.5 };
    objEquationPrompt.scripts = {
        new RZUF3_TextInput(equationPromptOptions),
        new ExpressionInput(),
        new RZUF3_WindowRectAnchor(RZUF3_WindowRectAnchorOptions{{
            {0.0, 0.0, 16, 16}, 
            {1.0, 0.0, -16, 16}
        }})
    };
    m_sceneDef->objects.push_back(objEquationPrompt);

    // graph //
    MathGraphOptions graphOptions;
    graphOptions.expression = "x^2";
    graphOptions.textFontFilepath = "assets/fonts/roboto-regular.ttf";

    RZUF3_DraggableOptions graphDraggableOptions;
    graphDraggableOptions.centerFix = false;
    graphDraggableOptions.scaleDragFix = true;
    graphDraggableOptions.lockCursor = true;
    graphDraggableOptions.maxScale = 10000.0;
    graphDraggableOptions.minScale = 0.1;
    graphDraggableOptions.initialScale = 50.0;

    RZUF3_ObjectDefinition objGraph;
    objGraph.name = "graph";
    objGraph.scripts = {
        new MathGraph(graphOptions),
        new RZUF3_Draggable(graphDraggableOptions),
        new RZUF3_WindowRectAnchor(RZUF3_WindowRectAnchorOptions{{
            {0.0, 0.0, 16, 58},
            {1.0, 1.0, -16, -52}
        }})
    };
    m_sceneDef->objects.push_back(objGraph);

    // author //
    RZUF3_ObjectDefinition objAuthor;
	objAuthor.name = "author";

    RZUF3_TextRendererOptions authorOptions;
    authorOptions.fontFilepath = "assets/fonts/roboto-regular.ttf";
    authorOptions.text = "izawartka 2024.11";
    authorOptions.style.useLangFile = false;
    authorOptions.style.alignment = RZUF3_Align_BottomRight;

    objAuthor.scripts = {
        new RZUF3_TextRenderer(authorOptions),
        new RZUF3_WindowAnchor({1, 1, -16, -16})
    };
    m_sceneDef->objects.push_back(objAuthor);

    return m_sceneDef;
}

void MainScene::destroy()
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