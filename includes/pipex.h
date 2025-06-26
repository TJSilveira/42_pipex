#include "../libft/libft.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

char	**path_extractor(char **envp);
char	*ft_strjoin_3(const char *s1, char connector, const char *s2);
void	exec_command(char *command, char *envp[]);
int		executor(char *command, char *envp[]);
void	write_line(char *limit, int fd);
void	heredoc(char *argv[]);
int		open_fd(char *path, char option);
int		format_check(int argc, char *argv[]);
int		main(int argc, char *argv[], char *envp[]);
void	error_handler(char *msg, char *file_name);
