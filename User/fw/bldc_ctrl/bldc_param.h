//===============================================
//
//	File: bldc_param.h
//	Author: afterGlow,4ever
//	Date: 06252022
//	Version: v1.0
//
// 	This file is including bldc basic parameters.
//
//===============================================

#ifndef _BLDC_PARAM_H_
#define _BLDC_PARAM_H_

//===============================================
// BLDC basic parameters
//===============================================
#define BLDC_MAX_SPEED 4500
#define BLDC_MAX_SPEED_PROTECT 2000
#define BLDC_MIN_SPEED_PROTECT 300
#define BLDC_STALL_THRESHOLD 1
#define BLDC_HALL_TRUETABLE {1, 3, 2, 6, 4, 5}//Clockwise

//===============================================
// Temp sensor parameters
//===============================================
#define Ka 273.15       // 0?? ʱ??Ӧ???¶ȣ??????ģ?
#define R25 10000.0     // 25?? ????ֵ
#define T25 298.15     // 25?? ʱ??Ӧ???¶ȣ??????ģ?
#define B 3950.0        /* B-??????B = ln(R25 / Rt) / (1 / T ?C 1 / T25)?????? T = 25 + 273.15 */

//===============================================
// Filter parameters
//===============================================                             
#define SPEED_FILTER_NUM 30

//===============================================
// Pid parameters
//===============================================   
#define SPEED_POS_PID_KP 1
#define SPEED_POS_PID_KI 0.05
#define SPEED_POS_PID_KD 0.1

#define SPEED_INC_PID_KP 1
#define SPEED_INC_PID_KI 0.03
#define SPEED_INC_PID_KD 0.1

#endif
