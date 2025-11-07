#include <iostream>
#include "lunam/Lexer.h"

int main()
{
	Lexer scanner{"+ - * / % = & | ~ ^ ! += -= *= /= %= &= |= ^= << >> <<= >>= ++ -- == != < > <= >= && || ? ?. ?? ??= ; . , $ : -> @: namespace import class abstract enum extern public protected private static inline override overload if else switch case default for while do new del return null hello\n//AAHello, World!AA what the\nH\n/* hai */1\n2.3\n0xFF\n\n\"Hello, World!\""};

	Token t = scanner.nextToken();
	while (1)
	{
		std::cout << t << '\n';
		if (t.type == TokenType::EndOfFile)
			break;
		t = scanner.nextToken();
	}

	return 0;
}