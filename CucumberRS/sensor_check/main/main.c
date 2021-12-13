/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "i2cdev.h"
#include "bmp280.h"
#include "hts221.h"

i2c_dev_t dev;

void bmp280_task(void *pvParamters) {
    bmp280_params_t params;
    bmp280_t bmp280_dev;

    bmp280_init_default_params(&params);
    memset(&bmp280_dev, 0, sizeof(bmp280_t));
    memcpy(&(bmp280_dev.i2c_dev), &dev, sizeof(i2c_dev_t));
    bmp280_dev.i2c_dev.addr = BMP280_I2C_ADDRESS_0;
    bmp280_init(&bmp280_dev, &params);

    while(1) {
        float temperature, pressure, humidity;
        bmp280_dev.i2c_dev.addr = BMP280_I2C_ADDRESS_0;
        if (bmp280_read_float(&bmp280_dev, &temperature, &pressure, &humidity) != ESP_OK) {
            printf("Temperature/pressure reading failed\n");
            continue;
        }
        printf("Pressure: %.2f Pa, Temperature: %.2f C\n", pressure, temperature);        
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }    
}

void hts221_task(void *pvParamters) {
    hts221_params_t params;
    hts221_t hts221_dev;

    hts221_init_default_params(&params);
    memset(&hts221_dev, 0, sizeof(hts221_t));
    memcpy(&(hts221_dev.i2c_dev), &dev, sizeof(i2c_dev_t));
    hts221_dev.i2c_dev.addr = HTS221_I2C_ADDRESS;
    if (hts221_init(&hts221_dev, &params) != ESP_OK) {
        printf("Error at initialization\n");
    }

    while(1) {
        float temperature, humidity;
        hts221_dev.i2c_dev.addr = HTS221_I2C_ADDRESS;
        if (hts221_read_float(&hts221_dev, &temperature, &humidity) != ESP_OK) {
            printf("Temperature/pressure reading failed\n");
            continue;
        }
        printf("Humidity: %.2f %%RH, Temperature: %.2f C\n", humidity, temperature);        
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }    
}

void mpu6050_task(void *pvParamters) {
    const uint8_t MPU6050_ADDR = 0x68;
    const uint8_t MPU6050_WHOAMI_ADDR = 0x75;
    const uint8_t MPU6050_WHOAMI_VAL = 0x68;
    uint8_t buf;

    while(1) {
        dev.addr = MPU6050_ADDR;
        I2C_DEV_TAKE_MUTEX(&dev);
        i2c_dev_read_reg(&dev, MPU6050_WHOAMI_ADDR, &buf, 1);
        if (buf == MPU6050_WHOAMI_VAL) {
            printf("Found MPU6050\n");
        } else {
            printf("MPU6050: got %2X\n", buf);
        }
        I2C_DEV_GIVE_MUTEX(&dev);
        vTaskDelay(2000 / portTICK_PERIOD_MS);
    }    
}

void app_main(void) {

    printf("Starting I2C\n");
    i2cdev_init();
    dev.port = 0;
    dev.cfg.sda_io_num = 41;
    dev.cfg.scl_io_num = 40;
    dev.cfg.master.clk_speed = 400000;
    i2c_dev_create_mutex(&dev);

    xTaskCreate(hts221_task, "HTS221 task", configMINIMAL_STACK_SIZE*8, NULL, 6, NULL); 
    vTaskDelay(10 / portTICK_PERIOD_MS);
    xTaskCreate(bmp280_task, "BMP280 task", configMINIMAL_STACK_SIZE*8, NULL, 5, NULL); 
    vTaskDelay(10 / portTICK_PERIOD_MS);
    xTaskCreate(mpu6050_task, "MPU6050 task", configMINIMAL_STACK_SIZE*8, NULL, 7, NULL); 
}
