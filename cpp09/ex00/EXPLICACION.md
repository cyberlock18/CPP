# CPP09 — ex00: Bitcoin Exchange (`std::map`)

> Objetivo de este documento: que entiendas **qué hace cada línea**, **por qué
> está ahí** y **qué pasa con cada línea del fichero de entrada**. Al final hay
> trazas paso a paso, salidas reales del programa y las preguntas típicas de
> evaluación.

---

## Índice

1. [Qué pide el ejercicio](#1-qué-pide-el-ejercicio)
2. [Mapa de ficheros y flujo del programa](#2-mapa-de-ficheros-y-flujo-del-programa)
3. [Por qué `std::map`](#3-por-qué-stdmap)
4. [main.cpp línea a línea](#4-maincpp-línea-a-línea)
5. [BitcoinExchange.hpp línea a línea](#5-bitcoinexchangehpp-línea-a-línea)
6. [BitcoinExchange.cpp línea a línea](#6-bitcoinexchangecpp-línea-a-línea)
7. [Traza completa con `input.txt`](#7-traza-completa-con-inputtxt)
8. [Tabla de pruebas (salida real)](#8-tabla-de-pruebas-salida-real)
9. [Decisiones de diseño](#9-decisiones-de-diseño)
10. [Makefile](#10-makefile)
11. [Preguntas típicas de evaluación](#11-preguntas-típicas-de-evaluación)

---

## 1. Qué pide el ejercicio

Un programa `btc` que recibe **un fichero** como argumento. Cada línea del
fichero tiene el formato `fecha | valor` (una cantidad de bitcoins en una
fecha). Por cada línea hay que imprimir:

```
fecha => cantidad = cantidad * precio_del_bitcoin_en_esa_fecha
```

El precio sale de una **base de datos CSV** (`data.csv`) que da el enunciado,
con líneas `fecha,precio`.

### Lo que dice exactamente el PDF (y cómo se cumple)

| Frase del enunciado | Qué implica | Dónde se cumple |
|---|---|---|
| *"The program name is btc"* | El ejecutable se llama `btc` | `Makefile`: `NAME = btc` |
| *"must take a file as an argument"* | `argc == 2` | `main.cpp` |
| Ejemplo `$> ./btc` → `Error: could not open file.` | Sin argumento, ese mensaje exacto | `main.cpp` |
| *"Each line ... format: "date \| value""* | Separador exacto `" \| "` (espacio, barra, espacio) | `processInput`: `line.find(" \| ")` |
| *"A valid date ... Year-Month-Day"* | `YYYY-MM-DD` y que la fecha exista | `isValidDate` (incluye días por mes y bisiestos) |
| *"A valid value must be either a float or a positive integer, between 0 and 1000"* | Solo números; rechazar negativos y > 1000 | `isNumber` + `isValidValue` |
| *"use at least one container"* | Usar un contenedor STL | `std::map<std::string, double> _db` |
| *"handle possible errors with an appropriate error message"* | Un mensaje para cada error | `bad input`, `not a positive number`, `too large a number`, `could not open file`, `no data for this date` |
| *"If the date ... does not exist in your DB ... use the closest date ... the lower date and not the upper one"* | Si no hay fecha exacta, usar la **anterior** | `getRate` con `lower_bound` + `--it` |
| *"The container(s) you use ... will no longer be usable for the rest of this module"* | `map` queda "gastado" | ex01 usa `stack`, ex02 `vector` + `deque` |

---

## 2. Mapa de ficheros y flujo del programa

```
 ┌────────────┐  #include   ┌──────────────────────┐
 │ main.cpp   │ ──────────► │ BitcoinExchange.hpp  │  declara la clase
 └─────┬──────┘             └──────────▲───────────┘
       │                               │ #include
       ▼                               │
 ┌───────────────────────────────────────────────────┐
 │ BitcoinExchange.cpp  (implementa los métodos)     │
 └───────────────────────────────────────────────────┘

 Ficheros de datos (en el directorio desde donde ejecutas):
   data.csv   → base de datos de precios (fecha,precio)
   input.txt  → fichero de entrada de ejemplo (fecha | cantidad)
```

Flujo de ejecución:

```
main
 ├─ ¿argc == 2? no → "Error: could not open file." y return 1
 ├─ btc.loadDatabase("data.csv")        → llena el map _db
 │     └─ no se abre o queda vacío → error y return 1
 └─ btc.processInput(argv[1])            → por cada línea del input:
        ├─ stripCR(line)                 → quita un '\r' final (ficheros Windows)
        ├─ separar "fecha | valor"       → si no hay " | "         → bad input
        ├─ isValidDate(fecha)            → si la fecha no existe   → bad input
        ├─ isValidValue(valor)           → no numérico → bad input
        │                                  < 0 → not a positive number
        │                                  > 1000 → too large a number
        ├─ getRate(fecha, rate)          → fecha exacta o la anterior
        │                                  anterior a toda la BD → no data for this date
        └─ imprime "fecha => amount = amount * rate"
```

---

## 3. Por qué `std::map`

```cpp
std::map<std::string, double> _db;
```

- Un `map` guarda pares **clave → valor**, **ordenados por clave** y sin
  claves repetidas. Internamente es un árbol rojo‑negro (árbol binario de
  búsqueda balanceado).
- Clave = la fecha como `std::string` (`"2011-01-03"`), valor = el precio
  (`0.30`).
- **Truco clave**: las fechas en formato `YYYY-MM-DD` se ordenan igual
  alfabéticamente que cronológicamente, porque todos los campos tienen ancho
  fijo y van de mayor a menor importancia (año, luego mes, luego día).
  `"2011-01-03" < "2011-06-01"` como string **y** como fecha. Por eso no hace
  falta convertir la fecha a número: comparar strings basta.
- Como el map está ordenado, podemos usar `lower_bound` para encontrar la
  fecha más cercana en O(log n), que es justo lo que pide el enunciado.

| Operación | Coste |
|---|---|
| `_db[date] = rate` (insertar/actualizar) | O(log n) |
| `_db.lower_bound(date)` (buscar) | O(log n) |
| Recorrer en orden | O(n) |

¿Por qué no un `vector` de pares? Habría que ordenarlo a mano y hacer la
búsqueda binaria a mano. El `map` ya da las dos cosas.

---

## 4. main.cpp línea a línea

```cpp
1  #include "BitcoinExchange.hpp"
```
Incluye la declaración de la clase. A través de ella también llega
`<iostream>` (el `.hpp` lo incluye), por eso aquí se puede usar `std::cerr`.

```cpp
3  int main(int argc, char** argv)
```
`argc` = número de argumentos (incluido el nombre del programa).
`argv[0]` = `"./btc"`, `argv[1]` = el fichero de entrada.

```cpp
5  if (argc != 2)
6  {
7      std::cerr << "Error: could not open file." << std::endl;
8      return 1;
9  }
```
Si no hay exactamente un argumento: mensaje por la **salida de error**
(`cerr`) y código de salida `1`. Es exactamente lo que muestra el ejemplo del
PDF (`$> ./btc` → `Error: could not open file.`).

```cpp
11 BitcoinExchange btc;
```
Crea el objeto. Se llama al constructor por defecto, que deja `_db` vacío.

```cpp
12 if (!btc.loadDatabase("data.csv"))
13     return 1;
```
Carga la base de datos. La ruta `"data.csv"` es **relativa**: se busca en el
directorio desde el que ejecutas el programa, no donde está el binario. Si
falla, `loadDatabase` ya imprime el error y devuelve `false`, y `main` sale
con `1`.

```cpp
15 btc.processInput(argv[1]);
16 return 0;
```
Procesa el fichero del usuario. `argv[1]` es un `char*`; se convierte
implícitamente a `std::string` porque el parámetro es `std::string const&`
(se crea un string temporal).

---

## 5. BitcoinExchange.hpp línea a línea

```cpp
#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP
...
#endif
```
**Include guard**: si el header se incluye dos veces en la misma unidad de
compilación, la segunda vez `BITCOINEXCHANGE_HPP` ya está definido y se salta
todo el contenido. Evita errores de "redefinición de clase".

```cpp
#include <map>       // std::map
#include <string>    // std::string
#include <iostream>  // std::cout, std::cerr, std::endl
```

```cpp
class BitcoinExchange
{
public:
    BitcoinExchange();                                        // constructor por defecto
    BitcoinExchange(BitcoinExchange const& other);            // constructor de copia
    BitcoinExchange& operator=(BitcoinExchange const& other); // asignación
    ~BitcoinExchange();                                       // destructor
```
**Forma canónica ortodoxa** (Coplien), obligatoria en todos los módulos CPP:
los 4 miembros especiales declarados explícitamente.

```cpp
    bool loadDatabase(std::string const& csvFile);
    void processInput(std::string const& inputFile) const;
```
- `loadDatabase`: lee `data.csv` y llena `_db`. **No es `const`** porque
  modifica `_db`.
- `processInput`: lee el fichero de entrada e imprime resultados. **Es
  `const`** porque solo lee `_db`.

```cpp
private:
    std::map<std::string, double> _db;
```
El único atributo: la base de datos fecha → precio.

```cpp
    static void stripCR(std::string& line);
    static bool isValidDate(std::string const& date);
    static bool isNumber(std::string const& val);
    static bool isValidValue(std::string const& val, std::string const& line, double& out);
    bool        getRate(std::string const& date, double& rate) const;
```
Funciones auxiliares **privadas**.
- Las cuatro primeras son **`static`**: no usan `_db` ni ningún atributo;
  solo trabajan con lo que reciben. Marcarlas `static` deja claro que no
  dependen del estado del objeto.
- `getRate` **no** puede ser `static` porque consulta `_db`; es `const`
  porque no lo modifica.
- `isValidValue` recibe `line` (la línea completa) para poder imprimir
  `Error: bad input => <línea>` cuando el valor no es numérico.
- `out` y `rate` son **parámetros de salida** (referencia no constante):
  la función devuelve `bool` (¿ha ido bien?) y deja el resultado ahí.

---

## 6. BitcoinExchange.cpp línea a línea

### 6.1 Includes (líneas 1‑3)

```cpp
#include "BitcoinExchange.hpp"
#include <fstream>   // std::ifstream (leer ficheros)
#include <cstdlib>   // std::atof, std::atoi, std::strtod
```

### 6.2 Forma canónica (líneas 5‑12)

```cpp
BitcoinExchange::BitcoinExchange() {}
```
Constructor por defecto. No hace nada: el `map` se construye vacío solo.

```cpp
BitcoinExchange::BitcoinExchange(BitcoinExchange const& other) : _db(other._db) {}
```
Constructor de copia. En la **lista de inicialización** copia el map del
otro objeto (`std::map` ya sabe copiarse: copia profunda de todos los pares).

```cpp
BitcoinExchange& BitcoinExchange::operator=(BitcoinExchange const& other)
{
    if (this != &other) _db = other._db;
    return *this;
}
```
- `this != &other`: protección contra la **autoasignación** (`a = a;`).
- `_db = other._db`: el `operator=` de `std::map` hace la copia.
- `return *this`: permite encadenar `a = b = c;`.

```cpp
BitcoinExchange::~BitcoinExchange() {}
```
Destructor vacío: el map libera su memoria solo (RAII). No hay `new`, así
que no hay nada que `delete`.

### 6.3 `stripCR` (líneas 15‑19)

```cpp
void BitcoinExchange::stripCR(std::string& line)
{
    if (!line.empty() && line[line.length() - 1] == '\r')
        line.erase(line.length() - 1);
}
```
Los ficheros guardados en Windows terminan cada línea con `\r\n`. `getline`
corta en `\n`, así que el `\r` se queda pegado al final de la línea
(`"2011-01-03 | 3\r"`) y el valor `"3\r"` dejaría de ser un número.
Esta función quita ese `\r` si existe.
- `!line.empty()` va **primero**: en una línea vacía, `line.length() - 1`
  sería un número enorme (`size_t` no tiene negativos) y se leería fuera.
- `erase(pos)` borra desde `pos` hasta el final.

### 6.4 `loadDatabase` (líneas 21‑48)

```cpp
std::ifstream file(csvFile.c_str());
```
Abre el fichero para lectura. En **C++98** el constructor de `ifstream` no
acepta `std::string`, solo `const char*`, por eso `.c_str()`.

```cpp
if (!file.is_open())
{
    std::cerr << "Error: could not open database." << std::endl;
    return false;
}
```
Si el fichero no existe o no hay permisos, error y `false`.

```cpp
std::string line;
std::getline(file, line); // skip header
```
`getline` lee una línea entera (hasta `\n`, que no se guarda). La primera
línea es la cabecera `date,exchange_rate` y se descarta.

```cpp
while (std::getline(file, line))
```
Bucle de lectura: `getline` devuelve el stream; convertido a `bool` es
`false` cuando llega al final del fichero (EOF) o hay error. Así el bucle
termina solo.

```cpp
    stripCR(line);
    if (line.empty()) continue;
```
Quita el `\r` de Windows e ignora líneas vacías.

```cpp
    size_t comma = line.find(',');
    if (comma == std::string::npos) continue;
```
Busca la coma. `find` devuelve la posición o `std::string::npos` (un valor
especial que significa "no encontrado"). Si no hay coma, la línea se ignora.

```cpp
    std::string date = line.substr(0, comma);
```
`substr(pos, len)`: desde la posición 0, `comma` caracteres → todo lo que hay
antes de la coma.
Ejemplo: `"2011-01-03,0.30"` → `comma = 10` → `date = "2011-01-03"`.

```cpp
    double rate = std::atof(line.substr(comma + 1).c_str());
```
`substr(comma + 1)` sin longitud = desde después de la coma hasta el final
→ `"0.30"`. `atof` lo convierte a `double` → `0.3`.
(La base de datos la da el enunciado y se considera fiable, por eso no se
valida con el mismo rigor que el input del usuario).

```cpp
    _db[date] = rate;
```
`operator[]` del map: si la clave no existe la **crea**, si existe la
**sobrescribe**. Después de esta línea, `_db["2011-01-03"] == 0.3`.

```cpp
if (_db.empty())
{
    std::cerr << "Error: database is empty." << std::endl;
    return false;
}
return true;
```
Si el CSV existe pero no tiene ninguna línea válida, no tiene sentido seguir:
todas las búsquedas fallarían. El `ifstream` se cierra solo al salir de la
función (su destructor llama a `close()`).

Estado de `_db` después de cargar el `data.csv` de este repo (29 entradas,
ya ordenadas por el map):

```
"2009-01-02" → 0
"2010-07-17" → 0.04
"2010-10-25" → 0.10
"2011-01-03" → 0.30
"2011-06-01" → 8.00
 ...
"2021-04-01" → 58000
"2021-11-01" → 61000
"2022-01-01" → 46000
"2022-06-01" → 29000
```

> ⚠️ El `data.csv` de este repo es una versión **reducida** (30 líneas). El
> del enunciado tiene muchas más fechas. En la evaluación lo normal es
> usar el `data.csv` oficial: por eso el ejemplo del PDF da
> `2011-01-09 => 1 = 0.32` y aquí, con fechas distintas, los números cambian.

### 6.5 `isValidDate` (líneas 50‑70)

```cpp
if (date.length() != 10) return false;
```
`YYYY-MM-DD` tiene exactamente 10 caracteres.

```cpp
if (date[4] != '-' || date[7] != '-') return false;
```
Posiciones de los guiones:
```
índice: 0 1 2 3 4 5 6 7 8 9
        2 0 1 1 - 0 1 - 0 3
```

```cpp
for (int i = 0; i < 10; i++)
{
    if (i == 4 || i == 7) continue;
    if (date[i] < '0' || date[i] > '9') return false;
}
```
Todos los demás caracteres tienen que ser dígitos. Se compara con `'0'` y
`'9'` porque en ASCII los dígitos son consecutivos (48…57).

```cpp
int year  = std::atoi(date.substr(0, 4).c_str());
int month = std::atoi(date.substr(5, 2).c_str());
int day   = std::atoi(date.substr(8, 2).c_str());
```
`substr(5, 2)` → `"01"` → `atoi` → `1`. Ya sabemos que son dígitos, así que
`atoi` no puede fallar.

```cpp
if (month < 1 || month > 12) return false;
```
Mes fuera de rango. Con `2001-42-42`: mes 42 → `false` → `bad input`.
Esta comprobación va **antes** de usar `month - 1` como índice en la tabla:
si no, `daysInMonth[41]` leería fuera del array.

```cpp
static const int daysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
int maxDay = daysInMonth[month - 1];
```
Tabla con los días de cada mes (enero = índice 0). `static const`: se crea
una sola vez para todo el programa, no en cada llamada.

```cpp
bool leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
if (month == 2 && leap)
    maxDay = 29;
return (day >= 1 && day <= maxDay);
```
**Año bisiesto** (calendario gregoriano):
- divisible entre 4 → bisiesto (2012, 2024)…
- …salvo si es divisible entre 100 → no (1900, 2100)…
- …salvo si es divisible entre 400 → sí (2000).

| Fecha | Resultado |
|---|---|
| `2012-02-29` | ✓ (2012 es bisiesto) |
| `2011-02-29` | ✗ |
| `1900-02-29` | ✗ (divisible entre 100, no entre 400) |
| `2000-02-29` | ✓ (divisible entre 400) |
| `2023-04-31` | ✗ (abril tiene 30) |
| `2012-02-30` | ✗ |

### 6.6 `isNumber` (líneas 74‑91)

```cpp
size_t i = 0;
if (i < val.length() && (val[i] == '-' || val[i] == '+'))
    i++;
```
Permite un signo opcional al principio. Se acepta el `-` a propósito: así
`-1` llega a `isValidValue` y se muestra el mensaje específico
`Error: not a positive number.` (el que pide el PDF), en vez de un
`bad input` genérico.

```cpp
bool digits = false;
bool dot = false;
for (; i < val.length(); i++)
{
    if (val[i] >= '0' && val[i] <= '9')
        digits = true;
    else if (val[i] == '.' && !dot)
        dot = true;
    else
        return false;
}
return digits;
```
Recorre el resto carácter a carácter:
- dígito → apunta que ha visto al menos uno;
- punto → solo se permite **uno** (`dot` pasa a `true`; un segundo punto
  cae en el `else`);
- cualquier otra cosa → no es un número.

Al final exige **al menos un dígito** (`"."`, `"-"` o `""` no son números).

| `val` | Resultado | Motivo |
|---|---|---|
| `"3"`, `"1.2"`, `".5"`, `"-1"` | ✓ | |
| `""` | ✗ | sin dígitos |
| `"-"` | ✗ | sin dígitos |
| `"abc"` | ✗ | carácter no válido |
| `"1.2.3"` | ✗ | dos puntos |
| `"1e3"` | ✗ | `e` no permitida (el PDF pide float o entero, no notación científica) |
| `" 5"` | ✗ | espacio |
| `"inf"`, `"nan"` | ✗ | letras |

¿Por qué no basta con `strtod`? Porque `strtod` es muy permisivo: acepta
espacios al principio, `1e3`, `inf`, `nan`, y con `""` no da error. Validar a
mano primero garantiza que solo pasan números con el formato esperado.

### 6.7 `isValidValue` (líneas 93‑112)

```cpp
if (!isNumber(val))
{
    std::cerr << "Error: bad input => " << line << std::endl;
    return false;
}
```
Formato incorrecto → `bad input` con la línea completa (por eso se pasa
`line`).

```cpp
out = std::strtod(val.c_str(), NULL);
```
Ahora sí se convierte. Como `isNumber` ya garantizó el formato, `strtod`
siempre convierte el texto entero; por eso se le pasa `NULL` en vez de un
puntero para saber dónde paró.

```cpp
if (out < 0)
{
    std::cerr << "Error: not a positive number." << std::endl;
    return false;
}
if (out > 1000)
{
    std::cerr << "Error: too large a number." << std::endl;
    return false;
}
return true;
```
Rango del enunciado: 0 a 1000 (ambos incluidos). Los mensajes son
exactamente los del PDF.
`2147483648` → `out = 2147483648.0` (cabe en un `double` sin problema) →
`> 1000` → `too large a number`. Al usar `double` no hay overflow de `int`.

### 6.8 `getRate` — la búsqueda de la fecha (líneas 115‑125)

Es la parte más preguntada en la evaluación.

```cpp
std::map<std::string, double>::const_iterator it = _db.lower_bound(date);
```
- `const_iterator` porque el método es `const` (no se puede obtener un
  iterador normal de un map constante).
- `lower_bound(date)` devuelve un iterador al **primer elemento cuya clave es
  `>= date`**.

```cpp
if (it == _db.end() || it->first != date)
{
    if (it == _db.begin()) return false;
    --it;
}
rate = it->second;
return true;
```
Cuatro casos posibles:

```
_db:  [2009-01-02] ... [2011-01-03] [2011-06-01] ... [2022-06-01]   end()

Caso A: fecha exacta, date = "2011-06-01"
        lower_bound → [2011-06-01]   it->first == date → no entra al if
        rate = 8.00

Caso B: fecha intermedia, date = "2011-01-09"
        lower_bound → [2011-06-01]   (primera >= 2011-01-09)
        it->first != date → entra al if → --it → [2011-01-03]
        rate = 0.30   ← la fecha ANTERIOR, como pide el enunciado

Caso C: fecha posterior a todas, date = "2030-01-01"
        lower_bound → end()          (no hay ninguna >= 2030)
        it == end() → entra → --it → [2022-06-01] (la última)
        rate = 29000

Caso D: fecha anterior a todas, date = "2008-01-01"
        lower_bound → begin()  [2009-01-02]
        it->first != date → entra → it == begin() → no hay fecha anterior
        return false → processInput muestra "Error: no data for this date"
```
- `it->first` = la clave (fecha), `it->second` = el valor (precio). El
  iterador de un map apunta a un `std::pair<const std::string, double>`.
- La comprobación `it == _db.begin()` es imprescindible: hacer `--it` sobre
  `begin()` es **comportamiento indefinido**.
- El orden de la condición `it == _db.end() || it->first != date` importa:
  si `it == end()`, el `||` corta y **no** se evalúa `it->first`
  (desreferenciar `end()` también sería comportamiento indefinido).
- Caso D: si no hay ninguna fecha anterior, no podemos inventarnos un precio.
  Devolver `false` permite avisar al usuario en vez de imprimir un resultado
  falso de `0`.

### 6.9 `processInput` (líneas 127‑171)

```cpp
std::ifstream file(inputFile.c_str());
if (!file.is_open())
{
    std::cerr << "Error: could not open file." << std::endl;
    return;
}
```
Mismo patrón que `loadDatabase`, con el mensaje exacto del PDF.

```cpp
std::string line;
std::getline(file, line); // skip header
while (std::getline(file, line))
{
    stripCR(line);
    if (line.empty()) continue;
```
Salta la cabecera `date | value`, quita el `\r` de Windows y salta líneas
vacías.

```cpp
    size_t sep = line.find(" | ");
    if (sep == std::string::npos)
    {
        std::cerr << "Error: bad input => " << line << std::endl;
        continue;
    }
```
Busca el separador **completo** `" | "` (3 caracteres). Si no está (por
ejemplo `2001-42-42` o `2012-01-11|1`), `bad input` con la línea entera.
`continue` pasa a la siguiente línea sin cortar el programa.

```cpp
    std::string date = line.substr(0, sep);
    std::string val  = line.substr(sep + 3);
```
```
line = "2011-01-03 | 1.2"
        0123456789012345
sep  = 10
date = substr(0, 10)  = "2011-01-03"
val  = substr(13)     = "1.2"      (10 + 3 = salta " | ")
```

```cpp
    if (!isValidDate(date))
    {
        std::cerr << "Error: bad input => " << line << std::endl;
        continue;
    }
```
Fecha mal formada o inexistente → `bad input`.

```cpp
    double amount;
    if (!isValidValue(val, line, amount))
        continue;
```
Si el valor no es válido, `isValidValue` ya imprimió el mensaje
correspondiente; aquí solo se salta la línea.

```cpp
    double rate;
    if (!getRate(date, rate))
    {
        std::cerr << "Error: no data for this date => " << date << std::endl;
        continue;
    }
```
Fecha anterior a la primera de la base de datos.

```cpp
    std::cout << date << " => " << amount << " = " << amount * rate << std::endl;
```
Resultado por la **salida estándar**. `std::cout` con un `double` usa por
defecto 6 cifras significativas y quita los ceros sobrantes: `0.3 * 3` →
`0.9`, `1.2 * 0.3` → `0.36`, `0.5 * 58000` → `29000`.

---

## 7. Traza completa con `input.txt`

```
$ ./btc input.txt
```

| Línea del input | Separar | isValidDate | isValidValue | getRate | Salida |
|---|---|---|---|---|---|
| `date \| value` | — | — | — | — | (cabecera, se salta) |
| `2011-01-03 \| 3` | `"2011-01-03"`, `"3"` | ✓ | 3 ✓ | exacta → 0.30 | `2011-01-03 => 3 = 0.9` |
| `2011-01-03 \| 2` | | ✓ | 2 ✓ | 0.30 | `2011-01-03 => 2 = 0.6` |
| `2011-01-03 \| 1` | | ✓ | 1 ✓ | 0.30 | `2011-01-03 => 1 = 0.3` |
| `2011-01-03 \| 1.2` | | ✓ | 1.2 ✓ | 0.30 | `2011-01-03 => 1.2 = 0.36` |
| `2011-06-01 \| 1` | | ✓ | 1 ✓ | exacta → 8.00 | `2011-06-01 => 1 = 8` |
| `2012-01-11 \| -1` | | ✓ | -1 → `< 0` ✗ | — | `Error: not a positive number.` |
| `2001-42-42` | no hay `" \| "` ✗ | — | — | — | `Error: bad input => 2001-42-42` |
| `2012-01-11 \| 1` | | ✓ | 1 ✓ | exacta → 7.10 | `2012-01-11 => 1 = 7.1` |
| `2012-01-11 \| 2147483648` | | ✓ | `> 1000` ✗ | — | `Error: too large a number.` |
| `2021-04-01 \| 0.5` | | ✓ | 0.5 ✓ | exacta → 58000 | `2021-04-01 => 0.5 = 29000` |
| `2030-01-01 \| 10` | | ✓ | 10 ✓ | `end()` → `--it` → 29000 | `2030-01-01 => 10 = 290000` |

Salida real:
```
2011-01-03 => 3 = 0.9
2011-01-03 => 2 = 0.6
2011-01-03 => 1 = 0.3
2011-01-03 => 1.2 = 0.36
2011-06-01 => 1 = 8
Error: not a positive number.
Error: bad input => 2001-42-42
2012-01-11 => 1 = 7.1
Error: too large a number.
2021-04-01 => 0.5 = 29000
2030-01-01 => 10 = 290000
```

> Los errores van por `stderr` y los resultados por `stdout`. Si haces
> `./btc input.txt 2>/dev/null` solo verás las líneas buenas.

---

## 8. Tabla de pruebas (salida real)

| Comando / línea | Salida | Qué se comprueba |
|---|---|---|
| `./btc` | `Error: could not open file.` (código 1) | Sin argumento (igual que el PDF) |
| `./btc noexiste.txt` | `Error: could not open file.` | Fichero inexistente |
| `2012-01-11 \| ` | `Error: bad input => 2012-01-11 \| ` | Valor vacío |
| `2012-01-11 \| abc` | `Error: bad input => 2012-01-11 \| abc` | Valor no numérico |
| `2012-01-11 \| 1e3` | `Error: bad input => ...` | Notación científica rechazada |
| `2012-01-11 \| nan` / `inf` | `Error: bad input => ...` | Pseudo‑valores rechazados |
| `2012-01-11 \|  5` | `Error: bad input => ...` | Espacio extra |
| `2012-01-11 \| 1.2.3` | `Error: bad input => ...` | Dos puntos |
| `2012-01-11 \| -` | `Error: bad input => ...` | Signo sin dígitos |
| `2012-01-11 \| .5` | `2012-01-11 => 0.5 = 3.55` | Float sin cero delante |
| `2012-01-11 \| 1000` | `2012-01-11 => 1000 = 7100` | 1000 está incluido |
| `2012-01-11\|1` | `Error: bad input => 2012-01-11\|1` | Falta el espacio alrededor de `\|` |
| `2012-02-29 \| 1` | `2012-02-29 => 1 = 7.1` | Bisiesto válido |
| `2011-02-29 \| 1` | `Error: bad input => ...` | No bisiesto |
| `1900-02-29 \| 1` | `Error: bad input => ...` | Regla del 100 |
| `2023-04-31 \| 1` | `Error: bad input => ...` | Abril tiene 30 días |
| `2012-02-30 \| 1` | `Error: bad input => ...` | Día imposible |
| `2008-01-01 \| 1` | `Error: no data for this date => 2008-01-01` | Anterior a la BD |
| `2009-01-02 \| 1` | `2009-01-02 => 1 = 0` | Primera fecha de la BD (precio real 0) |
| `2012-01-11 \| 2` con `\r\n` | `2012-01-11 => 2 = 14.2` | Fin de línea Windows |

---

## 9. Decisiones de diseño

Cosas que el enunciado no fija y hay que saber justificar:

- **Fecha anterior a la base de datos** → error `no data for this date`. El
  enunciado solo dice "usa la fecha inferior más cercana"; si no existe,
  inventar un precio (por ejemplo 0) daría un resultado falso.
- **Formato estricto del valor**: solo signo opcional, dígitos y un punto.
  El PDF habla de "float or positive integer"; `1e3`, `inf` o espacios
  extra se consideran entrada incorrecta.
- **Fechas reales**: se comprueban días por mes y años bisiestos. Una fecha
  como `2012-02-30` tiene el formato correcto pero no existe.
- **El año no tiene rango**: cualquier `YYYY` vale. Las fechas antes de la BD
  dan `no data`, y las posteriores usan el último precio conocido.
- **La base de datos no se valida a fondo**: viene con el enunciado y se
  considera fiable; solo se ignoran líneas sin coma.

---

## 10. Makefile

```make
NAME    = btc                         # nombre del ejecutable (lo exige el PDF)
CXX      = c++                        # compilador
CXXFLAGS = -Wall -Wextra -Werror -std=c++98   # flags obligatorias en 42
SRCS     = main.cpp BitcoinExchange.cpp
OBJS     = $(SRCS:.cpp=.o)            # main.o BitcoinExchange.o

all: $(NAME)
$(NAME): $(OBJS)                      # enlaza los .o en el ejecutable
	$(CXX) $(CXXFLAGS) -o $(NAME) $(OBJS)
%.o: %.cpp                            # regla patrón: cada .cpp → .o
	$(CXX) $(CXXFLAGS) -c $< -o $@    # $< = el .cpp, $@ = el .o
clean:  ; rm -f $(OBJS)               # borra objetos
fclean: clean ; rm -f $(NAME)         # borra objetos y ejecutable
re: fclean all                        # recompila todo
.PHONY: all clean fclean re           # no son ficheros reales
```

---

## 11. Preguntas típicas de evaluación

**¿Por qué `map` y no otro contenedor?**
Porque necesito buscar por fecha y, si no está, encontrar la anterior. El map
está ordenado y `lower_bound` lo hace en O(log n). Con un `vector` tendría
que ordenar y buscar a mano.

**¿Por qué puedes comparar fechas como strings?**
Porque `YYYY-MM-DD` tiene ancho fijo y va de lo más significativo a lo menos
significativo; el orden alfabético coincide con el cronológico.

**¿Qué hace `lower_bound` exactamente?**
Devuelve el primer elemento con clave `>=` que la buscada. Si no es igual,
retrocedo uno para tener la fecha anterior. Si es `end()`, retrocedo a la
última. Si es `begin()` y no coincide, no hay fecha anterior → error.

**¿Y `upper_bound`?**
Primer elemento con clave **estrictamente** `>`. También serviría: con
`upper_bound` siempre retrocedes uno (y si da `begin()`, no hay fecha
anterior). Es una alternativa igual de válida.

**¿Qué pasa si haces `--it` sobre `begin()`?**
Comportamiento indefinido. Por eso `getRate` lo comprueba antes.

**¿Cómo compruebas que una fecha existe?**
Formato con dígitos y guiones, mes 1‑12, y día entre 1 y los días de ese mes
(tabla), con febrero de 29 días en años bisiestos.

**¿Por qué `isNumber` en vez de solo `strtod`?**
Porque `strtod` acepta cosas que no son "un float o un entero" (`1e3`,
`inf`, `nan`, espacios delante) y con un string vacío no da error.

**¿Por qué `double` y no `float` / `int`?**
Para que `2147483648` no haga overflow y para tener precisión decimal en los
precios y cantidades.

**¿Por qué algunas funciones son `static`?**
Porque no usan ningún atributo del objeto; solo trabajan con sus
parámetros. `getRate` no puede serlo porque consulta `_db`.

**¿Por qué `.c_str()` al abrir el fichero?**
En C++98 el constructor de `std::ifstream` solo acepta `const char*`.

**¿Por qué los errores van a `std::cerr`?**
Es la salida de errores; así se pueden separar de los resultados con
redirecciones (`2>/dev/null`).

**¿Qué diferencia hay entre `_db[date]` y `_db.find(date)`?**
`operator[]` crea la clave si no existe (por eso no se puede usar en un
método `const`). `find` solo busca y devuelve `end()` si no está.
