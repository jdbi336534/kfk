/**
 * Copyright (c) 2016 ET iLink.
 * All rights reserved.
**/
/**
 *
 * @file et_std.h
 * @brief 本文件中定义了SDK运行所需的ANSI C99函数，用户需按照ANSI C99标准实现函数功能。
 * @date 3/10/2016
 * @author wangjia
 *
 */
#ifndef ET_STD_H
#define ET_STD_H

#include "et_types.h"
#include <stdio.h>
#include "esp_common.h"

/**
 * SDK将调用该接口打印日志
 * @note 该接口为变参，实现可参考示例代码
 * @code
 * //使用printf进行打印
 * et_int32 et_printf(const et_int8 *format, ...)
 * {
 *      int done;
 *      va_list args;
 *      va_start(args, format);
 *      done = vprintf(format, args);
 *      va_end(args);
 *      return done;
 * }
 * @endcode
 */
//extern et_int32 et_printf(const et_int8 *format, ...);
#define et_printf(format, ...)	os_printf(format, ##__VA_ARGS__)
/**
 * SDK将调用该函数进行格式化
 * @code
 * et_int32 et_snprintf(et_int8 *dest, et_uint32 size, const et_int8 *format, ...)
 * {
 *      int done;
 *      va_list args;
 *      va_start(args, format);
 *      done = vsnprintf (dest, size, format, args);
 *      va_end(args);
 *      return done;
 * }
 * @endcode
 */
extern et_int32 et_snprintf(et_int8 *dest, et_uint32 size, const et_int8 *format, ...);

extern void *et_malloc(et_uint32 size);
extern void *et_realloc(void *mem, et_uint32 size);
extern void et_free(void *mem);

extern void *et_memcpy(void *dest, const void *src, et_uint32 size);
extern void *et_memset(void *dest, et_int32 val, et_uint32 size);
extern et_int32 et_memcmp(const void *buf1, const void *buf2, et_uint32 size);

extern void *et_strncpy(et_int8 *str1, const et_int8* str2, et_uint32 size);
extern et_uint32 et_strlen(const et_int8 *str);
extern et_int32 et_strcmp(const et_int8 *str1, const et_int8 *str2);
extern et_int32 et_strncmp(const et_int8 *str1, const et_int8 *str2, et_uint32 size);

extern et_double et_floor(et_double x);
extern et_double et_pow(et_double x, et_double y);
extern et_double et_fabs(et_double x);
#endif
