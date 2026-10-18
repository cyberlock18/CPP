# CPP06 — ex00: ScalarConverter (Castings y detección de tipos)

> Objetivo de este documento: que entiendas **por qué** cada línea está ahí,
> cómo se relacionan los ficheros entre sí y qué pasa exactamente con cada
> entrada. Al final hay trazas paso a paso con salidas reales del programa.

---

## Índice

1. [Qué pide el ejercicio](#1-qué-pide-el-ejercicio)
2. [Mapa de ficheros y cómo se relacionan](#2-mapa-de-ficheros-y-cómo-se-relacionan)
3. [La idea central: todo pasa por un `double`](#3-la-idea-central-todo-pasa-por-un-double)
4. [main.cpp línea a línea](#4-maincpp-línea-a-línea)
5. [ScalarConverter.hpp línea a línea](#5-scalarconverterhpp-línea-a-línea)
6. [ScalarConverter.cpp línea a línea](#6-scalarconvertercpp-línea-a-línea)
7. [Trazas completas con ejemplos](#7-trazas-completas-con-ejemplos)
8. [Tabla de pruebas (salida real)](#8-tabla-de-pruebas-salida-real)
9. [Protección contra overflow](#9-protección-contra-overflow)
10. [Limitaciones conocidas de esta implementación](#10-limitaciones-conocidas-de-esta-implementación)
11. [Los 4 castings de C++](#11-los-4-castings-de-c)
12. [Preguntas típicas de evaluación](#12-preguntas-típicas-de-evaluación)
13. [Resumen de conceptos](#13-resumen-de-conceptos)

---

## 1. Qué pide el ejercicio

Se crea un programa `convert` que recibe **un único argumento** (un string) que
representa un literal de C++ de uno de estos 4 tipos:

| Tipo     | Ejemplos de literal          |
|----------|------------------------------|
| `char`   | `'a'`, `'c'` (con comillas) o `a` sin ellas |
| `int`    | `0`, `-42`, `42`             |
| `float`  | `0.0f`, `-4.2f`, `4.2f`, `-inff`, `+inff`, `nanf` |
| `double` | `0.0`, `-4.2`, `4.2`, `-inf`, `+inf`, `nan`       |

El programa tiene que:

1. **Detectar** de qué tipo es el literal.
2. **Convertirlo** a su tipo real.
3. **Convertirlo explícitamente** a los otros tres tipos.
4. **Imprimir** los 4 resultados, avisando si una conversión es imposible o
   si el `char` no es imprimible.

Además el enunciado obliga a que `ScalarConverter` **no sea instanciable**
(solo tiene un método estático).

### Lo que dice exactamente el PDF (y cómo se cumple)

| Frase del enunciado | Qué implica | Dónde se cumple |
|---|---|---|
| *"a string representation of a C++ literal in its most common form"* | Un char se escribe `'a'` (con comillas) | `detectType`: caso `'x'` de longitud 3 |
| *"Examples of char literals: 'c', 'a'"* | Aceptar `'c'` | ídem; también se acepta `c` sin comillas porque la shell suele quitar las comillas si no se escapan |
| *"non-displayable characters shouldn't be used as inputs"* | No hace falta aceptar caracteres de control como entrada | — |
| *"If a conversion to char is not displayable, print an informative message"* | `Non displayable` | `printChar` |
| *"Except for char parameters, only the decimal notation will be used"* | **No** hace falta soportar `1e5`, `0x2A`... | Se rechazan como `INVALID` |
| *"handle these pseudo-literals: -inff, +inff, nanf / -inf, +inf, nan"* | Casos especiales | `SPECIAL` en `detectType` |
| *"If a conversion does not make any sense or overflows, display ... impossible"* | Entradas sin sentido (`hola`) y overflow → `impossible` | `INVALID` + comprobaciones de rango (sección 9) |
| *"Include any header you need in order to handle numeric limits"* | Usar `INT_MAX`, `FLT_MAX`... | `<climits>`, `<cfloat>` |
| *"convert it explicitly to the three other data types"* | Cast explícito | `static_cast` en todas las conversiones |
| *"Additional Rule: ... a specific type of casting. Your choice will be reviewed"* | Justificar el cast | ex00 → `static_cast` (sección 11) |

Los tres ejemplos del PDF (`0`, `nan`, `42.0f`) producen exactamente la salida
que muestra el enunciado (ver sección 8).

---

## 2. Mapa de ficheros y cómo se relacionan

```
 ┌────────────┐   #include    ┌───────────────────────┐
 │ main.cpp   │ ────────────► │ ScalarConverter.hpp   │  (declara la clase)
 └─────┬──────┘               └───────────▲───────────┘
       │ llama a                          │ #include
       │ ScalarConverter::convert(argv[1])│
       ▼                                  │
 ┌──────────────────────────────────────────────────────┐
 │ ScalarConverter.cpp                                  │
 │                                                      │
 │  convert() ──► detectType()   (¿qué tipo es?)        │
 │      │                                               │
 │      ├─► caso INVALID: todo "impossible" y return    │
 │      │                                               │
 │      ├─► caso SPECIAL: imprime nan/inf y return      │
 │      │                                               │
 │      ├─► obtiene un double `val`                     │
 │      │      (el carácter si es CHAR, strtod si no)   │
 │      │                                               │
 │      ├─► si strtod desbordó (ERANGE): todo           │
 │      │   "impossible" y return                       │
 │      │                                               │
 │      └─► printChar(val)                              │
 │          printInt(val)                               │
 │          printFloat(val)                             │
 │          printDouble(val)                            │
 │              └─► printNumber() da formato a ambos    │
 └──────────────────────────────────────────────────────┘
```

- `main.cpp` **solo** valida que haya 1 argumento y delega.
- `ScalarConverter.hpp` es el "contrato": dice qué se puede usar desde fuera
  (`convert`) y prohíbe crear objetos (constructores privados).
- `ScalarConverter.cpp` contiene toda la lógica. Las funciones auxiliares
  (`detectType`, `printChar`…) son `static` a nivel de fichero: **nadie fuera
  de este .cpp puede verlas**. Son "privadas del fichero".
- El `Makefile` compila `main.cpp` y `ScalarConverter.cpp` por separado a `.o`
  y luego los enlaza en el ejecutable `convert`.

---

## 3. La idea central: todo pasa por un `double`

Esta es la clave para entender el diseño. En lugar de escribir 4×4 = 16
conversiones (char→int, char→float, int→char, ...), el código hace:

```
literal (string)  ──►  double val  ──►  char
                                  ──►  int
                                  ──►  float
                                  ──►  double
```

¿Por qué `double`? Porque es el tipo "más grande" de los cuatro:

- Cualquier `char` cabe exactamente en un `double` (`'a'` → `97.0`).
- Cualquier `int` de 32 bits cabe exactamente en un `double` (tiene 53 bits de
  precisión en la mantisa).
- Cualquier `float` cabe exactamente en un `double`.
- Y `double` puede representar `nan` e `inf`.

Así que se pasa **una vez** del string al `double`, y desde ahí se hace un
`static_cast` a cada tipo, comprobando antes si cabe.

---

## 4. main.cpp línea a línea

```cpp
1  #include "ScalarConverter.hpp"
2  #include <iostream>
3
4  int main(int argc, char** argv)
5  {
6      if (argc != 2)
7      {
8          std::cerr << "Usage: ./convert <literal>" << std::endl;
9          return 1;
10     }
11     ScalarConverter::convert(argv[1]);
12     return 0;
13 }
```

| Línea | Qué hace | Por qué |
|-------|----------|---------|
| 1 | Incluye la declaración de la clase | Para que el compilador sepa que existe `ScalarConverter::convert` |
| 2 | Incluye `<iostream>` | Para usar `std::cerr` |
| 4 | `argc` = nº de argumentos, `argv` = array de strings C | `argv[0]` es el nombre del programa (`./convert`), `argv[1]` el literal |
| 6 | Si no hay exactamente 1 literal → error | `./convert` (argc=1) o `./convert 1 2` (argc=3) son incorrectos |
| 8 | Mensaje a `std::cerr` | Los errores van a la salida de error, no a la estándar |
| 9 | `return 1` | Código de salida ≠ 0 indica error a la shell |
| 11 | Llama al método **estático** | Fíjate: `ScalarConverter::convert`, sin objeto. `argv[1]` es un `char*` y se convierte automáticamente a `std::string` porque `convert` recibe `std::string const&` |

**Ejemplo:**
```
$ ./convert 42      → argc = 2, argv[1] = "42"    → convierte
$ ./convert         → argc = 1                     → "Usage: ..."
```

---

## 5. ScalarConverter.hpp línea a línea

```cpp
1  #ifndef SCALARCONVERTER_HPP
2  #define SCALARCONVERTER_HPP
3
4  #include <string>
5
6  class ScalarConverter
7  {
8  public:
9      static void convert(std::string const& literal);
10
11 private:
12     ScalarConverter();
13     ScalarConverter(ScalarConverter const&);
14     ScalarConverter& operator=(ScalarConverter const&);
15     ~ScalarConverter();
16 };
17
18 #endif
```

| Línea | Qué hace |
|-------|----------|
| 1-2, 18 | **Include guard**: si el header se incluye dos veces, la segunda vez se salta todo. Evita "redefinition of class". |
| 4 | Necesitamos `std::string` para la firma de `convert`. |
| 9 | `static` → el método pertenece a la **clase**, no a un objeto. Se llama `ScalarConverter::convert(...)`. No tiene `this`. |
| 9 | `std::string const&` → se pasa por **referencia constante**: no se copia el string y no se puede modificar. |
| 11-15 | La **forma canónica ortodoxa** (constructor, copia, asignación, destructor) pero en `private`. |

### ¿Por qué los constructores son privados?

```cpp
ScalarConverter sc;        // ERROR de compilación: constructor privado
ScalarConverter::convert("42");   // OK: método estático, no necesita objeto
```

La clase no guarda ningún dato (no tiene atributos), así que crear un objeto
no tendría sentido. Hacer privado el constructor es la forma de C++98 de
decir "esta clase es solo un contenedor de funciones".

> Nota: en C++98 no existe `= delete`. Por eso se declaran en `private` y
> **no se definen** (no hay cuerpo en el .cpp). Si alguien intentara usarlos
> desde dentro de la clase, fallaría en el enlazado.

---

## 6. ScalarConverter.cpp línea a línea

### 6.1 Includes

```cpp
#include "ScalarConverter.hpp"
#include <iostream>   // std::cout
#include <iomanip>    // std::setprecision, std::fixed
#include <string>     // std::string
#include <cstdlib>    // std::strtod
#include <climits>    // INT_MAX, INT_MIN
#include <cfloat>     // FLT_MAX
#include <cerrno>     // errno, ERANGE
#include <cmath>      // std::isnan, std::isinf, std::floor, std::fabs
```

`std::isdigit` y `std::isprint` vienen de `<cctype>`; aquí llegan incluidos
indirectamente por otros headers.

---

### 6.2 El enum `LiteralType`

```cpp
11  enum LiteralType { CHAR, INT, FLOAT, DOUBLE, SPECIAL, INVALID };
```

Un `enum` asigna nombres a enteros: `CHAR = 0`, `INT = 1`, `FLOAT = 2`,
`DOUBLE = 3`, `SPECIAL = 4`, `INVALID = 5`. Sirve para que `detectType`
devuelva algo legible en vez de un número mágico:

```cpp
if (type == INVALID)   // se entiende
if (type == 5)         // ¿qué es 5?
```

`INVALID` representa "esto no es ningún literal válido" (`hola`, `1e5`,
`42..0`...). El PDF pide que en ese caso se informe de que la conversión es
imposible.

---

### 6.3 `detectType` — el detector de tipos

```cpp
13  static LiteralType detectType(std::string const& s)
14  {
15      if (s.empty())
16          return INVALID;
17
18      if (s == "nan" || s == "+inf" || s == "-inf" ||
19          s == "nanf" || s == "+inff" || s == "-inff")
20          return SPECIAL;
21
22      if (s.length() == 3 && s[0] == '\'' && s[2] == '\'')
23          return CHAR;
24      if (s.length() == 1 && !std::isdigit(s[0]))
25          return CHAR;
26
27      int    digits = 0;
28      int    dots   = 0;
29      bool   hasF   = false;
30      size_t start  = (s[0] == '-' || s[0] == '+') ? 1 : 0;
31
32      for (size_t i = start; i < s.length(); i++)
33      {
34          if (std::isdigit(s[i]))                        digits++;
35          else if (s[i] == '.')                          dots++;
36          else if (s[i] == 'f' && i == s.length() - 1)   hasF = true;
37          else                                           return INVALID;
38      }
39
40      if (digits == 0 || dots > 1 || (hasF && dots == 0))
41          return INVALID;
42      if (hasF)  return FLOAT;
43      if (dots)  return DOUBLE;
44      return INT;
45  }
```

**Línea 13 — `static` delante de una función libre.** Aquí `static` NO
significa lo mismo que en la clase: significa "esta función solo es visible
dentro de este .cpp" (*internal linkage*). Así no contamina el espacio global.

**Líneas 15-16 — String vacío.** `./convert ""` es un argumento válido para
la shell pero no es ningún literal → `INVALID`. Se comprueba primero porque
el resto del código lee `s[0]`.

**Líneas 18-20 — Pseudoliterales.** Se comprueban **antes** de validar números
porque `nan` o `inff` contienen letras y el bucle los rechazaría como `INVALID`.
El orden de las comprobaciones importa.

**Líneas 22-23 — Char con comillas: `'a'`.** Es la forma "más común" de un
literal char en C++, que es lo que pide el PDF. Condiciones:
- longitud exactamente 3,
- primer carácter `'`,
- último carácter `'`.

El carácter de en medio puede ser cualquiera, incluso un dígito: `'7'` es el
**char** 7 (código 55), no el int 7.

> En la shell hay que proteger las comillas para que lleguen al programa:
> `./convert "'a'"`. Si escribes `./convert 'a'`, la shell quita las comillas y
> el programa recibe solo `a`.

**Líneas 24-25 — Char sin comillas: `a`.** Si el string mide 1 y **no** es un
dígito, también se trata como carácter (es lo que llega cuando la shell quitó
las comillas). ¿Por qué excluir los dígitos? Porque `./convert 7` debe ser el
**int 7**, no el carácter `'7'`.

**Líneas 27-30 — Contadores.**
- `digits`: cuántos dígitos hay. Un número necesita **al menos uno** (`.` solo
  no es un número, `-` solo tampoco).
- `dots`: cuántos puntos. Un número válido tiene **0 o 1**.
- `hasF`: si termina en `f`.
- `start`: ternario `condición ? si_true : si_false`. Si hay signo, se empieza
  a recorrer desde la posición 1 para no tomar el signo como carácter inválido.

```
"-42"   → start = 1   → se recorre "42"
"42"    → start = 0   → se recorre "42"
```

**Líneas 32-38 — El recorrido.** Cada carácter tiene que ser una de estas tres
cosas; si no, el literal no es válido:

| Si el carácter es... | Acción |
|---|---|
| un dígito | `digits++` |
| `.` | `dots++` |
| `f` **y es el último** | `hasF = true` |
| cualquier otra cosa (`e`, `x`, letras, espacios, una `f` en medio...) | `return INVALID` |

**Líneas 40-41 — Validación final.** Aunque todos los caracteres sean
"permitidos", la combinación puede no tener sentido:

| Condición | Ejemplo rechazado | Por qué |
|---|---|---|
| `digits == 0` | `.`, `+.`, `.f` | No hay número |
| `dots > 1` | `42..0`, `1.2.3` | Dos puntos decimales |
| `hasF && dots == 0` | `42f` | En C++ `42f` no es un literal válido: un float necesita punto (`42.0f` o `42.f`) |

**Líneas 42-44 — Decisión**, ya sabiendo que el número es válido:

| hasF  | dots | Tipo     | Ejemplos              |
|-------|------|----------|-----------------------|
| true  | 1    | `FLOAT`  | `4.2f`, `4.f`, `.5f`  |
| false | 1    | `DOUBLE` | `4.2`, `5.`, `.5`     |
| false | 0    | `INT`    | `42`, `-42`, `+7`     |

`if (dots)` funciona porque en C++ un entero distinto de 0 es `true`.

#### Mini traza de `detectType("-4.2f")`

```
s = "-4.2f"   longitud 5
línea 15: no está vacío
línea 18: no es especial
línea 22: longitud ≠ 3
línea 24: longitud ≠ 1
línea 30: s[0] = '-'  → start = 1
i=1 '4' → dígito      → digits=1
i=2 '.' → punto       → dots=1
i=3 '2' → dígito      → digits=2
i=4 'f' → 'f' y es el último → hasF=true
línea 40: digits=2 ✔, dots=1 ✔, hasF con punto ✔
línea 42: hasF → return FLOAT
```

#### Mini traza de `detectType("'a'")`

```
línea 22: longitud 3, s[0]='\'', s[2]='\'' → return CHAR
```

#### Mini traza de `detectType("hola")`

```
línea 22: longitud 4 ≠ 3
línea 24: longitud 4 ≠ 1
start = 0
i=0 'h' → no es dígito, ni '.', ni 'f' final → return INVALID
```

#### Mini traza de `detectType("1e5")`

```
i=0 '1' → dígito
i=1 'e' → no es dígito, ni '.', ni 'f' → return INVALID
```

(el PDF dice que solo se usará notación decimal, así que `1e5` no es una
entrada que haya que convertir)

---

### 6.4 `printChar`

```cpp
47 static void printChar(double val)
48 {
49     if (val < 0 || val > 127 || std::isnan(val) || std::isinf(val))
50         std::cout << "char: impossible" << std::endl;
51     else if (!std::isprint(static_cast<int>(val)))
52         std::cout << "char: Non displayable" << std::endl;
53     else
54         std::cout << "char: '" << static_cast<char>(val) << "'" << std::endl;
55 }
```

Tres niveles de comprobación, del más grave al menos:

1. **Línea 49 — ¿Cabe en un char ASCII?** Un `char` ASCII va de 0 a 127.
   Fuera de ese rango (o si es nan/inf) → `impossible`.
2. **Línea 51 — ¿Se puede ver?** Los códigos 0-31 y 127 son caracteres de
   control (salto de línea, tabulador, `DEL`...). Existen, pero no se pueden
   imprimir → `Non displayable`. Se hace `static_cast<int>` porque `isprint`
   recibe un `int`.
3. **Línea 54 — Imprimir.** `static_cast<char>(val)` convierte `42.0` en el
   carácter con código 42, que es `'*'`.

```
val = 42.0   → 0..127 ✔, isprint(42) ✔  → char: '*'
val = 9.0    → 0..127 ✔, isprint(9) ✘   → char: Non displayable  (es un TAB)
val = 200.0  → > 127                     → char: impossible
val = -42.0  → < 0                       → char: impossible
val = 4.2    → 0..127 ✔, isprint(4) ✘   → char: Non displayable  (se trunca a 4)
```

---

### 6.5 `printInt`

```cpp
57 static void printInt(double val)
58 {
59     if (std::isnan(val) || std::isinf(val) ||
60         val > static_cast<double>(INT_MAX) || val < static_cast<double>(INT_MIN))
61         std::cout << "int: impossible" << std::endl;
62     else
63         std::cout << "int: " << static_cast<int>(val) << std::endl;
64 }
```

- `INT_MAX` = 2147483647, `INT_MIN` = -2147483648 (de `<climits>`).
- **¿Por qué comprobar antes de castear?** Convertir a `int` un `double` que no
  cabe es **comportamiento indefinido** en C++: podría dar cualquier valor.
  Por eso primero se mira si está dentro del rango.
- `static_cast<double>(INT_MAX)` convierte el límite a double para comparar
  double con double (evita conversiones implícitas confusas).
- `static_cast<int>(4.7)` **trunca** hacia cero → `4`. `static_cast<int>(-4.7)` → `-4`.

```
val = 42.0          → int: 42
val = 4.2           → int: 4
val = 2147483648.0  → > INT_MAX → int: impossible
```

---

### 6.6 `printNumber` — el formateador común

```cpp
66 static void printNumber(double n)
67 {
68     if (n == std::floor(n))
69     {
70         std::cout << std::fixed << std::setprecision(1) << n;
71         std::cout.unsetf(std::ios::fixed);
72         std::cout << std::setprecision(6);
73     }
74     else
75         std::cout << n;
76 }
```

`printFloat` y `printDouble` tienen que hacer lo mismo (decidir si el número
tiene decimales y darle formato), así que esa lógica vive en una sola función.

- **Línea 68 — ¿Tiene decimales?** `std::floor(n)` redondea hacia abajo al
  entero más cercano **pero sigue siendo un `double`**. Si `n` es igual a su
  `floor`, no tiene parte decimal.
  - `floor(42.0) = 42.0` → iguales → entero.
  - `floor(4.2)  = 4.0`  → distintos → tiene decimales.

  > Antes se usaba `n == static_cast<long>(n)`. Eso es **comportamiento
  > indefinido** si `n` no cabe en un `long` (por ejemplo `1e20` o `inf`).
  > `std::floor` trabaja siempre con `double`, así que no puede desbordarse.
  > Ver [sección 9](#9-protección-contra-overflow).

- **Línea 70 — Formato fijo con 1 decimal.** Por defecto `std::cout` imprime
  `42.0` como `42` y `1234567.0` como `1.23457e+06`. Con `std::fixed`
  (notación normal, nunca científica) y `std::setprecision(1)` (1 decimal),
  sale `42.0` y `1234567.0`.
- **Líneas 71-72 — Restaurar `cout`.** `std::fixed` y `setprecision` son
  **persistentes**: se quedan activos para todo lo que se imprima después. Si
  no se deshacen, el siguiente número con decimales (`4.25`) saldría como `4.2`.
  `unsetf(std::ios::fixed)` quita el modo fijo y `setprecision(6)` vuelve al
  valor por defecto.
- **Línea 75 — Con decimales.** Se imprime tal cual (`4.2`, `0.1`).

---

### 6.7 `printFloat` y `printDouble`

```cpp
78 static void printFloat(double val)
79 {
80     if (std::fabs(val) > FLT_MAX)
81     {
82         std::cout << "float: impossible" << std::endl;
83         return;
84     }
85     float f = static_cast<float>(val);
86     std::cout << "float: ";
87     printNumber(f);
88     std::cout << "f" << std::endl;
89 }
90
91 static void printDouble(double val)
92 {
93     std::cout << "double: ";
94     printNumber(val);
95     std::cout << std::endl;
96 }
```

- **Línea 80 — ¿Cabe en un float?** `FLT_MAX` (≈ 3.4·10³⁸) es el mayor `float`
  finito. `std::fabs` es el valor absoluto, así se cubren positivos y negativos
  con una sola comparación. Es la misma idea que `INT_MAX` en `printInt`:
  **comprobar antes de convertir**.
- **Línea 85** — Ya sabemos que cabe → `static_cast<float>` seguro. Puede
  perder precisión (un float tiene ~7 cifras significativas), pero no desborda.
- **Línea 87** — `f` (float) se pasa a `printNumber`, que recibe `double`:
  conversión implícita float→double, siempre exacta.
- **Línea 88** — Se añade la `f` final del literal float.
- `printDouble` no necesita comprobar rango: `val` ya **es** un `double`.

```
val = 42.0   → cabe → printNumber(42.0)  → "42.0"  + "f" → float: 42.0f
val = 4.2    → cabe → printNumber(4.2f)  → "4.2"   + "f" → float: 4.2f
val = 1e39   → 1e39 > FLT_MAX            →                  float: impossible
```

---

### 6.8 `ScalarConverter::convert` — el director de orquesta

```cpp
98  void ScalarConverter::convert(std::string const& literal)
99  {
100     LiteralType type = detectType(literal);
101     double val = 0.0;
102
103     if (type == INVALID)
104     {
105         std::cout << "char: impossible" << std::endl;
106         std::cout << "int: impossible" << std::endl;
107         std::cout << "float: impossible" << std::endl;
108         std::cout << "double: impossible" << std::endl;
109         return;
110     }
111
112     if (type == SPECIAL)
113     {
114         if (literal == "nanf" || literal == "nan")
115         {
116             std::cout << "char: impossible" << std::endl;
117             std::cout << "int: impossible" << std::endl;
118             std::cout << "float: nanf" << std::endl;
119             std::cout << "double: nan" << std::endl;
120         }
121         else
122         {
123             std::string sign = (literal[0] == '-') ? "-" : "+";
124             std::cout << "char: impossible" << std::endl;
125             std::cout << "int: impossible" << std::endl;
126             std::cout << "float: " << sign << "inff" << std::endl;
127             std::cout << "double: " << sign << "inf" << std::endl;
128         }
129         return;
130     }
131
132     if (type == CHAR)
133     {
134         char c = (literal.length() == 3) ? literal[1] : literal[0];
135         val = static_cast<double>(c);
136     }
137     else
138     {
139         char* end;
140         errno = 0;
141         val = std::strtod(literal.c_str(), &end);
142         if (errno == ERANGE && std::isinf(val))
143         {
144             std::cout << "char: impossible" << std::endl;
145             std::cout << "int: impossible" << std::endl;
146             std::cout << "float: impossible" << std::endl;
147             std::cout << "double: impossible" << std::endl;
148             return;
149         }
150     }
151
152     printChar(val);
153     printInt(val);
154     printFloat(val);
155     printDouble(val);
156 }
```

| Línea | Qué hace |
|-------|----------|
| 98 | En el .cpp **no** se repite `static`; se usa `ScalarConverter::` para indicar a qué clase pertenece. |
| 100 | Paso 1: detectar el tipo. |
| 101 | Variable puente donde se guardará el valor convertido. |
| 103-110 | **Caso INVALID** (`hola`, `1e5`, `42..0`, `""`...): la entrada no tiene sentido como literal → el PDF pide informar de que la conversión es imposible. `return` para no seguir. |
| 112-130 | **Caso especial**: nan/inf. Se imprime a mano y `return`. |
| 114 | `nan` y `nanf` dan la misma salida: no hay "signo" para NaN. |
| 123 | Para `inf`, se mira el primer carácter para saber el signo. Así `-inff` y `-inf` dan `-inff` / `-inf`. |
| 132-136 | **Caso CHAR**: se elige el carácter. Si el literal mide 3 es `'a'` → el carácter está en la posición 1 (entre las comillas). Si mide 1 es `a` → posición 0. Luego `static_cast<double>` lo convierte a su código ASCII: `'a'` → `97.0`. |
| 139 | Puntero que `strtod` rellenará con "dónde se paró". |
| 140 | `errno = 0`: se resetea **antes** de llamar a `strtod`. `errno` es una variable global que las funciones de C ponen a un código de error cuando algo falla, pero **nunca la vuelven a poner a 0** al ir bien. Si no se resetea, podría quedar un error antiguo. |
| 141 | `literal.c_str()` convierte el `std::string` a `const char*` porque `strtod` es una función de C. |
| 142 | Si el número no cabe ni en un `double`, `strtod` devuelve `±HUGE_VAL` (= `inf`) y pone `errno = ERANGE` ("resultado fuera de rango"). Se exige también `isinf(val)` porque `ERANGE` también se activa en *underflow* (números diminutos), que no es un problema: el resultado es `0` o casi `0` y se imprime sin más. |
| 143-148 | Overflow total: ningún tipo puede representar el valor → todo `impossible`. Ojo: esto es distinto de escribir `inf` a mano, que sí es un valor válido (caso SPECIAL). |
| 152-155 | Paso final: desde el `double` se imprimen los 4 tipos. |

#### Cómo funciona `strtod`

```
strtod("4.2f", &end)

  "4.2f"
   ^^^      ← lee esto como número: 4.2
      ^     ← end apunta aquí ('f'), primer carácter que no entendió
```

`strtod` acepta tanto `"4.2"` como `"4.2f"` (se para en la `f`), por eso
`FLOAT`, `DOUBLE` e `INT` comparten el mismo camino.

---

## 7. Trazas completas con ejemplos

### Ejemplo A: `./convert 42`

```
main: argc=2 → convert("42")
detectType("42"):
    no vacío, no especial, longitud 2, start=0
    '4' dígito, '2' dígito → digits=2, dots=0, hasF=false → INT
convert: type=INT → rama else → strtod("42") = 42.0
printChar(42.0):  0..127 ✔, isprint(42) ✔ → char: '*'
printInt(42.0):   en rango               → int: 42
printFloat(42.0): cabe, floor(42)=42      → float: 42.0f
printDouble(42.0): floor(42)=42          → double: 42.0
```

### Ejemplo B: `./convert "'a'"` y `./convert a`

```
detectType("'a'"): longitud 3 entre comillas → CHAR
convert: longitud 3 → c = literal[1] = 'a'
         val = static_cast<double>('a') = 97.0
char: 'a'   int: 97   float: 97.0f   double: 97.0

detectType("a"): longitud 1 y no es dígito → CHAR
convert: longitud 1 → c = literal[0] = 'a'   → misma salida
```

### Ejemplo B2: `./convert hola`

```
detectType("hola") → INVALID   ('h' no es dígito, ni '.', ni 'f' final)
convert: type == INVALID → todo impossible, return
char: impossible   int: impossible   float: impossible   double: impossible
```

### Ejemplo C: `./convert -4.2f`

```
detectType → FLOAT (ver mini traza en 6.3)
strtod("-4.2f") = -4.2   (se para en 'f')
printChar(-4.2):  < 0                    → char: impossible
printInt(-4.2):   trunca                 → int: -4
printFloat(-4.2): cabe, floor(-4.2)=-5 ≠  → float: -4.2f
printDouble:                             → double: -4.2
```

### Ejemplo D: `./convert +inff`

```
detectType → SPECIAL
convert: no es nan → sign = '+'
char: impossible   int: impossible   float: +inff   double: +inf
return
```

---

## 8. Tabla de pruebas (salida real)

Salida obtenida ejecutando el binario compilado:

| Entrada      | char               | int        | float           | double         |
|--------------|--------------------|------------|-----------------|----------------|
| `0`          | Non displayable    | 0          | 0.0f            | 0.0            |
| `42`         | `'*'`              | 42         | 42.0f           | 42.0           |
| `-42`        | impossible         | -42        | -42.0f          | -42.0          |
| `'a'`        | `'a'`              | 97         | 97.0f           | 97.0           |
| `'*'`        | `'*'`              | 42         | 42.0f           | 42.0           |
| `a`          | `'a'`              | 97         | 97.0f           | 97.0           |
| `hola`, `abc`| impossible         | impossible | impossible      | impossible     |
| `1e5`, `42f`, `42..0`, `""` | impossible | impossible | impossible | impossible |
| `.5`         | Non displayable    | 0          | 0.5f            | 0.5            |
| `42.0f`      | `'*'`              | 42         | 42.0f           | 42.0           |
| `4.2`        | Non displayable    | 4          | 4.2f            | 4.2            |
| `-4.2f`      | impossible         | -4         | -4.2f           | -4.2           |
| `127`        | Non displayable    | 127        | 127.0f          | 127.0          |
| `128`        | impossible         | 128        | 128.0f          | 128.0          |
| `nan`        | impossible         | impossible | nanf            | nan            |
| `+inff`      | impossible         | impossible | +inff           | +inf           |
| `-inf`       | impossible         | impossible | -inff           | -inf           |
| `2147483647` | impossible         | 2147483647 | 2147483648.0f   | 2147483647.0   |
| `2147483648` | impossible         | impossible | 2147483648.0f   | 2147483648.0   |
| `-2147483649`| impossible         | impossible | -2147483648.0f  | -2147483649.0  |
| `1234567.0`  | impossible         | 1234567    | 1234567.0f      | 1234567.0      |
| `99999999999999999999` | impossible | impossible | 100000002004087734272.0f | 100000000000000000000.0 |
| `1` + 39 ceros + `.0` | impossible | impossible | impossible | 999999999999999939709166371603178586112.0 |
| 400 nueves   | impossible         | impossible | impossible      | impossible     |

> ¿Por qué `2147483647` da `2147483648.0f`? No es un error: un `float` solo
> tiene 24 bits de mantisa (~7 cifras), y el `float` más cercano a
> 2147483647 es exactamente 2147483648. Igual con `99999999999999999999`: el
> `float` más cercano es `100000002004087734272`. El `double` (53 bits) sí es
> exacto hasta mucho más lejos.

---

## 9. Protección contra overflow

Regla general: **convertir un número de coma flotante a un tipo donde no cabe
es comportamiento indefinido**. El programa puede imprimir cualquier cosa,
o funcionar "por casualidad" en tu ordenador y fallar en otro. Por eso cada
conversión se protege **antes** de hacerse:

| Paso | Riesgo | Protección | Dónde |
|------|--------|------------|-------|
| string → double | El número es tan grande que ni un `double` lo representa | `strtod` no desborda: devuelve `inf` y pone `errno = ERANGE`; se detecta y se imprime todo `impossible` | `convert`, línea 142 |
| double → char | Fuera de 0..127, nan, inf | `val < 0 \|\| val > 127 \|\| isnan \|\| isinf` | `printChar`, línea 49 |
| double → int | Fuera de `INT_MIN..INT_MAX`, nan, inf | Comparación con `INT_MAX` / `INT_MIN` | `printInt`, líneas 59-60 |
| double → float | `|val| > FLT_MAX` | `std::fabs(val) > FLT_MAX` | `printFloat`, línea 80 |
| "¿tiene decimales?" | Antes: `static_cast<long>(n)` desbordaba con valores > 9.2·10¹⁸ | Ahora: `n == std::floor(n)`, sin cast | `printNumber`, línea 68 |

### Cómo se comprobó

Compilando con el detector de comportamiento indefinido de GCC:

```
c++ -std=c++98 -fsanitize=undefined,float-cast-overflow main.cpp ScalarConverter.cpp
```

**Antes del arreglo:**
```
$ ./convert 99999999999999999999
ScalarConverter.cpp:59:29: runtime error: 1e+20 is outside the range of representable values of type 'long int'
ScalarConverter.cpp:67:31: runtime error: 1e+20 is outside the range of representable values of type 'long int'
```

**Después del arreglo:** ningún aviso con todas las entradas de la tabla de
la sección 8.

### ¿Por qué `inf` escrito a mano no es "impossible" en float/double?

```
./convert +inf         →  float: +inff    double: +inf      (valor válido)
./convert 999...999    →  float: impossible  double: impossible  (overflow)
```

`+inf` es un valor que existe en `float` y `double` (IEEE 754), así que se
puede representar. En cambio, 400 nueves es un número **finito** concreto que
no cabe: convertirlo a `inf` sería dar un resultado falso.

---

## 10. Limitaciones conocidas de esta implementación

| Entrada        | Salida actual            | Qué pasa |
|----------------|--------------------------|----------|
| `2147483647.5` | `double: 2.14748e+09`    | Los números **con decimales** se imprimen con la precisión por defecto de `cout` (6 cifras significativas), así que los grandes salen en notación científica. Solo afecta al formato, no es overflow. |
| `1234.5678`    | `1234.57`                | Mismo motivo: se redondea a 6 cifras. |
| `2147483647.5` | `int: impossible`        | Decisión de diseño: el valor es mayor que `INT_MAX`, aunque truncado cabría. El PDF no lo especifica; las dos opciones son defendibles. |
| `-`, `.`, `*`  | char `'-'` / `'.'` / `'*'` | Un único carácter no dígito se trata como char (es lo que llega si la shell quita las comillas de `'-'`). |

---

## 11. Los 4 castings de C++

| Cast | Cuándo se comprueba | Uso típico | Ejercicio |
|------|---------------------|------------|-----------|
| `static_cast<T>(x)`      | En compilación | Conversiones numéricas (double→int), upcasts | **ex00** |
| `reinterpret_cast<T>(x)` | Nada           | Puntero ↔ entero, puntero ↔ puntero de otro tipo | **ex01** |
| `dynamic_cast<T>(x)`     | En ejecución   | Downcast seguro en clases polimórficas | **ex02** |
| `const_cast<T>(x)`       | En compilación | Quitar/añadir `const` | — |

**¿Por qué no usar el cast de C `(int)val`?** Porque el cast de C prueba
todos los anteriores hasta que uno funcione, y no deja claro qué intención
tienes. `static_cast` es explícito y el compilador te avisa si la conversión
no tiene sentido (por ejemplo, `static_cast<int*>(3.5)` no compila).

Ejemplo de `static_cast` en este ejercicio:

```cpp
static_cast<char>(42.0)    // '*'
static_cast<int>(4.7)      // 4  (trunca)
static_cast<float>(0.1)    // 0.1f (pierde algo de precisión)
static_cast<double>('a')   // 97.0
```

---

## 12. Preguntas típicas de evaluación

- **¿Por qué la clase no se puede instanciar?** Porque los constructores son
  privados; no guarda estado, solo ofrece `convert`.
- **¿Qué cast has usado y por qué?** `static_cast`, porque son conversiones
  entre tipos numéricos relacionados, comprobables en compilación.
- **¿Por qué pasas por `double`?** Es el tipo que puede representar todos los
  valores de los otros tres sin pérdida, además de nan/inf.
- **¿Qué pasa si conviertes a int un double fuera de rango?** Es
  comportamiento indefinido, por eso se comprueba con `INT_MAX`/`INT_MIN` antes.
- **¿Diferencia entre "impossible" y "Non displayable"?** Impossible: el valor
  no cabe en el tipo. Non displayable: cabe, pero es un carácter de control.
- **¿Qué significa `static` en `detectType`?** Que la función solo existe
  dentro de ese .cpp (enlace interno).
- **¿Está protegido contra overflow?** Sí: cada conversión comprueba el rango
  antes del cast (`INT_MAX`, `FLT_MAX`, 0..127), el overflow de `strtod` se
  detecta con `errno == ERANGE`, y para saber si hay decimales se usa
  `std::floor` en lugar de un cast a `long`.
- **¿Por qué resetear `errno` a 0?** Porque las funciones de C solo lo
  escriben cuando hay error; nunca lo limpian.
- **¿Qué pasa con `./convert hola`?** No es ningún literal → `INVALID` → todo
  `impossible`, como pide el PDF ("does not make any sense").
- **¿Y con `'a'`?** Es la forma estándar de un literal char en C++. Se detecta
  por longitud 3 y comillas en los extremos, y se toma el carácter del medio.
- **¿Por qué no aceptas `1e5`?** El PDF dice que, salvo los char, solo se
  usará notación decimal.
- **¿Por qué `42f` es inválido?** Porque en C++ un literal float necesita punto:
  `42.0f` o `42.f`.

---

## 13. Resumen de conceptos

| Concepto | Qué es |
|----------|--------|
| **Método static** | Pertenece a la clase, se llama sin objeto: `Clase::metodo()` |
| **Constructor privado** | Impide crear objetos de la clase |
| **Función libre static** | Solo visible dentro de su fichero .cpp |
| **enum** | Lista de constantes enteras con nombre legible |
| **static_cast** | Casting seguro entre tipos relacionados, comprobado en compilación |
| **strtod** | Convierte string a double e indica dónde paró |
| **c_str()** | Obtiene un `const char*` desde un `std::string` |
| **INT_MAX / INT_MIN** | Límites del tipo `int` |
| **isnan / isinf** | Detectan NaN e infinito |
| **isprint / isdigit** | Comprueban si un char es imprimible / es un dígito |
| **Truncar** | Quitar la parte decimal hacia cero: 4.7 → 4, -4.7 → -4 |
| **FLT_MAX** | Mayor valor finito de un `float` (≈ 3.4·10³⁸) |
| **errno / ERANGE** | Variable global de errores de C / código "resultado fuera de rango" |
| **std::floor / std::fabs** | Redondeo hacia abajo / valor absoluto, sin salir de `double` |
| **std::fixed / setprecision** | Notación no científica / nº de decimales; son persistentes en `cout` |
| **Comportamiento indefinido** | El estándar no garantiza nada: puede funcionar, dar basura o fallar |
