#ifndef TOKEN_H
#define TOKEN_H

#include <cstdint>
#include <variant>
#include <string>
#include <ostream>

#include "magic_enum/magic_enum.hpp"

enum class TokenType
{
	StringLiteral,
	NumberLiteral,
	BoolLiteral,

	Identifier,
	Keyword,

	Plus,	   // +
	Minus,	   // -
	Star,	   // *
	Slash,	   // /
	Modulo,	   // %
	Equals,	   // =
	Ampersand, // &
	Pipe,	   // |
	Tilde,	   // ~
	Caret,	   // ^
	Bang,	   // !

	PlusEquals,	  // +=
	MinusEquals,  // -=
	StarEquals,	  // *=
	SlashEquals,  // /=
	ModuloEquals, // %=

	AmpersandEquals, // &=
	PipeEquals,		 // |=
	CaretEquals,	 // ^=

	LeftShift,		  // <<
	RightShift,		  // >>
	LeftShiftEquals,  // <<=
	RightShiftEquals, // >>=

	Increment, // ++
	Decrement, // --

	EqualsTo,	   // ==
	NotEquals,	   // !=
	LessThan,	   // <
	GreaterThan,   // >
	LessEquals,	   // <=
	GreaterEquals, // >=

	And, // &&
	Or,	 // ||

	Question,	  // ?
	NullAccess,	  // ?.
	NullCoalesce, // ??
	NullAssign,	  // ??=

	Semicolon,		 // ;
	Dot,			 // .
	Comma,			 // ,
	Dollar,			 // $
	Colon,			 // :
	Arrow,			 // ->
	Meta,			 // @:
	NamespaceAccess, // ::

	LeftParen,	  // (
	RightParen,	  // )
	LeftBrace,	  // {
	RightBrace,	  // }
	LeftBracket,  // [
	RightBracket, // ]

	Comment,
	EndOfFile,
	Illegal,
};

enum class KeywordType
{
	Import,
	Namespace,
	Class,
	Abstract,
	Interface,
	Enum,

	Public,
	Protected,
	Private,
	Static,
	Inline,
	Override,
	Overload,

	If,
	Else,
	Switch,
	Case,
	Break,
	Default,

	For,
	While,
	Do,
	Continue,

	New,
	Del,

	Throw,
	Try,
	Catch,
	Finally,

	Super,
	This,
	Return,
	Cast,

	Null
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