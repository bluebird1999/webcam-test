/*
 * recorder.c
 *
 *  Created on: Oct 6, 2020
 *      Author: ning
 */



/*
 * header
 */
//system header
#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <stdint.h>
#include <rtscamkit.h>
#include <rtsavapi.h>
#include <rtsvideo.h>
//program header
#include "../../manager/manager_interface.h"
#include "../../server/realtek/realtek_interface.h"
#include "../../tools/tools_interface.h"
#include "../../server/config/config_recorder_interface.h"
#include "../../server/recorder/recorder_interface.h"
#include "../../server/config/config_interface.h"
#include "../../server/miio/miio_interface.h"
#include "../../server/video/video_interface.h"
#include "../../server/audio/audio_interface.h"
//server header
#include "recorder.h"
#include "recorder_interface.h"

/*
 * static
 */
//variable
static 	message_buffer_t		message;
static 	server_info_t 			info;
static	recorder_config_t		config;
static 	message_buffer_t		video_buff;
static 	message_buffer_t		audio_buff;
static 	recorder_job_t			jobs[MAX_RECORDER_JOB];

//function
//common
static void *server_func(void);
static int server_message_proc(void);
static int server_release(void);
static void task_default(void);
static void task_error(void);
//specific
static int recorder_main(void);
static int recorder_send_ack(message_t *msg, int id, int receiver, int result, void *arg, int size);
static int recorder_send_message(int receiver, message_t *msg);
static int recorder_add_job( message_t* msg );
static int count_job_number(void);
static int *recorder_func(void *arg);
static int recorder_func_init_mp4v2( recorder_job_t *ctrl);
static int recorder_write_mp4_video( recorder_job_t *ctrl, message_t *msg );
static int recorder_func_close( recorder_job_t *ctrl );
static int recorder_check_finish( recorder_job_t *ctrl );
static int recorder_func_error( recorder_job_t *ctrl);
static int recorder_func_pause( recorder_job_t *ctrl);
static int recorder_destroy( recorder_job_t *ctrl );

/*
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 */

/*
 * helper
 */
static int recorder_check_finish( recorder_job_t *ctrl )
{
	int ret = 0;
	long long int now = 0;
	now = time_get_now_stamp();
	if( now >= ctrl->run.stop )
		ret = 1;
	return ret;
}
static int recorder_func_close( recorder_job_t *ctrl )
{
	char oldname[MAX_SYSTEM_STRING_SIZE*2];
	char start[MAX_SYSTEM_STRING_SIZE*2];
	char stop[MAX_SYSTEM_STRING_SIZE*2];
	char prefix[MAX_SYSTEM_STRING_SIZE];
	int ret = 0;
	if(ctrl->run.mp4_file != MP4_INVALID_FILE_HANDLE) {
		log_info("+++MP4Close\n");
		MP4Close(ctrl->run.mp4_file, MP4_CLOSE_DO_NOT_COMPUTE_BITRATE);
	}
	if( (ctrl->run.last_write - ctrl->run.real_start) < ctrl->config.profile.min_length ) {
		log_info("Recording file %s is too short, removed!", ctrl->run.file_path);
		//remove file here.
		remove(ctrl->run.file_path);
		return -1;
	}
	ctrl->run.real_stop = ctrl->run.last_write;
	memset(oldname,0,sizeof(oldname));
	memset(start,0,sizeof(start));
	memset(stop,0,sizeof(stop));
	memset(prefix,0,sizeof(prefix));
	time_stamp_to_date(ctrl->run.real_start, start);
	time_stamp_to_date(ctrl->run.last_write, stop);
	strcpy(oldname, ctrl->run.file_path);
	if( ctrl->init.type == RECORDER_TYPE_NORMAL)
		strcpy( &prefix, ctrl->config.profile.normal_prefix);
	else if( ctrl->init.type == RECORDER_TYPE_MOTION_DETECTION)
		strcpy( &prefix, ctrl->config.profile.motion_prefix);
	else if( ctrl->init.type == RECORDER_TYPE_ALARM)
		strcpy( &prefix, ctrl->config.profile.alarm_prefix);
	snprintf( ctrl->run.file_path, "%s%s/%s-%s_%s.mp4",ctrl->config.profile.directory,prefix,prefix,start,stop);
	ret = rename(oldname, ctrl->run.file_path);
	if(ret) {
		log_err("rename recording file %s to %s failed.\n", oldname, ctrl->run.file_path);
	}
	else {
		log_info("Record file is %s\n", ctrl->run.file_path);
	}
	return ret;
}

