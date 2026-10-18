# CPP06 — ex02: Identify real type (dynamic_cast)

> Objetivo de este documento: entender cómo un programa puede averiguar en
> **tiempo de ejecución** de qué clase es realmente un objeto cuando solo
> tiene un puntero o referencia a su clase base.

---

## Índice

1. [Qué pide el ejercicio](#1-qué-pide-el-ejercicio)
2. [Mapa de ficheros y cómo se relacionan](#2-mapa-de-ficheros-y-cómo-se-relacionan)
3. [La idea central: tipo estático vs tipo dinámico](#3-la-idea-central-tipo-estático-vs-tipo-dinámico)
4. [Base.hpp línea a línea](#4-basehpp-línea-a-línea)
5. [Base.cpp línea a línea](#5-basecpp-línea-a-línea)
6. [main.cpp línea a línea](#6-maincpp-línea-a-línea)
7. [Trazas completas con ejemplos](#7-trazas-completas-con-ejemplos)
8. [Cómo sabe dynamic_cast el tipo: vtable y RTTI](#8-cómo-sabe-dynamic_cast-el-tipo-vtable-y-rtti)
9. [Puntero vs referencia: comparación](#9-puntero-vs-referencia-comparación)
10. [Preguntas típicas de evaluación](#10-preguntas-típicas-de-evaluación)
11. [Resumen de conceptos](#11-resumen-de-conceptos)

---

## 1. Qué pide el ejercicio

- Una clase `Base` con **solo** un destructor virtual público.
- Tres clases vacías `A`, `B`, `C` que heredan públicamente de `Base`.
- Tres funciones:
  - `Base* generate(void);` → crea al azar un `A`, `B` o `C` y lo devuelve como `Base*`.
  - `void identify(Base* p);` → imprime `"A"`, `"B"` o `"C"` según el tipo real.
  - `void identify(Base& p);` → lo mismo, pero **sin usar punteros dentro**.
- Prohibido incluir `<typeinfo>` (por tanto, prohibido `typeid`).

---

## 2. Mapa de ficheros y cómo se relacionan

```
 ┌──────────┐
 │ Base.hpp │   class Base { virtual ~Base(); }
 │          │   class A, B, C : public Base
 │          │   declara generate / identify(Base*) / identify(Base&)
 └──▲────▲──┘
    │    │ #include
    │  ┌─┴────────┐
    │  │ Base.cpp │   define ~Base(), generate, los dos identify
    │  └──────────┘
    │ #include
 ┌──┴───────┐
 │ main.cpp │   srand → bucle 6 veces: generate → identify(ptr) → identify(ref) → delete
 └──────────┘
```

Flujo de una iteración del bucle de `main`:

```
generate()  ──► new A / new B / new C  ──► se devuelve como Base*
                                                │
                     ┌──────────────────────────┤
                     ▼                          ▼
            identify(obj)               identify(*obj)
            (versión puntero)           (versión referencia)
            dynamic_cast<X*>            dynamic_cast<X&> + try/catch
                     │                          │
                     └──────────► imprime "A"/"B"/"C"
                                                │
                                          delete obj
```

---

## 3. La idea central: tipo estático vs tipo dinámico

```cpp
Base* obj = new B();
```

Aquí hay **dos tipos** distintos:

| | Tipo | Quién lo conoce |
|---|---|---|
| **Estático** | `Base*` | El compilador (es lo que está escrito en el código) |
| **Dinámico** | `B` | Solo se sabe en ejecución (es lo que realmente se creó con `new`) |

```
obj (Base*)  ───►  ┌──────────────┐
                   │  objeto B    │   ← en memoria hay un B de verdad
                   │  (parte Base)│
                   └──────────────┘
```

El compilador no puede saber si `generate()` devolverá un `A`, `B` o `C`
(depende de `rand()`). Por eso necesitamos una herramienta que pregunte **en
ejecución**: "¿este objeto es en realidad un A?". Esa herramienta es
`dynamic_cast`.

---

## 4. Base.hpp línea a línea

```cpp
1  #ifndef BASE_HPP
2  #define BASE_HPP
3
4  class Base
5  {
6  public:
7      virtual ~Base();
8  };
9
10 class A : public Base {};
11 class B : public Base {};
12 class C : public Base {};
13
14 Base*  generate(void);
15 void   identify(Base* p);
16 void   identify(Base& p);
17
18 #endif
```

| Línea | Qué hace | Por qué |
|-------|----------|---------|
| 7 | Destructor **virtual** | Hace que `Base` sea una clase **polimórfica**. Esto tiene dos efectos: (1) `delete obj` llama al destructor correcto de A/B/C; (2) permite usar `dynamic_cast` para bajar de `Base` a `A`/`B`/`C`. |
| 10-12 | Tres clases que heredan públicamente de `Base` | `{}` = cuerpo vacío. No añaden nada; solo existen para tener tres tipos distintos que identificar. Al heredar de una clase polimórfica, también son polimórficas. |
| 14-16 | Declaraciones de funciones libres (no son métodos de ninguna clase) | `identify` está **sobrecargada**: mismo nombre, distinto parámetro. El compilador elige cuál llamar según le pases un `Base*` o un `Base&`. |

### ¿Qué pasaría sin `virtual`?

```cpp
class Base { public: ~Base(); };   // sin virtual
...
dynamic_cast<A*>(p);   // ERROR DE COMPILACIÓN:
                       // "cannot dynamic_cast 'p' (of type 'class Base*')
                       //  to type 'class A*' (source type is not polymorphic)"
```

No es un fallo silencioso: el compilador directamente se niega. `dynamic_cast`
hacia abajo **exige** que la clase base tenga al menos una función virtual.
El destructor es la elección natural porque además es necesario para que
`delete` sobre un `Base*` funcione bien.

---

## 5. Base.cpp línea a línea

### 5.1 Includes y destructor

```cpp
1  #include "Base.hpp"
2  #include <iostream>    // std::cout
3  #include <cstdlib>     // std::rand
4  #include <ctime>       // (std::time, se usa en main)
5  #include <stdexcept>   // std::exception
6
7  Base::~Base() {}
```

- Línea 5: se incluye `<stdexcept>` para tener `std::exception`. **No** se
  incluye `<typeinfo>` (prohibido), que es donde vive `std::bad_cast`. Por eso
  más abajo se captura `std::exception&` (la clase madre de `bad_cast`) en vez
  de `std::bad_cast&`.
- Línea 7: definición del destructor virtual (vacío). Hay que definirlo porque
  es virtual y se declaró en el header; sin esta línea fallaría el enlazado.

### 5.2 `generate`

```cpp
9  Base* generate(void)
10 {
11     int r = std::rand() % 3;
12     if (r == 0) return new A();
13     if (r == 1) return new B();
14     return new C();
15 }
```

| Línea | Qué hace |
|-------|----------|
| 11 | `std::rand()` da un número pseudoaleatorio grande. `% 3` (resto de dividir entre 3) lo reduce a `0`, `1` o `2`. |
| 12-14 | Crea en el **heap** (`new`) un objeto del tipo elegido. |
| 9 | Devuelve `Base*`: un `A*` se convierte automáticamente a `Base*` (**upcast** implícito, siempre seguro porque un A *es un* Base). |

```
rand() = 1804289383  →  % 3 = 1  →  new B()  →  devuelto como Base*
```

El que llama recibe un `Base*` y **pierde la información** de qué tipo era.
Recuperarla es el objetivo de `identify`.

### 5.3 `identify(Base* p)` — versión puntero

```cpp
17 void identify(Base* p)
18 {
19     if (dynamic_cast<A*>(p))      std::cout << "A" << std::endl;
20     else if (dynamic_cast<B*>(p)) std::cout << "B" << std::endl;
21     else if (dynamic_cast<C*>(p)) std::cout << "C" << std::endl;
22     else                          std::cout << "unknown" << std::endl;
23 }
```

`dynamic_cast<A*>(p)` pregunta: **"¿el objeto al que apunta `p` es un A?"**

- **Sí** → devuelve un `A*` válido (no nulo).
- **No** → devuelve `NULL` (puntero nulo).

Y en C++ un puntero dentro de un `if` se evalúa como:
- no nulo → `true`
- nulo → `false`

Así que cada línea es un "intento": se prueba A, si falla se prueba B, si
falla se prueba C. La línea 22 cubre el caso de que `p` sea `NULL` o de un
tipo que no sea ninguno de los tres.

```
p apunta a un B:
  línea 19: dynamic_cast<A*>(p) → NULL     → false, sigue
  línea 20: dynamic_cast<B*>(p) → B* válido → true  → imprime "B"
  (las líneas 21-22 ni se evalúan, por el else)
```

### 5.4 `identify(Base& p)` — versión referencia

```cpp
25 void identify(Base& p)
26 {
27     try { (void)dynamic_cast<A&>(p); std::cout << "A" << std::endl; return; }
28     catch (std::exception&) {}
29     try { (void)dynamic_cast<B&>(p); std::cout << "B" << std::endl; return; }
30     catch (std::exception&) {}
31     try { (void)dynamic_cast<C&>(p); std::cout << "C" << std::endl; return; }
32     catch (std::exception&) {}
33     std::cout << "unknown" << std::endl;
34 }
```

**El problema:** una referencia **no puede ser nula**. Así que si
`dynamic_cast<A&>` falla, no puede devolver "nada". En su lugar **lanza una
excepción** `std::bad_cast`.

**La solución:** usar `try/catch` como forma de preguntar.

Desglose de la línea 27:

```cpp
try {
    (void)dynamic_cast<A&>(p);   // 1. intenta el cast
    std::cout << "A" << std::endl; // 2. solo llega aquí si NO lanzó
    return;                      // 3. ya lo sabemos, salimos
}
catch (std::exception&) {}       // 4. si lanzó, lo atrapamos y no hacemos nada
                                 //    → seguimos con el siguiente try
```

- **`(void)`** delante del cast: le dice al compilador "sé que no uso el
  resultado, es a propósito". Sin él, con `-Wall -Wextra -Werror` podría
  aparecer un aviso de valor no usado. Lo que nos interesa no es el valor sino
  **si lanza o no**.
- **`return`** dentro del `try`: si el cast funcionó, salimos de la función
  para no seguir probando B y C.
- **`catch (std::exception&)`**: `std::bad_cast` hereda de `std::exception`,
  así que este `catch` lo captura sin necesidad de incluir `<typeinfo>`.
- **`{}` vacío**: no hacemos nada con el error; simplemente pasamos al
  siguiente intento.

```
p es un C:
  línea 27: dynamic_cast<A&> → lanza bad_cast → salta a línea 28 (catch) → nada
  línea 29: dynamic_cast<B&> → lanza bad_cast → salta a línea 30 (catch) → nada
  línea 31: dynamic_cast<C&> → OK → imprime "C" → return
```

El enunciado exige **no usar punteros** dentro de esta función. Por eso no
vale hacer el truco de `identify(&p)`: hay que usar el cast a referencia.

---

## 6. main.cpp línea a línea

```cpp
1  #include "Base.hpp"
2  #include <iostream>
3  #include <cstdlib>
4  #include <ctime>
5
6  int main()
7  {
8      std::srand(std::time(NULL));
9
10     for (int i = 0; i < 6; i++)
11     {
12         Base* obj = generate();
13         std::cout << "Via pointer : ";
14         identify(obj);
15         std::cout << "Via reference: ";
16         identify(*obj);
17         std::cout << std::endl;
18         delete obj;
19     }
20     return 0;
21 }
```

| Línea | Qué hace | Por qué |
|-------|----------|---------|
| 8 | `srand(time(NULL))` = **semilla** del generador aleatorio con la hora actual. | Sin esto, `rand()` daría siempre la misma secuencia en cada ejecución. |
| 10 | Repite 6 veces | Para ver varios tipos distintos en una sola ejecución. |
| 12 | Obtiene un objeto aleatorio como `Base*`. | Llama a `Base.cpp:9`. |
| 14 | `identify(obj)` → `obj` es `Base*` → el compilador elige la **versión puntero**. | `Base.cpp:17`. |
| 16 | `identify(*obj)` → `*obj` es un `Base&` (el objeto en sí) → el compilador elige la **versión referencia**. | `Base.cpp:25`. Así se demuestra que ambas dan el mismo resultado. |
| 18 | `delete obj` | Libera la memoria del `new` de `generate`. Gracias al destructor **virtual**, se llama al destructor de A/B/C y luego al de Base. Sin `virtual`, sería comportamiento indefinido. |

---

## 7. Trazas completas con ejemplos

### Salida real de una ejecución

```
Via pointer : C
Via reference: C

Via pointer : A
Via reference: A

Via pointer : A
Via reference: A

Via pointer : B
Via reference: B

Via pointer : B
Via reference: B

Via pointer : A
Via reference: A
```

(El orden cambia en cada ejecución por `srand(time(NULL))`.)

### Traza de la primera iteración

```
generate():
    rand() % 3 = 2  →  new C()  →  devuelve Base* obj

identify(obj)   [versión Base*]
    dynamic_cast<A*>(obj) → NULL   ✘
    dynamic_cast<B*>(obj) → NULL   ✘
    dynamic_cast<C*>(obj) → C*     ✔  → "C"

identify(*obj)  [versión Base&]
    dynamic_cast<A&>(*obj) → throw bad_cast → catch
    dynamic_cast<B&>(*obj) → throw bad_cast → catch
    dynamic_cast<C&>(*obj) → OK              → "C", return

delete obj  →  ~C()  →  ~Base()
```

---

## 8. Cómo sabe dynamic_cast el tipo: vtable y RTTI

Cuando una clase tiene al menos una función virtual, el compilador:

1. Crea **una tabla virtual (vtable) por cada clase** (`Base`, `A`, `B`, `C`).
   Contiene los punteros a sus funciones virtuales y un puntero a la
   **información de tipo** de esa clase (RTTI, *Run-Time Type Information*).
2. Añade a **cada objeto** un puntero oculto (*vptr*) que apunta a la vtable
   de su clase real.

```
obj (Base*) ──►  ┌─────────────┐
                 │ vptr ───────┼──►  vtable de C
                 └─────────────┘     ├─ &C::~C
                  objeto C           └─ info de tipo: "soy C, hijo de Base"
```

Cuando haces `dynamic_cast<A*>(obj)`:

1. Sigue el `vptr` del objeto hasta su vtable.
2. Lee la info de tipo: "soy C".
3. ¿C es A o hereda de A? No → devuelve `NULL` (o lanza `bad_cast` con referencias).

Por eso:
- Hace falta una función virtual (si no, no hay `vptr` ni RTTI que consultar).
- `dynamic_cast` es algo más lento que `static_cast`: hace trabajo en ejecución.

### ¿Y si usara `static_cast`?

```cpp
A* a = static_cast<A*>(obj);   // compila SIEMPRE, aunque obj sea un C
```

`static_cast` confía ciegamente en ti. Si `obj` era un C, `a` sería un
puntero inválido y usarlo sería comportamiento indefinido. `dynamic_cast` es
la versión **comprobada**.

---

## 9. Puntero vs referencia: comparación

| | `dynamic_cast<A*>(p)` | `dynamic_cast<A&>(r)` |
|---|---|---|
| Si el tipo coincide | Devuelve `A*` válido | Devuelve `A&` válida |
| Si NO coincide | Devuelve `NULL` | Lanza `std::bad_cast` |
| Cómo se comprueba | `if (resultado)` | `try { ... } catch (...)` |
| Por qué es así | Un puntero puede ser nulo | Una referencia no puede ser nula |

---

## 10. Preguntas típicas de evaluación

- **¿Por qué `Base` necesita un destructor virtual?** Para ser polimórfica
  (sin eso `dynamic_cast` no compila) y para que `delete` sobre un `Base*`
  destruya correctamente el objeto derivado.
- **¿Qué devuelve `dynamic_cast` cuando falla?** `NULL` con punteros; lanza
  `std::bad_cast` con referencias.
- **¿Por qué capturas `std::exception` y no `std::bad_cast`?** Porque
  `bad_cast` está en `<typeinfo>`, que está prohibido; y `bad_cast` hereda de
  `std::exception`, así que se captura igual.
- **¿Para qué sirve el `(void)`?** Para indicar que descartamos el resultado a
  propósito; solo importa si lanza o no.
- **¿Por qué `srand(time(NULL))`?** Para que la secuencia aleatoria cambie en
  cada ejecución.
- **¿Qué es upcast y downcast?** Upcast: derivada → base (`A*` → `Base*`),
  implícito y seguro. Downcast: base → derivada (`Base*` → `A*`), necesita
  comprobación → `dynamic_cast`.

---

## 11. Resumen de conceptos

| Concepto | Qué es |
|----------|--------|
| **Clase polimórfica** | Clase con al menos una función virtual |
| **Tipo estático** | El tipo declarado en el código (`Base*`) |
| **Tipo dinámico** | El tipo real del objeto en memoria (`A`, `B` o `C`) |
| **Upcast** | Derivada → base. Implícito y siempre seguro |
| **Downcast** | Base → derivada. Necesita `dynamic_cast` para ser seguro |
| **dynamic_cast con puntero** | Devuelve `NULL` si el cast falla |
| **dynamic_cast con referencia** | Lanza `std::bad_cast` si el cast falla |
| **vtable / vptr** | Tabla por clase con funciones virtuales e info de tipo / puntero oculto en cada objeto hacia ella |
| **RTTI** | Información de tipo en ejecución; es lo que consulta `dynamic_cast` |
| **Destructor virtual** | Garantiza el `delete` correcto a través de un puntero base |
| **Sobrecarga** | Dos funciones con el mismo nombre y distintos parámetros (`identify`) |
| **srand / rand** | Semilla y generador de números pseudoaleatorios |
