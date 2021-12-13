/**
 * @file hts221.c
 *
 * MIT Licensed as described in the file LICENSE
 */
#include <esp_log.h>
#include <esp_idf_lib_helpers.h>
#include "hts221.h"

esp_err_t hts221_init_default_params(hts221_params_t *params) 
{
    params->internal_avg_temp = HTS221_AVGT_4;
    params->internal_avg_humid = HTS221_AVGH_4;
    params->data_rate = HTS221_ODR_7HZ;
    params->BDU_flag = HTS221_BDU_ON;
    params->internal_heater = HTS221_HEATER_OFF;
    params->DRDY_pin = HTS221_DRDY_DISABLE;
    params->DRDY_mode = HTS221_DRDY_PUSHPULL;
    params->DRDY_active = HTS221_DRDY_ACTIVEH;
    return ESP_OK;
}

esp_err_t hts221_init(hts221_t *dev, hts221_params_t *params)
{
    uint8_t buf[2];
    uint16_t T0_out, T1_out;
    uint16_t T0_degC_x8, T1_degC_x8, T0_degC, T1_degC;
    uint16_t H0_out, H1_out;
    uint8_t H0_rH, H1_rH;

    I2C_DEV_TAKE_MUTEX(&dev->i2c_dev);

    // Check chip ID
    i2c_dev_read_reg(&dev->i2c_dev, HTS221_WHO_AM_I, &dev->id, 1);
    if (dev->id != HTS221_CHIP_ID) {
        I2C_DEV_GIVE_MUTEX(&dev->i2c_dev);
        return ESP_FAIL;
    }

    // Enable chip, configure data rate and BDU
    i2c_dev_read_reg(&dev->i2c_dev, HTS221_CTRL_REG1, &buf[0], 1);
    buf[0] &= ~(HTS221_CTRL_REG1_MASK);
    buf[0] |= (HTS221_POWER_ACTIVE | params->data_rate | params->BDU_flag);
    i2c_dev_write_reg(&dev->i2c_dev, HTS221_CTRL_REG1, &buf[0], 1);
    // Configure internal heater
    i2c_dev_read_reg(&dev->i2c_dev, HTS221_CTRL_REG2, &buf[0], 1);
    buf[0] &= ~(HTS221_CTRL_REG2_MASK);
    buf[0] |= (params->internal_heater);
    i2c_dev_write_reg(&dev->i2c_dev, HTS221_CTRL_REG2, &buf[0], 1);
    // Configure DRDY pin
    i2c_dev_read_reg(&dev->i2c_dev, HTS221_CTRL_REG3, &buf[0], 1);
    buf[0] &= ~(HTS221_CTRL_REG3_MASK);
    buf[0] |= (params->DRDY_pin | params->DRDY_mode | params->DRDY_active);
    i2c_dev_write_reg(&dev->i2c_dev, HTS221_CTRL_REG2, &buf[0], 1);

    // Set internal averaging parameters
    i2c_dev_read_reg(&dev->i2c_dev, HTS221_AV_CONF, &buf[0], 1);
    buf[0] &= ~(HTS221_AV_CONF_MASK);
    buf[0] |= (params->internal_avg_temp | params->internal_avg_humid);
    i2c_dev_write_reg(&dev->i2c_dev, HTS221_AV_CONF, &buf[0], 1);

    // Read temperature calibration coefficients
    i2c_dev_read_reg(&dev->i2c_dev, HTS221_T0_OUT, &buf[0], 1);
    i2c_dev_read_reg(&dev->i2c_dev, HTS221_T0_OUT+1, &buf[1], 1);
    T0_out = ((uint16_t)buf[1] << 8) | (uint16_t)buf[0];
    i2c_dev_read_reg(&dev->i2c_dev, HTS221_T1_OUT, &buf[0], 1);
    i2c_dev_read_reg(&dev->i2c_dev, HTS221_T1_OUT+1, &buf[1], 1);
    T1_out = ((uint16_t)buf[1] << 8) | (uint16_t)buf[0];
    i2c_dev_read_reg(&dev->i2c_dev, HTS221_T0_DEGC_X8, &buf[0], 1);
    T0_degC_x8 = buf[0];
    i2c_dev_read_reg(&dev->i2c_dev, HTS221_T1_DEGC_X8, &buf[0], 1);
    T1_degC_x8 = buf[0];
    i2c_dev_read_reg(&dev->i2c_dev, HTS221_T0_T1_DEGC_H2, &buf[0], 1);
    T0_degC_x8 |= (((uint16_t)buf[0] & 0x03) << 8);
    T1_degC_x8 |= (((uint16_t)buf[0] & 0x0C) << 6);
    T0_degC = T0_degC_x8 >> 3;
    T1_degC = T1_degC_x8 >> 3;
    dev->T_degC_slope = (float)(T1_degC - T0_degC) / (float)(T1_out - T0_out); 
    dev->T_degC_offset = T0_degC;
    dev->T_offset = (float)T0_out;

    // Read humidity calibration coefficients
    i2c_dev_read_reg(&dev->i2c_dev, HTS221_H0_T0_OUT, &buf[0], 1);
    i2c_dev_read_reg(&dev->i2c_dev, HTS221_H0_T0_OUT+1, &buf[1], 1);
    H0_out = ((uint16_t)buf[1] << 8) | (uint16_t)buf[0];
    i2c_dev_read_reg(&dev->i2c_dev, HTS221_H1_T0_OUT, &buf[0], 1);
    i2c_dev_read_reg(&dev->i2c_dev, HTS221_H1_T0_OUT+1, &buf[1], 1);
    H1_out = ((uint16_t)buf[1] << 8) | (uint16_t)buf[0];
    i2c_dev_read_reg(&dev->i2c_dev, HTS221_H0_RH_X2, buf, 1);
    H0_rH = buf[0] >> 1;
    i2c_dev_read_reg(&dev->i2c_dev, HTS221_H1_RH_X2, buf, 1); 
    H1_rH = buf[0] >> 1;
    dev->H_rH_slope = (float)(H1_rH - H0_rH) / (float)(H1_out - H0_out);
    dev->H_rH_offset = H0_rH;
    dev->H_offset = (float)H0_out;

    I2C_DEV_GIVE_MUTEX(&dev->i2c_dev);
    return ESP_OK;
}

