/*
 * message.c
 *
 *  Created on: Sep 17, 2020
 *      Author: ning
 */


/*
 * header
 */
//system header
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <ctype.h>
#include <pthread.h>
#include <unistd.h>
//program header
#include "../log.h"
//server header
#include "msg_buffer.h"

/*
 * static
 */
//variable

//function;

//specific
static int msg_buffer_is_empty(message_buffer_t *buffer);
static int msg_buffer_is_full(message_buffer_t *buffer);
static int msg_buffer_num_items(message_buffer_t *buffer);
/*
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 * %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
 */

/*
 * helper
 */
static int msg_buffer_is_empty(message_buffer_t *buffer)
{
  return (buffer->head == buffer->tail);
}

static int msg_buffer_is_full(message_buffer_t *buffer)
{
  return ((buffer->head - buffer->tail) & MSG_BUFFER_MASK) == MSG_BUFFER_MASK;
}

static int msg_buffer_num_items(message_buffer_t *buffer)
{
  return ((buffer->head - buffer->tail) & MSG_BUFFER_MASK);
}

/*
 * interface
 */
void msg_buffer_init(message_buffer_t *buff,int overflow )
{
	if( !buff->init ) {
		buff->head = 0;
		buff->tail = 0;
		pthread_rwlock_init(&buff->lock, NULL);
		buff->init = 1;
		buff->overflow = overflow;
		for(int i=0;i<MSG_BUFFER_SIZE;i++) {
			if( buff->buffer[i].arg != NULL && buff->buffer[i].arg_size>0) {
				free( buff->buffer[i].arg );
				buff->buffer[i].arg = NULL;
				buff->buffer[i].arg_size = 0;
			}
			if( buff->buffer[i].extra != NULL && buff->buffer[i].extra_size>0) {
				free( buff->buffer[i].extra );
				buff->buffer[i].extra = NULL;
				buff->buffer[i].extra_size = 0;
			}
		}
	}
}

void msg_buffer_release(message_buffer_t *buff)
{
	if( buff->init ) {
		buff->head = 0;
		buff->tail = 0;
		pthread_rwlock_destroy(&buff->lock);
		buff->init = 0;
		for(int i=0;i<MSG_BUFFER_SIZE;i++) {
			if( buff->buffer[i].arg != NULL && buff->buffer[i].arg_size>0 ) {
				free( buff->buffer[i].arg );
				buff->buffer[i].arg = NULL;
				buff->buffer[i].arg_size = 0;
			}
			if( buff->buffer[i].extra != NULL && buff->buffer[i].extra_size>0) {
				free( buff->buffer[i].extra );
				buff->buffer[i].extra = NULL;
				buff->buffer[i].extra_size = 0;
			}
		}
	}
}

int msg_buffer_pop(message_buffer_t *buff, message_t *data )
{
	unsigned char *block1 = NULL, *block2 = NULL;
	if( data == NULL) {
		return -1;
	}
	if( msg_buffer_is_empty(buff) ) {
		return 1;
	}
	//copy struct first
	memcpy( data, &(buff->buffer[buff->tail]), sizeof(message_t));
	//deep copy for arg and extra
	if( data->arg_size > 0 && data->arg != NULL) {
		block1 = calloc( data->arg_size, 1);
		if( block1 == NULL ) {
			log_err("Ring buffer memory allocation failed for size=%d", data->arg_size );
			return -1;
		}
		//deep structure copy
		data->arg = block1;
		memcpy( data->arg, buff->buffer[buff->tail].arg, data->arg_size );
		// free the memory allocated in the push process
		free( buff->buffer[buff->tail].arg );
		buff->buffer[buff->tail].arg = NULL;
		buff->buffer[buff->tail].arg_size = 0;
	}
	if( data->extra_size > 0 && data->extra != NULL) {
		block2 = calloc( data->extra_size, 1);
		if( block2 == NULL ) {
			log_err("Ring buffer memory allocation failed for size=%d", data->extra_size );
			return -1;
		}
		//deep structure copy
		data->extra = block2;
		memcpy( data->extra, buff->buffer[buff->tail].extra, data->extra_size );
		// free the memory allocated in the push process
		free( buff->buffer[buff->tail].extra );
		buff->buffer[buff->tail].extra = NULL;
		buff->buffer[buff->tail].extra_size = 0;
	}
	//update tail
	buff->tail = ((buff->tail + 1) & MSG_BUFFER_MASK);
	return 0;
}

int msg_buffer_push(message_buffer_t *buff, message_t *data)
{
	unsigned char *block1 = NULL;
	unsigned char *block2 = NULL;

	if( data == NULL) {
		return -1;
	}
	//check and replace oldest item
	if( msg_buffer_is_full(buff) ) {
		if( buff->overflow == MSG_BUFFER_OVERFLOW_YES) {
			//free old data
			if( buff->buffer[buff->tail].arg != NULL && buff->buffer[buff->tail].arg_size>0) {
				free( buff->buffer[buff->tail].arg );
				buff->buffer[buff->tail].arg = NULL;
				buff->buffer[buff->tail].arg_size = 0;
			}
			if( buff->buffer[buff->tail].extra != NULL && buff->buffer[buff->tail].extra_size>0) {
				free( buff->buffer[buff->tail].extra );
				buff->buffer[buff->tail].extra = NULL;
				buff->buffer[buff->tail].extra_size = 0;
			}
			buff->tail = ((buff->tail + 1) & MSG_BUFFER_MASK);
		}
		else
			return 1;
	}
	//data copy
	//remove older pointer to be in the safe side
	if( buff->buffer[buff->head].arg != NULL && buff->buffer[buff->head].arg_size>0)
		free( buff->buffer[buff->head].arg );
	if( buff->buffer[buff->head].extra != NULL)
		free( buff->buffer[buff->head].extra && buff->buffer[buff->head].extra_size>0);
	memcpy( &(buff->buffer[buff->head]), data, sizeof(message_t));
	//allocate memory here, and make sure to free it afterwards inside buffer function.
	if( data->arg_size > 0 && data->arg != NULL) {
		block1 = calloc( data->arg_size, 1);
		if( block1 == NULL ) {
			log_err("Ring buffer memory allocation failed for size=%d", data->arg_size );
			return -1;
		}
		//assign the new memory
		buff->buffer[buff->head].arg = block1;
		//copy data
		memcpy( buff->buffer[buff->head].arg, data->arg, data->arg_size );
	}
	if( data->extra_size > 0 && data->extra != NULL) {
		block2 = calloc( data->extra_size, 1);
		if( block2 == NULL ) {
			log_err("Ring buffer memory allocation failed for size=%d", data->extra_size );
			return -1;
		}
		//assign the new memory
		buff->buffer[buff->head].extra = block2;
		//copy data
		memcpy( buff->buffer[buff->head].extra, data->extra, data->extra_size );
	}
	//renew head
	buff->head = ((buff->head + 1) & MSG_BUFFER_MASK);
	return 0;
}

int msg_init(message_t *data)
{
	memset(data, 0, sizeof(message_t));
}

int msg_free(message_t *data)
{
	if( data == NULL )
		return -1;
	if( data->arg!=NULL && data->arg_size>0 ) {
		free(data->arg);
		data->arg = NULL;
		data->arg_size = 0;
	}
	if( data->extra!=NULL && data->extra_size>0) {
		free(data->extra);
		data->extra = NULL;
		data->extra_size = 0;
	}
	return 0;
}
