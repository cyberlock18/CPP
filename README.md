# CPP (42 C++ Modules)

English | [Español](README.es.md)

This repository contains my solutions for the 42 School **CPP Modules** (CPP00 → CPP04). Each module is split into exercises (ex00, ex01, …), each one with its own small program and (in most cases) a dedicated `Makefile`.

## Contents

- `CPP00/` – basics, compilation, classes, member functions
- `CPP01/` – memory, references, pointers, file streams
- `CPP02/` – ad-hoc polymorphism, operator overloading, canonical form
- `CPP03/` – inheritance
- `CPP04/` – subtype polymorphism, abstract classes, interfaces

> Note: build/run is done per-exercise (inside `CPPXX/exYY/`).

## Build & Run (from the repo’s Makefiles)

Each exercise directory provides classic 42-style targets: `all`, `clean`, `fclean`, `re`.

### Example: CPP00/ex00

```sh
cd CPP00/ex00
make
./megaphone
```

### Example: CPP00/ex01

```sh
cd CPP00/ex01
make
./phonebook
```

### Example: CPP00/ex02

```sh
cd CPP00/ex02
make
./account
```

To clean:

```sh
make clean   # removes objects
make fclean  # removes objects + binary
make re      # full rebuild
```

## What you learn / skills acquired

- Building C++98 projects with **Makefiles** (`all/clean/fclean/re`) and strict flags (`-Wall -Wextra -Werror`).
- Writing classes following the **Orthodox Canonical Form** (constructors, copy, assignment, destructor).
- Understanding **stack vs heap**, references, pointers, and basic resource/lifetime management.
- Using **operator overloading** and const-correctness to model value types.
- Applying **inheritance** and **polymorphism** (virtual functions, abstract classes) across small programs.
- Practicing clean code structure across multiple independent exercises.

## Author

- ruortiz-