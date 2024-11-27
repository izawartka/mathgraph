#pragma once
#include "common.h"

class MainScene {
public:
	static RZUF3_SceneDefinition* getSceneDef();
	static void destroy();

private:
	static RZUF3_SceneDefinition* m_sceneDef;
};