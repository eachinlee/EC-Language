# EC Programming Language (Elegant Code)

```
    ███████╗ ██████╗
    ██╔════╝██╔════╝
    █████╗  ██║     
    ██╔══╝  ██║     
    ███████╗╚██████╗
    ╚══════╝ ╚═════╝
    Next-Gen Minimalist Language
```

**Version**: 1.1.0  
**License**: MIT License  
**Platforms**: Windows / Linux / macOS

---

## Table of Contents

1. [Introduction](#introduction)
2. [Installation](#installation)
3. [Quick Start](#quick-start)
4. [Syntax Reference](#syntax-reference)
5. [Complete Instruction Set](#complete-instruction-set)
6. [Advanced Features](#advanced-features)
7. [Examples](#examples)

---

## Introduction

**EC (Elegant Code)** is a next-generation minimalist interpreted programming language. The design goal is to provide complete programming functionality with a core instruction set of only 30 commands.

### Design Philosophy

- **Minimal**: 30 core instructions cover all programming needs
- **Unified**: Use `EC` keyword for all variable declarations with dynamic typing
- **Flexible**: Supports both OOP and procedural programming
- **Interoperable**: Built-in C and Python code execution

### Use Cases

- Education: Simple syntax for learning programming concepts
- Rapid Prototyping: Quick algorithm validation
- Scripting: Batch processing and system automation
- Integration: Glue language for C/Python programs

---

## Installation

### Requirements

- GCC Compiler (Windows: MinGW / Linux: gcc)
- Python 3.x (optional, for PYRUN command)

### Windows Build

```batch
cd src
gcc -o EC.exe ec.c -lm
move EC.exe ..
```

### Linux / macOS Build

```bash
cd src
gcc -o EC ec.c -lm
chmod +x EC
mv EC ..
```

### Verify Installation

```bash
./EC --version
# Output: EC Language Interpreter v1.1.0
```

---

## Quick Start

### Hello World

Create file `hello.ec`:

```ec
# My first EC program
OUT "Hello, World!"
END
```

Execute:

```bash
./EC hello.ec
```

### Basic Variable Operations

```ec
# Declare numeric variables
EC age 25
EC pi 3.14159

# Declare string variables
EC name "Alice"

# Modify values
SET age 26
SET name "Bob"

# Output
OUT "Name: " + name
OUT "Age: " + age

END
```

---

## Syntax Reference

### Comments

```ec
# Single line comment
// Also a comment
EC x 10  # Inline comment
```

### Variable Naming Rules

- Must start with letter or underscore
- Can contain letters, digits, underscores
- Case-sensitive
- Cannot use reserved words

### Data Types

| Type | Description | Example |
|------|-------------|---------|
| Number | Integer or float | `42`, `3.14`, `-100` |
| String | Double-quoted text | `"Hello"`, `"EC"` |
| Array | Fixed-size numeric array | `ARR data 10` |
| Object | Class instance | `NEW obj MyClass` |

### Operators

| Type | Operators |
|------|-----------|
| Arithmetic | `+` `-` `*` `/` `%` |
| Comparison | `==` `!=` `>` `<` `>=` `<=` |

---

## Complete Instruction Set

EC language has **30 instructions** in 7 categories:

### 1. Variables (3)

#### EC - Declare Variable

```ec
EC name                  # Declare empty variable
EC count 0               # Declare with number
EC message "Hello"       # Declare with string
EC result a + b * 2      # Declare with expression
```

#### SET - Assign Value

```ec
EC x 10
SET x 20                 # x becomes 20
SET x x + 5              # x becomes 25
SET name "NewName"       # Set string
```

#### ARR - Declare Array

```ec
ARR numbers 10           # Array of size 10
SET numbers[0] 100       # Set first element
SET numbers[1] 200       # Set second element
EC val numbers[0]        # Read element
```

---

### 2. Arithmetic (5)

#### ADD - Addition

```ec
EC x 10
ADD x 5                  # x = x + 5 = 15
ADD x y                  # x = x + y
```

#### SUB - Subtraction

```ec
EC x 10
SUB x 3                  # x = x - 3 = 7
```

#### MUL - Multiplication

```ec
EC x 5
MUL x 4                  # x = x * 4 = 20
```

#### DIV - Division

```ec
EC x 20
DIV x 4                  # x = x / 4 = 5
```

#### MOD - Modulo

```ec
EC x 17
MOD x 5                  # x = x % 5 = 2
```

---

### 3. Input/Output (2)

#### OUT - Output

```ec
OUT "Hello, World!"              # Output string
OUT 42                           # Output number
OUT name                         # Output variable
OUT "Sum = " + a + b             # String concatenation
OUT "Result: " + (x * 2 + 1)     # Output expression
```

#### IN - Input

```ec
IN name "Enter your name: "      # Input with prompt
IN age                           # Input without prompt
# Automatically detects number or string
```

---

### 4. Conditionals (4)

#### IF / ELIF / ELSE / ENDIF

```ec
EC score 85

IF score >= 90
    OUT "Grade: A"
ELIF score >= 80
    OUT "Grade: B"
ELIF score >= 70
    OUT "Grade: C"
ELSE
    OUT "Grade: F"
ENDIF
```

Comparison operators:

| Operator | Description |
|----------|-------------|
| `==` | Equal |
| `!=` | Not equal |
| `>` | Greater than |
| `<` | Less than |
| `>=` | Greater or equal |
| `<=` | Less or equal |

---

### 5. Loops (4)

#### LOOP / ENDLOOP

```ec
# Infinite loop with BREAK
EC i 0
LOOP
    OUT i
    ADD i 1
    IF i >= 5
        BREAK
    ENDIF
ENDLOOP

# Conditional loop
EC count 10
LOOP count > 0
    OUT count
    SUB count 1
ENDLOOP
```

#### BREAK - Exit Loop

```ec
EC i 0
LOOP
    IF i == 5
        BREAK           # Exit when i=5
    ENDIF
    OUT i
    ADD i 1
ENDLOOP
```

#### CONTINUE - Next Iteration

```ec
EC i 0
LOOP i < 10
    ADD i 1
    IF i == 5
        CONTINUE        # Skip 5
    ENDIF
    OUT i
ENDLOOP
```

---

### 6. Functions (4)

#### FN / ENDFN - Define Function

```ec
FN greet(name)
    OUT "Hello, " + name + "!"
ENDFN

FN add(a, b)
    EC sum a + b
    RET sum
ENDFN
```

#### CALL - Call Function

```ec
CALL greet("World")
CALL add(10, 20)
```

#### RET - Return Value

```ec
FN factorial(n)
    IF n <= 1
        RET 1
    ENDIF
    EC result 1
    EC i 2
    LOOP i <= n
        MUL result i
        ADD i 1
    ENDLOOP
    RET result
ENDFN
```

---

### 7. Object-Oriented (4)

#### CLASS / ENDCLASS - Define Class

```ec
CLASS Person
    EC name
    EC age
    
    FN introduce()
        OUT "I am " + name
    ENDFN
ENDCLASS
```

#### NEW - Create Object

```ec
NEW person Person
SET person.name "Alice"
SET person.age 25
```

#### THIS - Current Object Reference

```ec
CLASS Counter
    EC value 0
    
    FN increment()
        ADD THIS.value 1
    ENDFN
ENDCLASS
```

---

### 8. External Execution (3)

#### EXEC - Execute System Command

```ec
EXEC "echo Hello from shell" result
OUT "Shell output: " + result

EXEC "ls -la" files
OUT files
```

#### PYRUN - Execute Python

```ec
# Run Python script
PYRUN "script.py" output

# Call Python function
PYRUN "math_utils.py" calculate(10, 20) result
OUT "Python result: " + result
```

#### CRUN - Compile and Run C

```ec
# Compile and run C source
CRUN "helper.c" result
OUT "C output: " + result
```

---

### 9. Program Control (1)

#### END - End Program

```ec
OUT "Program finished"
END
```

---

## Advanced Features

### Recursive Functions

EC supports recursive calls:

```ec
FN fibonacci(n)
    IF n <= 1
        RET n
    ENDIF
    CALL fibonacci(n - 1)
    EC a return_value
    CALL fibonacci(n - 2)
    EC b return_value
    RET a + b
ENDFN
```

### Nested Loops

```ec
EC i 1
LOOP i <= 9
    EC j 1
    LOOP j <= 9
        EC result i * j
        OUT i + " x " + j + " = " + result
        ADD j 1
    ENDLOOP
    ADD i 1
ENDLOOP
```

### C/Python Interop

See `examples/advanced/` directory for examples.

---

## Examples

### Directory Structure

```
examples/
├── 01_hello.ec           # Hello World
├── 02_variables.ec       # Variables
├── 03_arithmetic.ec      # Arithmetic
├── 04_conditions.ec      # Conditionals
├── 05_loops.ec           # Loops
├── 06_arrays.ec          # Arrays
├── 07_functions.ec       # Functions
├── 08_classes.ec         # OOP
├── 09_multiplication.ec  # Multiplication Table
├── 10_guessing_game.ec   # Number Guessing
└── advanced/
    ├── recursion.ec      # Recursion
    ├── python_call.ec    # Python Integration
    ├── python_helper.py  # Python Helper
    ├── c_call.ec         # C Integration
    └── c_helper.c        # C Helper
```

---

## Error Handling

EC interpreter provides line number error messages:

```
Error line 15: Unknown command 'PIRNT'
Error line 23: Division by zero
Error line 8: Function 'calc' not found
Error line 12: Class 'Person' not found
```

---

## Development

- **Language**: C (C99)
- **Source**: `src/ec.c`
- **Compiler**: GCC 4.8+
- **Dependencies**: Standard C Library, math (-lm)

---

## Contributing

Issues and Pull Requests welcome.

## License

MIT License - Free to use, modify, and distribute.
