/*
 * misc.h
 *
 *  Created on: Aug 27, 2020
 *      Author: ning
 */

#ifndef TOOLS_MISC_H_
#define TOOLS_MISC_H_

/*
 * header
 */

/*
 * define
 */

/*
 * structure
 */

/*
 * function
 */
int misc_generate_random_id(void);
void misc_set_thread_name(char *name);
int misc_get_bit(int a, int bit);
int misc_set_bit(int *a, int bit, int value);
int misc_clear_bit(int *a, int bit);

#endif /* TOOLS_MISC_H_ */
