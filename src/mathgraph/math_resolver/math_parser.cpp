#include "math_parser.h"

bool MathParser::parseExpression(std::string strExpression, MathExpression& outExpression, MathError& error)
{

	error.type = NO_ERROR;
	error.position = 0;

	removeSpaces(strExpression);
	translateShortMultiplication(strExpression);
	// spdlog::debug("Parsing expression: {}", strExpression);
	if(!checkUnbalancedBrackets(strExpression, error)) return false;

	MathOperand outOperand;
	if (!parseOperations(strExpression, outExpression, error, outOperand, 0)) return false;

	if (outOperand.type != OP_RESULT) {
		outExpression.operations.push_back({ NONE, {outOperand} });
	}

	return true;
}

void MathParser::debugPrintExpression(MathExpression& expression)
{
    spdlog::info("Constants:");
    for (double constant : expression.constants) {
		spdlog::info("{}", constant);
	}
	spdlog::info("");

    spdlog::info("Variables:");
    for (const std::string& variable : expression.variables) {
        spdlog::info("{}", variable);
    }
	spdlog::info("");

    spdlog::info("Operations:");
    for (const MathOperation& operation : expression.operations) {
        switch (operation.operation) {
		case ADD: spdlog::info("ADD"); break;
		case SUB: spdlog::info("SUB"); break;
		case MULTIPLY: spdlog::info("MULTIPLY"); break;
		case DIVIDE: spdlog::info("DIVIDE"); break;
		case POWER: spdlog::info("POWER"); break;
		case SQRT: spdlog::info("SQRT"); break;
		case SIN: spdlog::info("SIN"); break;
		case COS: spdlog::info("COS"); break;
		case CTG: spdlog::info("CTG"); break;
		case TG: spdlog::info("TG"); break;
		default: spdlog::info("UNKNOWN"); break;
		}

		for (const MathOperand& operand : operation.operands) {
			switch (operand.type) {
				case CONST: spdlog::info("CONST {}", expression.constants[operand.index]); break;
				case VARIABLE: spdlog::info("VAR {}", expression.variables[operand.index]); break;
				case OP_RESULT: spdlog::info("OP_RESULT {}", operand.index); break;
				default: spdlog::info("UNKNOWN"); break;
			}
		}

		spdlog::info("");
	}

    spdlog::info("End of expression.");

}

void MathParser::removeSpaces(std::string& str)
{
	std::string newStr = "";
	for (char c : str) {
		if (c != ' ') newStr += c;
	}

	str = newStr;
}

void MathParser::translateShortMultiplication(std::string& strExpression)
{
	std::string newExpression = "";

	std::string lastAlpha = "";
	bool lastCharNumeric = false;
	char lastChar = ' ';
	for (int i = 0; i < strExpression.size(); ++i) {
		char thisChar = strExpression[i];

		bool thisCharAlpha = isalpha(thisChar);
		bool thisCharNumeric = isdigit(thisChar) || thisChar == '.';
		bool isStartOfFunction = thisChar == '(' && findFunctionName(lastAlpha) != -1;

		if (lastChar == ')') {
			if (thisChar == '(') newExpression += "*";
			else if (isalpha(thisChar)) newExpression += "*";
			else if (isdigit(thisChar) || thisChar == '.') newExpression += "*";
		}
		else if (lastCharNumeric) {
			if (thisChar == '(') newExpression += "*";
			else if (thisCharAlpha) newExpression += "*";
		}
		else if (!thisCharAlpha && lastAlpha.size() > 0 && findFunctionName(lastAlpha) == -1) {
			int alphaSize = lastAlpha.size();
			bool skipLast = thisChar != '(' && !thisCharNumeric;
			addSeparationCharToString(newExpression, newExpression.size() - alphaSize, alphaSize, '*', skipLast);
		}
		else if (thisChar == '(') {
			if (lastAlpha.size() > 0 && !isStartOfFunction) {
				newExpression += "*";
			}
		}

		newExpression += thisChar;
		lastChar = thisChar;
		lastCharNumeric = thisCharNumeric;
		lastAlpha = thisCharAlpha ? lastAlpha + thisChar : "";
	}

	if (lastAlpha.size() > 0 && findFunctionName(lastAlpha) == -1) {
		int alphaSize = lastAlpha.size();
		addSeparationCharToString(newExpression, newExpression.size() - alphaSize, alphaSize, '*', true);
	}

	strExpression = newExpression;
}

bool MathParser::checkUnbalancedBrackets(std::string strExpression, MathError& error)
{
	int depth = 0;
	int bracketLength = 0;
	for (int i = 0; i < strExpression.size(); ++i) {
		if (strExpression[i] == '(') {
			++depth;
			bracketLength = 0;
		}
		else if (strExpression[i] == ')') {
			--depth;

			if (bracketLength < 2) {
				error.type = EMPTY_BRACKET;
				error.position = i;
				return false;
			}
		} 

		++bracketLength;

		if (depth < 0) {
			error.type = UNEXPECTED_BRACKET;
			error.position = i;
			return false;
		}
	}

	if (depth != 0) {
		error.type = UNCLOSED_BRACKET;
		error.position = strExpression.size();
		return false;
	}

	return true;
}

