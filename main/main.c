#include "hardware/gpio.h"
#include "hardware/i2c.h"
#include "mpu6050.h"
#include "pico/stdlib.h"
#include "stdio.h"

int main() {
    // Inicializa stdio para USB serial
    stdio_init_all();

    const uint SDA_PIN = 4;
    const uint SCL_PIN = 5;

    // Configura MPU6050
    imu_c imu_config;
    i2c_inst_t *I2C_PORT = i2c0;  // Removido 'const'
    mpu6050_set_config(&imu_config, I2C_PORT, SDA_PIN, SCL_PIN, 2); // acc_scale = ±2G

    // Debug: Tentando inicializar MPU6050
    printf("Attempting MPU6050 initialization...\n");

    if (!mpu6050_init(imu_config)) {
        printf("Falha ao inicializar MPU6050\n");
        printf("erro\n");
        return 1;
    }

    printf("MPU6050 initialized successfully\n");  // Mensagem de sucesso
    while (1) {
        int16_t accel[3];
        int16_t gyro[3];
        int16_t temp;
        // Ler dados do MPU6050
        if (mpu6050_read_acc(imu_config, accel) &&
            mpu6050_read_gyro(imu_config, gyro) &&
            mpu6050_read_temp(imu_config, &temp)) {
            
            printf("Accel: X=%d Y=%d Z=%d | Gyro: X=%d Y=%d Z=%d | Temp: %d\n", 
                   accel[0], accel[1], accel[2],
                   gyro[0], gyro[1], gyro[2],
                   temp);
        } else {
            printf("Erro na leitura dos dados do MPU6050\n");
        }

        sleep_ms(110);
    }

    return 0;
}
