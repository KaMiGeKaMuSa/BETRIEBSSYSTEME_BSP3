#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE 4  

int main()
{
    int key = 1001;
    char *data;
    int shmid;
    
    /*  create the segment: */
    if ((shmid = shmget(key, SHM_SIZE, IPC_CREAT | IPC_EXCL)) == -1) {
        perror("shmget");
        exit(1);
    }

    /* attach to the segment to get a pointer to it: */
    data = shmat(shmid, (void *)0, 0);
    if (data == (char *)(-1)) {
        perror("shmat");
        exit(1);
    }
    
    /* Detach the shared memory segment */
    shmdt(&data);
    
    /* Deallocate the shared memory segment.  */ 
  shmctl (shmid, IPC_RMID, 0); 

    return 0;
}
