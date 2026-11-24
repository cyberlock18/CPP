# CPP09 — STL avanzado

Cada ejercicio tiene su propia explicación detallada, línea por línea:

| Ejercicio | Contenedor | Explicación |
|---|---|---|
| ex00 — Bitcoin Exchange | `std::map` | [ex00/EXPLICACION.md](ex00/EXPLICACION.md) |
| ex01 — RPN | `std::stack` sobre `std::list` | [ex01/EXPLICACION.md](ex01/EXPLICACION.md) |
| ex02 — PmergeMe (Ford‑Johnson) | `std::vector` + `std::deque` | [ex02/EXPLICACION.md](ex02/EXPLICACION.md) |

Cada documento incluye: qué pide el enunciado y dónde se cumple, el flujo del
programa, cada fichero explicado línea a línea, trazas paso a paso, salidas
reales, decisiones de diseño y preguntas típicas de evaluación.

---

## Regla clave del módulo

**Cada contenedor solo se puede usar en UN ejercicio.** Una vez usado, queda
prohibido en los siguientes:

```
ex00  std::map            → ya no se puede usar en ex01 ni ex02
ex01  std::stack + std::list → ya no se pueden usar en ex02
ex02  std::vector + std::deque
```

`std::stack` es un adaptador: por defecto guarda los datos en un `std::deque`.
Como ex02 usa `std::deque`, en ex01 se declara
`std::stack<int, std::list<int> >` para que el stack use `std::list` por
dentro y ningún contenedor se repita entre ejercicios.

---

## Conceptos que aparecen en todo el módulo

| Concepto | Qué es | Dónde |
|---|---|---|
| **Contenedor** | Clase de la STL que guarda una colección de elementos | Todos |
| **Adaptador de contenedor** | Interfaz restringida sobre otro contenedor (`stack` usa `deque` por debajo) | ex01 |
| **Iterador** | Objeto que apunta a un elemento y se mueve por el contenedor (`++it`, `*it`) | ex00, ex02 |
| **`const_iterator`** | Iterador que no permite modificar el elemento; obligatorio en métodos `const` | ex00, ex02 |
| **`lower_bound`** | Primer elemento `>=` que un valor (búsqueda binaria, O(log n)) | ex00, ex02 |
| **`upper_bound`** | Primer elemento `>` que un valor | ex02 |
| **`std::ifstream` + `getline`** | Leer un fichero línea a línea | ex00 |
| **`std::istringstream`** | Stream sobre un string para trocearlo con `>>` | ex01, ex02 |
| **`std::runtime_error`** | Excepción con mensaje; se captura como `std::exception&` | ex01, ex02 |
| **`clock()` / `CLOCKS_PER_SEC`** | Tiempo de CPU para medir rendimiento | ex02 |
| **Forma canónica ortodoxa** | Constructor, copia, `operator=`, destructor | Todas las clases |

## Complejidades de un vistazo

| Operación | `map` | `stack` | `vector` | `deque` |
|---|---|---|---|---|
| Acceso por índice | — | — | O(1) | O(1) |
| Buscar por clave | O(log n) | — | O(n) / O(log n) si está ordenado | ídem |
| Insertar | O(log n) | O(1) `push` | O(1) final, O(n) medio | O(1) extremos, O(n) medio |
| Eliminar | O(log n) | O(1) `pop` | O(1) final, O(n) medio | O(1) extremos, O(n) medio |
