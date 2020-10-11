/*
 * config.h
 *
 *  Created on: Aug 16, 2020
 *      Author: ning
 */

#ifndef CONFIG_CONFIG_H_
#define CONFIG_CONFIG_H_

/*
 * header
 */

/*
 * define
 */


/*
 * structure
 */
//config map data type
typedef enum {
	cfg_u32 = 0,
	cfg_u16,
	cfg_u8,
	cfg_s32,
	cfg_s16,
	cfg_s8,
	cfg_float,
	cfg_string,
	cfg_stime,
	cfg_slice,
} config_data_type_t;

//config map item
typedef struct config_map_t
{
    char* 					string_name;
    void* 					data_address;
    config_data_type_t		data_type;
    char*                   default_value;

    int 					mode;
    double 					min;
    double 					max;

    char* 					description;
} config_map_t;

/*
 * function
 */
int read_config_file(config_map_t *map, char *fpath);
int write_config_file(config_map_t *map, char *fpath);

#endif /* CONFIG_CONFIG_H_ */
