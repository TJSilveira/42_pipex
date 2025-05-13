#include "../includes/pipex.h"

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

int main(int argc, char *argv[]/*, char *envp[]*/)
{
	int	num;
	int	fd[2];

	if (ft_strncmp(argv[1],"here_doc",8) == 0)
	{
		num = 3;
		// add input function for here_doc
		fd[1] = open_fd(argv[argc - 1],'H');
	}
	else
	{
		num = 2;
		fd[0] = open_fd(argv[1],'I');
		if (dup2(fd[0], STDIN_FILENO) == -1)
			perror("Error: duplicating read-end pipe to STDIN\n");
		fd[1] = open_fd(argv[argc - 1],'O');
	}
	while (num < argc - 2)
	{
		printf("This is the argv: %s\n", argv[num]);
		num++;
	}
	printf("This is the argv: %s\n", argv[num]);
	// TODO: while loop with the creation of the different 
	return 0;
}
