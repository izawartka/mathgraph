#pragma once
#include "events.h"

class User_SetMathExpressionEvent : public RZUF3_Event {
public:
	User_SetMathExpressionEvent(std::string text) {
		m_text = text;
	}

	std::string getText() const { return m_text; }

private:
	std::string m_text;
};
