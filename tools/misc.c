/*
 * misc.c
 *
 *  Created on: Aug 27, 2020
 *      Author: ning
 */


/*
 * header
 */
//system header
#include <stdio.h>
#include <pthread.h>
#include <syscall.h>
#include <sys/prctl.h>
#include <sys/time.h>
//program header

//server header
#include "misc.h"
#include "log.h"
/*
 * static
 */
//variable

//function;



/*
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 */

int misc_generate_random_id(void)
{
	struct timeval t1 = {0, 0};
    int value = 0;
    int max = 20000;
    int min = 10000;

    unsigned int srandVal;
    gettimeofday(&t1, NULL);
    srandVal = t1.tv_sec + t1.tv_usec;
	srand( srandVal );

	//[10000, 20000]
	value = rand() % (max + 1 - min) + min;

	return value;
}

void misc_set_thread_name(char *name)
{
    prctl(PR_SET_NAME, (unsigned long)name, 0,0,0);
    pid_t tid;
    tid = syscall(SYS_gettid);
    log_info("set pthread name:%d, %s, %s pid:%d tid:%d\n", __LINE__, __func__, name, getpid(), tid);

}

int misc_get_bit(int a, int bit)
{
	return ( a >> bit) & 0x01;
}

int misc_set_bit(int *a, int bit, int value)
{
	*a &= ~(0x1<<bit);
	*a |= value<<bit;
}

int misc_clear_bit(int *a, int bit)
{
	*a &= ~(0x1<<bit);
}
