#pragma once
#include "../common.h"
#include "math_expression.h"

#define SMALL_DOUBLE 1e-12

class MathSolver {
public:
	static bool solveExpression(MathExpression& expression, std::vector<double> variables, double& outResult, MathError& error);

private:
	static bool solveOperation(MathOperationType operationType, std::vector<double>& operandValues, double& outResult, MathError& error);
};