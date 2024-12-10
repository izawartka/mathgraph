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
	void onUIValueChange(RZUF3_UIValueChangeEvent* event);
	void onMathExpressionError(User_MathExpressionErrorEvent* event);

	void removeTextInput();
	void createTextInput();
	void updateBorderColor(bool error);

	RZUF3_TextInput* m_textInput = nullptr;

	_DECLARE_LISTENER(UIValueChange)
	_DECLARE_LISTENER(MathExpressionError)
};