NAME = fractol

SRC = main.c draw.c fractals.c events.c param.c colors.c

OBJ = ${SRC:%.c=%.o}

FLAGS =  -Wall -Wextra -Werror 

COPILATION =  -Lmlx_linux -lmlx_Linux -L/usr/lib -Imlx_linux -lXext -lX11 -lm -lz

%.o:%.c
		cc $(FLAGS) -I/usr/include -Imlx_linux -c $< -o $@

all : $(NAME)

$(NAME): $(OBJ)
	cc $(FLAGS) $(OBJ) $(COPILATION)  -o $(NAME)

clean :
	rm -rf $(OBJ) 

fclean : clean
	rm -rf  $(NAME) 
re : fclean all

