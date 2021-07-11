//#include <stdio.h>
//#include "usr_ASXL343_I2C.h"

//void func(void)
//{

// }


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


#include <stdio.h>
#include <stdlib.h>
#include "esp_log.h"
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
//#include "esp_spi_flash.h"

#include "usr_leds.h"
#include "usr_ASXL343_I2C.h"
static const char* TAG_ASCL343 = "usr_ASCL343";

uint8_t state_machine_position = 0;
xQueueHandle gpio_evt_queue = NULL;

/*
*    FUNCIONES DE INICIALIZACIÓN I2C
*/ 

esp_err_t i2c_master_driver_initialize(void)
{
    i2c_config_t conf_i2c = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = i2c_gpio_sda,
        .sda_pullup_en = GPIO_PULLUP_DISABLE,
        .scl_io_num = i2c_gpio_scl,
        .scl_pullup_en = GPIO_PULLUP_DISABLE,
        .master.clk_speed = i2c_frequency
    };
    return i2c_param_config(i2c_port, &conf_i2c);
}

/*
*   FIN DE FUNCIONES DE INICIALIZACIÓN I2C
*/ 

void accelerometer_interrupt(void* arg)
{
    uint32_t io_num;
    for(;;) {
        if(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
            ESP_LOGD(TAG_ASCL343, "GPIO[%d] intr, val: %d\n", io_num, gpio_get_level(io_num));
            // printf("GPIO[%d] intr, val: %d\n", io_num, gpio_get_level(io_num));
            if(io_num ==0 && gpio_get_level(io_num)==1){

                state_machine_position +=1;
                if (state_machine_position == 6){
                    state_machine_position = 0;
                }

switch(state_machine_position){
int med;
                case 0:
                    // Demo 1
                    luminosity_bk.R  = luminosity.R;
                    luminosity_bk.G  = luminosity.G;
                    luminosity_bk.B  = luminosity.B;
                    luminosity_bk.T = (luminosity.R + luminosity.G + luminosity.B)/3;
                    luminosity.T = luminosity_bk.T;
                    vTaskResume( xDemo03Handle );
                    ESP_LOGD(TAG_ASCL343, "Demo colors\n");
                    // printf("Demo colors");
                    break;
                case 1:
                    //vTaskSuspend( xDemo01Handle );
                    vTaskSuspend( xDemo03Handle );
                    
                    // Whitew colors
                    // All PWM leds to luminosity value

                    // med = (luminosity.R + luminosity.G + luminosity.B)/3;
                    luminosity.R = luminosity_bk.T;
                    luminosity.G = luminosity_bk.T;
                    luminosity.B = luminosity_bk.T;
                    set_led_RGB();
                    ESP_LOGD(TAG_ASCL343, "White colors\n");
                    // printf("White colors");
                    break;
                case 2:
                    // Red color

                    luminosity_bk.R  = luminosity.R;
                    luminosity_bk.G  = luminosity.G;
                    luminosity_bk.B  = luminosity.B;
                    luminosity_bk.T = (luminosity.R + luminosity.G + luminosity.B)/3;
                    luminosity.G = 0;
                    luminosity.B = 0;
                    set_led_RGB();
                    ESP_LOGD(TAG_ASCL343, "Red colors\n");
                    // printf("RED color");
                    break;
                case 3:
                    // Green color
                    luminosity_bk.R = luminosity.R;
                    luminosity.R = 0;
                    luminosity.G = luminosity_bk.G;
                    luminosity.B = 0;
                    set_led_RGB();
                    ESP_LOGD(TAG_ASCL343, "Green colors\n");
                    // printf("Green color");
                    break;
                case 4:
                    // Blue color
                    luminosity_bk.G = luminosity.G;
                    luminosity.R = 0;
                    luminosity.G = 0;
                    luminosity.B = luminosity_bk.B;
                    set_led_RGB();
                    ESP_LOGD(TAG_ASCL343, "Blue colors\n");
                    // printf("Blue color");
                    break;
                case 5:
                    // RGB color
                    luminosity_bk.B = luminosity.B;
                    luminosity.R = luminosity_bk.R;
                    luminosity.G = luminosity_bk.G;
                    luminosity.B = luminosity_bk.B;
                                       
                    set_led_RGB();
                    ESP_LOGD(TAG_ASCL343, "RGB color\n");
                    // printf("Blue color");
                    break;
            default:
                    // default statements
                    ESP_LOGE(TAG_ASCL343, "Estate machine error case.\n");
                    // printf("fak");
                    //break;
            
            }  // end switch case
        

            ESP_LOGD(TAG_ASCL343, "State machine position == %d \n", state_machine_position);
            // printf("State machine position == %d \n", state_machine_position);
            } // end if
        }  // end if
    vTaskDelay(100 / portTICK_PERIOD_MS );
    } // end infinite bucle
} // end task function