static int recorder_write_mp4_video( recorder_job_t *ctrl, message_t *msg )
{
	unsigned char *p_data = (unsigned char*)msg->extra;
	unsigned int data_length = msg->extra_size;
	av_data_info_t *info = (av_data_info_t*)(msg->arg);
	nalu_unit_t nalu;
	memset(&nalu, 0, sizeof(nalu_unit_t));
	int pos = 0, len = 0;
	while ( (len = h264_read_nalu(p_data, data_length, pos, &nalu)) != 0) {
		switch ( nalu.type) {
			case 0x07:
				if ( ctrl->run.video_track == MP4_INVALID_TRACK_ID ) {
					ctrl->run.video_track = MP4AddH264VideoTrack(ctrl->run.mp4_file, 90000,
							90000 / info->fps,
							info->width,
							info->height,
							nalu.data[1], nalu.data[2], nalu.data[3], 3);
					if( ctrl->run.video_track == MP4_INVALID_TRACK_ID ) {
						return -1;
					}
					MP4SetVideoProfileLevel( ctrl->run.mp4_file, 0x7F);
					MP4AddH264SequenceParameterSet( ctrl->run.mp4_file, ctrl->run.video_track, nalu.data, nalu.size);
					}
					break;
			case 0x08:
				if ( ctrl->run.video_track == MP4_INVALID_TRACK_ID)
					break;
				MP4AddH264PictureParameterSet(ctrl->run.mp4_file, ctrl->run.video_track, nalu.data, nalu.size);
				break;
			case 0x1:
			case 0x5:
				if ( ctrl->run.video_track == MP4_INVALID_TRACK_ID ) {
					return -1;
				}
				int nlength = nalu.size + 4;
				unsigned char *data = (unsigned char *)malloc(nlength);
				if(!data) {
					log_err("mp4_video_frame_write malloc failed\n");
					return -1;
				}
				data[0] = nalu.size >> 24;
				data[1] = nalu.size >> 16;
				data[2] = nalu.size >> 8;
				data[3] = nalu.size & 0xff;
				memcpy(data + 4, nalu.data, nalu.size);
				if (!MP4WriteSample(ctrl->run.mp4_file, ctrl->run.video_track, data, nlength, MP4_INVALID_DURATION, 0, 1)) {
				  free(data);
				  return -1;
				}
				free(data);
			break;
			  default :
				  break;
		}
		pos += len;
	}
	return 0;
}


static int recorder_func_init_mp4v2( recorder_job_t *ctrl)
{
	int ret = 0;
	char fname[MAX_SYSTEM_STRING_SIZE*2];
	char prefix[MAX_SYSTEM_STRING_SIZE];
	char timestr[MAX_SYSTEM_STRING_SIZE];
	memset( fname, 0, sizeof(fname));
	if( ctrl->init.type == RECORDER_TYPE_NORMAL)
		strcpy( &prefix, ctrl->config.profile.normal_prefix);
	else if( ctrl->init.type == RECORDER_TYPE_MOTION_DETECTION)
		strcpy( &prefix, ctrl->config.profile.motion_prefix);
	else if( ctrl->init.type == RECORDER_TYPE_ALARM)
		strcpy( &prefix, ctrl->config.profile.alarm_prefix);
	time_stamp_to_date(ctrl->run.start, timestr);
	memset(timestr,0,sizeof(timestr));
	sprintf(fname,"%s%s/%s-%s",ctrl->config.profile.directory,prefix,prefix,timestr);
	ctrl->run.mp4_file = MP4CreateEx(fname,	0, 1, 1, 0, 0, 0, 0);
	if ( ctrl->run.mp4_file == MP4_INVALID_FILE_HANDLE) {
		printf("MP4CreateEx file failed.\n");
		return -1;
	}
	MP4SetTimeScale( ctrl->run.mp4_file, 90000);
	ctrl->run.audio_track = MP4AddALawAudioTrack( ctrl->run.mp4_file, ctrl->config.profile.quality[ctrl->init.quality].audio_sample);
	if ( ctrl->run.audio_track == MP4_INVALID_TRACK_ID) {
		printf("add audio track failed.\n");
		return -1;
	}
	MP4SetTrackIntegerProperty( ctrl->run.mp4_file, ctrl->run.audio_track, "mdia.minf.stbl.stsd.alaw.channels", 1);
	memset( ctrl->run.file_path, 0, sizeof(ctrl->run.file_path));
	strcpy(ctrl->run.file_path, fname);
	return ret;
}

