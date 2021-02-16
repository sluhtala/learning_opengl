NAME=test
SRC=main.cpp
OBJECTS = $(SRC:.c=.o)
#INCLUDES=glfw-3.3.2.bin.WIN32\glfw-3.3.2.bin.WIN32\include
#LIB=glfw-3.3.2.bin.WIN32\glfw-3.3.2.bin.WIN32\lib-mingw
INCLUDE=include
LIBRARIES= -L lib -lglew32 -D GLEW_STATIC -lglfw3 -lgdi32 -luser32 -lkernel32  -lopengl32

all: $(NAME)

$(NAME):
	g++ -g $(SRC) -I $(INCLUDE) $(LIBRARIES)  -o $(NAME)

clean:
	rm -f $(OBJECTS)

fclean: clean
	rm -f $(NAME)

re:fclean all

