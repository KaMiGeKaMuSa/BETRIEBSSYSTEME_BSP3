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
#include <unistd.h>

/*
 * --------------------------------------------------------------- defines --
 */



/*
 * --------------------------------------------------- function prototypes --
 */
int parseParameter(int argc, char **argv);
int testfunct();


#endif /* _SHAREDMEMORY_H_ */
