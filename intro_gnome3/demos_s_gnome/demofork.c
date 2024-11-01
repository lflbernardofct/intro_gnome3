/*****************************************************************************\
 * Redes Integradas de Telecomunicacoes
 * MIEEC/MEEC - FCT NOVA  2024/2025
 *
 * demofork.c
 *
 * Demonstration program for fork command
 *
 * @author  Luis Bernardo
\*****************************************************************************/
#include <sys/types.h>
#include <sys/socket.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
//

void
reaper (int sig)
{
  sigset_t set, oldset;
  pid_t pid;
  int status;

  // Blocks other SIGCHLD signals
  sigemptyset (&set);
  sigaddset (&set, SIGCHLD);
  sigprocmask (SIG_BLOCK, &set, &oldset);

  fprintf (stderr, "reaper\n");
  while ((pid = wait3 (&status, WNOHANG, 0)) > 0)
    {
      if (WIFEXITED (status))
	fprintf (stderr, "child process (pid= %d) ended with exit(%d)\n",
		 (int) pid, (int) WEXITSTATUS (status));
      else if (WIFSIGNALED (status))
	fprintf (stderr, "child process (pid= %d) ended with kill(%d)\n",
		 (int) pid, (int) WTERMSIG (status));
      else
	fprintf (stderr, "child process (pid= %d) ended\n", (int) pid);
      continue;
    }
  // Reinstall signal handlers
  signal (SIGCHLD, reaper);
  // Unlock SIGCHLD signal
  sigemptyset (&set);
  sigaddset (&set, SIGCHLD);
  sigprocmask (SIG_UNBLOCK, &set, &oldset);
  //
  fprintf (stderr, "reaper ended\n");
}


int
main (int argc, char *argv[])
{
  int p[2];			// pipe descriptor
  char buf[256];
  int n, m;

  signal (SIGCHLD, reaper);	// set callback for SIGCHLD signal
  n= socketpair(AF_UNIX, SOCK_STREAM, 0, p);  // creates a pair of local sockets
  if (n == -1) {
    perror ("socketpair failed");
    exit (-1);
  }

  n = fork ();			// Creates subprocess
  if (n == -1) {
    perror ("fork failed");
    exit (-1);
  }
  if (n == 0) {
/***************************************************************************/
// Subprocess code
      char *msg = "Ola";
      fprintf (stderr, "filho (pid = %d)\n", (int) getpid ());
      close (p[0]);		// p[0] é used in the main process
      sleep (2);		// Sleep 2 seconds
      write (p[1], msg, strlen (msg) + 1);	// Send msg to main process
      close (p[1]);
      fprintf (stderr, "son died\n");
      _exit (1);		// Ends subprocess
/***************************************************************************/
    }
  // Main process code
  fprintf (stderr, "main process: started subprocess with pid %d\n", n);
  close (p[1]);			// p[1] is used by the subprocess
  do
    {
      m = read (p[0], buf, sizeof (buf));	// Wait for subprocess' message
    }
  while ((m == -1) && (errno == EINTR));	// Repeat if interrupted by signal
  fprintf (stderr, "Child process %d returned :%s\n", n, buf);
  return 0;
}
