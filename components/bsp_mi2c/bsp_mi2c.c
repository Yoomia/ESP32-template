/*
 * @Description:
 * @Author:
 * @Date: 2021-12-17 14
 * @Path: D:\ESP32\Pwc\components\bsp_mi2c\bsp_mi2c.c
 */
#include <stdio.h>
#include "bsp_mi2c.h"

#include "esp_log.h"
#include "driver/i2c.h"

#define BSP_MI2C_TIMEOUT 10

#define BSP_MI2C_ACK_EN 1
#define BSP_MI2C_ACK_DISEN 0

#define BSP_MI2C_WRITE 0
#define BSP_MI2C_READ 1

static const char *TAG = "mi2c";

/**
 * @description:
 * @param:  null
 * @return: null
 * @note:
 */
void bsp_mi2c_write(bsp_i2c_dev_t *dev, uint8_t *data, size_t size)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, ((dev->address << 1) | BSP_MI2C_WRITE), BSP_MI2C_ACK_EN);
    i2c_master_write(cmd, data, size, BSP_MI2C_ACK_EN);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(dev->port, cmd, BSP_MI2C_TIMEOUT);
    i2c_cmd_link_delete(cmd);
}

/**
 * @description: The mode without writing device registers is applicable to a few I2C devices such as bh1750 and ads1115 / 1118, which usually have few internal registers
 * @param:  null
 * @return: null
 * @note:
 */
void bsp_mi2c_read(bsp_i2c_dev_t *dev, uint8_t *data, size_t size)
{
    if (size == 0)
    {
        return;
    }
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, ((dev->address << 1) | BSP_MI2C_READ), BSP_MI2C_ACK_EN);
    if (size > 1)
    {
        i2c_master_read(cmd, data, size - 1, I2C_MASTER_ACK);
    }
    i2c_master_read_byte(cmd, data + (size - 1), I2C_MASTER_NACK);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(dev->port, cmd, BSP_MI2C_TIMEOUT);
    i2c_cmd_link_delete(cmd);
}

/**
 * @description:
 * @param:  null
 * @return: null
 * @note:
 */
void bsp_mi2c_write_reg(bsp_i2c_dev_t *dev, uint8_t reg, uint8_t *data, size_t size)
{

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, ((dev->address << 1) | BSP_MI2C_WRITE), BSP_MI2C_ACK_EN);
    i2c_master_write_byte(cmd, reg, BSP_MI2C_ACK_EN);
    i2c_master_write(cmd, data, size, BSP_MI2C_ACK_EN);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(dev->port, cmd, BSP_MI2C_TIMEOUT);
    i2c_cmd_link_delete(cmd);
}

/**
 * @description:
 * @param:  null
 * @return: null
 * @note:
 */
void bsp_mi2c_read_reg(bsp_i2c_dev_t *dev, uint8_t reg, uint8_t *data, size_t size)
{
    if (size == 0)
    {
        return;
    }
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, ((dev->address << 1) | BSP_MI2C_WRITE), BSP_MI2C_ACK_EN);
    i2c_master_write_byte(cmd, reg, BSP_MI2C_ACK_EN);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, ((dev->address << 1) | BSP_MI2C_READ), BSP_MI2C_ACK_EN);
    if (size > 1)
    {
        i2c_master_read(cmd, data, size - 1, I2C_MASTER_ACK);
    }
    i2c_master_read_byte(cmd, data + (size - 1), I2C_MASTER_NACK);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(dev->port, cmd, BSP_MI2C_TIMEOUT);
    i2c_cmd_link_delete(cmd);
}

/**
 * @description:
 * @param:  null
 * @return: null
 * @note:
 */
void bsp_mi2c_read_16bit_reg(bsp_i2c_dev_t *dev, uint16_t reg, uint8_t *data, size_t size)
{
    if (size == 0)
    {
        return;
    }
    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, ((dev->address << 1) | BSP_MI2C_WRITE), BSP_MI2C_ACK_EN);
    i2c_master_write_byte(cmd, (reg >> 8), BSP_MI2C_ACK_EN);
    i2c_master_write_byte(cmd, (reg & 0xFF), BSP_MI2C_ACK_EN);
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, ((dev->address << 1) | BSP_MI2C_READ), BSP_MI2C_ACK_EN);
    if (size > 1)
    {
        i2c_master_read(cmd, data, size - 1, I2C_MASTER_ACK);
    }
    i2c_master_read_byte(cmd, data + (size - 1), I2C_MASTER_NACK);
    i2c_master_stop(cmd);
    i2c_master_cmd_begin(dev->port, cmd, BSP_MI2C_TIMEOUT);
    i2c_cmd_link_delete(cmd);
}

/**
 * @description:
 * @param:  null
 * @return: null
 * @note:
 */
void bsp_mi2c_init(uint8_t port, uint32_t scl, uint32_t sda, uint32_t frequency)
{
    i2c_config_t config =
        {
            .mode = I2C_MODE_MASTER,
            .sda_io_num = sda,
            .sda_pullup_en = GPIO_PULLUP_ENABLE,
            .scl_io_num = scl,
            .scl_pullup_en = GPIO_PULLUP_ENABLE,
            .master.clk_speed = frequency,
        };
    i2c_param_config(port, &config);
    i2c_driver_install(port, config.mode, 0, 0, 0);
}
