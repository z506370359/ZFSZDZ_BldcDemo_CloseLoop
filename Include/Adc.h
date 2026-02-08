/***********************************************************************
* @file           Adc.h
* @author         ZhaoYun(ZFSZDZ)
* @date           2024.10.12
* @Version        V1.0.0
* @description    模拟量处理模块
***********************************************************************/
#include "../Include/Define.h"

void AdcSample(void);
void AdcFilter(void);
void AdcApp(void);

static void AdcSetSpeed(void);

tAdc_Type tAdc;
extern tDMA_Type tDMA;

/*******************Copyright (c) ZFSZDZ ******END OF FILE************/
