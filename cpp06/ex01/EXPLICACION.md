# CPP06 — ex01: Serializer (reinterpret_cast)

> Objetivo de este documento: entender qué es un puntero "por dentro", por qué
> se puede guardar en un número y cómo `reinterpret_cast` hace ese viaje de
> ida y vuelta sin tocar los datos.

---

## Índice

1. [Qué pide el ejercicio](#1-qué-pide-el-ejercicio)
2. [Mapa de ficheros y cómo se relacionan](#2-mapa-de-ficheros-y-cómo-se-relacionan)
3. [La idea central: un puntero ES un número](#3-la-idea-central-un-puntero-es-un-número)
4. [Data.hpp línea a línea](#4-datahpp-línea-a-línea)
5. [Serializer.hpp línea a línea](#5-serializerhpp-línea-a-línea)
6. [Serializer.cpp línea a línea](#6-serializercpp-línea-a-línea)
7. [main.cpp línea a línea](#7-maincpp-línea-a-línea)
8. [Salida real y cómo leerla](#8-salida-real-y-cómo-leerla)
9. [¿Por qué no `static_cast`? ¿Por qué no `int`?](#9-por-qué-no-static_cast-por-qué-no-int)
10. [Peligros de reinterpret_cast](#10-peligros-de-reinterpret_cast)
11. [Preguntas típicas de evaluación](#11-preguntas-típicas-de-evaluación)
12. [Resumen de conceptos](#12-resumen-de-conceptos)

---

## 1. Qué pide el ejercicio

Crear una clase `Serializer` **no instanciable** con dos métodos estáticos:

```cpp
uintptr_t serialize(Data* ptr);     // puntero → entero
Data*     deserialize(uintptr_t raw); // entero  → puntero
```

Y una estructura `Data` **no vacía**. Después, comprobar en el `main` que
`deserialize(serialize(&obj)) == &obj`, es decir, que el puntero sobrevive al
viaje de ida y vuelta.

---

## 2. Mapa de ficheros y cómo se relacionan

```
 ┌──────────┐
 │ Data.hpp │  define struct Data { id, label, value }
 └────▲─────┘
      │ #include
 ┌────┴───────────┐   #include <stdint.h>  (para uintptr_t)
 │ Serializer.hpp │  declara serialize / deserialize
 └────▲───────▲───┘
      │       │ #include
      │  ┌────┴────────────┐
      │  │ Serializer.cpp  │  implementa los dos reinterpret_cast
      │  └─────────────────┘
      │ #include
 ┌────┴─────┐
 │ main.cpp │  crea un Data, lo serializa, lo deserializa y compara
 └──────────┘
```

`main.cpp` solo incluye `Serializer.hpp`, pero como ese header ya incluye
`Data.hpp`, `main` también conoce `Data`. Los headers se "encadenan".

---

## 3. La idea central: un puntero ES un número

La memoria del ordenador es como una calle enorme con casas numeradas. Cada
byte tiene un número (su **dirección**). Un puntero no es más que una variable
que guarda **uno de esos números**.

```
Memoria (simplificado)
dirección          contenido
0x7ffc1d2a73f0  →  [ id = 42 ]                ← &original apunta aquí
0x7ffc1d2a73f8  →  [ label = "hello..." ]
0x7ffc1d2a7418  →  [ value = 3.14 ]
```

- `&original` vale `0x7ffc1d2a73f0` (hexadecimal).
- Ese mismo número en decimal es `140720797807600`.
- **Son el mismo valor escrito de dos formas.**

Serializar aquí es simplemente: "dame ese número como un entero normal".
Deserializar: "trata este entero como una dirección de un `Data`".

```
  Data*  ──serialize──►  uintptr_t  ──deserialize──►  Data*
0x7ffc..73f0         140720797807600              0x7ffc..73f0
```

Los datos (`id`, `label`, `value`) **no se mueven ni se copian** en ningún
momento. Solo cambia la "etiqueta de tipo" con la que el compilador mira ese
número.

---

## 4. Data.hpp línea a línea

```cpp
1  #ifndef DATA_HPP
2  #define DATA_HPP
3
4  #include <string>
5
6  struct Data
7  {
8      int         id;
9      std::string label;
10     double      value;
11 };
12
13 #endif
```

| Línea | Qué hace |
|-------|----------|
| 1-2, 13 | Include guard (evita definir `Data` dos veces si se incluye varias veces). |
| 4 | Necesario para `std::string label`. |
| 6 | `struct` = igual que `class` pero sus miembros son **públicos por defecto**. Así en `main` se puede escribir `original.id = 42` directamente. |
| 8-10 | Tres miembros de tipos distintos. Da igual cuáles sean; el enunciado solo exige que no esté vacía. Se usan tipos variados para que se vea claramente al imprimir que se recupera todo. |

---

## 5. Serializer.hpp línea a línea

```cpp
1  #ifndef SERIALIZER_HPP
2  #define SERIALIZER_HPP
3
4  #include <stdint.h>
5  #include "Data.hpp"
6
7  class Serializer
8  {
9  public:
10     static uintptr_t serialize(Data* ptr);
11     static Data*     deserialize(uintptr_t raw);
12
13 private:
14     Serializer();
15     Serializer(Serializer const&);
16     Serializer& operator=(Serializer const&);
17     ~Serializer();
18 };
19
20 #endif
```

| Línea | Qué hace |
|-------|----------|
| 4 | `<stdint.h>` define `uintptr_t`. Se usa la versión C (`stdint.h`) porque `<cstdint>` es de C++11 y estamos en C++98. |
| 5 | Necesitamos saber qué es `Data` para usar `Data*`. |
| 10 | `serialize`: recibe un puntero a `Data`, devuelve un entero sin signo. |
| 11 | `deserialize`: la operación inversa. |
| 10-11 | `static` → se llaman como `Serializer::serialize(...)`, sin objeto. |
| 13-17 | Constructores privados → la clase no se puede instanciar (igual que en ex00). |

### ¿Qué es `uintptr_t`?

- `u` = unsigned (sin signo, las direcciones nunca son negativas)
- `int` = entero
- `ptr` = del tamaño de un puntero
- `_t` = convención para "tipo"

Es un entero **garantizado** a ser lo bastante grande como para guardar
cualquier puntero sin perder bits:

| Sistema | `sizeof(void*)` | `sizeof(uintptr_t)` | `sizeof(int)` |
|---------|-----------------|---------------------|---------------|
| 32 bits | 4 bytes         | 4 bytes             | 4 bytes       |
| 64 bits | 8 bytes         | 8 bytes             | **4 bytes**   |

En 64 bits, un `int` no basta: perderías la mitad de la dirección.

---

## 6. Serializer.cpp línea a línea

```cpp
1  #include "Serializer.hpp"
2
3  uintptr_t Serializer::serialize(Data* ptr)
4  {
5      return reinterpret_cast<uintptr_t>(ptr);
6  }
7
8  Data* Serializer::deserialize(uintptr_t raw)
9  {
10     return reinterpret_cast<Data*>(raw);
11 }
```

**Línea 5 — `reinterpret_cast<uintptr_t>(ptr)`**

Lee los bits del puntero y los entrega tal cual como un `uintptr_t`. No hay
cálculo, ni redondeo, ni copia de los datos apuntados.

```
ptr  (Data*)     = 0x00007ffc1d2a73f0
       │ mismos bits, distinto tipo
       ▼
raw  (uintptr_t) = 140720797807600
```

**Línea 10 — `reinterpret_cast<Data*>(raw)`**

El camino inverso: toma el número y dice "a partir de ahora, esto es la
dirección de un `Data`". El compilador te cree (no comprueba nada). Si el
número venía de un `Data*` real, el resultado es exactamente el puntero
original.

> No se definen los constructores privados: como nadie los usa, no hace falta
> darles cuerpo.

---

## 7. main.cpp línea a línea

```cpp
1  #include "Serializer.hpp"
2  #include <iostream>
3
4  int main()
5  {
6      Data original;
7      original.id    = 42;
8      original.label = "hello from 42";
9      original.value = 3.14;
10
11     std::cout << "Original pointer : " << &original << std::endl;
12     std::cout << "  id    = " << original.id    << std::endl;
13     std::cout << "  label = " << original.label << std::endl;
14     std::cout << "  value = " << original.value << std::endl;
15
16     uintptr_t raw = Serializer::serialize(&original);
17     std::cout << "\nSerialized (uintptr_t): " << raw << std::endl;
18
19     Data* recovered = Serializer::deserialize(raw);
20     std::cout << "\nRecovered pointer: " << recovered << std::endl;
21     std::cout << "  id    = " << recovered->id    << std::endl;
22     std::cout << "  label = " << recovered->label << std::endl;
23     std::cout << "  value = " << recovered->value << std::endl;
24
25     std::cout << "\nPointers match: " << (&original == recovered ? "YES" : "NO") << std::endl;
26
27     return 0;
28 }
```

| Línea | Qué hace | Relación con el resto |
|-------|----------|-----------------------|
| 6 | Crea un `Data` en la **pila** (stack). | Es el objeto cuya dirección vamos a serializar. |
| 7-9 | Rellena sus campos con valores reconocibles. | Para poder comprobar después que se recuperan iguales. |
| 11 | `&original` = dirección del objeto. `cout` imprime punteros en hexadecimal. | Valor "antes". |
| 12-14 | Imprime los campos con `.` (porque `original` es un objeto, no un puntero). | |
| 16 | **Serializa**: dirección → número. | Llama a `Serializer.cpp:5`. |
| 17 | Imprime el número. `cout` imprime `uintptr_t` en **decimal**. | Por eso parece distinto al de la línea 11, pero es el mismo valor. |
| 19 | **Deserializa**: número → `Data*`. | Llama a `Serializer.cpp:10`. |
| 20 | Imprime el puntero recuperado (hex de nuevo). | Debe coincidir con la línea 11. |
| 21-23 | Accede a los campos con `->` (porque `recovered` es un puntero). `p->id` es lo mismo que `(*p).id`. | Si salen 42 / "hello from 42" / 3.14, el puntero es válido. |
| 25 | Compara las dos direcciones con `==`. Ternario: si son iguales → `"YES"`. | La prueba definitiva del ejercicio. |

---

## 8. Salida real y cómo leerla

```
Original pointer : 0x7ffc1d2a73f0
  id    = 42
  label = hello from 42
  value = 3.14

Serialized (uintptr_t): 140720797807600

Recovered pointer: 0x7ffc1d2a73f0
  id    = 42
  label = hello from 42
  value = 3.14

Pointers match: YES
```

Comprobación a mano (puedes hacerlo en una terminal):

```
$ printf "%d\n" 0x7ffc1d2a73f0
140720797807600
```

El hexadecimal de arriba y el decimal del medio son **el mismo número**.

> La dirección concreta cambia en cada ejecución (por ASLR, una medida de
> seguridad del sistema operativo que aleatoriza las direcciones), pero
> siempre coinciden entre sí dentro de una misma ejecución.

---

## 9. ¿Por qué no `static_cast`? ¿Por qué no `int`?

### `static_cast` no compila

```cpp
uintptr_t raw = static_cast<uintptr_t>(ptr);   // ERROR de compilación
```

`static_cast` solo permite conversiones que "tienen sentido" para el
compilador (números entre sí, punteros dentro de una jerarquía de clases...).
Un puntero y un entero son cosas de naturaleza distinta, así que la única
herramienta que lo permite es `reinterpret_cast`: "sé lo que hago, reinterpreta
los bits".

### `int` pierde información en 64 bits

```cpp
int raw = reinterpret_cast<int>(ptr);   // ERROR en 64 bits: pérdida de precisión
```

Un puntero de 8 bytes no cabe en un `int` de 4 bytes. `uintptr_t` siempre
tiene el tamaño adecuado.

---

## 10. Peligros de reinterpret_cast

`reinterpret_cast` no comprueba nada. Esto compila, pero es **comportamiento
indefinido**:

```cpp
Data* p = Serializer::deserialize(12345);   // dirección inventada
std::cout << p->id;                          // crash (segfault) o basura
```

Otro caso peligroso: el objeto deja de existir.

```cpp
uintptr_t raw;
{
    Data tmp;
    raw = Serializer::serialize(&tmp);
}   // tmp se destruye aquí
Data* p = Serializer::deserialize(raw);  // p apunta a memoria ya liberada
```

Por eso este tipo de "serialización" solo vale **dentro del mismo proceso y
mientras el objeto exista**. No sirve para guardar en un fichero y leerlo en
otra ejecución (la dirección ya no significaría nada). La serialización "real"
(JSON, binario...) copia los **datos**, no la dirección.

---

## 11. Preguntas típicas de evaluación

- **¿Qué hace `reinterpret_cast`?** Reinterpreta los bits de un valor como
  otro tipo, sin convertirlos.
- **¿Por qué `uintptr_t` y no `int` o `long`?** Porque está garantizado que
  tiene el tamaño de un puntero en cualquier arquitectura.
- **¿Se copian los datos de `Data`?** No. Solo viaja la dirección; el objeto
  sigue en el mismo sitio.
- **¿Por qué el número serializado parece distinto al puntero?** Es el mismo
  valor; uno se imprime en hexadecimal y otro en decimal.
- **¿Por qué `struct` y no `class`?** Por comodidad: miembros públicos por
  defecto. Funcionaría igual con `class` y `public:`.

---

## 12. Resumen de conceptos

| Concepto | Qué es |
|----------|--------|
| **Puntero** | Variable que guarda una dirección de memoria |
| **&variable** | Operador que da la dirección de una variable |
| **p->campo** | Acceder a un campo a través de un puntero (`(*p).campo`) |
| **reinterpret_cast** | Reinterpreta los bits de un tipo como otro, sin conversión ni comprobación |
| **uintptr_t** | Entero sin signo del tamaño exacto de un puntero |
| **stdint.h** | Header de C con los tipos de tamaño fijo (C++98 no tiene `<cstdint>`) |
| **struct** | Como `class` pero con miembros públicos por defecto |
| **Serialización** | Convertir algo a un formato almacenable/transmisible y poder recuperarlo |
| **ASLR** | Aleatorización de direcciones: por eso cambian en cada ejecución |
