/*
 * config_miss.c
 *
 *  Created on: Aug 28, 2020
 *      Author: ning
 */


/*
 * header
 */
//system header
#include <pthread.h>
#include <stdio.h>
//program header
#include "../../tools/tools_interface.h"
#include "../../manager/manager_interface.h"
//server header
#include "config.h"
#include "config_miss.h"
#include "config_miss_interface.h"
#include "rwio.h"

/*
 * static
 */
static pthread_rwlock_t			lock;
static int						dirty;
static miss_config_t			miss_config;
static config_map_t miss_config_profile_map[] = {
    {"sdk_type",  				&(miss_config.profile.sdk_type),  			cfg_string, 'device',0, 0,32,    },
    {"max_session_num", 		&(miss_config.profile.max_session_num), 	cfg_s32, 128,0, -1,10000,    },
    {"max_video_recv_size",     &(miss_config.profile.max_video_recv_size), cfg_s32, 512000,0, -1,1000000,	},
    {"max_audio_recv_size",     &(miss_config.profile.max_audio_recv_size), cfg_s32, 51200,0, -1,1000000,  	},
    {"max_video_send_size",     &(miss_config.profile.max_video_send_size), cfg_s32, 512000,0, -1,1000000,	},
    {"max_audio_send_size",     &(miss_config.profile.max_audio_send_size), cfg_s32, 51200,0, -1,1000000,	},
    {NULL,},
};

//function
static int miss_config_device_read(void);
static int miss_config_device_write(void);
static int miss_config_save(void);

/*
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 */

/*
 * helper
 */
static int miss_config_save(void)
{
	int ret = 0;
	message_t msg;
	ret = pthread_rwlock_wrlock(&lock);
	if(ret)	{
		log_err("add lock fail, ret = %d\n", ret);
		return ret;
	}
	if( misc_get_bit(dirty, CONFIG_MISS_PROFILE) ) {
		ret = write_config_file(&miss_config_profile_map, CONFIG_MISS_PROFILE_PATH);
		if(!ret)
			misc_set_bit(&dirty, CONFIG_MISS_PROFILE, 0);
	}
	if( !dirty ) {
		/********message body********/
		msg_init(&msg);
		msg.message = MSG_MANAGER_TIMER_REMOVE;
		msg.arg_in.handler = miss_config_save;
		/****************************/
		manager_message(&msg);
	}
	ret = pthread_rwlock_unlock(&lock);
	if (ret)
		log_err("add unlock fail, ret = %d\n", ret);

	return ret;
}

static int miss_config_device_read(void)
{
	FILE *fp = NULL;
	int pos = 0;
	int len = 0;
	char *data = NULL;
	int fileSize = 0;
	int ret;
    memset(&miss_config.profile, 0, sizeof(miss_profile_t));
	//read device.conf
	fp = fopen(CONFIG_MISS_DEVICE_PATH, "rb");
	if (fp == NULL) {
		return -1;
	}
	if (0 != fseek(fp, 0, SEEK_END)) {
		fclose(fp);
		return -1;
	}
	fileSize = ftell(fp);
    if(fileSize > 0) {
    	data = malloc(fileSize);
    	if(!data) {
    		fclose(fp);
    		return -1;
    	}
    	memset(data, 0, fileSize);
    	if(0 != fseek(fp, 0, SEEK_SET)) {
    		free(data);
    		fclose(fp);
    		return -1;
    	}
    	if (fread(data, 1, fileSize, fp) != (fileSize)) {
    		free(data);
    		fclose(fp);
    		return -1;
    	}
    	fclose(fp);
    	char *ptr_did = 0;
    	char *ptr_key = 0;
    	char *ptr_model = 0;
    	char *ptr_mac = 0;
    	ptr_did = strstr(data, "did=");
    	ptr_key = strstr(data, "key=");
    	ptr_mac = strstr(data, "mac=");
    	ptr_model = strstr(data, "model=");
    	char *p,*m;
    	if(ptr_did&&ptr_key) {
    		len = 9;//did length
    		memcpy(miss_config.profile.did,ptr_did+4,len);
    		len = 16;//key length
    		memcpy(miss_config.profile.key,ptr_key+4,len);
    		len = 17;//mac length
    		memcpy(miss_config.profile.mac,ptr_mac+4,len);
    		p = ptr_model+6; m = miss_config.profile.model;
    		while(*p!='\n' && *p!='\0') {
    			memcpy(m, p, 1);
    			m++;p++;
    		}
    		*m = '\0';
    	}
    	free(data);
    }
	fileSize = 0;
	len = 0;
	//read device.token
	fp = fopen(CONFIG_MISS_TOKEN_PATH, "rb");
	if (fp == NULL) {
		return -1;
	}
	if (0 != fseek(fp, 0, SEEK_END)) {
		fclose(fp);
		return -1;
	}
	fileSize = ftell(fp);
    if(fileSize > 0) {
    	data = malloc(fileSize);
    	if(!data) {
    		fclose(fp);
    		return -1;
    	}
    	memset(data, 0, fileSize);
    	if(0 != fseek(fp, 0, SEEK_SET)) {
    		free(data);
    		fclose(fp);
    		return -1;
    	}
    	if (fread(data, 1, fileSize, fp) != (fileSize)) {
    		free(data);
    		fclose(fp);
    		return -1;
    	}
    	fclose(fp);
	    if(data[strlen((char*)data) - 1] == 0xa)
            data[strlen((char*)data) - 1] = 0;
		memcpy(miss_config.profile.token,data,fileSize);
    	free(data);
    }
    else {
		log_err("device.token -->file date err!!!\n");
        return -1;
    }
	return 0;
}

