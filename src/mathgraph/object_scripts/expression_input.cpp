#include "expression_input.h"
#include "../events/events.h"

ExpressionInput::ExpressionInput(std::string startExpression)
{
	mi_expression = startExpression;
}

ExpressionInput::~ExpressionInput()
{

}

void ExpressionInput::init()
{
	createTextInput();
	updateBorderColor();

	RZUF3_EventsManager* objEventsManager = m_object->getEventsManager();
	_ADD_LISTENER(objEventsManager, UIValueChange);

	RZUF3_EventsManager* eventsManager = g_scene->getEventsManager();
	_ADD_LISTENER_CL(eventsManager, MathExpressionError, User);

	setUseOnUpdate(true);
}

void ExpressionInput::deinit()
{
	RZUF3_EventsManager* objEventsManager = m_object->getEventsManager();
	_REMOVE_LISTENER(objEventsManager, UIValueChange);

	RZUF3_EventsManager* eventsManager = g_scene->getEventsManager();
	_REMOVE_LISTENER_CL(eventsManager, MathExpressionError, User);

	setUseOnUpdate(false);

	removeTextInput();
}

void ExpressionInput::onUpdate(RZUF3_UpdateEvent* event)
{
	if (!m_useOnUpdate) return;
	sendExpression();
	setUseOnUpdate(false);
}

void ExpressionInput::onUIValueChange(RZUF3_UIValueChangeEvent* event)
{
	if(event->getTypeIndex() != typeid(std::string)) return;
	std::string value = *(std::string*)event->getValue();

	mi_expression = value;
	sendExpression();
}

void ExpressionInput::onMathExpressionError(User_MathExpressionErrorEvent* event)
{
	mi_error = event->getErrorType() != NO_ERROR;
	updateBorderColor();
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
	textInputOptions.text = mi_expression;
	textInputOptions.styleSet.styles[0].textStyle.fontFilepath = MAIN_FONT;
	textInputOptions.styleSet.copyFromNormal();
	textInputOptions.styleSet.defaultColors();
	textInputOptions.multiline = false;

	m_textInput = new RZUF3_TextInput(textInputOptions);
	m_object->addScript(m_textInput);
}

void ExpressionInput::updateBorderColor()
{
	if (!m_textInput) return;

	RZUF3_TextInputStyle style = m_textInput->getStyle(false);
	style.borderBoxStyle.color = mi_error ? SDL_Color{ 128, 64, 64, 255 } : SDL_Color{ 128, 128, 128, 255 };

	m_textInput->setStyle(style, false);

	RZUF3_TextInputStyle focusedStyle = m_textInput->getStyle(true);
	focusedStyle.borderBoxStyle.color = mi_error ? SDL_Color{ 255, 0, 0, 255 } : SDL_Color{ 255, 255, 255, 255 };

	m_textInput->setStyle(focusedStyle, true);
}

void ExpressionInput::sendExpression()
{
	User_SetMathExpressionEvent objEvent(mi_expression);
	g_scene->getEventsManager()->dispatchEvent(&objEvent);
}

void ExpressionInput::setUseOnUpdate(bool useOnUpdate)
{
	m_useOnUpdate = useOnUpdate;

	if (useOnUpdate && !m_hasOnUpdateListener) {
		RZUF3_EventsManager* eventsManager = g_scene->getEventsManager();
		_ADD_LISTENER(eventsManager, Update);
		m_hasOnUpdateListener = true;
	}

	if (!useOnUpdate && m_hasOnUpdateListener) {
		RZUF3_EventsManager* eventsManager = g_scene->getEventsManager();
		_REMOVE_LISTENER(eventsManager, Update);
		m_hasOnUpdateListener = false;
	}
}
