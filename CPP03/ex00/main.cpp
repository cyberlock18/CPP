#include "ClapTrap.hpp"

int	main(void)
{
	std::cout << "=== Creando ClapTraps ===" << std::endl;
	ClapTrap	a("Alice");
	ClapTrap	b("Bob");

	std::cout << std::endl << "=== Acciones ===" << std::endl;
	a.attack("Bob");
	b.takeDamage(5);
	b.beRepaired(3);

	std::cout << std::endl << "=== Atacar sin energia ===" << std::endl;
	for (int i = 0; i < 10; i++)
		a.attack("Bob");
	a.attack("Bob");

	std::cout << std::endl << "=== Recibir dano mortal ===" << std::endl;
	b.takeDamage(100);
	b.takeDamage(1);
	b.beRepaired(5);

	std::cout << std::endl << "=== Destruccion ===" << std::endl;
	return 0;
}
