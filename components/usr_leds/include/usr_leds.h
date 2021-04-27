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


struct luminosityes{
    int16_t R;
    int16_t G;
    int16_t B;
    int16_t T;
};


extern int16_t threshold_high;
extern int16_t threshold_low;

extern struct luminosityes luminosity;
extern struct luminosityes luminosity_bk;

extern ledc_timer_config_t ledc_timer_0;
extern ledc_timer_config_t ledc_timer_1;
extern ledc_timer_config_t ledc_timer_2;

// Manejadores de tareas
    TaskHandle_t xDemo01Handle;
    TaskHandle_t xDemo02Handle;
    TaskHandle_t xDemo03Handle;


void set_led_RGB( void );
void demoColors_01(void *pvParameters);
void demoColors_02(void *pvParameters);
void demoColors_03(void *pvParameters);

#endif
