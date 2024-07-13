#include "Pulse_Measure.h"
volatile int T1 = 0;
volatile int T2 = 0;
volatile int delta = 0;
volatile uint8_t first_rising_captured = 0;
#define BUFFER_MAX_SIZE 1000
volatile uint32_t pulse_durations[BUFFER_MAX_SIZE] = {0};
volatile int count = 0;

void measure_pulses(TIM_HandleTypeDef* htim){
	count = 0;
	HAL_TIM_IC_Start_IT(htim,TIM_CHANNEL_1);
	HAL_Delay(1000); //take measurements for 1s
	HAL_TIM_IC_Stop_IT(htim,TIM_CHANNEL_1);
	HAL_Delay(1);
}


float calculate_mean(){
	float v = 0.0f;
	for(int i=0;i<count;i++){
		v += (float)(pulse_durations[i]);
	}
	v = v/((float)count);
	return v;
}

float calculate_std(float mean){
	float v = 0.0f;
	for(int i=0;i<count;i++){
		float t= (float)(pulse_durations[i]);
		v += t*t;
	}
	v = v/((float)count);
	return v - mean*mean;
}
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim){
	if(count >= BUFFER_MAX_SIZE){// if we have filled the buffer do nothing!
		return;
	}
	if(htim->Channel != HAL_TIM_ACTIVE_CHANNEL_1){//not relevant to us
		return;
	}
	if(first_rising_captured==0){ //this ithe first instance that this is captured
		T1 = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
		first_rising_captured = 1;
		return;
	}
	T2 = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);
	if(T2 > T1){
		delta = (T2-T1);
	}
	else{
		delta = (65535-T1)+T2;
	}
	pulse_durations[count] = delta;
	count++;
	__HAL_TIM_SET_COUNTER(htim,0);
	first_rising_captured = 0;
}
void complete_pulse_measure(){
	measure_pulses(&htim2);
	float mean = calculate_mean();
	CDC_Transmit_FS((uint8_t*)(&mean),sizeof(float));
}