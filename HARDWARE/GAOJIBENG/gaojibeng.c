#include "gaojibeng.h"
#include "adc.h"
#include "dac.h"
#include "lcd.h"



		extern	u16 adcx;	 
    extern u16 dacval;
		extern 	float temp;
void GaoJiBeng_XianShi(void)
{     
				
	
	
      adcx=Get_Adc_Average(ADC_Channel_1,10);		//�õ�ADCת��ֵ	                           �����2
			temp=(float)adcx*(3.3/4096);			//�õ�ADC��ѹֵ
			adcx=temp;
 			LCD_ShowxNum(155,150,temp,1,24,0);     	//��ʾ��ѹֵ��������
 			temp-=adcx;
			temp*=1000;
			LCD_ShowxNum(179,150,temp,3,24,0X80); 	//��ʾ��ѹֵ��С������
	
	

}