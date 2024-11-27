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

	// Math errors
	NO_OPERATIONS,
	NO_OPERANDS,
	UNSUPPORTED_OPERAND_TYPE,
	UNSUPPORTED_OPERATION,
	VARIABLE_NOT_FOUND,
	NOT_A_NUMBER,
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
	SQRT,
	SIN,
	COS,
	CTG,
	TG,
	ABS,
	LN,
	LOG,
	EXP
};

struct MathFunctionsItem {
	std::string name;
	MathOperationType operation;
};

const std::vector<MathFunctionsItem> mathFunctions = {
	{"sqrt", SQRT},
	{"sin", SIN},
	{"cos", COS},
	{"ctg", CTG},
	{"tg", TG},
	{"abs", ABS},
	{"ln", LN},
	{"log", LOG},
	{"exp", EXP}
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