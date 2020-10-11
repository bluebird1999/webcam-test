/*
 * rwio.h
 *
 *  Created on: Aug 27, 2020
 *      Author: ning
 */

#ifndef SERVER_CONFIG_RWIO_H_
#define SERVER_CONFIG_RWIO_H_

/*
 * header
 */
#include "../../tools/cJSON/cJSON.h"
#include "config.h"

/*
 * define
 */

/*
 * structure
 */

/*
 * function
 */
char* read_json_file(const char *filename);
int write_json_file(const char *file, const char *data);
cJSON* load_json(const char *file);
int cjson_to_data_by_map(config_map_t *map, cJSON *json);
int data_to_json_by_map(config_map_t *map, cJSON *root);
int config_add_property(config_map_t *map, cJSON *root);

#endif /* SERVER_CONFIG_RWIO_H_ */
