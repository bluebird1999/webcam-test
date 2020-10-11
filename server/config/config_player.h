/*
 * config_player.h
 *
 *  Created on: Aug 16, 2020
 *      Author: ning
 */

#ifndef CONFIG_CONFIG_PLAYER_H_
#define CONFIG_CONFIG_PLAYER_H_

/*
 * header
 */
#include "config_player_interface.h"
/*
 * define
 */
#define 	CONFIG_PLAYER_PROFILE_PATH			"/opt/qcy/config/player_profile.config"

/*
 * structure
 */

/*
 * function
 */
int config_player_read(void);
int config_player_get(void **t, int *size);
int config_player_set(int module, void *arg);
int config_player_get_config_status(int module);

#endif /* CONFIG_CONFIG_PLAYER_H_ */
