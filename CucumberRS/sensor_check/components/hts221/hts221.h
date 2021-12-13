/**
 * @file hts221.h
 *
 * MIT Licensed as described in the file LICENSE
 */
#ifndef __HTS221_H__
#define __HTS221_H__

#include <stdint.h>
#include <stdbool.h>
#include <esp_err.h>
#include <i2cdev.h>


#ifdef __cplusplus
extern "C" {
#endif

#define HTS221_I2C_ADDRESS      0x5F

#define HTS221_WHO_AM_I         0x0F
#define HTS221_CHIP_ID          0xBC

#define HTS221_AV_CONF          0x10
#define HTS221_AV_CONF_MASK     (0x38 | 0x07)
#define HTS221_AVGH_4           0x00
#define HTS221_AVGH_8           0x01
#define HTS221_AVGH_16          0x02
#define HTS221_AVGH_32          0x03
#define HTS221_AVGH_64          0x04
#define HTS221_AVGH_128         0x05
#define HTS221_AVGH_256         0x06
#define HTS221_AVGH_512         0x07
#define HTS221_AVGT_2           0x00      
#define HTS221_AVGT_4           0x08
#define HTS221_AVGT_8           0x10
#define HTS221_AVGT_16          0x18
#define HTS221_AVGT_32          0x20
#define HTS221_AVGT_64          0x28
#define HTS221_AVGT_128         0x30
#define HTS221_AVGT_256         0x38

#define HTS221_CTRL_REG1        0x20
#define HTS221_CTRL_REG1_MASK   (0x04 | 0x03)
#define HTS221_ODR_ONESHOT      0x00
#define HTS221_ODR_1HZ          0x01
#define HTS221_ODR_7HZ          0x02
#define HTS221_ODR_12_5HZ       0x03
#define HTS221_BDU_OFF          0x00           
#define HTS221_BDU_ON           0x04
#define HTS221_POWER_ACTIVE     0x80

#define HTS221_CTRL_REG2        0x21
#define HTS221_CTRL_REG2_MASK   (0x80 | 0x02 | 0x01)
#define HTS221_ONESHOT_START    0x01
#define HTS221_HEATER_OFF       0x00
#define HTS221_HEATER_ON        0x02
#define HTS221_MEMORY_REBOOT    0x80

#define HTS221_CTRL_REG3        0x22
#define HTS221_CTRL_REG3_MASK   (0x80 | 0x40 | 0x04)
#define HTS221_DRDY_DISABLE     0x00
#define HTS221_DRDY_ENABLE      0x04
#define HTS221_DRDY_PUSHPULL    0x00
#define HTS221_DRDY_OPENDRAIN   0x40
#define HTS221_DRDY_ACTIVEH     0x00
#define HTS221_DRDY_ACTIVEL     0x80

#define HTS221_STATUS_REG       0x27
#define HTS221_STATUS_REG_MASK  (0x02 | 0x01)
#define HTS221_TEMP_RDY_BIT     0x01       
#define HTS221_HUMID_RDY_BIT    0x02

#define HTS221_HUMIDITY_OUT_L   0x28
#define HTS221_HUMIDITY_OUT_H   0x29
#define HTS221_TEMP_OUT_L       0x2A
#define HTS221_TEMP_OUT_H       0x2B

#define HTS221_H0_RH_X2         0x30
#define HTS221_H1_RH_X2         0x31

#define HTS221_T0_DEGC_X8       0x32
#define HTS221_T1_DEGC_X8       0x33
#define HTS221_T0_T1_DEGC_H2    0x35

#define HTS221_H0_T0_OUT        0x36
#define HTS221_H1_T0_OUT        0x3A

#define HTS221_T0_OUT           0x3C
#define HTS221_T1_OUT           0x3E

/**
 * Configuration parameters for BMP280 module.
 * Use function bmp280_init_default_params to use default configuration.
 */
typedef struct {
    uint8_t internal_avg_temp;
    uint8_t internal_avg_humid;
    uint8_t data_rate;
    uint8_t BDU_flag;
    uint8_t internal_heater;
    uint8_t DRDY_pin;
    uint8_t DRDY_mode;
    uint8_t DRDY_active;
} hts221_params_t;

/**
 * Device descriptor
 */
typedef struct {
    float T_degC_slope;
    float T_degC_offset;
    float T_offset;

    float H_rH_slope;
    float H_rH_offset;
    float H_offset;

    i2c_dev_t i2c_dev;  //!< I2C device descriptor
    uint8_t   id;       //!< Chip ID
} hts221_t;

/**
 * Initialize default parameters.
 * Default configuration:
 */
esp_err_t hts221_init_default_params(hts221_params_t *params);

/**
 * Initialize HTS221 module, probes for the device, soft resets the device,
 * reads the calibration constants, and configures the device using the supplied
 * parameters. Returns `ESP_OK` on success.
 *
 * This may be called again to soft reset the device and initialize it again.
 */
esp_err_t hts221_init(hts221_t *dev, hts221_params_t *params);

/**
 * Read compensated temperature and humidity data:
 *  Temperature in degrees Celsius.
 *  Humidity in %RH.
 */
esp_err_t hts221_read_float(hts221_t *dev, float *temperature, float *humidity);

#ifdef __cplusplus
}
#endif

#endif  // __HTS221_H__