#pragma once
#include "../common.h"
#include "../events/events.h"

class SceneSwitchButton : public RZUF3_ObjectScript {
public:
	SceneSwitchButton(RZUF3_SceneDefinition* sceneDef);
	~SceneSwitchButton();

	void init();
	void deinit();

protected:
	void onUIButtonClick(RZUF3_UIButtonClickEvent* event);

	RZUF3_SceneDefinition* mp_sceneDef = nullptr;

	_DECLARE_LISTENER(UIButtonClick)
};