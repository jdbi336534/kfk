#include "et_types.h"
#include "et_std.h"
#include "stdarg.h"
#include "stdio.h"
#include <string.h>
#include <stdlib.h>
#include <tgmath.h>
#include "esp_common.h"


/*et_int32 et_printf(const et_int8 *format, ...)	///< SDK将调用该接口打印日志 
{
    int done;
    va_list args;
    va_start(args, format);
    // done = vprintf(format, args);
    // done = vfprintf(stdout, format, args);
    va_end(args);
    return done;
}*/
et_int32 et_snprintf(et_int8 *dest, et_uint32 size, const et_int8 *format, ...)
{
    int done;
    va_list args;
    va_start(args, format);
    done = vsnprintf (dest, size, format, args);   
    va_end(args);
    return done;
}

void *et_malloc(et_uint32 size)
{
    return malloc(size);
}

void *et_realloc(void *mem, et_uint32 size)
{
	return realloc(mem, size);
}

void et_free(void *mem)
{
    return free(mem);
}

void *et_memcpy(void *dest, const void *src, et_uint32 size)
{
    return memcpy(dest, src, size);
}

void *et_memset(void *dest, et_int32 val, et_uint32 size)
{
    return memset(dest, val, size);
}

et_int32 et_memcmp(const void *buf1, const void *buf2, et_uint32 size)
{
    return memcmp(buf1, buf2, size);
}

void *et_strncpy(et_int8 *str1, const et_int8* str2, et_uint32 size)
{
    return strncpy(str1, str2, size);
}

et_uint32 et_strlen(const et_int8 *str)
{
    return strlen(str);
}

et_int32 et_strncmp(const et_int8 *str1, const et_int8 *str2, et_uint32 size)
{
    return strncmp(str1, str2, size);
}

et_int32 et_strcmp(const et_int8 *str1, const et_int8 *str2)
{
    return strcmp(str1, str2);
}

et_double et_floor(et_double x)
{
    return x;
}
et_double et_pow(et_double x, et_double y)
{
    return 1;
}
et_double et_fabs(et_double x)
{
    return fabs(x);
}
