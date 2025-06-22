MyShell User Manual

====================

Welcome to MyShell - a simple command line shell.

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

Features:

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

- Command Prompt:
  Shows the current directory path.

---

Notes:

- The 'help' command displays this manual.
- Make sure your batch files contain one command per line.
- If an error occurs, the shell will display an error message.
- The shell environment variable 'shell' contains the full path to myshell.
- The parent process environment variable 'parent' contains the path to myshell.

---

Thank you for using MyShell!

For questions or help, contact: your_email@example.com
