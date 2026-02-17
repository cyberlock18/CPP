#include "ScavTrap.hpp"

int	main(void)
{
	std::cout << "=== Creando ClapTrap ===" << std::endl;
	ClapTrap	clap("Clappy");

	std::cout << std::endl << "=== Creando ScavTrap ===" << std::endl;
	ScavTrap	scav("Scavvy");

	std::cout << std::endl << "=== Acciones ClapTrap ===" << std::endl;
	clap.attack("enemy");
	clap.takeDamage(5);
	clap.beRepaired(3);

	std::cout << std::endl << "=== Acciones ScavTrap ===" << std::endl;
	scav.attack("enemy");
	scav.takeDamage(30);
	scav.beRepaired(10);
	scav.guardGate();

	std::cout << std::endl << "=== Destruccion (orden inverso) ===" << std::endl;
	return 0;
}
