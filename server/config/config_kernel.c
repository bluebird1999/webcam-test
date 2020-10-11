/*
 * config_kernel.c
 *
 *  Created on: Aug 16, 2020
 *      Author: ning
 */

/*
 * header
 */
//system header
#include <pthread.h>
#include <stdio.h>
//program header
#include "../../tools/log.h"
#include "../../manager/manager_interface.h"
//server header
#include "config.h"
#include "config_kernel.h"
#include "config_kernel_interface.h"
#include "rwio.h"

/*
 * static
 */
//variable
static pthread_rwlock_t			lock;
static int						dirty;
static kernel_config_t			kernel_config;

static config_map_t kernel_config_system_map[] = {
    {"device_name", &(kernel_config.system.device_name),	cfg_string, "qcy",0,1,32,},
    {"device_type",	&(kernel_config.system.device_type),	cfg_string, "web-camera",0,1,32,},
    {"chip_type",   &(kernel_config.system.chip_type),   	cfg_string, "realtek",0,1,32,},
    {"sensor_type", &(kernel_config.system.sensor_type), 	cfg_string, "ov2710_v10",0,1,32,},
    {"svn_version", &(kernel_config.system.svn_version), 	cfg_string, "1.0.0",0,1,32,},
    {"make_date",   &(kernel_config.system.make_date),   	cfg_string, "2020-08-10",0,1,32,},
    {NULL,},
};
//function
static int kernel_config_save(void);



/*
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 */



/*
 * interface
 */
static int kernel_config_save(void)
{
	int ret = 0;
	message_t msg;
	ret = pthread_rwlock_wrlock(&lock);
	if(ret)	{
		log_err("add lock fail, ret = %d\n", ret);
		return ret;
	}
	if( misc_get_bit(dirty, CONFIG_KERNEL_SYSTEM) ) {
		ret = write_config_file(&kernel_config_system_map, CONFIG_KERNEL_SYSTEM_PATH);
		if(!ret)
			misc_set_bit(&dirty, CONFIG_KERNEL_SYSTEM, 0);
	}
	if( !dirty ) {
		/********message body********/
		msg_init(&msg);
		msg.message = MSG_MANAGER_TIMER_REMOVE;
		msg.arg_in.handler = kernel_config_save;
		/****************************/
		manager_message(&msg);
	}
	ret = pthread_rwlock_unlock(&lock);
	if (ret)
		log_err("add unlock fail, ret = %d\n", ret);

	return ret;
}

int config_kernel_read(void)
{
	int ret,ret1=0;
	pthread_rwlock_init(&lock, NULL);
	ret = pthread_rwlock_wrlock(&lock);
	if(ret)	{
		log_err("add lock fail, ret = %d\n", ret);
		return ret;
	}
	ret = read_config_file(&kernel_config_system_map, CONFIG_KERNEL_SYSTEM_PATH);
	if(!ret)
		misc_set_bit(&kernel_config.status, CONFIG_KERNEL_SYSTEM,1);
	else
		misc_set_bit(&kernel_config.status, CONFIG_KERNEL_SYSTEM,0);
	ret1 |= ret;
	ret = pthread_rwlock_unlock(&lock);
	if (ret)
		log_err("add unlock fail, ret = %d\n", ret);
	ret1 |= ret;
	return ret1;
}

int config_kernel_get(void **t, int *size)
{
	int ret;
	ret = pthread_rwlock_wrlock(&lock);
	if(ret)	{
		log_err("add lock fail, ret = %d\n", ret);
		return ret;
	}
	*t = calloc( sizeof(kernel_config_t), 1);
	if( *t == NULL ) {
		log_err("memory allocation failed");
		return -1;
	}
	memcpy( (kernel_config_t*)(*t), &kernel_config, sizeof(kernel_config_t));
	*size = sizeof(kernel_config_t);
	ret = pthread_rwlock_unlock(&lock);
	if (ret)
		log_err("add unlock fail, ret = %d\n", ret);
	return ret;
}

int config_kernel_set(int module, void *arg)
{
	int ret = 0;
	ret = pthread_rwlock_wrlock(&lock);
	if(ret)	{
		log_err("add lock fail, ret = %d\n", ret);
		return ret;
	}
	if(dirty==0) {
		message_t msg;
	    /********message body********/
		msg_init(&msg);
		msg.message = MSG_MANAGER_TIMER_ADD;
		msg.sender = SERVER_CONFIG;
		msg.arg_in.cat = 60000;	//1min
		msg.arg_in.dog = 0;
		msg.arg_in.duck = 0;
		msg.arg_in.handler = &kernel_config_save;
		/****************************/
		manager_message(&msg);
	}
	misc_set_bit(&dirty, module, 1);
	if( module == CONFIG_KERNEL_SYSTEM) {
		memcpy( (kernel_system_config_t*)(&kernel_config.system), arg, sizeof(kernel_system_config_t));
	}
	ret = pthread_rwlock_unlock(&lock);
	if (ret)
		log_err("add unlock fail, ret = %d\n", ret);
	return ret;
}

int config_kernel_get_config_status(int module)
{
	int st,ret=0;
	ret = pthread_rwlock_wrlock(&lock);
	if(ret)	{
		log_err("add lock fail, ret = %d\n", ret);
		return ret;
	}
	if(module==-1)
		st = kernel_config.status;
	else
		st = misc_get_bit(kernel_config.status, module);
	ret = pthread_rwlock_unlock(&lock);
	if (ret)
		log_err("add unlock fail, ret = %d\n", ret);
	return st;
}
