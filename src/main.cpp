#include <iostream>
#include "lunam/Lexer.h"

int main()
{
	Lexer scanner{"34.432432\n3.141\n0xFF\n37\n0b10010"};

	Token t = scanner.nextToken();
	while (1)
	{
		std::cout << t << '\n';
		if (t.type == TokenType::EoF)
			break;
		t = scanner.nextToken();
	}

	return 0;
}