# CPP09 — ex01: RPN, Notación Polaca Inversa (`std::stack`)

> Objetivo: que entiendas qué es la notación polaca inversa, por qué se evalúa
> con una pila y qué hace **cada línea** del código. Al final hay trazas paso a
> paso con el estado de la pila, salidas reales y preguntas de evaluación.

---

## Índice

1. [Qué pide el ejercicio](#1-qué-pide-el-ejercicio)
2. [Qué es la notación polaca inversa](#2-qué-es-la-notación-polaca-inversa)
3. [Por qué `std::stack` (y por qué sobre `std::list`)](#3-por-qué-stdstack-y-por-qué-sobre-stdlist)
4. [Mapa de ficheros y flujo](#4-mapa-de-ficheros-y-flujo)
5. [main.cpp línea a línea](#5-maincpp-línea-a-línea)
6. [RPN.hpp línea a línea](#6-rpnhpp-línea-a-línea)
7. [RPN.cpp línea a línea](#7-rpncpp-línea-a-línea)
8. [Trazas completas con la pila](#8-trazas-completas-con-la-pila)
9. [Tabla de pruebas (salida real)](#9-tabla-de-pruebas-salida-real)
10. [Decisiones de diseño](#10-decisiones-de-diseño)
11. [Preguntas típicas de evaluación](#11-preguntas-típicas-de-evaluación)

---

## 1. Qué pide el ejercicio

| Frase del enunciado | Qué implica | Dónde se cumple |
|---|---|---|
| *"The program name is RPN"* | Ejecutable `RPN` | `Makefile` |
| *"take an inverted Polish mathematical expression as an argument"* | Toda la expresión en **un** argumento entre comillas | `main`: `argc != 2` |
| *"numbers ... will always be less than 10"* | Los operandos de entrada son de **un dígito** (0‑9) | `token.length() != 1` → error; `isdigit` |
| *"The calculation itself but also the result do not take into account this rule"* | Resultados intermedios y final pueden ser cualquier `int` (negativos, > 9) | La pila es de `int`; `apply` detecta overflow |
| *"output the correct result on the standard output"* | `std::cout` | `main` |
| *"If an error occurs ... error message ... on the standard error"* | `std::cerr` | `main`, `catch` |
| *"handle ... tokens: "+ - / *""* | Los 4 operadores | `apply` |
| *"You don't need to handle brackets or decimal numbers"* | `(` y `.` son error | Cualquier token desconocido → `Error` |
| *"The container(s) you used in the previous exercise are forbidden here"* | No se puede usar `map` | Se usa `stack` sobre `list` |

---

## 2. Qué es la notación polaca inversa

En la notación normal (**infija**) el operador va **entre** los operandos y
necesitamos paréntesis y prioridades: `(3 + 4) * 2`.

En **RPN** (postfija) el operador va **después** de sus dos operandos:

| Infija | RPN |
|---|---|
| `3 + 4` | `3 4 +` |
| `(3 + 4) * 2` | `3 4 + 2 *` |
| `3 + 4 * 2` | `3 4 2 * +` |
| `(8 - 2) / 3` | `8 2 - 3 /` |

Ventajas: **no hacen falta paréntesis ni reglas de prioridad**; el orden de
los tokens ya dice qué se calcula primero. Y se evalúa leyendo de izquierda
a derecha una sola vez, con una pila:

1. Si el token es un **número** → se apila.
2. Si es un **operador** → se sacan los dos de arriba, se opera y se apila el
   resultado.
3. Al final, en la pila debe quedar **exactamente un** número: el resultado.

---

## 3. Por qué `std::stack` (y por qué sobre `std::list`)

`std::stack` es una pila **LIFO** (*Last In, First Out*: el último que entra
es el primero que sale). Justo lo que necesita el algoritmo: un operador
siempre trabaja con los **dos últimos** valores apilados.

```cpp
std::stack<int, std::list<int> > st;
st.push(8);    // [8]
st.push(9);    // [8, 9]        ← top = 9
st.top();      // 9  (mira sin sacar)
st.pop();      // [8]           (saca sin devolver)
st.size();     // 1
st.empty();    // false
```

- `std::stack` no es un contenedor en sí, es un **adaptador de
  contenedor**: guarda los datos en otro contenedor y solo expone `push`,
  `pop`, `top`, `size` y `empty`. No tiene iteradores.
- **Por defecto** ese contenedor interno es un `std::deque`
  (`std::stack<int>` = `std::stack<int, std::deque<int> >`).
- **Regla del módulo**: *"Once a container is used you cannot use it for the
  rest of the module"*. El ex02 usa `std::deque`. Si aquí usáramos
  `std::stack<int>`, internamente estaríamos usando un `deque` y un
  evaluador podría considerar que el `deque` ya está gastado. Para evitar
  cualquier discusión, el segundo parámetro de plantilla le dice al stack
  que use **`std::list`**, que no aparece en ningún otro ejercicio.
- El contenedor interno debe ofrecer `back()`, `push_back()` y `pop_back()`;
  `std::list` los tiene, así que vale.
- El espacio en `std::list<int> >` es **obligatorio en C++98**: sin él,
  `>>` se interpreta como el operador de desplazamiento y no compila.
- `pop()` devuelve `void`: hay que leer con `top()` **antes** de hacer
  `pop()`. Por eso en el código siempre aparece `st.top(); st.pop();`.

---

## 4. Mapa de ficheros y flujo

```
main.cpp ──#include──► RPN.hpp ◄──#include── RPN.cpp

main
 ├─ ¿argc == 2? no → "Usage..." (stderr), return 1
 ├─ RPN rpn;
 └─ try
      cout << rpn.evaluate(argv[1])      → resultado por stdout
    catch (std::exception& e)
      cerr << e.what()                   → mensaje por stderr, return 1

evaluate(expr)
 ├─ istringstream sobre expr
 └─ por cada token separado por espacios:
      ├─ longitud != 1  → throw "Error"
      ├─ dígito         → push
      ├─ + - * /        → ¿size < 2? → throw "Error"
      │                   b = top, pop; a = top, pop
      │                   push(apply(a, b, op))
      │                     └─ apply: comprueba división por 0 y overflow
      │                        ANTES de operar
      └─ otra cosa      → throw "Error"
    al final: ¿size != 1? → throw "Error" ; return top
```

---

## 5. main.cpp línea a línea

```cpp
1  #include "RPN.hpp"
2  #include <iostream>       // std::cout, std::cerr
```

```cpp
4  int main(int argc, char** argv)
5  {
6      if (argc != 2)
7      {
8          std::cerr << "Usage: ./RPN \"<expression>\"" << std::endl;
9          return 1;
10     }
```
La expresión tiene que ir **entera en un solo argumento**, entre comillas.
`./RPN 1 2 +` (sin comillas) son 3 argumentos → `argc == 4` → error de uso.
`\"` dentro del string escribe unas comillas literales en el mensaje.

```cpp
12     RPN rpn;
```
Objeto sin estado (la clase no tiene atributos).

```cpp
13     try
14     {
15         std::cout << rpn.evaluate(argv[1]) << std::endl;
16     }
```
Se evalúa y se imprime el resultado. Si `evaluate` lanza una excepción, la
línea 15 se interrumpe **antes** de imprimir nada y se salta al `catch`.

```cpp
17     catch (std::exception& e)
18     {
19         std::cerr << e.what() << std::endl;
20         return 1;
21     }
22     return 0;
```
- Se captura por **referencia** (`std::exception&`): no se copia el objeto y
  se respeta el polimorfismo (`what()` es virtual, así que se llama a la
  versión de `std::runtime_error`, que devuelve el texto que se le pasó).
- `e.what()` devuelve `"Error"`, `"Error: division by zero"` o
  `"Error: overflow"`.
- Código de salida `1` en caso de error.

---

## 6. RPN.hpp línea a línea

```cpp
#ifndef RPN_HPP
#define RPN_HPP

#include <string>

class RPN
{
public:
    RPN();
    RPN(RPN const& other);
    RPN& operator=(RPN const& other);
    ~RPN();
```
Include guard y forma canónica ortodoxa (obligatoria aunque la clase no
tenga atributos).

```cpp
    int evaluate(std::string const& expression) const;
```
- Devuelve `int`: el resultado de la expresión.
- `const`: no modifica el objeto.
- **La pila no es atributo**, es una variable **local** de `evaluate`. Así
  cada llamada empieza con una pila vacía y el objeto no guarda estado entre
  evaluaciones.

```cpp
private:
    static int apply(int a, int b, char op);
};

#endif
```
- `apply` calcula `a op b` comprobando antes que el resultado cabe en un
  `int`. Separarlo de `evaluate` deja el bucle principal corto y legible.
- **`private`**: es un detalle interno, nadie de fuera necesita llamarla.
- **`static`**: no usa ningún atributo del objeto (no hay), solo sus
  parámetros.

---

## 7. RPN.cpp línea a línea

### 7.1 Includes y forma canónica (líneas 1‑11)

```cpp
#include "RPN.hpp"
#include <stack>       // std::stack
#include <list>        // std::list (contenedor interno del stack)
#include <sstream>     // std::istringstream
#include <stdexcept>   // std::runtime_error
#include <cctype>      // std::isdigit
#include <climits>     // INT_MAX, INT_MIN
```

```cpp
RPN::RPN() {}
RPN::RPN(RPN const&) {}
RPN& RPN::operator=(RPN const&) { return *this; }
RPN::~RPN() {}
```
Como no hay atributos, no hay nada que copiar. El parámetro **no tiene
nombre** (`RPN const&`) para que `-Wextra` no avise de "parámetro sin usar"
(`-Werror` lo convertiría en error de compilación).

### 7.2 `apply` — operar sin desbordar (líneas 15‑50)

En C++, el desbordamiento de un entero **con signo** (`int`) es
**comportamiento indefinido**: el programa puede dar cualquier resultado. Por
eso no se puede "operar y luego mirar si se pasó"; hay que comprobar
**antes** de operar, usando solo operaciones que no desbordan.

`INT_MAX` = 2 147 483 647, `INT_MIN` = −2 147 483 648.

**Suma**
```cpp
if (op == '+')
{
    if ((b > 0 && a > INT_MAX - b) || (b < 0 && a < INT_MIN - b))
        throw std::runtime_error("Error: overflow");
    return a + b;
}
```
- Si `b > 0`, `a + b > INT_MAX` ⇔ `a > INT_MAX − b`. Y `INT_MAX − b` no
  desborda porque `b` es positivo.
- Si `b < 0`, `a + b < INT_MIN` ⇔ `a < INT_MIN − b`. `INT_MIN − b` no
  desborda porque `b` es negativo (se suma algo positivo a `INT_MIN`).
- Si `b == 0`, nunca desborda.

**Resta**
```cpp
if (op == '-')
{
    if ((b < 0 && a > INT_MAX + b) || (b > 0 && a < INT_MIN + b))
        throw std::runtime_error("Error: overflow");
    return a - b;
}
```
Misma idea: restar un negativo es sumar.
- `b < 0`: `a − b > INT_MAX` ⇔ `a > INT_MAX + b`.
- `b > 0`: `a − b < INT_MIN` ⇔ `a < INT_MIN + b`.

**Multiplicación**
```cpp
if (op == '*')
{
    if (a != 0 && b != 0)
    {
        if (a > 0 && b > 0 && a > INT_MAX / b) throw ...;   // + * + > INT_MAX
        if (a > 0 && b < 0 && b < INT_MIN / a) throw ...;   // + * - < INT_MIN
        if (a < 0 && b > 0 && a < INT_MIN / b) throw ...;   // - * + < INT_MIN
        if (a < 0 && b < 0 && a < INT_MAX / b) throw ...;   // - * - > INT_MAX
    }
    return a * b;
}
```
Se divide en los cuatro casos de signo. En cada uno se compara con el límite
**dividido** entre uno de los factores (dividir nunca desborda aquí).
Ejemplo: `a = 3, b = 900 000 000` → `INT_MAX / b = 2` → `3 > 2` → overflow.
En el último caso, al dividir entre un negativo la desigualdad se invierte:
`a·b > INT_MAX` ⇔ `a < INT_MAX / b` (con `b < 0`).

**División**
```cpp
if (b == 0)
    throw std::runtime_error("Error: division by zero");
if (a == INT_MIN && b == -1)
    throw std::runtime_error("Error: overflow");
return a / b;
```
- Dividir entre 0 en enteros hace que el programa se cuelgue (`SIGFPE`).
- `INT_MIN / -1` = 2 147 483 648, que no cabe en un `int`: es el único caso
  en que una división entera desborda.
- `a / b` es **división entera**: trunca hacia cero. `7 2 /` → `3`,
  `0 7 - 2 /` (−7 / 2) → `-3`.

### 7.3 `evaluate` — preparación

```cpp
int RPN::evaluate(std::string const& expression) const
{
    std::stack<int, std::list<int> > st;
    std::istringstream ss(expression);
    std::string        token;
```
- `st`: la pila de operandos/resultados, sobre una `std::list` (sección 3).
- `ss`: un **stream de entrada sobre un string**. Funciona igual que `cin`,
  pero lee de `expression` en vez de del teclado.
- `token`: donde se guarda cada "palabra" leída.

### 7.4 El bucle de tokens

```cpp
    while (ss >> token)
```
`ss >> token` lee la siguiente palabra **saltándose los espacios** (uno o
varios, tabuladores incluidos). Devuelve el stream; convertido a `bool` es
`false` cuando ya no quedan palabras → el bucle termina.

```
expression = "8 9 *"
1ª vuelta: token = "8"
2ª vuelta: token = "9"
3ª vuelta: token = "*"
4ª: ss >> token falla → sale del while
```
> Consecuencia: `"89*"` sin espacios es **un solo token** → error. El formato
> exige separar los tokens con espacios.

```cpp
        if (token.length() != 1)
            throw std::runtime_error("Error");
```
Todo token válido (un dígito o un operador) tiene **exactamente un
carácter**. Así se rechazan de golpe `"10"` (número ≥ 10), `"(1"`, `"++"`,
`"1.5"`…

```cpp
        char c = token[0];
```
Como ya sabemos que tiene un carácter, lo guardamos en `c` para no repetir
`token[0]`.

### 7.5 Caso número

```cpp
        if (std::isdigit(static_cast<unsigned char>(c)))
        {
            st.push(c - '0');
            continue;
        }
```
- `isdigit`: ¿es `'0'`…`'9'`?
- `static_cast<unsigned char>`: `isdigit` espera un valor representable como
  `unsigned char` (o `EOF`). Un `char` con un carácter no ASCII puede ser
  negativo y pasarlo directamente sería comportamiento indefinido.
- `c - '0'`: convierte el carácter a su valor numérico. En ASCII `'7'` = 55 y
  `'0'` = 48 → `55 − 48 = 7`.
- `continue`: pasa al siguiente token.

### 7.6 Caso operador

```cpp
        if (c == '+' || c == '-' || c == '*' || c == '/')
        {
            if (st.size() < 2)
                throw std::runtime_error("Error");
```
Un operador necesita **dos** operandos. Si hay menos (`"1 +"` o `"+"`),
hacer `top()` sobre una pila vacía sería **comportamiento indefinido**, así
que se lanza excepción antes.

```cpp
            int b = st.top(); st.pop();
            int a = st.top(); st.pop();
```
**El orden importa**. El que está arriba es el **segundo** operando (`b`),
porque se apiló el último. Para `"8 2 -"`:
```
pila: [8, 2]   top = 2
b = 2, pop → [8]
a = 8, pop → []
a - b = 8 - 2 = 6     ✓   (si se invirtiera daría -4 ✗)
```
Para `+` y `*` el orden da igual (conmutativos), pero para `-` y `/` es
crucial.

```cpp
            st.push(apply(a, b, c));
            continue;
        }
```
Se calcula (con las comprobaciones de `apply`) y se **apila el resultado**,
que puede servir de operando para el siguiente operador.

### 7.7 Cualquier otra cosa

```cpp
        throw std::runtime_error("Error");
    }
```
Un carácter que no es ni dígito ni operador (`"("`, `"a"`, `"."`).

### 7.8 Comprobación final

```cpp
    if (st.size() != 1)
        throw std::runtime_error("Error");

    return st.top();
}
```
Una expresión RPN correcta deja **exactamente un** valor:
- `size() == 0` → expresión vacía (`""`).
- `size() > 1` → sobran números (`"1 2"`: dos números sin operador).

---

## 8. Trazas completas con la pila

### `./RPN "8 9 * 9 - 9 - 9 - 4 - 1 +"` → 42

| Token | Acción | Pila después |
|---|---|---|
| `8` | push 8 | `[8]` |
| `9` | push 9 | `[8, 9]` |
| `*` | b=9, a=8 → 8*9=72 | `[72]` |
| `9` | push 9 | `[72, 9]` |
| `-` | b=9, a=72 → 63 | `[63]` |
| `9` | push | `[63, 9]` |
| `-` | 63-9 = 54 | `[54]` |
| `9` | push | `[54, 9]` |
| `-` | 54-9 = 45 | `[45]` |
| `4` | push | `[45, 4]` |
| `-` | 45-4 = 41 | `[41]` |
| `1` | push | `[41, 1]` |
| `+` | 41+1 = 42 | `[42]` |
| fin | size == 1 → return 42 | |

### `./RPN "1 2 * 2 / 2 * 2 4 - +"` → 0

| Token | Acción | Pila |
|---|---|---|
| `1` | push | `[1]` |
| `2` | push | `[1, 2]` |
| `*` | 1*2 = 2 | `[2]` |
| `2` | push | `[2, 2]` |
| `/` | 2/2 = 1 | `[1]` |
| `2` | push | `[1, 2]` |
| `*` | 1*2 = 2 | `[2]` |
| `2` | push | `[2, 2]` |
| `4` | push | `[2, 2, 4]` |
| `-` | b=4, a=2 → 2-4 = **-2** | `[2, -2]` |
| `+` | 2 + (-2) = 0 | `[0]` |
| fin | return 0 | |

Aquí se ve que en la pila pueden quedar **varios** valores esperando
(`[2, 2, 4]`), y que los resultados pueden ser negativos.

### `./RPN "9 9 * 9 * 9 * 9 * 9 * 9 * 9 * 9 * 9 * 9 *"` → Error: overflow

| Paso | Pila | Comentario |
|---|---|---|
| … | `[387420489]` | 9⁹, todavía cabe |
| `9 *` | — | `INT_MAX / 9 = 238609294` y `387420489 > 238609294` → overflow detectado **antes** de multiplicar |

### `./RPN "(1 + 1)"` → Error

| Token | Acción |
|---|---|
| `(1` | longitud 2 → `throw "Error"` |

### `./RPN "1 +"` → Error

| Token | Acción | Pila |
|---|---|---|
| `1` | push | `[1]` |
| `+` | `size() == 1 < 2` → `throw "Error"` | |

### `./RPN "1 2"` → Error

Ambos se apilan → `[1, 2]` → al final `size() == 2 != 1` → `throw "Error"`.

---

## 9. Tabla de pruebas (salida real)

| Comando | stdout | stderr |
|---|---|---|
| `./RPN "8 9 * 9 - 9 - 9 - 4 - 1 +"` | `42` | |
| `./RPN "7 7 * 7 -"` | `42` | |
| `./RPN "1 2 * 2 / 2 * 2 4 - +"` | `0` | |
| `./RPN "(1 + 1)"` | | `Error` |
| `./RPN "1 0 /"` | | `Error: division by zero` |
| `./RPN "1 +"` | | `Error` |
| `./RPN "1 2"` | | `Error` |
| `./RPN "10 2 +"` | | `Error` (10 no es < 10) |
| `./RPN ""` | | `Error` (pila vacía al final) |
| `./RPN "5"` | `5` | |
| `./RPN "0 7 - 2 /"` | `-3` | (división entera trunca hacia 0) |
| `./RPN "9 9 * 9 * 9 * 9 * 9 * 9 * 9 * 9 *"` | `387420489` | (9⁹, cabe en un `int`) |
| `./RPN "9 9 * 9 * 9 * 9 * 9 * 9 * 9 * 9 * 9 * 9 *"` | | `Error: overflow` |

Los tres primeros ejemplos y `(1 + 1)` coinciden exactamente con el PDF.

---

## 10. Decisiones de diseño

- **Resultados limitados a `int`**: el enunciado dice que el resultado no
  está limitado a < 10, pero no pide números arbitrariamente grandes. Si un
  cálculo se sale de `int`, se informa con `Error: overflow` en vez de dar un
  número incorrecto.
- **`std::stack` sobre `std::list`**: para no usar `std::deque` (el
  contenedor por defecto del stack), que se usa en ex02.
- **Mensajes**: `Error` genérico como en el PDF; para división entre 0 y
  overflow se añade el motivo (sigue empezando por `Error`).

---

## 11. Preguntas típicas de evaluación

**¿Por qué una pila?**
Porque un operador siempre opera sobre los dos últimos valores producidos,
y eso es exactamente lo que da una LIFO.

**¿Por qué sacas primero `b` y luego `a`?**
Porque el último en entrar fue el segundo operando. En `8 2 -` hay que
calcular `8 - 2`, no `2 - 8`.

**¿Qué es `std::stack` por dentro? ¿Por qué `std::list`?**
Un adaptador sobre otro contenedor; por defecto `std::deque`. Como ex02 usa
`deque` y cada contenedor solo se puede usar en un ejercicio, aquí se le
indica `std::list` como contenedor interno.

**¿Cómo detectas el overflow?**
Antes de operar, comparando con `INT_MAX`/`INT_MIN` usando restas o
divisiones que no pueden desbordar. Operar primero y mirar después no sirve:
el overflow de `int` es comportamiento indefinido.

**¿Qué pasa si hay un solo número? (`"5"`)**
Se apila, al final `size() == 1` y se imprime `5`. Es una expresión RPN
válida.

**¿Qué pasa con espacios múltiples (`"1   2 +"`)?**
`>>` salta cualquier cantidad de espacios en blanco: funciona igual.

**¿Por qué el resultado puede ser mayor que 9 o negativo?**
El enunciado solo limita los números de **entrada**; los cálculos y el
resultado no.

**¿Por qué `runtime_error` y no `exception`?**
`std::exception` no tiene constructor con mensaje en C++98;
`std::runtime_error` sí y hereda de `std::exception`, así que se captura con
`catch (std::exception&)`.

**¿Complejidad?**
O(n) en tiempo (cada token se procesa una vez, push/pop son O(1)) y O(n) en
memoria en el peor caso (todos números).
