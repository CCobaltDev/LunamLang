#include "lunam/Token.h"

std::ostream &operator<<(std::ostream &os, const Token &obj)
{
	os << magic_enum::enum_name(obj.type);

	switch (obj.type)
	{
	case TokenType::STRING_LITERAL:
	case TokenType::IDENTIFIER:
	case TokenType::COMMENT:
		os << '(' << std::get<std::string>(obj.value) << ')';
		break;
	case TokenType::INTEGER_LITERAL:
		os << '(' << std::get<int64_t>(obj.value) << ')';
		break;
	case TokenType::FLOAT_LITERAL:
		os << '(' << std::get<double>(obj.value) << ')';
		break;
	case TokenType::BOOL_LITERAL:
		os << '(' << std::get<bool>(obj.value) << ')';
		break;
	case TokenType::KEYWORD:
		os << '(' << magic_enum::enum_name(std::get<KeywordType>(obj.value)) << ')';
		break;
	default:
		break;
	}

	os << ' ' << obj.line << ':' << obj.col;

	return os;
}