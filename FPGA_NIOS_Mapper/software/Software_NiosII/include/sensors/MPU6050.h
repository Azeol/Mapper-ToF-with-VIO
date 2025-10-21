/**
 * @file    MPU6050.h
 * @author  Azeol
 * @brief   MPU6050 driver (register map, init, config, read APIs)
 * This driver is mostly based on the MPU6050 driver by ElectronicCats on github
 * @version 0.1
 * @date    2025-10-18
 */

#ifndef MPU6050_H_
#define MPU6050_H_

#include <stdint.h>
#include "alt_types.h"
#include "altera_avalon_i2c.h"

/* Default I2C address when AD0 = GND is 0x68, when AD0 = VCC is 0x69 */

#ifndef MPU6050_I2C_ADDR_DEFAULT
#define MPU6050_I2C_ADDR_DEFAULT 0x68u
#endif

/* Registers (partial, enough for basic operation) */

#define MPU6050_REG_SMPLRT_DIV     0x19u
#define MPU6050_REG_CONFIG         0x1Au
#define MPU6050_REG_GYRO_CONFIG    0x1Bu
#define MPU6050_REG_ACCEL_CONFIG   0x1Cu
#define MPU6050_REG_INT_STATUS     0x3Au
#define MPU6050_REG_ACCEL_XOUT_H   0x3Bu /* 3B..40 accel xyz */
#define MPU6050_REG_TEMP_OUT_H     0x41u /* 41..42 */
#define MPU6050_REG_GYRO_XOUT_H    0x43u /* 43..48 gyro xyz */
#define MPU6050_REG_PWR_MGMT_1     0x6Bu
#define MPU6050_REG_WHO_AM_I       0x75u

/* Bit fields */

#define MPU6050_PWR1_DEVICE_RESET  (1u<<7)
#define MPU6050_PWR1_SLEEP         (1u<<6)
#define MPU6050_PWR1_CYCLE         (1u<<5)
/* CLKSEL bits [2:0] in PWR_MGMT_1 */

/* CONFIG (0x1A) DLPF_CFG [2:0] */
typedef enum {
	MPU6050_DLPF_260HZ = 0, /* 260Hz accel/gyro bandwidth, 1kHz sample */
	MPU6050_DLPF_184HZ = 1,
	MPU6050_DLPF_94HZ  = 2,
	MPU6050_DLPF_44HZ  = 3,
	MPU6050_DLPF_21HZ  = 4,
	MPU6050_DLPF_10HZ  = 5,
	MPU6050_DLPF_5HZ   = 6
} mpu6050_dlpf_t;

/* GYRO_CONFIG FS_SEL [4:3] */
typedef enum {
	MPU6050_GYRO_250DPS  = 0,
	MPU6050_GYRO_500DPS  = 1,
	MPU6050_GYRO_1000DPS = 2,
	MPU6050_GYRO_2000DPS = 3
} mpu6050_gyro_range_t;

/* ACCEL_CONFIG AFS_SEL [4:3] */
typedef enum {
	MPU6050_ACCEL_2G  = 0,
	MPU6050_ACCEL_4G  = 1,
	MPU6050_ACCEL_8G  = 2,
	MPU6050_ACCEL_16G = 3
} mpu6050_accel_range_t;

typedef struct {
	ALT_AVALON_I2C_DEV_t* i2c; /* I2C controller */
	alt_u8 addr;              /* Device I2C address */

	/* cached scale factors derived from ranges */
	float accel_lsb_per_g;     /* e.g., 16384 for 2G */
	float gyro_lsb_per_dps;    /* e.g., 131 for 250 dps */
} mpu6050_t;

// Global variables

static mpu6050_t g_imu = {
	I2C_BASE,
	MPU6050_I2C_ADDR_DEFAULT,
	16384.0f,
	131.0f
}; // Global IMU Variable
static alt_u8 g_who; // Global WHO_AM_I Variable

/* Initialization: set clock, wake, DLPF, sample rate, default ranges */

int mpu6050_init(mpu6050_t* imu, ALT_AVALON_I2C_DEV_t* i2c, alt_u8 addr);

/* Identification */

int mpu6050_who_am_i(mpu6050_t* imu, alt_u8* who);

/* Configuration */

int mpu6050_set_sleep(mpu6050_t* imu, int enable);
int mpu6050_set_clock_source(mpu6050_t* imu, alt_u8 clksel);
int mpu6050_set_sample_rate_div(mpu6050_t* imu, alt_u8 div);
int mpu6050_set_dlpf(mpu6050_t* imu, mpu6050_dlpf_t dlpf);
int mpu6050_set_accel_range(mpu6050_t* imu, mpu6050_accel_range_t range);
int mpu6050_set_gyro_range(mpu6050_t* imu, mpu6050_gyro_range_t range);

/* Reading */

int mpu6050_read_raw(mpu6050_t* imu,
					 int16_t* ax, int16_t* ay, int16_t* az,
					 int16_t* gx, int16_t* gy, int16_t* gz,
					 int16_t* temp_raw);

int mpu6050_read_conv(mpu6050_t* imu,
					  float* ax_g, float* ay_g, float* az_g,
					  float* gx_dps, float* gy_dps, float* gz_dps,
					  float* temp_c);

/*
Quick usage:
	ALT_AVALON_I2C_DEV_t* i2c = i2c_init(I2C_NAME, MPU6050_I2C_ADDR_DEFAULT);
	mpu6050_t imu; 
    mpu6050_init(&imu, i2c, MPU6050_I2C_ADDR_DEFAULT);
	alt_u8 who; 
    mpu6050_who_am_i(&imu, &who); // expect 0x68
	float ax, ay, az, gx, gy, gz, tc;
	mpu6050_read_conv(&imu, &ax, &ay, &az, &gx, &gy, &gz, &tc);
*/

#endif /* MPU6050_H_ */
