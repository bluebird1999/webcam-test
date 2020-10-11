/*
 * log.h
 *
 *  Created on: Aug 15, 2020
 *      Author: ning
 */

#ifndef TOOLS_LOG_H_
#define TOOLS_LOG_H_

/*
 * header
 */

/*
 * define
 */
#define log_debug(S,...)      printf("\n[%s-%s-%d]"S"\n",__FILE__,__func__,__LINE__, ##__VA_ARGS__)
#define log_info(S,...)      printf("\n[%s-%s-%d]"S"\n",__FILE__,__func__,__LINE__, ##__VA_ARGS__)
#define log_err(S,...)       printf("\n[%s-%s-%d]"S"\n",__FILE__,__func__,__LINE__, ##__VA_ARGS__)
#define log_warning(S,...)     printf("\n[%s-%s-%d]"S"\n",__FILE__,__func__,__LINE__, ##__VA_ARGS__)

/*
 * structure
 */

/*
 * function
 */



#endif /* TOOLS_LOG_H_ */
