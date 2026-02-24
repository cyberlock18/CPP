# CPP (Módulos C++ de 42)

[English](README.md) | Español

Este repositorio contiene las soluciones para los **Módulos CPP** de la escuela 42 (CPP00 a CPP04). Cada módulo es un conjunto de ejercicios (`ex00`, `ex01`, ...), cada uno con su propio programa en C++ y un `Makefile` dedicado.

## Estructura del proyecto

```
CPP/
├── CPP00/   # Básicos: clases, funciones miembro, E/S
│   ├── ex00/   → binario: megaphone
│   ├── ex01/   → binario: phonebook
│   └── ex02/   → binario: account
├── CPP01/   # Memoria, referencias, punteros, flujos de archivo
│   ├── ex00/   → binario: zombie
│   ├── ex01/   → binario: zombieHorde
│   ├── ex02/   → binario: brain
│   ├── ex03/   → binario: violence
│   ├── ex04/   → binario: sed
│   └── ex05/   → binario: harl
├── CPP02/   # Sobrecarga de operadores, punto fijo, forma canónica
│   ├── ex00/   → binario: fixed
│   ├── ex01/   → binario: fixed
│   └── ex02/   → binario: fixed
├── CPP03/   # Herencia
│   ├── ex00/   → binario: claptrap
│   ├── ex01/   → binario: scavtrap
│   ├── ex02/   → binario: fragtrap
│   └── ex03/   → binario: diamondtrap
└── CPP04/   # Polimorfismo de subtipo, clases abstractas, interfaces
    ├── ex00/   → binario: animal
    ├── ex01/   → binario: brain
    └── ex02/   → binario: abstract
```

> La compilación y ejecución se realiza **por ejercicio** (dentro de `CPPXX/exYY/`). No existe un Makefile raíz.

## Compilar y ejecutar

Cada directorio de ejercicio expone los objetivos estándar del estilo 42: `all`, `clean`, `fclean`, `re`.
Todos los proyectos se compilan con `c++` y los flags `-Wall -Wextra -Werror -std=c++98`.

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

### Objetivos de utilidad (dentro del directorio de cualquier ejercicio)

```sh
make clean   # elimina los archivos objeto
make fclean  # elimina los archivos objeto y el binario
make re      # recompilación completa (fclean + all)
```

## Qué se aprende / Habilidades adquiridas

- Construir proyectos en C++98 con **Makefiles** (`all/clean/fclean/re`) usando flags de compilación estrictos (`-Wall -Wextra -Werror`).
- Escribir clases siguiendo la **Forma Canónica Ortodoxa** (constructor por defecto, constructor de copia, operador de asignación por copia, destructor).
- Comprender la gestión de memoria en **stack y heap**, aritmética de punteros y semántica de referencias.
- Implementar **sobrecarga de operadores** y aritmética de punto fijo para modelar tipos de valor numérico.
- Aplicar **herencia** (simple y múltiple/diamante) y comprender el orden de construcción y destrucción.
- Usar **funciones virtuales**, **funciones puramente virtuales** y **clases abstractas** para lograr polimorfismo en tiempo de ejecución.
- Practicar el diseño de interfaces y la separación entre contratos abstractos e implementaciones concretas.
- Gestionar recursos y tiempos de vida de objetos de forma segura en múltiples programas independientes.

## Autor

- ruortiz-
