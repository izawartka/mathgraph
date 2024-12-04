#pragma once
#include "../common.h"
#include "../events/events.h"

class ExpressionInput : public RZUF3_ObjectScript {
public:
	ExpressionInput();
	~ExpressionInput();

	void init();
	void deinit();

protected:
	void onUIStringValueChange(RZUF3_UIStringValueChangeEvent* event);
	void onMathExpressionError(User_MathExpressionErrorEvent* event);

	void removeTextInput();
	void createTextInput();
	void updateBorderColor(bool error);

	RZUF3_TextInput* m_textInput = nullptr;

	_DECLARE_LISTENER(UIStringValueChange);
	_DECLARE_LISTENER(MathExpressionError)
};