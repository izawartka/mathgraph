#pragma once
#include "../common.h"

enum MathErrorType {
	NO_ERROR,

	// Parsing errors
	UNCLOSED_BRACKET,
	UNEXPECTED_BRACKET,
	EMPTY_BRACKET,
	EMPTY_SUBEXPRESSION,
	MIN_TWO_OPERANDS,
	UNKNOWN_SYMBOL,
	UNEXPECTED_OPERATOR,
	EMPTY_EXPRESSION,
	INVALID_NUMBER,
	BRACKETS_REQUIRED,
	NOT_ENOUGH_OPERANDS,
	TOO_MANY_OPERANDS,

	// Math errors
	NO_OPERATIONS,
	NO_OPERANDS,
	UNSUPPORTED_OPERAND_TYPE,
	UNSUPPORTED_OPERATION,
	VARIABLE_NOT_FOUND,
	NOT_A_NUMBER,

	// Graph errors
	TOO_MUCH_VARIABLES
};

struct MathError {
	MathErrorType type = NO_ERROR;
	unsigned int position = 0;
};

enum MathOperationType {
	NONE,
	// 0 level
	ADD,
	SUB,
	// 1 level
	MULTIPLY,
	DIVIDE,
	// 2 level
	POWER,
	// 3 level
	ROOT,
	SQRT,
	ASINH,
	ACOSH,
	ACTGH,
	ATGH,
	SINH,
	COSH,
	CTGH,
	TGH,
	ASIN,
	ACOS,
	ACTG,
	ATG,
	SIN,
	COS,
	CTG,
	TG,
	ABS,
	LN,
	LOG,
	EXP,
	MOD,
	MIN,
	MAX
};

struct MathFunctionsItem {
	std::string name;
	MathOperationType operation;
	int minOperands = 1;
	int maxOperands = 1;
};

const std::vector<MathFunctionsItem> mathFunctions = {
	{"sqrt", SQRT},
	{"root", ROOT, 2, 2},
	{"asinh", ASINH},
	{"acosh", ACOSH},
	{"actgh", ACTGH},
	{"atgh", ATGH},
	{"sinh", SINH},
	{"cosh", COSH},
	{"ctgh", CTGH},
	{"tgh", TGH},
	{"asin", ASIN},
	{"acos", ACOS},
	{"actg", ACTG},
	{"atg", ATG},
	{"sin", SIN},
	{"cos", COS},
	{"ctg", CTG},
	{"tg", TG},
	{"abs", ABS},
	{"ln", LN},
	{"log", LOG, 1, 2},
	{"exp", EXP},
	{"mod", MOD, 2, 2},
	{"min", MIN, 1, -1},
	{"max", MAX, 1, -1}
};

struct MathOperationLevelsItem {
	char operators[3];
	MathOperationType positiveOperation;
	MathOperationType negativeOperation;
};

const std::vector<MathOperationLevelsItem> mathOperationLevels = {
	{ "+-", ADD, SUB },
	{ "*/", MULTIPLY, DIVIDE },
	{ "^ ", POWER, SQRT }
};

enum MathOperandType {
	CONST,
	VARIABLE,
	OP_RESULT,
};

struct MathOperand {
	MathOperandType type;
	unsigned long long index;
};

struct MathOperation {
	MathOperationType operation;
	std::vector<MathOperand> operands;
};

struct MathExpression {
	std::vector<double> constants;
	std::vector<std::string> variables;
	std::vector<MathOperation> operations;
};

struct MathSubExpression {
	std::string expression;
	bool isNegative;
};