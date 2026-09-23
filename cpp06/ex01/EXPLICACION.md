# CPP06 — ex01: Serializer (reinterpret_cast)

---

## ¿Qué es serializar?

Serializar = convertir un objeto/puntero a un formato que se puede guardar o transmitir.
Deserializar = recuperar el objeto original desde ese formato.
Aquí lo hacemos con un puntero → número entero → puntero.

---

## uintptr_t — el tipo que guarda punteros como enteros

```cpp
#include <stdint.h>
uintptr_t serialize(Data* ptr);
```

`uintptr_t` es un entero sin signo garantizado de ser lo suficientemente grande
para guardar cualquier puntero. En sistemas de 64 bits son 8 bytes, en 32 bits son 4.
Sin este tipo, guardar un puntero en un int podría perder información.

---

## reinterpret_cast — reinterpretar bytes

```cpp
uintptr_t Serializer::serialize(Data* ptr)
{
    return reinterpret_cast<uintptr_t>(ptr);
}

Data* Serializer::deserialize(uintptr_t raw)
{
    return reinterpret_cast<Data*>(raw);
}
```

`reinterpret_cast<uintptr_t>(ptr)`:
- Toma la dirección de memoria del puntero (ej: 0x7ffdf5da4620).
- La interpreta como un número entero sin signo.
- NO copia ni convierte los datos: solo dice "trátame como este otro tipo".

`reinterpret_cast<Data*>(raw)`:
- Toma ese número entero.
- Lo interpreta como una dirección de memoria a un objeto Data.
- Si el número es la dirección válida de un Data, funciona perfectamente.

---

## La prueba: mismo puntero antes y después

```cpp
Data original;
original.id = 42;

uintptr_t raw = Serializer::serialize(&original);
Data* recovered = Serializer::deserialize(raw);

std::cout << (&original == recovered ? "YES" : "NO") << std::endl; // YES
```

`&original` → dirección de memoria de `original`.
Después de serialize + deserialize, `recovered` apunta a la misma dirección.
Son el mismo objeto en memoria → los campos son idénticos.

---

## La struct Data

```cpp
struct Data
{
    int         id;
    std::string label;
    double      value;
};
```

`struct` es como una clase pero con `public` por defecto.
Aquí solo la usamos para tener algo que serializar.
El enunciado dice explícitamente que no puede estar vacía (debe tener miembros).

---

## Conceptos nuevos

| Concepto | Qué es |
|----------|--------|
| **reinterpret_cast** | Reinterpreta los bytes de un tipo como otro tipo sin conversión |
| **uintptr_t** | Entero suficientemente grande para guardar cualquier puntero |
| **struct** | Como class pero atributos públicos por defecto |
| **Serialización** | Convertir un objeto a un formato transmisible/almacenable |
| **&variable** | Dirección de memoria donde está guardada esa variable |
