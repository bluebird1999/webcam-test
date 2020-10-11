/*
 * config_audio.h
 *
 *  Created on: Sep 1, 2020
 *      Author: ning
 */

#ifndef SERVER_CONFIG_CONFIG_AUDIO_H_
#define SERVER_CONFIG_CONFIG_AUDIO_H_

/*
 * header
 */

/*
 * define
 */
#define 	CONFIG_AUDIO_PROFILE_PATH					"/opt/qcy/config/audio_profile.config"
#define 	CONFIG_AUDIO_CAPTURE_PATH					"/opt/qcy/config/audio_capture.config"

/*
 * structure
 */

/*
 * function
 */
int config_audio_read(void);
int config_audio_get(void **t, int *size);
int config_audio_set(int module, void *arg);
int config_audio_get_config_status(int module);

#endif /* SERVER_CONFIG_CONFIG_AUDIO_H_ */
