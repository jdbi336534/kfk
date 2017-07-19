/**@mainpage
 * Copyright (c) 2016 ET iLink.
 * All rights reserved.
 *
 * iLink U-SDK将帮助用户的设备联网，并与Android、IOS、Web、Windows、Linux端
 * 进行IM通信、文件传输。
 *
 * \b 使用流程如下：\n
 *
 * 	1、创建Context：et_create_context()
 * 	2、连接：et_connect()
 * 	3、发送消息：et_chat_to()
 *
 * \b 重连流程：\n
 *
 *  1、自动重连：在et_connect中传入连接参数et_cloud_connect_type设置auto_relogin为ET_TRUE，\n
 * 则SDK在首次连接成功之后，若出现断连则自动重连，用户可再在EVENT_CLOUD_DISCONNECT事件中无需再调用重连函数et_relogin_cloud();
 *  2、手动重连：用户将auto_relogin设置为ET_FALSE时，需要处理EVENT_CLOUD_DISCONNECT事件，\n
 * 在该事件中调用et_reconnect()则可进行自动重连，重连失败将再次发生该事件;
 *  3、被踢下线：用户被踢下线，SDK将不会自动重连，而是产生EVENT_LOGIN_KICK事件;
 *
 */

/**
 * @file et_client.h
 * @brief 本文件提供了SDK的所有API接口
 * @date 3/11/2016
 * @author wangjia
 */
#ifndef __ET_CLIENT_H__
#define __ET_CLIENT_H__

#include "et_types.h"
#include "et_config.h"
#include "et_std.h"
#include "et_base.h"
#if ET_CONFIG_RTOS_EN
#include "et_rtos.h"
#endif

/**
 * 用户日志打印
 * @see ET_LOG_USER_EN
 */
#if ET_LOG_USER_EN
#define ET_LOG_USER(format, ...)    {et_printf("User log: "format, ##__VA_ARGS__);}
#else
#define ET_LOG_GEN(format, ...)     {}
#endif

typedef struct{
	et_bool clr_offline_mes;
	et_bool auto_relogin;
	et_uint16 alive_sec;
}et_cloud_connect_type;

typedef enum{
	LIST_ONLINE = 1
}et_buddies_list_type;

/**
 * 获取SDK版本信息
 * @return SDK版本字符串
 */
et_char *et_get_sdk_version(void);

/**
 * 创建上下文
 * @note 该接口不能在回调函数中使用
 * @brief 与et_destroy_context配对使用
 * @param userid 指向UID buffer
 * @param app_key 指向AppKey buffer
 * @param secret_key 指向SecretKey buffer
 * @param lb_info 负载均衡地址信息
 *
 * @return 上下文句柄\n
 *          0 表示失败
 */
et_cloud_handle et_create_context(const et_char *userid, const et_char *app_key,    \
                                  const et_char *secret_key, et_net_addr_type lb_info);

/**
 * 销毁上下文
 * @note 该接口不能在回调函数中使用
 * @brief 与et_create_context配套使用
 * @param cloud_handle 由et_create_context返回
 *
 * @return 0 表示成功\n
 *         <0 表示失败，详见@see et_code
 */
et_int32 et_destroy_context(et_cloud_handle *cloud_handle);

/**
  * 设置回调
  * @note 在非RTOS系统中，回到将占用SDK处理事件；
  *       在RTOS系统中，回调将消耗任务堆栈，请勿在回调中进行过于复杂的操作
  * @param cloud_handle 由et_create_context返回
  * @param mes_call 消息回调,详见@see et_msg_process_Type
  * @param event_call 事件回调,详见@see et_event_process_type
  *
  * @return 0 表示成功\n
  *         <0 表示失败，详见@see et_code
  */
et_int32 et_set_callback(et_cloud_handle cloud_handle, et_msg_process_Type mes_call, et_event_process_type event_call);

#if ET_CONFIG_SERVER_EN
/**
 * 内网轮询
 * @note 用户需要在用户程序中循环调用该接口
 * @param cloud_handle 由et_create_context返回
 *
 * @return 0 表示成功\n
 *         <0 表示失败，详见@see et_code
 */
et_int32 et_server_loop(et_cloud_handle cloud_handle);

