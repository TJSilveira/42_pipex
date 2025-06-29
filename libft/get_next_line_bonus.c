/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/17 10:49:35 by tsilveir          #+#    #+#             */
/*   Updated: 2025/06/17 10:49:37 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*new_line(char *buffer)
{
	int		i;
	int		j;
	char	*res;

	i = 0;
	while (buffer[i] && buffer[i] != '\n')
		i++;
	if (!buffer[i])
	{
		free(buffer);
		return (NULL);
	}
	res = malloc(sizeof(char) * (ft_strlen(buffer) - i + 1));
	if (!res)
	{
		free(res);
		return (NULL);
	}
	i++;
	j = 0;
	while (buffer[i])
		res[j++] = buffer[i++];
	res[j] = 0;
	free(buffer);
	return (res);
}

char	*read_line(char *buffer)
{
	int		i;
	char	*res;

	i = 0;
	if (!buffer)
		return (NULL);
	while (buffer[i] != '\n' && buffer[i] != 0)
		i++;
	if (i == 0 && buffer[i] == 0)
		return (NULL);
	res = ft_substr(buffer, 0, i + (buffer[i] == '\n'));
	if (!res)
	{
		free(res);
		return (NULL);
	}
	return (res);
}

int	read_buffer_loop(int fd, char **buffer, char **str)
{
	char	*temp;
	int		bytes;

	bytes = 1;
	while (!ft_strchr(*buffer, '\n') && bytes != 0)
	{
		bytes = read(fd, *str, BUFFER_SIZE);
		if (bytes < 0)
		{
			free(*str);
			free(*buffer);
			return (1);
		}
		(*str)[bytes] = 0;
		temp = *buffer;
		*buffer = ft_strjoin(*buffer, *str);
		free(temp);
		if (!(*buffer))
		{
			free(*str);
			return (1);
		}
	}
	return (0);
}

char	*read_buffer(int fd, char *buffer)
{
	char	*str;

	str = malloc((BUFFER_SIZE + 1) * sizeof(char));
	if (!str)
	{
		free(str);
		return (NULL);
	}
	if (read_buffer_loop(fd, &buffer, &str))
		return (NULL);
	free(str);
	return (buffer);
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	*buffer[FD_MAX];

	if (fd < 0 || fd >= FD_MAX || BUFFER_SIZE <= 0)
		return (NULL);
	if (!buffer[fd])
		buffer[fd] = ft_strdup("");
	buffer[fd] = read_buffer(fd, buffer[fd]);
	if (!buffer[fd])
	{
		free(buffer[fd]);
		return (NULL);
	}
	line = read_line(buffer[fd]);
	buffer[fd] = new_line(buffer[fd]);
	return (line);
}
