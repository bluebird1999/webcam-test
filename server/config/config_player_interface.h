/*
 * config_player_interface.h
 *
 *  Created on: Aug 28, 2020
 *      Author: ning
 */

#ifndef SERVER_CONFIG_CONFIG_PLAYER_INTERFACE_H_
#define SERVER_CONFIG_CONFIG_PLAYER_INTERFACE_H_

/*
 * header
 */
#include "../../manager/global_interface.h"
/*
 * define
 */
#define		CONFIG_PLAYER_PROFILE				0

/*
 * structure
 */
//system info
typedef struct player_profile_config_t {
	int	player_mode;
} player_profile_config_t;

typedef struct player_config_t {
	int	status;
	player_profile_config_t	profile;
} player_config_t;
/*
 * function
 */


#endif /* SERVER_CONFIG_CONFIG_PLAYER_INTERFACE_H_ */
