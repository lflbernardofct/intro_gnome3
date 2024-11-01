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
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
//

#define BUFFER_SIZE		20

/**
 * Source code executed in the thread
 */
void *thread_routine ( void *ptr )
{
  char *ret_value = malloc(BUFFER_SIZE);		// Allocate the return value
  strcpy(ret_value,"hello world");	// set the return value contents

  // ptr was initialized in pthread_create argument
  int *state= (int *)ptr;
  /* function code */
  fprintf (stderr, "thread started\n");
  *state= 2; 		// Changing the state
  fprintf (stderr, "thread is sleeping for 5 seconds\n");
  sleep (5);		// Sleep 2 seconds
  fprintf (stderr, "thread stopped\n");

  pthread_exit((void*)ret_value);  // Finishes the thread and returns a string
}



int
main (int argc, char *argv[])
{
  pthread_t tid;	// thread id
  int state= 1;
  char *return_value;

  fprintf (stderr, "main: The value of state before running the thread is %d\n", state);
  /* Start the thread */
  if (pthread_create(&tid, NULL, thread_routine, (void *) &state)) {
	  fprintf(stderr, "main: error starting thread\n");
	  return 1;
  }
  fprintf (stderr, "main: started thread with id %u\n", (unsigned)tid);
  /* Wait for the thread to finish */
  if(pthread_join(tid, (void *)&return_value)) {
	  fprintf(stderr, "main: error joining thread\n");
	  return 2;
  }
  fprintf(stderr, "main: Thread %u returned : %s\n", (unsigned)tid, return_value);
  free(return_value);  // Free the return value memory allocated in the thread
  fprintf (stderr, "main: The value of state after the thread end is %d\n", state);
  return 0;
}
