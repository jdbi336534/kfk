/**
 * Copyright (c) 2016 ET iLink.
 * All rights reserved.
 */
/**
 *
 * @file et_rtos.h
 * @brief
 *  本文件包含的接口为RTOS接口，本接口与et_config.h中的配置相关
 * @see ET_CONFIG_RTOS_EN = 1，表明用户将SDK移植到具备RTOS的平台，此时必须实现
 * @see ET_CONFIG_RTOS_EN = 0，表明用户将SDK移植到裸机平台，此时需实现
 * @note 本文件中将RTOS创建的一个独立线程描述为task，等同于某些系统中的thread
 * @date 3/10/2016
 * @author wangjia
 *
 */
#ifndef ET_RTOS_H
#define ET_RTOS_H

#include "et_config.h"
#include "et_types.h"

#if ET_CONFIG_RTOS_EN

/*****************************************************mutex接口*************************************************/
typedef void * et_mutex_handle_t;

/**
 * 创建互斥锁
 *
 * @return mutex句柄
 * 		   NULL 表示失败
 */
 extern et_mutex_handle_t et_mutex_create(void);
 
 /**
 * 销毁互斥锁
 * @param mutex_handle 要销毁的mutex句柄，由et_mutex_create返回
 *
 * @return 0 表示成功
 * 		   -1 表示失败
 */
 extern et_int32 et_mutex_destroy(et_mutex_handle_t mutex_handle);
 
 /**
 * 互斥锁加锁
 * @param mutex_handle 要销毁的mutex句柄，由et_mutex_create返回
 * @note 在某些系统中mutex接口具备超时参数，那么若产生了超时失败，SDK将认为当前操作不具备权限，\n
 *       默认情况下期望用户能够阻塞et_mutex_lock直至获取权限，用户可依据实际使用情况设置.
 *
 * @return 0 表示成功
 * 		   -1 表示失败
 */
 extern et_int32 et_mutex_lock(et_mutex_handle_t mutex_handle);
 
 /** 
  * 互斥锁解锁
  * @param mutex_handle 要销毁的mutex句柄，由et_mutex_create返回
 *
 * @return 0 表示成功
 * 		   -1 表示失败
 */
 extern et_int32 et_mutex_unlock(et_mutex_handle_t mutex_handle);
#endif

#endif
