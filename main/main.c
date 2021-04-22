#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/spi_master.h"
#include "soc/gpio_struct.h"
#include "driver/gpio.h"
#include "driver/uart.h"
#include "soc/uart_struct.h"
#include <math.h>

#include "esp_dsp.h"
#include "Arduino.h"



// This example shows how to use FFT from esp-dsp library

#define N_SAMPLES 8192
int N = N_SAMPLES;
// Input test array
// Window coefficients
// working complex array
float y_cf[N_SAMPLES * 2];
// Pointers to result arrays
float vReal[N_SAMPLES];     //数据实部

unsigned long time_now = 0;    //时间变量

void app_main()
{
	//esp_err_t ret;
	//ret = dsps_fft2r_init_fc32(NULL, CONFIG_DSP_MAX_FFT_SIZE);
	// Generate hann window
	double cycles = (((1024 - 1) * 1001) / 80000);     //周期(从0计算，所以要减1)
	for(uint16_t i = 0 ; i < 1024 ; i++)
	{
		vReal[i] = INT8_C((100 * (sin((i * (3.14159265 * 2 * cycles)) / 1024.0))) / 2.0);
		//printf("%f\n", vReal[i]);
	}

	for (int i = 0; i < N; i++)
	{
		y_cf[i * 2 + 0] = vReal[i];   //数据实部
		y_cf[i * 2 + 1] = 0;   //数据虚部
	}
	
	// FFT
	
	time_now=millis();
	printf("%ld\n", millis());
	dsps_fft2r_fc32(y_cf, N);
	// Bit reverse
	dsps_bit_rev_fc32(y_cf, N);
	// Convert one complex vector to two complex vectors
	dsps_cplx2reC_fc32(y_cf, N);
	
	

	//for (int i = 0; i < N / 2; i++) {
	//	y1_cf[i] = sqrt((y_cf[i * 2 + 0] * y_cf[i * 2 + 0] + y_cf[i * 2 + 1] * y_cf[i * 2 + 1])) / N;
	//	printf("%f,%d\n", y1_cf[i],i);
	//}
	//printf("%ld\n", xxxx);
	printf("%ld\n", millis()-time_now);
	
	
	//dsps_view(y1_cf, N / 2, 128, 20, 0, 5, '|');
}
