# CPP05 — ex03: Intern (Factory pattern sin if/else)

---

## El problema: crear formularios sin if/else

El enunciado prohíbe expresamente una cadena de `if/else if`. ¿Por qué?
Porque sería código frágil y difícil de mantener: añadir un nuevo tipo
de formulario exigiría modificar el if/else. En su lugar usamos una tabla.

---

## Solución: tabla de punteros a función

```cpp
struct FormEntry
{
    const char* name;
    AForm*      (*create)(std::string const&);
};
```

`AForm* (*create)(std::string const&)` es un **puntero a función**.
Lee de adentro hacia afuera: `create` es un puntero (`*`) a una función
que toma `std::string const&` y devuelve `AForm*`.

---

## Las funciones de creación (free functions)

```cpp
static AForm* makeShrubbery(std::string const& target)
{
    return new ShrubberyCreationForm(target);
}

static AForm* makeRobotomy(std::string const& target)
{
    return new RobotomyRequestForm(target);
}

static AForm* makePardon(std::string const& target)
{
    return new PresidentialPardonForm(target);
}
```

`static` aquí (fuera de clase) = visibles solo en este fichero .cpp.
Cada función crea el tipo correspondiente y devuelve un puntero a AForm.

---

## makeForm(): recorre la tabla

```cpp
AForm* Intern::makeForm(std::string const& name, std::string const& target) const
{
    static const FormEntry table[] = {
        { "shrubbery creation",  makeShrubbery },
        { "robotomy request",    makeRobotomy  },
        { "presidential pardon", makePardon    }
    };
    static const int SIZE = 3;

    for (int i = 0; i < SIZE; i++)
    {
        if (name == table[i].name)
        {
            std::cout << "Intern creates " << name << std::endl;
            return table[i].create(target);
        }
    }
    std::cerr << "Intern: unknown form '" << name << "'" << std::endl;
    return NULL;
}
```

- `static const FormEntry table[]` → la tabla existe UNA SOLA VEZ en memoria
  (static local), no se crea en cada llamada.
- El bucle compara el nombre con cada entrada.
- Si encuentra coincidencia: llama a `table[i].create(target)` → invoca el
  puntero a función correspondiente → crea y devuelve el formulario.
- Si no: imprime error y devuelve NULL.

---

## En el main: gestión de memoria

```cpp
AForm* rrf = someRandomIntern.makeForm("robotomy request", "Bender");
if (rrf)
{
    // usar el formulario
    delete rrf;   // ← OBLIGATORIO: lo creamos con new, debemos hacer delete
}
```

`makeForm` usa `new` → el que llama es responsable del `delete`.
Si no hacemos delete → memory leak.

---

## Por qué devolver AForm* y no el tipo concreto

```cpp
AForm* makeForm(std::string const& name, std::string const& target) const;
```

La función devuelve `AForm*` (puntero a la base) aunque cree subclases.
Esto permite usarla con cualquier tipo sin conocer el tipo exacto en tiempo
de compilación. El polimorfismo hace el resto: `rrf->execute(...)` llama
al execute() correcto del subtipo.

---

## Patron de diseño: Factory Method

Lo que hemos implementado es un **Factory Method** (Método Fábrica):
una función que crea objetos sin que el llamante sepa qué tipo exacto se crea.
El llamante solo sabe que obtiene un `AForm*`. Es uno de los patrones de diseño
más usados en software profesional.

---

## Conceptos nuevos

| Concepto | Qué es |
|----------|--------|
| **Puntero a función** | Variable que guarda la dirección de una función |
| **Tabla de dispatch** | Array de structs con nombre + función, evita if/else |
| **static local** | Variable que se inicializa una sola vez y persiste entre llamadas |
| **Factory Method** | Patrón que delega la creación de objetos a una función/clase |
| **NULL check** | Comprobar `if (rrf)` antes de usar un puntero que puede ser NULL |
| **delete en el llamante** | Si una función hace new y devuelve puntero, el receptor hace delete |
