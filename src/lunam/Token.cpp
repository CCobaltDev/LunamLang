#include "lunam/Token.h"

std::ostream &operator<<(std::ostream &os, const Token &obj)
{
	os << magic_enum::enum_name(obj.type);

	switch (obj.type)
	{
	case TokenType::StringLiteral:
	case TokenType::Identifier:
	case TokenType::Comment:
		os << '(' << std::get<std::string>(obj.value) << ')';
		break;
	case TokenType::NumberLiteral:
		os << '(' << std::get<double>(obj.value) << ')';
		break;
	case TokenType::BoolLiteral:
		os << '(' << std::get<bool>(obj.value) << ')';
		break;
	case TokenType::Keyword:
		os << '(' << magic_enum::enum_name(std::get<KeywordType>(obj.value)) << ')';
		break;
	default:
		break;
	}

	os << ' ' << obj.line << ':' << obj.col;

	return os;
}