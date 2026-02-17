#include "Dog.hpp"
#include "Cat.hpp"
#include "WrongCat.hpp"

int	main(void)
{
	std::cout << "=== Test del subject ===" << std::endl;
	const Animal* meta = new Animal();
	const Animal* j = new Dog();
	const Animal* i = new Cat();

	std::cout << j->getType() << " " << std::endl;
	std::cout << i->getType() << " " << std::endl;
	i->makeSound();
	j->makeSound();
	meta->makeSound();

	delete i;
	delete j;
	delete meta;

	std::cout << std::endl << "=== Test WrongAnimal (sin virtual) ===" << std::endl;
	const WrongAnimal* wrongMeta = new WrongAnimal();
	const WrongAnimal* wrongCat = new WrongCat();

	std::cout << wrongCat->getType() << " " << std::endl;
	wrongCat->makeSound();
	wrongMeta->makeSound();

	delete wrongCat;
	delete wrongMeta;

	std::cout << std::endl << "=== Test en stack ===" << std::endl;
	Dog dog;
	Cat cat;
	std::cout << "Dog type: " << dog.getType() << std::endl;
	std::cout << "Cat type: " << cat.getType() << std::endl;
	dog.makeSound();
	cat.makeSound();

	return 0;
}
