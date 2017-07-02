#include "et_types.h"
#include "user_config.h"

void fac_read_uart_task(void *pvParameters);
SpiFlashOpResult get_server(et_server_info_t *server);
SpiFlashOpResult get_uid(id_info_t *uid);
SpiFlashOpResult get_fac_norm_mode(et_uchar *result);