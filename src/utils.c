#include "../includes/pipex.h"

void	error_handler(char *msg, char *file_name)
{
	char *err_msg;

	if (file_name == NULL)
	{
		perror(msg);
		exit(EXIT_FAILURE);
	}
	else
	{
		err_msg = ft_strjoin("./pipex: ", file_name);
		perror(err_msg);
		free(err_msg);
		exit(EXIT_SUCCESS);
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
		error_handler("Malloc problem in ft_strjoin_3 function", NULL);
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

/* The else if covers the cases where the user gives the full path to the
   program. In those cases, you will not have a final_path that needs
   to be freed. You just need to call commands[0] as the first element
   of execve.*/
void	execve_checker(char *final_path, char **commands, char* envp[], char** paths)
{
	if (execve(final_path, commands, envp) == -1 && final_path != NULL)
	{
		free(final_path);
		free_arrays(commands);
		free_arrays(paths);
		error_handler("execve call:", NULL);
	}
	else if (execve(commands[0], commands, envp) == -1 && final_path == NULL)
	{
		free_arrays(commands);
		free_arrays(paths);
		error_handler("execve call:", NULL);
	}
}

void	exec_command(char *command, char *envp[])
{
	int		i;
	char	**paths;
	char	*final_path;
	char	**commands;

	paths = path_extractor(envp);
	if (paths == NULL)
		error_handler("Error: problem envp file path", NULL);
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
	free_arrays(paths);
	free_arrays(commands);
	error_handler("command not found", NULL);
}

int	executor(char *command, char *envp[])
{
	int	pipe_fd[2];
	int	pid;

	if (pipe(pipe_fd) == -1)
		error_handler("Laying down the pipe(s)", NULL);
	pid = fork();
	if (pid == -1)
		exit(EXIT_FAILURE);
	if (pid == 0)
	{
		close(pipe_fd[0]);
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
			error_handler("Duplicating write-end pipe to STDOUT\n", NULL);
		close(pipe_fd[1]);
		exec_command(command, envp);
	}
	else
	{
		close(pipe_fd[1]);
		if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
			error_handler("Duplicating read-end pipe to STDIN\n", NULL);
		close(pipe_fd[0]);
		waitpid(pid, NULL, 0);
	}
	return (0);
}

