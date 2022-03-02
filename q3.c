#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/stat.h>

#define SHM_SHARED_KEY 9002

int main(int argc, char const *argv[])
{
	key_t key = SHM_SHARED_KEY;
	struct stat info;
	int childs=atoi(argv[1]);

  char target= *argv[2];
  
  lstat("data.txt",&info);
  int length = info.st_size;
  printf("Reading file of %dbytes into shared memory....\n",length );
  printf("Creating %d child process for counting..\n",childs );
  int fd = open("data.txt",O_RDONLY);
	int shmid = shmget( key, 2, IPC_CREAT | IPC_EXCL | 0660 );
	  
  if ( shmid == -1 )
  {
    perror( "shmget() failed" );
    return EXIT_FAILURE;
  }
  /* attach to the shared memory segment */
  char * data = shmat( shmid, NULL, 0 );

  if ( data == (void *)-1 )
  {
    perror( "shmat() failed" );
    return EXIT_FAILURE;
  }
read(fd,data,length);
printf("%s",data );
int mission = length/childs;
for (int j = 0; j < childs; ++j)
{
	int p = fork();
	if (p==0)
	{
		int count=0;
		for (int i = 0; i < mission; ++i)
		{
			//printf("%c\n",data[i+j*mission] );
			if (data[i+j*mission]=='\0')
			{
				printf("found %d '%c' characters in range %d~%d\n",count,target,j*mission,length );
				exit(0);
			}
			if (data[i+j*mission]==target)
			{
				count++;
			}
		}
		printf("found %d '%c' characters in range%d~%d\n",count,target,j*mission,j*mission+mission );
		exit(0);
	}
}
 while(childs>0){ 
    waitpid( -1, NULL, 0 );
    childs--;
}
shmdt(data);
    if ( shmctl( shmid, IPC_RMID, 0 ) == -1 )
    {
      perror( "shmctl() failed" );
      exit( EXIT_FAILURE );
    }
    printf("ALL Done\n");
	return 0;
}