/**
 * 启动内网服务
 * @note 请勿在回调函数中调用该接口
 * @param cloud_handle 由et_create_context返回
 *
 * @return 0 表示成功\n
 *         <0 表示失败，详见@see et_code
 */
et_int32 et_start_server(et_cloud_handle cloud_handle);
/**
 * 停止内网服务
 * @note 请勿在回调函数中调用该接口
 * @param cloud_handle 由et_create_context返回
 *
 * @return 0 表示成功\n
 *         <0 表示失败，详见@see et_code
 */
et_int32 et_stop_server(et_cloud_handle cloud_handle);
/**
 * 获取内网连接用户
 * @param cloud_handle 由et_create_context返回
 * @param local_users_buf 出参，已连接内网用户的UID存储buffer
 * @param users_num local_users_buf允许存储的UID数量，即该二维数组的第一维大小
 *
 * @return 获取到的UID数\n
 *         <0 表示失败，详见@see et_code
 */
et_int32 et_get_local_users(et_cloud_handle cloud_handle, et_char local_users_buf[][ET_USER_ID_LEN_MAX], et_uint32 users_num);
#endif

#if ET_CONFIG_CLOUD_EN
/**
 * 外网轮询
 * @note 用户需要在用户程序中循环调用该接口
 * @param cloud_handle 由et_create_context返回
 *
 * @return 0 表示成功\n
 *         <0 表示失败，详见@see et_code
 */
et_int32  et_ilink_loop(et_cloud_handle cloud_handle);

/**
 * iLink连接
 * @note 与et_disconnect配套使用，若断连重登请调用 @see et_reconnect;\n
 * 请勿在回调函数中调用该接口
 * @param cloud_handle 由et_create_context返回
 * @param connect_para 连接参数，详见@see et_cloud_connect_type
 *
 * @return 0 表示成功\n
 *         <0 表示失败，详见@see et_code
 */
et_int32 et_connect(et_cloud_handle cloud_handle, et_cloud_connect_type connect_para);

/**
 * 重连iLink
 * @note 若服务器断连则调用该函数重登
 * @param cloud_handle 由et_create_context返回
 * @param connect_para 连接参数，详见@see et_cloud_connect_type
 *
 * @param 0 表示成功\n
 *        <0 表示失败，详见@see et_code
 */
et_int32 et_reconnect(et_cloud_handle cloud_handle, et_cloud_connect_type connect_para);

/**
 * 获取用户状态
 * @note 获取成功后，将在消息回调中收到MES_USER_STATUS类型消息
 * @param cloud_handle 由et_create_context返回
 * @param userid 期望获取的用户UID
 *
 * @return 0 表示成功\n
 *         <0 表示失败，详见@see et_code
 */
et_int32 et_get_user_state(et_cloud_handle cloud_handle, et_char *userid);

/**
 * 状态订阅
 * @note 订阅成功后，userid用户上下线将MES_USER_ONLINE、MES_USER_OFFLINE类型消息
 * @param cloud_handle 由et_create_context返回
 * @param userid 期望订阅的用户UID
 *
 * @return 0 表示成功\n
 *         <0 表示失败，详见@see et_code
 */
et_int32 et_sub_user_state(et_cloud_handle cloud_handle, et_char *userid);

/**
 * 取消状态订阅
 * @param cloud_handle 由et_create_context返回
 * @param userid 期望取消订阅的用户UID
 *
 * @return 0 表示成功\n
 *         <0 表示失败，详见@see et_code
 */
et_int32 et_unsub_user_state(et_cloud_handle cloud_handle, et_char *userid);

/**
 * 获取离线消息
 * @param cloud_handle 由et_create_context返回
 *
 * @return 0 表示成功\n
 *         <0 表示失败，详见@see et_code
 */
et_int32 et_request_offline_message(et_cloud_handle cloud_handle);

/**
 * 订阅主题
 * @param cloud_handle 由et_create_context返回
 * @param topic 消息主题
 * @param qos 消息级别，详见@see et_mes_qos_type
 *
 * @return 0 表示成功\n
 *         <0 表示失败，详见@see et_code
 */
et_int32 et_subscribe(et_cloud_handle cloud_handle, const et_char* topic, et_mes_qos_type qos);

