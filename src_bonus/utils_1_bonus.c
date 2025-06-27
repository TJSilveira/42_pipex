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

char	**path_extractor(char **envp)
{
	int		i;
	char	**paths;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 4) == 0)
		{
			paths = ft_split(envp[i] + 5, ':');
			return (paths);
		}
		i++;
	}
	return (NULL);
}

void	execve_checker(char *f_path, char **comms, char *envp[], char **paths)
{
	if (execve(f_path, comms, envp) == -1 && f_path != NULL)
	{
		free(f_path);
		free_arrays(comms);
		free_arrays(paths);
		error_handler("execve call:", NULL, 1);
	}
	else if (execve(comms[0], comms, envp) == -1 && f_path == NULL)
	{
		free_arrays(comms);
		free_arrays(paths);
		error_handler("execve call:", NULL, 1);
	}
}

int	exec_command(char *command, char *envp[])
{
	int		i;
	char	**paths;
	char	*final_path;
	char	**commands;

	paths = path_extractor(envp);
	if (paths == NULL)
		error_handler("Error: problem envp file path", NULL, 1);
	commands = ft_split(command, ' ');
	i = 0;
	while (paths[i])
	{
		final_path = ft_strjoin_3(paths[i], '/', commands[0]);
		if (access(final_path, F_OK) == 0)
			execve_checker(final_path, commands, envp, paths);
		free (final_path);
		i++;
	}
	if (access(commands[0], F_OK) == 0)
		execve_checker(NULL, commands, envp, paths);
	free_arrays(commands);
	free_arrays(paths);
	error_handler("command not found", NULL, 127);
	return (0);
}

int	executor(char *command, char *envp[])
{
	int	pipe_fd[2];
	int	pid;

	if (pipe(pipe_fd) == -1)
		error_handler("Laying down the pipe(s)", NULL, 1);
	pid = fork();
	if (pid == -1)
		exit(EXIT_FAILURE);
	if (pid == 0)
	{
		close(pipe_fd[0]);
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
			error_handler("Duplicating write-end pipe to STDOUT\n", NULL, 1);
		close(pipe_fd[1]);
		exec_command(command, envp);
	}
	else
	{
		close(pipe_fd[1]);
		if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
			error_handler("Duplicating read-end pipe to STDIN\n", NULL, 1);
		close(pipe_fd[0]);
		waitpid(pid, NULL, 0);
	}
	return (0);
}
