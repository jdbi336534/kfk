/**
 * Copyright (c) 2016 ET iLink.
 * All rights reserved.
 */
/**
 * 
 * @file et_types.h
 * @brief 本文件包含iLink U-SDK所使用的数据类型及数据结构
 * @date 3/10/2016
 * @author wangjia
 * 
 */
#ifndef ET_TYPE_H
#define ET_TYPE_H

#include "et_config.h"

#define ET_NET_IP_LEN_MAX			32
#define ET_NET_PORT_LEN_MAX			8
#define ET_FILE_NAME_LEN_MAX        128
#define ET_FILE_ID_LEN_MAX			128
#define ET_FILE_DESCN_LEN_MAX       32

/**
 * @name 重定义的标准数据类型
 * 用户需根据不同的平台进行适当修改,该部分内容的变化将影响整个SDK
 */
///@{
typedef unsigned char       et_uchar;
typedef unsigned char		et_uint8;
typedef char                et_char;
typedef char                et_int8;

typedef unsigned short		et_ushort;
typedef unsigned short		et_uint16;
typedef short               et_short;
typedef short               et_int16;

typedef unsigned int		et_uint;
typedef unsigned int		et_uint32;
typedef int                 et_int;
typedef int                 et_int32;

typedef unsigned long		et_ulong32;
typedef long                et_long32;

typedef unsigned long long	et_uint64;
typedef long long           et_int64;

typedef float               et_float;
typedef double              et_double;

typedef et_uint32           et_size_t;
///@}

/**
 * 定义bool型数据
 */
typedef enum{
	ET_FALSE	= 0, 
	ET_TRUE		= 1
}et_bool;

/**
 * SDK错误代码
 */
