# CPP06 — ex02: Identify real type (dynamic_cast)

---

## La jerarquía

```cpp
class Base { public: virtual ~Base(); };
class A : public Base {};
class B : public Base {};
class C : public Base {};
```

`Base` tiene un destructor virtual. Esto es OBLIGATORIO para que `dynamic_cast`
funcione correctamente. Sin destructor virtual, no hay tabla virtual (vtable)
y dynamic_cast no puede identificar el tipo real en tiempo de ejecución.

A, B, C son clases vacías que solo heredan de Base.

---

## generate() — polimorfismo en acción

```cpp
Base* generate(void)
{
    int r = std::rand() % 3;
    if (r == 0) return new A();
    if (r == 1) return new B();
    return new C();
}
```

Devuelve `Base*` pero el objeto real es A, B o C.
Esto es el corazón del polimorfismo: el puntero es de tipo base pero el objeto
en memoria es un subtipo concreto.

---

## identify(Base* p) — dynamic_cast con puntero

```cpp
void identify(Base* p)
{
    if (dynamic_cast<A*>(p))      std::cout << "A" << std::endl;
    else if (dynamic_cast<B*>(p)) std::cout << "B" << std::endl;
    else if (dynamic_cast<C*>(p)) std::cout << "C" << std::endl;
}
```

`dynamic_cast<A*>(p)`:
- Intenta hacer un downcast de `Base*` a `A*`.
- Si el objeto real ES un A → devuelve un `A*` válido (no nulo) → condición VERDADERA.
- Si NO es un A → devuelve `nullptr` → condición FALSA.

El casting falla silenciosamente con punteros (devuelve null). 
Por eso podemos usarlo en un if sin try/catch.

---

## identify(Base& p) — dynamic_cast con referencia

```cpp
void identify(Base& p)
{
    try { (void)dynamic_cast<A&>(p); std::cout << "A" << std::endl; return; }
    catch (std::exception&) {}
    try { (void)dynamic_cast<B&>(p); std::cout << "B" << std::endl; return; }
    catch (std::exception&) {}
    try { (void)dynamic_cast<C&>(p); std::cout << "C" << std::endl; return; }
    catch (std::exception&) {}
}
```

Con REFERENCIAS, dynamic_cast NO puede devolver null (una referencia no puede ser null).
Si el cast falla → lanza `std::bad_cast`.
Por eso usamos try/catch: si no lanza, el cast fue exitoso → encontramos el tipo.

`(void)dynamic_cast<A&>(p)` → el (void) descarta el resultado del cast
(lo que nos importa es si lanzó o no, no el valor devuelto).

El enunciado prohíbe usar punteros dentro de esta función y prohíbe `<typeinfo>`.

---

## ¿Por qué necesitamos `virtual ~Base()`?

Sin destructor virtual:
- La vtable (tabla de funciones virtuales) no existe o es incompleta.
- `dynamic_cast` necesita la vtable para saber el tipo real del objeto.
- Sin ella, el comportamiento es undefined (o simplemente no funciona).

Con destructor virtual:
- C++ crea una vtable para Base.
- Cada subtipo (A, B, C) tiene su propia entrada en la vtable.
- `dynamic_cast` consulta la vtable en tiempo de ejecución para identificar el tipo.

---

## Conceptos nuevos

| Concepto | Qué es |
|----------|--------|
| **dynamic_cast con puntero** | Devuelve nullptr si el cast falla (safe downcast) |
| **dynamic_cast con referencia** | Lanza std::bad_cast si el cast falla |
| **vtable** | Tabla interna que C++ crea para clases con funciones virtuales |
| **destructor virtual** | Obligatorio en clases base para delete correcto y dynamic_cast |
| **downcast** | Cast de clase base a clase derivada |
| **RTTI** | Run-Time Type Information — mecanismo que permite dynamic_cast |
