//===============================================
//
//	File: bldc_ctrl.c
//	Author: afterGlow,4ever
//	Date: 06252022
//	Version: v1.0
//
// 	This file is used to operate cotrollers in the loop. 
//
//===============================================

#include "bldc.h"

extern bldc_hall_status_t BLDC0;
extern pid_pos_controller_t pid_pos_controller0;
extern pid_inc_controller_t pid_inc_controller0;

//===============================================
// bldc system loop controller init
//===============================================

void bldc0_system_init(void)
{
#ifdef SPEED_POS_PID		
	pid_pos_controller0_init();
#endif		
#ifdef SPEED_INC_PID	
	pid_inc_controller0_init();
#endif		
#ifdef LOCATION_POS_PID		
#endif		
#ifdef LOCATION_INC_PID		
#endif			
	bldc0_para_reset();
	BLDC0.error_status = MOTOR_NORMAL;
	bldc0_actutor_init();
	bldc0_feedback_init();
	bldc0_loop_init();
}

void bldc0_system_enable(void)
{
	BLDC0.speed.speed_set = BLDC0.speed.speed_target;
	bldc0_actutor_speed_set();// start speed
	bldc0_actutor_dir_set(BLDC0.direction.dir_target);// start direction		
#ifdef SPEED_POS_PID			
	pid_pos_controller0.target_val = BLDC0.speed.speed_target;	
#endif		
#ifdef SPEED_INC_PID	
	pid_inc_controller0.target_val = BLDC0.speed.speed_target;	
#endif		
#ifdef LOCATION_POS_PID		
#endif		
#ifdef LOCATION_INC_PID		
#endif			
	BLDC0.enable_status = MOTOR_ENABLE;
	BLDC0.operation_status = MOTOR_START_STAGE;
	BLDC0.error_status = MOTOR_NORMAL;
	bldc0_actutor_enable();
	delay_ms(50);
	bldc0_feedback_enable();
  // commutation at start
	HAL_TIM_TriggerCallback(&BLDC0.TIM_HALL_InitStructure);
  bldc0_loop_enable();
}

void bldc0_system_disable(void)
{
	bldc0_actutor_disable();
	bldc0_feedback_disable();
	bldc0_loop_disable();
#ifdef SPEED_POS_PID		
	pid_pos_controller0_para_reset();
#endif		
#ifdef SPEED_INC_PID		
	pid_inc_controller0_para_reset();
#endif		
#ifdef LOCATION_POS_PID		
#endif		
#ifdef LOCATION_INC_PID		
#endif			
	bldc0_para_reset();
}

void bldc0_para_reset(void)
{
	BLDC0.enable_status = MOTOR_DISABLE;
	BLDC0.operation_status = MOTOR_STOP;
	BLDC0.speed.speed_target = 0;	
	BLDC0.speed.speed_set = 0;		
	BLDC0.speed.speed_set_dutycycle = 0;		
	BLDC0.speed.speed_feedback = 0;
	BLDC0.speed.filter_member_count = 0;
}

//===============================================
// bldc system operation set
//===============================================

void bldc0_speed_up(void)
{
	BLDC0.speed.speed_target = BLDC0.speed.speed_target + 100;
	if(BLDC0.speed.speed_target > 0)
	{
		if(BLDC0.speed.speed_target > BLDC_MAX_SPEED_PROTECT)
			BLDC0.speed.speed_target = BLDC_MAX_SPEED_PROTECT;
		if(BLDC0.speed.speed_target < BLDC_MIN_SPEED_PROTECT)
			BLDC0.speed.speed_target = BLDC_MIN_SPEED_PROTECT;		
	}
	else if(BLDC0.speed.speed_target < 0)
	{
		if(BLDC0.speed.speed_target < -BLDC_MAX_SPEED_PROTECT)
			BLDC0.speed.speed_target = -BLDC_MAX_SPEED_PROTECT;
		if(BLDC0.speed.speed_target > -BLDC_MIN_SPEED_PROTECT)
			BLDC0.speed.speed_target = -BLDC_MIN_SPEED_PROTECT;		
	}		
#ifdef SPEED_POS_PID		
	pid_pos_controller0.target_val = BLDC0.speed.speed_target;
#endif
#ifdef SPEED_INC_PID	
	pid_inc_controller0.target_val = BLDC0.speed.speed_target;
#endif
}

void bldc0_speed_down(void)
{
	BLDC0.speed.speed_target = BLDC0.speed.speed_target - 100;
	if(BLDC0.speed.speed_target > 0)
	{
		if(BLDC0.speed.speed_target > BLDC_MAX_SPEED_PROTECT)
			BLDC0.speed.speed_target = BLDC_MAX_SPEED_PROTECT;
		if(BLDC0.speed.speed_target < BLDC_MIN_SPEED_PROTECT)
			BLDC0.speed.speed_target = BLDC_MIN_SPEED_PROTECT;		
	}
	else if(BLDC0.speed.speed_target < 0)
	{
		if(BLDC0.speed.speed_target < -BLDC_MAX_SPEED_PROTECT)
			BLDC0.speed.speed_target = -BLDC_MAX_SPEED_PROTECT;
		if(BLDC0.speed.speed_target > -BLDC_MIN_SPEED_PROTECT)
			BLDC0.speed.speed_target = -BLDC_MIN_SPEED_PROTECT;		
	}	
#ifdef SPEED_POS_PID		
	pid_pos_controller0.target_val = BLDC0.speed.speed_target;
#endif
#ifdef SPEED_INC_PID	
	pid_inc_controller0.target_val = BLDC0.speed.speed_target;
#endif
}

