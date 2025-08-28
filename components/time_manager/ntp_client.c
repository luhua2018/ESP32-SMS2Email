/*
 * NTP客户端组件 - 仅支持IPv4网络时间获取
 * 通过网络获取 NTP 服务器的时间
 * 基于 ESP-IDF SNTP 示例修改
 * 原始来源: https://github.com/espressif/esp-idf/blob/47a659cd3ee5f73a253d2a7af02781a73a44356d/examples/protocols/sntp/main/sntp_example_main.c
 */

/* Includes - Standard APIs ----------------------------------------------------- */ // ---------------------------------
#include <string.h>
#include <time.h>
#include <sys/time.h>

/* Includes - System APIs ------------------------------------------------------- */ // ---------------------------------
#include "esp_system.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_netif_sntp.h"
#include "esp_sntp.h"
#include "lwip/ip_addr.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

/* Includes - User APIs --------------------------------------------------------- */ // ---------------------------------
#include "ntp_client.h"

/* Private Configuration -------------------------------------------------------- */ // ---------------------------------
#define TAG                         "NTP_CLIENT"

/* Private Type Definitions ----------------------------------------------------- */ // ---------------------------------
typedef struct {
    bool initialized;
    bool sync_in_progress;
    time_t retrieved_timestamp;
    ntp_client_config_t config;
} ntp_client_context_t;

/* Global Constants ------------------------------------------------------------- */ // ---------------------------------
const ntp_client_config_t NTP_CLIENT_DEFAULT_CONFIG = {
    .sync_timeout_ms = CONFIG_NTP_CLIENT_SYNC_TIMEOUT_MS,
    .retry_count = CONFIG_NTP_CLIENT_RETRY_COUNT
};

/* Static Variables ------------------------------------------------------------- */ // ---------------------------------
static ntp_client_context_t s_context = {0};

/* Private Function Declarations ----------------------------------------------- */ // ---------------------------------
static bool ntp_client_is_network_connected(void);
static int ntp_client_configure_servers(void);
static void ntp_client_print_servers(void);
static void ntp_client_sync_notification_cb(struct timeval *tv);

/* Private Function Implementations -------------------------------------------- */ // ---------------------------------

/**
 * @brief 检查IPv4网络连接状态
 * @return true 已连接，false 未连接
 */
static bool ntp_client_is_network_connected(void)
{
    esp_netif_t* netif = esp_netif_get_handle_from_ifkey("WIFI_STA_DEF");
    if (netif == NULL) {
        netif = esp_netif_get_handle_from_ifkey("ETH_DEF");
    }
    
    if (netif == NULL) {
        return false;
    }
    
    esp_netif_ip_info_t ip_info;
    if (esp_netif_get_ip_info(netif, &ip_info) == ESP_OK) {
        return (ip_info.ip.addr != 0);  // 有IPv4地址说明已连接
    }
    
    return false;
}

/**
 * @brief 配置NTP服务器
 * @return NTP_CLIENT_OK 成功，其他值为错误码
 */
static int ntp_client_configure_servers(void)
{
    /* 仅初始化SNTP服务，不预设任何服务器，服务器配置在 get_timestamp 中动态设置 */
    esp_sntp_config_t config = ESP_NETIF_SNTP_DEFAULT_CONFIG("temp.server");
    config.sync_cb = ntp_client_sync_notification_cb;
    config.smooth_sync = false;
    
    esp_err_t err = esp_netif_sntp_init(&config);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize SNTP: %s", esp_err_to_name(err));
        return NTP_CLIENT_ERR_SYNC_FAILED;
    }
    
    /* 服务器配置在 get_timestamp 中动态设置 */
    return NTP_CLIENT_OK;
}

/**
 * @brief 打印配置的NTP服务器列表（仅IPv4）
 */
static void ntp_client_print_servers(void)
{
    ESP_LOGI(TAG, "Configured IPv4 NTP servers:");
    
    for (uint8_t i = 0; i < SNTP_MAX_SERVERS; ++i) {
        const char* server_name = esp_sntp_getservername(i);
        if (server_name) {
            ESP_LOGI(TAG, "  Server %d: %s", i, server_name);
        } else {
            const ip_addr_t* ip = esp_sntp_getserver(i);
            if (ip && IP_IS_V4(ip)) {  // 仅处理IPv4地址
                ESP_LOGI(TAG, "  Server %d: " IPSTR, i, IP2STR(&ip->u_addr.ip4));
            }
        }
    }
}

/**
 * @brief SNTP时间同步通知回调
 * @note 仅保存时间戳，不修改系统时间
 */
static void ntp_client_sync_notification_cb(struct timeval *tv)
{
    if (tv) {
        s_context.retrieved_timestamp = tv->tv_sec;
        ESP_LOGI(TAG, "NTP timestamp retrieved: %ld", s_context.retrieved_timestamp);
    }
    s_context.sync_in_progress = false;
}

