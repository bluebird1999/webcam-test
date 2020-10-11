/*
 * config.c
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
#include <signal.h>
//program header
#include "../../manager/manager_interface.h"
#include "../../tools/tools_interface.h"
#include "../../server/audio/audio_interface.h"
//
#include "config.h"

#include "../../manager/global_interface.h"
#include "config_interface.h"
#include "config_miio.h"
#include "config_kernel.h"
#include "config_video.h"
#include "config_recorder.h"
#include "config_player.h"
#include "rwio.h"

/*
 * static
 */
//variable
static server_info_t 		info;
//static config_config_t	config;
static message_buffer_t	message;

//function
//common
static void *server_func(void);
static int server_message_proc(void);
static int server_none(void);
static int server_wait(void);
static int server_setup(void);
static int server_idle(void);
static int server_start(void);
static int server_run(void);
static int server_stop(void);
static int server_restart(void);
static int server_error(void);
static int server_release(void);
static int server_get_status(int type);
static int server_set_status(int type, int st);
static void server_thread_termination(void);
//specific
static int server_dispatch_message(message_t *msg, int target);
static int server_get_config(int server, void **para, int *size);
static int server_set_config(int server, int module, void* arg);
static int server_get_config_status(int server, int module);

/*
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 */

/*
 * helper
 */
static void server_thread_termination(void)
{
	message_t msg;
    /********message body********/
	msg_init(&msg);
	msg.message = MSG_CONFIG_SIGINT;
	msg.sender = msg.receiver = SERVER_CONFIG;
	/***************************/
	manager_message(&msg);
}

static int server_release(void)
{
	int ret = 0;
	msg_buffer_release(&message);
	return ret;
}

static int server_set_status(int type, int st)
{
	int ret=-1;
	ret = pthread_rwlock_wrlock(&info.lock);
	if(ret)	{
		log_err("add lock fail, ret = %d", ret);
		return ret;
	}
	if(type == STATUS_TYPE_STATUS)
		info.status = st;
	else if(type==STATUS_TYPE_EXIT)
		info.exit = st;
	ret = pthread_rwlock_unlock(&info.lock);
	if (ret)
		log_err("add unlock fail, ret = %d", ret);
	return ret;
}

static int server_get_status(int type)
{
	int st;
	int ret;
	ret = pthread_rwlock_wrlock(&info.lock);
	if(ret)	{
		log_err("add lock fail, ret = %d", ret);
		return ret;
	}
	if(type == STATUS_TYPE_STATUS)
		st = info.status;
	else if(type== STATUS_TYPE_EXIT)
		st = info.exit;
	ret = pthread_rwlock_unlock(&info.lock);
	if (ret)
		log_err("add unlock fail, ret = %d", ret);
	return st;
}


static int server_get_config(int server, void **para, int *size)
{
	int ret=0;
	switch(server) {
	case SERVER_CONFIG:
		break;
	case SERVER_DEVICE:
		break;
	case SERVER_KERNEL:
		ret = config_kernel_get( para, size);
		break;
	case SERVER_REALTEK:
		break;
	case SERVER_MIIO:
		ret = config_miio_get(para, size);
		break;
	case SERVER_MISS:
		ret = config_miss_get(para, size);
		break;
	case SERVER_MICLOUD:
		break;
	case SERVER_VIDEO:
		ret = config_video_get(para, size);
		break;
	case SERVER_AUDIO:
		ret = config_audio_get(para, size);
		break;
	case SERVER_RECORDER:
		ret = config_recorder_get(para, size);
		break;
	case SERVER_PLAYER:
		ret = config_player_get(para, size);
		break;
	}
	return ret;
}

static int server_set_config(int server, int module, void* arg)
{
	int ret = 0;
	switch(server) {
	case SERVER_CONFIG:
		break;
	case SERVER_DEVICE:
		break;
	case SERVER_KERNEL:
		ret = config_kernel_set(module,arg);
		break;
	case SERVER_REALTEK:
		break;
	case SERVER_MIIO:
		ret = config_miio_set(module,arg);
		break;
	case SERVER_MISS:
		ret = config_miss_set(module,arg);
		break;
	case SERVER_MICLOUD:
		break;
	case SERVER_VIDEO:
		ret = config_video_set(module,arg);
		break;
	case SERVER_AUDIO:
		ret = config_audio_set(module,arg);
		break;
	case SERVER_RECORDER:
		ret = config_recorder_set(module,arg);
		break;
	case SERVER_PLAYER:
		ret = config_player_set(module,arg);
		break;
	}
	return ret;
}

