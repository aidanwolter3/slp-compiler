# ult-compiler
This Ultimate homemade compiler was originally made for a Programming Translation course at LeTourneau University, but it has since been modified and tuned.

The compiler translates the Ultimate language into x86 assembly for either Mac or Ubuntu or x64 assembly for Mac.

*Fun fact: the language is based off of the sport Ultimate Frisbee.*

# build
Make was used to simplify the build process. The compiler will generate an x86/x64 assembly file that can then be assembled using Nasm. The ```makefile``` includes targets for assembling to either Mac or Ubuntu.

***Compiling the compiler***
```bash
$ make
```

***Run the compiler on an input file***
```bash
$ ./ult [-f format] [-t target] filename
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

This is a sample of the Ultimate language.
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

The parse table was generated using [lalr1-gen](https://github.com/aidanwolter3/lalr1-gen).

The language is an LALR1 and the productions are listed below.

* S' -> S
* S -> S S
* S ->
* S -> id assign E
* S -> huck id
* E -> num
* E -> id
* E -> str
* E -> E B E
* L -> E comma L
* L -> E
* B -> plus
* B -> minus
* B -> mult
* B -> div

