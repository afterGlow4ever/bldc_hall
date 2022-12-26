//===============================================
//
//	File: pwm.c
//	Author: afterGlow,4ever
//	Date: 06252022
//	Version: v1.0
//
// 	This file is related to pwm output. 
//
//===============================================

#include "bldc.h"

extern bldc_hall_status_t BLDC0;

//===============================================
// pwm gpio init
//===============================================

static void bldc0_pwm_gpio_init(void) 
{
  BLDC0_OCPWM1_GPIO_CLK_ENABLE();
  BLDC0_OCNPWM1_GPIO_CLK_ENABLE();
  BLDC0_OCPWM2_GPIO_CLK_ENABLE();
  BLDC0_OCNPWM2_GPIO_CLK_ENABLE();
  BLDC0_OCPWM3_GPIO_CLK_ENABLE();
  BLDC0_OCNPWM3_GPIO_CLK_ENABLE();
														   
  BLDC0.GPIO_PWM_InitStructure.Pull = GPIO_NOPULL;
  BLDC0.GPIO_PWM_InitStructure.Speed = GPIO_SPEED_HIGH;
  BLDC0.GPIO_PWM_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; 

	// u-
  BLDC0.GPIO_PWM_InitStructure.Pin = BLDC0_OCNPWM1_PIN;
  HAL_GPIO_Init(BLDC0_OCNPWM1_GPIO_PORT, &BLDC0.GPIO_PWM_InitStructure);	

	// v-
  BLDC0.GPIO_PWM_InitStructure.Pin = BLDC0_OCNPWM2_PIN;	
  HAL_GPIO_Init(BLDC0_OCNPWM2_GPIO_PORT, &BLDC0.GPIO_PWM_InitStructure);

	// w-
  BLDC0.GPIO_PWM_InitStructure.Pin = BLDC0_OCNPWM3_PIN;	
  HAL_GPIO_Init(BLDC0_OCNPWM3_GPIO_PORT, &BLDC0.GPIO_PWM_InitStructure);	

  BLDC0.GPIO_PWM_InitStructure.Mode = GPIO_MODE_AF_PP;  
	
	// u+
  BLDC0.GPIO_PWM_InitStructure.Pin = BLDC0_OCPWM1_PIN;
  BLDC0.GPIO_PWM_InitStructure.Alternate = BLDC0_OCPWM1_AF;	
  HAL_GPIO_Init(BLDC0_OCPWM1_GPIO_PORT, &BLDC0.GPIO_PWM_InitStructure);
  
	// v+
  BLDC0.GPIO_PWM_InitStructure.Pin = BLDC0_OCPWM2_PIN;	
  BLDC0.GPIO_PWM_InitStructure.Alternate = BLDC0_OCPWM2_AF;	
  HAL_GPIO_Init(BLDC0_OCPWM2_GPIO_PORT, &BLDC0.GPIO_PWM_InitStructure);

	// w+
  BLDC0.GPIO_PWM_InitStructure.Pin = BLDC0_OCPWM3_PIN;	
  BLDC0.GPIO_PWM_InitStructure.Alternate =BLDC0_OCPWM3_AF;	
  HAL_GPIO_Init(BLDC0_OCPWM3_GPIO_PORT, &BLDC0.GPIO_PWM_InitStructure);
}

//===============================================
// pwm timer init
// system clock 400MHz
// advanced timer clock 200MHz
// period 10Hz 100ms
//===============================================

