# CPP09 — STL avanzado

Regla clave: cada contenedor solo se puede usar UNA VEZ en todo el módulo.
- ex00 BitcoinExchange: `std::map`
- ex01 RPN: `std::stack`
- ex02 PmergeMe: `std::vector` + `std::deque`

---

## ex00 — Bitcoin Exchange

### std::map — diccionario ordenado por clave

```cpp
std::map<std::string, double> _db;
```

`map` guarda pares clave-valor, ordenados por clave (alfabéticamente para strings).
Internamente es un árbol binario balanceado (red-black tree).
Búsqueda: O(log n). Inserción: O(log n).

```cpp
_db[date] = rate;   // inserta o actualiza
```

### lower_bound — búsqueda de fecha más cercana

```cpp
double BitcoinExchange::getRate(std::string const& date) const
{
    std::map<std::string, double>::const_iterator it = _db.lower_bound(date);
    if (it == _db.end() || it->first != date)
    {
        if (it == _db.begin()) return 0.0;
        --it;
    }
    return it->second;
}
```

`lower_bound(date)` → iterador al primer elemento cuya clave >= date.
Si la fecha exacta no está, apunta a la siguiente → retrocedemos con `--it`
para obtener la fecha anterior (la más cercana por debajo).
`it->first` → la clave (fecha). `it->second` → el valor (tasa).

---

## ex01 — RPN (Notación Polaca Inversa)

### ¿Qué es RPN?

En RPN los operadores van DESPUÉS de los operandos.
`3 4 +` = 3 + 4 = 7.  `8 9 *` = 72.
Se evalúa con una pila (stack): números se apilan, operadores sacan dos, operan, apilan resultado.

### std::stack — pila LIFO

```cpp
std::stack<int> st;
st.push(8);      // pila: [8]
st.push(9);      // pila: [8, 9]
int b = st.top(); st.pop();  // b=9, pila: [8]
int a = st.top(); st.pop();  // a=8, pila: []
st.push(a * b);              // pila: [72]
```

```cpp
// Parsing con istringstream
std::istringstream ss(expression);
std::string token;
while (ss >> token) { ... }
```

`std::istringstream` → stream de entrada sobre un string. `>>` extrae tokens
separados por espacios. Muy conveniente para parsear expresiones.

---

## ex02 — PmergeMe (Ford-Johnson / Merge-Insert Sort)

### ¿Por qué Ford-Johnson?

Es el algoritmo de ordenación que minimiza el número de COMPARACIONES.
No es el más rápido en tiempo de pared, pero hace menos comparaciones que mergesort.
Fue el óptimo teórico para n pequeño durante décadas.

### Pasos del algoritmo

**1. Formar pares y ordenarlos**
```
[3, 5, 9, 7, 4]
Pares: (3,5) (7,9)  Straggler: 4
Dentro de cada par, el menor va primero: (3,5) (7,9)
```

**2. Extraer los mayores y ordenarlos recursivamente**
```
Mayores: [5, 9]
Recursión → ya ordenados: [5, 9]
```

**3. Construir la cadena principal (main chain)**
```
[a0, b0, b1] = [3, 5, 9]
(a0=3 es el menor del par con el menor mayor → ya sabemos que 3 ≤ 5)
```

**4. Insertar elementos pendientes (pend) en orden Jacobsthal**
```
pend = [7]  (el menor del par (7,9))
Insertamos 7 buscando solo hasta la posición de su par mayor (9):
[3, 5, 7, 9]
```

**5. Insertar el straggler**
```
Insertar 4 en la cadena: [3, 4, 5, 7, 9]
```

### Los números de Jacobsthal

```cpp
// 0, 1, 3, 5, 11, 21, 43, 85, ...
// t(n) = t(n-1) + 2 * t(n-2)
```

Definen en qué orden insertar los pend para minimizar comparaciones.
El orden es: [0, 2, 1, 4, 3, 10, 9, 8, 7, 6, 5, ...]
(grupos de mayor a menor dentro de cada grupo)

### std::vector vs std::deque

Ambos son secuencias de acceso aleatorio. Diferencias:
- `vector`: bloque continuo de memoria. `push_back` muy rápido. Inserción en medio lenta.
- `deque`: bloques de memoria conectados. `push_front` y `push_back` rápidos.

Para Ford-Johnson ambos funcionan igual. El subject pide comparar sus velocidades.

### std::binary_search vs lower_bound/upper_bound

```cpp
// Para insertar manteniendo orden:
std::lower_bound(begin, limit, val)
// → iterador a la primera posición donde val puede ir (manteniendo orden)

std::upper_bound(begin, end, val)
// → iterador a la primera posición DESPUÉS de val
```

`limit` en nuestro caso es la posición del par mayor del elemento que insertamos.
Sabemos que pend[i] ≤ sorted[i+1].second, así que no necesitamos buscar más allá.
Esto reduce el número de comparaciones (clave del algoritmo).

---

## Timing con clock()

```cpp
clock_t start = clock();
sortVector();
clock_t end = clock();

double us = static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000000.0;
```

`clock()` devuelve el número de "tics" del procesador desde el inicio del programa.
`CLOCKS_PER_SEC` es cuántos tics hay por segundo.
`tics / CLOCKS_PER_SEC * 1000000` → microsegundos.

---

## Conceptos nuevos

| Concepto | Qué es |
|----------|--------|
| **std::map** | Diccionario ordenado clave-valor, O(log n) |
| **lower_bound** | Primer elemento >= valor buscado |
| **std::stack** | Pila LIFO (Last In First Out) |
| **istringstream** | Stream de entrada sobre un string para parsear |
| **std::vector** | Array dinámico de memoria contigua |
| **std::deque** | Array dinámico con inserciones eficientes en ambos extremos |
| **Ford-Johnson** | Algoritmo que minimiza el número de comparaciones al ordenar |
| **Jacobsthal** | Secuencia que define el orden óptimo de inserción en Ford-Johnson |
| **clock()** | Función para medir tiempo de CPU |