/* Public Function Implementations ---------------------------------------------- */ // ---------------------------------

int ntp_client_init(const ntp_client_config_t* config)
{
    if (s_context.initialized) {
        ESP_LOGW(TAG, "NTP client already initialized");
        return NTP_CLIENT_OK;
    }
    
    // 使用提供的配置或默认配置
    if (config) {
        s_context.config = *config;
    } else {
        s_context.config = NTP_CLIENT_DEFAULT_CONFIG;
    }
    
    s_context.initialized = true;
    s_context.sync_in_progress = false;
    s_context.retrieved_timestamp = 0;
    
    ESP_LOGI(TAG, "NTP client initialized (IPv4 only)");
    return NTP_CLIENT_OK;
}

void ntp_client_deinit(void)
{
    if (!s_context.initialized) {
        return;
    }
    
    esp_netif_sntp_deinit();
    s_context.initialized = false;
    s_context.sync_in_progress = false;
    s_context.retrieved_timestamp = 0;
    
    ESP_LOGI(TAG, "NTP client deinitialized");
}

int ntp_client_get_timestamp(time_t* timestamp)
{
    if (!s_context.initialized) {
        ESP_LOGE(TAG, "NTP client not initialized");
        return NTP_CLIENT_ERR_NOT_INIT;
    }
    
    if (!timestamp) {
        ESP_LOGE(TAG, "Invalid parameter: timestamp is NULL");
        return NTP_CLIENT_ERR_INVALID_PARAM;
    }
    
    /* 检查IPv4网络连接状态 */
    if (!ntp_client_is_network_connected()) {
        ESP_LOGE(TAG, "IPv4 network not connected");
        return NTP_CLIENT_ERR_NO_NETWORK;
    }
    
    /* 配置并启动SNTP服务 */
    int ret = ntp_client_configure_servers();
    if (ret != NTP_CLIENT_OK) {
        return ret;
    }
    
    ntp_client_print_servers();
    s_context.retrieved_timestamp = 0;
    
    ESP_LOGI(TAG, "Retrieving NTP timestamp from IPv4 servers...");
    
    /* NTP服务器列表，按优先级排序 */
    const char* servers[] = {
        CONFIG_NTP_CLIENT_PRIMARY_SERVER,
        CONFIG_NTP_CLIENT_SECONDARY_SERVER, 
        CONFIG_NTP_CLIENT_TERTIARY_SERVER
    };
    
    /* 依次尝试每个NTP服务器 */
    for (int server_idx = 0; server_idx < 3 && s_context.retrieved_timestamp == 0; server_idx++) {
        ESP_LOGI(TAG, "Trying server %d: %s", server_idx, servers[server_idx]);
        
        /* 设置当前服务器为主服务器并重启SNTP */
        esp_sntp_setservername(0, servers[server_idx]);
        esp_sntp_restart();
        
        s_context.sync_in_progress = true;
        uint32_t retry = 0;
        /* 每个服务器分配总超时时间的1/3 */
        uint32_t server_timeout = s_context.config.sync_timeout_ms / 3;
        uint32_t wait_time = server_timeout / s_context.config.retry_count;
        
        /* 对当前服务器进行重试直到成功或达到重试上限 */
        while (s_context.sync_in_progress && retry < s_context.config.retry_count) {
            esp_err_t sync_result = esp_netif_sntp_sync_wait(wait_time / portTICK_PERIOD_MS);
            if (sync_result == ESP_OK) {
                ESP_LOGI(TAG, "Successfully got timestamp from server %d: %s", server_idx, servers[server_idx]);
                break;
            }
            
            retry++;
            ESP_LOGI(TAG, "Server %d (%s) attempt %"PRIu32"/%d timeout", 
                     server_idx, servers[server_idx], retry, s_context.config.retry_count);
        }
        
        /* 如果成功获取时间戳，退出服务器循环 */
        if (s_context.retrieved_timestamp != 0) {
            break;
        }
        
        ESP_LOGW(TAG, "Server %d (%s) failed after %d attempts, trying next server", 
                 server_idx, servers[server_idx], s_context.config.retry_count);
    }
    
    /* 清理SNTP资源 */
    esp_netif_sntp_deinit();
    
    /* 检查是否成功获取时间戳 */
    if (s_context.retrieved_timestamp == 0) {
        ESP_LOGE(TAG, "All NTP servers failed to respond");
        s_context.sync_in_progress = false;
        return NTP_CLIENT_ERR_TIMEOUT;
    }
    
    *timestamp = s_context.retrieved_timestamp;
    ESP_LOGI(TAG, "NTP timestamp successfully retrieved: %ld", *timestamp);
    
    return NTP_CLIENT_OK;
}
