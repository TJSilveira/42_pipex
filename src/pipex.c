/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 17:56:20 by tsilveir          #+#    #+#             */
/*   Updated: 2025/06/27 17:56:23 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int	open_fd(char *path, char option)
{
	int	fd;

	fd = -1;
	if (option == 'I')
		fd = open(path, O_RDONLY);
	else if (option == 'O')
		fd = open(path, O_WRONLY | O_TRUNC | O_CREAT, 0777);
	if (fd == -1 && option == 'I')
		perror("Error: opening file");
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
		exit (1);
	}
	if (ft_strncmp(argv[0], "./pipex", 7) == 0)
		return (0);
	else
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
