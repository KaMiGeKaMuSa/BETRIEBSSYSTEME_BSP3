/**
 * @file sharedfunctions.c
 * sharedmemory
 * Beispiel 3
 *
 * @author Karin Kalman <karin.kalman@technikum-wien.at>
 * @author Michael Mueller <michael.mueller@technikum-wien.at>
 * @author Gerhard Sabeditsch <gerhard.sabeditsch@technikum-wien.at>
 * @date 2016/04/17
 *
 * @version $Revision: 1 $
 *
 *
 * URL: $HeadURL$
 *
 * Last Modified: $Author: Gerhard $
 */

/**
 * -------------------------------------------------------------- includes --
 */
#include "sharedmemory.h"

//DEBUG:
#define SHM_SIZE 4  
/**
 * -------------------------------------------------------------- global static variables --
 */
int testfunct()
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

/**
 * -------------------------------------------------------------- getopt - function --
 */
int parseParameter(int argc, char **argv){
    int ret = 0, fail = 0, c;
	
    while ((c = getopt(argc, argv, "m:")) != EOF) {
        switch (c) {
        case 'm':
            ret = atoi(optarg);
            break;
        default:
            fail=1;
            break;
        }
    }
	
    if (fail) {
        fprintf(stderr,"usage: %s [-m size] ...\n", argv[0]);
        return(fail);
    }
	
	return ret;
}