static int server_get_config_status(int server, int module)
{
	int st;
	switch(server) {
	case SERVER_CONFIG:
		break;
	case SERVER_DEVICE:
		break;
	case SERVER_KERNEL:
		st = config_kernel_get_config_status(module);
		break;
	case SERVER_REALTEK:
		break;
	case SERVER_MIIO:
		st = config_miio_get_config_status(module);
		break;
	case SERVER_MISS:
		st = config_miss_get_config_status(module);
		break;
	case SERVER_MICLOUD:
		break;
	case SERVER_VIDEO:
		st = config_video_get_config_status(module);
		break;
	case SERVER_AUDIO:
		st = config_audio_get_config_status(module);
		break;
	case SERVER_RECORDER:
		st = config_recorder_get_config_status(module);
		break;
	case SERVER_PLAYER:
		st = config_player_get_config_status(module);
		break;
	}
	return st;
}

static int server_dispatch_message(message_t *msg, int target)
{
	int st;
	switch(target) {
	case SERVER_CONFIG:
		break;
	case SERVER_DEVICE:
		break;
	case SERVER_KERNEL:
//		st = server_kernel_message(msg);
		break;
	case SERVER_REALTEK:
		break;
	case SERVER_MIIO:
		st = server_miio_message(msg);
		break;
	case SERVER_MISS:
		st = server_miss_message(msg);
		break;
	case SERVER_MICLOUD:
		break;
	case SERVER_VIDEO:
		st = server_video_message(msg);
		break;
	case SERVER_AUDIO:
		st = server_audio_message(msg);
		break;
	case SERVER_RECORDER:
		st = server_recorder_message(msg);
		break;
	case SERVER_PLAYER:
//		st = server_player_message(msg);
		break;
	}
	return st;
}

static int server_message_proc(void)
{
	int ret = 0, ret1 = 0;
	message_t msg;
	message_t send_msg;
	msg_init(&msg);
	msg_init(&send_msg);
	ret = pthread_rwlock_wrlock(&message.lock);
	if(ret)	{
		log_err("add message lock fail, ret = %d\n", ret);
		return ret;
	}
	ret = msg_buffer_pop(&message, &msg);
	ret1 = pthread_rwlock_unlock(&message.lock);
	if (ret1) {
		log_err("add message unlock fail, ret = %d\n", ret1);
	}
	if( ret == -1) {
		msg_free(&msg);
		return -1;
	}
	else if( ret == 1) {
		return 0;
	}
	switch(msg.message){
	case MSG_MANAGER_EXIT:
		server_set_status(STATUS_TYPE_EXIT,1);
		break;
	case MSG_CONFIG_READ:
		send_msg.arg = NULL;
		ret = server_get_config(msg.sender, &send_msg.arg, &send_msg.arg_size);
		send_msg.message = MSG_CONFIG_READ_ACK;
		send_msg.result = ret;
		server_dispatch_message(&send_msg, msg.receiver);
		break;
	case MSG_CONFIG_WRITE:
		ret = server_set_config(msg.sender, msg.arg_in.cat, msg.arg );
		break;
	case MSG_CONFIG_READ_STATUS:
		ret = server_get_config_status( msg.sender, msg.arg_in.cat );
		send_msg.message = MSG_CONFIG_READ_STATUS_ACK;
		send_msg.result = 0;
		send_msg.arg_in.cat = ret;
		server_dispatch_message(&send_msg, msg.sender);
		break;
	case MSG_MANAGER_TIMER_ACK:
		((HANDLER)msg.arg_in.handler)();
		break;
	}
	msg_free(&msg);
	return ret;
}

/*
 * State Machine
 */
static int server_none(void)
{
	int ret = 0;
	server_set_status(STATUS_TYPE_STATUS,STATUS_WAIT);
	return ret;
}

static int server_wait(void)
{
	int ret = 0;
	server_set_status(STATUS_TYPE_STATUS,STATUS_SETUP);
	return ret;
}

