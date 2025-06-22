MyShell User Manual

====================

Welcome to MyShell - a simple command line shell.
" We present "MyShell", a simple yet powerful command-line shell developed as part of the requirements for the Operating Systems course. This project aims to implement fundamental operating system concepts such as process management, I/O redirection, and environment variable management, providing an interactive "

---
Team Members

This project is the result of a collaborative effort by:

•
Nedaa Khater

•
Hanin Ostaz

•
Raghad Altalaa

•
Supervised and guided by:

Teaching Assistant : Eng. Amal Mahfouz

---

How to run MyShell:

1. Compile the shell program using:
   gcc myshell.c -o myshell

2. Run the shell by typing:
   ./myshell

3. To run commands from a batch file, type:
   ./myshell batchfile

---

Supported Internal Commands:

- cd [directory]
  Change the current directory to the specified directory.
  If no directory is given, it shows the current directory.

- clr
  Clear the screen.

- dir [directory]
  List the contents of the specified directory.

- environ
  Display all environment variables.

- echo [text]
  Display the given text.

- help
  Display this user manual.

- pause
  Pause the shell until Enter is pressed.

- quit
  Exit the shell.

---

Advanced Features:

- Input and Output Redirection:
  Use '<' to redirect input from a file.
  Use '>' to redirect output to a file (overwrite).
  Use '>>' to append output to a file.

  Example:
  ls > output.txt
  sort < input.txt > sorted.txt

- Background Execution:
  Add '&' at the end of a command to run it in the background.

  Example:
  sleep 10 &
  
- Batch File Execution :
    MyShell can read and execute commands from a text file, line by line.

- Dynamic Command Prompt:
  Shows the current directory path.

---

Notes:

- The 'help' command displays this manual.
- Make sure your batch files contain one command per line.
- If an error occurs, the shell will display an error message.
- The shell environment variable 'shell' contains the full path to myshell.
- The parent process environment variable 'parent' contains the path to myshell.

---
Suggested Future Enhancements

We believe MyShell has great potential for future development. Some ideas that can be explored include :

- Command History: Saving previous commands and allowing the user to navigate through them using arrow keys.

- Tab Completion: Automatically completing filenames and commands when the Tab key is pressed.
  
- Piping: Supporting the redirection of one command's output as input to another command (e.g., ls | grep .txt).
  
- Custom Environment Variable Support: Allowing users to set new environment variables within the shell.

- Job Control: Providing commands to manage background processes (e.g., jobs, fg, bg

---

Thank you for using MyShell!

For questions or help, contact: your_email@example.com
