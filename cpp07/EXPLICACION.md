# CPP07 — Templates (Plantillas)

---

## ¿Qué es un template?

Un template es un "molde" de código genérico. En vez de escribir la misma función
para `int`, `double`, `string`, etc., escribes UNA función con un tipo genérico `T`
y el compilador genera la versión concreta al usarla.

---

## ex00 — swap, min, max

```cpp
template <typename T>
void swap(T& a, T& b)
{
    T tmp = a;
    a = b;
    b = tmp;
}
```

`template <typename T>` → "esta función funciona para cualquier tipo T".
`T tmp = a` → crea una variable temporal del tipo que sea T.
`T& a, T& b` → referencias al tipo T. Las referencias son obligatorias:
sin `&`, copiaríamos los valores y el swap no afectaría a las variables originales.

```cpp
template <typename T>
T const& min(T const& a, T const& b)
{
    return (b < a) ? b : a;
}
```

Devuelve `T const&` (referencia constante). ¿Por qué no `T`?
- Si devolviéramos `T`, haríamos una COPIA del valor → ineficiente para strings, etc.
- Con `T const&` devolvemos una referencia al original, sin copia.
- `const` porque no permitimos modificar el valor devuelto.

Están definidas en el .hpp porque los templates DEBEN estar en headers.
El compilador necesita ver el código del template para generar las especializaciones.

---

## ex01 — iter (función que aplica otra función a un array)

```cpp
template <typename T, typename F>
void iter(T* arr, std::size_t len, F func)
{
    for (std::size_t i = 0; i < len; i++)
        func(arr[i]);
}
```

Dos parámetros de tipo: `T` (el tipo del array) y `F` (el tipo de la función).
`F func` puede ser cualquier callable: una función, un functor, etc.
`std::size_t` es el tipo correcto para índices de arrays (entero sin signo, tamaño de puntero).

```cpp
// Uso:
iter(arr, 5, printElement<int>);  // pasa función como parámetro
```

`printElement<int>` → versión concreta de la función template para int.
El resultado es un recorrido de todo el array aplicando esa función a cada elemento.

---

## ex02 — Array (clase template)

```cpp
template <typename T>
class Array
{
public:
    Array(unsigned int n) : _data(new T[n]()), _size(n) {}
    // ...
};
```

`new T[n]()` → crea un array de n elementos de tipo T, inicializados por defecto.
El `()` al final llama al constructor por defecto de T (0 para int, "" para string, etc.).

### operator[] con bounds checking

```cpp
T& operator[](unsigned int idx)
{
    if (idx >= _size)
        throw std::out_of_range("index out of bounds");
    return _data[idx];
}
```

A diferencia de arrays normales de C, este lanza excepción al acceder fuera de rango.
Con arrays C++ normales `arr[-2]` es undefined behavior (se corrompe memoria).
Aquí detectamos el error y lanzamos.

Versión `const` también necesaria (para arrays constantes):
```cpp
T const& operator[](unsigned int idx) const { ... }
```

### Deep copy en operator=

```cpp
Array& operator=(Array const& other)
{
    if (this != &other)
    {
        delete[] _data;              // libera memoria antigua
        _size = other._size;
        _data = new T[_size]();      // aloca nueva memoria
        for (unsigned int i = 0; i < _size; i++)
            _data[i] = other._data[i]; // copia elemento a elemento
    }
    return *this;
}
```

"Deep copy" (copia profunda): copiamos los DATOS, no el puntero.
Si solo copiáramos el puntero (shallow copy), ambos arrays apuntarían al mismo
bloque de memoria, y al modificar uno se modificaría el otro.

---

## Por qué los templates van en .hpp

El compilador necesita el CÓDIGO COMPLETO del template para generar
cada especialización concreta (`Array<int>`, `Array<string>`, etc.).
Si el código estuviera en un .cpp separado, el compilador no lo vería
al compilar otros archivos y daría error de "undefined reference".

Excepción: se pueden separar en ficheros `.tpp` que se incluyen al final del `.hpp`.

---

## Conceptos nuevos

| Concepto | Qué es |
|----------|--------|
| **template** | Código genérico que funciona para cualquier tipo |
| **typename T** | Parámetro de tipo — puede llamarse como quieras |
| **T const&** | Referencia constante a T — evita copias |
| **size_t** | Tipo sin signo para tamaños/índices |
| **new T[n]()** | Aloca array con inicialización por defecto |
| **delete[]** | Libera arrays alojados con new[] |
| **Deep copy** | Copiar los datos, no solo el puntero |
| **out_of_range** | Excepción estándar para accesos fuera de rango |
