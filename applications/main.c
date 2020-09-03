/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-01-08     shelton      first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include "board.h"
#include "drv_gpio.h"
#include "drv_flash.h"

/* defined the LED2 pin: PD13 */
#define LED2_PIN    GET_PIN(D, 13)
/* defined the LED3 pin: PD14 */
#define LED3_PIN    GET_PIN(D, 14)
/* defined the LED4 pin: PD15 */
#define LED4_PIN    GET_PIN(D, 15)

int main(void)
{
    uint32_t Speed = 200;
    /* set LED2 pin mode to output */
    rt_pin_mode(LED2_PIN, PIN_MODE_OUTPUT);
    /* set LED3 pin mode to output */
    rt_pin_mode(LED3_PIN, PIN_MODE_OUTPUT);
    /* set LED4 pin mode to output */
    rt_pin_mode(LED4_PIN, PIN_MODE_OUTPUT);

    while (1)
    {
        rt_pin_write(LED2_PIN, PIN_LOW);
        rt_thread_mdelay(Speed);
        rt_pin_write(LED3_PIN, PIN_LOW);
        rt_thread_mdelay(Speed);
        rt_pin_write(LED4_PIN, PIN_LOW);
        rt_thread_mdelay(Speed);
        rt_pin_write(LED2_PIN, PIN_HIGH);
        rt_thread_mdelay(Speed);
        rt_pin_write(LED3_PIN, PIN_HIGH);
        rt_thread_mdelay(Speed);
        rt_pin_write(LED4_PIN, PIN_HIGH);
        rt_thread_mdelay(Speed);
    }
}

#define BUF_SIZE  16
static rt_uint32_t test_addr = 0x80F0000;
static rt_uint8_t r_buf[BUF_SIZE];
static rt_uint8_t w_buf[BUF_SIZE] = {'h', 'e', 'l', 'l', 'o', ',', ' ', 'w', 'o', 'r', 'l', 'd', '!', '!', '!', '\0'};

static void show_hex(rt_uint8_t *buf, size_t size)
{
    int pos = 0;
    for (pos=0; pos<size; pos++)
    {
        if (pos%8 == 0)
            rt_kprintf("\n");

        rt_kprintf("%x ", buf[pos]);
    }
    rt_kprintf("\n");
}

static void flash_read(int argc, char *argv[])
{
    rt_kprintf("flash read\n");

    //int at32_flash_read(rt_uint32_t addr, rt_uint8_t *buf, size_t size);
    at32_flash_read(test_addr, r_buf, BUF_SIZE);

    show_hex(r_buf, BUF_SIZE);
    rt_kprintf("read: %s\n", r_buf);
}

static void flash_write(int argc, char *argv[])
{
    rt_kprintf("flash write\n");

    //int at32_flash_write(rt_uint32_t addr, const rt_uint8_t *buf, size_t size);
    at32_flash_write(test_addr, w_buf, BUF_SIZE);

    show_hex(w_buf, BUF_SIZE);
    rt_kprintf("write: %s\n", w_buf);
}

static void flash_erase(int argc, char *argv[])
{
    rt_kprintf("flash erase\n");

    //int at32_flash_erase(rt_uint32_t addr, size_t size);
    at32_flash_erase(test_addr, BUF_SIZE);
}

#ifdef FINSH_USING_MSH
MSH_CMD_EXPORT(flash_read,  Read at32 flash);
MSH_CMD_EXPORT(flash_write, Write at32 flash);
MSH_CMD_EXPORT(flash_erase, Erase at32 flash);
#endif