static int recorder_func_error( recorder_job_t *ctrl)
{
	int ret = 0;
	return ret;
}

static int recorder_func_pause( recorder_job_t *ctrl)
{
	int ret = 0;
	long long int temp = 0;
	if( ctrl->init.repeat==0 ) {
		ctrl->run.exit = 1;
		return 0;
	}
	else {
		temp = ctrl->run.start;
		ctrl->run.start = ctrl->run.stop + ctrl->init.repeat_interval;
		ctrl->run.stop = ctrl->run.start + (ctrl->run.stop - temp);
		ctrl->status = RECORDER_THREAD_STARTED;
	}
	return ret;
}

static int recorder_func_run( recorder_job_t *ctrl)
{
	message_t		vmsg, amsg;
	int ret_video, 	ret_audio, ret;
	av_data_info_t *info;
	unsigned char	*p;
	char			flag;
    //read video frame
	ret = pthread_rwlock_wrlock(&video_buff.lock);
	if(ret)	{
		log_err("add message lock fail, ret = %d\n", ret);
		return ERR_LOCK;
	}
	msg_init(&vmsg);
	ret_video = msg_buffer_pop(&video_buff, &vmsg);
	ret = pthread_rwlock_unlock(&video_buff.lock);
	if (ret) {
		log_err("add message unlock fail, ret = %d\n", ret);
		ret = ERR_LOCK;
		goto exit;
	}
    //read audio frame
	ret = pthread_rwlock_wrlock(&video_buff.lock);
	if(ret)	{
		log_err("add message lock fail, ret = %d\n", ret);
		ret = ERR_LOCK;
		goto exit;
	}
	msg_init(&amsg);
	ret_audio = msg_buffer_pop(&audio_buff, &amsg);
	ret = pthread_rwlock_unlock(&audio_buff.lock);
	if (ret) {
		log_err("add message unlock fail, ret = %d\n", ret);
		ret = ERR_LOCK;
		goto exit;
	}
	if( ret_audio && ret_video ) {	//no data
		usleep(10000);
		ret = ERR_NO_DATA;
		goto exit;
	}
	if ( !ret_audio ) {
		info = (av_data_info_t*)(amsg.arg);
		p = (unsigned char*)amsg.extra;
		if( !MP4WriteSample( ctrl->run.mp4_file, ctrl->run.audio_track, p, amsg.extra_size , 320, 0, 1) ) {
			log_err("MP4WriteSample audio failed.\n");
			ret = ERR_NO_DATA;
		}
		ctrl->run.last_write = time_get_now_stamp();
	}
	if( !ret_video ) {
		info = (av_data_info_t*)(vmsg.arg);
		p = (unsigned char*)vmsg.extra;
		flag = p[4];
		if( !ctrl->run.i_frame_read ) {
			if( h264_is_iframe( &flag )  ) {
				ctrl->run.i_frame_read = 1;
				ctrl->run.real_start = time_get_now_stamp();
				ctrl->run.fps = info->fps;
				ctrl->run.width = info->width;
				ctrl->run.height = info->height;
			}
		}
		else {
			ret = ERR_NO_DATA;
			goto exit;
		}
		if( !h264_is_iframe(&flag) && !h264_is_pframe(&flag) ) {
			ret = ERR_NO_DATA;
			goto exit;
		}
		if( info->fps != ctrl->run.fps) {
			log_err("the video fps has changed, stop recording!");
			ret = ERR_ERROR;
			goto close_exit;
		}
		if( info->width != ctrl->run.width || info->height != ctrl->run.height ) {
			log_err("the video dimention has changed, stop recording!");
			ret = ERR_ERROR;
			goto close_exit;
		}
		ret = recorder_write_mp4_video( &ctrl, &vmsg );
		if(ret < 0) {
			log_err("MP4WriteSample video failed.\n");
			ret = ERR_NO_DATA;
			goto exit;
		}
		ctrl->run.last_write = time_get_now_stamp();
		if( recorder_check_finish(ctrl) ) {
			log_info("recording finished!");
			goto close_exit;
		}
	}
exit:
	if( !ret_audio )
		msg_free(&vmsg);
    if( !ret_audio )
    	msg_free(&amsg);
    return ret;
close_exit:
	ret = recorder_func_close(ctrl);
	if( !ret )
		ctrl->status = RECORDER_THREAD_PAUSE;
	if( !ret_audio )
		msg_free(&vmsg);
    if( !ret_audio )
    	msg_free(&amsg);
    return ret;
}

