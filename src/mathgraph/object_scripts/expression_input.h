#pragma once
#include "../common.h"

class ExpressionInput : public RZUF3_ObjectScript {
public:
	ExpressionInput();
	~ExpressionInput();

	void init();
	void deinit();

protected:
	void onUIStringValueChange(RZUF3_UIStringValueChangeEvent* event);

	_DECLARE_LISTENER(UIStringValueChange);
};