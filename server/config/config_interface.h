/*
 * config_interface.h
 *
 *  Created on: Aug 28, 2020
 *      Author: ning
 */

#ifndef SERVER_CONFIG_CONFIG_INTERFACE_H_
#define SERVER_CONFIG_CONFIG_INTERFACE_H_

/*
 * header
 */
#include "../../manager/manager_interface.h"

/*
 * define
 */
#define		SERVER_CONFIG_VERSION_STRING			"alpha-3.1"

#define		MSG_CONFIG_BASE						(SERVER_CONFIG<<16)
#define		MSG_CONFIG_SIGINT					MSG_CONFIG_BASE | 0x0000
#define		MSG_CONFIG_SIGINT_ACK				MSG_CONFIG_BASE | 0x1000
#define		MSG_CONFIG_READ						MSG_CONFIG_BASE | 0X0010
#define		MSG_CONFIG_READ_ACK					MSG_CONFIG_BASE | 0X1010
#define		MSG_CONFIG_WRITE					MSG_CONFIG_BASE | 0X0011
#define		MSG_CONFIG_WRITE_ACK				MSG_CONFIG_BASE | 0X1011
#define		MSG_CONFIG_READ_STATUS				MSG_CONFIG_BASE | 0X0012
#define		MSG_CONFIG_READ_STATUS_ACK			MSG_CONFIG_BASE | 0X1012

/*
 * structure
 */


/*
 * function
 */
int server_config_start(void);

#endif /* SERVER_CONFIG_CONFIG_INTERFACE_H_ */