/**
 * 取消订阅主题
 * @param cloud_handle 由et_create_context返回
 * @param topic 消息主题
 *
 * @return 0 表示成功\n
 *         <0 表示失败，详见@see et_code
 */
et_int32 et_unsubscribe(et_cloud_handle cloud_handle, const et_char* topic);
/**
 * 断开iLink连接
 * @note 请勿在回调函数中调用该接口
 * @param cloud_handle 由et_create_context返回
 *
 * @return 0 表示成功\n
 *         <0 表示失败，详见@see et_code
 */
et_int32 et_disconnect(et_cloud_handle cloud_handle);
#endif
#if ET_CONFIG_CLOUD_EN || ET_CONFIG_SERVER_EN
/**
 * 发送点对点消息
 * @note 此处返回成功只表示成功进入消息队列，当事件通知消息ID发送成功时表示socket发送成功
 * @param cloud_handle 由et_create_context返回
 * @param data 待发送的数据
 * @param len 待发送的数据长度，单位byte
 * @param userid 目标UID
 * @param send_method 发送方式，详见@see et_send_method_type
 *
 * @return 发送成功时返回消息ID\n
 *         <0 表示失败，详见@see et_code
 */
et_int32 et_chat_to(et_cloud_handle cloud_handle, et_uchar *data, et_uint32 len,     \
                    const et_char *userid, et_send_method_type send_method);

/**
 * 发布主题消息
 * @note 此处返回成功只表示成功进入消息队列，当事件通知消息ID发送成功时表示socket发送成功
 * @param cloud_handle 由et_create_context返回
 * @param data 待发送的数据
 * @param len 待发送的数据长度，单位byte
 * @param topic 消息主题,主题中不能保护空格、#、+、&、@等特殊字符
 * @param qos 消息级别
 * @param send_method 发送方法
 *
 * @return ET_QOS0发送成功时返回0,ET_QOS1、ET_QOS2返回消息UID\n
 *         <0 表示失败，详见@see et_code
*/
et_int32 et_publish(et_cloud_handle cloud_handle, et_uchar *data, et_uint32 len,     \
                    const et_char *topic, et_mes_qos_type qos, et_send_method_type send_method);
#endif
#if ET_CONFIG_FILE_EN
/**
 * 解析文件信息
 * @param cloud_handle 由et_create_context返回
 * @param file_str 待解析的文件消息
 * @param file_info 出参，解析出的文件信息存储结构，详见@see et_dfs_file_info_type
 *
 * @return 0 表示成功\n
 *         <0 表示失败，详见@see et_code
 */
et_int32 et_file_info(et_cloud_handle cloud_handle, et_char *file_str, et_dfs_file_info_type *file_info);

/**
 * 下载文件
 * @note 仅限iLink外网
 * @param cloud_handle 由et_create_context返回
 * @param file_info 待下载的文件信息
 * @param down_cb 文件下载回调，将在文件下载完成后调用，不需要可传NULL
 *
 * @return 0 表示成功\n
 *         <0 表示失败，详见@see et_code
 */
et_int32 et_download_file(et_cloud_handle cloud_handle, et_dfs_file_info_type *file_info,   \
                      et_file_down_process_type down_cb);
/**
 * 上传文件
 * @brief 将文件上传到文件服务器,但无接收方，仅用于存储
 * @note 仅限iLink外网
 * @param cloud_handle 由et_create_context返回
 * @param file_name 待上传的文件名
 * @param file_size 待上传的文件大小，单位byte,必须大于0
 * @param file_info 出参，发送的文件信息
 * @param upload_cb 发送文件回调,用户在回调中使用TCP发送文件数据,祥见@see et_file_uplaod_process_type
 *
 * @return 0 表示成功\n
 *         <0 表示失败，详见@see et_code
 */
et_int32 et_upload_file(et_cloud_handle cloud_handle, et_int8 *file_name, et_uint32 file_size,   \
                        et_dfs_file_info_type *file_info, et_file_uplaod_process_type upload_cb);

