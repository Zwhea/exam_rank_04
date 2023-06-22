# Exam Details

Write a program that will behave like executing a shell command.

# Expected File
```
microshell.c
```
# Allowed Functions
```
malloc, free, write, close, fork, waitpid, signal, kill, exit, chdir, execve, dup, dup2, pipe, strcmp, strncmp
```

# Prototype
```
int main(int argc, char **argv, char **env)
```

# Ressources

Test your filedescriptors leaks with 
```
lsof -c ./microshell /bin/ls "|" /usr/bin/grep microshell ";" /bin/echo i love my microshell
```
* [Fd leaks examples](https://www.thegeekstuff.com/2012/08/lsof-command-examples/)

<br>

# Exam Norminette

There is no Norminette in this exam. 

<br>

# Exam Login Details

    login -> exam
    password -> exam

Type examshell in terminal

    login -> your 42 intra username (eg. prossi)
    pasword -> your password

Allow photo check

In the  examshell, you have only the following 3 commands:

- status - check the time and other things

- grademe - checking pushed exercise (moulinet)

- finish - closing examshell and finish exam

Open new terminal

    You can find your exam subject in the directory subject
    Then you can see directory Rendu. It is your repository.
    You must create a folder named by the project and start working.
    
# Exam Submission Details

When you are ready to sumbit your function/program, go into Rendu and push the folder you created by doing the following:

1. git add .

2. git status

2. git commit -m "any commit message you like"

3. git push

4. After you git push, type `grademe` in the examshell for your project to be submitted. 

<br>

# Exam IDE

You should be able to use both VSCode and Vim in the exam.


<br>

# Exam Practice

Practice the exam just like you would in the real exam using this tool - [Grademe](https://skillicons.dev)