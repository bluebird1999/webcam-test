/*
 * config_recorder.h
 *
 *  Created on: Aug 16, 2020
 *      Author: ning
 */

#ifndef CONFIG_CONFIG_RECORDER_H_
#define CONFIG_CONFIG_RECORDER_H_

/*
 * header
 */
#include "config_recorder_interface.h"
/*
 * define
 */
#define 	CONFIG_RECORDER_PROFILE_PATH				"/opt/qcy/config/recorder_profile.config"

/*
 * structure
 */

/*
 * function
 */
int config_recorder_read(void);
int config_recorder_get(void **t, int *size);
int config_recorder_set(int module, void *arg);
int config_recorder_get_config_status(int module);

#endif /* CONFIG_CONFIG_RECORDER_H_ */
