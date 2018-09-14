NAME = hashgraph

SRC = Event.cpp Person.cpp main.cpp md5.cpp Showcaser.cpp

OBJ = $(SRC:.cpp=.o)

FRAMEWORKS = -framework OpenGl

GREEN = \033[1;38;2;0;175;117m
RED = \033[1;38;2;225;20;20m
WHITE = \033[1;38;2;255;251;214m
YELLO = \033[1;38;2;255;200;0m

BREW_INC = -I ~/.brew/include

SDL_LINK = -L ~/.brew/lib -l SDL2

CPPFLAGS = -Wall -Wextra -Werror -O2

CPP = @clang++ -std=c++11

all: $(NAME)

$(NAME): brew obj compile done

clean:
	@echo "${RED}Removing object files"	
	@rm -f *.o
	@rm -f Log*
	@rm -rf *.dSYM

fclean: clean
	@echo "${RED}Removing Executable >.<"	
	@rm -f $(NAME)

re: fclean all

brew:
ifneq ("$(shell test -e $(HOME)/.brew && echo ex)" , "ex")
	@echo "BREW INSTALLING ..."
	@mkdir $(HOME)/.brew && curl -fsSL https://github.com/Homebrew/brew/tarball/master | tar xz --strip 1 -C $(HOME)/.brew
	@mkdir -p /tmp/.$(whoami)-brew-locks
	@mkdir -p $(HOME)/.brew/var/homebrew
	@ln -s /tmp/.$(whoami)-brew-locks $(HOME)/.brew/var/homebrew/locks
	@export PATH="$(HOME)/.brew/bin:$(PATH)"
	@brew update && brew upgrade
	@echo "${YELLO}INSTALLING SDL2: please be patient"
endif

compile :
	@echo "${YELLO}Checking for graphics library ..."	
	@brew install sdl2
	@echo "${YELLO}Compiling ...${WHITE}"
	$(CPP) $(CPPFLAGS) $(OBJ) -o $(NAME) $(FRAMEWORKS) $(BREW_INC) $(SDL_LINK)

obj: $(SRC) *.hpp
	@echo "${WHITE}Creating $@"
	$(CPP) -c $(SRC) $(BREW_INC)

done:
	@echo "${GREEN}Done! =)"