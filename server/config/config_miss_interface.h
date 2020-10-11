/*
 * config_miss_interface.h
 *
 *  Created on: Aug 28, 2020
 *      Author: ning
 */

#ifndef SERVER_CONFIG_CONFIG_MISS_INTERFACE_H_
#define SERVER_CONFIG_CONFIG_MISS_INTERFACE_H_

/*
 * header
 */
#include <miss.h>

#include "../../manager/global_interface.h"

/*
 * define
 */
#define		CONFIG_MISS_MODULE_NUM 		1
#define		CONFIG_MISS_PROFILE			0

#define 	CONFIG_MISS_LOG_PATH			"/mnt/log/miss.log"
/*
 * structure
 */
typedef struct miss_profile_t {
	char 	did[MAX_SYSTEM_STRING_SIZE];
	char 	key[MAX_SYSTEM_STRING_SIZE];
	char 	mac[MAX_SYSTEM_STRING_SIZE];
	char 	model[MAX_SYSTEM_STRING_SIZE];
	char 	sdk_type[MAX_SYSTEM_STRING_SIZE];
	char 	token[2*MAX_SYSTEM_STRING_SIZE];
	int		max_session_num;
	int		max_video_recv_size;
	int		max_audio_recv_size;
	int		max_video_send_size;
	int		max_audio_send_size;
} miss_profile_t;

typedef struct miss_config_t {
	int							status;
	miss_profile_t				profile;
} miss_config_t;

/*
 * function
 */

#endif /* SERVER_CONFIG_CONFIG_MISS_INTERFACE_H_ */