static int recorder_func_started( recorder_job_t *ctrl )
{
	int ret;
	if( time_get_now_stamp() >= ctrl->run.start ) {
		ret = recorder_func_init_mp4v2( ctrl );
		if( ret ) {
			log_err("init mp4v2 failed!");
			ctrl->status == RECORDER_THREAD_ERROR;
		}
		else {
			ctrl->status == RECORDER_THREAD_RUN;
		    /********message body********/
			message_t msg;
			memset(&msg,0,sizeof(message_t));
			msg.message = MSG_VIDEO_START;
			msg.sender = msg.receiver = SERVER_RECORDER;
		    if( server_video_message(&msg)!=0 ) {
		    	log_err("video start failed from recorder!");
		    }
			memset(&msg,0,sizeof(message_t));
			msg.message = MSG_AUDIO_START;
			msg.sender = msg.receiver = SERVER_RECORDER;
		    if( server_audio_message(&msg)!=0 ) {
		    	log_err("audio start failed from recorder!");
		    }
		    /****************************/
		}
	}
	else
		usleep(1000);
	return ret;
}

static int *recorder_func(void *arg)
{
	recorder_job_t ctrl;
	memcpy(&ctrl, (recorder_job_t*)arg, sizeof(recorder_job_t));
    misc_set_thread_name("server_recorder_");
    pthread_detach(pthread_self());

    ctrl.status = RECORDER_THREAD_STARTED;
    while( !info.exit && !ctrl.run.exit ) {
    	switch( ctrl.status ) {
    		case RECORDER_THREAD_STARTED:
    			recorder_func_started(&ctrl);
    			break;
    		case RECORDER_THREAD_RUN:
    			recorder_func_run(&ctrl);
    			break;
    		case RECORDER_THREAD_PAUSE:
    			recorder_func_pause(&ctrl);
    			break;
    		case RECORDER_THREAD_ERROR:
    			recorder_func_error(&ctrl);
    			break;
    	}
    }
    //release
    recorder_destroy(&ctrl);
    log_info("-----------thread exit: server_recorder_-----------");
    pthread_exit(0);
}

