/**
 * @file    MPU6050.c
 * @author  Azeol
 * @brief   MPU6050 driver implementation for Nios II using Altera I2C HAL
 * This driver is mostly based on the MPU6050 driver by ElectronicCats on github
 * @version 0.1
 * @date    2025-10-18
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "alt_types.h"
#include "altera_avalon_i2c.h"

#include "drivers/i2c.h"          /* wrapper APIs available in repo */
#include "sensors/MPU6050.h"

/* --- Low-level helpers -------------------------------------------------- */

static ALT_AVALON_I2C_STATUS_CODE reg_write(mpu6050_t* imu, alt_u8 reg, alt_u8 val)
{
	alt_u8 buf[2] = { reg, val };
	return i2c_write(imu->i2c, buf, 2);
}

static ALT_AVALON_I2C_STATUS_CODE reg_read(mpu6050_t* imu, alt_u8 reg, alt_u8* val)
{
	alt_u8 r = reg; alt_u8 v = 0;
	ALT_AVALON_I2C_STATUS_CODE st = i2c_write_read(imu->i2c, &r, 1, &v, 1);
	if (st == ALT_AVALON_I2C_SUCCESS) *val = v;
	return st;
}

static ALT_AVALON_I2C_STATUS_CODE burst_read(mpu6050_t* imu, alt_u8 start_reg, alt_u8* rx, size_t len)
{
	return i2c_write_read(imu->i2c, &start_reg, 1, rx, len);
}

/* --- Scale helpers ------------------------------------------------------ */

static void update_scales(mpu6050_t* imu, mpu6050_accel_range_t arange, mpu6050_gyro_range_t grange)
{
	switch (arange) {
		case MPU6050_ACCEL_2G:  imu->accel_lsb_per_g = 16384.0f; break;
		case MPU6050_ACCEL_4G:  imu->accel_lsb_per_g = 8192.0f;  break;
		case MPU6050_ACCEL_8G:  imu->accel_lsb_per_g = 4096.0f;  break;
		case MPU6050_ACCEL_16G: imu->accel_lsb_per_g = 2048.0f;  break;
		default:                imu->accel_lsb_per_g = 16384.0f; break;
	}
	switch (grange) {
		case MPU6050_GYRO_250DPS:  imu->gyro_lsb_per_dps = 131.0f;  break;
		case MPU6050_GYRO_500DPS:  imu->gyro_lsb_per_dps = 65.5f;   break;
		case MPU6050_GYRO_1000DPS: imu->gyro_lsb_per_dps = 32.8f;   break;
		case MPU6050_GYRO_2000DPS: imu->gyro_lsb_per_dps = 16.4f;   break;
		default:                   imu->gyro_lsb_per_dps = 131.0f;  break;
	}
}

/* --- Public API --------------------------------------------------------- */

/**
 * @brief Initialize the MPU6050 sensor
 * 
 * @param imu  Pointer to the MPU6050 structure
 * @param i2c  Pointer to the I2C device structure
 * @param addr Address of the MPU6050 (0 or default 0x68)
 * @return int 
 */
int mpu6050_init(mpu6050_t* imu, ALT_AVALON_I2C_DEV_t* i2c, alt_u8 addr)
{
	if (!imu || !i2c) return -1;
	memset(imu, 0, sizeof(*imu));
	imu->i2c = i2c;
	imu->addr = addr ? addr : MPU6050_I2C_ADDR_DEFAULT;

	/* The repo's i2c_init already set target; enforce the address in case */
	alt_avalon_i2c_master_target_set(i2c, imu->addr);

	/* Wake device: clear SLEEP, set CLKSEL to PLL with X axis gyro (1) */
	alt_u8 pwr1 = 0;
	if (reg_read(imu, MPU6050_REG_PWR_MGMT_1, &pwr1) != ALT_AVALON_I2C_SUCCESS) return -2;
	pwr1 &= ~(MPU6050_PWR1_SLEEP);
	pwr1 = (pwr1 & ~0x07u) | 0x01u; /* CLKSEL=1 */
	if (reg_write(imu, MPU6050_REG_PWR_MGMT_1, pwr1) != ALT_AVALON_I2C_SUCCESS) return -3;

	/* DLPF default */
	if (mpu6050_set_dlpf(imu, MPU6050_DLPF_94HZ) != 0) return -4;
	/* Sample rate: 1kHz / (1+div). With DLPF on, internal is 1kHz. Choose 100Hz => div=9 */
	if (mpu6050_set_sample_rate_div(imu, 9) != 0) return -5;
	/* Ranges defaults */
	if (mpu6050_set_accel_range(imu, MPU6050_ACCEL_2G) != 0) return -6;
	if (mpu6050_set_gyro_range(imu, MPU6050_GYRO_250DPS) != 0) return -7;

	return 0;
}

