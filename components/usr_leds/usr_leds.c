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


#include "usr_leds.h"


/*
*    TASKS
*/ 

// Leds TASKS
void set_led_RGB( void ){
        // RED LEDs
        int ch;
        for (ch = 0; ch < 3; ch++) {
            ledc_set_duty(ledc_channel[ch].speed_mode,   ledc_channel[ch].channel, luminosity.R);
            ledc_update_duty(ledc_channel[ch].speed_mode, ledc_channel[ch].channel);        
        } // end for

        // GREEN LEDs
        for (ch = 3; ch < 6; ch++) {
            ledc_set_duty(ledc_channel[ch].speed_mode,   ledc_channel[ch].channel, luminosity.G);
            ledc_update_duty(ledc_channel[ch].speed_mode, ledc_channel[ch].channel);        
        } // end for

        // BLUE LEDs
        for (ch = 6; ch < 9; ch++) {
            ledc_set_duty(ledc_channel[ch].speed_mode,   ledc_channel[ch].channel, luminosity.B);
            ledc_update_duty(ledc_channel[ch].speed_mode, ledc_channel[ch].channel);        
        } // end for
}


void demoColors_01(void *pvParameters)
{

    vTaskDelay( 10 / portTICK_PERIOD_MS );
    int ch;
	uint32_t j = 0;
     
    for (ch = 0; ch < LEDC_TEST_CH_NUM; ch++) {
        ledc_channel_config(&ledc_channel[ch]);
    }

    void led_update(int j)
    {
        // All PWM leds to 0
        for (ch = 0; ch < LEDC_TEST_CH_NUM; ch++) {
            ledc_set_duty(ledc_channel[ch].speed_mode,   ledc_channel[ch].channel, 0);
            ledc_update_duty(ledc_channel[ch].speed_mode, ledc_channel[ch].channel);        
        } // end for

        // Turn on one led a time
        ledc_set_duty(ledc_channel[j].speed_mode,   ledc_channel[j].channel, 16);    // RGB
        ledc_set_duty(ledc_channel[(j+3)%9].speed_mode,   ledc_channel[(j+3)%9].channel, 16);  // GBR ( + RGB  = YCM)  
        ledc_update_duty(ledc_channel[j].speed_mode, ledc_channel[j].channel);  
        
    } // END led_update()


    for( ; ; )
    {
        led_update(j);
        j++;
        if (j == LEDC_TEST_CH_NUM)
            
            {
            j=0;
            vTaskResume( xDemo02Handle );  // 
            vTaskSuspend( xDemo01Handle );
            // xTaskCreate(demoColors_02,"Change Colors 02",4*1024, NULL, 5, NULL);
            // vTaskDelete(NULL);
            }


        vTaskDelay( 100 / portTICK_PERIOD_MS );
    } //end infinite loop
    vTaskDelete(NULL);
}


void demoColors_02(void *pvParameters)
{   

    vTaskDelay( 10 / portTICK_PERIOD_MS );
    int ch;
	uint32_t j = 0;
     
    for (ch = 0; ch < LEDC_TEST_CH_NUM; ch++) {
        ledc_channel_config(&ledc_channel[ch]);
    }

    void led_update(int j)
    {
        // All PWM leds to 0
        for (ch = 0; ch < LEDC_TEST_CH_NUM; ch++) {
            ledc_set_duty(ledc_channel[ch].speed_mode,   ledc_channel[ch].channel, 0);
            ledc_update_duty(ledc_channel[ch].speed_mode, ledc_channel[ch].channel);        
        } // end for

        // Turn on one led a time
        ledc_set_duty(ledc_channel[j].speed_mode,   ledc_channel[j].channel, luminosity.R);    // RGB
        // ledc_set_duty(ledc_channel[(j+3)%9].speed_mode,   ledc_channel[(j+3)%9].channel, 16);  // GBR ( + RGB  = YCM)  
        ledc_update_duty(ledc_channel[j].speed_mode, ledc_channel[j].channel);  
        
    } // END led_update()


    for( ; ; )
    {
        led_update(j);
        j++;
        if (j == LEDC_TEST_CH_NUM)
            {
            j=0;
            vTaskResume( xDemo01Handle );
            vTaskSuspend( xDemo02Handle );
            // xTaskCreate(demoColors_01,"Change Colors 01",4*1024, NULL, 5, NULL);
            // vTaskDelete(NULL);
            }
        vTaskDelay( 100 / portTICK_PERIOD_MS );
    } //end infinite loop
    vTaskDelete(NULL);
}