void write_ADXL343(uint8_t address, uint8_t register1, uint8_t data){
   // printf("read function");
   
           /*
	 * COMUNICACION I2C
	 *   SDA-> GPIO21
	 *   SCL-> GPIO19
	 */


    // uint8_t DATAX1 = 0x33;
    int ret;
    

	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	//vTaskDelay(5 / portTICK_RATE_MS);
    i2c_master_write_byte(cmd, (address<<1) | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, register1, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, data, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    
	i2c_master_cmd_begin(i2c_port, cmd, 100 / portTICK_RATE_MS);
	i2c_cmd_link_delete(cmd);
	

	/*
	 * FIN DE COMUNICACIÓN I2C
	 */

}


uint8_t read_ADXL343(uint8_t address, uint8_t register1, uint8_t len){
   // printf("read function");
   
           /*
	 * COMUNICACION I2C
	 *   SDA-> GPIO21
	 *   SCL-> GPIO19
	 */

    int ret;
    uint8_t data[len];

	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
	//vTaskDelay(5 / portTICK_RATE_MS);
    i2c_master_write_byte(cmd, (address<<1) | WRITE_BIT, ACK_CHECK_EN);
    i2c_master_write_byte(cmd, register1, ACK_CHECK_EN);
    i2c_master_stop(cmd);
    
	i2c_master_cmd_begin(i2c_port, cmd, 100 / portTICK_RATE_MS);
	i2c_cmd_link_delete(cmd);

	cmd = i2c_cmd_link_create();
	i2c_master_start(cmd);
 	i2c_master_write_byte(cmd, (address<<1) | READ_BIT, ACK_CHECK_EN);

    if (len > 1) {
        i2c_master_read(cmd, data, len - 1, ACK_VAL);
    }
    i2c_master_read_byte(cmd, data + len - 1, NACK_VAL);
    
    
 
    //vTaskDelay(5 / portTICK_RATE_MS);
    i2c_master_stop(cmd);

	ret = i2c_master_cmd_begin(i2c_port, cmd, 100 / portTICK_RATE_MS);
    i2c_cmd_link_delete(cmd);
	
	// Se espera 0xE5
	
	// printf("write_add = 0x%02x \n", (dev_address<<1) | WRITE_BIT);
	// printf("read_add = 0x%02x \n", (dev_address<<1) | READ_BIT);
	if(ret == ESP_ERR_TIMEOUT){
        ESP_LOGE(TAG_ASCL343, "BUS IS BUSY \n");
	    // printf("BUS IS BUSY \n");
	    }
	// printf("ret: %i \n", ret);
	
	for (int i = 0; i < len; i++) {
        ESP_LOGD(TAG_ASCL343, "0x%02x \n;", data[i]);
        // printf("0x%02x ;", data[i]);

        ESP_LOGD(TAG_ASCL343, "int: %i \n", data[i]);
        // printf("int: %i \n", data[i]);
        if ((i + 1) % 6 == 0) {
            ESP_LOGD(" ", "\r\n");
            // printf("\r\n");
        }  //endif
    }  // endfor
    
    if (len==6){

        acc2.FX0 = data[0];
        acc2.FX1 = data[1];
        acc2.FY0 = data[2];
        acc2.FY1 = data[3];
        acc2.FZ0 = data[4];
        acc2.FZ1 = data[5];

        acc.FX = data[1];
        acc.FX <<=8;
        acc.FX |= data[0]; 
        acc.FY = data[3];
        acc.FY <<=8;
        acc.FY |= data[2];
        acc.FZ = data[5];
        acc.FZ <<=8;
        acc.FZ |= data[4];
    }
    //    printf("0x%02x ", dato);
    //    printf("int: %i ", dato);
	
	// free(data);
	/*
	 * FIN DE COMUNICACIÓN I2C
	 */
    return data;
}


// Accelerometer Task

void getAccelerometer(void *pvParameters)
{
    // int i = 0;
    // uint8_t WIAM = 0;
    // int DATA_LENGTH = 1;
    read_ADXL343(dev_address, who_i_am_reg, 1);
    // printf("Resp acc: 0x%02x \n", response);
    write_ADXL343(dev_address, power_ctl_reg, 0x08);
    for( ; ; ){

    read_ADXL343(dev_address, DATAX0, 6);
    
    // printf("X1 :: X0 -> 0x%02X :: 0x%02X\n", acc2.FX1, acc2.FX0);
    // printf("Y1 :: Y0 -> 0x%02X :: 0x%02X\n", acc2.FY1, acc2.FY0);
    // printf("Z1 :: Z0 -> 0x%02X :: 0x%02X\n", acc2.FZ1, acc2.FZ0);
    ESP_LOGD(TAG_ASCL343, "DATAX:  0x%02x OR %i; \n", acc.FX, acc.FX);
    // printf("DATAX:  0x%02x OR %i; \n", acc.FX, acc.FX);
    //printf("DATAX1:  0x%02x OR %d; \n", acc.X1, acc.X1);
    ESP_LOGD(TAG_ASCL343, "DATAY:  0x%02x OR %i; \n", acc.FY, acc.FY);
    // printf("DATAY:  0x%02x OR %i; \n", acc.FY, acc.FY);
    //printf("DATAY1:  0x%02x OR %d; \n", acc.Y1, acc.Y1);
    ESP_LOGD(TAG_ASCL343, "DATAZ:  0x%02x OR %i; \n\n", acc.FZ, acc.FZ);
    // printf("DATAZ:  0x%02x OR %i; \n\n", acc.FZ, acc.FZ);
    //printf("DATAZ1:  0x%02x OR %d; \n", acc.Z1, acc.Z1);
    
    if (acc.FY < threshold_low){

        switch(state_machine_position){         

        case 0:
            luminosity.T = (luminosity.T << 1 | 0x01);
            luminosity.T = luminosity.T & 0x3ff;
            break;

        case 1:
            luminosity.T = (luminosity.T << 1 | 0x01);
            luminosity.T = luminosity.T & 0x3ff;
            luminosity.R = luminosity.T;
            luminosity.G = luminosity.T;
            luminosity.B = luminosity.T;
            set_led_RGB();
            break;

        case 2:
            luminosity.R = (luminosity.R << 1 | 0x01);
            luminosity.R = luminosity.R & 0x3ff;
        //luminosity.G = luminosity.R;
        //luminosity.B = luminosity.R;
            set_led_RGB();
            break;
        case 3:
            luminosity.G = (luminosity.G << 1 | 0x01);
            luminosity.G = luminosity.G & 0x3ff;
            set_led_RGB();
            break;
        case 4:
            luminosity.B = (luminosity.B << 1 | 0x01);
            luminosity.B = luminosity.B & 0x3ff;
            set_led_RGB();
            break;
        case 5:
            luminosity.R = (luminosity.R*2);
            //luminosity.R = luminosity.R & 0x3ff;
            if (luminosity.R > 0x3ff){
                luminosity.R = 0x2ff;
            }
            
            luminosity.G = (luminosity.G*2);
            // luminosity.G = luminosity.G & 0x3ff;
            if (luminosity.G > 0x3ff){
                luminosity.G = 0x3ff;
            }

            luminosity.B = (luminosity.B*2);
            //luminosity.B = luminosity.B & 0x3ff;
            if (luminosity.B > 0x3ff){
                luminosity.B = 0x3ff;
            }
            
            set_led_RGB();
            break;
        }; // end switch case
    // printf("threshold_high; luminosity = 0x%02X; \n ", luminosity.R);

    }

    if (acc.FY > threshold_high){

        switch(state_machine_position){  
        case 0:
            luminosity.T = (luminosity.T >> 1 );
            break;
        case 1:
            luminosity.T = (luminosity.T >> 1 );
            luminosity.R  = luminosity.T;
            luminosity.G  = luminosity.T;  
            luminosity.B  = luminosity.T; 
            set_led_RGB();
            break;
        case 2:
            luminosity.R = (luminosity.R >> 1 );
            set_led_RGB();
            break;
        case 3:
            luminosity.G = (luminosity.G >> 1 );
            set_led_RGB();
            break;
        case 4:
            luminosity.B = (luminosity.B >> 1 );
            set_led_RGB();
            break;
        case 5:
            luminosity.R = (luminosity.R/2 );
            luminosity.G = (luminosity.G/2 );
            luminosity.B = (luminosity.B/2 );
            set_led_RGB();
            break;
        }

    }

    // vTaskDelay(5 / portTICK_RATE_MS);
    // response = read_ADXL343(dev_address, DATAX0, 6);

                /*  BLOQUEADO: CÓDIGO  POR ELIMINAR
                    // RUTINA DE SLEEP 
            
            	esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);
            	esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_SLOW_MEM, ESP_PD_OPTION_ON);
            	esp_sleep_pd_config(ESP_PD_DOMAIN_XTAL, ESP_PD_OPTION_ON);
            	esp_sleep_pd_config(ESP_PD_DOMAIN_MAX, ESP_PD_OPTION_ON);
            	int t = 10000000;
            	esp_sleep_enable_timer_wakeup(t);
                    
            	i = i + 1;
                    printf("getAccelerometer task %d.\n", i);
            	
            	if(i%10 == 0){
            	printf("OK\n");
                    // esp_light_sleep_start();
                    // esp_deep_sleep(t);
            	} //endif
            
                */  // FIN DE RUTINA DE SLEEP

    vTaskDelay( 1000 / portTICK_PERIOD_MS );
    } //end infiniteloop

    vTaskDelete(NULL);
} //end task