typedef enum{
    ET_SUCCESS                      = 0,		///< 成功
    ET_FAILURE                      = -1,		///< 执行失败
    ET_ERR_BUFFER_OVERFLOW          = -2,		///< 数据过长，内存溢出
    ET_ERR_PARAM_INVALID            = -3,       ///< 参数错误
    ET_ERR_ALLOC_FAILED             = -4,       ///< 申请内存失败
    ET_ERR_CONFIG_NONSUPPORT        = -5,       ///< 不支持的设置
    ET_ERR_MUTEX_FAILED             = -6,       ///< 创建mutex失败
    ET_ERR_ACCOUNT_INVALID          = -7,       ///< 账号信息无效
    ET_ERR_LB_SOCKET_FAILED         = -8,       ///< 创建LB Socket失败
    ET_ERR_LB_DNS_FAILED            = -9,       ///< LB DNS解析失败
    ET_ERR_LB_CONN_FAILED           = -10,      ///< LB 连接失败
    ET_ERR_LB_GET_SERVER_FAILED     = -11,      ///< 从LB获取服务器地址失败
    ET_ERR_SERVER_DNS_FAILED        = -12,      ///< 解析各功能服务器地址失败

    //
    ET_ERR_IM_UNKNOWN               = -0x1000,      ///< 未知的IM错误
    ET_ERR_IM_ILINK_REPEAT          = -0x1001,      ///< 重复登录iLink
    ET_ERR_IM_ILINK_SOCKET_FAILED   = -0x1002,      ///< 创建iLink socket失败
    ET_ERR_IM_ILINK_CONN_FAILED     = -0x1003,      ///< 连接iLink服务器失败
    ET_ERR_IM_ILINK_PROTOCOL_VER    = -0x1010,      ///< 连接已拒绝，不支持的协议版本
    ET_ERR_IM_ILINK_CLIENT_ID       = -0x1011,      ///< 连接已拒绝，非法的客户端标识符
    ET_ERR_IM_ILINK_UNAVAILABLE     = -0x1012,      ///< 连接已拒绝，服务器端不可用
    ET_ERR_IM_ILINK_UID_APPKEY      = -0x1013,      ///< 连接已拒绝，无效的UID或AppKey
    ET_ERR_IM_ILINK_UNAUTHORIZED    = -0x1014,      ///< 连接已拒绝，未授权
    ET_ERR_IM_ILINK_TASK_CREATE     = -0x1110,      ///< 创建iLink任务失败
    ET_ERR_IM_ILINK_UNCONNECT       = -0x1111,      ///< iLink连接已断开，API执行失败
    ET_ERR_IM_ILINK_RECV_TIMEOUT    = -0x1305,      ///< 消息接收超時
    ET_ERR_IM_LOCAL_REPEAT          = -0x1200,      ///< 重复启动内网服务
    ET_ERR_IM_LOCAL_SOCKET_FAILED   = -0x1201,      ///< 初始化内网服务Socket失败
    ET_ERR_IM_LOCAL_GET_LOCALIP     = -0x1202,      ///< 获取本机IP失败
    ET_ERR_IM_LOCAL_DEV_NOTIFY      = -0x1203,      ///< 内网设备启动通知失败
    ET_ERR_IM_LOCAL_TASK_CREATE     = -0x1204,      ///< 创建内网任务失败
    ET_ERR_IM_LOCAL_USER_LIMIT      = -0x1205,      ///< 内网连接已达到上限
    ET_ERR_IM_QUEUE_FULL            = -0x1300,      ///< 消息队列已满
    ET_ERR_IM_DATA_PARSE            = -0x1301,      ///< 数据解析失败
    ET_ERR_IM_WAITACK_TIMEOUT       = -0x1302,      ///< 等待ACK超时
    ET_ERR_IM_SEND_FAILED           = -0x1303,      ///< 消息发送失败
    ET_ERR_IM_SOCKET_FAILURE        = -0x1304,      ///< IM socket接口出错
    ET_ERR_IM_MSG_TOOLONG           = -0x1305,      ///< IM消息过长
    //
    ET_ERR_FILE_UNKNOWN             = -0x2000,      ///< 未知的文件错误
    ET_ERR_FILE_INFO_PARSE          = -0x2001,      ///< 文件信息解析失败
    ET_ERR_FILE_INFO_TYPE           = -0x2002,      ///< 文件消息缺少type信息 或 type信息不合法
    ET_ERR_FILE_INFO_ID             = -0x2003,      ///< 文件消息缺少id信息 或 id信息不合法
    ET_ERR_FILE_INFO_NAME           = -0x2004,      ///< 文件消息缺少name信息 或 name信息不合法
    ET_ERR_FILE_INFO_SIZE           = -0x2005,      ///< 文件消息缺少size信息 或 size信息不合法
    ET_ERR_FILE_INFO_IP             = -0x2006,      ///< 文件消息缺少ip信息 或 ip信息不合法
    ET_ERR_FILE_INFO_PORT           = -0x2007,      ///< 文件消息缺少port信息 或 port信息不合法
    ET_ERR_FILE_INFO_UNACCEPT       = -0x2100,      ///< 不合法的文件消息类型
    ET_ERR_FILE_CLIENT_INIT         = -0x2200,      ///< 初始化文件客户端失败
    ET_ERR_FILE_CONN_FAILED         = -0x2201,      ///< 连接文件服务器失败
    ET_ERR_FILE_FILEID_INVALID      = -0x2202,      ///< 文件ID无效
    ET_ERR_FILE_GET_HEADER          = -0x2203,      ///< 获取文件头信息失败
    ET_ERR_FILE_STATE_EXCEPTION     = -0x2204,      ///< 文件状态异常
    ET_ERR_FILE_RECV_FAILED         = -0x2205,      ///< 接收文件内容失败
    ET_ERR_FILE_DOWN_CB_RETURN      = -0x2206,      ///< 文件下载回调执行返回失败
    ET_ERR_FILE_GET_UPLOAD_PATH     = -0x2207,      ///< 获取文件上传路径失败
    ET_ERR_FILE_UPLOAD_FAILED       = -0x2208,      ///< 文件上传失败
    ET_ERR_FILE_MESSAGE_PACKAG      = -0x2209,      ///< 封装文件消息失败
    ET_ERR_FILE_MESSAGE_SEND        = -0x220A,      ///< 发送文件消息失败
    //
    ET_ERR_HTTP_UNKNOWN             = -0x3000,      ///< 未知的HTTP错误
    ET_ERR_HTTP_DATA_PACKAG         = -0x3001,      ///< 封装数据失败
    ET_ERR_HTTP_DATA_TOOLONG        = -0x3002,      ///< 数据长度超限
    ET_ERR_HTTP_REQUEST_SEND        = -0x3003,      ///< 数据发送失败
    ET_ERR_HTTP_RESPONSE_RECV       = -0x3004,      ///< 接收应答数据失败
    ET_ERR_HTTP_RESPONSE_APRSE      = -0x3005,      ///< 解析应答数据失败
    ET_ERR_HTTP_APPTOKEN_INVALID	= -0x3100,		///< 无效的apptoken
    ET_ERR_HTTP_KEY_INVALID         = -0x3101,		///< 无效的appkey或secretkey
    ET_ERR_HTTP_BASE64_INVALID		= -0x3102,		///< 无效的base64编码失败
    ET_ERR_HTTP_PARA_INVALID		= -0x3103,		///< 无效的参数
    ET_ERR_HTTP_API_NONE			= -0x3104,		///< 无效的API
    ET_ERR_HTTP_EXECUTE_FAILED		= -0x3105,		///< 执行出错
    ET_ERR_HTTP_REDO                = -0x3106,		///< 重复操作无效
    ET_ERR_HTTP_PERMISSION_DENIED	= -0x3107,		///< 权限不足
    ET_ERR_HTTP_SEVER_FAILED		= -0x3108,		///< HTTP通知IM失败，操作无效
    ET_ERR_HTTP_DATA_ABSENT         = -0x3109		///< 数据不存在
}et_code;

