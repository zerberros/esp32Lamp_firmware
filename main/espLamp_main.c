/* esp_Lamp

   This code is in the GPLv2 

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

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
#include "esp_spi_flash.h"


static xQueueHandle gpio_evt_queue = NULL;

// USER LIBS
#include "usr_leds.h"
#include "usr_ADXL343_I2C.h"

#define ESP_INTR_FLAG_DEFAULT 0

// #define INFO        
#define STACK_SIZE    4*1024
// StackType_t xStack[ STACK_SIZE ];

#define GPIO_INPUT_INT_BUTTON (0)





/*
*    TASKS
*/ 



static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}




/*
*    END TASKS
*/


void app_main(void){
    
    #ifdef INFO

    /*
    * Print chip information 
    */
    
    printf("Hola app_main\n");
    
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    printf("This is %s chip with %d CPU cores, WiFi%s%s, ",
            CONFIG_IDF_TARGET,
            chip_info.cores,
            (chip_info.features & CHIP_FEATURE_BT) ? "/BT" : "",
            (chip_info.features & CHIP_FEATURE_BLE) ? "/BLE" : "");

    printf("silicon revision %d, ", chip_info.revision);

    printf("%dMB %s flash\n", spi_flash_get_chip_size() / (1024 * 1024),
            (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    printf("Free heap: %d\n", esp_get_free_heap_size());

    
    /*
    * END Print chip information 
    */

   #endif
    
         
    
    
    /*
    *  INICIALIZACIÓN I2C
    */
    
    i2c_driver_install(i2c_port, I2C_MODE_MASTER,
        I2C_MASTER_RX_BUF_DISABLE,I2C_MASTER_TX_BUF_DISABLE, 0);
    i2c_master_driver_initialize();
    //i2c_isr_free();

    /*
    *  FIN DE INICIALIZACION I2C
    */
    
    
    
    /*	
    *  Configuración de GPIOS de INTERRUPCIONES del acelerómetro
    */
  
    gpio_config_t io_conf;
    io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
    io_conf.intr_type = GPIO_INTR_POSEDGE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_down_en = 1;
    gpio_config(&io_conf);

    /*
    *  FIN DE CONFIGURACIÓN DE GPIOS DE INTERRUPCIONES DEL ACELERÓMETRO
    */ 


    //create a queue to handle gpio event from isr
    gpio_evt_queue = xQueueCreate(1, sizeof(uint32_t));
    //start gpio task


    //install gpio isr service
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(GPIO_INPUT_INT_1, gpio_isr_handler, (void*) GPIO_INPUT_INT_1);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(GPIO_INPUT_INT_2, gpio_isr_handler, (void*) GPIO_INPUT_INT_2);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(GPIO_INPUT_INT_BUTTON, gpio_isr_handler, (void*) GPIO_INPUT_INT_BUTTON);

    /*
    *   Congifuracion Timers para los leds
    */


    ledc_timer_config(&ledc_timer_0);
    ledc_timer_config(&ledc_timer_1);
    ledc_timer_config(&ledc_timer_2);

    /*
    *    Fin de configuración de timers para leds
    */

    /*
    *  Fader para leds
    */

    // Initialize fade service.
    ledc_fade_func_install(0);


    /*
    *  FIN INSTALL Fader para leds
    */

    /*
    *    TASKS
    */ 


    int T=1;   // SELECT TASK TO EXECUTE

    //  vTaskStartScheduler();
    
    if (T==1){
        xTaskCreate(getAccelerometer, "getAcc", 4*1024, NULL, 1, NULL );
    }
    if (T==1){
        xTaskCreate(accelerometer_interrupt, "accelerometer_interrupt", 2048, NULL, 10, NULL);
    }
    if (T==0){
        xTaskCreate(demoColors_01,"Change Colors 01",4*1024, NULL, 5, &xDemo01Handle);
        xTaskCreate(demoColors_02,"Change Colors 02",4*1024, NULL, 5, &xDemo02Handle);
        vTaskSuspend( xDemo02Handle );
    }

    if (T==1){
        xTaskCreate(demoColors_03,"Change Colors 03",4*1024, NULL, 5, &xDemo03Handle);
    }

// return 0;

}
