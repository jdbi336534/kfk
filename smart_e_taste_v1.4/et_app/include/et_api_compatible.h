/**
 * Copyright (c) 2016 ET iLink.
 * All rights reserved.
 */
/**
 *
 * @file et_config.h
 * @brief 本文件提供兼容V3.0.0.3版及之前的API名称
 * @date 5/10/2016
 * @author wangjia
 *
 */

#ifndef ET_API_COMPAT_H
#define ET_API_COMPAT_H

#include "et_client.h"

#define et_sdk_ver()    et_get_sdk_version()
//#define et_cloud_handle et_create_context(userid, app_key, secret_key)
//#define et_destroy_context(cloud_handle)
//#define et_set_callback(cloud_handle, mes_call, event_call)

#if ET_CONFIG_SERVER_EN

#define et_start_svr(cloud_handle)  et_start_server(cloud_handle)

#define et_stop_srv(cloud_handle)   et_stop_server(cloud_handle)
//#define et_get_local_users(cloud_handle, local_users_buf, users_num)

#endif

#if ET_CONFIG_CLOUD_EN

#define et_login_cloud(cloud_handle, connect_para)      et_connect(cloud_handle, connect_para)

#define et_relogin_cloud(cloud_handle, connect_para)    et_reconnect(cloud_handle, connect_para)
//#define et_get_user_state(cloud_handle, userid)
//#define et_sub_user_state(cloud_handle, userid)
//#define et_unsub_user_state(cloud_handle, userid)
#define et_req_offline_msg(cloud_handle)    et_request_offline_message(cloud_handle)
//#define et_subscribe(cloud_handle, topic, qos)
//#define et_unsubscribe(cloud_handle, topic)
#define et_logout_cloud(cloud_handle)       et_disconnect(cloud_handle)

#endif

#if ET_CONFIG_CLOUD_EN || ET_CONFIG_SERVER_EN

//#define et_chat_to(cloud_handle, data, len, userid, send_method)
//#define et_publish(cloud_handle, data, len, topic, qos, send_method)

#endif
#if ET_CONFIG_FILE_EN

//#define et_file_info(cloud_handle, file_str, file_info)
#define et_down_file(cloud_handle, file_info, down_cb)          et_download_file(cloud_handle, file_info, down_cb)
//#define et_upload_file(cloud_handle, file_name, file_size, file_info, upload_cb)
//#define et_file_to(cloud_handle, userid, file_name, file_size, file_info, upload_cb);
#endif

#if ET_CONFIG_HTTP_EN

#define et_http_add_buddy(cloud_handle, friendid)               et_add_buddy(cloud_handle, friendid)

#define et_http_add_buddy_ex(cloud_handle, friendid, notify)    et_add_buddy_notify(cloud_handle, friendid, notify)

#define et_http_delete_buddy(cloud_handle, friendid)            et_remove_buddy(cloud_handle, friendid)

#define et_http_delete_buddy_ex(cloud_handle, friendid, notify) et_remove_buddy_notify(cloud_handle, friendid, notify)

#define et_http_get_buddies(cloud_handle, friends_list, list_num, buddies_num, buddies_num_max, summary_info)   \
        et_get_buddies(cloud_handle, friends_list, list_num, buddies_num, buddies_num_max, summary_info)

#define et_http_create_group(cloud_handle, group_name, members_list, members_num, group_topic, group_topic_size)    \
        et_create_group(cloud_handle, group_name, members_list, members_num, group_topic, group_topic_size)

#define et_http_get_groups(cloud_handle, groups_list, list_num, groups_num, groups_num_max, summary_info)   \
        et_get_groups(cloud_handle, groups_list, list_num, groups_num, groups_num_max, summary_info)

#define et_http_release_group(cloud_handle, groups_topic)       et_destroy_group(cloud_handle, groups_topic)

#define et_http_logout_group(cloud_handle, group_topic)         et_exit_group(cloud_handle, group_topic)

#define et_http_add_group_membs(cloud_handle, group_topic, members_list, members_num)   \
        et_add_group_members(cloud_handle, group_topic, members_list, members_num)

#define et_http_delete_group_membs(cloud_handle, group_topic, members_list, members_num)    \
        et_remove_group_members(cloud_handle, group_topic, members_list, members_num)

#define et_http_get_group_membs(cloud_handle, group_topic, members_list, list_num, members_num, members_num_max)    \
        et_get_group_members(cloud_handle, group_topic, members_list, list_num, members_num, members_num_max)

#define et_http_add_user(cloud_handle, http_server, app_key, secret_key, user_name, userid, userid_size)    \
        et_add_user(cloud_handle, http_server, app_key, secret_key, user_name, userid, userid_size)
#endif


#endif