esp_err_t hts221_read_float(hts221_t *dev, float *temperature, float *humidity)
{
    uint8_t buf[2];
    uint16_t T_out, H_out;

    I2C_DEV_TAKE_MUTEX(&dev->i2c_dev);

    // Check data status
    i2c_dev_read_reg(&dev->i2c_dev, HTS221_STATUS_REG, buf, 1);
    while ((buf[0] & HTS221_STATUS_REG_MASK) != (HTS221_TEMP_RDY_BIT | HTS221_HUMID_RDY_BIT)) {
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }

    // Read raw temperature data
    i2c_dev_read_reg(&dev->i2c_dev, HTS221_TEMP_OUT_L, &buf[0], 1);
    i2c_dev_read_reg(&dev->i2c_dev, HTS221_TEMP_OUT_H, &buf[1], 1);
    T_out = ((uint16_t)buf[1] << 8) | (uint16_t)buf[0];
    *temperature = (T_out - dev->T_offset) * dev->T_degC_slope  +  dev->T_degC_offset;

    // Read raw humidity data
    i2c_dev_read_reg(&dev->i2c_dev, HTS221_HUMIDITY_OUT_L, &buf[0], 1);
    i2c_dev_read_reg(&dev->i2c_dev, HTS221_HUMIDITY_OUT_H, &buf[1], 1);
    H_out = ((uint16_t)buf[1] << 8) | (uint16_t)buf[0];
    *humidity = (H_out - dev->H_offset) * dev->H_rH_slope + dev->H_rH_offset;

    I2C_DEV_GIVE_MUTEX(&dev->i2c_dev);
    return ESP_OK;
}