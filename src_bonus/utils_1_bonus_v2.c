/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 17:57:13 by tsilveir          #+#    #+#             */
/*   Updated: 2025/06/27 17:57:15 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int	exec_command_v2(t_px *px, int i)
{
	int		j;
	char	**paths;
	char	*final_path;
	char	**commands;

	paths = path_extractor(px->envp);
	if (paths == NULL)
		error_handler("Error: problem envp file path", NULL, 1);
	commands = ft_split(px->argv[i + px->argc - px->num_commands], ' ');
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
	/* Free px function missing */
	error_handler("command not found", NULL, 127);
	return (0);
}

int	executor_v2(t_px *px, int i)
{
	int	j;

	px->pids[i] = fork();
	if (px->pids[i] == -1)
		exit(EXIT_FAILURE);
	if (px->pids[i] == 0)
	{
		if (i == 0)
		{
			if (dup2(px->fd_input, STDIN_FILENO) == -1)
				error_handler("Duplicating read-end pipe to STDIN", NULL, 1);
			if (dup2(px->pipes[0][WRITE], STDOUT_FILENO) == -1)
				error_handler("Duplicating write-end pipe to STDOUT\n", NULL, 1);
		}
		else if (i < px->num_pipes)
		{
			if (dup2(px->pipes[i - 1][READ], STDIN_FILENO) == -1)
				error_handler("Duplicating read-end pipe to STDIN", NULL, 1);
			if (dup2(px->pipes[i][WRITE], STDOUT_FILENO) == -1)
				error_handler("Duplicating write-end pipe to STDOUT\n", NULL, 1);
		}
		else 
		{
			if (dup2(px->pipes[i - 1][READ], STDIN_FILENO) == -1)
				error_handler("Duplicating read-end pipe to STDIN", NULL, 1);
			if (dup2(px->fd_output, STDOUT_FILENO) == -1)
				error_handler("Duplicating write-end pipe to STDOUT\n", NULL, 1);
		}
		j = -1;
		while (++j < px->num_pipes)
		{
			close(px->pipes[j][0]);
			close(px->pipes[j][1]);
		}
		exec_command_v2(px, i);
	}
	return (0);
}

int	executor_heredoc(t_px *px, int i)
{
		int	j;

	px->pids[i] = fork();
	if (px->pids[i] == -1)
		exit(EXIT_FAILURE);
	if (px->pids[i] == 0)
	{
		if (i == 0)
		{
			if (dup2(px->pipes[0][WRITE], STDOUT_FILENO) == -1)
				error_handler("Duplicating write-end pipe to STDOUT\n", NULL, 1);
		}
		else 
		{
			if (dup2(px->pipes[0][READ], STDIN_FILENO) == -1)
				error_handler("Duplicating read-end pipe to STDIN", NULL, 1);
			if (dup2(px->fd_output, STDOUT_FILENO) == -1)
				error_handler("Duplicating write-end pipe to STDOUT\n", NULL, 1);
		}
		j = -1;
		while (++j < px->num_pipes)
		{
			close(px->pipes[j][0]);
			close(px->pipes[j][1]);
		}
		exec_command_v2(px, i);
	}
	return (0);
}
