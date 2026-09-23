# CPP08 — Contenedores STL, iteradores y algoritmos

---

## ¿Qué es la STL?

Standard Template Library — biblioteca estándar de C++ con:
- **Contenedores**: estructuras que guardan datos (vector, list, map, stack, deque...)
- **Algoritmos**: funciones que operan sobre contenedores (sort, find, min, max...)
- **Iteradores**: "punteros inteligentes" que recorren contenedores

---

## ex00 — easyfind (template + std::find)

```cpp
template <typename T>
typename T::iterator easyfind(T& container, int val)
{
    typename T::iterator it = std::find(container.begin(), container.end(), val);
    if (it == container.end())
        throw std::runtime_error("value not found in container");
    return it;
}
```

- `typename T::iterator` → el tipo iterador del contenedor T.
  Para `vector<int>` sería `vector<int>::iterator`.
  Necesita `typename` porque el compilador no sabe si `T::iterator` es un tipo
  o un valor estático hasta que se instancia el template.
- `std::find(begin, end, val)` → busca `val` en el rango [begin, end).
  Devuelve un iterador al elemento encontrado, o `end` si no lo encuentra.
- `container.end()` → iterador "más allá del último elemento". NO apunta a nada válido,
  solo sirve como centinela para indicar "no encontrado".

---

## ex01 — Span (distancias entre números)

```cpp
class Span {
    unsigned int     _maxSize;
    std::vector<int> _numbers;
};
```

Usa `std::vector<int>` internamente para guardar los números.

### shortestSpan()

```cpp
int Span::shortestSpan() const
{
    std::vector<int> sorted(_numbers);
    std::sort(sorted.begin(), sorted.end());

    int shortest = INT_MAX;
    for (size_t i = 1; i < sorted.size(); i++)
    {
        int diff = sorted[i] - sorted[i - 1];
        if (diff < shortest) shortest = diff;
    }
    return shortest;
}
```

Estrategia: ordenar primero, luego la diferencia mínima está siempre entre
dos elementos adyacentes. Sin ordenar habría que comparar todos los pares: O(n²).
Ordenando: O(n log n) + O(n) = O(n log n). Mucho más eficiente.

`INT_MAX` → el entero máximo posible. Empezamos con él para que cualquier
diferencia real sea menor.

### longestSpan()

```cpp
int Span::longestSpan() const
{
    int minVal = *std::min_element(_numbers.begin(), _numbers.end());
    int maxVal = *std::max_element(_numbers.begin(), _numbers.end());
    return maxVal - minVal;
}
```

La distancia máxima siempre es entre el mínimo y el máximo global.
`std::min_element` y `std::max_element` devuelven ITERADORES → desreferenciamos con `*`.

### addRange() con iteradores

```cpp
template <typename Iterator>
void addRange(Iterator begin, Iterator end)
{
    while (begin != end)
    {
        addNumber(*begin);
        ++begin;
    }
}
```

Acepta CUALQUIER par de iteradores compatibles (de vector, list, array, etc.).
`*begin` → desreferencia para obtener el valor. `++begin` → avanza al siguiente.

---

## ex02 — MutantStack (herencia de contenedor STL)

```cpp
template <typename T>
class MutantStack : public std::stack<T>
{
public:
    typedef typename std::stack<T>::container_type::iterator iterator;

    iterator begin() { return this->c.begin(); }
    iterator end()   { return this->c.end();   }
};
```

`std::stack` internamente usa `std::deque` como contenedor (llamado `c`).
`std::stack` no expone iteradores porque no es un contenedor iterable por diseño.
Nosotros heredamos de él y exponemos `begin()` y `end()` del contenedor interno `c`.

`this->c` → accede al miembro protegido `c` (el contenedor interno) de `std::stack`.
Sin `this->`, el compilador no sabría que `c` viene de la clase base template.

`typedef` → crea un alias de tipo. Permite escribir `MutantStack<int>::iterator`
en vez de `MutantStack<int>::std::stack<int>::container_type::iterator`.

---

## Resumen de contenedores

| Contenedor | Tipo | Acceso | Inserción | Uso típico |
|------------|------|--------|-----------|------------|
| vector | Secuencial | O(1) por índice | O(1) al final | Array dinámico |
| list | Secuencial | O(n) | O(1) en cualquier posición | Inserción frecuente |
| deque | Secuencial | O(1) | O(1) al inicio y final | Cola doble |
| map | Asociativo | O(log n) por clave | O(log n) | Clave-valor ordenado |
| stack | Adaptador | Solo top | O(1) | LIFO (último en entrar, primero en salir) |

---

## Conceptos nuevos

| Concepto | Qué es |
|----------|--------|
| **Iterator** | "Puntero inteligente" que recorre un contenedor |
| **begin() / end()** | Iteradores al primero y "más allá del último" elemento |
| **std::find** | Busca un valor en un rango, devuelve iterador |
| **std::sort** | Ordena un rango usando quicksort/heapsort híbrido |
| **std::min/max_element** | Encuentra el mínimo/máximo en un rango |
| **typename T::iterator** | Tipo iterador de un contenedor template (necesita typename) |
| **typedef** | Crea un alias de tipo para simplificar declaraciones |
| **this->c** | Acceso a miembro de clase base template desde subclase |
