# LunamLang

A High-level, Object-oriented, Modern Programming Language

```
import std::io;

public class Main {
	public static function main():void {
		var a:i32 = 9;
		var b = 10; // i32

		io::println("What's {} + {}? {}", a, b, add(a, b + 2)); // What's 9 + 10? 21 (/ref)
	}

	static inline function add(a: i32, b: i32): i32 {
		return a + b;
	}
}```