void demoColors_03(void *pvParameters)
{   

    vTaskDelay( 10 / portTICK_PERIOD_MS );
    int ch;
	uint32_t j = 0;
     
    for (ch = 0; ch < LEDC_TEST_CH_NUM; ch++) {
        ledc_channel_config(&ledc_channel[ch]);
    }
    
       // All PWM leds to 0
    for (ch = 0; ch < LEDC_TEST_CH_NUM; ch++) {
        ledc_set_duty(ledc_channel[ch].speed_mode,   ledc_channel[ch].channel, 0);
        ledc_update_duty(ledc_channel[ch].speed_mode, ledc_channel[ch].channel);        
    } // end for

    void led_update(int j)
    {
 

        vTaskDelay( 100 / portTICK_PERIOD_MS );

        // Rojo on
        for (ch = 0; ch <3; ch++){
        ledc_set_fade_with_time(ledc_channel[ch].speed_mode, ledc_channel[ch].channel, luminosity.T, 250);
        ledc_fade_start(ledc_channel[ch].speed_mode, ledc_channel[ch].channel, LEDC_FADE_WAIT_DONE);
        }
        
        vTaskDelay( 300 / portTICK_PERIOD_MS );

        // Azul Off
        for (ch = 6; ch <9; ch++){
        ledc_set_fade_with_time(ledc_channel[ch].speed_mode, ledc_channel[ch].channel, 0, 250);
        ledc_fade_start(ledc_channel[ch].speed_mode, ledc_channel[ch].channel, LEDC_FADE_WAIT_DONE);
        }

        vTaskDelay( 300 / portTICK_PERIOD_MS );

        // Verde on
        for (ch = 3; ch <6; ch++){
        ledc_set_fade_with_time(ledc_channel[ch].speed_mode, ledc_channel[ch].channel, luminosity.T, 250);
        ledc_fade_start(ledc_channel[ch].speed_mode, ledc_channel[ch].channel, LEDC_FADE_WAIT_DONE);
        }
        
        vTaskDelay( 300 / portTICK_PERIOD_MS );

        // Rojo Off
        for (ch = 0; ch <3; ch++){
        ledc_set_fade_with_time(ledc_channel[ch].speed_mode, ledc_channel[ch].channel, 0, 250);
        ledc_fade_start(ledc_channel[ch].speed_mode, ledc_channel[ch].channel, LEDC_FADE_WAIT_DONE);
        }

        vTaskDelay( 300 / portTICK_PERIOD_MS );

        // Azul on
        for (ch = 6; ch <9; ch++){
        ledc_set_fade_with_time(ledc_channel[ch].speed_mode, ledc_channel[ch].channel, luminosity.T, 250);
        ledc_fade_start(ledc_channel[ch].speed_mode, ledc_channel[ch].channel, LEDC_FADE_WAIT_DONE);
        }

        vTaskDelay( 300 / portTICK_PERIOD_MS );
        
        // Verde Off
        for (ch = 3; ch <6; ch++){
        ledc_set_fade_with_time(ledc_channel[ch].speed_mode, ledc_channel[ch].channel, 0, 250);
        ledc_fade_start(ledc_channel[ch].speed_mode, ledc_channel[ch].channel, LEDC_FADE_WAIT_DONE);
        }

        vTaskDelay( 300 / portTICK_PERIOD_MS );

        /*
        // Turn on one led a time
        ledc_set_duty(ledc_channel[j].speed_mode,   ledc_channel[j].channel, 16);    // RGB
        // ledc_set_duty(ledc_channel[(j+3)%9].speed_mode,   ledc_channel[(j+3)%9].channel, 16);  // GBR ( + RGB  = YCM)  
        ledc_update_duty(ledc_channel[j].speed_mode, ledc_channel[j].channel);  
        */  
       
    } // END led_update()


    for( ; ; )
    {
        led_update(j);
        j++;
        if (j == LEDC_TEST_CH_NUM)
            {
            j=0;
            //vTaskResume( xDemo01Handle );
            //vTaskSuspend( xDemo02Handle );
            // xTaskCreate(demoColors_01,"Change Colors 01",4*1024, NULL, 5, NULL);
            // vTaskDelete(NULL);
            }
        // vTaskDelay( 100 / portTICK_PERIOD_MS );
    } //end infinite loop
    vTaskDelete(NULL);
}
