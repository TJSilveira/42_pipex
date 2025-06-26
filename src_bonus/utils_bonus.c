#include "../includes/pipex.h"

void	error_handler(char *msg, char *file_name, int error_code)
{
	char *err_msg;

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

void	execve_checker(char *final_path, char **commands, char* envp[], char** paths)
{
	if (execve(final_path, commands, envp) == -1 && final_path != NULL)
	{
		free(final_path);
		free_arrays(commands);
		free_arrays(paths);
		error_handler("execve call:", NULL, 1);
	}
	else if (execve(commands[0], commands, envp) == -1 && final_path == NULL)
	{
		free_arrays(commands);
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
