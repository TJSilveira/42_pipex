#include "../includes/pipex.h"

int	open_fd(char *path, char option)
{
	int	fd;

	fd = -1;
	if (option == 'I')
		fd = open(path, O_RDONLY);
	else if (option == 'O')
		fd = open(path, O_WRONLY | O_TRUNC | O_CREAT, 0777);
	if (fd == -1 && option == 'I')
		error_handler("Error: opening file", path, 128);
	else if (fd == -1 && option == 'O')
		error_handler("Error: opening file", NULL, 1);
	return (fd);
}

/* This function has argv simply because it is needed in the bonus
   section and I did not want to have multiple header files just
   to accomodate to that change.                                 */
int	format_check(int argc, char *argv[])
{
	if (argc != 5)
		error_handler("Please provide 5 arguments", NULL, 1);
	if (ft_strncmp(argv[0], "./pipex", 7) == 0)
		return (0);
	else
		return (0);
}

int	main(int argc, char *argv[], char *envp[])
{
	int	num;
	int	fd[2];

	format_check(argc, argv);
	fd[0] = open_fd(argv[1], 'I');
	if (dup2(fd[0], STDIN_FILENO) == -1)
		error_handler("Duplicating read-end pipe to STDIN", NULL, 1);
	fd[1] = open_fd(argv[argc - 1], 'O');
	num = 1;
	while (++num < argc - 2)
		executor(argv[num], envp);
	if (dup2(fd[1], STDOUT_FILENO) == -1)
		error_handler("Duplicating write-end pipe to STDOUT", NULL, 1);
	return (exec_command(argv[num], envp));
}
