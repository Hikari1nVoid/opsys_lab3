/* count-shm.c */

/* create a shared memory segment, then create a child process
    such that both parent and child are attached to the shared memory */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>

/* This constant defines the shared memory segment such that
   multiple processes can attach to this segment */
#define SHM_SHARED_KEY 8999

int main(int argc,char const * argv[])
{
  /* create the shared memory segment with a size of 4 bytes */
  key_t key = SHM_SHARED_KEY;
  int shmid = shmget( key, sizeof( int ), IPC_CREAT | IPC_EXCL | 0660 );
                        /* ^^^^^^^^^^^^^
                            request a shared memory segment of this size */
  int pids = atoi(argv[1]);
  int p;
  printf( "shmget() returned %d\n", shmid );

  if ( shmid == -1 )
  {
    perror( "shmget() failed" );
    return EXIT_FAILURE;
  }


  /* attach to the shared memory segment */
  int * data = shmat( shmid, NULL, 0 );

  if ( data == (void *)-1 )
  {
    perror( "shmat() failed" );
    return EXIT_FAILURE;
  }


  /* the memory created by shmget() is zeroed out */
  *data = 0;

  /* create a child process */
  for (int i = 0; i < pids; ++i)
  {
  	p=fork();
  	if (p==0)
  	{
  		printf("CHILD: adding 1..10000 to shared memory segment\n");
  	for (int i = 1 ; i <= 10000 ; i++ )
  {

    //printf( "%s: adding %d\n", ( p > 0 ? "PARENT" : "CHILD" ), i );


    *data += i;


   // int tmp = *data;
   // tmp += i;
   // *data = tmp;


    usleep( 10 );
  }

  		exit(0);
  	}
  	else{usleep(10);}
  }

  /* both parent and child processes will run the code below, */
  /*  both accessing (and changing the shared memory segment) */
 
  //int i, stop = 10000;


	/* code */

  
 while(pids>0){ 
    waitpid( -1, NULL, 0 );
    pids--;
}
printf( "%s: Sum 1..%d is %d\n", "PARENT", 10000, *data );

  /* detach from the shared memory segment */
  int rc = shmdt( data );

  if ( rc == -1 )
  {
    perror( "shmdt() failed" );
    exit( EXIT_FAILURE );
  }

   /* parent process only */

    /* remove the shared memory segment */
    if ( shmctl( shmid, IPC_RMID, 0 ) == -1 )
    {
      perror( "shmctl() failed" );
      exit( EXIT_FAILURE );
    }
  

  return EXIT_SUCCESS;
}