/**
 * 主动发送文件给iLink用户
 * @brief 将文件上传到文件服务器，指定接收UID，指定的接收方将收到一条文件消息
 * @note 仅限iLink外网
 * @param cloud_handle 由et_create_context返回
 * @param userid 接收方UID
 * @param file_name 待发送的文件名
 * @param file_size 待发送的文件大小，单位byte,必须大于0
 * @param file_info 出参，发送的文件信息
 * @param upload_cb 发送文件回调,用户在回调中使用TCP发送文件数据,祥见@see et_file_uplaod_process_type
 *
 * @return 0 表示成功\n
 *         <0 表示失败，详见@see et_code
 */
et_int32 et_file_to(et_cloud_handle cloud_handle, et_int8 *userid, et_int8 *file_name, et_uint32 file_size, \
                    et_dfs_file_info_type *file_info,et_file_uplaod_process_type upload_cb);
#endif

#if !ET_CONFIG_RTOS_EN
/**
 * IM轮询
 * @note 当用户在非RTOS环境下使用SDK时需调用该API进行轮询
 * @param cloud_handle 由et_create_context返回
 *
 * @return 0 表示成功
 *         <0 表示失败，详见@see et_code
 */
et_int32 et_im_loop(et_cloud_handle cloud_handle);
#endif

#if ET_CONFIG_HTTP_EN

/**
 * 添加好友
 * @param cloud_handle 由et_create_context返回
 * @param friendid 好友ID
 *
 * @return 0表示成功\n
 *         <0表示失败，详见@see et_code
 */
et_int32 et_add_buddy(et_cloud_handle cloud_handle, const et_char * friendid);

/**
 * 添加好友并通知
 * @note 操作将发送通知，且要求用户必须在线才能操作成功
 * @param cloud_handle 由et_create_context返回
 * @param friendid 好友ID
 * @param notify 是否通知，ET_TRUE-通知，ET_FALSE-不通知
 *
 * @return 0表示成功\n
 *         <0表示失败，详见@see et_code
 */
et_int32 et_add_buddy_notify(et_cloud_handle cloud_handle, const et_char * friendid, et_bool notify);

/**
 * 删除好友
 * @param cloud_handle 由et_create_context返回
 * @param friendid 好友ID
 *
 * @return 0表示成功\n
 *         <0表示失败，详见@see et_code
 */
et_int32 et_remove_buddy(et_cloud_handle cloud_handle, const et_char * friendid);

/**
 * 删除好友并通知
 * @note 操作将发送通知，且要求用户必须在线才能操作成功
 * @param cloud_handle 由et_create_context返回
 * @param friendid 好友ID
 * @param notify 是否通知，ET_TRUE-通知，ET_FALSE-不通知
 *
 * @return 0表示成功\n
 *         <0表示失败，详见@see et_code
 */
et_int32 et_remove_buddy_notify(et_cloud_handle cloud_handle, const et_char * friendid, et_bool notify);

/**
 * 获取好友列表
 * @param cloud_handle 由et_create_context返回
 * @param friends_list 出参，好友UID存储buffer
 * @param list_num friends_list第一维大小
 * @param buddies_num 出参，存储到friends_list中的UID数量，不需要获取则传NULL
 * @param buddies_num_max 出参，获取到的好友UID数，不需要获取则传NULL
 * @param summary_info 获取简要信息类型设置，ET-TRUE：只获取UID信息；ET-FALSE:获取完整的信息，包括UID、nickname、username
 *
 * @return 0表示成功\n
 *         <0表示失败，详见@see et_code
 */
et_int32 et_get_buddies(et_cloud_handle cloud_handle, et_char friends_list[ ] [ ET_USER_ID_LEN_MAX ],   \
                        et_uint32 list_num, et_int32 * buddies_num, et_int32 *buddies_num_max, et_bool summary_info);


/**
 * 创建群
 * @param cloud_handle 由et_create_context返回
 * @param group_name 群名称
 * @param members_list 加入该群的群成员UID列表
 * @param members_num members_list中存储的UID数量
 * @param group_topic 出参，创建的群topic，系统唯一标识
 * @param group_topic_size group_topic的大小，单位byte
 *
 * @return 0表示成功\n
 *         <0表示失败，详见@see et_code
 */
et_int32 et_create_group(et_cloud_handle cloud_handle, const et_char * group_name,     \
                         const et_char members_list[][ET_USER_ID_LEN_MAX], et_uint32 members_num, 	\
                         et_char *group_topic, et_int32 group_topic_size);

