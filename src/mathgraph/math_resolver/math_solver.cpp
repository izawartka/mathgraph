#include "math_solver.h"

bool MathSolver::solveExpression(MathExpression& expression, std::vector<double> variables, double& outResult, MathError& error)
{
	if(expression.operations.size() == 0)
	{
		error.type = NO_OPERATIONS;
		return false;
	}

	error.type = NO_ERROR;
	error.position = 0;

	std::vector<double> operationResults;

	for (int i = 0; i < expression.operations.size(); i++)
	{
		MathOperation& operation = expression.operations[i];
		std::vector<double> operandValues;

		for (auto& operand : operation.operands)
		{
			switch (operand.type)
			{
			case MathOperandType::CONST:
				operandValues.push_back(expression.constants[operand.index]);
				break;
			case MathOperandType::VARIABLE:
				if (operand.index >= variables.size()) {
					error.type = VARIABLE_NOT_FOUND;
					return false;
				}
				operandValues.push_back(variables[operand.index]);
				break;
			case MathOperandType::OP_RESULT:
				operandValues.push_back(operationResults[operand.index]);
				break;
			default:
				error.type = UNSUPPORTED_OPERAND_TYPE;
				return false;
			}
		}

		double result;
		if (!solveOperation(operation.operation, operandValues, result, error)) {
			return false;
		}

		operationResults.push_back(result);
	}

	outResult = operationResults[operationResults.size() - 1];

	return true;
}

bool MathSolver::solveOperation(MathOperationType operationType, std::vector<double>& operandValues, double& outResult, MathError& error)
{
	if(operandValues.size() == 0)
	{
		error.type = NO_OPERANDS;
		return false;
	}

	switch (operationType)
	{
	case NONE:
		outResult = operandValues[0];
		break;
	case ADD:
		outResult = 0;
		for(auto& operand : operandValues)
			outResult += operand;
		break;
	case SUB:
		outResult = 0;
		for(auto& operand : operandValues)
			outResult -= operand;
		break;
	case MULTIPLY:
		outResult = 1;
		for(auto& operand : operandValues)
			outResult *= operand;
		break;
	case DIVIDE:
		outResult = 1;
		for(auto& operand : operandValues)
			outResult /= operand;
		break;
	case POWER:
		outResult = operandValues[0];
		for(int i = 1; i < operandValues.size(); i++)
			outResult = pow(outResult, operandValues[i]);
		break;
	case SQRT:
		/// TODO: non square roots
		outResult = sqrt(operandValues[0]);
		break;
	case SIN:
		outResult = sin(operandValues[0]);
		break;
	case COS:
		outResult = cos(operandValues[0]);
		break;
	case CTG:
		outResult = 1 / tan(operandValues[0]);
		break;
	case TG:
		outResult = tan(operandValues[0]);
		break;
	case ABS:
		outResult = abs(operandValues[0]);
		break;
	case LN:
		outResult = log(operandValues[0]);
		break;
	case LOG:
		outResult = log10(operandValues[0]);
		break;
	case EXP:
		outResult = exp(operandValues[0]);
		break;
	default:
		error.type = UNSUPPORTED_OPERATION;
		return false;
	}

	if (isnan(outResult)) {
		error.type = NOT_A_NUMBER;
		return false;
	}

	return true;
}