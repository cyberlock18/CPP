#include "Serializer.hpp"
#include <iostream>

int main()
{
	Data original;
	original.id    = 42;
	original.label = "hello from 42";
	original.value = 3.14;

	std::cout << "Original pointer : " << &original << std::endl;
	std::cout << "  id    = " << original.id    << std::endl;
	std::cout << "  label = " << original.label << std::endl;
	std::cout << "  value = " << original.value << std::endl;

	uintptr_t raw = Serializer::serialize(&original);
	std::cout << "\nSerialized (uintptr_t): " << raw << std::endl;

	Data* recovered = Serializer::deserialize(raw);
	std::cout << "\nRecovered pointer: " << recovered << std::endl;
	std::cout << "  id    = " << recovered->id    << std::endl;
	std::cout << "  label = " << recovered->label << std::endl;
	std::cout << "  value = " << recovered->value << std::endl;

	std::cout << "\nPointers match: " << (&original == recovered ? "YES" : "NO") << std::endl;

	return 0;
}
