/*
 * config_recorder_interface.h
 *
 *  Created on: Aug 28, 2020
 *      Author: ning
 */

#ifndef SERVER_CONFIG_CONFIG_RECORDER_INTERFACE_H_
#define SERVER_CONFIG_CONFIG_RECORDER_INTERFACE_H_

/*
 * header
 */
#include "../../manager/global_interface.h"

/*
 * define
 */
#define		CONFIG_RECORDER_MODULE_NUM			1
#define		CONFIG_RECORDER_PROFILE				0

/*
 * structure
 */
typedef struct recorder_quality_t {
	int	bitrate;
	int audio_sample;
} recorder_quality_t;

typedef struct recorder_profile_config_t {
	unsigned int	enable;
	recorder_quality_t	quality[3];
	unsigned int	max_length;		//in seconds
	unsigned int	min_length;
	char			directory[MAX_SYSTEM_STRING_SIZE];
	char			normal_prefix[MAX_SYSTEM_STRING_SIZE];
	char			motion_prefix[MAX_SYSTEM_STRING_SIZE];
	char			alarm_prefix[MAX_SYSTEM_STRING_SIZE];
} recorder_profile_config_t;

typedef struct recorder_config_t {
	int	status;
	recorder_profile_config_t	profile;
} recorder_config_t;
/*
 * function
 */


#endif /* SERVER_CONFIG_CONFIG_RECORDER_INTERFACE_H_ */
