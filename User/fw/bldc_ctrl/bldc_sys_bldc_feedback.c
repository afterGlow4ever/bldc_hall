//===============================================
//
//	File: bldc_drive.c
//	Author: afterGlow,4ever
//	Date: 06252022
//	Version: v1.0
//
// 	This file is used to operate feedback in the loop. 
//
//===============================================

#include "bldc.h"

extern bldc_hall_status_t BLDC0;
extern commutation_funptr bldc0_commutation_funptr[6];

//===============================================
// bldc system feedback init
//===============================================

void bldc0_feedback_init(void)
{
	//	BLDC_ADC_Init();
	bldc0_hall_init();
}

//===============================================
// bldc system feedback switch
//===============================================

void bldc0_feedback_enable(void)
{
	BLDC0.stall_timeout = 0;
	bldc0_hall_enable();
}

void bldc0_feedback_disable(void)
{
	bldc0_hall_disable();
//	BLDC0.stall_timeout = 0;
}

//===============================================
// speed from hall sensor
// Forward/Anticlockwise: 154623
// Backward/Clockwise: 645132
//===============================================

static void bldc0_speed_dir_feedback(uint8_t hall_step)
{
  uint8_t step[6] = {1, 3, 2, 6, 4, 5};//Clockwise

  static uint8_t hall_pos_last = 0;
  uint8_t hall_pos_temp = 0;    // 记录当前霍尔位置
  int8_t dir = 1;
  
  for (hall_pos_temp = 0; hall_pos_temp < 6; hall_pos_temp++)
  {
    if (step[hall_pos_temp] == hall_step)    // 找到当前霍尔的位置
    {
      break;
    }
  }
  
  /* 端点处理 */
  if (hall_pos_temp == 0)
  {
    if (hall_pos_last == 1)
    {
      dir = 1;
    }
    else if (hall_pos_last == 5)
    {
      dir = -1;
    }
  }
  /* 端点处理 */
  else if (hall_pos_temp == 5)
  {
    if (hall_pos_last == 0)
    {
      dir = 1;
    }
    else if (hall_pos_last == 4)
    {
      dir = -1;
    }
  }
  else if (hall_pos_temp > hall_pos_last)
  {
    dir = -1;
  }
  else if (hall_pos_temp < hall_pos_last)
  {
    dir = 1;
  }
  
  hall_pos_last = hall_pos_temp;
	BLDC0.speed.speed_feedback_raw_array[BLDC0.speed.filter_member_count-1] *= dir;
}

void bldc0_speed_feedback(uint8_t hall_step, uint32_t time)
{
  float speed_temp = 0;
  float f = 0;
	uint8_t i;
  /* 计算速度：
     电机每转一圈共用12个脉冲
	(1.0/(200000000.0/HALL_PRESCALER_COUNT)为计数器的周期
	(1.0/(200000000.0/HALL_PRESCALER_COUNT) * time)为时间长。
  */

  if (time == 0)
		BLDC0.speed.speed_feedback_raw_array[BLDC0.speed.filter_member_count++] = 0;
  else
  {
    f = (1.0f / (200000000.0f / (float)BLDC0_HALL_PRESCALER_COUNT) * (float)time);
    f = (1.0f / 12.0f) / (f  / 60.0f);
		BLDC0.speed.speed_feedback_raw_array[BLDC0.speed.filter_member_count++] = f;
  }
	
	bldc0_speed_dir_feedback(hall_step);
	
	// When the number of raw speed data is more than filter member number, switching speed filter algorithm.
  if (BLDC0.speed.filter_member_count >= SPEED_FILTER_NUM)
  {
		BLDC0.operation_status = MOTOR_ACCLERATE_STAGE;
    BLDC0.speed.filter_member_count = 0;
  }

  speed_temp = 0;

	// Using average filter
  if (BLDC0.operation_status > MOTOR_START_STAGE)
  {
    for (i = 0; i < SPEED_FILTER_NUM; i++)
    {
      speed_temp += BLDC0.speed.speed_feedback_raw_array[i];
    }
    BLDC0.speed.speed_feedback = speed_temp / (float)SPEED_FILTER_NUM;
  }
  else
  {
    for (i = 0; i < BLDC0.speed.filter_member_count; i++)
    {
      speed_temp += BLDC0.speed.speed_feedback_raw_array[i];
    }
    BLDC0.speed.speed_feedback = speed_temp / BLDC0.speed.filter_member_count;
  }
}