/**
 * 获取群列表
 * @param cloud_handle 由et_create_context返回
 * @param groups_list 出参，群列表buffer
 * @param list_num  groups_list可存储的群Topic数量，即第一维的大小
 * @param groups_num 出参，存储到groups_list中的Topic数量，不需要则传NULL
 * @param groups_num_max 出参，获取到的群Topic数量，不需要则传NULL
 * @param summary_info 获取简要信息设置，ET_TRUE：只获取群Topic；ET_FAISE：获取完整的信息，包括groupname、grouptopic
 *
 * @return 0表示成功\n
 *         <0表示失败，详见@see et_code
 */
et_int32 et_get_groups(et_cloud_handle cloud_handle, et_char groups_list[][ET_TOPIC_LEN_MAX], et_uint32 list_num, 	\
                       et_int32 *groups_num, et_int32 *groups_num_max, et_bool summary_info);
/**
 * 解散群
 * @note 只有群的创建者才可解散群
 * @param cloud_handle 由et_create_context返回
 * @param groups_topic 期望解散的群唯一标识，群topic
 *
 * @return 0表示成功\n
 *         <0表示失败，详见@see et_code
 */
et_int32 et_destroy_group(et_cloud_handle cloud_handle, const et_char *groups_topic);

/**
 * 退出群
 * @param cloud_handle 由et_create_context返回
 * @param groups_topic 期望退出的群唯一标识，群topic
 *
 * @return 0表示成功\n
 *         <0表示失败，详见@see et_code
 */
et_int32 et_exit_group(et_cloud_handle cloud_handle, const et_char *group_topic);

/**
 * 添加群成员
 * @param cloud_handle 由et_create_context返回
 * @param groups_topic 添加到的群唯一标识，群topic
 * @param members_list 待添加的成员UID列表
 * @param members_num members_list中的UID数量
 *
 * @return 0表示成功\n
 *         <0表示失败，详见@see et_code
 */
et_int32 et_add_group_members(et_cloud_handle cloud_handle, const et_char *group_topic,      \
                              const et_char members_list[][ET_USER_ID_LEN_MAX], et_uint32 members_num);

/**
 * 删除群成员
 * @param cloud_handle 由et_create_context返回
 * @param groups_topic 删除成员的群唯一标识，群topic
 * @param members_list 待删除的成员UID列表
 * @param members_num members_list中的UID数量
 *
 * @return 0表示成功\n
 *         <0表示失败，详见@see et_code
 */
et_int32 et_remove_group_members(et_cloud_handle cloud_handle, const et_char *group_topic,   \
                                 const et_char members_list[][ET_USER_ID_LEN_MAX], et_uint32 members_num);

/**
 * 获取群成员列表
 * @param cloud_handle 由et_create_context返回
 * @param groups_topic 期望获取的群唯一标识，群topic
 * @param members_list 出参，群成员UID存储buffer
 * @param list_num members_list中可存储的UID数量
 * @param members_num 出参，存储到members_list中的UID数量，不需要时传NULL
 * @param members_num_max 出参，获取到的群成员数量，不需要时传NULL
 *
 * @return 0表示成功\n
 *         <0表示失败，详见@see et_code
 */
et_int32 et_get_group_members(et_cloud_handle cloud_handle, const et_char *group_topic,      \
                              et_char members_list[][ET_USER_ID_LEN_MAX], et_uint32 list_num, 	\
                              et_int32 *members_num, et_int32 *members_num_max);

/**
 * 添加用户
 * @param cloud_handle 由et_create_context返回
 * @param http_server HTTP服务器地址，应为包含IP及端口的字符串，e.g."192.168.1.1:1000"
 * @note 若http_server参数传入NULL则使用et_http_init初始化到的地址
 * @param app_key 添加用户到该appkey,注册业务时获得
 * @param secret_key 注册业务时获得
 * @param user_name 用户账号
 * @param userid 出参，指向存储UID的buffer
 * @param userid_size userid的大小，单位byte
 *
 * @return 0表示成功\n
 *         <0表示失败，详见@see et_code
 */
et_int32 et_add_user(et_cloud_handle cloud_handle, const et_char *http_server, const et_char *app_key,     \
                     const et_char *secret_key, const et_char *user_name, et_char *userid, et_int32 userid_size);
#endif
#endif

