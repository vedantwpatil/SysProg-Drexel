1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

   > **Answer**: execvp completely replaces the current process with a new program so to continue allowing our current process to continue running we first have to run fork to create a child process and then the child will call execvp

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

   > **Answer**: We return a error message -1, in my implementation i check for the return code of fork and handle it by printing out a error message saying fork failed

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

   > **Answer**: The execvp method search through the directories listed in the path environment variable

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

   > **Answer**: This helps us prevent zombie processes, meaning that the system process has terminated but it is still taking up resources

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

   > **Answer**: The exit status code from wait. It allows us to tell if a function has succeeded or failed

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

   > **Answer**: By tracking whether parsing is currently inside a quoted string. When a quote character is encountered, it toggles a boolean flag (in_quotes).

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

   > **Answer**: _start here_

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

  > **Answer**: _start here_

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

  > **Answer**: _start here_

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

  > **Answer**: _start here_
