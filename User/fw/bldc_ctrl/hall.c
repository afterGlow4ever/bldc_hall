//===============================================
//
//	File: hall.c
//	Author: afterGlow,4ever
//	Date: 06252022
//	Version: v1.0
//
// 	This file is related to hall sensor. 
//
//===============================================

#include "bldc.h"

extern bldc_hall_status_t BLDC0;

//===============================================
// hall gpio init
//===============================================

static void bldc0_hall_gpio_init(void)
{
  BLDC0_HALL_U_GPIO_CLK_ENABLE();
  BLDC0_HALL_V_GPIO_CLK_ENABLE();
  BLDC0_HALL_W_GPIO_CLK_ENABLE();
  
	// hall u
  BLDC0.GPIO_HALL_InitStructure.Pin = BLDC0_HALL_U_PIN;
  BLDC0.GPIO_HALL_InitStructure.Mode = GPIO_MODE_AF_PP;
  BLDC0.GPIO_HALL_InitStructure.Pull = GPIO_PULLUP;
  BLDC0.GPIO_HALL_InitStructure.Alternate = BLDC0_HALL_U_AF;
  HAL_GPIO_Init(BLDC0_HALL_U_GPIO_PORT, &BLDC0.GPIO_HALL_InitStructure);
  
	// hall v
  BLDC0.GPIO_HALL_InitStructure.Pin = BLDC0_HALL_V_PIN;
  BLDC0.GPIO_HALL_InitStructure.Alternate = BLDC0_HALL_V_AF;
  HAL_GPIO_Init(BLDC0_HALL_V_GPIO_PORT, &BLDC0.GPIO_HALL_InitStructure);
  
	// hall w
  BLDC0.GPIO_HALL_InitStructure.Pin = BLDC0_HALL_W_PIN;
  BLDC0.GPIO_HALL_InitStructure.Alternate = BLDC0_HALL_W_AF;  
  HAL_GPIO_Init(BLDC0_HALL_W_GPIO_PORT, &BLDC0.GPIO_HALL_InitStructure);
}

//===============================================
// hall timer init
// system clock 400MHz
// advanced timer clock 200MHz
// period 20kHz 50us
//===============================================

static void bldc0_hall_tim_init(void)
{
  BLDC0_HALL_TIM_CLK_ENABLE();
  
  BLDC0.TIM_HALL_InitStructure.Instance = BLDC0_HALL_TIM;
  BLDC0.TIM_HALL_InitStructure.Init.Prescaler = BLDC0_HALL_PRESCALER_COUNT - 1;    
  BLDC0.TIM_HALL_InitStructure.Init.CounterMode = TIM_COUNTERMODE_UP;        
  BLDC0.TIM_HALL_InitStructure.Init.Period = BLDC0_HALL_PERIOD_COUNT - 1;        
  BLDC0.TIM_HALL_InitStructure.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;    
  
  BLDC0.TIM_HALL_Sensor_InitStructure.IC1Prescaler = TIM_ICPSC_DIV1;           
  BLDC0.TIM_HALL_Sensor_InitStructure.IC1Polarity = TIM_ICPOLARITY_BOTHEDGE;    
  BLDC0.TIM_HALL_Sensor_InitStructure.IC1Filter = 10;                           
  BLDC0.TIM_HALL_Sensor_InitStructure.Commutation_Delay = 0U;                 
  HAL_TIMEx_HallSensor_Init(&BLDC0.TIM_HALL_InitStructure,&BLDC0.TIM_HALL_Sensor_InitStructure);
  
  HAL_NVIC_SetPriority(BLDC0_HALL_TIM_IRQn, 0, 0);   
  HAL_NVIC_EnableIRQ(BLDC0_HALL_TIM_IRQn);         
}

//===============================================
// hall init
//===============================================

void bldc0_hall_init(void)
{
	bldc0_hall_gpio_init();
	bldc0_hall_tim_init();
}

//===============================================
// switch for hall
//===============================================

void bldc0_hall_enable(void)
{
  __HAL_TIM_ENABLE_IT(&BLDC0.TIM_HALL_InitStructure, TIM_IT_TRIGGER);
  __HAL_TIM_ENABLE_IT(&BLDC0.TIM_HALL_InitStructure, TIM_IT_UPDATE);
  HAL_TIMEx_HallSensor_Start(&BLDC0.TIM_HALL_InitStructure);
}

void bldc0_hall_disable(void)
{
  __HAL_TIM_DISABLE_IT(&BLDC0.TIM_HALL_InitStructure, TIM_IT_TRIGGER);
  __HAL_TIM_DISABLE_IT(&BLDC0.TIM_HALL_InitStructure, TIM_IT_UPDATE);
  HAL_TIMEx_HallSensor_Stop(&BLDC0.TIM_HALL_InitStructure);
}

//===============================================
// stator position feedback from hall sensor
//===============================================

uint8_t bldc0_hall_getstate(void)
{
	uint8_t BLDC_state_temp = 0;
  // u
  if(HAL_GPIO_ReadPin(BLDC0_HALL_U_GPIO_PORT, BLDC0_HALL_U_PIN) != GPIO_PIN_RESET)
  {
    BLDC_state_temp |= 0x01U;
  }
	// v
  if(HAL_GPIO_ReadPin(BLDC0_HALL_V_GPIO_PORT, BLDC0_HALL_V_PIN) != GPIO_PIN_RESET)
  {
    BLDC_state_temp |= 0x02U;
  }
  //w
  if(HAL_GPIO_ReadPin(BLDC0_HALL_W_GPIO_PORT, BLDC0_HALL_W_PIN) != GPIO_PIN_RESET)
  {
    BLDC_state_temp |= 0x04U;
  }
	BLDC0.hall.hall_state = BLDC_state_temp;	
  return BLDC_state_temp;
}

void bldc0_hall_failure(void)
{
	bldc0_system_disable();				
	BLDC0.error_status = MOTOR_HALL_ERROR;	
}

//===============================================
// hall int handler
//===============================================

void BLDC0_HALL_TIM_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&BLDC0.TIM_HALL_InitStructure);
}

