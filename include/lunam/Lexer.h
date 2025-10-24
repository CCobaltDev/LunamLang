#ifndef LEXER_H
#define LEXER_H

#include <unordered_map>
#include <string_view>
#include <cstdint>
#include <string>
#include "lunam/Token.h"

class Lexer
{
private:
	uint32_t pos = 0;
	uint32_t line = 1, col = 1, tokenLine = 1, tokenCol = 1;

	std::string src;

	static inline std::unordered_map<std::string, KeywordType> keywords{
		{"namespace", KeywordType::NAMESPACE},
		{"import", KeywordType::IMPORT},
		{"class", KeywordType::CLASS},
		{"abstract", KeywordType::ABSTRACT},
		{"enum", KeywordType::ENUM},
		{"extern", KeywordType::EXTERN},
		{"public", KeywordType::PUBLIC},
		{"protected", KeywordType::PROTECTED},
		{"private", KeywordType::PRIVATE},
		{"static", KeywordType::STATIC},
		{"inline", KeywordType::INLINE},
		{"override", KeywordType::OVERRIDE},
		{"overload", KeywordType::OVERLOAD},
		{"if", KeywordType::IF},
		{"else", KeywordType::ELSE},
		{"switch", KeywordType::SWITCH},
		{"case", KeywordType::CASE},
		{"break", KeywordType::BREAK},
		{"default", KeywordType::DEFAULT},
		{"for", KeywordType::FOR},
		{"while", KeywordType::WHILE},
		{"do", KeywordType::DO},
		{"continue", KeywordType::CONTINUE},
		{"new", KeywordType::NEW},
		{"del", KeywordType::DEL},
		{"try", KeywordType::TRY},
		{"catch", KeywordType::CATCH},
		{"super", KeywordType::SUPER},
		{"this", KeywordType::THIS},
		{"return", KeywordType::RETURN},
		{"null", KeywordType::NULL_T},
	};

	bool match(char c) noexcept;
	void advance() noexcept;
	const char peek(uint32_t ahead = 1) const noexcept { return src[pos + ahead]; };
	const char cur() const noexcept { return src[pos]; };
	bool isAtEnd() const noexcept { return pos >= src.length(); };

	[[nodiscard]] Token make(TokenType type) const noexcept;
	[[nodiscard]] Token make(TokenType type, TokenValue value) const noexcept;
	[[nodiscard]] Token make(TokenType type, uint32_t line, uint32_t col) const noexcept;
	[[nodiscard]] Token make(TokenType type, TokenValue value, uint32_t line, uint32_t col) const noexcept;

public:
	Token nextToken();

	Lexer(std::string src) : src{std::move(src)} {};
};

#endif