static int miss_config_device_write(void)
{
	int ret=0;
	return ret;
}

/*
 * interface
 */
int config_miss_read(void)
{
	int ret,ret1=0;
	pthread_rwlock_init(&lock, NULL);
	ret = pthread_rwlock_wrlock(&lock);
	if(ret)	{
		log_err("add lock fail, ret = %d\n", ret);
		return ret;
	}
	ret = miss_config_device_read();
	ret1 |= ret;
	ret = read_config_file(&miss_config_profile_map, CONFIG_MISS_PROFILE_PATH);
	if(!ret1) {
		misc_set_bit(&miss_config.status, CONFIG_MISS_PROFILE,1);
	}
	else
		misc_set_bit(&miss_config.status, CONFIG_MISS_PROFILE,0);
	ret1 |= ret;
	ret = pthread_rwlock_unlock(&lock);
	if (ret)
		log_err("add unlock fail, ret = %d\n", ret);
	ret1 |= ret;
	return ret1;
}

int config_miss_get(void **t, int *size)
{
	int ret = 0;
	ret = pthread_rwlock_wrlock(&lock);
	if(ret)	{
		log_err("add lock fail, ret = %d\n", ret);
		return ret;
	}
	*t = calloc( sizeof(miss_config_t), 1);
	if( *t == NULL ) {
		log_err("memory allocation failed");
		return -1;
	}
	memcpy( (miss_config_t*)(*t), &miss_config, sizeof(miss_config_t));
	*size = sizeof(miss_config_t);
	ret = pthread_rwlock_unlock(&lock);
	if (ret)
		log_err("add unlock fail, ret = %d\n", ret);
	return ret;
}

int config_miss_set(int module, void *arg)
{
	int ret = 0;
	ret = pthread_rwlock_wrlock(&lock);
	if(ret)	{
		log_err("add lock fail, ret = %d\n", ret);
		return ret;
	}
	if(dirty==0) {
		message_t msg;
		message_arg_t arg;
	    /********message body********/
		msg_init(&msg);
		msg.message = MSG_MANAGER_TIMER_ADD;
		msg.sender = SERVER_CONFIG;
		msg.arg_in.cat = 60000;	//1min
		msg.arg_in.dog = 0;
		msg.arg_in.duck = 0;
		msg.arg_in.handler = &miss_config_save;
		/****************************/
		manager_message(&msg);
	}
	misc_set_bit(&dirty, module, 1);
	if( module == CONFIG_MISS_PROFILE) {
		memcpy( (miss_profile_t*)(&miss_config.profile), arg, sizeof(miss_profile_t));
	}
	ret = pthread_rwlock_unlock(&lock);
	if (ret)
		log_err("add unlock fail, ret = %d\n", ret);
	return ret;
}

int config_miss_get_config_status(int module)
{
	int st,ret=0;
	ret = pthread_rwlock_wrlock(&lock);
	if(ret)	{
		log_err("add lock fail, ret = %d\n", ret);
		return ret;
	}
	if(module==-1)
		st = miss_config.status;
	else
		st = misc_get_bit(miss_config.status, module);
	ret = pthread_rwlock_unlock(&lock);
	if (ret)
		log_err("add unlock fail, ret = %d\n", ret);
	return st;
}
