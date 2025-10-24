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
		return make(TokenType::EoF);
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
			return make(match('+')	 ? TokenType::INCREMENT
						: match('=') ? TokenType::PLUS_EQUALS
									 : TokenType::PLUS);
		case '-':
			return make(match('>')	 ? TokenType::ARROW
						: match('-') ? TokenType::DECREMENT
						: match('=') ? TokenType::MINUS_EQUALS
									 : TokenType::MINUS);
		case '*':
			return make(match('=') ? TokenType::STAR_EQUALS
								   : TokenType::STAR);
		case '/':
			if (match('/'))
			{
				advance();
				uint32_t oldPos = pos;
				while (cur() != '\n')
				{
					advance();
				}
				return make(TokenType::COMMENT, src.substr(oldPos, pos - oldPos));
			}
			else if (match('*'))
			{
				advance();
				uint32_t oldPos = pos;
				while (!(cur() == '*' && peek() == '/'))
				{
					advance();
				}
				uint32_t endPos = pos;
				advance();

				return make(TokenType::COMMENT, src.substr(oldPos, endPos - oldPos));
			}

			return make(match('=') ? TokenType::SLASH_EQUALS
								   : TokenType::SLASH);
		case '%':
			return make(match('=') ? TokenType::MODULO_EQUALS
								   : TokenType::MODULO);
		case '=':
			return make(match('=') ? TokenType::EQUALS_TO
								   : TokenType::EQUALS);
		case '&':
			return make(match('&')	 ? TokenType::AND
						: match('=') ? TokenType::AMPERSAND_EQUALS
									 : TokenType::AMPERSAND);
		case '|':
			return make(match('|')	 ? TokenType::OR
						: match('=') ? TokenType::PIPE_EQUALS
									 : TokenType::PIPE);
		case '~':
			return make(TokenType::TILDA);
		case '^':
			return make(match('=') ? TokenType::CARAT_EQUALS
								   : TokenType::CARAT);
		case '!':
			return make(match('=') ? TokenType::NOT_EQUALS
								   : TokenType::BANG);
		case '<':
			return make(match('<')	 ? match('=') ? TokenType::LSHIFT_EQUALS
												  : TokenType::LSHIFT
						: match('=') ? TokenType::LESS_EQUALS
									 : TokenType::LESS_EQUALS);
		case '>':
			return make(match('>')	 ? match('=') ? TokenType::RSHIFT_EQUALS
												  : TokenType::RSHIFT
						: match('=') ? TokenType::GREATER_EQUALS
									 : TokenType::GREATER_THAN);
		case '?':
			return make(match('?')	 ? match('=') ? TokenType::NULL_ASSIGN
												  : TokenType::NULL_COALESCE
						: match('.') ? TokenType::NULL_ACCESS
									 : TokenType::QUESTION);
		case ';':
			return make(TokenType::SEMICOLON);
		case '.':
			return make(TokenType::DOT);
		case ',':
			return make(TokenType::COMMA);
		case '$':
			return make(TokenType::DOLLAR);
		case ':':
			return make(TokenType::SEMICOLON);
		case '@':
			return make(match(':') ? TokenType::META
								   : TokenType::ILLEGAL);
		case '(':
			return make(TokenType::LPAREN);
		case ')':
			return make(TokenType::RPAREN);
		case '{':
			return make(TokenType::LBRACE);
		case '}':
			return make(TokenType::RBRACE);
		case '[':
			return make(TokenType::LBRACKET);
		case ']':
			return make(TokenType::RBRACKET);
		default:
			return make(TokenType::ILLEGAL);
		}
	}();

	advance();
	return t;
}