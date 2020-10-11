/*
 * config_audio_interface.h
 *
 *  Created on: Sep 1, 2020
 *      Author: ning
 */

#ifndef SERVER_CONFIG_CONFIG_AUDIO_INTERFACE_H_
#define SERVER_CONFIG_CONFIG_AUDIO_INTERFACE_H_

/*
 * header
 */
#include "../../manager/global_interface.h"

/*
 * define
 */
#define		CONFIG_AUDIO_MODULE_NUM			2
#define		CONFIG_AUDIO_PROFILE			0
#define		CONFIG_AUDIO_CAPTURE			1

/*
 * structure
 */
typedef struct audio_profile_t {
	int							run_mode;
} audio_profile_t;

typedef struct audio_config_t {
	int							status;
	audio_profile_t				profile;
	struct rts_audio_attr 		capture;
} audio_config_t;

/*
 * function
 */

#endif /* SERVER_CONFIG_CONFIG_AUDIO_INTERFACE_H_ */