static int server_setup(void)
{
	int ret = 0, ret1=0;
	//load all
	ret = config_miio_read();
	ret = config_miss_read();
	ret = config_kernel_read();
	ret = config_video_read();
	ret = config_audio_read();
	ret = config_recorder_read();
	ret = config_player_read();
	server_set_status(STATUS_TYPE_STATUS,STATUS_IDLE);
	return ret;
}

static int server_idle(void)
{
	int ret = 0;
	server_set_status(STATUS_TYPE_STATUS,STATUS_START);
	return ret;
}

static int server_start(void)
{
	int ret = 0;
	server_set_status(STATUS_TYPE_STATUS,STATUS_RUN);
	return ret;
}

static int server_run(void)
{
	int ret = 0;
	server_message_proc();
	return ret;
}

static int server_stop(void)
{
	int ret = 0;
	return ret;
}

static int server_restart(void)
{
	int ret = 0;
	return ret;
}

static int server_error(void)
{
	int ret = 0;
	log_err("!!!!!!!!error in config!!!!!!!!");
	return ret;
}

static void *server_func(void)
{
    signal(SIGINT, server_thread_termination);
    signal(SIGTERM, server_thread_termination);
	misc_set_thread_name("server_config");
	pthread_detach(pthread_self());

	while( !server_get_status(STATUS_TYPE_EXIT) ) {
	switch( server_get_status(STATUS_TYPE_STATUS) ){
		case STATUS_NONE:
			server_none();
			break;
		case STATUS_WAIT:
			server_wait();
			break;
		case STATUS_SETUP:
			server_setup();
			break;
		case STATUS_IDLE:
			server_idle();
			break;
		case STATUS_START:
			server_start();
			break;
		case STATUS_RUN:
			server_run();
			break;
		case STATUS_STOP:
			server_stop();
			break;
		case STATUS_RESTART:
			server_restart();
			break;
		case STATUS_ERROR:
			server_error();
			break;
		}
	}
	server_release();
	log_info("-----------thread exit: server_config-----------");
	message_t msg;
	memset(&msg,0,sizeof(message_t));
	msg.message = MSG_MANAGER_EXIT_ACK;
	msg.sender = SERVER_CONFIG;
	manager_message(&msg);
	pthread_exit(0);
}

/*
 * internal interface
 */
int read_config_file(config_map_t *map, char *fpath)
{
	cJSON *root;
	root = load_json(fpath);
	if( cjson_to_data_by_map(map,root) ) {
		log_err("Reading mi configuration file failed: %s", fpath);
		return -1;
	}
	cJSON_Delete(root);
	return 0;
}

int write_config_file(config_map_t *map, char *fpath)
{
    cJSON *root;
    char *out;
    root = cJSON_CreateObject();
    data_to_json_by_map(map,root);
    out = cJSON_Print(root);
    int ret = write_json_file(fpath, out);
    if (ret != 0) {
        log_err("CfgWriteToFile %s error.", fpath);
        free(out);
        return -1;
    }
    free(out);
    cJSON_Delete(root);
    return 0;
}

/*
 *  external interface
 */
int server_config_start(void)
{
	int ret=-1;
	msg_buffer_init(&message, MSG_BUFFER_OVERFLOW_NO);
	pthread_rwlock_init(&info.lock, NULL);
	ret = pthread_create(&info.id, NULL, server_func, NULL);
	if(ret != 0) {
		log_err("config server create error! ret = %d",ret);
		 return ret;
	 }
	else {
		log_err("config server create successful!");
		return 0;
	}
}

int server_config_message(message_t *msg)
{
	int ret,ret1;
	if( server_get_status(STATUS_TYPE_STATUS)!= STATUS_RUN ) {
		log_err("config server is not ready!");
		return -1;
	}
	ret = pthread_rwlock_wrlock(&message.lock);
	if(ret)	{
		log_err("add message lock fail, ret = %d\n", ret);
		return ret;
	}
	ret = msg_buffer_push(&message, msg);
	log_info("push into the config message queue: sender=%d, message=%d, ret=%d", msg->sender, msg->message, ret);
	if( ret!=0 )
		log_err("message push in config error =%d", ret);
	ret1 = pthread_rwlock_unlock(&message.lock);
	if (ret1)
		log_err("add message unlock fail, ret = %d\n", ret1);
	return ret;
}
