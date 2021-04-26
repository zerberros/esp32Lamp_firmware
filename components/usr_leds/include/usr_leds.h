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

#ifndef __USR_LEDS__
#define __USR_LEDS__

#include <stdio.h>
#include <stdlib.h>
#include "esp_sleep.h"
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/ledc.h"
#include "driver/gpio.h"
#include "driver/i2c.h"
#include "freertos/queue.h"
#include "esp_err.h"
// #include "esp_spi_flash.h"

#define LED_R1       (17)
#define LED_G1       (18)
#define LED_B1       (16)

#define LED_R2       (27)
#define LED_G2       (4)
#define LED_B2       (26)

#define LED_R3       (33)
#define LED_G3       (25)
#define LED_B3       (32)

#define LEDC_LS_TIMER          LEDC_TIMER_1
#define LEDC_LS_MODE           LEDC_LOW_SPEED_MODE
#define LEDC_HS_TIMER          LEDC_TIMER_0
#define LEDC_HS_MODE           LEDC_HIGH_SPEED_MODE
#define LEDC_TEST_CH_NUM       9

int16_t threshold_high = 256;
int16_t threshold_low = -25;

struct luminosityes{
    int16_t R;
    int16_t G;
    int16_t B;
    int16_t T;
};

struct luminosityes luminosity = {.R = 32, \
            .G = 32, \
            .B = 32, \
            .T = 32};

struct luminosityes  luminosity_bk = {.R = 32, \
            .G = 32, \
            .B = 32, \
            .T = 32};

/*
*   CONFIG LEDS
*/ 

// TIMERS
ledc_timer_config_t ledc_timer_0 = {
    .duty_resolution = LEDC_TIMER_10_BIT, // resolution of PWM duty
    .freq_hz = 5000,                      // frequency of PWM signal
    .speed_mode = LEDC_LOW_SPEED_MODE,   // timer mode 
    .timer_num = LEDC_TIMER_0,            // timer index
    .clk_cfg = LEDC_AUTO_CLK,             // Auto select the source clock
};
    
ledc_timer_config_t ledc_timer_1 = {
    .duty_resolution = LEDC_TIMER_10_BIT, // resolution of PWM duty
    .freq_hz = 5000,                      // frequency of PWM signal
    .speed_mode = LEDC_LOW_SPEED_MODE,    // timer mode
    .timer_num = LEDC_TIMER_1,            // timer index
    .clk_cfg = LEDC_AUTO_CLK,              // Auto select the source clock
};

ledc_timer_config_t ledc_timer_2 = {
    .duty_resolution = LEDC_TIMER_10_BIT, // resolution of PWM duty
    .freq_hz = 5000,                      // frequency of PWM signal
    .speed_mode = LEDC_HIGH_SPEED_MODE,    // timer mode
    .timer_num = LEDC_TIMER_2,            // timer index
    .clk_cfg = LEDC_AUTO_CLK,              // Auto select the source clock
};

// LEDS
ledc_channel_config_t ledc_channel[LEDC_TEST_CH_NUM] = {
    {       
        .channel    = LEDC_CHANNEL_0,
        .duty       = 0,
        .gpio_num   = LED_R1,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .hpoint     = 0,
        .timer_sel  = LEDC_TIMER_0
    },
    {
        .channel    = LEDC_CHANNEL_1,
        .duty       = 0,
        .gpio_num   = LED_R2,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .hpoint     = 0,
        .timer_sel  = LEDC_TIMER_0
    },
    {
        .channel    = LEDC_CHANNEL_2,
        .duty       = 0,
        .gpio_num   = LED_R3,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .hpoint     = 0,
        .timer_sel  = LEDC_TIMER_0
    },
    {
        .channel    = LEDC_CHANNEL_3,
        .duty       = 0,
        .gpio_num   = LED_G1,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .hpoint     = 0,
        .timer_sel  = LEDC_TIMER_1
    },
    {
        .channel    = LEDC_CHANNEL_4,
        .duty       = 0,
        .gpio_num   = LED_G2,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .hpoint     = 0,
        .timer_sel  = LEDC_TIMER_1
    },
    {
        .channel    = LEDC_CHANNEL_5,
        .duty       = 0,
        .gpio_num   = LED_G3,
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .hpoint     = 0,
        .timer_sel  = LEDC_TIMER_1
    },
    {
        .channel    = LEDC_CHANNEL_0,
        .duty       = 0,
        .gpio_num   = LED_B1,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .hpoint     = 0,
        .timer_sel  = LEDC_TIMER_2
    },
    {
        .channel    = LEDC_CHANNEL_1,
        .duty       = 0,
        .gpio_num   = LED_B2,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .hpoint     = 0,
        .timer_sel  = LEDC_TIMER_2
    },
    {
        .channel    = LEDC_CHANNEL_2,
        .duty       = 0,
        .gpio_num   = LED_B3,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .hpoint     = 0,
        .timer_sel  = LEDC_TIMER_2
    }
};

/*
*   FIN CONFIG LEDS
*/

// Manejadores de tareas
    TaskHandle_t xDemo01Handle;
    TaskHandle_t xDemo02Handle;
    TaskHandle_t xDemo03Handle;


void set_led_RGB( void );
void demoColors_01(void *pvParameters);
void demoColors_02(void *pvParameters);
void demoColors_03(void *pvParameters);

#endif
