# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/wait.h>
# include <stdbool.h>

void	ft_putstr_fd(int fd, char *s)
{
	int	i = 0;

	while (s[i])
	{
		write(fd, &s[i], 1);
		i++;
	}
}

void	cd_builtin(char **av)
{
	if ((!av[1] || av[2]))
		ft_putstr_fd(2, "error: cd: bad arguments\n");
	else
	{
		if (chdir(av[1]) != 0)
		{
			ft_putstr_fd(2, "error: cd: cannot change directory to ");
			ft_putstr_fd(2, av[1]);
			ft_putstr_fd(2, "\n");
		}
	}
}

void	execute(char **av, char **env, bool is_last_cmd)
{
	pid_t	pid;
	int		fd[2];

	if (strcmp(av[0], "cd") == 0)
		return (cd_builtin(av));
	if (pipe(fd) == -1)
	{
		ft_putstr_fd(2, "error: fatal\n");
		exit(0);
	}
	pid = fork();
	if (pid < 0)
	{
		ft_putstr_fd(2, "error: fatal\n");
		close(fd[0]);
		close(fd[1]);
		exit(0);
	}
	if (pid > 0)
	{
		close(fd[1]);
		if (is_last_cmd == false)
			dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		waitpid(-1, 0, 0);
	}
	else
	{
		close(fd[0]);
		if (is_last_cmd == false)
			dup2(fd[1], STDOUT_FILENO);
		close(fd[1]);
		if (execve(av[0], av, env) == -1)
		{
			ft_putstr_fd(2, "error: cannot execute ");
			ft_putstr_fd(2, av[0]);
			ft_putstr_fd(2, "\n");
			exit(1);
		}
	}
}

int	main(int ac, char **av, char **env)
{
	int	i = 1;
	int j = 1;

	while (i < ac)
	{
		if ((strcmp(av[i], ";")) == 0)
		{
			av[i] = NULL;
			if (av[j] && strcmp(av[j], ";") != 0)
				execute(&av[j], env, true);
			j = i + 1;
		}
		else if ((strcmp(av[i], "|") == 0))
		{
			av[i] = NULL;
			if (av[j] && strcmp(av[j], "|") != 0)
				execute(&av[j], env, false);
			j = i + 1;
		}
		i++;
	}
	if (j < ac && av[j])
		execute(&av[j], env, true);
	return (0);
}