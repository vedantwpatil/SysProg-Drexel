1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?

We ensure that we close all the child processes before accepting some new user input, this is accomplished by looping through the processes

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

This ensures that we are preventing file descriptor leaks as well as proper end of file signaling in the pipeline

3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

We need it to be a built in command because we need to change the working directory of the shell process rather than a child process. By implementing cd as a child process the shell would not be aware of the directory change making for inconsistent development

4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?

We have to dynamically allocate memory to our buffer while also using a dynamically allocated array. The issue with dynmically allocating memory is that it can lead to a lot of access memory usage when resizing by doubling. Error handling becomes more difficult as well and its alot easier to make simple mistakes
