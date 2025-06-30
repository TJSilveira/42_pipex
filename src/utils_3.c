/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 17:56:38 by tsilveir          #+#    #+#             */
/*   Updated: 2025/06/27 17:56:39 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int	exec_command(t_px *px, int i)
{
	int		j;
	char	**paths;
	char	*final_path;
	char	**commands;

	paths = path_extractor(px->envp);
	if (paths == NULL)
		error_handler("Error: problem envp file path", NULL, 1);
	commands = ft_split(px->argv[i + 2 + px->here_doc], ' ');
	j = 0;
	while (paths[j])
	{
		final_path = ft_strjoin_3(paths[j], '/', commands[0]);
		if (access(final_path, F_OK) == 0)
			execve_checker(final_path, commands, px->envp, paths);
		free (final_path);
		j++;
	}
	if (access(commands[0], F_OK) == 0)
		execve_checker(NULL, commands, px->envp, paths);
	free_arrays(commands);
	free_arrays(paths);
	free_px(px);
	error_handler("command not found", NULL, 127);
	return (0);
}

void	child_pipe_setup(t_px *px, int i)
{
	if (i == 0)
	{
		if (px->here_doc == 0 && dup2(px->fd_input, STDIN_FILENO) == -1)
			error_handler("Duplicating read pipe to STDIN", NULL, 1);
		if (dup2(px->pipes[0][WRITE], STDOUT_FILENO) == -1)
			error_handler("Duplicating write pipe to STDOUT\n", NULL, 1);
	}
	else if (i < px->num_pipes)
	{
		if (dup2(px->pipes[i - 1][READ], STDIN_FILENO) == -1)
			error_handler("Duplicating read pipe to STDIN", NULL, 1);
		if (dup2(px->pipes[i][WRITE], STDOUT_FILENO) == -1)
			error_handler("Duplicating write pipe to STDOUT\n", NULL, 1);
	}
	else
	{
		if (dup2(px->pipes[i - 1][READ], STDIN_FILENO) == -1)
			error_handler("Duplicating read pipe to STDIN", NULL, 1);
		if (dup2(px->fd_output, STDOUT_FILENO) == -1)
			error_handler("Duplicating write pipe to STDOUT\n", NULL, 1);
	}
}

int	executor(t_px *px, int i)
{
	int	j;

	px->pids[i] = fork();
	if (px->pids[i] == -1)
		exit(EXIT_FAILURE);
	if (px->pids[i] == 0)
	{
		child_pipe_setup(px, i);
		j = -1;
		while (++j < px->num_pipes)
		{
			close(px->pipes[j][0]);
			close(px->pipes[j][1]);
		}
		exec_command(px, i);
	}
	return (0);
}

t_px	*initialize_px(int argc, char *argv[], char *envp[])
{
	t_px	*px;

	px = malloc(sizeof(t_px));
	malloc_error_handler(px, EXIT_FAILURE);
	px->pids = malloc(sizeof(pid_t) * (argc - 3));
	malloc_error_handler(px->pids, EXIT_FAILURE);	
	px->fd_input = open_fd(argv[1], 'I');
	px->fd_output = open_fd(argv[argc - 1], 'O');
	px->num_commands = argc - 3;
	px->num_pipes = argc - 4;
	px->here_doc = 0;
	px->argc = argc;
	px->argv = argv;
	px->envp = envp;
	create_pipeline(px);
	return (px);
}
