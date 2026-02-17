# Variables
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98
SRC = megaphone.cpp
OBJ = $(SRC:.cpp=.o)
NAME = megaphone

# Regla por defecto (compila el programa)
all: $(NAME)

# Compilación del binario
$(NAME): $(OBJ)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)

# Regla para compilar el .cpp a .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regla para limpiar archivos objeto y el binario
clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

# Regla para reconstruir todo
re: fclean all

# Regla para compilar y limpiar
.PHONY: all clean fclean re
