#include "scene_switch_button.h"

SceneSwitchButton::SceneSwitchButton(RZUF3_SceneDefinition* sceneDef)
{
	mp_sceneDef = sceneDef;
}

SceneSwitchButton::~SceneSwitchButton()
{
}

void SceneSwitchButton::init()
{
	RZUF3_EventsManager* objEventsManager = m_object->getEventsManager();

	_ADD_LISTENER(objEventsManager, UIButtonClick);
}

void SceneSwitchButton::deinit()
{
	RZUF3_EventsManager* objEventsManager = m_object->getEventsManager();

	_REMOVE_LISTENER(objEventsManager, UIButtonClick);
}

void SceneSwitchButton::onUIButtonClick(RZUF3_UIButtonClickEvent* event)
{
	if (mp_sceneDef == nullptr) {
		spdlog::error("SceneSwitchButton: scene definition is nullptr");
		return;
	}

	g_game->setScene(mp_sceneDef);
}
