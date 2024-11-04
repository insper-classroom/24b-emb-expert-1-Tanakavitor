#include "mpu6050.h"
#include <stdint.h>
#include <stdio.h>

#define MPU_ADDRESS 0x68 

// Função para configurar o struct de configuração do MPU6050
void mpu6050_set_config(imu_c *config, i2c_inst_t *i2c, int pin_sda, int pin_scl, int acc_scale) {
    if (config) {
        config->i2c = i2c;
        config->pin_sda = pin_sda;
        config->pin_scl = pin_scl;
        config->acc_scale = acc_scale;
    }
}

// Inicializa o MPU6050
int mpu6050_init(imu_c config) {
    if (!config.i2c) {
        return 0;
    }
    
    i2c_init(config.i2c, 100 * 1000);
    gpio_set_function(config.pin_sda, GPIO_FUNC_I2C);
    gpio_set_function(config.pin_scl, GPIO_FUNC_I2C);
    gpio_pull_up(config.pin_sda);
    gpio_pull_up(config.pin_scl);
    uint8_t buf[] = {0x6B, 0x00};
    if (i2c_write_blocking(config.i2c, MPU_ADDRESS, buf, 2, false) != 2) {
        return 0;
    }

    return 1;
}

// Reinicia o MPU6050
int mpu6050_reset(imu_c config) {
    uint8_t buf[] = {0x6B, 0x00};
    if (i2c_write_blocking(config.i2c, MPU_ADDRESS, buf, 2, false) != 2) {
        return 0;
    }
    return 1;
}

// Lê os dados do acelerômetro
int mpu6050_read_acc(imu_c config, int16_t accel[3]) {
    uint8_t buffer[6];
    uint8_t val = 0x3B;
    if (i2c_write_blocking(config.i2c, MPU_ADDRESS, &val, 1, true) != 1) {
        return 0;
    }
    if (i2c_read_blocking(config.i2c, MPU_ADDRESS, buffer, 6, false) != 6) {
        return 0;
    }

    for (int i = 0; i < 3; i++) {
        accel[i] = (buffer[i * 2] << 8) | buffer[(i * 2) + 1];
    }
    return 1;
}

// Lê os dados do giroscópio
int mpu6050_read_gyro(imu_c config, int16_t gyro[3]) {
    uint8_t buffer[6];
    uint8_t val = 0x43;
    if (i2c_write_blocking(config.i2c, MPU_ADDRESS, &val, 1, true) != 1) {
        return 0;
    }
    if (i2c_read_blocking(config.i2c, MPU_ADDRESS, buffer, 6, false) != 6) {
        return 0;
    }

    for (int i = 0; i < 3; i++) {
        gyro[i] = (buffer[i * 2] << 8) | buffer[(i * 2) + 1];
    }
    return 1;
}

// Lê a temperatura
int mpu6050_read_temp(imu_c config, int16_t *temp) {
    uint8_t buffer[2];
    uint8_t reg = 0x41;
    if (i2c_write_blocking(config.i2c, MPU_ADDRESS, &reg, 1, true) != 1) {
        return 0;
    }
    if (i2c_read_blocking(config.i2c, MPU_ADDRESS, buffer, 2, false) != 2) {
        return 0;
    }

    *temp = (buffer[0] << 8) | buffer[1];
    return 1;
}

// Configura a detecção de movimento
int mpu6050_set_motion_detection(imu_c config, int enable) {
    uint8_t reg = 0x1E;
    uint8_t val = enable ? 0x01 : 0x00;
    if (i2c_write_blocking(config.i2c, MPU_ADDRESS, &reg, 1, true) != 1) {
        return 0;
    }
    if (i2c_write_blocking(config.i2c, MPU_ADDRESS, &val, 1, false) != 1) {
        return 0;
    }
    return 1;
}

// Obtém o status da interrupção de detecção de movimento
int mpu6050_get_motion_interrupt_status(imu_c config) {
    uint8_t reg = 0x3A;
    uint8_t status;
    if (i2c_write_blocking(config.i2c, MPU_ADDRESS, &reg, 1, true) != 1) {
        return 0;
    }
    if (i2c_read_blocking(config.i2c, MPU_ADDRESS, &status, 1, false) != 1) {
        return 0;
    }
    return status & 0x01;
}

// Configura o limiar de detecção de movimento
int mpu6050_set_motion_detection_threshold(imu_c config, uint8_t thr) {
    uint8_t reg = 0x1F;
    if (i2c_write_blocking(config.i2c, MPU_ADDRESS, &reg, 1, true) != 1) {
        return 0;
    }
    if (i2c_write_blocking(config.i2c, MPU_ADDRESS, &thr, 1, false) != 1) {
        return 0;
    }
    return 1;
}

// Configura a duração da detecção de movimento
int mpu6050_set_motion_detection_duration(imu_c config, uint8_t dur) {
    uint8_t reg = 0x20;
    if (i2c_write_blocking(config.i2c, MPU_ADDRESS, &reg, 1, true) != 1) {
        return 0;
    }
    if (i2c_write_blocking(config.i2c, MPU_ADDRESS, &dur, 1, false) != 1) {
        return 0;
    }
    return 1;
}