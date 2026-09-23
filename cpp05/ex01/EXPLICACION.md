# CPP05 — ex01: Form (Formularios y firma)

Este ejercicio añade la clase `Form` y conecta `Bureaucrat` con ella mediante `signForm()`.

---

## El problema del include circular

`Bureaucrat` necesita conocer `Form` (para `signForm`).
`Form` necesita conocer `Bureaucrat` (para `beSigned`).
Si cada uno incluye al otro con `#include`, el compilador entraría en un bucle infinito.

**Solución: forward declaration (declaración anticipada)**

En `Form.hpp` escribimos:
```cpp
class Bureaucrat;   // "oye compilador, existe una clase llamada Bureaucrat"
```
Y en `Bureaucrat.hpp`:
```cpp
class Form;         // "oye compilador, existe una clase llamada Form"
```

Esto le dice al compilador "confía en mí, esa clase existe" sin necesitar incluir el header entero.
Solo funciona cuando usamos la clase como referencia o puntero (`Form&`, `Form*`), no como valor directo.
El `#include` real lo hacemos en los `.cpp`, donde ya se necesita saber todo.

---

## Form.hpp — La clase formulario

```cpp
class Form
{
private:
    std::string const   _name;           // nombre del formulario, inmutable
    bool                _signed;         // ¿está firmado? false al construir
    int const           _gradeToSign;    // grado mínimo para firmar
    int const           _gradeToExecute; // grado mínimo para ejecutar
```

Cuatro atributos privados. `_name`, `_gradeToSign` y `_gradeToExecute` son `const`
porque un formulario no cambia de nombre ni de requisitos una vez creado.
Solo `_signed` cambia (de `false` a `true` cuando se firma).

---

## Form.cpp — La implementación

### Constructor principal

```cpp
Form::Form(std::string const& name, int gradeToSign, int gradeToExecute)
    : _name(name), _signed(false), _gradeToSign(gradeToSign), _gradeToExecute(gradeToExecute)
{
    if (_gradeToSign < GRADE_MAX || _gradeToExecute < GRADE_MAX)
        throw GradeTooHighException();
    if (_gradeToSign > GRADE_MIN || _gradeToExecute > GRADE_MIN)
        throw GradeTooLowException();
}
```

Inicializamos todos los atributos en la lista de inicialización (obligatorio para los `const`).
Usamos `||` porque basta con que UNO de los dos grados sea inválido para lanzar la excepción.

### beSigned()

```cpp
void Form::beSigned(Bureaucrat const& b)
{
    if (b.getGrade() > _gradeToSign)
        throw GradeTooLowException();
    _signed = true;
}
```

Recibe un `Bureaucrat` por referencia constante (no lo modificamos, solo leemos su grado).
Recuerda: grado 1 > grado 50 en términos de jerarquía, pero numéricamente 1 < 50.
Si el grado del burócrata (número) es MAYOR que el requerido, significa que su rango es MENOR
y no tiene autoridad suficiente → excepción.
Si pasa la comprobación, marcamos el formulario como firmado: `_signed = true`.

### Operador <<

```cpp
std::ostream& operator<<(std::ostream& out, Form const& f)
{
    out << "Form [" << f.getName() << "]"
        << " | signed: " << (f.getSigned() ? "yes" : "no")
        << " | grade to sign: " << f.getGradeToSign()
        << " | grade to execute: " << f.getGradeToExecute();
    return out;
}
```

`f.getSigned() ? "yes" : "no"` es el **operador ternario**: si la condición es verdadera
devuelve "yes", si no "no". Es una forma compacta de escribir un if/else en una línea.

---

## Bureaucrat.cpp — signForm() añadido

```cpp
void Bureaucrat::signForm(Form& f)
{
    try
    {
        f.beSigned(*this);
        std::cout << getName() << " signed " << f.getName() << std::endl;
    }
    catch (std::exception& e)
    {
        std::cout << getName() << " couldn't sign " << f.getName()
                  << " because " << e.what() << std::endl;
    }
}
```

`*this` → `this` es un puntero al objeto actual (el Bureaucrat que llama a signForm).
Con `*this` lo desreferenciamos para pasarlo como referencia a `beSigned`.
El `try/catch` aquí no relanza la excepción: la captura, imprime el mensaje y sigue.
Así el programa no se cae si un burócrata no puede firmar un formulario.

---

## Flujo completo de una firma exitosa

```
manager.signForm(taxForm)
  └─→ taxForm.beSigned(*this)          ← el burócrata se pasa a sí mismo
        └─→ b.getGrade() <= _gradeToSign  ← ¿tiene grado suficiente?
              └─→ _signed = true           ← formulario firmado
  └─→ imprime "Carlos signed Tax Return"
```

## Flujo de una firma fallida

```
junior.signForm(secretForm)
  └─→ secretForm.beSigned(*this)
        └─→ b.getGrade() > _gradeToSign   ← grado insuficiente
              └─→ throw GradeTooLowException()
  └─→ catch captura la excepción
  └─→ imprime "Bob couldn't sign Top Secret because form grade is too low"
```

---

## Conceptos nuevos en este ejercicio

| Concepto | Qué es |
|----------|--------|
| **Forward declaration** | Declarar que una clase existe sin incluir su header completo |
| **Include circular** | Problema cuando A incluye B y B incluye A |
| **Referencia constante** | `const Bureaucrat& b` — acceso sin copia, sin modificar |
| **`*this`** | Desreferencia del puntero al objeto actual, para pasarlo como referencia |
| **Operador ternario** | `condicion ? valor_si_true : valor_si_false` |
| **catch sin relanzar** | Capturar la excepción para imprimir un mensaje en lugar de crashear |
