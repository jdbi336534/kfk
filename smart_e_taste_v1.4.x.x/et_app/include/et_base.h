/**
 * Copyright (c) 2016 ET iLink.
 * All rights reserved.
 */
/**
 *
 * @file et_base.h 
 * @brief 本文件包含的接口为硬件相关接口，用户在移植时必须实现
 * @date 3/10/2016
 * @author wangjia
 *
 */
#ifndef ET_BASEAPI_H
#define ET_BASEAPI_H

#include "et_types.h"
#include "et_config.h"

/*******************************************************宏******************************************************/

#define ET_IPSTR_LEN_MAX    32          ///< IP地址字符串存储buffer大小

/**
 * @name SDK监听的端口
 * @note 用户修改无效
 */
///@{
#define ET_LISTEN_PORT_TCP        18883 ///< TCP服务端口
#define ET_LISTEN_PORT_UDP        18886 ///< UDP服务端口
#define ET_LISTEN_PORT_UDP_BROAD  2073  ///< UDP广播端口
///@}

#define ET_SOCKET_NULL            ((et_socket_t)-1)    ///< 无效的Socket，用于表示Socket创建失败返回
/*****************************************************time接口**************************************************/
/**
 * 获取系统时间，单位为ms
 * @note 该时间为系统运行到调用该接口的ms计数值，SDK需要判断超时等条件时将调用该接口
 *
 * @return 当前的系统时间数，单位为ms
 */
extern et_uint32 et_system_time_ms(void);

/**
 * 睡眠当前线程,单位为ms
 * @note 无论是否有RTOS，SDK都将调用该函数睡眠当前task
 * @param milliseconds 需要睡眠的时间
 */
extern void et_sleep_ms(et_int32 milliseconds);


/****************************************************socket接口*************************************************/
/**
 * socket类型
 * @note 用户可依据TCp/IP协议栈socket实现情况修改
 */
typedef et_int32 et_socket_t;

/**
 * TCP/IP地址信息
 */
typedef struct{
    et_int8 ip_str[ET_IPSTR_LEN_MAX];  ///< IP,e.g:"192.168.10.1"
    et_uint16 port;                ///< 端口
} et_addr_info_t;

/**
 * socket 类型
 */
typedef enum{
    ET_SOCKET_UDP,		///< UDP socket
    ET_SOCKET_TCP		///< TCP socket
} et_socket_proto_t;

/**
 * 创建socket
 * @note 该接口应可以创建UDP及TCP socket，SDK内部将调用该接口同时创建多个UDP及TCP socket\n
 *		 其中UDP socket应能接收广播包，TCP socket为长连接.
 * @param type socket类型
 *
 * @return socket值
 *         @see ET_SOCKET_NULL 表示创建失败
 */
extern et_socket_t et_socket_create(et_socket_proto_t type);

/**
 * 关闭socket
 * @note 
 * @param socket 由et_socket_create返回
 *
 * @return 0 表示成功
 * 		   -1 表示失败
 */
extern et_int32 et_socket_close(et_socket_t socket);

/**
 * 连接服务器
 * @param socket 由et_socket_create返回
 * @param ip 服务器IP地址，格式为以结束符'\0'结尾的字符串，如“192.168.1.10”
 * @param port 服务器断开号
 * @param time_out_ms 超时时间，单位为ms;希望使用默认值时传0；
 *
 * @return 0 表示成功
 * 		   -1 表示失败
 */
extern et_int32 et_socket_connect(et_socket_t socket, et_int8 *ip, et_uint16 port, et_uint32 time_out_ms);

/**
 * TCP发送数据
 * @note 注意区分发送字节数为0和出错失败
 * @param socket 由et_socket_create返回
 * @param send_buf 发送数据缓冲
 * @param buf_len 待发送数据的长度，单位为Byte
 * @param time_out_ms 超时时间，单位为ms
 *
 * @return 成功的返回已发送的数据长度，不包含包头
 * 		0 表示无数据发送\n
 * 		-1 表示发送出错
 *
 * @code
 * //数据发送示例
 * et_int32 et_socket_send(et_socket_t socket, et_uint8 *send_buf, et_uint32 buf_len, et_uint32 time_out_ms)
 * {
 *  	struct timeval timer = {time_out_ms/1000, time_out_ms%1000*1000};
 *  	fd_set write_set;
 *  	et_int32 rc = 0;
 *
 *  	FD_ZERO(&write_set);
 *  	FD_SET(socket, &write_set);
 *
 *  	if((rc = select(socket+1, NULL, &write_set, NULL, &timer)) > 0)
 *  		rc = send(socket, send_buf, buf_len, 0);
 *  	if(rc < 0 && !(errno == 0 || errno == EINPROGRESS || errno == EINTR || errno == EAGAIN || errno == EWOULDBLOCK))
 *  	{
 *  		rc = ET_FAILURE;
 *  	}
 *  	return rc;
 *  }
 * @endcode
 */
extern et_int32 et_socket_send(et_socket_t socket, et_uint8 *send_buf, et_uint32 buf_len, et_uint32 time_out_ms);

