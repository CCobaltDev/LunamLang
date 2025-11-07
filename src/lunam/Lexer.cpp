#include "lunam/Lexer.h"

bool Lexer::match(char c) noexcept
{
	if (isAtEnd())
		return false;
	bool matched = peek() == c;
	if (matched)
		advance();
	return matched;
}

Token Lexer::make(TokenType type) const noexcept
{
	return Token(type, std::monostate{}, tokenLine, tokenCol);
}

Token Lexer::make(TokenType type, TokenValue value) const noexcept
{
	return Token(type, value, tokenLine, tokenCol);
}

Token Lexer::make(TokenType type, uint32_t line, uint32_t col) const noexcept
{
	return Token(type, std::monostate{}, line, col);
}

Token Lexer::make(TokenType type, TokenValue value, uint32_t line, uint32_t col) const noexcept
{
	return Token(type, value, line, col);
}

void Lexer::advance() noexcept
{
	char current = cur();
	pos++;
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
		return make(TokenType::EndOfFile, tokenLine, tokenCol + 1);
	while (cur() == ' ' || cur() == '\t' || cur() == '\n' || cur() == '\r')
		advance();

	const char current = cur();
	tokenCol = col;
	tokenLine = line;

	Token t = [&]()
	{
		switch (current)
		{
		case '+':
			return make(match('+')	 ? TokenType::Increment
						: match('=') ? TokenType::PlusEquals
									 : TokenType::Plus);
		case '-':
			return make(match('>')	 ? TokenType::Arrow
						: match('-') ? TokenType::Decrement
						: match('=') ? TokenType::MinusEquals
									 : TokenType::Minus);
		case '*':
			return make(match('=') ? TokenType::StarEquals
								   : TokenType::Star);
		case '/':
			if (match('/'))
			{
				advance();
				uint32_t startPos = pos;
				while (cur() != '\n' && !isAtEnd())
				{
					advance();
				}
				return make(TokenType::Comment, src.substr(startPos, pos - startPos));
			}
			else if (match('*')) // TODO handle unterminated comment
			{
				advance();
				uint32_t startPos = pos;
				while (!(cur() == '*' && peek() == '/') && !isAtEnd(1))
				{
					advance();
				}
				if (isAtEnd())
				{
				}
				uint32_t endPos = pos;
				advance();

				return make(TokenType::Comment, src.substr(startPos, endPos - startPos));
			}

			return make(match('=') ? TokenType::SlashEquals
								   : TokenType::Slash);
		case '%':
			return make(match('=') ? TokenType::ModuloEquals
								   : TokenType::Modulo);
		case '=':
			return make(match('=') ? TokenType::EqualsTo
								   : TokenType::Equals);
		case '&':
			return make(match('&')	 ? TokenType::And
						: match('=') ? TokenType::AmpersandEquals
									 : TokenType::Ampersand);
		case '|':
			return make(match('|')	 ? TokenType::Or
						: match('=') ? TokenType::PipeEquals
									 : TokenType::Pipe);
		case '~':
			return make(TokenType::Tilde);
		case '^':
			return make(match('=') ? TokenType::CaretEquals
								   : TokenType::Caret);
		case '!':
			return make(match('=') ? TokenType::NotEquals
								   : TokenType::Bang);
		case '<':
			return make(match('<')	 ? match('=') ? TokenType::LeftShiftEquals
												  : TokenType::LeftShift
						: match('=') ? TokenType::LessEquals
									 : TokenType::LessThan);
		case '>':
			return make(match('>')	 ? match('=') ? TokenType::RightShiftEquals
												  : TokenType::RightShift
						: match('=') ? TokenType::GreaterEquals
									 : TokenType::GreaterThan);
		case '?':
			return make(match('?')	 ? match('=') ? TokenType::NullAssign
												  : TokenType::NullCoalesce
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
			return make(match(':') ? TokenType::Meta
								   : TokenType::Illegal);
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
		case '"': // TODO handle unterminated string
		case '\'':
		{
			advance();
			uint32_t startPos = pos;
			while (!(peek() == current && cur() != '\\') && !isAtEnd(1))
			{
				advance();
			}
			if (isAtEnd())
			{
				// error
			}
			uint32_t endPos = pos;
			advance();

			return make(TokenType::StringLiteral, src.substr(startPos, endPos - startPos));
		}

		default:
			if (std::isdigit(static_cast<unsigned char>(cur())))
			{
				bool hex = (peek() == 'x' || peek() == 'X') && cur() == '0';
				bool binary = (peek() == 'b' || peek() == 'B') && cur() == '0';

				const auto &format = hex	  ? hexadecimal_num_format
									 : binary ? binary_num_format
											  : decimal_num_format;

				if (binary || hex)
				{
					advance();
					advance();

					uint64_t value;
					char *start = src.data() + pos;
					auto result = fast_float::from_chars_advanced(start, src.data() + src.size(), value, format);
					if (!result)
					{
						return make(TokenType::Illegal);
					}
					uint32_t size = result.ptr - start;
					for (uint32_t i = 0; i < size; i++)
						advance();

					return make(TokenType::NumberLiteral, static_cast<double>(value));
				}
				else
				{
					double value;
					char *start = src.data() + pos;
					auto result = fast_float::from_chars_advanced(start, src.data() + src.size(), value, format);
					if (!result)
					{
						return make(TokenType::Illegal);
					}
					uint32_t size = result.ptr - start;
					for (uint32_t i = 0; i < size; i++)
						advance();

					return make(TokenType::NumberLiteral, value);
				}
			}
			else if (std::isalpha(static_cast<unsigned char>(cur())))
			{
				uint32_t startPos = pos;

				while (std::isalpha(static_cast<unsigned char>(peek())) || peek() == '_' || std::isdigit(static_cast<unsigned char>(peek())))
				{
					advance();
				}

				std::string identifier{src.substr(startPos, pos - startPos + 1)};
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