static int recorder_destroy( recorder_job_t *ctrl )
{
	int ret=0,ret1;
	int i;
	ret = pthread_rwlock_wrlock(&info.lock);
	if(ret)	{
		log_err("add message lock fail, ret = %d\n", ret);
		return ret;
	}
	misc_set_bit(&info.thread_exit, ctrl->t_id, 1);
	memset(&jobs[ctrl->t_id], 0, sizeof(recorder_job_t));
	if( info.thread_exit == 0) {
	    /********message body********/
		message_t msg;
		memset(&msg,0,sizeof(message_t));
		msg.message = MSG_VIDEO_STOP;
		msg.sender = msg.receiver = SERVER_RECORDER;
	    if( server_video_message(&msg)!=0 ) {
	    	log_err("video start failed from recorder!");
	    }
		memset(&msg,0,sizeof(message_t));
		msg.message = MSG_AUDIO_STOP;
		msg.sender = msg.receiver = SERVER_RECORDER;
	    if( server_audio_message(&msg)!=0 ) {
	    	log_err("audio start failed from recorder!");
	    }
	    /****************************/
	}
	ret1 = pthread_rwlock_unlock(&info.lock);
	if (ret1)
		log_err("add message unlock fail, ret = %d\n", ret1);
	return ret;
}

static int recorder_send_message(int receiver, message_t *msg)
{
	int st;
	switch(receiver) {
	case SERVER_CONFIG:
		st = server_config_message(msg);
		break;
	case SERVER_DEVICE:
		break;
	case SERVER_KERNEL:
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
	case SERVER_AUDIO:
		st = server_audio_message(msg);
		break;
	case SERVER_RECORDER:
		break;
	case SERVER_PLAYER:
		break;
	case SERVER_MANAGER:
		st = manager_message(msg);
		break;
	}
}

static int recorder_send_ack(message_t *msg, int id, int receiver, int result, void *arg, int size)
{
	int ret = 0;
    /********message body********/
	msg_init(msg);
	msg->message = id | 0x1000;
	msg->sender = msg->receiver = SERVER_RECORDER;
	msg->result = result;
	msg->arg = arg;
	msg->arg_size = size;
	ret = recorder_send_message(receiver, msg);
	/***************************/
	return ret;
}

static int count_job_number(void)
{
	int i,num=0;
	for( i=0; i<MAX_RECORDER_JOB; i++ ) {
		if( jobs[i].status>0 )
			num++;
	}
	return num;
}

static int recorder_add_job( message_t* msg )
{
	message_t send_msg;
	int i=-1;
	int ret = 0;
	if( count_job_number() == MAX_RECORDER_JOB) {
		recorder_send_ack( &send_msg, msg->message, msg->receiver, -1, 0, 0);
		return -1;
	}
	for(i = 0;i<MAX_RECORDER_JOB;i++) {
		if( jobs[i].status == RECORDER_THREAD_NONE ) {
			memcpy( &jobs[i].init, msg->arg, sizeof(recorder_init_t));
			if (jobs[i].init.start[0] == '0') jobs[i].run.start = time_get_now_stamp();
			else jobs[i].run.start = time_date_to_stamp(jobs[i].init.start);
			if (jobs[i].init.stop[0] == '0') jobs[i].run.stop = jobs[i].run.start + config.profile.max_length;
			else jobs[i].run.stop = time_date_to_stamp(jobs[i].init.stop);
			if( (jobs[i].run.stop - jobs[i].run.start) < config.profile.min_length )
				jobs[i].run.stop = jobs[i].run.start + jobs[i].config.profile.max_length;
			jobs[i].status = RECORDER_THREAD_INITED;
			break;
		}
	}
	recorder_send_ack( &send_msg, msg->message, msg->receiver, 0, 0, 0);
	return ret;
}

static int recorder_main(void)
{
	int ret, i;
	for( i=0; i<MAX_RECORDER_JOB; i++) {
		switch( jobs[i].status ) {
			case RECORDER_THREAD_INITED:
				//start the thread
				jobs[i].t_id = i;
				memcpy( &(jobs[i].config), &config, sizeof(recorder_config_t));
				pthread_rwlock_init(&jobs[i].run.lock, NULL);
				ret = pthread_create(&jobs[i].run.pid, NULL, recorder_func, (void*)&jobs[i]);
				if(ret != 0) {
					log_err("recorder thread create error! ret = %d",ret);
					jobs[i].status = RECORDER_THREAD_NONE;
				 }
				else {
					log_info("recorder thread create successful!");
					misc_set_bit(&info.thread_start,i,1);
					jobs[i].status = RECORDER_THREAD_STARTED;
				}
				break;
			case RECORDER_THREAD_STARTED:
				break;
			case RECORDER_THREAD_ERROR:
				break;
		}
		usleep(1000);
	}
	return ret;
}

