#include "DiamondTrap.hpp"

int	main(void)
{
	std::cout << "=== Creando DiamondTrap ===" << std::endl;
	DiamondTrap	diamond("Diamondy");

	std::cout << std::endl << "=== Acciones DiamondTrap ===" << std::endl;
	diamond.attack("enemy");
	diamond.takeDamage(40);
	diamond.beRepaired(15);

	std::cout << std::endl << "=== Habilidades especiales ===" << std::endl;
	diamond.whoAmI();
	diamond.guardGate();
	diamond.highFivesGuys();

	std::cout << std::endl << "=== Creando otro DiamondTrap ===" << std::endl;
	DiamondTrap	diamond2("Sparky");
	diamond2.whoAmI();

	std::cout << std::endl << "=== Destruccion ===" << std::endl;
	return 0;
}
