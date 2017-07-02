/*******************************************************************************
 * Copyright (c) 2014 et Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *

 *
 * Contributors:
 *    peter - initial API and implementation and/or initial documentation
 *******************************************************************************/
#include <string.h>
#include "esp_common.h"
#include "et_fifo.h"

#define min(x,y) ({ (x) < (y) ? (x) : (y); })
#define max(x,y) ({ (x) > (y) ? (x) : (y); })

static int Heap_roundup(int size)
{
	static et_int32 multsize = 4*sizeof(et_int32);

	if (size % multsize != 0)
		size += multsize - (size % multsize);
	return size;
}


et_uint32 kfifo_init(struct kfifo *fifo,et_uint32 size)
{
	et_uchar *bufff = NULL;
	size = Heap_roundup(size);
	fifo->size = size;
	bufff = malloc(fifo->size);
	if(bufff == NULL)
	{
		os_printf("now malloc fail please check\n");
		return -1;

	}
	fifo->buffer = bufff;	
	fifo->in = 0;
	fifo->out = 0;
}

void kfifo_exit(struct kfifo *fifo)
{
	free(fifo->buffer);
}



et_uint32 kfifo_put(struct kfifo *fifo,et_uchar *buffer, et_uint32 len)	
{	
	et_uint32 l;   
  
	len = min(len, fifo->size - fifo->in + fifo->out);	 
  
	
  
	/* first put the data starting from fifo->in to buffer end */	
	l = min(len, fifo->size - (fifo->in & (fifo->size - 1)));	
	memcpy(fifo->buffer + (fifo->in & (fifo->size - 1)), buffer, l);   
  
	/* then put the rest (if any) at the beginning of the buffer */   
	memcpy(fifo->buffer, buffer + l, len - l);	 
  
	 
	fifo->in += len;   
  
	return len;   
}  
  
et_uint32 kfifo_get(struct kfifo *fifo,  et_uchar *buffer, et_uint32 len)	
{	
	et_uint32 l;   
  
	len = min(len, fifo->in - fifo->out);	
  
	
  
	/* first get the data from fifo->out until the end of the buffer */   
	l = min(len, fifo->size - (fifo->out & (fifo->size - 1)));	 
	memcpy(buffer, fifo->buffer + (fifo->out & (fifo->size - 1)), l);	
  
	/* then get the rest (if any) from the beginning of the buffer */	
	memcpy(buffer + l, fifo->buffer, len - l);	 
  
	
  
	fifo->out += len;	
  
	return len;   
}	






