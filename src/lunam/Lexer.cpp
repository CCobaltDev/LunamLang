#include "lunam/Lexer.h"

Lexer::Lexer(std::string source) : src{std::move(source)}
{
	src_iter = src.begin();
	end_iter = src.end();

	cur_iter  = src_iter;
	cur_char  = utf8::next(src_iter, end_iter);
	peek_iter = src_iter;
	peek_char = src_iter == end_iter ? EOF_VALUE : utf8::next(src_iter, end_iter);
}

bool Lexer::match(utf8::utfchar32_t c) noexcept
{
	if (isAtEnd())
		return false;
	bool matched = peek() == c;
	if (matched)
		advance();
	return matched;
}

void Lexer::advance() noexcept
{
	if (isAtEnd())
		return;

	utf8::utfchar32_t current = cur();
	cur_iter				  = peek_iter;
	cur_char				  = peek_char;
	peek_iter				  = src_iter;
	peek_char				  = src_iter == end_iter ? EOF_VALUE : utf8::next(src_iter, end_iter);

	col++;
	if (current == '\n')
	{
		col = 1;
		line++;
	}
}

Token Lexer::nextToken()
{
	if (isAtEnd())
		return make(TokenType::EndOfFile, line, col);

	while (cur() == ' ' || cur() == '\t' || cur() == '\n' || cur() == '\r')
		advance();

	utf8::utfchar32_t current = cur();

	token_col  = col;
	token_line = line;

	Token t = [&]()
	{
		switch (current)
		{
		case '+':
			return make(match('+') ? TokenType::Increment : match('=') ? TokenType::PlusEquals : TokenType::Plus);
		case '-':
			return make(match('>')	 ? TokenType::Arrow
						: match('-') ? TokenType::Decrement
						: match('=') ? TokenType::MinusEquals
									 : TokenType::Minus);
		case '*':
			return make(match('=') ? TokenType::StarEquals : TokenType::Star);
		case '/':
			if (match('/'))
			{
				advance();
				auto start = cur_iter;
				while (cur() != '\n' && !isAtEnd())
				{
					advance();
				}

				return make(TokenType::Comment, std::string{start, cur_iter});
			}
			else if (match('*'))
			{
				advance();
				auto start = cur_iter;

				while (!(cur() == '*' && peek() == '/'))
				{
					advance();
				}
				if (isAtEnd())
				{
					// TODO handle unterminated comment
				}
				auto end = cur_iter;
				advance();
				return make(TokenType::Comment, std::string{start, end});
			}

			return make(match('=') ? TokenType::SlashEquals : TokenType::Slash);
		case '%':
			return make(match('=') ? TokenType::ModuloEquals : TokenType::Modulo);
		case '=':
			return make(match('=') ? TokenType::EqualsTo : TokenType::Equals);
		case '&':
			return make(match('&') ? TokenType::And : match('=') ? TokenType::AmpersandEquals : TokenType::Ampersand);
		case '|':
			return make(match('|') ? TokenType::Or : match('=') ? TokenType::PipeEquals : TokenType::Pipe);
		case '~':
			return make(TokenType::Tilde);
		case '^':
			return make(match('=') ? TokenType::CaretEquals : TokenType::Caret);
		case '!':
			return make(match('=') ? TokenType::NotEquals : TokenType::Bang);
		case '<':
			return make(match('<')	 ? match('=') ? TokenType::LeftShiftEquals : TokenType::LeftShift
						: match('=') ? TokenType::LessEquals
									 : TokenType::LessThan);
		case '>':
			return make(match('>')	 ? match('=') ? TokenType::RightShiftEquals : TokenType::RightShift
						: match('=') ? TokenType::GreaterEquals
									 : TokenType::GreaterThan);
		case '?':
			return make(match('?')	 ? match('=') ? TokenType::NullAssign : TokenType::NullCoalesce
						: match('.') ? TokenType::NullAccess
									 : TokenType::Question);
		case ';':
			return make(TokenType::Semicolon);
		case '.':
			return make(TokenType::Dot);
		case ',':
			return make(TokenType::Comma);
		case '$':
			return make(TokenType::Dollar);
		case ':':
			return make(match(':') ? TokenType::NamespaceAccess : TokenType::Colon);
		case '@':
			return make(match(':') ? TokenType::Meta : TokenType::Illegal);
		case '(':
			return make(TokenType::LeftParen);
		case ')':
			return make(TokenType::RightParen);
		case '{':
			return make(TokenType::LeftBrace);
		case '}':
			return make(TokenType::RightBrace);
		case '[':
			return make(TokenType::LeftBracket);
		case ']':
			return make(TokenType::RightBracket);
		case '"':
		case '\'':
		{
			advance();
			auto start = cur_iter;
			while (!(peek() == current && cur() != current) && !isAtEnd())
			{
				advance();
			}
			if (isAtEnd())
			{
				// TODO handle unterminated string literal
			}
			auto end = cur_iter;
			advance();

			return make(TokenType::StringLiteral, std::string(start, end));
		}

		default:
			if (current >= '0' && current <= '9')
			{
				bool hex	= (peek() == 'x' || peek() == 'X') && current == '0';
				bool binary = (peek() == 'b' || peek() == 'B') && current == '0';

				const auto &format = hex ? hexadecimal_num_format : binary ? binary_num_format : decimal_num_format;

				if (binary || hex)
				{
					advance();
					advance();
				}

				uint64_t value;
				const char *start = &(*cur_iter);

				auto result = fast_float::from_chars_advanced(start, src.data() + src.size(), value, format);
				if (!result)
				{
					while (peek() >= '0' && peek() <= '9')
					{
						advance();
					}
					return make(TokenType::Illegal);
				}

				uintptr_t diff = (result.ptr - start);

				col			 += diff;
				auto new_src  = cur_iter + diff;
				cur_iter	  = new_src;
				cur_char	  = new_src == end_iter ? EOF_VALUE : utf8::next(new_src, end_iter);
				peek_iter	  = new_src;
				peek_char	  = new_src == end_iter ? EOF_VALUE : utf8::next(new_src, end_iter);
				src_iter	  = new_src;
				return make(TokenType::NumberLiteral, (binary || hex) ? static_cast<double>(value) : value);
			}
			else if ((current >= 'a' && current <= 'z') || (current >= 'A' && cur() <= 'Z') || current == '_' ||
					 current >= 128)
			{
				auto start = cur_iter;

				while ((cur() >= 'a' && cur() <= 'z') || (cur() >= 'A' && cur() <= 'Z') ||
					   (cur() >= '0' && cur() <= '9') || cur() == '_' || cur() >= 128 && !isAtEnd())
				{
					advance();
				}

				std::string identifier{start, cur_iter};
				auto keyword = keywords.find(identifier);
				if (keyword != keywords.end())
				{
					return make(TokenType::Keyword, keyword->second);
				}
				else
				{
					return make(TokenType::Identifier, identifier);
				}
			}

			return make(TokenType::Illegal);
		}
	}();

	advance();
	return t;
}

Token Lexer::make(TokenType type) const noexcept { return Token(type, std::monostate{}, token_line, token_col); }

Token Lexer::make(TokenType type, TokenValue value) const noexcept { return Token(type, value, token_line, token_col); }

Token Lexer::make(TokenType type, uint32_t line, uint32_t col) const noexcept
{
	return Token(type, std::monostate{}, line, col);
}

Token Lexer::make(TokenType type, TokenValue value, uint32_t line, uint32_t col) const noexcept
{
	return Token(type, value, line, col);
}