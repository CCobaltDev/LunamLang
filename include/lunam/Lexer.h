#ifndef LEXER_H
#define LEXER_H

#include <unordered_map>
#include <cstdint>
#include <string>

#include "fast_float/fast_float.h"

#include "lunam/Token.h"

class Lexer
{
private:
	uint32_t pos = 0;
	uint32_t line = 1, col = 1, tokenLine = 1, tokenCol = 1;

	std::string src;

	static inline constexpr auto num_flags = fast_float::chars_format::general | fast_float::chars_format::allow_leading_plus;
	static inline constexpr auto decimal_num_format = fast_float::parse_options{num_flags, '.', 10};
	static inline constexpr auto hexadecimal_num_format = fast_float::parse_options{num_flags, '.', 16};
	static inline constexpr auto binary_num_format = fast_float::parse_options{num_flags, '.', 2};

	static inline const std::unordered_map<std::string, KeywordType> keywords{
		{"import", KeywordType::Import},
		{"namespace", KeywordType::Namespace},
		{"class", KeywordType::Class},
		{"abstract", KeywordType::Abstract},
		{"interface", KeywordType::Interface},
		{"enum", KeywordType::Enum},
		{"public", KeywordType::Public},
		{"protected", KeywordType::Protected},
		{"private", KeywordType::Private},
		{"static", KeywordType::Static},
		{"inline", KeywordType::Inline},
		{"override", KeywordType::Override},
		{"overload", KeywordType::Overload},
		{"if", KeywordType::If},
		{"else", KeywordType::Else},
		{"switch", KeywordType::Switch},
		{"case", KeywordType::Case},
		{"break", KeywordType::Break},
		{"default", KeywordType::Default},
		{"for", KeywordType::For},
		{"while", KeywordType::While},
		{"do", KeywordType::Do},
		{"continue", KeywordType::Continue},
		{"new", KeywordType::New},
		{"del", KeywordType::Del},
		{"throw", KeywordType::Throw},
		{"try", KeywordType::Try},
		{"catch", KeywordType::Catch},
		{"finally", KeywordType::Finally},
		{"super", KeywordType::Super},
		{"this", KeywordType::This},
		{"return", KeywordType::Return},
		{"cast", KeywordType::Cast},
		{"null", KeywordType::Null},
	};

	bool match(char c) noexcept;
	void advance() noexcept;
	const char peek(uint32_t ahead = 1) const noexcept { return src[pos + ahead]; };
	const char cur() const noexcept { return src[pos]; };
	bool isAtEnd(uint32_t ahead = 0) const noexcept { return (pos + ahead) >= src.length(); };

	[[nodiscard]] Token make(TokenType type) const noexcept;
	[[nodiscard]] Token make(TokenType type, TokenValue value) const noexcept;
	[[nodiscard]] Token make(TokenType type, uint32_t line, uint32_t col) const noexcept;
	[[nodiscard]] Token make(TokenType type, TokenValue value, uint32_t line, uint32_t col) const noexcept;

public:
	Token nextToken();

	Lexer(std::string src) : src{std::move(src)} {};
};

#endif