/**
 * 依据情况选择定义NULL
 */
#ifndef NULL
    #define NULL (void *)0
#endif

/**
 * 消息发送方式
 */
typedef enum{
    SEND_TO_LOCAL = 1,      ///< 将消息发送给本地连接用户
    SEND_TO_CLOUD,          ///< 将消息发送给iLink用户
    SEND_TO_ALL,            ///< 将消息同时发送给iLink及本地用户
#if ET_CONFIG_CLOUD_EN && ET_CONFIG_SERVER_EN
    SEND_TO_CLOUD_FIRST,    ///< 优先将消息发送给iLink用户，若iLink未连接则发送给本地用户
    SEND_TO_LOCAL_FIRST     ///< 优先将消息发送给本地用户，若本地无用户连接则发送给iLink用户
#endif
}et_send_method_type;

/**
 * 消息服务级别
 */
typedef enum{
    ET_QOS0 = 0,    ///< 最多一次接收
    ET_QOS1,        ///< 最少一次接收
    ET_QOS2         ///< 只有一次接收
}et_mes_qos_type;

/**
 * 消息类型
 */
typedef enum{
    MES_CHAT_TO_CHAT = 1,       ///< 点对点消息
    MES_FROM_GROUP_SUBTOPIC,    ///< 群消息或主题消息
    MES_FROM_LOCAL,             ///< 本地消息
    MES_FILE_TRANSFERS,         ///< 文件消息
    MES_USER_ONLINE,            ///< 用户上线通知消息
    MES_USER_OFFLINE,           ///< 用户下线通知消息
    MES_USER_STATUS,            ///< 用户状态消息
    MES_NOTICE_ADD_BUDDY,       ///< 添加用户通知消息
    MES_NOTICE_REMOVE_BUDDY,    ///< 删除用户通知消息
    MES_ONLINE_BUDDIES_LIST,    ///< 在线好友列表消息
    MES_CHAT_TO_CHAT_EX         ///< 点对点扩展消息
}et_mes_type;

/**
 * 事件类型
 */
