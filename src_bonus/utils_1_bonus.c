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

void	execve_checker(char *f_path, char **comms, char **paths, t_px *px)
{
	if (execve(f_path, comms, px->envp) == -1 && f_path != NULL)
	{
		free(f_path);
		free_arrays(comms);
		free_arrays(paths);
		free_px(px);
		error_handler("execve call:", NULL, 1);
	}
	else if (execve(comms[0], comms, px->envp) == -1 && f_path == NULL)
	{
		free_arrays(comms);
		free_arrays(paths);
		free_px(px);
		error_handler("execve call:", NULL, 1);
	}
}

void	create_pipeline(t_px *px)
{
	int	i;

	px->pipes = malloc(sizeof(int *) * (px->num_pipes));
	if (!px->pipes)
		error_handler("malloc in pipe creation", NULL, EXIT_FAILURE);
	i = 0;
	while (i < px->num_pipes)
	{
		px->pipes[i] = malloc(sizeof(int) * 2);
		if (!px->pipes[i])
			error_handler("malloc in pipe creation", NULL, EXIT_FAILURE);
		if (pipe(px->pipes[i]) == -1)
			error_handler("Pipe creation", NULL, EXIT_FAILURE);
		i++;
	}
}

void	malloc_error_handler(void *ptr, int error_code)
{
	if (!ptr)
		error_handler("Error with malloc", NULL, error_code);
}
