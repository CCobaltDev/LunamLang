# LunamLang

A High-Level, Modern, Object-Oriented/Multi-Paradigm Programming Language

## Code example

```
import std::io;
public class Main {
    public static function main():void {
        var a:i32 = 9;
        var b = 10; // i32
        io::println("What's {} + {}? {}", a, b, add(a, b + 2));
		
        for (var i = 0; i < io::args.length(); i++) {
            io::println(io::args[i]);    
        }
    }
    
    static inline function add(a: i32, b: i32): i32 {
        return a + b;
    }
}
```