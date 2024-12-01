# Assembler

This program generates a listing file from assembly code, similar to NASM. It uses **Flex** and **Bison** for parsing.


## Quick Start

1. Open a terminal and run:
```bash
make
```

2. The following files are generated:
- Listing File: `output.lst`
- Symbol Table: `p.sym`

By default, the assembler parses the code from `t.s`.



## Using Your Own Assembly File
1. Compile the parser by running:
```bash
make p.o
```

2. Provide your assembly file as input:
```bash
./p.o < yourfile.s
```