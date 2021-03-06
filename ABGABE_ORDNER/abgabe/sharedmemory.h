/**
 * @file sharedmemory.h
 * sharedmemory
 * Beispiel 3
 *
 * @author Karin Kalman <karin.kalman@technikum-wien.at>
 * @author Michael Mueller <michael.mueller@technikum-wien.at>
 * @author Gerhard Sabeditsch <gerhard.sabeditsch@technikum-wien.at>
 * @date 2016/05/28
 *
 * @version $Revision: 1 $
 *
 *
 * URL: $HeadURL$
 *
 * Last Modified: $Author: Gerhard $
 **/


#ifndef _SHAREDMEMORY_H_
#define _SHAREDMEMORY_H_

/*
 * -------------------------------------------------------------- includes --
 */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#include <sem182.h>
#include <signal.h>


/*
 * --------------------------------------------------------------- defines --
 */


#define SHM_KEY getuid() * 1000 + 0
#define SEM_R_KEY getuid() * 1000 + 1
#define SEM_W_KEY getuid() * 1000 + 2

#define READ_MODE 0
#define WRITE_MODE 1

typedef struct {
	char *segment;
    int shmid;
    int sem_r;
	int sem_w;
	int shm_size;
	int use_mode;
} data_collect;

/*
 * --------------------------------------------------- function prototypes --
 */
data_collect createSegment(int shm_size, int shm_mode);
int closeSegment(data_collect shm_sem);
int parseParameter(int argc, char * argv[]);


#endif /* _SHAREDMEMORY_H_ */
