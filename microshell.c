#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

void ft_putstr_fd(char *str, int fd) 
{
    int i = -1;

    while (str[++i]) 
        write(fd, &str[i], 1);
}

void exec(char **argv, char **env, int last) {
    pid_t pid;
    int fd[2];

    if (strcmp(argv[0], "cd") == 0)
    {
        if ((!argv[1] || argv[2]))
            ft_putstr_fd("error: cd: bad arguments\n", 2);
        else 
        {
            if (chdir(argv[1]) != 0) 
            {
                ft_putstr_fd("error: cd: cannot change directory to ", 2);
                ft_putstr_fd(argv[1], 2);
                ft_putstr_fd("\n", 2);
            }
        }
        return;
    }
    if (pipe(fd) == -1) 
    {
        ft_putstr_fd("error: fatal\n", 2);
        exit(0);
    }
    pid = fork();
    if (pid < 0) 
    {
        ft_putstr_fd("error: fatal\n", 2);
        close(fd[0]);
        close(fd[1]);
        exit(0);
    }
    if (pid > 0) 
    {
        // Parent
        close(fd[1]);
        if (last == 0) {
            dup2(fd[0], STDIN_FILENO);
    }
        close(fd[0]);
        waitpid(-1, 0, 0);
    }
    else 
    {
        // Child
        close(fd[0]);
        if (last == 0)
            dup2(fd[1], STDOUT_FILENO);
        close(fd[1]);
        if (execve(argv[0], argv, env) == -1) 
        {
            ft_putstr_fd("error: cannot execute ", 2);
            ft_putstr_fd(argv[0], 2);
            ft_putstr_fd("\n", 2);
            exit (1);
        }
    }
}

int main(int argc, char **argv, char **env)
{
    int i = 1;
    int x = 1;

    while (i < argc)
    {
        if (!strcmp(argv[i], ";"))
        {
            argv[i] = NULL;
            if (argv[x] && strcmp(argv[x], ";"))
                exec(&argv[x], env, 1);
            x = i + 1;
        }
        else if (!strcmp(argv[i], "|"))
        {
            argv[i] = NULL;
            exec(&argv[x], env, 0);
            x = i + 1;
        }
        i++;
    }
    if (x < argc && argv[x])
        exec(&argv[x], env, 1);
    return (0);
}