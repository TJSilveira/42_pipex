#include "../includes/pipex.h"

void	heredoc(char *argv[])
{
	int	pipe_fd[2];
	int	pid;

	if (pipe(pipe_fd) == -1)
	{
		perror("Laying down the pipe(s)");
		exit(3);
	}
	pid = fork();
	if (pid == -1)
		exit (EXIT_FAILURE);
	if (pid == 0)
	{
		close(pipe_fd[0]);
		write_line(argv[2], pipe_fd[1]);
	}
	else
	{
		close(pipe_fd[1]);
		if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
			perror("Duplicating read-end pipe to STDOUT\n");
		close(pipe_fd[0]);
		waitpid(pid, NULL, 0);
	}
}

int	open_fd(char *path, char option)
{
	int	fd;

	fd = -1;
	if (option == 'I')
		fd = open(path, O_RDONLY);
	else if (option == 'O')
		fd = open(path, O_WRONLY | O_TRUNC | O_CREAT, 0777);
	else if (option == 'H')
		fd = open(path, O_WRONLY | O_APPEND | O_CREAT, 0777);
	if (fd == -1)
		perror("Error: opening file\n");
	return (fd);
}

int	format_check(int argc, char *argv[])
{
	if (ft_strncmp(argv[1], "here_doc", 8) == 0)
	{
		if (argc < 6)
		{
			ft_putstr_fd("Less than 6 arguments for here_doc option", 2);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		if (argc < 5)
		{
			ft_putstr_fd("Less than 5 arguments provided\n", 2);
			exit(EXIT_FAILURE);
		}
	}
	return (0);
}

int	main(int argc, char *argv[], char *envp[])
{
	int	num;
	int	fd[2];

	format_check(argc, argv);
	if (ft_strncmp(argv[1], "here_doc", 8) == 0)
	{
		num = 2;
		heredoc(argv);
		fd[1] = open_fd(argv[argc - 1], 'H');
	}
	else
	{
		num = 1;
		fd[0] = open_fd(argv[1], 'I');
		if (dup2(fd[0], STDIN_FILENO) == -1)
			perror("Duplicating read-end pipe to STDIN\n");
		fd[1] = open_fd(argv[argc - 1], 'O');
	}
	while (++num < argc - 2)
		executor(argv[num], envp);
	if (dup2(fd[1], STDOUT_FILENO) == -1)
		perror("Duplicating write-end pipe to STDOUT\n");
	exec_command(argv[num], envp);
	return (0);
}
