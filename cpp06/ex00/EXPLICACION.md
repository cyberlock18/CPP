# CPP06 — ex00: ScalarConverter (Castings y detección de tipos)

---

## Qué hace el programa

Recibe un literal como string: `"42"`, `"42.0f"`, `"A"`, `"nan"`, etc.
Detecta qué tipo es, lo convierte a los 4 tipos básicos y los imprime.

---

## Clase no instanciable (static only)

```cpp
class ScalarConverter
{
public:
    static void convert(std::string const& literal);

private:
    ScalarConverter();               // constructor privado
    ScalarConverter(ScalarConverter const&);
    ScalarConverter& operator=(ScalarConverter const&);
    ~ScalarConverter();
};
```

Al poner el constructor en `private`, nadie puede crear un objeto de esta clase.
Solo se puede usar el método estático: `ScalarConverter::convert("42")`.
Tiene sentido: esta clase no necesita estado, solo funcionalidad.

---

## Detección del tipo

```cpp
enum LiteralType { CHAR, INT, FLOAT, DOUBLE, SPECIAL };
```

Un `enum` es una lista de constantes enteras con nombre. Más legible que usar 0,1,2,3,4.

```cpp
static LiteralType detectType(std::string const& s)
{
    // 1. ¿Es especial? (nan, inf, inff)
    if (s == "nan" || s == "+inf" || ...) return SPECIAL;

    // 2. ¿Es un char? (longitud 1 y no es dígito)
    if (s.length() == 1 && !std::isdigit(s[0])) return CHAR;

    // 3. Recorre el string buscando '.' y 'f'
    for (size_t i = start; i < s.length(); i++)
    {
        if (s[i] == '.')      { hasDot = true; continue; }
        if (s[i] == 'f' && i == s.length() - 1) { hasF = true; continue; }
        if (!std::isdigit(s[i])) return CHAR;
    }

    if (hasDot && hasF)  return FLOAT;   // "42.0f"
    if (hasDot && !hasF) return DOUBLE;  // "42.0"
    return INT;                           // "42"
}
```

---

## Los 4 castings de C++

### static_cast — conversión entre tipos relacionados

```cpp
static_cast<char>(val)      // double → char
static_cast<int>(val)       // double → int
static_cast<float>(val)     // double → float
```

Es el casting más seguro. El compilador verifica que la conversión tiene sentido.
Se usa para conversiones numéricas, upcast/downcast en jerarquías conocidas.

### reinterpret_cast — reinterpretar memoria (ex01)

Reinterpreta los bytes de una variable como otro tipo. Sin conversión real.
Usado en ex01 para convertir puntero ↔ entero.

### dynamic_cast — downcast seguro en polimorfismo (ex02)

Verifica en tiempo de ejecución si el cast es válido.
Devuelve nullptr si falla (para punteros) o lanza std::bad_cast (para referencias).

### const_cast — quitar/añadir const

Permite quitar el modificador const de una variable.
Peligroso si se modifica algo que era verdaderamente const.

---

## Conversión con strtod

```cpp
char* end;
val = std::strtod(literal.c_str(), &end);
```

`strtod` ("string to double") convierte un string a double.
`end` apunta al primer carácter que NO pudo convertir.
Si `*end != '\0'`, significa que la conversión no fue completa → error.

---

## Comprobaciones antes de imprimir

```cpp
static void printChar(double val)
{
    if (val < 0 || val > 127 || std::isnan(val) || std::isinf(val))
        std::cout << "char: impossible" << std::endl;
    else if (!std::isprint(static_cast<int>(val)))
        std::cout << "char: Non displayable" << std::endl;
    else
        std::cout << "char: '" << static_cast<char>(val) << "'" << std::endl;
}
```

- `std::isnan(val)` → verdadero si val es NaN (Not a Number).
- `std::isinf(val)` → verdadero si val es infinito.
- `std::isprint(c)` → verdadero si c es un carácter imprimible (espacio, letras, etc.).
- Solo convertimos si todos los checks pasan.

---

## Manejo de los especiales (nan, inf)

```cpp
if (literal == "nanf" || literal == "nan")
{
    std::cout << "char: impossible" << std::endl;
    std::cout << "int: impossible" << std::endl;
    std::cout << "float: nanf" << std::endl;
    std::cout << "double: nan" << std::endl;
}
```

NaN e Infinito no se pueden representar como char o int → "impossible".
El formato C++98 para NaN en float es `nanf` y en double `nan`.

---

## Conceptos nuevos

| Concepto | Qué es |
|----------|--------|
| **enum** | Lista de constantes enteras con nombre legible |
| **static_cast** | Casting seguro entre tipos relacionados |
| **reinterpret_cast** | Reinterpreter memoria como otro tipo |
| **dynamic_cast** | Casting con verificación en tiempo de ejecución |
| **const_cast** | Quitar o añadir const |
| **strtod** | Convierte string a double, indica dónde paró |
| **isnan / isinf** | Detectan NaN e infinito en floats/doubles |
| **isprint** | Comprueba si un char es imprimible |
