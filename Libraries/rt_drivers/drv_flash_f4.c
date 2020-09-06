/*
 * drv_flash_f4.c
 *
 * Change Logs:
 * Date           Author            Notes
 * 2020-09-03     qiyongzhong       first version
 */

#include "board.h"
 
#ifdef BSP_USING_ON_CHIP_FLASH
#include "drv_flash.h"
 
#if defined(PKG_USING_FAL)
#include "fal.h"
#endif
 
 //#define DRV_DEBUG
#define LOG_TAG                "drv.flash"
#include <drv_log.h>

 
 /**
  * Read data from flash.
  * @note This operation's units is byte.
  *
  * @param addr flash address
  * @param buf buffer to store read data
  * @param size read bytes size
  *
  * @return result
  */
int at32_flash_read(rt_uint32_t addr, rt_uint8_t *buf, size_t size)
{
    size_t i;
    rt_uint8_t *p = (rt_uint8_t *)addr;
    rt_uint32_t end_addr = addr + size;

    if (end_addr > AT32_FLASH_END_ADDRESS)
    {
        LOG_E("read outrange flash size! addr is (0x%p)", (void*)end_addr);
        return -RT_EINVAL;
    }

    for (i = 0; i<size; i++)
    {
        *buf++ = *p++;
    }

    return size;
}
 
 /**
  * Write data to flash.
  * @note This operation's units is halfword.
  * @note This operation must after erase. @see flash_erase.
  *
  * @param addr flash address
  * @param buf the write data buffer
  * @param size write bytes size
  *
  * @return result
  */
 
int at32_flash_write(rt_uint32_t addr, const rt_uint8_t *buf, size_t size)
{
    rt_err_t result = RT_EOK;
    rt_uint16_t write_data = 0;
    rt_uint32_t end_addr = addr + size;

    if (end_addr > AT32_FLASH_END_ADDRESS)
    {
        LOG_E("ERROR: write outrange flash size! addr is (0x%p)\n", (void*)end_addr);
        return -RT_EINVAL;
    }

    if(addr % 2 != 0)
    {
        LOG_E("write addr must be 2-byte alignment");
        return -RT_EINVAL;
    }
    
    if(size % 2 != 0)
    {
        LOG_E("write size must be 2 multiples");
        return -RT_EINVAL;
    }

    FLASH_Unlock();
  
    while (addr < end_addr)
    {
        rt_uint8_t *pd = (rt_uint8_t *)&write_data;
        
        pd[0] = *buf++;
        pd[1] = *buf++;

        if (FLASH_ProgramHalfWord(addr, write_data) != FLASH_PRC_DONE)
        {
            result = -RT_ERROR;
            break;
        }
        if (*(rt_uint16_t*)addr != write_data)
        {
            LOG_E("ERROR: write data != read data\n");
            result = -RT_ERROR;
            break;
        }
        addr += 2;
    }
 
    FLASH_Lock();
     
    if (result != RT_EOK)
    {
        return result;
    }
 
    return size;
}
 
/**
 * Erase data on flash.
 * @note This operation is irreversible.
 * @note This operation's units is different which on many chips.
 *
 * @param addr flash address
 * @param size erase bytes size
 *
 * @return result
 */
int at32_flash_erase(rt_uint32_t addr, size_t size)
{
    rt_err_t result = RT_EOK;
    rt_uint32_t begin_addr = addr;
    rt_uint32_t end_addr = addr + size;

    if (end_addr > AT32_FLASH_END_ADDRESS)
    {
        LOG_E("ERROR: erase outrange flash size! addr is (0x%p)\n", (void*)end_addr);
        return -RT_EINVAL;
    }
    
    addr &= ~(AT32_FLASH_PAGE_SIZE - 1);
    begin_addr = addr;
    size = 0;

    FLASH_Unlock();

    while (addr < end_addr)
    {
        if (FLASH_ErasePage(addr) != FLASH_PRC_DONE)
        {
            result = -RT_ERROR;
            break;
        }
        addr += AT32_FLASH_PAGE_SIZE;
        size += AT32_FLASH_PAGE_SIZE;
    }

    FLASH_Lock();

    if (result != RT_EOK)
    {
        return result;
    }

    LOG_D("erase done: addr (0x%p), size %d", (void*)begin_addr, size);
    return size;
}
 
#if defined(PKG_USING_FAL)
 
 static int fal_flash_read(long offset, rt_uint8_t *buf, size_t size);
 static int fal_flash_write(long offset, const rt_uint8_t *buf, size_t size);
 static int fal_flash_erase(long offset, size_t size);
 
 const struct fal_flash_dev at32_onchip_flash = { "onchip_flash", AT32_FLASH_START_ADRESS, AT32_FLASH_SIZE, AT32_FLASH_PAGE_SIZE, {NULL, fal_flash_read, fal_flash_write, fal_flash_erase} };
 
 static int fal_flash_read(long offset, rt_uint8_t *buf, size_t size)
 {
     return at32_flash_read(at32_onchip_flash.addr + offset, buf, size);
 }
 
 static int fal_flash_write(long offset, const rt_uint8_t *buf, size_t size)
 {
     return at32_flash_write(at32_onchip_flash.addr + offset, buf, size);
 }
 
 static int fal_flash_erase(long offset, size_t size)
 {
     return at32_flash_erase(at32_onchip_flash.addr + offset, size);
 }
 
#endif
#endif /* BSP_USING_ON_CHIP_FLASH */