void bldc0_reverse(void)
{
	BLDC0.speed.speed_target = - BLDC0.speed.speed_target;	
#ifdef SPEED_CLOSE_LOOP	
	#ifdef SPEED_POS_PID		
	pid_pos_controller0.target_val = BLDC0.speed.speed_target;
	#endif
	#ifdef SPEED_INC_PID	
	pid_inc_controller0.target_val = BLDC0.speed.speed_target;
	#endif	
#endif
}

//===============================================
// bldc system loop init
//===============================================

static void bldc0_loop_tim_init(void)
{
	BLDC0_LOOP_TIM_CLK_ENABLE(); 

	BLDC0.TIM_loop_InitStructure.Instance = BLDC0_LOOP_TIM;
	BLDC0.TIM_loop_InitStructure.Init.Period = BLDC0_LOOP_PERIOD_COUNT - 1;       

	BLDC0.TIM_loop_InitStructure.Init.Prescaler = BLDC0_LOOP_PRESCALER_COUNT - 1;	
  BLDC0.TIM_loop_InitStructure.Init.CounterMode = TIM_COUNTERMODE_UP;        
  BLDC0.TIM_loop_InitStructure.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;  

	HAL_TIM_Base_Init(&BLDC0.TIM_loop_InitStructure);

	HAL_NVIC_SetPriority(BLDC0_LOOP_TIM_IRQn, 1, 3);
	HAL_NVIC_EnableIRQ(BLDC0_LOOP_TIM_IRQn);	
}

void bldc0_loop_init(void)
{
	bldc0_loop_tim_init();
}

void bldc0_loop_enable(void)
{
	HAL_TIM_Base_Start_IT(&BLDC0.TIM_loop_InitStructure);	
}

void bldc0_loop_disable(void)
{
	HAL_TIM_Base_Stop_IT(&BLDC0.TIM_loop_InitStructure);	
}

void BLDC0_LOOP_TIM_IRQHandler(void)
{
  HAL_TIM_IRQHandler(&BLDC0.TIM_loop_InitStructure);
}

//===============================================
// bldc system loop controller
//===============================================

static void bldc0_speed_loop(void)
{
	if(BLDC0.enable_status == MOTOR_ENABLE)
	{
#ifdef SPEED_CLOSE_LOOP	
	#ifdef SPEED_POS_PID		
		pid_pos_controller0.target_val = BLDC0.speed.speed_target;
		BLDC0.speed.speed_set = pid_pos_controller0_control(BLDC0.speed.speed_feedback);
	#endif		
	#ifdef SPEED_INC_PID		
		pid_inc_controller0.target_val = BLDC0.speed.speed_target;
		BLDC0.speed.speed_set = pid_inc_controller0_control(BLDC0.speed.speed_feedback);
	#endif		
#else
		BLDC0.speed.speed_set = BLDC0.speed.speed_target;	
#endif		
		if(BLDC0.speed.speed_set < 0)
		{
			bldc0_actutor_dir_set(MOTOR_REV);
			if((- BLDC0.speed.speed_set) > BLDC_MAX_SPEED_PROTECT)
				BLDC0.speed.speed_set =  - BLDC_MAX_SPEED_PROTECT;	
		}
		else
		{
			bldc0_actutor_dir_set(MOTOR_FWD);
			if(BLDC0.speed.speed_set > BLDC_MAX_SPEED_PROTECT)
				BLDC0.speed.speed_set = BLDC_MAX_SPEED_PROTECT;	
		}		
		bldc0_actutor_speed_set();
	}
}

#if 0
float Location_control=0;

static void BLDC_Location_Loop(void)
{
	float Location_fb;
	
	if(BLDC1_status.BLDC_normal_status == MOTOR_ENABLE)
	{
#ifdef LOCATION_CLOSE_LOOP					
		Location_fb = BLDC_Location_Get();
	#if defined(LOCATION_POS_PID)		
		Location_control = Pos_Pid_Location_Control(Location_fb);
	#endif		
	#if defined(LOCATION_INC_PID)		
		Location_control = Inc_Pid_Location_Control(Location_fb);
	#endif		
		if(Location_control < 0)
		{
			Location_control = -Location_control;
			BLDC_Direction_Set(MOTOR_REV);
		}
		else
		{
			BLDC_Direction_Set(MOTOR_FWD);			
		}
		if(Location_control > 2500)
			Location_control = 2500;	
		BLDC_Speed_Set(Location_control);				
#endif	
	}
}
#endif

void bldc0_loop(void)
{
	bldc0_speed_loop();
//	bldc0_location_loop();
#ifdef MINIBALANCE_SSDP_EN 				
	DataScope();
#endif	
}