bool MathParser::getSubExpressions(std::string strExpression, std::vector<MathSubExpression>& subExpressions, MathError& error, char operators[2], bool allowEmpty)
{
	subExpressions.clear();

	int size = strExpression.size();
	int depth = 0;
	int start = 0;
	int end = start;
	bool isNextNegative = false;

	for (int i = 0; i < size; ++i) {
		if (strExpression[i] == '(') {
			++depth;
		}
		else if (strExpression[i] == ')') {
			--depth;
		}
		if (depth != 0) continue;

		bool isLastChar = i == size - 1;
		if (strExpression[i] != operators[0] && strExpression[i] != operators[1]) {
			end = i + 1;
			if (!isLastChar) continue;
		}
		else if (isLastChar) {
			error.type = UNEXPECTED_OPERATOR;
			error.position = i;
			return false;
		}

		bool isEmpty = end - start <= 0;
		if (!isEmpty) {
			std::string subexpression = strExpression.substr(start, end - start);
			subExpressions.push_back({
				subexpression,
				isNextNegative
				});
		}

		if (!isLastChar) start = findNextNonSpaceCharIndex(strExpression, i + 1);

		if (isEmpty) {
			if (!allowEmpty) {
				error.type = EMPTY_SUBEXPRESSION;
				error.position = i;
				return false;
			}

			if (strExpression[i] == operators[1]) {
				isNextNegative = !isNextNegative;
			}
		}
		else {
			isNextNegative = (strExpression[i] == operators[1]);
		}
	}

	return true;
}

bool MathParser::parseOperations(std::string strExpression, MathExpression& outExpression, MathError& error, MathOperand& outOperand, int level)
{
	if (level >= mathOperationLevels.size()) return parseSpecial(strExpression, outExpression, error, outOperand);

	if(strExpression.size() == 0) {
		error.type = EMPTY_EXPRESSION;
		error.position = 0;
		return false;
	}

	MathOperationLevelsItem levelItem = mathOperationLevels[level];
	std::vector<MathSubExpression> subExpressions;
	if (!getSubExpressions(strExpression, subExpressions, error, levelItem.operators, level == 0)) return false;
	int subExpressionsCount = subExpressions.size();
	std::vector<MathOperand> positiveOperands;
	std::vector<MathOperand> negativeOperands;

	for (int i = 0; i < subExpressionsCount; i++) {
		MathSubExpression& subExpression = subExpressions[i];

		MathOperand operand;
		if (!parseOperations(subExpression.expression, outExpression, error, operand, level + 1)) return false;

		if (subExpression.isNegative) negativeOperands.push_back(operand);
		else positiveOperands.push_back(operand);
	}

	bool hasPositiveOperands = positiveOperands.size() > 0;
	bool hasNegativeOperands = negativeOperands.size() > 0;

	if (subExpressionsCount == 1) {
		// only one negative
		if (hasNegativeOperands) {
			MathOperation operation = { levelItem.negativeOperation, {negativeOperands[0]}};
			outExpression.operations.push_back(operation);
			outOperand = { OP_RESULT, outExpression.operations.size() - 1 };
			return true;
		}
		// only one positive
		else {
			outOperand = positiveOperands[0];
			return true;
		}
	}

	if (hasNegativeOperands) {
		MathOperation negativeOperation = { levelItem.negativeOperation, negativeOperands};
		outExpression.operations.push_back(negativeOperation);
	}

	if (hasPositiveOperands) {
		if (hasNegativeOperands) {
			positiveOperands.push_back({ OP_RESULT, outExpression.operations.size() - 1 });
		}

		MathOperation positiveOperation = { levelItem.positiveOperation, positiveOperands };
		outExpression.operations.push_back(positiveOperation);
	}

	outOperand = { OP_RESULT, outExpression.operations.size() - 1 };

	return true;
}

