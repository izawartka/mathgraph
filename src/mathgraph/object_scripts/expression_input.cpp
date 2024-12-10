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
	createTextInput();

	RZUF3_EventsManager* objEventsManager = m_object->getEventsManager();
	_ADD_LISTENER(objEventsManager, UIValueChange);

	RZUF3_EventsManager* eventsManager = g_scene->getEventsManager();
	_ADD_LISTENER_CL(eventsManager, MathExpressionError, User);
}

void ExpressionInput::deinit()
{
	RZUF3_EventsManager* objEventsManager = m_object->getEventsManager();
	_REMOVE_LISTENER(objEventsManager, UIValueChange);

	RZUF3_EventsManager* eventsManager = g_scene->getEventsManager();
	_REMOVE_LISTENER_CL(eventsManager, MathExpressionError, User);

	removeTextInput();
}

void ExpressionInput::onUIValueChange(RZUF3_UIValueChangeEvent* event)
{
	if(event->getTypeIndex() != typeid(std::string)) return;
	std::string value = *(std::string*)event->getValue();

	User_SetMathExpressionEvent objEvent(value);
	g_scene->getEventsManager()->dispatchEvent(&objEvent);
}

void ExpressionInput::onMathExpressionError(User_MathExpressionErrorEvent* event)
{
	updateBorderColor(event->getErrorType() != NO_ERROR);
}

void ExpressionInput::removeTextInput()
{
	m_object->removeScript(m_textInput);
	delete m_textInput;
	m_textInput = nullptr;
}

void ExpressionInput::createTextInput()
{
	RZUF3_TextInputOptions textInputOptions;
	textInputOptions.text = "x^2";
	textInputOptions.style.textStyle.fontFilepath = "assets/fonts/roboto-regular.ttf";
	textInputOptions.multiline = false;
	textInputOptions.style.borderBoxStyle.color = { 128, 128, 128, 255 };
	textInputOptions.focusedStyle = textInputOptions.style;
	textInputOptions.focusedStyle.borderBoxStyle.color = { 255, 255, 255, 255 };

	m_textInput = new RZUF3_TextInput(textInputOptions);
	m_object->addScript(m_textInput);
}

void ExpressionInput::updateBorderColor(bool error)
{
	if (!m_textInput) return;

	RZUF3_TextInputStyle style = m_textInput->getStyle(false);
	style.borderBoxStyle.color = error ? SDL_Color{ 128, 64, 64, 255 } : SDL_Color{ 128, 128, 128, 255 };

	m_textInput->setStyle(style, false);

	RZUF3_TextInputStyle focusedStyle = m_textInput->getStyle(true);
	focusedStyle.borderBoxStyle.color = error ? SDL_Color{ 255, 0, 0, 255 } : SDL_Color{ 255, 255, 255, 255 };

	m_textInput->setStyle(focusedStyle, true);
}
