# CPP09 — ex02: PmergeMe, Ford‑Johnson / merge‑insert (`std::vector` + `std::deque`)

> Objetivo: que entiendas el algoritmo de Ford‑Johnson **a mano**, por qué cada
> paso ahorra comparaciones, y qué hace **cada línea** del código. Las trazas
> están generadas con el mismo algoritmo (instrumentado) y las salidas son
> reales.

---

## Índice

1. [Qué pide el ejercicio](#1-qué-pide-el-ejercicio)
2. [La idea de Ford‑Johnson en 5 pasos](#2-la-idea-de-fordjohnson-en-5-pasos)
3. [Por qué funciona y por qué ahorra comparaciones](#3-por-qué-funciona-y-por-qué-ahorra-comparaciones)
4. [Jacobsthal: el orden de inserción](#4-jacobsthal-el-orden-de-inserción)
5. [El truco de ordenar posiciones en vez de valores](#5-el-truco-de-ordenar-posiciones-en-vez-de-valores)
6. [Mapa de ficheros y flujo](#6-mapa-de-ficheros-y-flujo)
7. [main.cpp línea a línea](#7-maincpp-línea-a-línea)
8. [PmergeMe.hpp línea a línea](#8-pmergemehpp-línea-a-línea)
9. [PmergeMe.cpp línea a línea](#9-pmergemecpp-línea-a-línea)
10. [Trazas completas](#10-trazas-completas)
11. [Verificación: número de comparaciones](#11-verificación-número-de-comparaciones)
12. [vector vs deque y la medición de tiempo](#12-vector-vs-deque-y-la-medición-de-tiempo)
13. [Tabla de pruebas (salida real)](#13-tabla-de-pruebas-salida-real)
14. [Decisiones de diseño](#14-decisiones-de-diseño)
15. [Preguntas típicas de evaluación](#15-preguntas-típicas-de-evaluación)

---

## 1. Qué pide el ejercicio

| Frase del enunciado | Qué implica | Dónde se cumple |
|---|---|---|
| *"The name of the program is PmergeMe"* | Ejecutable `PmergeMe` | `Makefile` |
| *"use a positive integer sequence as an argument"* | Cada número es un argumento; solo enteros positivos | `parseNumber` (1 … `INT_MAX`) |
| *"must use the merge-insert sort algorithm ... Ford-Johnson"* (Knuth, TAOCP vol. 3, p. 184) | Implementar Ford‑Johnson de verdad | `fjVec`, `fjDeq` (hacen exactamente las comparaciones del libro, ver sección 11) |
| *"If an error occurs ... standard error"* | `std::cerr` | `main` |
| *"at least two different containers"* | Dos contenedores | `std::vector` y `std::deque` |
| *"handle at least 3000 different integers"* | Rendimiento razonable | Probado con 3000 |
| *"strongly advised to implement your algorithm for each container ... avoid using a generic function"* | Nada de plantilla para el algoritmo | `fjVec` y `fjDeq` son funciones separadas (solo `printContainer` es plantilla) |
| Línea 1: *"explicit text followed by the unsorted ... sequence"* | `Before: ...` | `run` |
| Línea 2: *"... sorted ..."* | `After: ...` | `run` |
| Líneas 3 y 4: tiempo con cada contenedor | `Time to process a range of N elements with std::vector : X us` | `run` |
| *"precision ... clearly see the difference"* | Unidad pequeña | microsegundos (`us`) |
| *"indicate the time used to perform all your operations, both the sorting part and the data management part"* | El tiempo incluye cargar los datos en el contenedor | `sortVector`/`sortDeque` cargan **y** ordenan dentro del tramo cronometrado |
| *"The container(s) you used in the previous exercises are forbidden"* | Ni `map` (ex00) ni `list` (ex01, dentro del stack) | ✓ |
| *"The management of errors related to duplicates is left to your discretion"* | Decides tú | Se **aceptan** duplicados |

---

## 2. La idea de Ford‑Johnson en 5 pasos

Ejemplo del PDF: `3 5 9 7 4`

**Paso 1 — Emparejar.** Se agrupan los elementos de dos en dos y en cada par
se separa el menor del mayor (1 comparación por par). Si hay un número impar
de elementos, el último se queda solo (**straggler**, "rezagado").
```
(3,5) (9,7)→ menor 7, mayor 9      straggler = 4
```

**Paso 2 — Ordenar recursivamente los mayores** con el mismo algoritmo.
```
mayores = [5, 9]  → ordenados: [5, 9]
```

**Paso 3 — Nombrar.** En notación de Knuth, tras ordenar los mayores:
`a0 ≤ a1 ≤ …` son los mayores ordenados y `b_i` es la pareja (menor) de
`a_i`. El straggler, si existe, es el último `b` y **no tiene pareja**.
```
a = [5, 9]     b = [3, 7, 4]     (b0=3 va con a0=5, b1=7 con a1=9, b2=4 straggler)
```

**Paso 4 — Cadena principal (main chain).** `b0` seguido de todos los `a`:
```
chain = [3, 5, 9]
```
`b0` va delante **sin comparar**: `b0 ≤ a0` y `a0` es el menor de los
mayores.

**Paso 5 — Insertar el resto de `b`** con **búsqueda binaria**, en orden de
Jacobsthal, buscando **solo a la izquierda de su pareja** (sabemos que
`b_i ≤ a_i`). El straggler, que no tiene pareja, busca en toda la cadena.
```
orden: b2, b1
b2 = 4 (straggler) → busca en [3, 5, 9]           → [3, 4, 5, 9]
b1 = 7 → busca solo antes de a1 = 9: [3, 4, 5]    → [3, 4, 5, 7, 9]
```

---

## 3. Por qué funciona y por qué ahorra comparaciones

- **Correcto**: tras el paso 4 la cadena está ordenada. Cada inserción usa
  búsqueda binaria dentro de una zona ordenada, así que sigue ordenada. Al
  final contiene todos los elementos.
- **Buscar solo hasta la pareja**: si `b_i ≤ a_i`, todo lo que hay desde
  `a_i` hacia la derecha es ≥ `b_i`; nunca irá allí. Zona más pequeña →
  menos comparaciones.
- **Por qué importa el orden**: una búsqueda binaria en una zona de
  `2^k − 1` posiciones cuesta como mucho `k` comparaciones; en una de `2^k`
  ya puede costar `k+1`. El orden de Jacobsthal está elegido para que cada
  búsqueda caiga en una zona de **exactamente** `2^k − 1` posiciones (el caso
  en que la búsqueda binaria no desperdicia nada). En la traza de la sección
  10 se ve: todas las búsquedas son en zonas de 3 o 7 posiciones.
- **Resultado**: Ford‑Johnson hace **menos comparaciones** que merge sort o
  insertion sort; durante décadas fue lo mejor conocido. **Ojo**: menos
  comparaciones ≠ más rápido en tiempo real. Inserta en medio de
  contenedores (caro), así que en segundos suele ser más lento que
  `std::sort`.

---

## 4. Jacobsthal: el orden de inserción

Números de Jacobsthal: `J(k) = J(k−1) + 2·J(k−2)`
```
1, 3, 5, 11, 21, 43, 85, 171, ...
```
Knuth (con `b` numerados desde 1) inserta en grupos que acaban en un número
de Jacobsthal, y dentro de cada grupo de **mayor a menor**:
```
b3 b2 | b5 b4 | b11 b10 … b6 | b21 … b12 | …
```
En el código los índices empiezan en 0 (`b0` es el que ya está en la cadena),
así que se resta 1 a todo:
```
2 1 | 4 3 | 10 9 … 5 | 20 … 11 | …
```
Orden real que genera `buildInsertOrder(m)` (`m` = número total de `b`):
```
m=2:  1
m=3:  2 1
m=5:  2 1 4 3
m=6:  2 1 4 3 5
m=11: 2 1 4 3 10 9 8 7 6 5
m=12: 2 1 4 3 10 9 8 7 6 5 11
```
Si un grupo pasaría del último `b` que existe, se recorta (por eso con
`m = 6` el tercer grupo solo tiene el `5`).

---

## 5. El truco de ordenar posiciones en vez de valores

Después de ordenar los mayores recursivamente hay que saber **qué menor iba
con cada mayor**. Si la recursión devolviera solo valores ordenados, habría
que buscar cada mayor entre los pares (lento, O(n²), y delicado con valores
repetidos).

La solución: `fjVec` / `fjDeq` **no devuelven los valores ordenados, sino
las posiciones en orden**.

```
keys   = [30, 10, 20]      (posiciones 0, 1, 2)
result = [ 1,  2,  0]      → keys[1]=10, keys[2]=20, keys[0]=30
```

Así, cuando la recursión ordena los mayores, devuelve **qué par** va
primero, segundo, etc. (`pairOrder`). Con eso se sabe directamente cuál es el
menor de cada par: `smallPos[pairOrder[i]]`. Es O(1) por elemento y funciona
igual con duplicados, porque cada posición es única aunque los valores se
repitan.

Al final, `sortVector` convierte las posiciones en valores:
`_vec.push_back(keys[order[i]])`.

---

## 6. Mapa de ficheros y flujo

```
main.cpp ──#include──► PmergeMe.hpp ◄──#include── PmergeMe.cpp

main
 ├─ ¿argc < 2? → "Usage..." (stderr), return 1
 └─ try  pm.run(argc, argv)  catch → cerr << e.what(), return 1

run
 ├─ validateArgs        → si algún argumento es inválido: "Error" sin imprimir nada más
 ├─ print "Before:"
 ├─ clock ─ sortVector(argc, argv) ─ clock     ← cargar en vector + Ford-Johnson
 ├─ clock ─ sortDeque(argc, argv)  ─ clock     ← cargar en deque  + Ford-Johnson
 ├─ print "After:"
 └─ print los dos tiempos en microsegundos

fjVec(keys) → posiciones ordenadas            (recursiva)
 ├─ n < 2 → caso base
 ├─ 1. pares: smallPos / largePos / largeKeys
 ├─ 2. pairOrder = fjVec(largeKeys)           ← recursión
 ├─ 3. a[i], b[i] según pairOrder (+ straggler al final de b)
 ├─ 4. chain = [b0, a0, a1, ...];  posA[i] = posición de a[i] en chain
 └─ 5. insertar b1.. en orden Jacobsthal con búsqueda binaria en [0, posA[i])
```

---

## 7. main.cpp línea a línea

```cpp
1  #include "PmergeMe.hpp"
2  #include <iostream>
4  int main(int argc, char** argv)
5  {
6      if (argc < 2)
7      {
8          std::cerr << "Usage: ./PmergeMe <numbers...>" << std::endl;
9          return 1;
10     }
```
Al menos un número. Cada número es un argumento distinto. Por eso en el PDF
se usa ``./PmergeMe `shuf -i 1-100000 -n 3000 | tr "\n" " "` ``: las
comillas invertidas sustituyen el comando por su salida y la shell la parte
en 3000 argumentos.

```cpp
12     PmergeMe pm;
13     try
14     {
15         pm.run(argc, argv);
16     }
17     catch (std::exception& e)
18     {
19         std::cerr << e.what() << std::endl;
20         return 1;
21     }
22     return 0;
```
Cualquier error de entrada se lanza como `std::runtime_error("Error")` y se
imprime aquí por `stderr` (el PDF: `./PmergeMe "-1" "2"` → `Error`).

---

## 8. PmergeMe.hpp línea a línea

```cpp
#include <vector>
#include <deque>
#include <string>
#include <iostream>

class PmergeMe
{
public:
    PmergeMe();
    PmergeMe(PmergeMe const& other);
    PmergeMe& operator=(PmergeMe const& other);
    ~PmergeMe();

    void run(int argc, char** argv);
```
Forma canónica ortodoxa y una única función pública que hace todo.

```cpp
private:
    std::vector<int> _vec;
    std::deque<int>  _deq;
```
El resultado ordenado con cada contenedor.

```cpp
    static int  parseNumber(std::string const& s);
    static void validateArgs(int argc, char** argv);
```
Lectura y validación de la entrada. `static` porque no tocan atributos.

```cpp
    void sortVector(int argc, char** argv);
    void sortDeque(int argc, char** argv);
```
Cada una **carga** los números en su contenedor y los **ordena**. Reciben
`argc`/`argv` porque cargar los datos forma parte de lo que hay que
cronometrar.

```cpp
    static std::vector<size_t> fjVec(std::vector<int> const& keys);
    static std::deque<size_t>  fjDeq(std::deque<int> const& keys);
```
- `fj` = Ford‑Johnson. Reciben los valores (`keys`) **por referencia
  constante** (no se modifican ni se copian) y devuelven las **posiciones**
  en orden (sección 5).
- `size_t`: tipo sin signo pensado para tamaños e índices.
- `static`: no usan atributos, y así pueden llamarse recursivamente con
  contenedores temporales.

```cpp
    template <typename C>
    static void printContainer(std::string const& label, C const& c)
    {
        std::cout << label;
        for (typename C::const_iterator it = c.begin(); it != c.end(); ++it)
            std::cout << *it << " ";
        std::cout << std::endl;
    }
};
```
- Plantilla para imprimir cualquier contenedor. Es solo para imprimir, no
  para el algoritmo, así que no va contra el consejo del enunciado.
- Es la única función implementada en el `.hpp`, y está permitido: las
  reglas generales prohíben implementaciones en headers **excepto plantillas
  de función** (el compilador necesita ver su código donde se usan).
- `typename C::const_iterator`: hace falta `typename` porque el tipo
  **depende** del parámetro `C`; sin él el compilador no sabe si es un tipo.

---

## 9. PmergeMe.cpp línea a línea

### 9.1 Includes y forma canónica (líneas 1‑13)

```cpp
#include "PmergeMe.hpp"
#include <stdexcept>   // std::runtime_error
#include <ctime>       // clock(), clock_t, CLOCKS_PER_SEC
#include <climits>     // INT_MAX
```
No se incluye `<algorithm>`: la búsqueda binaria está escrita a mano para
que se vea (y se pueda contar) cada comparación.

```cpp
PmergeMe::PmergeMe() {}
PmergeMe::PmergeMe(PmergeMe const& other) : _vec(other._vec), _deq(other._deq) {}
PmergeMe& PmergeMe::operator=(PmergeMe const& other)
{
    if (this != &other) { _vec = other._vec; _deq = other._deq; }
    return *this;
}
PmergeMe::~PmergeMe() {}
```
Copia de los dos contenedores, protección de autoasignación, destructor
vacío (RAII).

### 9.2 `buildInsertOrder` (líneas 22‑37)

```cpp
static std::vector<size_t> buildInsertOrder(size_t m)
```
- `static` **fuera de una clase** = la función solo es visible en este
  `.cpp` (enlace interno). No es un método.
- `m` = número total de elementos `b` (incluido `b0`, que ya está en la
  cadena). Devuelve los índices de `b` en el orden en que hay que insertarlos.
- Usa `std::vector` también para la versión deque: es una simple lista de
  índices auxiliar, y `vector` es uno de los contenedores de este ejercicio.

```cpp
    std::vector<size_t> order;
    size_t prev = 1;    // J(k-1)
    size_t curr = 3;    // J(k)
```
Dos números de Jacobsthal consecutivos: el grupo actual son los `b` con
índice (1‑based) en `(prev, curr]`.

```cpp
    while (prev < m)
    {
        size_t hi = (curr < m) ? curr : m;
        for (size_t i = hi; i > prev; i--)
            order.push_back(i - 1);
```
- Mientras queden `b` por cubrir (`prev < m`).
- `hi` recorta el grupo si pasaría del último `b` existente.
- Recorre de `hi` bajando hasta `prev + 1` (1‑based) y guarda `i − 1`
  (0‑based). Así queda "de mayor a menor dentro del grupo".
- `i` es `size_t` (sin signo): por eso la condición es `i > prev` y se
  guarda `i − 1`, en vez de bajar hasta 0 (con `i--` desde 0 daría la vuelta
  a un número enorme).

```cpp
        size_t next = curr + 2 * prev;   // J(k+1) = J(k) + 2 * J(k-1)
        prev = curr;
        curr = next;
    }
    return order;
```
Avanza al siguiente número de Jacobsthal.

Traza con `m = 6`:

| prev | curr | hi | índices añadidos (0‑based) | order |
|---|---|---|---|---|
| 1 | 3 | 3 | 2, 1 | `[2, 1]` |
| 3 | 5 | 5 | 4, 3 | `[2, 1, 4, 3]` |
| 5 | 11 | 6 | 5 | `[2, 1, 4, 3, 5]` |
| 11 | 21 | — | (`11 < 6` falso, termina) | |

### 9.3 `fjVec` — Ford‑Johnson con `std::vector` (líneas 45‑113)

**Caso base**
```cpp
size_t n = keys.size();
std::vector<size_t> result;
if (n < 2)
{
    if (n == 1) result.push_back(0);
    return result;
}
```
Con 0 elementos no hay nada; con 1, el orden es simplemente "la posición 0".

**Paso 1 — pares**
```cpp
size_t k = n / 2;
std::vector<size_t> smallPos, largePos;
std::vector<int>    largeKeys;
for (size_t i = 0; i < k; i++)
{
    size_t x = 2 * i, y = 2 * i + 1;
    if (keys[x] > keys[y]) { size_t t = x; x = y; y = t; }
    smallPos.push_back(x);
    largePos.push_back(y);
    largeKeys.push_back(keys[y]);
}
```
- `k` = número de pares (división entera: con `n` impar sobra uno, el
  straggler).
- El par `i` lo forman las posiciones `2i` y `2i+1`.
- **1 comparación por par**: si el de la izquierda es mayor, se
  intercambian las **posiciones** `x` e `y` (no los valores). Tras esto
  `x` = posición del menor, `y` = posición del mayor.
- `smallPos[i]` / `largePos[i]`: dónde están el menor y el mayor del par `i`.
- `largeKeys[i]`: el **valor** del mayor del par `i`, para ordenarlos en la
  recursión.

**Paso 2 — recursión**
```cpp
std::vector<size_t> pairOrder = fjVec(largeKeys);
```
Ordena los mayores. Como devuelve posiciones dentro de `largeKeys`, y la
posición `i` de `largeKeys` es el par `i`, `pairOrder` dice directamente el
**orden de los pares**: `pairOrder[0]` es el par con el mayor más pequeño.

**Paso 3 — `a` y `b`**
```cpp
std::vector<size_t> a, b;
for (size_t i = 0; i < k; i++)
{
    a.push_back(largePos[pairOrder[i]]);
    b.push_back(smallPos[pairOrder[i]]);
}
if (n % 2 == 1)
    b.push_back(n - 1);
```
- `a[i]` = posición del i‑ésimo mayor más pequeño; `b[i]` = posición de su
  pareja. Así se cumple `keys[b[i]] ≤ keys[a[i]]` y `a` está ordenado.
- El straggler (última posición, `n − 1`) se añade como `b[k]`: el único `b`
  **sin pareja** (no existe `a[k]`). Así se inserta con el resto, en su
  turno de Jacobsthal, como hace Knuth.

**Paso 4 — cadena principal y posiciones de las parejas**
```cpp
std::vector<size_t> chain;
chain.push_back(b[0]);
for (size_t i = 0; i < k; i++)
    chain.push_back(a[i]);
std::vector<size_t> posA;
for (size_t i = 0; i < k; i++)
    posA.push_back(i + 1);
```
- `chain = [b0, a0, a1, …, a(k−1)]`: ordenada sin ninguna comparación extra.
- `posA[i]` = **en qué posición de `chain` está `a[i]`**. Al principio
  `a[i]` está en `i + 1` (el `+1` es por `b0` delante). Guardarlo evita
  tener que **buscar** la pareja en la cadena (buscarla costaría
  comparaciones extra que Ford‑Johnson no hace).

**Paso 5 — inserción en orden de Jacobsthal**
```cpp
std::vector<size_t> order = buildInsertOrder(b.size());
for (size_t t = 0; t < order.size(); t++)
{
    size_t i = order[t];
    int    value = keys[b[i]];
```
Para cada `b_i` en el orden calculado, se coge su valor.

```cpp
    size_t lo = 0;
    size_t hi = (i < k) ? posA[i] : chain.size();
```
**Zona de búsqueda** `[lo, hi)`:
- Si `b_i` tiene pareja (`i < k`): desde el principio hasta la posición de
  `a_i`, **sin incluirla**. Como `b_i ≤ a_i`, su sitio está como muy tarde
  justo antes de `a_i`.
- Si es el straggler (`i == k`): toda la cadena.

```cpp
    while (lo < hi)
    {
        size_t mid = lo + (hi - lo) / 2;
        if (keys[chain[mid]] < value)
            lo = mid + 1;
        else
            hi = mid;
    }
```
**Búsqueda binaria** (equivalente a `std::lower_bound`): encuentra la
primera posición cuyo valor es `>= value`.
- `mid = lo + (hi − lo) / 2` en vez de `(lo + hi) / 2`: misma cuenta, pero
  sin riesgo de desbordar al sumar.
- `keys[chain[mid]]`: `chain` guarda posiciones; el valor está en `keys`.
- Si el valor del medio es menor, el sitio está a la derecha (`lo = mid+1`);
  si no, en `mid` o a la izquierda (`hi = mid`).
- Cada vuelta hace **una** comparación y reduce la zona a la mitad.
- Al acabar, `lo == hi` = sitio de inserción.

Ejemplo: insertar `7` en `[3, 4, 5]` (zona antes de `9`):
```
lo=0 hi=3 → mid=1 (4):  4 < 7 → lo=2
lo=2 hi=3 → mid=2 (5):  5 < 7 → lo=3
lo=3 hi=3 → fin → insertar en la posición 3 → [3, 4, 5, 7, 9]
```

```cpp
    chain.insert(chain.begin() + lo, b[i]);
```
Inserta la posición `b[i]` en el sitio encontrado. En un `vector` esto
desplaza a la derecha todos los elementos posteriores.

```cpp
    for (size_t j = 0; j < k; j++)
        if (posA[j] >= lo)
            posA[j]++;
}
return chain;
```
Al insertar en `lo`, todo lo que estaba en `lo` o más a la derecha se mueve
una posición. Se actualiza `posA` para que siga apuntando a cada `a_j`. Esto
**no** son comparaciones entre elementos de la secuencia (solo entre índices),
así que no cuentan para el algoritmo.

`chain` acaba conteniendo todas las posiciones de `keys` en orden: es el
resultado.

### 9.4 `fjDeq` (líneas 117‑179)

**Exactamente la misma lógica** con `std::deque` para `keys`, `result`,
`smallPos`, `largePos`, `largeKeys`, `pairOrder`, `a`, `b`, `chain` y `posA`.
Solo `order` sigue siendo un `std::vector<size_t>`, porque viene de
`buildInsertOrder` (lista auxiliar de índices).

Diferencias de rendimiento:
- Acceso por índice en `deque` = O(1) pero con dos indirecciones (bloque +
  posición dentro del bloque), más lento que en `vector`.
- `insert` en medio: O(n) en los dos; el `deque` puede desplazar hacia el
  extremo más cercano, pero su memoria no es contigua y aprovecha peor la
  caché.

Se escribe dos veces (en vez de una plantilla) porque el enunciado lo
recomienda: *"implement your algorithm for each container and thus to avoid
using a generic function"*.

### 9.5 `parseNumber` (líneas 184‑203)

```cpp
int PmergeMe::parseNumber(std::string const& s)
{
    size_t i = 0;
    if (i < s.length() && s[i] == '+')
        i++;
    if (i == s.length())
        throw std::runtime_error("Error");
```
- Permite un `+` opcional (`+5` es un entero positivo válido).
- Si después no queda nada (`""` o `"+"`), error.
- **No** se acepta `-`: el PDF exige que `-1` dé `Error`.

```cpp
    long val = 0;
    for (; i < s.length(); i++)
    {
        if (s[i] < '0' || s[i] > '9')
            throw std::runtime_error("Error");
        val = val * 10 + (s[i] - '0');
        if (val > INT_MAX)
            throw std::runtime_error("Error");
    }
```
- Convierte dígito a dígito: `"123"` → `1` → `12` → `123`.
- Cualquier carácter que no sea dígito (`"12a"`, `"1 2"`, `"-1"`, `"1.5"`)
  → error.
- Se comprueba `INT_MAX` **en cada paso**: en cuanto se pasa, error. Así
  `val` nunca crece más allá de `INT_MAX · 10 + 9`, que cabe en un `long`
  de 64 bits, y un número de 30 cifras no desborda nada.

```cpp
    if (val == 0)
        throw std::runtime_error("Error");
    return static_cast<int>(val);
}
```
- `0` no es positivo → error.
- `static_cast<int>` es seguro: ya sabemos que `1 ≤ val ≤ INT_MAX`.

### 9.6 `validateArgs` (líneas 205‑209)

```cpp
void PmergeMe::validateArgs(int argc, char** argv)
{
    for (int i = 1; i < argc; i++)
        parseNumber(argv[i]);
}
```
Comprueba **todos** los argumentos antes de imprimir nada. Si uno falla, el
programa solo muestra `Error` (y no una línea `Before:` a medias).

### 9.7 `sortVector` / `sortDeque` (líneas 214‑234)

```cpp
void PmergeMe::sortVector(int argc, char** argv)
{
    std::vector<int> keys;
    for (int i = 1; i < argc; i++)
        keys.push_back(parseNumber(argv[i]));
    std::vector<size_t> order = fjVec(keys);
    _vec.clear();
    for (size_t i = 0; i < order.size(); i++)
        _vec.push_back(keys[order[i]]);
}
```
Todo lo que hace falta para obtener la secuencia ordenada con un `vector`:
1. **Gestión de datos**: convertir los argumentos y cargarlos en el
   contenedor.
2. **Ordenar**: `fjVec` devuelve las posiciones en orden.
3. Construir `_vec` con los valores en ese orden.

Como `run` cronometra esta función entera, el tiempo incluye la carga de
datos, tal como pide el enunciado. `sortDeque` es idéntica con `std::deque`.

### 9.8 `run` (líneas 236‑265)

```cpp
if (argc < 2)
    throw std::runtime_error("Error");

validateArgs(argc, argv);
```
Sin números o con alguno inválido → `Error`.

```cpp
std::cout << "Before: ";
for (int i = 1; i < argc; i++)
    std::cout << parseNumber(argv[i]) << " ";
std::cout << std::endl;
```
Línea 1: la secuencia sin ordenar, tal como llegó (normalizada: `+5` se
muestra como `5`).

```cpp
clock_t startV = clock();
sortVector(argc, argv);
clock_t endV = clock();

clock_t startD = clock();
sortDeque(argc, argv);
clock_t endD = clock();
```
`clock()` devuelve el tiempo de **CPU** consumido por el proceso, en "ticks".
Cada contenedor se mide por separado.

```cpp
printContainer("After:  ", _vec);
```
Línea 2: la secuencia ordenada.

```cpp
double timeV = static_cast<double>(endV - startV) / CLOCKS_PER_SEC * 1000000.0;
double timeD = static_cast<double>(endD - startD) / CLOCKS_PER_SEC * 1000000.0;
```
`ticks / CLOCKS_PER_SEC` = segundos; `× 1 000 000` = microsegundos.
El `static_cast<double>` es imprescindible: sin él, `ticks / CLOCKS_PER_SEC`
sería **división entera** y casi siempre daría 0.

```cpp
std::cout << "Time to process a range of " << _vec.size()
          << " elements with std::vector : " << timeV << " us" << std::endl;
std::cout << "Time to process a range of " << _deq.size()
          << " elements with std::deque  : " << timeD << " us" << std::endl;
```
Líneas 3 y 4, nombrando cada contenedor.

---

## 10. Trazas completas

Generadas con una copia del algoritmo que imprime cada paso. La sangría
indica el nivel de recursión.

### `./PmergeMe 3 5 9 7 4`

```
entrada: 3 5 9 7 4
pares (menor,mayor): (3,5) (7,9)  straggler=4
  entrada: 5 9
  pares (menor,mayor): (5,9)
    entrada: 9                                  ← caso base
  a: 9   b: 5
  chain: 5 9   orden b: (nada)
a: 5 9   b: 3 7 4
chain: 3 5 9   orden b: 2 1
b2=4 busca en 3 pos (straggler, toda la cadena), 2 comp -> 3 4 5 9
b1=7 busca en 3 pos (hasta su a), 2 comp -> 3 4 5 7 9
comparaciones totales: 7
```
Fíjate en `b1 = 7`: aunque la cadena ya tiene 4 elementos, solo busca en
las 3 posiciones anteriores a su pareja `9`.

### `./PmergeMe 8 3 11 6 1 9 4 12 7 2 10` (11 elementos)

```
entrada: 8 3 11 6 1 9 4 12 7 2 10
pares (menor,mayor): (3,8) (6,11) (1,9) (4,12) (2,7)  straggler=10
  entrada: 8 11 9 12 7
  pares (menor,mayor): (8,11) (9,12)  straggler=7
    entrada: 11 12
    pares (menor,mayor): (11,12)
      entrada: 12
    a: 12   b: 11
    chain: 11 12   orden b:
  a: 11 12   b: 8 9 7
  chain: 8 11 12   orden b: 2 1
  b2=7 busca en 3 pos (straggler, toda la cadena), 2 comp -> 7 8 11 12
  b1=9 busca en 3 pos (hasta su a), 2 comp -> 7 8 9 11 12
a: 7 8 9 11 12   b: 2 3 1 6 4 10
chain: 2 7 8 9 11 12   orden b: 2 1 4 3 5
b2=1 busca en 3 pos (hasta su a), 2 comp -> 1 2 7 8 9 11 12
b1=3 busca en 3 pos (hasta su a), 2 comp -> 1 2 3 7 8 9 11 12
b4=4 busca en 7 pos (hasta su a), 3 comp -> 1 2 3 4 7 8 9 11 12
b3=6 busca en 7 pos (hasta su a), 3 comp -> 1 2 3 4 6 7 8 9 11 12
b5=10 busca en 10 pos (straggler, toda la cadena), 3 comp -> 1 2 3 4 6 7 8 9 10 11 12
comparaciones totales: 25
```

Cómo leer el nivel superior:
- Los mayores ordenados son `a = [7, 8, 9, 11, 12]`, y sus parejas
  `b = [2, 3, 1, 6, 4]`; el straggler `10` se añade como `b5`.
- `chain = [2 | 7 8 9 11 12]`.
- **Todas las búsquedas con pareja son en zonas de 3 o 7 posiciones**
  (`2² − 1` y `2³ − 1`): eso es exactamente lo que consigue el orden de
  Jacobsthal. Por ejemplo, `b2 = 1` (pareja `9`) busca en `[2, 7, 8]`; luego
  `b1 = 3` (pareja `8`), aunque `1` ya se insertó, sigue buscando en solo 3
  posiciones: `[1, 2, 7]`.
- 25 comparaciones; el máximo teórico de Ford‑Johnson para 11 elementos es
  26.

---

## 11. Verificación: número de comparaciones

Knuth (TAOCP vol. 3, sección 5.3.1) da el número de comparaciones de
Ford‑Johnson en el peor caso, `F(n)`. Con un programa de prueba que cuenta
las comparaciones de este código (3000 secuencias aleatorias por cada `n`,
con y sin duplicados) el máximo observado coincide con `F(n)` en todos los
casos:

| n | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 | 13 | 14 | 15 | 16 | 17 | 18 | 19 | 20 | 21 |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| F(n) | 0 | 1 | 3 | 5 | 7 | 10 | 13 | 16 | 19 | 22 | 26 | 30 | 34 | 38 | 42 | 46 | 50 | 54 | 58 | 62 | 66 |
| máximo medido | 0 | 1 | 3 | 5 | 7 | 10 | 13 | 16 | 19 | 22 | 26 | 30 | 34 | 38 | 42 | 46 | 50 | 54 | 58 | 62 | 66 |

La misma prueba comprueba que `fjVec` y `fjDeq` ordenan igual que
`std::sort` para tamaños hasta 3000.

---

## 12. vector vs deque y la medición de tiempo

| | `std::vector` | `std::deque` |
|---|---|---|
| Memoria | Un único bloque contiguo | Varios bloques + tabla de punteros |
| Acceso `[i]` | O(1), muy rápido (una suma) | O(1), más lento (dos indirecciones) |
| `push_back` | O(1) amortizado | O(1) |
| `push_front` | O(n) | O(1) |
| `insert` en medio | O(n) (desplaza a la derecha) | O(n) (desplaza hacia el extremo más cercano) |
| Caché | Excelente (contiguo) | Peor |

Ejemplo real con 3000 números:
```
Time to process a range of 3000 elements with std::vector : 5765 us
Time to process a range of 3000 elements with std::deque  : 83847 us
```
El algoritmo accede muchísimo por índice (búsqueda binaria, `posA`, `keys`),
y ahí el `vector` es mucho más rápido. Los números cambian en cada ejecución
y en cada máquina.

---

## 13. Tabla de pruebas (salida real)

| Comando | Salida |
|---|---|
| `./PmergeMe 3 5 9 7 4` | `Before: 3 5 9 7 4` / `After:  3 4 5 7 9` / dos líneas de tiempo |
| `./PmergeMe 5 5 3 1 1` | `After:  1 1 3 5 5` (duplicados aceptados) |
| `./PmergeMe "-1" "2"` | `Error` (igual que el PDF) |
| `./PmergeMe 0` | `Error` (0 no es positivo) |
| `./PmergeMe 1 abc` | `Error` |
| `./PmergeMe 12a` | `Error` |
| `./PmergeMe "1 2"` | `Error` (dos números en un argumento) |
| `./PmergeMe 2147483648` | `Error` (> `INT_MAX`) |
| `./PmergeMe 2147483647` | se acepta |
| `./PmergeMe +5 3` | `Before: 5 3` / `After:  3 5` |
| `./PmergeMe` | `Usage: ./PmergeMe <numbers...>` |
| ``./PmergeMe `shuf -i 1-100000 -n 3000 \| tr "\n" " "` `` | ordenado correctamente |

Para comprobar tú mismo que sale ordenado:
```bash
./PmergeMe `shuf -i 1-100000 -n 3000 | tr "\n" " "` | sed -n 2p \
  | tr ' ' '\n' | sed '/^$/d;1d' | sort -nc && echo OK
```

---

## 14. Decisiones de diseño

- **Duplicados aceptados**: el enunciado lo deja a tu elección. Ordenar
  posiciones hace que funcionen sin casos especiales.
- **`0` rechazado**: el enunciado pide enteros positivos y el 0 no es
  positivo (positivo = estrictamente mayor que 0).
- **Validar todo antes de imprimir**: ante un error solo aparece `Error`.
- **El tiempo incluye la carga de datos** (conversión de argumentos y
  llenado del contenedor) y la ordenación, como pide el enunciado. No incluye
  imprimir.
- **Búsqueda binaria a mano** en vez de `std::lower_bound`: hace lo mismo,
  pero deja ver cada comparación (útil para explicar y verificar el número
  de comparaciones).
- **Contenedores usados**: solo `std::vector` y `std::deque` (no se repite
  ni `map` de ex00 ni `list` de ex01).

---

## 15. Preguntas típicas de evaluación

**¿Qué es el straggler?**
El último elemento cuando hay un número impar. No tiene pareja; se trata
como el último `b` y se inserta en su turno de Jacobsthal buscando en toda
la cadena.

**¿Por qué `b0` va a la cadena sin comparar?**
Porque `b0 ≤ a0` y `a0` es el menor de todos los mayores; por tanto `b0` es
≤ que todo lo que hay en la cadena.

**¿Por qué buscas solo hasta la pareja?**
Porque `b_i ≤ a_i`; desde `a_i` hacia la derecha todo es mayor o igual.

**¿Cómo sabes dónde está la pareja sin buscarla?**
Con `posA`: guarda la posición de cada `a_i` en la cadena y se actualiza en
cada inserción. Buscarla costaría comparaciones extra.

**¿Para qué sirve Jacobsthal?**
Para que cada búsqueda binaria se haga en una zona de `2^k − 1` posiciones,
el caso en que la búsqueda binaria no desperdicia ninguna comparación. En la
traza de 11 elementos todas las búsquedas con pareja son en zonas de 3 o 7.

**¿Por qué devuelves posiciones y no valores?**
Para saber en O(1) qué menor iba con cada mayor tras la recursión, incluso
con valores repetidos.

**¿Por qué rechazas el 0?**
El enunciado pide una secuencia de enteros **positivos** (*"positive integer
sequence"*), y el 0 no es positivo: positivo significa estrictamente mayor
que 0. Por eso `0` da `Error`, igual que los negativos. Es una decisión de
diseño; si se quisiera aceptar, bastaría con quitar el `if (val == 0)` de
`parseNumber`.

**¿Por qué dos funciones y no una plantilla?**
Lo recomienda el enunciado, para implementar el algoritmo con cada
contenedor.

**¿Por qué el vector es más rápido?**
Memoria contigua: mejor uso de la caché y acceso por índice más directo.

**¿Qué mide `clock()`?**
Tiempo de CPU del proceso (no tiempo de reloj de pared). Dividido entre
`CLOCKS_PER_SEC` da segundos.

**¿Complejidad?**
Comparaciones: el mínimo de Ford‑Johnson, unas `n·log2(n) − 1.415·n`.
Tiempo total: O(n²) en el peor caso, por los `insert` en medio del
contenedor y la actualización de `posA`.
