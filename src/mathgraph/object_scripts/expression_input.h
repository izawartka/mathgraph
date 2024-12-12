#pragma once
#include "../common.h"
#include "../events/events.h"

class ExpressionInput : public RZUF3_ObjectScript {
public:
	ExpressionInput(std::string startExpression);
	~ExpressionInput();

	void init();
	void deinit();

protected:
	void onUpdate(RZUF3_UpdateEvent* event);
	void onUIValueChange(RZUF3_UIValueChangeEvent* event);
	void onMathExpressionError(User_MathExpressionErrorEvent* event);

	void removeTextInput();
	void createTextInput();
	void updateBorderColor();
	void sendExpression();
	void setUseOnUpdate(bool useOnUpdate);

	RZUF3_TextInput* m_textInput = nullptr;
	bool m_useOnUpdate = false;
	bool m_hasOnUpdateListener = false;

	std::string mi_expression; // interscene
	bool mi_error = false; // interscene

	_DECLARE_LISTENER(Update)
	_DECLARE_LISTENER(UIValueChange)
	_DECLARE_LISTENER(MathExpressionError)
};