typedef enum{
    EVENT_CLOUD_CONNECT = 1,    ///< iLink连接事件
    EVENT_CLOUD_DISCONNECT,     ///< iLink断连时间
    EVENT_LOGIN_KICK,           ///< 被踢下线事件
    EVENT_CLOUD_SEND_FAILED,    ///< iLink消息发送失败事件
    EVENT_CLOUD_SEND_SUCCESS,   ///< iLink消息发送成功事件
    EVENT_LOCAL_SEND_FAILED,    ///< 本地消息发送失败事件
    EVENT_LOCAL_SEND_SUCCESS,   ///< 本地消息发送成功事件
    EVENT_LOCAL_CONNECT,        ///< 本地连接事件
    EVENT_LOCAL_DISCONNECT,     ///< 本地断连事件
    EVENT_HTTP_INIT_SUCCESS,    ///< HTTP初始化成功事件
    EVENT_HTTP_INIT_FAILED      ///< HTTP初始化失败事件
}et_event_no_type;

/**
 * 文件下载方式
 */
typedef enum{
    ET_FILE_DOWN_TO_BUFF = 1,
    ET_FILE_DOWN_TO_FILE,
    ET_FILE_DOWN_TO_CALLBACK
}et_file_down_type;

/**
 * 消息结构\n
 */
typedef struct{
    et_uint16 mes_id;
    et_mes_qos_type mes_qos;
    et_uchar *payload;
    et_uint32 payload_len;
}et_context_mes_type;

/**
 * 事件结构
 */
typedef struct{
    et_event_no_type event_no;
    et_int32 data;
}et_event_type;

/**
 * 文件消息类型
 */
typedef enum{
    ET_FILE_MSG_PUSH = 0,   ///< 消息发送方主动发送文件
    ET_FILE_MSG_PULL,       ///< 消息发送方索取文件
    ET_FILE_MSG_ERROR       ///< 发生错误,发送错误时,错误信息存放在descn中
}et_file_msg_type;
/**
 * 文件信息结构
 */
typedef struct {
    et_file_msg_type type;                      ///< 文件消息类型,5 bytes
    et_uint32 crc32;                            ///< CRC32校验码,10 bytes
    et_long32 file_size;                        ///< 文件大小,20 bytes
    et_char source_ip_addr[ET_NET_IP_LEN_MAX];  ///< 文件存储IP
    et_char port[ET_NET_PORT_LEN_MAX];          ///< 端口
    et_char file_id[ET_FILE_ID_LEN_MAX];        ///< 文件ID
    et_char file_name[ET_FILE_NAME_LEN_MAX];    ///< 文件名
    et_char descn[ET_FILE_DESCN_LEN_MAX];       ///< 异常信息
} et_dfs_file_info_type;

/**
 * 网络地址信息
 */
typedef struct{
    et_char *name_ip;   ///< 域名或地址
    et_uint16 port;     ///< 端口
}et_net_addr_type;

/**
 * iLink句柄
 */
typedef void * et_cloud_handle;

/**
 * 各类回调函数
 */
///@{
/**
 * 事件处理回调类型
 */
typedef void (*et_event_process_type)(et_event_type event);
/**
 * 消息处理回调类型
 */
typedef void (* et_msg_process_Type)(et_int32 type, et_char *send_userid, et_char *topic_name,
                                     et_int32 topic_len, et_context_mes_type *message);

/**
 * 文件下载回调类型
 * @param arg 文件名
 * @param file_size 文件大小
 * @param data 指向已接收到的数据
 * @param current_size 本次接收到的大小
 *
 * @return 接收处理成功返回0，-1表示失败
 */
typedef et_int32 (*et_file_down_process_type) (void *arg, const et_int64 file_size, const et_char *data,
                                               const et_int32 current_size);
/**
 * 文件上传回调类型
 * @param arg 文件名
 * @param file_size 需发送的文件大小
 * @param sock 发送socket,用户直接使用该socket进行TCP发送
 *
 * @return 发送处理成功则返回0，-1表示失败
 */
typedef et_int32 (*et_file_uplaod_process_type) (void *arg, const et_int64 file_size, et_int32 sock);
///@}

#endif