/**
 * TCP接收数据
 * @note 1、注意区分接收为0和出错失败，返回出错时SDK将关闭socket;\n
 *       2、当time_out_ms传入 0 时,当前无数据接收则立即返回;\n
 *       3、SDK不关注当前存在多少数据，当SDK需要一定长度数据时调用该接口获取,\n
 *       有可能只获取当前存在数据的一部分，剩余数据下一步再次读取；
 *       4、若TCP连接已断开，该接口应返回-1
 * @param socket 由et_socket_create返回
 * @param recv_buf 接收数据缓冲
 * @param buf_len 数据存储缓冲长度，单位为Byte
 * @param time_out_ms 超时时间，单位为ms
 *
 * @return 成功的返回已发送的数据长度，不包含包头
 * 		0 表示无数据接收\n
 * 		-1 表示出错或连接已断开
 * @code
 * //数据接收示例
 * et_int32 et_socket_recv(et_socket_t socket, et_uint8 *recv_buf, et_uint32 buf_len, et_uint32 time_out_ms)
 * {
 *  	struct timeval timer = {0, 0};
 *  	fd_set read_set;
 *  	et_int32 rc =0;
 *      timer.tv_sec = time_out_ms/1000;
 *  	timer.tv_usec = time_out_ms%1000*1000;
 *  	FD_ZERO(&read_set);
 *  	FD_SET(socket, &read_set);
 *
 *  	if((rc = select(socket+1, &read_set, NULL, NULL, &timer)) > 0)
 *  		rc = recv(socket, recv_buf, buf_len, 0);
 *  	if(rc < 0 && !(errno == 0 || errno == EINTR || errno == EAGAIN || errno == EINPROGRESS || errno == EWOULDBLOCK))	///< Socket error
 *  	{
 *  		ET_LOG_USER("recv failed %d\n", errno);
 *  		rc = ET_FAILURE;
 *  	}
 *  	return rc;
 *  }
 * @endcode
 */
extern et_int32 et_socket_recv(et_socket_t socket, et_uint8 *recv_buf, et_uint32 buf_len, et_uint32 time_out_ms);

#if ET_CONFIG_SERVER_EN
/**
 * 绑定socket
 * @note 在SDK需要监听时，调用该接口为Socket绑定端口，SDK不关心绑定的IP
 * @param socket 由et_socket_create返回
 * @param port 分配给Socket的端口
 *
 * @return 0 表示成功
 * 		   -1 表示失败
 */
extern et_int32 et_socket_bind(et_socket_t socket, et_uint16 port);

/**
 * 监听端口
 * @note SDK内部在需要监听端口时调用一次\n
 *       在SDK运行过程中listen一个端口后不会再取消listen
 * @param socket 由et_socket_create返回
 *
 * @return 0 表示成功
 *         -1 表示失败
 */
extern et_int32 et_socket_listen(et_socket_t socket);

/**
 * 检查监听端口连接
 * @note SDK监听端口后，将调用该接口检查连接到该端口的连接
 * @param socket 由et_socket_create返回
 * @param remote_info 出参，远端主机信息存储指针
 *
 * @return 建立连接的本地socket
 *         @see ET_SOCKET_NULL表示无连接
 */
extern et_socket_t et_socket_accept(et_socket_t socket, et_addr_info_t *remote_info);

/**
 * UDP发送数据
 * @note 当启用内网服务时需实现该接口
 * @param socket 由et_socket_create返回
 * @param send_buf 指向待发送的数据buffer
 * @param buf_len 待发送的数据长度，单位为Byte
 * @param remote_info 远端主机信息
 * @param time_out_ms 超时时间，单位为ms
 *
 * @return 发送成功的数据长度\n
 *         -1 表示失败
 */
extern et_int32 et_socket_send_to(et_socket_t socket, et_uint8 *send_buf, et_uint32 buf_len, et_addr_info_t *remote_info,
                                  et_uint32 time_out_ms);

/**
 * UDP接收数据
 * @note 当启用内网服务时需要实现该接口
 * @param socket 由et_socket_create返回
 * @param recv_buf 指向接收buffer
 * @param buf_len 接收buffer大小，单位为Byte
 * @param remote_info 出参，远端主机信息
 * @param time_out_ms 超时时间，单位为ms
 *
 * @return 接收数据长度
 *         -1 表示失败
*/
extern et_int32 et_socket_recv_from(et_socket_t socket, et_uint8 *recv_buf, et_uint32 buf_len, et_addr_info_t *remote_info,
                                    et_uint32 time_out_ms);

/**
 * 获取本机IP
 * @param local_ip 本机的IP存储Buffer，格式为字符串，例如"192.168.1.10"
 * @param size local_ip大小，单位byte
 *
 * @return 0 获取成功
 *         -1 获取失败
 */
extern et_int32 et_get_localip(et_int8 *local_ip, et_uint32 size);
#endif

/**
 * 通过域名解析获得地址信息
 * @note 解析返回的IP地址信息必须为以'\0'结尾的字符串\n
         该接口应为同步接口，SDK调用该接口时阻塞直至返回成功或失败
 * @param name 域名或IP端口信息，格式为以结束符'\0'结尾的字符串，如"www.kaifakuai.com"或“192.168.1.10”
 * @param addr 出参，存储获取到的IP地址,@see et_addr_info_t中的端口信息可不传
 * @param time_out_ms 超时时间，单位为ms
 *
 * @return 0 表示成功\n
 *         -1 表示失败
 *
 */
extern et_int32 et_gethostbyname(et_int8 *name, et_addr_info_t *addr, et_uint32 time_out_ms);


#endif
