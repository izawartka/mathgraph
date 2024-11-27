#include "expression_input.h"
#include "../events/events.h"

ExpressionInput::ExpressionInput()
{

}

ExpressionInput::~ExpressionInput()
{

}

void ExpressionInput::init()
{
	RZUF3_EventsManager* objEventsManager = m_object->getEventsManager();
	_ADD_LISTENER(objEventsManager, UIStringValueChange);
}

void ExpressionInput::deinit()
{
	RZUF3_EventsManager* objEventsManager = m_object->getEventsManager();
	_REMOVE_LISTENER(objEventsManager, UIStringValueChange);
}

void ExpressionInput::onUIStringValueChange(RZUF3_UIStringValueChangeEvent* event)
{
	User_SetMathExpressionEvent objEvent(event->getValue());
	g_scene->getEventsManager()->dispatchEvent(&objEvent);
}
