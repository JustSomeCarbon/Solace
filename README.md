# Solace

A small functional programming language written entirely in C.
Solace is built to be easy to pick up and use, as well as 
efficient and fast.

## The Compiler

The Solace compiler is called anvl.
Source file names must end with `.solx` to signify they are Solace
source files.

Usage::
```
$ ./anvl -[flags] [source file]
```

### Flags

- h -- Prints some information to the console regarding the basic
    information regarding the compiler.
- v -- Prints the version information of the compiler.
- x -- Prints the Token stack emitted from the lexer.
