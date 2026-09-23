# CPP05 — ex02: AForm + Formularios concretos (Polimorfismo)

---

## El concepto clave: clase abstracta

```cpp
class AForm
{
public:
    virtual void execute(Bureaucrat const& executor) const = 0;
};
```

`= 0` al final de una función virtual la convierte en **función virtual pura**.
Esto hace que `AForm` sea una **clase abstracta**: no se puede instanciar directamente.
`AForm* f = new AForm()` → ERROR de compilación.
Solo se puede instanciar a través de sus clases hijas concretas.

¿Por qué? Porque no tiene sentido ejecutar un "formulario genérico" — cada tipo
de formulario hace algo diferente. Obligamos a que cada subclase implemente `execute()`.

---

## checkExecution() — lógica compartida en la base

```cpp
void AForm::checkExecution(Bureaucrat const& executor) const
{
    if (!_signed)
        throw FormNotSignedException();
    if (executor.getGrade() > _gradeToExecute)
        throw GradeTooLowException();
}
```

Este método está en `AForm` (protected) y lo llaman todas las subclases al inicio
de su `execute()`. Así no repetimos la misma validación en cada subclase.
`protected` = visible para la clase y sus hijos, pero no para el exterior.

---

## ShrubberyCreationForm — escribe un fichero

```cpp
void ShrubberyCreationForm::execute(Bureaucrat const& executor) const
{
    checkExecution(executor);
    std::ofstream file((_target + "_shrubbery").c_str());
    if (!file.is_open())
        throw std::runtime_error("cannot open file");
    file << "       *       " << std::endl;
    // ...ASCII tree...
}
```

- `checkExecution(executor)` → valida firma y grado.
- `std::ofstream` → flujo de salida para ficheros (como cout pero para archivos).
- `.c_str()` → convierte std::string a `const char*` (lo que pide fstream en C++98).
- Si el fichero no abre → excepción.
- Escribe el árbol ASCII y cierra automáticamente al destruirse `file`.

---

## RobotomyRequestForm — 50% de éxito

```cpp
void RobotomyRequestForm::execute(Bureaucrat const& executor) const
{
    checkExecution(executor);
    std::cout << "BZZZZZT... DRRRRRR... WHIRRRRRR..." << std::endl;
    if (std::rand() % 2 == 0)
        std::cout << _target << " has been robotomized successfully!" << std::endl;
    else
        std::cout << "Robotomy of " << _target << " failed." << std::endl;
}
```

`std::rand() % 2` → genera un número aleatorio y lo divide entre 2.
El resto puede ser 0 o 1 → 50% de probabilidad cada uno.
Inicializamos la semilla con `std::srand(std::time(NULL))` en main para que
los números sean diferentes en cada ejecución.

---

## PresidentialPardonForm — el más sencillo

```cpp
void PresidentialPardonForm::execute(Bureaucrat const& executor) const
{
    checkExecution(executor);
    std::cout << _target << " has been pardoned by Zaphod Beeblebrox." << std::endl;
}
```

Pasa la validación y simplemente imprime. Los grados son los más restrictivos
(sign 25, exec 5) porque perdonar a alguien requiere mucho poder.

---

## executeForm() en Bureaucrat

```cpp
void Bureaucrat::executeForm(AForm const& f)
{
    try
    {
        f.execute(*this);
        std::cout << getName() << " executed " << f.getName() << std::endl;
    }
    catch (std::exception& e)
    {
        std::cerr << getName() << " couldn't execute " << f.getName()
                  << " because " << e.what() << std::endl;
    }
}
```

Recibe `AForm const&` → acepta CUALQUIER subclase (ShrubberyCreationForm,
RobotomyRequestForm, PresidentialPardonForm). Esto es **polimorfismo**.
El compilador llama al `execute()` correcto según el tipo real del objeto.

---

## Herencia y OCF en subclases

```cpp
ShrubberyCreationForm::ShrubberyCreationForm(std::string const& target)
    : AForm("ShrubberyCreation", 145, 137), _target(target) {}
```

En la lista de inicialización, llamamos al constructor de `AForm` con nombre y grados.
El hijo DEBE inicializar al padre explícitamente así.

```cpp
ShrubberyCreationForm& ShrubberyCreationForm::operator=(ShrubberyCreationForm const& other)
{
    if (this != &other)
    {
        AForm::operator=(other);   // llama al operator= del padre
        _target = other._target;   // luego copia lo propio
    }
    return *this;
}
```

En el operator= del hijo, primero llamamos al operator= del padre para
copiar los atributos heredados, luego copiamos los propios.

---

## Conceptos nuevos

| Concepto | Qué es |
|----------|--------|
| **Clase abstracta** | Clase con al menos una función virtual pura (`= 0`). No instanciable. |
| **Función virtual pura** | `virtual void f() = 0` — obliga a las subclases a implementarla |
| **Polimorfismo** | Llamar al método correcto según el tipo real del objeto en tiempo de ejecución |
| **protected** | Visible para la clase y sus hijos, no para el exterior |
| **std::ofstream** | Flujo de salida a ficheros |
| **AForm::operator=** | Llamar al operator= del padre desde el hijo |
| **Jerarquía de grados** | sign < exec en importancia: necesitas alto grado para ejecutar |
