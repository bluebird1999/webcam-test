/*
 * config_miio.h
 *
 *  Created on: Aug 16, 2020
 *      Author: ning
 */

#ifndef SERVER_CONFIG_MIIO_H_
#define SERVER_CONFIG_MIIO_H_

/*
 * header
 */
#include "config_miio_interface.h"

/*
 * define
 */
#define 	CONFIG_MIIO_IOT_PATH				"/opt/qcy/config/miio_iot.config"
#define		CONFIG_MIIO_DEVICE_PATH				"/etc/miio/device.conf"
#define		CONFIG_MIIO_TOKEN_PATH				"/etc/miio/device.token"
#define		CONFIG_MIIO_OSRELEASE_PATH			"/etc/miio/os-release"

/*
 * structure
 */

/*
 * function
 */
int config_miio_read(void);
int config_miio_get(void **t, int *size);
int config_miio_set(int module, void *arg);
int config_miio_get_config_status(int module);

#endif /* SERVER_CONFIG_MIIO_H_ */
