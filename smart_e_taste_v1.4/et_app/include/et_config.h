/**
 * Copyright (c) 2016 ET iLink.
 * All rights reserved.
 */
/**
 *
 * @file et_config.h
 * @brief 本文件提供SDK配置、裁剪接口
 * @date 3/10/2016
 * @author wangjia
 *
 */

#ifndef ET_CONFIG_H
#define ET_CONFIG_H
/**
 * @name 功能裁剪
 * 1 表示启用该功能，0 表示禁用该功能
 */
/**@{*/
#define ET_CONFIG_RTOS_EN	1	/**< RTOS接口，用户在RTOS下使用SDK应启用，否则禁用 */
#define ET_CONFIG_HTTP_EN	0	/**< HTTP功能 */
#define ET_CONFIG_FILE_EN	0	/**< 文件功能 */
#define ET_CONFIG_SERVER_EN	1	/**< 内网通信功能 */
#define ET_CONFIG_CLOUD_EN	1	/**< 外网通信功能 */
/**@}*/

/**
 * @name 日志打印配置
 * 1 表示启用该功能的日志打印，0 表示停用该功能的日志打印
 */
/**@{*/
#define ET_LOG_IM_EN		0	///< IM消息日志
#define ET_LOG_HTTP_EN		0	///< HTTP功能日志
#define ET_LOG_FILE_EN		0	///< 文件功能日志
#define ET_LOG_SOCKET_EN	0	///< Socket日志
#define ET_LOG_GEN_EN		0	///< 其他通用日志打印
#define ET_LOG_USER_EN      1   ///< 用户日志打印
/**@}*/

#define ET_USER_ID_LEN_MAX			36      ///< UID缓冲大小
#define ET_LOCAL_CONN_NUM_MAX       5       ///< 本地通信允许的最大连接数
#define ET_MSG_LEN_MAX				1024
#define ET_TOPIC_LEN_MAX			32

/**
 * @name 网络超时设定
 * @note Socket接收超时视使用情况定，取值范围0~ET_ILINK_DEF_TIMEOUT_MS之间
 */
///@{
#define ET_ILINK_DEF_TIMEOUT_MS     3000    ///< iLink 默认的超时时间,单位ms
#define ET_ILINK_SEND_TIMEOUT_MS    1000    ///< iLink 消息超时,包含等待ack,单位ms
#define ET_ILINK_RECV_TIMEOUT_MS    5000    ///< iLink 消息接收超时时间,单位ms
#define ET_ILINK_ACK_TIMEOUT_MS     3000    ///< iLink 消息等待ACK超时时间,单位ms,仅对ET_QOS1、ET_QOS2有效
#define ET_SOCKET_CONN_TIMEOUT_MS   3000    ///< Socket 连接超时时间,单位ms
#define ET_SOCKET_SEND_TIMEOUT_MS   500     ///< Socekt 发送超时时间,单位ms
#define ET_GET_HOST_TIMEOUT_MS      500     ///< 域名解析超时时间,单位ms
///@}

/**
 * @name 模式设定
 */
///@{
#define ET_MODE_RECV_UNBLOCKING     0   ///< 1-接收非阻塞 0-接收阻塞
#if !ET_CONFIG_RTOS_EN
#define ET_MODE_RECV_UNBLOCKING     1   ///< 无RTOS情况下采用非阻塞
#endif
///@}
#endif