static void bldc0_pwm_tim_init(void)
{
  BLDC0_PWM_TIM_CLK_ENABLE(); 
  BLDC0.TIM_PWM_InitStructure.Instance = BLDC0_PWM_TIM;
  BLDC0.TIM_PWM_InitStructure.Init.Period =  BLDC0_PWM_PERIOD_COUNT - 1;
  BLDC0.TIM_PWM_InitStructure.Init.Prescaler = BLDC0_PWM_PRESCALER_COUNT - 1;	
  BLDC0.TIM_PWM_InitStructure.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  BLDC0.TIM_PWM_InitStructure.Init.CounterMode = TIM_COUNTERMODE_UP;
  BLDC0.TIM_PWM_InitStructure.Init.RepetitionCounter = 0;	
  HAL_TIM_PWM_Init(&BLDC0.TIM_PWM_InitStructure);

  BLDC0.TIM_PWM_OCInitStructure.OCMode = TIM_OCMODE_PWM1;
  BLDC0.TIM_PWM_OCInitStructure.Pulse = 0;
  BLDC0.TIM_PWM_OCInitStructure.OCPolarity = TIM_OCPOLARITY_HIGH;
  BLDC0.TIM_PWM_OCInitStructure.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  BLDC0.TIM_PWM_OCInitStructure.OCIdleState = TIM_OCIDLESTATE_SET;
  BLDC0.TIM_PWM_OCInitStructure.OCNIdleState = TIM_OCNIDLESTATE_RESET;

  HAL_TIM_PWM_ConfigChannel(&BLDC0.TIM_PWM_InitStructure, &BLDC0.TIM_PWM_OCInitStructure, BLDC0_PWM_CHANNEL1);  
  HAL_TIM_PWM_ConfigChannel(&BLDC0.TIM_PWM_InitStructure, &BLDC0.TIM_PWM_OCInitStructure, BLDC0_PWM_CHANNEL2); 
  HAL_TIM_PWM_ConfigChannel(&BLDC0.TIM_PWM_InitStructure, &BLDC0.TIM_PWM_OCInitStructure, BLDC0_PWM_CHANNEL3); 
  
	//software mode instead of trigger mode
  HAL_TIMEx_ConfigCommutationEvent(&BLDC0.TIM_PWM_InitStructure, TIM_COM_TS_ITRx, TIM_COMMUTATION_SOFTWARE);
}

//===============================================
// pwm init
//===============================================

void bldc0_pwm_init(void)
{
	bldc0_pwm_gpio_init();
	bldc0_pwm_tim_init();
}

//===============================================
// switch for pwm
//===============================================

void bldc0_pwm_disable(void)
{
  HAL_TIM_PWM_Stop(&BLDC0.TIM_PWM_InitStructure, BLDC0_PWM_CHANNEL1);
  HAL_TIM_PWM_Stop(&BLDC0.TIM_PWM_InitStructure, BLDC0_PWM_CHANNEL2);
  HAL_TIM_PWM_Stop(&BLDC0.TIM_PWM_InitStructure, BLDC0_PWM_CHANNEL3);	
  __HAL_TIM_SET_COMPARE(&BLDC0.TIM_PWM_InitStructure, BLDC0_PWM_CHANNEL1, 0);
  __HAL_TIM_SET_COMPARE(&BLDC0.TIM_PWM_InitStructure, BLDC0_PWM_CHANNEL2, 0);
  __HAL_TIM_SET_COMPARE(&BLDC0.TIM_PWM_InitStructure, BLDC0_PWM_CHANNEL3, 0);
  HAL_GPIO_WritePin(BLDC0_OCNPWM1_GPIO_PORT, BLDC0_OCNPWM1_PIN, GPIO_PIN_RESET);   
  HAL_GPIO_WritePin(BLDC0_OCNPWM2_GPIO_PORT, BLDC0_OCNPWM2_PIN, GPIO_PIN_RESET);   
  HAL_GPIO_WritePin(BLDC0_OCNPWM3_GPIO_PORT, BLDC0_OCNPWM3_PIN, GPIO_PIN_RESET);   
}

void bldc0_pwm_enable(void)
{
	// u+
  HAL_TIM_PWM_Start(&BLDC0.TIM_PWM_InitStructure, BLDC0_PWM_CHANNEL1);
	// v+
  HAL_TIM_PWM_Start(&BLDC0.TIM_PWM_InitStructure, BLDC0_PWM_CHANNEL2);
	// w+
  HAL_TIM_PWM_Start(&BLDC0.TIM_PWM_InitStructure, BLDC0_PWM_CHANNEL3);	
}

//===============================================
// set pwm dutycycle
// implement in commutation
//===============================================

void bldc0_pwm_set(float speed)
{
	BLDC0.speed.speed_set_dutycycle = speed * BLDC0_PWM_PERIOD_COUNT / BLDC_MAX_SPEED ;
}

void bldc0_dir_set(bldc_dir_e dir)
{
	BLDC0.direction.dir_set = dir;
}


