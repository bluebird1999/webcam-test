/*
 * rwio.c
 *
 *  Created on: Aug 27, 2020
 *      Author: ning
 */

/*
 * header
 */
//system header
#include <pthread.h>
#include <stdio.h>
//program header
#include "../../tools/tools_interface.h"
#include "../../manager/manager_interface.h"
//server header
#include "config.h"

/*
 * static
 */
//variable

//function




/*
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 */


/*
 * interface
 */
char* read_json_file(const char *filename)
{
    //PRINT_INFO("to load %s\n", filename);
    FILE *fp = NULL;
    fp = fopen(filename, "rb");
    if (fp == NULL) {
        return NULL;
    }

    int fileSize;
    if (0 != fseek(fp, 0, SEEK_END)) {
        fclose(fp);
        return NULL;
    }
    fileSize = ftell(fp);

    char *data = NULL;
    data = malloc(fileSize);
    if(!data) {
        fclose(fp);
        return NULL;
    }
    memset(data, 0, fileSize);
    if(0 != fseek(fp, 0, SEEK_SET)) {
    	free(data);
        fclose(fp);
        return NULL;
    }
    if (fread(data, 1, fileSize, fp) != (fileSize)) {
    	free(data);
        fclose(fp);
        return NULL;
    }
    fclose(fp);
    return data;
}

int write_json_file(const char *file, const char *data)
{
    FILE *fp = NULL;
    fp = fopen(file, "wb+");
    if (fp == NULL) {
        log_err("fopen:%s fail\n", file);
        return -1;
    }

    int len = strlen(data);
    if (fwrite(data, 1, len, fp) != len) {
        log_err("fwrite:%s fail\n", file);
        fclose(fp);
        return -1;
    }

	fflush(fp);
    fclose(fp);
    return 0;
}

cJSON* load_json(const char *file)
{
    char *data = NULL;
    data = read_json_file(file);
    if (data == NULL) {
        log_info("load %s error, so to load default cfg param.\n", file);
        return NULL;
    }
    cJSON *json = NULL;
    json = cJSON_Parse(data);
    if (!json) {
        log_err("Error before: [%s]\n", cJSON_GetErrorPtr());
        free(data);
        return NULL;
    }
    free(data);
    return json;
}

int cjson_to_data_by_map(config_map_t *map, cJSON *json)
{
	int ret=0;
    int tmp;
	double temp;
	cJSON *js = NULL;

    int i =0;
    while(map[i].string_name != NULL) {
    	js = cJSON_GetObjectItem(json, map[i].string_name);
        switch (map[i].data_type) {
            case cfg_u32:
                tmp = js->valueint;
                if (tmp > map[i].max || tmp < map[i].min)
                    tmp = strtoul(map[i].default_value, NULL, 0);
                *((unsigned int *)(map[i].data_address)) = (unsigned int)tmp;
                break;
            case cfg_u16:
                tmp = js->valueint;
                if (tmp > map[i].max || tmp < map[i].min)
                    tmp = strtoul(map[i].default_value, NULL, 0);
                *((unsigned short *)(map[i].data_address)) = (unsigned short)tmp;
                break;
            case cfg_u8:
                tmp = js->valueint;
                if (tmp > map[i].max || tmp < map[i].min)
                    tmp = strtoul(map[i].default_value, NULL, 0);
                *((unsigned char *)(map[i].data_address)) = (unsigned char)tmp;
                break;
            case cfg_s32:
                tmp = js->valueint;
                if (tmp > map[i].max || tmp < map[i].min)
                    tmp = strtol(map[i].default_value, NULL, 0);
                *((int *)(map[i].data_address)) = tmp;
                break;
            case cfg_s16:
                tmp = js->valueint;
                if (tmp > map[i].max || tmp < map[i].min)
                    tmp = strtol(map[i].default_value, NULL, 0);
                *((signed short *)(map[i].data_address)) = (signed short)tmp;
                break;
            case cfg_s8:
                tmp = js->valueint;
                if (tmp > map[i].max || tmp < map[i].min)
                    tmp = strtol(map[i].default_value, NULL, 0);
                *((signed char *)(map[i].data_address)) = (signed char)tmp;
                break;
    		case cfg_float:
                temp = js->valuedouble;
                if (temp > map[i].max || temp < map[i].min)
                    temp = strtod(map[i].default_value, NULL);
                *(( float *)(map[i].data_address)) = ( float)temp;
                break;
            case cfg_string:
    			if (map[i].data_address && (map[i].max > 1)) {
    				tmp = (int)map[i].max - 1;
                    strncpy((char *)map[i].data_address, js->valuestring, tmp);
                    ((char *)map[i].data_address)[tmp] = '\0';
                } else {
                    log_err("if type is string, addr can't be null and the upper limit must greater than 1.");
                }
                break;
            default:
            	ret = 1;
                log_err("unknown data type in map definition!\n");
                break;
        }
        i++;
    }
    return ret;
}