/**
 * @brief Get the WHO_AM_I register value
 * 
 * @param imu Pointer to the MPU6050 structure
 * @param who Pointer to the variable to store the WHO_AM_I value
 * @return int 
 */
int mpu6050_who_am_i(mpu6050_t* imu, alt_u8* who)
{
	if (!imu || !who) return -1;
	alt_u8 v = 0;
	alt_avalon_i2c_master_target_set(imu->i2c, imu->addr);
	if (reg_read(imu, MPU6050_REG_WHO_AM_I, &v) != ALT_AVALON_I2C_SUCCESS) return -2;
	*who = v;
	return 0;
}

/**
 * @brief Set the sleep mode of the MPU6050
 * 
 * @param imu  Pointer to the MPU6050 structure
 * @param enable Variable to enable (non-zero) or disable (zero) sleep mode
 * @return int 
 */
int mpu6050_set_sleep(mpu6050_t* imu, int enable)
{
	if (!imu) return -1;
	alt_u8 v = 0;
	if (reg_read(imu, MPU6050_REG_PWR_MGMT_1, &v) != ALT_AVALON_I2C_SUCCESS) return -2;
	if (enable) v |= MPU6050_PWR1_SLEEP; else v &= ~MPU6050_PWR1_SLEEP;
	if (reg_write(imu, MPU6050_REG_PWR_MGMT_1, v) != ALT_AVALON_I2C_SUCCESS) return -3;
	return 0;
}

/**
 * @brief Set the clock source of the MPU6050
 * 
 * @param imu Pointer to the MPU6050 structure
 * @param clksel Clock source selection (0-7)
 * @return int 
 */
int mpu6050_set_clock_source(mpu6050_t* imu, alt_u8 clksel)
{
	if (!imu) return -1;
	alt_u8 v = 0;
	if (reg_read(imu, MPU6050_REG_PWR_MGMT_1, &v) != ALT_AVALON_I2C_SUCCESS) return -2;
	v = (v & ~0x07u) | (clksel & 0x07u);
	if (reg_write(imu, MPU6050_REG_PWR_MGMT_1, v) != ALT_AVALON_I2C_SUCCESS) return -3;
	return 0;
}

/**
 * @brief Set the sample rate divider of the MPU6050
 * 
 * @param imu Pointer to the MPU6050 structure
 * @param div Sample rate divider
 * @return int 
 */
int mpu6050_set_sample_rate_div(mpu6050_t* imu, alt_u8 div)
{
	if (!imu) return -1;
	if (reg_write(imu, MPU6050_REG_SMPLRT_DIV, div) != ALT_AVALON_I2C_SUCCESS) return -2;
	return 0;
}

/**
 * @brief Set the digital low-pass filter (DLPF) of the MPU6050
 * 
 * @param imu  Pointer to the MPU6050 structure
 * @param dlpf Digital low-pass filter setting
 * @return int 
 */
int mpu6050_set_dlpf(mpu6050_t* imu, mpu6050_dlpf_t dlpf)
{
	if (!imu) return -1;
	alt_u8 v = 0;
	if (reg_read(imu, MPU6050_REG_CONFIG, &v) != ALT_AVALON_I2C_SUCCESS) return -2;
	v = (v & ~0x07u) | ((alt_u8)dlpf & 0x07u);
	if (reg_write(imu, MPU6050_REG_CONFIG, v) != ALT_AVALON_I2C_SUCCESS) return -3;
	return 0;
}

int mpu6050_set_accel_range(mpu6050_t* imu, mpu6050_accel_range_t range)
{
	if (!imu) return -1;
	alt_u8 v = 0;
	if (reg_read(imu, MPU6050_REG_ACCEL_CONFIG, &v) != ALT_AVALON_I2C_SUCCESS) return -2;
	v = (v & ~(0x3u<<3)) | (((alt_u8)range & 0x3u) << 3);
	if (reg_write(imu, MPU6050_REG_ACCEL_CONFIG, v) != ALT_AVALON_I2C_SUCCESS) return -3;
	update_scales(imu, range, (imu->gyro_lsb_per_dps>100.0f)?MPU6050_GYRO_250DPS:MPU6050_GYRO_2000DPS /* kept */);
	return 0;
}

