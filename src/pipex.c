#include "../includes/pipex.h"

int	open_fd(char *path, char option)
{
	int	fd;

	fd = -1;
	if (option == 'I')
		fd = open(path, O_RDONLY);
	else if (option == 'O')
		fd = open(path, O_WRONLY | O_TRUNC | O_CREAT, 0777);
	if (fd == -1)
		perror("Error: opening file\n");
	return (fd);
}

/* This function has argv simply because it is needed in the bonus
   section and I did not want to have multiple header files just
   to accomodate to that change.                                 */
int	format_check(int argc, char *argv[])
{
	if (argc != 5)
	{
		ft_putstr_fd("Please provide 5 arguments\n", 2);
		exit(EXIT_FAILURE);
	}
	if (ft_strcmp(argv[0],"./pipex") == 0)
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
		perror("Duplicating read-end pipe to STDIN\n");
	fd[1] = open_fd(argv[argc - 1], 'O');
	while (++num < argc - 2)
		executor(argv[num], envp);
	if (dup2(fd[1], STDOUT_FILENO) == -1)
		perror("Duplicating write-end pipe to STDOUT\n");
	exec_command(argv[num], envp);
	close(fd[0]);
	close(fd[1]);
	return (0);
}
