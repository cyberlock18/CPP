# CPP (42 C++ Modules)

English | [Español](README.es.md)

This repository contains solutions for the 42 School **CPP Modules** (CPP00 to CPP04). Each module is a set of exercises (`ex00`, `ex01`, ...), each with its own small C++ program and a dedicated `Makefile`.

## Project Structure

```
CPP/
├── CPP00/   # Basics: classes, member functions, I/O
│   ├── ex00/   → binary: megaphone
│   ├── ex01/   → binary: phonebook
│   └── ex02/   → binary: account
├── CPP01/   # Memory, references, pointers, file streams
│   ├── ex00/   → binary: zombie
│   ├── ex01/   → binary: zombieHorde
│   ├── ex02/   → binary: brain
│   ├── ex03/   → binary: violence
│   ├── ex04/   → binary: sed
│   └── ex05/   → binary: harl
├── CPP02/   # Operator overloading, fixed-point numbers, canonical form
│   ├── ex00/   → binary: fixed
│   ├── ex01/   → binary: fixed
│   └── ex02/   → binary: fixed
├── CPP03/   # Inheritance
│   ├── ex00/   → binary: claptrap
│   ├── ex01/   → binary: scavtrap
│   ├── ex02/   → binary: fragtrap
│   └── ex03/   → binary: diamondtrap
└── CPP04/   # Subtype polymorphism, abstract classes, interfaces
    ├── ex00/   → binary: animal
    ├── ex01/   → binary: brain
    └── ex02/   → binary: abstract
```

> Build and run is done **per-exercise** (inside `CPPXX/exYY/`). There is no top-level Makefile.

## Build & Run

Every exercise directory exposes the standard 42-style Makefile targets: `all`, `clean`, `fclean`, `re`.
All projects are compiled with `c++`, flags `-Wall -Wextra -Werror -std=c++98`.

### CPP00

```sh
cd CPP00/ex00 && make && ./megaphone
cd CPP00/ex01 && make && ./phonebook
cd CPP00/ex02 && make && ./account
```

### CPP01

```sh
cd CPP01/ex00 && make && ./zombie
cd CPP01/ex01 && make && ./zombieHorde
cd CPP01/ex02 && make && ./brain
cd CPP01/ex03 && make && ./violence
cd CPP01/ex04 && make && ./sed
cd CPP01/ex05 && make && ./harl
```

### CPP02

```sh
cd CPP02/ex00 && make && ./fixed
cd CPP02/ex01 && make && ./fixed
cd CPP02/ex02 && make && ./fixed
```

### CPP03

```sh
cd CPP03/ex00 && make && ./claptrap
cd CPP03/ex01 && make && ./scavtrap
cd CPP03/ex02 && make && ./fragtrap
cd CPP03/ex03 && make && ./diamondtrap
```

### CPP04

```sh
cd CPP04/ex00 && make && ./animal
cd CPP04/ex01 && make && ./brain
cd CPP04/ex02 && make && ./abstract
```

### Utility targets (run from inside any exercise directory)

```sh
make clean   # remove object files
make fclean  # remove object files and binary
make re      # full rebuild (fclean + all)
```

## What You Learn / Skills Acquired

- Building C++98 projects with **Makefiles** (`all/clean/fclean/re`) using strict compiler flags (`-Wall -Wextra -Werror`).
- Writing classes following the **Orthodox Canonical Form** (default constructor, copy constructor, copy-assignment operator, destructor).
- Understanding **stack vs heap** memory allocation, pointer arithmetic, and reference semantics.
- Implementing **operator overloading** and fixed-point arithmetic to model numeric value types.
- Applying **inheritance** (single and multiple/diamond) and understanding construction/destruction order.
- Using **virtual functions**, **pure virtual functions**, and **abstract classes** to achieve runtime polymorphism.
- Practicing interface design and the separation of abstract contracts from concrete implementations.
- Managing resources and object lifetimes safely across multiple independent programs.

## Author

- ruortiz-
