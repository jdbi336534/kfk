/*******************************************************************************
 * Copyright (c) 2014 IBM Corp.
 *
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * and the Eclipse Distribution License is available at
 *   http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Allan Stockdill-Mander/Ian Craggs - initial API and implementation and/or initial documentation
 *******************************************************************************/

#ifndef __ET_FIFO_C_
#define __ET_FIFO_C_

#include "et_types.h"



struct kfifo {   
    et_uchar *buffer;    /* the buffer holding the data */   
    et_uint32 size;    /* the size of the allocated buffer */   
    et_uint32 in;    /* data is added at offset (in % size) */   
    et_uint32 out;    /* data is extracted from off. (out % size) */   
    //spinlock_t *lock;    /* protects concurrent modifications */   
};


et_uint32 kfifo_init(struct kfifo *fifo,et_uint32 size);
void kfifo_exit(struct kfifo *fifo);
et_uint32 kfifo_put(struct kfifo *fifo,et_uchar *buffer, et_uint32 len);
et_uint32 kfifo_get(struct kfifo *fifo,  et_uchar *buffer, et_uint32 len);


#endif
