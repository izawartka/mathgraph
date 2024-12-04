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

    // equation label //
    RZUF3_TextRendererOptions equationLabelOptions;
    equationLabelOptions.style.fontFilepath = "assets/fonts/roboto-regular.ttf";
    equationLabelOptions.text = "equation_label";
    equationLabelOptions.useLangFile = true;
    equationLabelOptions.alignment = RZUF3_Align_TopRight;
    equationLabelOptions.dstRect = { 32, 16+5, 0, 0 };

    RZUF3_ObjectDefinition objEquationLabel;
    objEquationLabel.name = "equation_label";
    objEquationLabel.parentName = "ui_root";
    objEquationLabel.scripts = {
		new RZUF3_TextRenderer(equationLabelOptions),
	};
    m_sceneDef->objects.push_back(objEquationLabel);

    // equation prompt //
    RZUF3_ObjectDefinition objEquationPrompt;
    objEquationPrompt.name = "equation_prompt";
    objEquationPrompt.parentName = "ui_root";
    objEquationPrompt.scripts = {
        new ExpressionInput(),
        new RZUF3_WindowRectAnchor(RZUF3_WindowRectAnchorOptions{{
            {0.0, 0.0, 32+4, 16}, 
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
            {1.0, 1.0, -16, -50}
        }})
    };
    m_sceneDef->objects.push_back(objGraph);

    // author //
    RZUF3_ObjectDefinition objAuthor;
	objAuthor.name = "author";

    RZUF3_TextRendererOptions authorOptions;
    authorOptions.style.fontFilepath = "assets/fonts/roboto-regular.ttf";
    authorOptions.text = "izawartka 2024.11";
    authorOptions.useLangFile = false;
    authorOptions.alignment = RZUF3_Align_BottomRight;

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