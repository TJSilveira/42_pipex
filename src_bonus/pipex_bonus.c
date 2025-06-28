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

void	create_pipeline(t_px *px)
{
	int	i;

	px->pipes = malloc(sizeof(int*) * (px->argc - 4));
	i = 0;
	while (i < px->argc - 4)
	{
		px->pipes[i] = malloc(sizeof(int) * 2);
		if (!px->pipes[i])
			error_handler("malloc in pipe creation", NULL, EXIT_FAILURE);
		if(pipe(px->pipes[i]) == -1)
			error_handler("Pipe creation", NULL, EXIT_FAILURE);
		i++;
	}
}

t_px	*initialize_px(int argc, char *argv[], char *envp[])
{
	t_px	*px;

	px = malloc(sizeof(t_px));
	if (ft_strncmp(argv[1], "here_doc", 8) == 0)
		px->pids = malloc(sizeof(pid_t) * (argc - 4));
	else
		px->pids = malloc(sizeof(pid_t) * (argc - 3));
	px->argc = argc;
	px->argv = argv;
	px->envp = envp;
	create_pipeline(px);
	return(px);
}

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
			exit(EXIT_SUCCESS);
		}
		if (write(fd, line, ft_strlen(line)) == -1)
			error_handler("Writing lines", NULL, 1);
		free(line);
	}
	free(line);
	free(limitor);
	close(fd);
	exit(EXIT_FAILURE);
}

void	heredoc(char *argv[])
{
	int	pipe_fd[2];
	int	pid;

	if (pipe(pipe_fd) == -1)
		error_handler("Laying down the pipe(s)", NULL, 1);
	pid = fork();
	if (pid == -1)
		error_handler("Fork creation", NULL, 1);
	if (pid == 0)
	{
		close(pipe_fd[0]);
		write_line(argv[2], pipe_fd[1]);
	}
	else
	{
		close(pipe_fd[1]);
		if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
			error_handler("Duplicating read-end pipe to STDOUT", NULL, 1);
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
		error_handler("Error: opening file", path, 0);
	else if (fd == -1)
		error_handler("Error: opening file", NULL, 1);
	return (fd);
}

int	format_check(int argc, char *argv[])
{
	if (ft_strncmp(argv[1], "here_doc", 8) == 0)
	{
		if (argc < 6)
			error_handler("Less than 6 arguments for here_doc option", NULL, 1);
	}
	else
	{
		if (argc < 5)
			error_handler("Less than 5 arguments provided\n", NULL, 1);
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
	px = initialize_px(argc, argv, envp);
	if (ft_strncmp(argv[1], "here_doc", 8) == 0)
	{
		num = -1;
		heredoc(argv);
		px->fd_output = open_fd(argv[argc - 1], 'H');
		px->num_commands = argc - 4;
		px->num_pipes = argc - 5;
		
		while (++num < px->num_commands)
			executor_heredoc(px, num);
		j = -1;
		while (++j < px->num_pipes)
		{
			close(px->pipes[j][0]);
			close(px->pipes[j][1]);
		}
		num = -1;
		while (++num < px->num_commands)
			waitpid(px->pids[num], &status, 0);
		return (status);
	}
	else
	{
		num = -1;
		px->fd_input = open_fd(argv[1], 'I');
		px->fd_output = open_fd(argv[argc - 1], 'O');
		px->num_commands = argc - 3;
		px->num_pipes = argc - 4;

		while (++num < px->num_commands)
			executor_v2(px, num);
		j = -1;
		while (++j < px->num_pipes)
		{
			close(px->pipes[j][0]);
			close(px->pipes[j][1]);
		}
		num = -1;
		while (++num < px->num_commands)
			waitpid(px->pids[num], &status, 0);
		return (status);
	}
	
}
