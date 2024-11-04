// mpu6050.h

#ifndef MPU6050_H
#define MPU6050_H
#include "hardware/i2c.h"
#include "pico/stdlib.h"
#include <stdint.h>

// Definição da estrutura de configuração do MPU6050
typedef struct {
    i2c_inst_t *i2c;  // Removido 'const'
    int pin_sda;
    int pin_scl;
    int acc_scale;
} imu_c;

// Funções de configuração e operação do MPU6050
void mpu6050_set_config(imu_c *config, i2c_inst_t *i2c, int pin_sda, int pin_scl, int acc_scale);
int mpu6050_init(imu_c config);
int mpu6050_reset(imu_c config);
int mpu6050_read_acc(imu_c config, int16_t accel[3]);
int mpu6050_read_gyro(imu_c config, int16_t gyro[3]);
int mpu6050_read_temp(imu_c config, int16_t *temp);
int mpu6050_set_motion_detection(imu_c config, int enable);
int mpu6050_get_motion_interrupt_status(imu_c config);
int mpu6050_set_motion_detection_threshold(imu_c config, uint8_t thr);
int mpu6050_set_motion_detection_duration(imu_c config, uint8_t dur);

#endif // MPU6050_H