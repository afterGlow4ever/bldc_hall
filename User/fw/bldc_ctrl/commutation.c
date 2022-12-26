//===============================================
//
//	File: commutation.c
//	Author: afterGlow,4ever
//	Date: 06252022
//	Version: v1.0
//
// 	This file is used to commutation according to
//  the commutation true table.
//
//===============================================

#include "bldc.h"

extern bldc_hall_status_t BLDC0;

//===============================================
// BLDC commutation function ptr
//===============================================

commutation_funptr bldc0_commutation_funptr[6] =
{
    &bldc0_commutation_uhwl, 
    &bldc0_commutation_vhul, 
    &bldc0_commutation_vhwl,
    &bldc0_commutation_whvl, 
    &bldc0_commutation_uhvl, 
    &bldc0_commutation_whul
};

//===============================================
// BLDC commutation function
// H PWM L ON
// Forward/Anticlockwise: 154623
// Backward/Clockwise: 645132
//===============================================

// U+ W-
// Forward/Anticlockwise: 1
// Backward/Clockwise: 6
void bldc0_commutation_uhwl(void)
{
	__HAL_TIM_SET_COMPARE(&BLDC0.TIM_PWM_InitStructure, BLDC0_PWM_CHANNEL1, BLDC0.speed.speed_set_dutycycle);	
	__HAL_TIM_SET_COMPARE(&BLDC0.TIM_PWM_InitStructure, BLDC0_PWM_CHANNEL2, 0);		
	__HAL_TIM_SET_COMPARE(&BLDC0.TIM_PWM_InitStructure, BLDC0_PWM_CHANNEL3, 0); 
	HAL_GPIO_WritePin(BLDC0_OCNPWM1_GPIO_PORT, BLDC0_OCNPWM1_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(BLDC0_OCNPWM2_GPIO_PORT, BLDC0_OCNPWM2_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(BLDC0_OCNPWM3_GPIO_PORT, BLDC0_OCNPWM3_PIN, GPIO_PIN_SET);
}

// V+ U-
// Forward/Anticlockwise: 2
// Backward/Clockwise: 5
void bldc0_commutation_vhul(void)
{
	__HAL_TIM_SET_COMPARE(&BLDC0.TIM_PWM_InitStructure, BLDC0_PWM_CHANNEL1, 0);	
	__HAL_TIM_SET_COMPARE(&BLDC0.TIM_PWM_InitStructure, BLDC0_PWM_CHANNEL2, BLDC0.speed.speed_set_dutycycle);		
	__HAL_TIM_SET_COMPARE(&BLDC0.TIM_PWM_InitStructure, BLDC0_PWM_CHANNEL3, 0); 
	HAL_GPIO_WritePin(BLDC0_OCNPWM1_GPIO_PORT, BLDC0_OCNPWM1_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(BLDC0_OCNPWM2_GPIO_PORT, BLDC0_OCNPWM2_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(BLDC0_OCNPWM3_GPIO_PORT, BLDC0_OCNPWM3_PIN, GPIO_PIN_RESET);
}

// V+ W-
// Forward/Anticlockwise: 3
// Backward/Clockwise: 4
void bldc0_commutation_vhwl(void)
{
	__HAL_TIM_SET_COMPARE(&BLDC0.TIM_PWM_InitStructure, BLDC0_PWM_CHANNEL1, 0);	
	__HAL_TIM_SET_COMPARE(&BLDC0.TIM_PWM_InitStructure, BLDC0_PWM_CHANNEL2, BLDC0.speed.speed_set_dutycycle);		
	__HAL_TIM_SET_COMPARE(&BLDC0.TIM_PWM_InitStructure, BLDC0_PWM_CHANNEL3, 0); 
	HAL_GPIO_WritePin(BLDC0_OCNPWM1_GPIO_PORT, BLDC0_OCNPWM1_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(BLDC0_OCNPWM2_GPIO_PORT, BLDC0_OCNPWM2_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(BLDC0_OCNPWM3_GPIO_PORT, BLDC0_OCNPWM3_PIN, GPIO_PIN_SET);
}

// W+ V-
// Forward/Anticlockwise: 4
// Backward/Clockwise: 3
void bldc0_commutation_whvl(void)
{
	__HAL_TIM_SET_COMPARE(&BLDC0.TIM_PWM_InitStructure, BLDC0_PWM_CHANNEL1, 0);	
	__HAL_TIM_SET_COMPARE(&BLDC0.TIM_PWM_InitStructure, BLDC0_PWM_CHANNEL2, 0);		
	__HAL_TIM_SET_COMPARE(&BLDC0.TIM_PWM_InitStructure, BLDC0_PWM_CHANNEL3, BLDC0.speed.speed_set_dutycycle); 
	HAL_GPIO_WritePin(BLDC0_OCNPWM1_GPIO_PORT, BLDC0_OCNPWM1_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(BLDC0_OCNPWM2_GPIO_PORT, BLDC0_OCNPWM2_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(BLDC0_OCNPWM3_GPIO_PORT, BLDC0_OCNPWM3_PIN, GPIO_PIN_RESET);
}

// U+ V-
// Forward/Anticlockwise: 5
// Backward/Clockwise: 2
void bldc0_commutation_uhvl(void)
{
	__HAL_TIM_SET_COMPARE(&BLDC0.TIM_PWM_InitStructure, BLDC0_PWM_CHANNEL1, BLDC0.speed.speed_set_dutycycle);	
	__HAL_TIM_SET_COMPARE(&BLDC0.TIM_PWM_InitStructure, BLDC0_PWM_CHANNEL2, 0);		
	__HAL_TIM_SET_COMPARE(&BLDC0.TIM_PWM_InitStructure, BLDC0_PWM_CHANNEL3, 0); 
	HAL_GPIO_WritePin(BLDC0_OCNPWM1_GPIO_PORT, BLDC0_OCNPWM1_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(BLDC0_OCNPWM2_GPIO_PORT, BLDC0_OCNPWM2_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(BLDC0_OCNPWM3_GPIO_PORT, BLDC0_OCNPWM3_PIN, GPIO_PIN_RESET);
}

// W+ U-
// Forward/Anticlockwise: 6
// Backward/Clockwise: 1
void bldc0_commutation_whul(void)
{
	__HAL_TIM_SET_COMPARE(&BLDC0.TIM_PWM_InitStructure, BLDC0_PWM_CHANNEL1, 0);	
	__HAL_TIM_SET_COMPARE(&BLDC0.TIM_PWM_InitStructure, BLDC0_PWM_CHANNEL2, 0);		
	__HAL_TIM_SET_COMPARE(&BLDC0.TIM_PWM_InitStructure, BLDC0_PWM_CHANNEL3, BLDC0.speed.speed_set_dutycycle); 
	HAL_GPIO_WritePin(BLDC0_OCNPWM1_GPIO_PORT, BLDC0_OCNPWM1_PIN, GPIO_PIN_SET);
	HAL_GPIO_WritePin(BLDC0_OCNPWM2_GPIO_PORT, BLDC0_OCNPWM2_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(BLDC0_OCNPWM3_GPIO_PORT, BLDC0_OCNPWM3_PIN, GPIO_PIN_RESET);
}

//===============================================
// commutation from hall sensor
// Forward/Anticlockwise: 154623
// Backward/Clockwise: 645132
//===============================================

void bldc0_hall_commutation(uint8_t hall_step)
{
	if(BLDC0.direction.dir_set == MOTOR_FWD)
	{
		bldc0_commutation_funptr[hall_step - 1]();
	}
	else
	{
		bldc0_commutation_funptr[(7 - hall_step) - 1]();
	}	
//		BLDC0.operation_status = MOTOR_DRIVING;		
}

void bldc0_hall_commutation_failure(void)
{
	if(BLDC0.stall_timeout++ > BLDC_STALL_THRESHOLD)// stall when not receive a valid hall value in a hall period
	{
		bldc0_system_disable();
		BLDC0.error_status = MOTOR_STALL;
	}	
}

