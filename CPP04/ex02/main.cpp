#include "Dog.hpp"
#include "Cat.hpp"

int	main(void)
{
	// AAnimal test = AAnimal(); // ERROR: no se puede instanciar una clase abstracta
	// Descomenta la linea de arriba para verificar que da error de compilacion

	std::cout << "=== Crear Dog y Cat como AAnimal* ===" << std::endl;
	const AAnimal* dog = new Dog();
	const AAnimal* cat = new Cat();

	std::cout << std::endl << "=== Polimorfismo ===" << std::endl;
	std::cout << "Type: " << dog->getType() << std::endl;
	std::cout << "Type: " << cat->getType() << std::endl;
	dog->makeSound();
	cat->makeSound();

	std::cout << std::endl << "=== Delete ===" << std::endl;
	delete dog;
	delete cat;

	std::cout << std::endl << "=== Test deep copy ===" << std::endl;
	Dog original;
	original.getBrain()->ideas[0] = "Chase the ball";

	Dog copy(original);
	std::cout << "Original: " << original.getBrain()->ideas[0] << std::endl;
	std::cout << "Copy: " << copy.getBrain()->ideas[0] << std::endl;

	copy.getBrain()->ideas[0] = "Eat the bone";
	std::cout << "After modifying copy:" << std::endl;
	std::cout << "Original: " << original.getBrain()->ideas[0] << std::endl;
	std::cout << "Copy: " << copy.getBrain()->ideas[0] << std::endl;

	std::cout << std::endl << "=== Destruccion ===" << std::endl;
	return 0;
}
