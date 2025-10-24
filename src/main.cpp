#include <iostream>
#include "lunam/Lexer.h"

int main()
{
	Lexer scanner{"+ - * / % = & | ~ ^ ! += -= *= /= %= &= |= ^= << >> <<= >>= ++ -- == != < > <= >= && || ? ?. ?? ??= ; . , $ : -> @: namespace import class abstract enum extern public protected private static inline override overload if else switch case default for while do new del return null\n//AAHello, World!AA what the FUCK\nH\n/* hai */"};

	Token t = scanner.nextToken();
	while (t.type != TokenType::EoF)
	{
		std::cout << t << '\n';
		t = scanner.nextToken();
	}

	return 0;
}