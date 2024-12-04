#pragma once
#include "events.h"
#include "../math_resolver/math_expression.h"

class User_MathExpressionErrorEvent : public RZUF3_Event {
public:
	User_MathExpressionErrorEvent(MathErrorType errorType) {
		m_error = errorType;
	}

	MathErrorType getErrorType() const { return m_error; }

private:
	MathErrorType m_error;
};
