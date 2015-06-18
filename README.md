# slp-compiler
This homemade compiler was originally made for a Programming Translation course at LeTourneau University, but it has since been modified and tuned.

The compiler translates a Straight Line Program (SLP) into x86 assembly for either Mac or Ubuntu.

# build
Make was used to simplify the build process. The compiler will generate an x86 assembly file that can then be assembled using Nasm. The ```makefile``` includes targets for assembling to either Mac or Ubuntu.

***Compiling the compiler***
```bash
$ make
```

***Run the compiler on an input file***
```bash
$ ./compiler filename mac
# or ./compiler filename ubu
```

***Assemble the output file***
```bash
$ make mac-exe
# or make ubu-exe
```

***Run the complete SLP***
```bash
$ ./output
```

# language

This is a sample of a Straight Line Program (SLP).
```
a = 1
b = 2
c = 3
d = 4

e = a + b + c + d
huck e
```

The language is defined in two files: ```lex_table.csv``` and ```parse_table.csv```.
* ```lex_table.csv``` defines the tokens to be matched in the lexer.
* ```parse_table.csv``` defines proper syntax.

The language is an LALR1 and the productions are listed below.

* S' -> S
* S -> S S
* S -> id = E
* S -> huck E
* S -> epsilon
* E -> id
* E -> num
* E -> E B E
* L -> E , L
* L -> E
* B -> +
* B -> -
* B -> *
* B -> /
