#include "../includes/pipex.h"

char **path_extractor(char **envp)
{
	int		i;
	char	**paths;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 4) == 0)
		{
			paths = ft_split(envp[i] + 5,':');
			return(paths);
		}
		i++;
	}
	return(NULL);
}

char *ft_strjoin_3(const char *s1, char connector, const char *s2)
{
	int		i;
	int		j;
	char	*res;

	res = malloc((ft_strlen(s1) + 2 + ft_strlen(s2)) * sizeof(char));
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
		res[i+j] = s2[j];
		j++;
	}
	res[i+j] = 0;
	return(res);
}


int	exec_command(char *command, char *envp[])
{
	int		i;
	char 	**paths;
	char 	*final_path;
	char	**commands;

	paths = path_extractor(envp);
	if (paths == NULL)
	{
		ft_putstr_fd("Error: problem envp file path", 2);
		return (1);
	}
	commands = ft_split(command, ' ');
	i = 0;
	while (paths[i])
	{
		final_path = ft_strjoin_3(paths[i], '/', commands[0]);
		if (access(final_path, F_OK) == 0)
			execve(final_path, commands, envp);
		free (final_path);
		i++;
	}
	perror("exec_command function");
	return (1);
}

int	executor(char *command, char *envp[])
{
	int	pipe_fd[2];
	int	pid;

	if (pipe(pipe_fd) == -1)
	{
		perror("Laying down the pipe(s)");
		return (3);
	}
	pid = fork();
	if (pid == -1)
		exit(EXIT_FAILURE);
	if (pid == 0)
	{
		close(pipe_fd[0]);
		if (dup2(pipe_fd[1], STDOUT_FILENO) == -1)
			perror("Duplicating write-end pipe to STDOUT\n");
		close(pipe_fd[1]);
		exec_command(command, envp);
	}
	else
	{
		close(pipe_fd[1]);
		if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
			perror("Duplicating read-end pipe to STDOUT\n");
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
		if (size == ft_strlen(line) && ft_strncmp(limitor,line,size) == 0)
		{
			free(line);
			free(limitor);
			close(fd);
			exit(EXIT_SUCCESS);
		}
		if (write(fd, line, ft_strlen(line)) == -1)
			perror("Writing lines");
		free(line);
	}
	free(line);
	free(limitor);
	close(fd);
	exit(EXIT_FAILURE);
}

void	heredoc(char *argv[])
{
	int	pipe_fd[2];
	int	pid;

	if (pipe(pipe_fd) == -1)
	{
		perror("Laying down the pipe(s)");
		exit(3);
	}
	pid = fork();
	if (pid == -1)
		exit(EXIT_FAILURE);	
	if (pid == 0)
	{
		close(pipe_fd[0]);
		write_line(argv[2], pipe_fd[1]);
	}
	else
	{
		close(pipe_fd[1]);
		if (dup2(pipe_fd[0], STDIN_FILENO) == -1)
			perror("Duplicating read-end pipe to STDOUT\n");
		close(pipe_fd[0]);
		waitpid(pid, NULL, 0);
	}
}

int	open_fd(char *path, char option)
{
	int	fd;

	fd = -1;
	if (option == 'I')
		fd = open(path, O_RDONLY);
	else if (option == 'O')
		fd = open(path, O_WRONLY | O_TRUNC | O_CREAT, 0777);
	else if (option == 'H')
		fd = open(path, O_WRONLY | O_APPEND | O_CREAT, 0777);
	if (fd == -1)
		perror("Error: opening file\n");
	return (fd);
}

int format_check(int argc, char *argv[])
{
	if (ft_strncmp(argv[1],"here_doc",8) == 0)
	{
		if (argc < 6)
		{
			ft_putstr_fd("Less than 6 arguments for here_doc option",2);
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		if (argc < 5)
		{
			ft_putstr_fd("Less than 5 arguments provided\n",2);
			exit(EXIT_FAILURE);
		}
	}
	return (0);
}

int main(int argc, char *argv[], char *envp[])
{
	int	num;
	int	fd[2];

	format_check(argc, argv);
	if (ft_strncmp(argv[1],"here_doc",8) == 0)
	{
		num = 3;
		heredoc(argv);
		fd[1] = open_fd(argv[argc - 1],'H');
	}
	else
	{
		num = 2;
		fd[0] = open_fd(argv[1],'I');
		if (dup2(fd[0], STDIN_FILENO) == -1)
			perror("Duplicating read-end pipe to STDIN\n");
		fd[1] = open_fd(argv[argc - 1],'O');
	}
	while (num < argc - 2)
	{
		executor(argv[num], envp);
		num++;
	}
	if (dup2(fd[1], STDOUT_FILENO) == -1)
		perror("Duplicating write-end pipe to STDOUT\n");
	exec_command(argv[num],envp);
	return 0;
}
