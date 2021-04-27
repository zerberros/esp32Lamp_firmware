 /* Copyright (C) - Jairo Esrefanía <cerverros@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef __USR_ADXL343_I2C__
#define __USR_ADXL343_I2C__


#define GPIO_INPUT_INT_1 (23)
#define GPIO_INPUT_INT_2 (22)

#define GPIO_INPUT_PIN_SEL  ((1ULL<<GPIO_INPUT_INT_1) | (1ULL<<GPIO_INPUT_INT_2) | (1ULL<<GPIO_INPUT_INT_BUTTON))


#define I2C_MASTER_TX_BUF_DISABLE 0 /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE 0 /*!< I2C master doesn't need buffer */
#define WRITE_BIT I2C_MASTER_WRITE  /*!< I2C master write */
#define READ_BIT I2C_MASTER_READ    /*!< I2C master read */
#define ACK_CHECK_EN 0x1            /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS 0x0           /*!< I2C master will not check ack from slave */
#define ACK_VAL 0x0                 /*!< I2C ack value */
#define NACK_VAL 0x1                /*!< I2C nack value */
#define ACK 0x0                     /*!< I2C ack value */
#define NACK 0x1                    /*!< I2C nack value */


static esp_err_t  i2c_gpio_sda  = 21;
static esp_err_t  i2c_gpio_scl  = 19;
static uint32_t   i2c_frequency = 100000;
static i2c_port_t i2c_port      = I2C_NUM_0;

// registros del acelerómetro


static uint8_t    dev_address   = 0x53;
static uint8_t    power_ctl_reg = 0x2D;
static uint8_t    who_i_am_reg  = 0x00;
static uint8_t    DATAX0        = 0x32;
// static uint8_t    DATAX1        = 0x33;
// static uint8_t    DATAY0        = 0x34;
// static uint8_t    DATAY1        = 0x35;
// static uint8_t    DATAZ0        = 0x36;
// static uint8_t    DATAZ1        = 0x37;

typedef struct{
    int16_t  FX;
    int16_t  FY;
    int16_t  FZ;
} fuerza;


typedef struct{
    uint8_t  FX0;
    uint8_t  FX1;
    uint8_t  FY0;
    uint8_t  FY1;
    uint8_t  FZ0;
    uint8_t  FZ1;
} force;

fuerza acc;
force acc2; 

extern uint8_t state_machine_position;
extern xQueueHandle gpio_evt_queue;

// Function prototypes
esp_err_t i2c_master_driver_initialize(void);
void getAccelerometer(void *pvParameters);
void accelerometer_interrupt(void* arg);

#endif
