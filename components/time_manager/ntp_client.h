#ifndef NTP_CLIENT_H
#define NTP_CLIENT_H

/* Includes - Standard APIs ----------------------------------------------------- */ // ---------------------------------
#include <stdint.h>
#include <stdbool.h>
#include <time.h>

/* Error Codes ------------------------------------------------------------------ */ // ---------------------------------
#define NTP_CLIENT_OK                 0
#define NTP_CLIENT_ERR_NOT_INIT      -1
#define NTP_CLIENT_ERR_NO_NETWORK    -2
#define NTP_CLIENT_ERR_SYNC_FAILED   -3
#define NTP_CLIENT_ERR_TIMEOUT       -4
#define NTP_CLIENT_ERR_INVALID_PARAM -5

/* Type Definitions - Structures ------------------------------------------------ */ // ---------------------------------
typedef struct {
    uint32_t sync_timeout_ms;   // NTP同步超时时间
    uint8_t retry_count;        // 最大重试次数
} ntp_client_config_t;

/* Global Constants ------------------------------------------------------------- */ // ---------------------------------
extern const ntp_client_config_t NTP_CLIENT_DEFAULT_CONFIG;

/* Public Function Declarations ------------------------------------------------- */ // ---------------------------------

/**
 * @brief 初始化NTP客户端
 * @param config 配置参数，NULL使用默认配置
 * @return NTP_CLIENT_OK 成功，其他值为错误码
 */
int ntp_client_init(const ntp_client_config_t* config);

/**
 * @brief 反初始化NTP客户端
 */
void ntp_client_deinit(void);

/**
 * @brief 从网络获取NTP时间戳
 * @param timestamp 输出参数，获取到的UTC时间戳
 * @return NTP_CLIENT_OK 成功，其他值为错误码
 * @note 此函数会检查网络连接状态，仅返回时间戳不修改系统时间
 */
int ntp_client_get_timestamp(time_t* timestamp);

#endif /* NTP_CLIENT_H */