int data_to_json_by_map(config_map_t *map, cJSON *root)
{
    int tmp;
	double temp;

    int i =0;
    while(map[i].string_name != NULL)
    {
		switch (map[i].data_type)
		{
			case cfg_u32:
				tmp = *((unsigned int *)(map[i].data_address));
				if (tmp > map[i].max || tmp < map[i].min)
					tmp = strtoul(map[i].default_value, NULL, 0);
				cJSON_AddNumberToObject(root, map[i].string_name, tmp);
				break;
			case cfg_u16:
				tmp = *((unsigned short *)(map[i].data_address));
				if (tmp > map[i].max || tmp < map[i].min)
					tmp = strtoul(map[i].default_value, NULL, 0);
				cJSON_AddNumberToObject(root, map[i].string_name, tmp);
				break;
			case cfg_u8:
				tmp = *((unsigned char *)(map[i].data_address));
				if (tmp > map[i].max || tmp < map[i].min)
					tmp = strtoul(map[i].default_value, NULL, 0);
				cJSON_AddNumberToObject(root, map[i].string_name, tmp);
				break;
			case cfg_s32:
				tmp = *((signed int *)(map[i].data_address));
				if (tmp > map[i].max || tmp < map[i].min)
					tmp = strtol(map[i].default_value, NULL, 0);
				cJSON_AddNumberToObject(root, map[i].string_name, tmp);
				break;
			case cfg_s16:
				tmp = *((signed short *)(map[i].data_address));
				if (tmp > map[i].max || tmp < map[i].min)
					tmp = strtol(map[i].default_value, NULL, 0);
				cJSON_AddNumberToObject(root, map[i].string_name, tmp);
				break;
			case cfg_s8:
				tmp = *((signed char *)(map[i].data_address));
				if (tmp > map[i].max || tmp < map[i].min)
					tmp = strtol(map[i].default_value, NULL, 0);
				cJSON_AddNumberToObject(root, map[i].string_name, tmp);
				break;
			case cfg_float:
				temp = *(( float *)(map[i].data_address));
				if (temp > map[i].max || temp < map[i].min)
					temp = strtod(map[i].default_value, NULL);
				cJSON_AddNumberToObject(root, map[i].string_name, temp);
				break;
			case cfg_string:
				cJSON_AddStringToObject(root, map[i].string_name, (char *)map[i].data_address);//
				break;
			default:
				log_err("unknown data type in map definition!\n");
				break;
		}
//	    config_add_property(map, root);
		i++;
    }
    return 0;
}

int config_add_property(config_map_t *map, cJSON *root)
{
	cJSON *Item = cJSON_CreateProperty();
	char etype[][8]={"U32","U16","U8","S32","S16","S8","FLOAT","STRING","STIME","SLICE"};
	char strname[128]={0};
	sprintf(strname,"%s%s",map->string_name,"Property");

	cJSON_AddItemToObject(Item, "type", cJSON_CreateString(etype[map->data_type]));
	cJSON_AddItemToObject(Item, "mode", cJSON_CreateString(map->mode==0?"ro":"rw"));
	cJSON_AddItemToObject(Item, "min", cJSON_CreateNumber(map->min));
	cJSON_AddItemToObject(Item, "max", cJSON_CreateNumber(map->max));
	if(map->data_type < cfg_string) {
		cJSON_AddItemToObject(Item, "def", cJSON_CreateNumber(strtoul(map->default_value, NULL, 0)));
	}
	else
		cJSON_AddItemToObject(Item, "def", cJSON_CreateString(map->default_value));
	if(map->description!=NULL)
		cJSON_AddItemToObject(Item, "opt", cJSON_CreateString(map->description));
	else
		cJSON_AddItemToObject(Item, "opt", cJSON_CreateString(""));

    cJSON_AddItemToObject(root, strname, Item);
    return 0;
}
