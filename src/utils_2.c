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

void	error_handler(char *msg, char *file_name, int error_code)
{
	char	*err_msg;

	if (file_name == NULL)
	{
		perror(msg);
		exit(error_code);
	}
	else
	{
		err_msg = ft_strjoin("./pipex: ", file_name);
		perror(err_msg);
		free(err_msg);
		exit(error_code);
	}
}

char	*ft_strjoin_3(const char *s1, char connector, const char *s2)
{
	int		i;
	int		j;
	char	*res;

	res = malloc((ft_strlen(s1) + 2 + ft_strlen(s2)) * sizeof(char));
	if (!res)
		error_handler("Malloc problem in ft_strjoin_3 function", NULL, 1);
	i = 0;
	while (s1[i])
	{
		res[i] = s1[i];
		i++;
	}
	res[i] = connector;
	i++;
	j = 0;
	while (s2[j])
	{
		res[i + j] = s2[j];
		j++;
	}
	res[i + j] = 0;
	return (res);
}

void	free_arrays(char **arrays)
{
	int	i;

	i = 0;
	while (arrays[i])
	{
		free(arrays[i]);
		i++;
	}
	free(arrays);
}

void	free_px(t_px *px)
{
	int	i;

	i = -1;
	while (++i < px->num_pipes)
		free(px->pipes[i]);
	free(px->pipes);
	i = -1;
	free(px->pids);
	close(px->fd_input);
	close(px->fd_output);
	free(px);
}
