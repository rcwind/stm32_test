#include "gaojibeng.h"
#include "adc.h"
#include "dac.h"
#include "lcd.h"



		extern	u16 adcx;	 
    extern u16 dacval;
		extern 	float temp;
void GaoJiBeng_XianShi(void)
{     
				
	
	
      adcx=Get_Adc_Average(ADC_Channel_1,10);		//得到ADC转换值	                           问题点2
			temp=(float)adcx*(3.3/4096);			//得到ADC电压值
			adcx=temp;
 			LCD_ShowxNum(155,150,temp,1,24,0);     	//显示电压值整数部分
 			temp-=adcx;
			temp*=1000;
			LCD_ShowxNum(179,150,temp,3,24,0X80); 	//显示电压值的小数部分
	
	

}