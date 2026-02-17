#include "Dog.hpp"
#include "Cat.hpp"

#define NUM_ANIMALS 4

int	main(void)
{
	std::cout << "=== Test del subject ===" << std::endl;
	const Animal* j = new Dog();
	const Animal* i = new Cat();
	delete j;
	delete i;

	std::cout << std::endl << "=== Array de Animals ===" << std::endl;
	Animal*	animals[NUM_ANIMALS];

	for (int k = 0; k < NUM_ANIMALS; k++)
	{
		if (k < NUM_ANIMALS / 2)
			animals[k] = new Dog();
		else
			animals[k] = new Cat();
	}

	std::cout << std::endl << "=== Sonidos ===" << std::endl;
	for (int k = 0; k < NUM_ANIMALS; k++)
		animals[k]->makeSound();

	std::cout << std::endl << "=== Borrar array ===" << std::endl;
	for (int k = 0; k < NUM_ANIMALS; k++)
		delete animals[k];

	std::cout << std::endl << "=== Test deep copy ===" << std::endl;
	Dog original;
	original.getBrain()->ideas[0] = "I want food";
	original.getBrain()->ideas[1] = "I want to play";

	Dog copy(original);
	std::cout << "Original idea[0]: " << original.getBrain()->ideas[0] << std::endl;
	std::cout << "Copy idea[0]: " << copy.getBrain()->ideas[0] << std::endl;

	copy.getBrain()->ideas[0] = "I want to sleep";
	std::cout << "After modifying copy:" << std::endl;
	std::cout << "Original idea[0]: " << original.getBrain()->ideas[0] << std::endl;
	std::cout << "Copy idea[0]: " << copy.getBrain()->ideas[0] << std::endl;

	std::cout << std::endl << "=== Destruccion ===" << std::endl;
	return 0;
}
