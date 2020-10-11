/*
 * config_kernel_interface.h
 *
 *  Created on: Aug 28, 2020
 *      Author: ning
 */

#ifndef SERVER_CONFIG_CONFIG_KERNEL_INTERFACE_H_
#define SERVER_CONFIG_CONFIG_KERNEL_INTERFACE_H_

/*
 * header
 */
#include "../../manager/global_interface.h"
/*
 * define
 */
#define		CONFIG_KERNEL_MODULE_NUM	1
#define		CONFIG_KERNEL_SYSTEM		0

/*
 * structure
 */
//system info
typedef struct kernel_system_config_t {
    char    	device_name[MAX_SYSTEM_STRING_SIZE];
    char    	device_type[MAX_SYSTEM_STRING_SIZE];
    char    	chip_type[MAX_SYSTEM_STRING_SIZE];
    char    	sensor_type[MAX_SYSTEM_STRING_SIZE];
    char    	svn_version[MAX_SYSTEM_STRING_SIZE];
    char    	make_date[MAX_SYSTEM_STRING_SIZE];
    char    	upgrade_version[MAX_SYSTEM_STRING_SIZE];
} kernel_system_config_t;

typedef struct kernel_config_t {
	int							status;
	kernel_system_config_t		system;
} kernel_config_t;

/*
 * function
 */


#endif /* SERVER_CONFIG_CONFIG_KERNEL_INTERFACE_H_ */