static void server_thread_termination(int sign)
{
    /********message body********/
	message_t msg;
	msg_init(&msg);
	msg.message = MSG_RECORDER_SIGINT;
	msg.sender = msg.receiver = SERVER_RECORDER;
	manager_message(&msg);
	/****************************/
}

static int server_release(void)
{
	int ret = 0;
	msg_buffer_release(&message);
	msg_buffer_release(&video_buff);
	msg_buffer_release(&audio_buff);
	msg_free(&info.task.msg);
	memset(&info,0,sizeof(server_info_t));
	memset(&config,0,sizeof(recorder_config_t));
	memset(&jobs,0,sizeof(recorder_job_t));
	return ret;
}

static int server_message_proc(void)
{
	int ret = 0, ret1 = 0;
	message_t msg,send_msg;
	msg_init(&msg);
	ret = pthread_rwlock_wrlock(&message.lock);
	if(ret)	{
		log_err("add message lock fail, ret = %d\n", ret);
		return ret;
	}
	if( info.msg_lock ) {
		ret1 = pthread_rwlock_unlock(&message.lock);
		return 0;
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
	else if( ret == 1)
		return 0;
	switch(msg.message) {
		case MSG_RECORDER_START:
			if( info.status != STATUS_RUN) {
				recorder_send_ack(&send_msg, MSG_RECORDER_START, msg.receiver, -1, 0, 0);
				break;
			}
			if( recorder_add_job(&msg) ) ret = -1;
			else ret = 0;
				recorder_send_ack(&send_msg, MSG_RECORDER_START, msg.receiver, ret, 0, 0);
			break;
		case MSG_MANAGER_EXIT:
			info.exit = 1;
			break;
		case MSG_CONFIG_READ_ACK:
			if( msg.result==0 )
				memcpy( (recorder_config_t*)(&config), (recorder_config_t*)msg.arg, msg.arg_size);
			break;
		case MSG_MANAGER_TIMER_ACK:
			((HANDLER)msg.arg_in.handler)();
			break;
		case MSG_DEVICE_GET_PARA_ACK:
/*			device_iot_config_t *temp = (device_iot_config_t*)msg.arg;
			if( temp ) {
				if( info.status == STATUS_IDLE )
					info.status = STATUS_START;
			}
*/
			break;
	}
	msg_free(&msg);
	return ret;
}

/*
 * task
 */
/*
 * task error: error->5 seconds->shut down server->msg manager
 */
static void task_error(void)
{
	unsigned int tick=0;
	switch( info.status ) {
		case STATUS_ERROR:
			log_err("!!!!!!!!error in recorder, restart in 5 s!");
			info.tick = time_get_now_ms();
			info.status = STATUS_NONE;
			break;
		case STATUS_NONE:
			tick = time_get_now_ms();
			if( (tick - info.tick) > 5000 ) {
				info.exit = 1;
				info.tick = tick;
			}
			break;
	}
	usleep(1000);
	return;
}
/*
 * default task: none->run
 */
static void task_default(void)
{
	message_t msg;
	int ret = 0;
	switch( info.status){
		case STATUS_NONE:
		    /********message body********/
			msg_init(&msg);
			msg.message = MSG_CONFIG_READ;
			msg.sender = msg.receiver = SERVER_RECORDER;
			ret = server_config_message(&msg);
			/***************************/
			if( !ret ) info.status = STATUS_WAIT;
			else sleep(1);
			break;
		case STATUS_WAIT:
			if( config.status == ( (1<<CONFIG_RECORDER_MODULE_NUM) -1 ) )
				info.status = STATUS_SETUP;
			else usleep(1000);
			break;
		case STATUS_SETUP:
			/********message body********/
			msg.message = MSG_DEVICE_GET_PARA;
			msg.sender = msg.receiver = SERVER_RECORDER;
//			ret = server_device_message(&msg);
			/****************************/
			info.status = STATUS_IDLE;
			break;
		case STATUS_IDLE:
			info.status = STATUS_START;
			break;
		case STATUS_START:
			info.status = STATUS_RUN;
			break;
		case STATUS_RUN:
			if( recorder_main() ) info.status = STATUS_ERROR;
			break;
		case STATUS_STOP:
			info.status = STATUS_IDLE;
			break;
		case STATUS_ERROR:
			info.task.func = task_error;
			break;
		default:
			break;
		}
	usleep(1000);
	return;
}

/*
 * server entry point
 */
static void *server_func(void)
{
    signal(SIGINT, server_thread_termination);
    signal(SIGTERM, server_thread_termination);
	misc_set_thread_name("server_recorder");
	pthread_detach(pthread_self());
	//default task
	info.task.func = task_default;
	info.task.start = STATUS_NONE;
	info.task.end = STATUS_RUN;
	while( !info.exit ) {
		info.task.func();
		server_message_proc();
	}
	if( info.exit ) {
		while( info.thread_exit != info.thread_start ) {
		}
	    /********message body********/
		message_t msg;
		msg_init(&msg);
		msg.message = MSG_MANAGER_EXIT_ACK;
		msg.sender = SERVER_RECORDER;
		manager_message(&msg);
		/***************************/
	}
	server_release();
	log_info("-----------thread exit: server_recorder-----------");
	pthread_exit(0);
}

/*
 * internal interface
 */

/*
 * external interface
 */
int server_recorder_start(void)
{
	int ret=-1;
	msg_buffer_init(&message, MSG_BUFFER_OVERFLOW_NO);
	pthread_rwlock_init(&info.lock, NULL);
	ret = pthread_create(&info.id, NULL, server_func, NULL);
	if(ret != 0) {
		log_err("recorder server create error! ret = %d",ret);
		 return ret;
	 }
	else {
		log_err("recorder server create successful!");
		return 0;
	}
}

int server_recorder_message(message_t *msg)
{
	int ret=0,ret1;
	ret = pthread_rwlock_wrlock(&message.lock);
	if(ret)	{
		log_err("add message lock fail, ret = %d\n", ret);
		return ret;
	}
	ret = msg_buffer_push(&message, msg);
	log_info("push into the recorder message queue: sender=%d, message=%d, ret=%d", msg->sender, msg->message, ret);
	if( ret!=0 )
		log_err("message push in recorder error =%d", ret);
	ret1 = pthread_rwlock_unlock(&message.lock);
	if (ret1)
		log_err("add message unlock fail, ret = %d\n", ret1);
	return ret;
}

int server_recorder_video_message(message_t *msg)
{
	int ret=0,ret1;
	ret = pthread_rwlock_wrlock(&video_buff.lock);
	if(ret)	{
		log_err("add message lock fail, ret = %d\n", ret);
		return ret;
	}
	ret = msg_buffer_push(&video_buff, msg);
	if( ret!=0 )
		log_err("message push in recorder error =%d", ret);
	ret1 = pthread_rwlock_unlock(&video_buff.lock);
	if (ret1)
		log_err("add message unlock fail, ret = %d\n", ret1);
	return ret;
}

int server_recorder_audio_message(message_t *msg)
{
	int ret=0,ret1=0;
	ret = pthread_rwlock_wrlock(&audio_buff.lock);
	if(ret)	{
		log_err("add message lock fail, ret = %d\n", ret);
		return ret;
	}
	ret = msg_buffer_push(&audio_buff, msg);
	if( ret!=0 )
		log_err("message push in recorder error =%d", ret);
	ret1 = pthread_rwlock_unlock(&audio_buff.lock);
	if (ret1)
		log_err("add message unlock fail, ret = %d\n", ret1);
	return ret;
}
