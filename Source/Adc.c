/***********************************************************************
* @file           Adc.c
* @author         ZhaoYun(ZFSZDZ)
* @date           2024.10.12
* @Version        V1.0.0
* @description    模拟量处理模块
***********************************************************************/
#include "../Include/Adc.h"

/***********************************************************************
* 函数名称  : AdcSample
* 功能描述  ：读取ADC转换结果瞬时值
* 形参      ：无
* 返回值    ：无
* 要点备注  ：AN0 --> A/B/C  AN3-->VBUS AN4-->IBUS  AN5-->POT AN9-->NTC
***********************************************************************/
void AdcSample(void)
{
    tAdc.AN0.ValueInstant = tDMA.ADDATA0;
    tAdc.AN3.ValueInstant = tDMA.ADDATA1;
    tAdc.AN4.ValueInstant = tDMA.ADDATA2;
    tAdc.AN5.ValueInstant = tDMA.ADDATA3;
    tAdc.AN9.ValueInstant = tDMA.ADDATA4;
}
/***********************************************************************
* 函数名称  : AdcFilter
* 功能描述  ：ADC转换数据滤波
* 形参      ：无
* 返回值    ：无
* 要点备注  ：NTC  POT  VBUS
***********************************************************************/
void AdcFilter(void)
{
    //Vbus 平均值求取
    tAdc.AN3.Sum += tAdc.AN3.ValueInstant;
    tAdc.AN3.Num ++;
    if(tAdc.AN3.Num >= 128)
    {
        tAdc.AN3.ValueAverage = tAdc.AN3.Sum >> 7;
        tAdc.AN3.Sum = 0;
        tAdc.AN3.Num = 0;
    }
    //NTC 平均值求取
    tAdc.AN9.Sum += tAdc.AN9.ValueInstant;
    tAdc.AN9.Num ++;
    if(tAdc.AN9.Num >= 128)
    {
        tAdc.AN9.ValueAverage = tAdc.AN9.Sum >> 7;
        tAdc.AN9.Sum = 0;
        tAdc.AN9.Num = 0;
    }
    //POT 平均值求取
    tAdc.AN5.Sum += tAdc.AN5.ValueInstant;
    tAdc.AN5.Num ++;
    if(tAdc.AN5.Num >= 64)
    {
        tAdc.AN5.ValueAverage = tAdc.AN5.Sum >> 6;
        tAdc.AN5.Sum = 0;
        tAdc.AN5.Num = 0;
    }
}
/***********************************************************************
* 函数名称  : AdcSetSpeed
* 功能描述  ：将旋钮AD值转换处理为PWM占空比比较值
* 形参      ：无
* 返回值    ：无
* 要点备注  ：ARR --> 2000 
***********************************************************************/
static void AdcSetSpeed(void)
{
   unsigned short Temp;
   unsigned short TempA;
    
   Temp = tAdc.AN5.ValueAverage >> 1; //0~2047
   if(Temp >= 2000)
       Temp = 2000;
   tAdc.PotValue = Temp; //0~2000
   
   TempA = tAdc.AN5.ValueAverage << 4; //0~65520
   tAdc.SetSpeed = TempA * 3000 >> 16; //0~2999;
}
/***********************************************************************
* 函数名称  : AdcApp
* 功能描述  ：ADC模块应用程序
* 形参      ：无
* 返回值    ：无
* 要点备注  ：无
***********************************************************************/
void AdcApp(void)
{
    AdcSetSpeed(); //获取处理后的旋钮值，作为占空比调节量
}
/*******************Copyright (c) ZFSZDZ ******END OF FILE************/
