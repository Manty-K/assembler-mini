# Assembler

This program generates a *listing file*, *symbol table* and *object code* from assembly code, similar to NASM. It uses **Flex** and **Bison** for parsing.


### Object Code Structure
```
---------------------------------------------
| 16 byte signature      (Manthan Khandale) |
---------------------------------------------
| 8 byte data offset   | 8 byte Text Offset |
---------------------------------------------
| 8 byte Symbol Table  | 8 byte String Off  |
---------------------------------------------
| Data Section                              |
---------------------------------------------
| Text Section                              |
---------------------------------------------
| Symbol Table                              |
---------------------------------------------
| String Table                              |
---------------------------------------------
```

## Quick Start

1. Open a terminal and run:
```bash
make SRC=youfile.s
```

2. The following files are generated:
- Listing File: `youfile.lst`
- Symbol Table: `youfile.sym`
- Object File: `youfile.o`

By default, the assembler parses the code from `t.s`.


