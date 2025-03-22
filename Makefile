##
## EPITECH PROJECT, 2023
## makefile
## File description:
## Task 01 - make file
##

SRC_DIR = .
OBJ_DIR = obj
CPP_FILES = $(wildcard src/*.cpp) \
			$(wildcard src/core/*.cpp)
O_FILES = $(CPP_FILES:%.cpp=$(OBJ_DIR)/%.o)
CPPFLAGS = -Wall -Wextra -std=c++17
LDFLAGS = -lncurses -framework IOKit -framework CoreFoundation
SFML_PATH = /opt/homebrew/Cellar/sfml@2/2.6.2_1
SFMLFLAGS = I$(SFML_PATH)/include -o bin/app -L$(SFML_PATH)/lib -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -lsfml-network
NAME = mytop

all: ${NAME}

${NAME}: ${O_FILES}
	$(CXX) -o ${NAME} ${O_FILES} ${CPPFLAGS} ${LDFLAGS}

$(OBJ_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) -c -o $@ $< ${CPPFLAGS} ${SFMLFLAGS}

clean:
	rm -rf ${OBJ_DIR}

fclean: clean
	rm -f ${NAME}

re: fclean all