int mpu6050_set_gyro_range(mpu6050_t* imu, mpu6050_gyro_range_t range)
{
	if (!imu) return -1;
	alt_u8 v = 0;
	if (reg_read(imu, MPU6050_REG_GYRO_CONFIG, &v) != ALT_AVALON_I2C_SUCCESS) return -2;
	v = (v & ~(0x3u<<3)) | (((alt_u8)range & 0x3u) << 3);
	if (reg_write(imu, MPU6050_REG_GYRO_CONFIG, v) != ALT_AVALON_I2C_SUCCESS) return -3;
	update_scales(imu, (imu->accel_lsb_per_g>10000.0f)?MPU6050_ACCEL_2G:MPU6050_ACCEL_16G /* kept */, range);
	return 0;
}

/**
 * @brief Read the raw sensor data from the MPU6050
 * 
 * @param imu Pointer to the MPU6050 structure
 * @param ax  Accelerometer X output
 * @param ay  Accelerometer Y output
 * @param az  Accelerometer Z output
 * @param gx  Gyroscope X output
 * @param gy  Gyroscope Y output
 * @param gz  Gyroscope Z output
 * @param temp_raw Raw temperature output
 * @return int 
 */
int mpu6050_read_raw(mpu6050_t* imu,
					 alt_16* ax, alt_16* ay, alt_16* az,
					 alt_16* gx, alt_16* gy, alt_16* gz,
					 alt_16* temp_raw)
{
	if (!imu) return -1;
	alt_u8 buf[14];
	ALT_AVALON_I2C_STATUS_CODE st = burst_read(imu, MPU6050_REG_ACCEL_XOUT_H, buf, sizeof(buf));
	if (st != ALT_AVALON_I2C_SUCCESS) return -2;

	if (ax) *ax = (alt_16)((buf[0]<<8) | buf[1]);
	if (ay) *ay = (alt_16)((buf[2]<<8) | buf[3]);
	if (az) *az = (alt_16)((buf[4]<<8) | buf[5]);
	if (temp_raw) *temp_raw = (alt_16)((buf[6]<<8) | buf[7]);
	if (gx) *gx = (alt_16)((buf[8]<<8) | buf[9]);
	if (gy) *gy = (alt_16)((buf[10]<<8) | buf[11]);
	if (gz) *gz = (alt_16)((buf[12]<<8) | buf[13]);
	return 0;
}

/**
 * @brief Read the raw sensor data from the MPU6050 and convert to physical units
 * 
 * @param imu Pointer to the MPU6050 structure
 * @param ax_g Accelerometer X output in g
 * @param ay_g Accelerometer Y output in g
 * @param az_g Accelerometer Z output in g
 * @param gx_dps Gyroscope X output in dps
 * @param gy_dps Gyroscope Y output in dps
 * @param gz_dps Gyroscope Z output in dps
 * @param temp_c Temperature X output in Celsius
 * @return int 
 */
int mpu6050_read_conv(mpu6050_t* imu,
					  alt_16* ax_g, alt_16* ay_g, alt_16* az_g,
					  alt_16* gx_dps, alt_16* gy_dps, alt_16* gz_dps,
					  alt_16* temp_c)
{
	if (!imu) return -1;
	alt_16 ax, ay, az, gx, gy, gz, traw;
	int rc = mpu6050_read_raw(imu, &ax, &ay, &az, &gx, &gy, &gz, &traw);
	if (rc != 0) return rc;

	if (ax_g) *ax_g = ((alt_16)ax) / (imu->accel_lsb_per_g);
	if (ay_g) *ay_g = ((alt_16)ay) / (imu->accel_lsb_per_g);
	if (az_g) *az_g = ((alt_16)az) / (imu->accel_lsb_per_g);
	if (gx_dps) *gx_dps = ((alt_16)gx) / (imu->gyro_lsb_per_dps);
	if (gy_dps) *gy_dps = ((alt_16)gy) / (imu->gyro_lsb_per_dps);
	if (gz_dps) *gz_dps = ((alt_16)gz) / (imu->gyro_lsb_per_dps);
	if (temp_c) *temp_c = ((alt_16)traw) / 340.0f + 36.53f; /* per datasheet */
	return 0;
}

