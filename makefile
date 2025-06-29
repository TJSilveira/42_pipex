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
SRC_FILES = pipex.c \
			utils_1.c \
			utils_2.c

SRC_FILES_BONUS = pipex_bonus.c \
				utils_1_bonus.c \
				utils_2_bonus.c \
				utils_3_bonus.c

# Object Files
OBJS_FILES = $(SRC_FILES:.c=.o)
OBJS_FILES_BONUS = $(SRC_FILES_BONUS:.c=.o)
OBJS = $(addprefix $(OBJS_DIR),$(OBJS_FILES))
OBJS_BONUS = $(addprefix $(OBJS_DIR), $(OBJS_FILES_BONUS))

# Libraries
LIBFT = $(LIBFT_DIR)libft.a

all: ${NAME}

$(LIBFT):
	make -C $(LIBFT_DIR)

# creating executables
${NAME}: $(LIBFT) $(OBJS_DIR) $(OBJS) 
	${CC} ${CFLAGS} ${OBJS} -L${LIBFT_DIR} -lft -o ${NAME}

bonus: $(LIBFT) $(OBJS_DIR) $(OBJS_BONUS) 
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
	rm -rf ./libft/*.o

fclean: clean
	rm -rf ${NAME} out* valgrind*
	rm -rf ./libft/*.a

re: fclean ${NAME}

rebonus: fclean bonus

.PHONY: all clean fclean re