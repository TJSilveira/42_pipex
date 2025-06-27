/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/27 17:57:50 by tsilveir          #+#    #+#             */
/*   Updated: 2025/06/27 17:57:52 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

/* utils_1.c*/
char	**path_extractor(char **envp);
int		exec_command(char *command, char *envp[]);
void	execve_checker(char *f_path, char **comms, char *envp[], char **paths);
int		executor(char *command, char *envp[]);

/* utils_2.c*/
void	error_handler(char *msg, char *file_name, int error_code);
char	*ft_strjoin_3(const char *s1, char connector, const char *s2);
void	free_arrays(char **arrays);

/* pipex.c */
void	write_line(char *limit, int fd);
void	heredoc(char *argv[]);
int		open_fd(char *path, char option);
int		format_check(int argc, char *argv[]);
int		main(int argc, char *argv[], char *envp[]);