bool MathParser::parseSpecial(std::string strExpression, MathExpression& outExpression, MathError& error, MathOperand& outOperand)
{
	if (strExpression.size() == 1 && isalpha(strExpression[0])) {
		auto it = std::find(outExpression.variables.begin(), outExpression.variables.end(), strExpression);
		if (it != outExpression.variables.end()) {
			unsigned long long index = std::distance(outExpression.variables.begin(), it);
			outOperand = { VARIABLE, index };
			return true;
		}

		outExpression.variables.push_back(strExpression);
		outOperand = { VARIABLE, outExpression.variables.size() - 1 };
		return true;
	}

	if (isStringNumber(strExpression)) {
		if (strExpression.size() > 300) {
			error.type = INVALID_NUMBER;
			error.position = 0;
			return false;
		}

		double constant;
		try {
			constant = std::stod(strExpression);
		}
		catch (std::exception e) {
			error.type = INVALID_NUMBER;
			error.position = 0;
			return false;
		}

		auto it = std::find(outExpression.constants.begin(), outExpression.constants.end(), constant);
		if (it != outExpression.constants.end()) {
			unsigned long long index = std::distance(outExpression.constants.begin(), it);
			outOperand = { CONST, index };
			return true;
		}

		outExpression.constants.push_back(constant);
		outOperand = { CONST, outExpression.constants.size() - 1 };
		return true;
	}

	if (strExpression[0] == '(' && strExpression[strExpression.size() - 1] == ')') {
		std::string innerExpression = strExpression.substr(1, strExpression.size() - 2);
		return parseOperations(innerExpression, outExpression, error, outOperand, 0);
	}

	const MathFunctionsItem* foundFunction;
	int functionNameIndex = findFunctionName(strExpression, &foundFunction);

	if(foundFunction == nullptr || functionNameIndex != 0) {
		error.type = UNKNOWN_SYMBOL;
		error.position = 0;
		return false;
	}

	int functionNameSize = foundFunction->name.size();
	bool hasBrackets = strExpression[functionNameSize] == '(' && strExpression[strExpression.size() - 1] == ')';
	std::string innerText = hasBrackets ? 
		strExpression.substr(functionNameSize + 1, strExpression.size() - functionNameSize - 2) : 
		strExpression.substr(functionNameSize);
	std::vector<std::string> innerExpressions;

	int depth = 0;
	int currentExpressionStart = 0;
	for(int i = 0; i < innerText.size(); ++i) {
		if (innerText[i] == '(') ++depth;
		else if (innerText[i] == ')') --depth;

		if (depth < 0) {
			error.type = UNEXPECTED_BRACKET;
			error.position = i;
			return false;
		}

		if (depth == 0 && innerText[i] == ',') {
			innerExpressions.push_back(innerText.substr(currentExpressionStart, i - currentExpressionStart));
			currentExpressionStart = i + 1;
		}

		if (i == innerText.size() - 1) {
			innerExpressions.push_back(innerText.substr(currentExpressionStart));
		}
	}

	if (depth != 0) {
		error.type = UNCLOSED_BRACKET;
		error.position = innerText.size();
		return false;
	}

	std::vector<MathOperand> operands;
	for(std::string& innerExpression : innerExpressions) {
		MathOperand innerOperand;
		if (!parseOperations(innerExpression, outExpression, error, innerOperand, 0)) return false;

		operands.push_back(innerOperand);
	}

	int operandCount = operands.size();
	if(operandCount > 1 && !hasBrackets) {
		error.type = BRACKETS_REQUIRED;
		error.position = functionNameSize;
		return false;
	}

	if (foundFunction->minOperands != -1 && operandCount < foundFunction->minOperands) {
		error.type = NOT_ENOUGH_OPERANDS;
		error.position = functionNameSize;
		return false;
	}

	if (foundFunction->maxOperands != -1 && operandCount > foundFunction->maxOperands) {
		error.type = TOO_MANY_OPERANDS;
		error.position = functionNameSize;
		return false;
	}

	MathOperation operation = { foundFunction->operation, operands };
	outExpression.operations.push_back(operation);
	outOperand = { OP_RESULT, outExpression.operations.size() - 1 };
	return true;
}

int MathParser::findNextNonSpaceCharIndex(std::string str, int start)
{
	for (int i = start; i < str.size(); ++i) {
		if (str[i] != ' ') return i;
	}

	return str.size();
}

int MathParser::findFunctionName(std::string str, const MathFunctionsItem** foundFunction)
{
	if (str.size() == 0) return -1;

	for (int i = 0; i < str.size(); ++i) {
		for (const MathFunctionsItem& item : mathFunctions) {
			if (str.find(item.name) == i) {
				if (foundFunction != nullptr) *foundFunction = &item;
				return i;
			}
		}
	}

	return -1;
}

bool MathParser::isStringNumber(std::string str)
{
	if(str.size() == 1 && !isdigit(str[0])) return false;

	for (char c : str) {
		if (!isdigit(c) && c != '.') return false;
	}

	return true;
}

int MathParser::findInnerStringLength(std::string str, int start)
{
	bool wasFunctionName = false;
	bool wasNumber = false;
	bool wasText = false;
	for (int i = start; i < str.size(); ++i) {
		char thisChar = str[i];
		if (thisChar == '(' || thisChar == ')' || thisChar == ' ') return i - start;
	}

	return str.size() - start;
}

void MathParser::addSeparationCharToString(std::string& str, int startIndex, int length, char separationChar, bool skipLast)
{
	std::string newStr = str.substr(0, startIndex);
	for (int i = 0; i < length; ++i) {
		newStr += str[startIndex + i];
		if (i < length - 1 || !skipLast) newStr += separationChar;
	}

	newStr += str.substr(startIndex + length);
	str = newStr;
}
