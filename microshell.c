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
	////verif si le nb arg est bon
	if ((!av[1] || av[2]))
		ft_putstr_fd(2, "error: cd: bad arguments\n");
	else
	{
		////changer de dir et proteger fonction
		if (chdir(av[1]) != 0)
		{
			ft_putstr_fd(2, "error: cd: cannot change directory to ");
			ft_putstr_fd(2, av[1]);
			ft_putstr_fd(2, "\n");
		}
	}
	return ;
}

void	execute(char **av, char **env, bool is_last_cmd)
{
	int	pid;
	int	fd[2];

	////si c'est la buitlin
	if (strcmp(av[0], "cd") == 0)
	{
		cd_builtin(av);
		return ;
	}
	////sinon pipe
	if (pipe(fd) == -1)
	{
		ft_putstr_fd(2, "error fatal\n");
		exit(0);
	}
	////fork et protection fork
	pid = fork();
	if (pid < 0)
	{
		ft_putstr_fd(2, "error: fatal\n");
		close(fd[0]);
		close(fd[1]);
		exit(0);
	}
	////process parent : close 1, dup, waitpid
	if (pid > 0)
	{
		close(fd[1]);
		////dup si on n'est pas sur la derniere commande - in
		if (is_last_cmd == false)
			dup2(fd[0], STDIN_FILENO);
		close(fd[0]);
		waitpid(-1, 0, 0);
	}
	////process child : close 0, dup, execve
	else
	{
		close(fd[0]);
		////dup si on n'est pas sur la derniere commande - out
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
		////cas: on a un ";"
		if ((strcmp(av[i], ";")) == 0)
		{
			////met a NULL, que l'on envoie a execute jusqu'a NULL
			av[i] = NULL;
			////on verifie qu'on ne soit pas sur un autre ";"
			if (av[j] && strcmp(av[j], ";") != 0)
				execute(&av[j], env, true);
				////il s'agit bien de la derniere commande de la ligne
			j = i + 1;
		}
		////cas: on a un "|"
		else if ((strcmp(av[i], "|") == 0))
		{
			av[i] = NULL;
			execute(&av[j], env, false);
			////il y a encore des commandes apres
			j = i + 1;
		}
		i++;
	}
	////cas: une seule commande sans ";" ou "|"
	if (j < ac && av[j])
		execute(&av[j], env, true);
		////il s'agit bien de la derniere commande de la ligne, il n'y en a qu 'une
	return (0);
}