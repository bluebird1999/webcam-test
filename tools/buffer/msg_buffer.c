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
int msg_deep_copy(message_t *dest, message_t *source)
{
	unsigned char *block;
	if( dest->arg_size > 0 && dest->arg != NULL) {
		free(dest->arg);
	}
	if( dest->extra_size > 0 && dest->extra != NULL) {
		free(dest->extra);
	}
	memcpy( dest, source, sizeof(message_t));
	if(  source->arg_size > 0 && source->arg != NULL) {
		block = calloc( source->arg_size, 1);
		if( block == NULL ) {
			log_err("buffer memory allocation failed for size=%d", source->arg_size );
			return -1;
		}
		//deep structure copy
		dest->arg = block;
		memcpy( dest->arg, source->arg, source->arg_size );
	}
	block = NULL;
	if(  source->extra_size > 0 && source->extra != NULL) {
		block = calloc( source->extra_size, 1);
		if( block == NULL ) {
			log_err("buffer memory allocation failed for size=%d", source->extra_size );
			return -1;
		}
		//deep structure copy
		dest->extra = block;
		memcpy( dest->extra, source->extra, source->extra_size );
	}
	return 0;
}

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
	if( data == NULL) {
		return -1;
	}
	if( msg_buffer_is_empty(buff) ) {
		return 1;
	}
	//copy message
	msg_deep_copy( data, &(buff->buffer[buff->tail]));
	// free the memory allocated in the push process
	if( buff->buffer[buff->tail].arg_size > 0 && buff->buffer[buff->tail].arg != NULL) {
		free( buff->buffer[buff->tail].arg );
		buff->buffer[buff->tail].arg = NULL;
		buff->buffer[buff->tail].arg_size = 0;
	}
	if( buff->buffer[buff->tail].extra_size > 0 && buff->buffer[buff->tail].extra != NULL) {
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
	msg_deep_copy( &(buff->buffer[buff->head]), data );
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
