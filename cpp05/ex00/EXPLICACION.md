# CPP05 — ex00: Bureaucrat (Excepciones)

---

## Bureaucrat.hpp — El "plano" de la clase

```cpp
#ifndef BUREAUCRAT_HPP
#define BUREAUCRAT_HPP
```
**Include guard.** Evita que este fichero se incluya dos veces en el mismo programa.
Si se incluyera dos veces, el compilador vería la clase definida dos veces y daría error.
`#ifndef` = "si NO está definido", entonces defínelo. La próxima vez que alguien intente
incluirlo, `BUREAUCRAT_HPP` ya existe y se salta todo el contenido.

---

```cpp
#include <iostream>    // para std::ostream (el operador <<)
#include <stdexcept>   // para std::exception (clase base de errores)
#include <string>      // para std::string
```
Incluimos las librerías que necesitamos.

---

```cpp
class Bureaucrat
{
```
Declaramos la clase `Bureaucrat`. Una **clase** en C++ es como un molde o plano:
defines qué datos tiene (atributos) y qué puede hacer (métodos/funciones).

---

```cpp
    class GradeTooHighException : public std::exception
    {
    public:
        const char* what() const throw();
    };
```
Clase de excepción anidada dentro de `Bureaucrat`.

- `: public std::exception` → hereda de la clase base estándar de errores.
  Esto nos permite hacer `catch(std::exception& e)` y capturar esta excepción.
- `what()` → devuelve el mensaje de error como texto.
- `const throw()` → garantiza que este método no lanza excepciones él mismo (C++98).

Lo mismo aplica a `GradeTooLowException`.

---

```cpp
    Bureaucrat();                                          // constructor por defecto
    Bureaucrat(std::string const& name, int grade);       // constructor con parámetros
    Bureaucrat(Bureaucrat const& other);                  // constructor de copia
    Bureaucrat& operator=(Bureaucrat const& other);       // operador de asignación
    ~Bureaucrat();                                        // destructor
```
**Forma Canónica Ortodoxa (OCF)** — obligatoria en 42 desde el módulo 02.
Son los 4 métodos que toda clase bien formada debe tener:

| Método | Cuándo se usa |
|--------|---------------|
| Constructor por defecto | `Bureaucrat b;` |
| Constructor de copia | `Bureaucrat b2 = b1;` |
| Operador de asignación | `b2 = b1;` (después de que b2 ya existe) |
| Destructor | Automáticamente al salir del scope o hacer delete |

---

```cpp
private:
    std::string const   _name;          // nombre: es CONST, no cambia nunca
    int                 _grade;         // grado: puede cambiar

    static const int GRADE_MAX = 1;    // grado máximo permitido
    static const int GRADE_MIN = 150;  // grado mínimo permitido
```
- `const _name` → el nombre es inmutable, una vez creado no se puede cambiar.
- `_grade` → el grado sí puede cambiar (con increment/decrement).
- `static const` → estas constantes pertenecen a la CLASE, no a cada objeto.
  Todos los Bureaucrat comparten los mismos límites. Se evitan así "números mágicos"
  sueltos por el código.

---

## Bureaucrat.cpp — La implementación

```cpp
const char* Bureaucrat::GradeTooHighException::what() const throw()
{
    return "grade is too high (min is 1)";
}
```
Implementación de `what()`. Leemos `Bureaucrat::GradeTooHighException::what`
de derecha a izquierda: el método `what`, que pertenece a `GradeTooHighException`,
que pertenece a `Bureaucrat`. El `::` es el operador de scope (ámbito).

---

```cpp
Bureaucrat::Bureaucrat() : _name("unnamed"), _grade(150)
{
}
```
Constructor por defecto. La parte `: _name("unnamed"), _grade(150)` es la
**lista de inicialización**. Es la forma correcta de inicializar atributos en C++,
especialmente los `const` — no puedes asignar un `const` dentro del cuerpo del
constructor porque ya sería demasiado tarde.

---

```cpp
Bureaucrat::Bureaucrat(std::string const& name, int grade) : _name(name), _grade(grade)
{
    if (_grade < GRADE_MAX)     // si grade < 1
        throw GradeTooHighException();
    if (_grade > GRADE_MIN)     // si grade > 150
        throw GradeTooLowException();
}
```
Constructor principal. Primero inicializa los atributos (lista de inicialización),
luego valida. Si el grado es inválido, lanzamos (`throw`) una excepción — esto para
el constructor inmediatamente y el objeto **nunca llega a existir**.

---

```cpp
Bureaucrat::Bureaucrat(Bureaucrat const& other) : _name(other._name), _grade(other._grade)
{
}
```
Constructor de copia. Crea un nuevo objeto copiando los valores de otro.
`other` es el objeto del que copiamos.

---

```cpp
Bureaucrat& Bureaucrat::operator=(Bureaucrat const& other)
{
    if (this != &other)
        _grade = other._grade;
    return *this;
}
```
Operador de asignación. `this` es el puntero al objeto actual.
`if (this != &other)` evita el caso `b = b` (autoasignación).
Solo copiamos `_grade` porque `_name` es `const` y no se puede reasignar.
Devolvemos `*this` (referencia al objeto actual) para poder encadenar: `a = b = c`.

---

```cpp
void Bureaucrat::incrementGrade()
{
    if (_grade - 1 < GRADE_MAX)
        throw GradeTooHighException();
    _grade--;
}
```
Subir de grado en burocracia = número más BAJO (grado 1 es el jefe supremo).
Antes de decrementar el número, comprobamos si ya estamos en el límite máximo.

```cpp
void Bureaucrat::decrementGrade()
{
    if (_grade + 1 > GRADE_MIN)
        throw GradeTooLowException();
    _grade++;
}
```
Bajar de grado = número más ALTO. Comprobamos el límite antes de cambiar.

---

```cpp
std::ostream& operator<<(std::ostream& out, Bureaucrat const& b)
{
    out << b.getName() << ", bureaucrat grade " << b.getGrade();
    return out;
}
```
Sobrecarga del operador `<<`. Permite hacer `std::cout << alice`.

- `std::ostream&` → tipo de `std::cout` (flujo de salida).
- Recibe el stream y el Bureaucrat, escribe en el stream, y devuelve el mismo stream
  para poder encadenar: `cout << a << b`.

---

## main.cpp — Los tests

| Test | Qué prueba |
|------|-----------|
| Test 1 | Bureaucrat normal, increment y decrement funcionan |
| Test 2 | Excepción al crear con grado 0 (demasiado alto) |
| Test 3 | Excepción al crear con grado 151 (demasiado bajo) |
| Test 4 | Excepción al incrementar desde grado 1 |
| Test 5 | Excepción al decrementar desde grado 150 |

Todos los errores se capturan con `try { ... } catch (std::exception& e) { ... }`.

---

## Conceptos clave aprendidos

| Concepto | Qué es |
|----------|--------|
| **Clase** | Molde que agrupa datos y funciones relacionados |
| **OCF** | Los 4 métodos obligatorios de toda clase bien formada |
| **Include guard** | Protección contra doble inclusión de headers |
| **Lista de inicialización** | `: attr(valor)` — forma correcta de inicializar atributos |
| **throw** | Lanza una excepción, detiene la ejecución normal |
| **try/catch** | Bloque que intenta código y captura errores si ocurren |
| **Clase anidada** | Clase definida dentro de otra clase |
| **static const** | Constante que pertenece a la clase, no a cada objeto |
| **Herencia** | `: public std::exception` — la excepción hereda comportamiento base |
