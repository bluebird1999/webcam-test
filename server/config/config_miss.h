/*
 * config_miss.h
 *
 *  Created on: Aug 28, 2020
 *      Author: ning
 */

#ifndef SERVER_CONFIG_CONFIG_MISS_H_
#define SERVER_CONFIG_CONFIG_MISS_H_

/*
 * header
 */
#include "config_miss_interface.h"

/*
 * define
 */
#define 	CONFIG_MISS_PROFILE_PATH			"/opt/qcy/config/miss_profile.config"
#define		CONFIG_MISS_DEVICE_PATH				"/etc/miio/device.conf"
#define		CONFIG_MISS_TOKEN_PATH				"/etc/miio/device.token"

/*
 * structure
 */

/*
 * function
 */
int config_miss_read(void);
int config_miss_get(void **t, int *size);
int config_miss_set(int module, void *arg);
int config_miss_get_config_status(int module);



#endif /* SERVER_CONFIG_CONFIG_MISS_H_ */
