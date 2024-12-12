#pragma once
#include "../common.h"
#include "math_expression.h"

class MathParser {
public:
	static bool parseExpression(std::string strExpression, MathExpression& outExpression, MathError& error);
	static void debugPrintExpression(MathExpression& expression);

private:
	static void removeSpaces(std::string& str);
	static void translateShortMultiplication(std::string& strExpression);
	static bool checkUnbalancedBrackets(std::string strExpression, MathError& error);

	static bool getSubExpressions(std::string strExpression, std::vector<MathSubExpression>& subExpressions, MathError& error, char operators[2], bool allowEmpty);
	static bool parseOperations(std::string strExpression, MathExpression& outExpression, MathError& error, MathOperand& outOperand, int level);
	static bool parseSpecial(std::string strExpression, MathExpression& outExpression, MathError& error, MathOperand& outOperand);

	static int findNextNonSpaceCharIndex(std::string str, int start);
	static int findFunctionName(std::string str, const MathFunctionsItem** foundFunction = nullptr);
	static bool isStringNumber(std::string str);
	static int findInnerStringLength(std::string str, int start);
	static void addSeparationCharToString(std::string& str, int startIndex, int length, char separationChar, bool skipLast = true);
};