/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 17:56:51 by tsilveir          #+#    #+#             */
/*   Updated: 2025/06/27 17:56:53 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	write_line(char *limit, int fd)
{
	char	*line;
	char	*limitor;
	size_t	size;

	limitor = ft_strjoin(limit, "\n");
	size = ft_strlen(limitor);
	while (1)
	{
		write(1, "> ", 2);
		line = get_next_line(0);
		if (size == ft_strlen(line) && ft_strncmp(limitor, line, size) == 0)
		{
			free(line);
			free(limitor);
			close(fd);
			get_next_line(-1);
			exit(EXIT_SUCCESS);
		}
		if (write(fd, line, ft_strlen(line)) == -1)
			error_handler("Writing lines", NULL, 1, NULL);
		free(line);
	}
	exit(EXIT_FAILURE);
}

void	heredoc(char *argv[])
{
	int	pipe_fd[2];
	int	pid;

	if (pipe(pipe_fd) == -1)
		error_handler("Laying down the pipe(s)", NULL, 1, NULL);
	pid = fork();
	if (pid == -1)
		error_handler("Fork creation", NULL, 1, NULL);
	if (pid == 0)
	{
		close(pipe_fd[0]);
		write_line(argv[2], pipe_fd[1]);
	}
	else
	{
		close(pipe_fd[1]);
		if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
			error_handler("Duplicating read-end pipe to STDOUT", NULL, 1, NULL);
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
	if (fd == -1 && option == 'I')
		perror("Error: opening file");
	return (fd);
}

int	format_check(int argc, char *argv[])
{
	if (argc > 1 && ft_strncmp(argv[1], "here_doc", 8) == 0)
	{
		if (argc < 6)
			error_handler("here_doc: Less than 5 arguments", NULL, 1, NULL);
	}
	else
	{
		if (argc < 5)
			error_handler("Less than 4 arguments provided", NULL, 1, NULL);
	}
	return (0);
}

int	main(int argc, char *argv[], char *envp[])
{
	int		j;
	int		num;
	t_px	*px;
	int		status;

	format_check(argc, argv);
	if (ft_strncmp(argv[1], "here_doc", 8) == 0)
		heredoc(argv);
	px = initialize_px(argc, argv, envp);
	num = -1;
	while (++num < px->num_commands)
		executor(px, num);
	j = -1;
	while (++j < px->num_pipes)
	{
		close(px->pipes[j][0]);
		close(px->pipes[j][1]);
	}
	num = -1;
	while (++num < px->num_commands)
		waitpid(px->pids[num], &status, 0);
	free_px(px);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (EXIT_FAILURE);
}
