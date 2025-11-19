#ifndef LEXER_H
#define LEXER_H

#include "fast_float/fast_float.h" // IWYU pragma: export
#include "lunam/Token.h"
#include "utf8.h"				   // IWYU pragma: export
#include <cstdint>
#include <string>
#include <unordered_map>

#define EOF_VALUE UINT32_MAX

class Lexer
{
private:
	uint32_t line		= 1;
	uint32_t col		= 1;
	uint32_t token_line = 1;
	uint32_t token_col	= 1;
	uint32_t pos		= 0;

	std::string::iterator cur_iter;
	std::string::iterator peek_iter;
	std::string::iterator src_iter;
	std::string::iterator end_iter;

	utf8::utfchar32_t cur_char;
	utf8::utfchar32_t peek_char;

	std::string src;

	static inline constexpr auto num_flags =
		fast_float::chars_format::general | fast_float::chars_format::allow_leading_plus;

	static inline constexpr auto decimal_num_format		= fast_float::parse_options{num_flags, '.', 10};
	static inline constexpr auto hexadecimal_num_format = fast_float::parse_options{num_flags, '.', 16};
	static inline constexpr auto binary_num_format		= fast_float::parse_options{num_flags, '.', 2};

	static inline const std::unordered_map<std::string, KeywordType> keywords{
		{"import", KeywordType::Import},
		{"namespace", KeywordType::Namespace},
		{"class", KeywordType::Class},
		{"abstract", KeywordType::Abstract},
		{"interface", KeywordType::Interface},
		{"struct", KeywordType::Struct},
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
		{"throw", KeywordType::Throw},
		{"try", KeywordType::Try},
		{"catch", KeywordType::Catch},
		{"finally", KeywordType::Finally},
		{"new", KeywordType::New},
		{"super", KeywordType::Super},
		{"this", KeywordType::This},
		{"return", KeywordType::Return},
		{"null", KeywordType::Null},
	};

	bool match(utf8::utfchar32_t c) noexcept;
	void advance() noexcept;
	const utf8::utfchar32_t peek() const noexcept { return peek_char; };
	const utf8::utfchar32_t cur() const noexcept { return cur_char; };
	bool isAtEnd() const noexcept { return cur() == EOF_VALUE; };

	[[nodiscard]] Token make(TokenType type) const noexcept;
	[[nodiscard]] Token make(TokenType type, TokenValue value) const noexcept;
	[[nodiscard]] Token make(TokenType type, uint32_t line, uint32_t col) const noexcept;
	[[nodiscard]] Token make(TokenType type, TokenValue value, uint32_t line, uint32_t col) const noexcept;

public:
	Token nextToken();

	Lexer(std::string source);
};

#endif