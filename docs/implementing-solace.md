# Implementing Solace
Solace is a functional programming language written entirely in C. This document details the definition of the language as well as necessary implementation details for the compiler. This document is a living docuemnt and is updated as work is completed for this project.

## Introduction to Solace
Solace is built to be simple to understand, so that, if you are familiar with some popular programming languages, you are able to begin using the language fairly quickly. The philosophy of the Solace compiler development is: **Simplicity and pragmatism**. Programs should be expressive enough to detail computation, but still remain powerfully consice. This balance is in most ways a contradiction, but it is in that contradiction that interesting things can be done.

The Solace compiler is called *anvl*. Solace is a compiled language, and *anvl* compiles the solace source files to llvm byte code that is passed to the llvm compiler for further compilation.

## Solace Program structure

Solace source files are marked with the extension: `[filename].solc`. Files that have any other extension fail.

Solace programs are made up of modules. Modules are spaces in which functions and data structures are defined. A single file can have multiple module definitions as module names are not directly tied to file names.
The entrypoint of a Solace program is the main function, which **must** be defined within the main module. Typically, the main function returns an integer to specify exit status (where `0` signals a clean exit).

```
mod Main do
    main int do
        0;
    end
end
```

Module spaces encapsulate functions and data structures, enabling control over the exportation of those functions and data strutures. This initial version of Solace does not support the ability to define anything as private, however, this is a concept that will be implemented in the future.

### Functions & Functoids
Functions are a core part of the Solace language. Like most modern functional programming languages, Solace treats functions as first class citizens. To fully encapsulate this ideal, anytime an assignment occurs in Solace, we are really defining a function.
All 'variable' definitions are expanded into single line function definitions; in Solace, these are called functoids. A functoid is defined by its name, followed by its return type, then an equal sign, `=`, and finally the expression the functoid computes and returns, ending with a semicolon `;`.

`value int = 0;`

While this may seem trivial, this gives the language the ability to open any and all functions to working with other functions without the need to specify which arguments are functions and which are not. Everything is a function.
We can pass arguments into functoids by wrapping them in parenthesis before the assignment.

`double int (n int) = n * 2;`

Functions that need more than one line can be defined using a block syntax. We define the body block of the function within a `do..end` block. We define the arguments for a block function as we do with functoids, wrapping them in parenthesis. If we are not accepting arguments for a function, we can omit the parenthesis altogether.
```
double int (n int) do
	n * 2;
end

main int do
	double(2);
end
```

#### Returning Values from Functions
There is no keyword for return in Solace. Instead, the semicolon character, `;`, is used to define what is being returned from a function. For this reason, semicolons are always expected at the end of a functoid.

`square int (n int) = n * n;`

The functoid, `square`, takes a single integer function, called `n`, and returns the result of calling `n` multiplied by `n`, which must evaluate to an integer value.

#### Function Redeclaration
All assignments in Solace are immutable. There is no way to redefine existing functoids and functions. This is to ensure the behavior of defined functions within a program.

## Types in Solace
Solace boasts a number of simple data types: *integers*, *floats*, *characters*, *strings*, and *booleans*.

### Arrays
Arrays are a composite type that allow you to store a sequence of values for a given type. Arrays are defined by surrounding a type with brackets (`[int]`, `[string]`, etc.). You do not need to specify the size of the array, but it is important to note that array sizes are fixed upon definition.

`array_values [char] = ['a', 'b', 'c'];`

We are technically able to increase the size of an array by creating a new array with the concat expression: `+>`. This, however, does not alter the array but returns a new array. The target being concatenated to the end of the array must also be an array and defined to the appropriate depth.
```
mod Main do
	main int do
		numbers [int] = [1,2,3];
		IO:out(numbers[1]) // prints the value 2
		
		new_numbers [int] = numbers +> [4];
		IO:out(new_numbers[3]) // prints the value 4
		
		matrix [[int]] = [[1,2], [3,4]];
		new_matrix [[int]] = matrix +> [[5,6]];
        IO:out(new_matrix) // prints [[1,2], [3,4], [5,6]]
		
		0;
	end
end
```
