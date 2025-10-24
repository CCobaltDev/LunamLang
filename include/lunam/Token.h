#ifndef TOKEN_H
#define TOKEN_H

#include <cstdint>
#include <variant>
#include <string>
#include <ostream>

#include "magic_enum/magic_enum.hpp"

enum class TokenType
{
	STRING_LITERAL,
	INTEGER_LITERAL,
	FLOAT_LITERAL,
	BOOL_LITERAL,

	IDENTIFIER,
	KEYWORD,

	PLUS,	   // +
	MINUS,	   // -
	STAR,	   // *
	SLASH,	   // /
	MODULO,	   // %
	EQUALS,	   // =
	AMPERSAND, // &
	PIPE,	   // |
	TILDA,	   // ~
	CARAT,	   // ^
	BANG,	   // !

	PLUS_EQUALS,   // +=
	MINUS_EQUALS,  // -=
	STAR_EQUALS,   // *=
	SLASH_EQUALS,  // /=
	MODULO_EQUALS, // %=

	AMPERSAND_EQUALS, // &=
	PIPE_EQUALS,	  // |=
	CARAT_EQUALS,	  // ^=

	LSHIFT,		   // <<
	RSHIFT,		   // >>
	LSHIFT_EQUALS, // <<=
	RSHIFT_EQUALS, // >>=

	INCREMENT, // ++
	DECREMENT, // --

	EQUALS_TO,		// ==
	NOT_EQUALS,		// !=
	LESS_THAN,		// <
	GREATER_THAN,	// >
	LESS_EQUALS,	// <=
	GREATER_EQUALS, // >=

	AND, // &&
	OR,	 // ||

	QUESTION,	   // ?
	NULL_ACCESS,   // ?.
	NULL_COALESCE, // ??
	NULL_ASSIGN,   // ??=

	SEMICOLON, // ;
	DOT,	   // .
	COMMA,	   // ,
	DOLLAR,	   // $
	COLON,	   // :
	ARROW,	   // ->
	META,	   // @:

	LPAREN,	  // (
	RPAREN,	  // )
	LBRACE,	  // {
	RBRACE,	  // }
	LBRACKET, // [
	RBRACKET, // ]

	COMMENT,
	EoF,
	ILLEGAL,
};

enum class KeywordType
{
	NAMESPACE,
	IMPORT,
	CLASS,
	ABSTRACT,
	ENUM,
	EXTERN,

	PUBLIC,
	PROTECTED,
	PRIVATE,
	STATIC,
	INLINE,
	OVERRIDE,
	OVERLOAD,

	IF,
	ELSE,
	SWITCH,
	CASE,
	BREAK,
	DEFAULT,

	FOR,
	WHILE,
	DO,
	CONTINUE,

	NEW,
	DEL,

	TRY,
	CATCH,
	FINALLY,

	SUPER,
	THIS,
	RETURN,
	NULL_T,
};

using TokenValue = std::variant<std::monostate, std::string, int64_t, double, bool, KeywordType>;

struct Token
{
	TokenType type;
	TokenValue value;

	uint32_t line, col;

	Token(TokenType type, TokenValue value, uint32_t line, uint32_t col)
		: type{type}, value{value}, line{line}, col{col} {};
};

std::ostream &operator<<(std::ostream &os, const Token &obj);
#endif