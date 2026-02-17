#include "ScavTrap.hpp"
#include "FragTrap.hpp"

int	main(void)
{
	std::cout << "=== Creando ClapTrap ===" << std::endl;
	ClapTrap	clap("Clappy");

	std::cout << std::endl << "=== Creando ScavTrap ===" << std::endl;
	ScavTrap	scav("Scavvy");

	std::cout << std::endl << "=== Creando FragTrap ===" << std::endl;
	FragTrap	frag("Fraggy");

	std::cout << std::endl << "=== Acciones ClapTrap ===" << std::endl;
	clap.attack("enemy");
	clap.takeDamage(3);

	std::cout << std::endl << "=== Acciones ScavTrap ===" << std::endl;
	scav.attack("enemy");
	scav.takeDamage(20);
	scav.guardGate();

	std::cout << std::endl << "=== Acciones FragTrap ===" << std::endl;
	frag.attack("enemy");
	frag.takeDamage(50);
	frag.beRepaired(20);
	frag.highFivesGuys();

	std::cout << std::endl << "=== Destruccion (orden inverso) ===" << std::endl;
	return 0;
}
