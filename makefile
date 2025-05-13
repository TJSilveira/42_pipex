NAME = pipex

CC = gcc

CFLAGS = -Wall -Werror -Wextra

# Includes
INCLUDES = -I ./includes/


# Directories
SRC_DIR = ./src/
SRC_DIR_BONUS = ./src_bonus/
OBJS_DIR = ./objs/
LIBFT_DIR = ./libft/

# Source Files
SRC_FILES = pipex.c

SRC_FILES_BONUS = pipex.c

# Object Files
OBJS_FILES = $(SRC_FILES:.c=.o)
OBJS_FILES_BONUS = $(SRC_FILES_BONUS:.c=.o)
OBJS = $(addprefix $(OBJS_DIR),$(OBJS_FILES))
OBJS_BONUS = $(addprefix $(OBJS_DIR), $(OBJS_FILES_BONUS))

all: ${NAME}

# creating executables
${NAME}: $(OBJS_DIR) $(OBJS) 
	${CC} ${CFLAGS} ${OBJS} -L${LIBFT_DIR} -lft -o ${NAME}

bonus: libft $(OBJS_DIR) $(OBJS_BONUS) 
	${CC} ${CFLAGS} ${OBJS_BONUS} -L${LIBFT_DIR} -lft -o ${NAME}

# Creating Object file folder
$(OBJS_DIR):
	mkdir -p $(OBJS_DIR)

# Creating object files
$(OBJS_DIR)%.o: $(SRC_DIR)%.c
	${CC} ${CFLAGS} -c ${INCLUDES} $< -o $@

$(OBJS_DIR)%.o: $(SRC_DIR_BONUS)%.c
	${CC} ${CFLAGS} -c ${INCLUDES} $< -o $@

clean:
	rm -rf $(OBJS_DIR)

fclean: clean
	rm -rf ${NAME} out*

re: fclean ${NAME}

rebonus: fclean bonus

.PHONY: all clean fclean re