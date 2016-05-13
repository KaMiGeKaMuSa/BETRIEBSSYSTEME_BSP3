/**
 * @file sharedmemory.h
 * sharedmemory
 * Beispiel 3
 *
 * @author Karin Kalman <karin.kalman@technikum-wien.at>
 * @author Michael Mueller <michael.mueller@technikum-wien.at>
 * @author Gerhard Sabeditsch <gerhard.sabeditsch@technikum-wien.at>
 * @date 2016/04/27
 *
 * @version $Revision: 1 $
 *
 *
 * URL: $HeadURL$
 *
 * Last Modified: $Author: Gerhard $
 */


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


/*
 * --------------------------------------------------------------- defines --
 */


#define D_KEY getuid()*1000


/*
 * --------------------------------------------------- function prototypes --
 */
char * createSegment(int shm_size);
int closeSegment(char *segment, int shm_size);
int parseParameter(int argc, char **argv);


#endif /* _SHAREDMEMORY_H_ */
