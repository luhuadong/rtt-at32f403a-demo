/*
 * drv_flash_f4.c
 *
 * Change Logs:
 * Date           Author            Notes
 * 2020-09-03     qiyongzhong       first version
 */

#ifndef __DRV_FLASH_H__
#define __DRV_FLASH_H__

#include <rtthread.h>
#include "rtdevice.h"
#include <rthw.h>
#include <drv_common.h>

#if (AT32_FLASH_SIZE < (256*1024))
#define AT32_FLASH_PAGE_SIZE    (1*1024)
#else
#define AT32_FLASH_PAGE_SIZE    (2*1024)
#endif

#ifdef __cplusplus
extern "C" {
#endif

int at32_flash_read(rt_uint32_t addr, rt_uint8_t *buf, size_t size);
int at32_flash_write(rt_uint32_t addr, const rt_uint8_t *buf, size_t size);
int at32_flash_erase(rt_uint32_t addr, size_t size);

#ifdef __cplusplus
}
#endif

#endif  /* __DRV_FLASH_H__ */
