/*
 * config_miio_interface.h
 *
 *  Created on: Aug 28, 2020
 *      Author: ning
 */

#ifndef SERVER_CONFIG_CONFIG_MIIO_INTERFACE_H_
#define SERVER_CONFIG_CONFIG_MIIO_INTERFACE_H_

/*
 * header
 */
#include "../../manager/global_interface.h"

/*
 * define
 */
#define		CONFIG_MIIO_MODULE_NUM 	1
#define		CONFIG_MIIO_DEVICE		0
//#define		CONFIG_MIIO_IOT			1

/*
 * structure
 */
typedef struct miio_config_device_t {
	char did[MAX_SYSTEM_STRING_SIZE];
	char key[MAX_SYSTEM_STRING_SIZE];
	char vendor[MAX_SYSTEM_STRING_SIZE];
	char mac[MAX_SYSTEM_STRING_SIZE];
	char model[MAX_SYSTEM_STRING_SIZE];
	char version[MAX_SYSTEM_STRING_SIZE];		//linux
	char miio_token[2*MAX_SYSTEM_STRING_SIZE];
} miio_config_device_t;

/*
typedef struct miio_config_iot_t {
	int	camera_on;
	int camera_image_rollover;
	int camera_night_shot;
	int camera_time_watermark;
	int camera_wdr_mode;
	int camera_glimmer_full_color;
	int camera_recording_mode;
	int camera_motion_tracking;
	int indicator_on;
	int indicator_mode;
	int indicator_brightness;
	int indicator_color;
	int indicator_color_temperature;
	int indicator_flow;
	int indicator_saturability;
	int sdcard_status;
	int sdcard_total_space;
	int sdcard_free_space;
	int sdcard_used_space;
	int motion_on;
	int motion_alarm_interval;
	int motion_sensitivity;
	char motion_start_time[MAX_SYSTEM_STRING_SIZE];
	char motion_end_time[MAX_SYSTEM_STRING_SIZE];
} miio_config_iot_t;
*/

typedef struct miio_config_t {
	int							status;
	miio_config_device_t		device;
//	miio_config_iot_t			iot;
} miio_config_t;

/*
 * function
 */

#endif /* SERVER_CONFIG_CONFIG_MIIO_INTERFACE_H_ */
