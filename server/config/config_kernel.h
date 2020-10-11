/*
 * config_kernel.h
 *
 *  Created on: Aug 16, 2020
 *      Author: ning
 */

#ifndef CONFIG_CONFIG_SYSTEM_H_
#define CONFIG_CONFIG_SYSTEM_H_

/*
 * header
 */
#include "config_kernel_interface.h"
/*
 * define
 */
#define 	CONFIG_KERNEL_SYSTEM_PATH			"/opt/qcy/config/kernel_system.config"

/*
 * structure
 */

/*
 * function
 */
int config_kernel_read(void);
int config_kernel_get(void **t, int *size);
int config_kernel_set(int module, void *arg);
int config_kernel_get_config_status(int module);

#endif /* CONFIG_CONFIG_SYSTEM_H_ */
