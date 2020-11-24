


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
