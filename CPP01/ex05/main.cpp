#include "Harl.hpp"

int	main(void)
{
	Harl	harl;

	// Probamos todos los niveles
	std::cout << "=== DEBUG ===" << std::endl;
	harl.complain("DEBUG");

	std::cout << std::endl << "=== INFO ===" << std::endl;
	harl.complain("INFO");

	std::cout << std::endl << "=== WARNING ===" << std::endl;
	harl.complain("WARNING");

	std::cout << std::endl << "=== ERROR ===" << std::endl;
	harl.complain("ERROR");

	// Probamos un nivel invalido (no hace nada)
	std::cout << std::endl << "=== INVALID ===" << std::endl;
	harl.complain("INVALID");

	return (0);
}
