#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "jidianqi.h"
#include "adc.h"
#include "dac.h"
#include "lcd.h"
#include "key.h"
#include "usmart.h"
#include "usart3.h"
#include "usart2.h"
#include "timer.h"
#include "exti.h"

#include "usmart.h" 
#include "malloc.h"
#include "sdio_sdcard.h"  
#include "w25qxx.h"    
#include "ff.h"  
#include "exfuns.h"   
#include "text.h"
#include "gaojibeng.h"


/************************************************
  ALIENTEK战舰STM32开发板实验1
  跑马灯实验 
  技术支持：www.openedv.com
  淘宝店铺：http://eboard.taobao.com 
  关注微信公众平台微信号："正点原子"，免费获取STM32资料。
  广州市星翼电子科技有限公司  
  作者：正点原子 @ALIENTEK
 ************************************************/
u8 zhuanhuanzhi;
u8 GJBZ;  //高级泵值获取
u8 Res;
u16 adcx;	 
u16 dacval;	
float temp; 
u8 biaozhi=0;//暂停标志位
u8  i;  //设置临时变量用于2Mpa循环

int main(void)
{

    u8 zsch; //初次读数暂时存储值
    u8 chazhi;
    u8 t = 0;
    u8 key;
    u8 Get ; //获取读取ADC的返回值
    JiDdianQi_Init();
    delay_init();
    Adc_Init();

    delay_init();	    	 //延时函数初始化	  
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
    uart_init(115200);	 	//串口1初始化为115200
    KEY_Init();			  //初始化按键程序
    LED_Init();			     //LED端口初始化
    LCD_Init();			 	 //LCD初始化
    usmart_dev.init(72);	//初始化USMART	
    Adc_Init();		  		//ADC初始化
    Dac1_Init();				//DAC初始化
    uart2_init(9600);     //串口3初始化
    uart3_init(9600);     //串口3初始化
    TIM3_Int_Init(4999,7199);//10Khz的计数频率，计数到5000为500ms  
    EXTIX_Init();

    printf("init...\r\n");
    //////////////////汉字显示初始化////////////////////////////////




    /////////////////汉字显示初始化////////////////////////


    POINT_COLOR=RED;//设置字体为红色 
    //	LCD_ShowString(60,50,200,16,16,"ZhuSaiBeng");	
    LCD_ShowString(60,70,200,16,16,"ZiDongKongZhi");	
    LCD_ShowString(60,90,200,16,16,"GongZhuang");
    LCD_ShowString(60,110,200,16,16,&Res);	


    //显示提示信息											      
    POINT_COLOR=BLUE;//设置字体为蓝色
    LCD_ShowString(60,150,200,16,24,"ZhuanSu:0.000V");
    //	LCD_ShowString(60,180,200,16,24,&Res);





    while(1)
    {
        double dzc_init, dzc_after, dzc_chazhi;

        Mpa_0 = 1;//0MPA电磁阀打开
        ChuJiBeng =1;//初级泵打开
        //	 delay_s(30);//延时30秒等待稳定

        //////////////////////////////初级泵///////////////////////////////////////		

        Dac1_Set_Vol(800);//高级泵打开 
        //   delay_s(4);//延时4秒等待稳定，等高级泵稳定

        delay_s(1);//测试用延时

        /////////////////////////////高级泵////////////////////////////////////////

        //////////////////////////// 记录转速、电压表、电流表、电子秤初始值		

        LED0=!LED0;//程序运行指示灯

        /* Res =USART_ReceiveData(USART3);	//电子秤读取接收到的数据 */
        /* Res = zsch;                      //记录电子秤初值在zsch里 */
        dzc_init = get_kg_data();
        LCD_ShowString(60,70,200,16,16,&Res);	//电子秤示数显示在LCD上
        GaoJiBeng_XianShi();//高级示数显示在LCD上

        //函数读取电压显示在LCD上，未写。
        //函数读取电流显示在LCD上，未写。


        delay_s(60);//等待60s做工



        dzc_after = get_kg_data();//USART_ReceiveData(USART3);	//做工后，电子秤读取接收到的数据。
        dzc_chazhi = (dzc_after - dzc_init);          //计算出差值
        printf("chazhi: %f\r\n", dzc_chazhi);
        LCD_ShowString(60,70,200,16,16,&chazhi);	//电子秤示数显示在LCD上
        GaoJiBeng_XianShi();//高级示数读取后泵示数显示在LCD上，此程序为子程序调用


        //函数读取电压显示在LCD上，未写。
        //函数读取电流显示在LCD上，未写。



        ////////////////////////////0Mpa自检结束//////////////////////////////////

        ////////////////////////////2Mp测试开始///////////////////////////////'///


        for(i=0;i<=3;i++)     //循环3次结束
        {
            Mpa_2 = 1; //2Mpa电磁阀打开
            delay_s(4);//等待4s等待稳定
            Mpa_0 = 0; //0Mpa电磁阀关闭
            delay_s(4);//等待4s等待稳定


            Res =USART_ReceiveData(USART3);	//电子秤读取接收到的数据
            Res = zsch;                      //记录电子秤初值在zsch里
            LCD_ShowString(60,70,200,16,16,&Res);	//电子秤示数显示在LCD上
            GaoJiBeng_XianShi();//高级示数显示在LCD上

            //函数读取电压显示在LCD上，未写。
            //函数读取电流显示在LCD上，未写。


            delay_s(60);//等待60s做工



            Res =USART_ReceiveData(USART3);	//做工后，电子秤读取接收到的数据。
            chazhi = (Res - zsch);          //计算出差值
            LCD_ShowString(60,70,200,16,16,&chazhi);	//电子秤示数显示在LCD上
            GaoJiBeng_XianShi();//高级示数读取后泵示数显示在LCD上，此程序为子程序调用


            //函数读取电压显示在LCD上，未写。
            //函数读取电流显示在LCD上，未写。	 

        }

        ///////////////////////////////2Mpa测试结束/////////////////////////////////////////////////	

        ////////////////////////////4Mp测试开始///////////////////////////////'///

        for(i=0;i<=3;i++)     //循环3次结束
        {
            Mpa_4 = 1; //2Mpa电磁阀打开
            delay_s(4);//等待4s等待稳定
            Mpa_2 = 0; //0Mpa电磁阀关闭
            delay_s(4);//等待4s等待稳定


            Res =USART_ReceiveData(USART3);	//电子秤读取接收到的数据
            Res = zsch;                      //记录电子秤初值在zsch里
            LCD_ShowString(60,70,200,16,16,&Res);	//电子秤示数显示在LCD上
            GaoJiBeng_XianShi();//高级示数显示在LCD上

            //函数读取电压显示在LCD上，未写。
            //函数读取电流显示在LCD上，未写。


            delay_s(60);//等待60s做工



            Res =USART_ReceiveData(USART3);	//做工后，电子秤读取接收到的数据。
            chazhi = (Res - zsch);          //计算出差值
            LCD_ShowString(60,70,200,16,16,&chazhi);	//电子秤示数显示在LCD上
            GaoJiBeng_XianShi();//高级示数读取后泵示数显示在LCD上，此程序为子程序调用


            //函数读取电压显示在LCD上，未写。
            //函数读取电流显示在LCD上，未写。	 

        }

        ///////////////////////////////4Mpa测试结束/////////////////////////////////////////////////	


        ////////////////////////////6Mp测试开始///////////////////////////////'///

        for(i=0;i<=3;i++)     //循环3次结束
        {
            Mpa_6 = 1; //2Mpa电磁阀打开
            delay_s(4);//等待4s等待稳定
            Mpa_4 = 1; //0Mpa电磁阀关闭
            delay_s(4);//等待4s等待稳定


            Res =USART_ReceiveData(USART3);	//电子秤读取接收到的数据
            Res = zsch;                      //记录电子秤初值在zsch里
            LCD_ShowString(60,70,200,16,16,&Res);	//电子秤示数显示在LCD上
            GaoJiBeng_XianShi();//高级示数显示在LCD上

            //函数读取电压显示在LCD上，未写。
            //函数读取电流显示在LCD上，未写。


            delay_s(60);//等待60s做工



            Res =USART_ReceiveData(USART3);	//做工后，电子秤读取接收到的数据。
            chazhi = (Res - zsch);          //计算出差值
            LCD_ShowString(60,70,200,16,16,&chazhi);	//电子秤示数显示在LCD上
            GaoJiBeng_XianShi();//高级示数读取后泵示数显示在LCD上，此程序为子程序调用


            //函数读取电压显示在LCD上，未写。
            //函数读取电流显示在LCD上，未写。	 

        }

        ///////////////////////////////6Mpa测试结束/////////////////////////////////////////////////	



        deyay_min(10);  //停机休息10分钟


        ////////////////////////////8Mp测试开始///////////////////////////////'///

        for(i=0;i<=3;i++)     //循环3次结束
        {
            Mpa_6 = 1; //2Mpa电磁阀打开
            delay_s(4);//等待4s等待稳定
            Mpa_4 = 1; //0Mpa电磁阀关闭
            delay_s(4);//等待4s等待稳定


            Res =USART_ReceiveData(USART3);	//电子秤读取接收到的数据
            Res = zsch;                      //记录电子秤初值在zsch里
            LCD_ShowString(60,70,200,16,16,&Res);	//电子秤示数显示在LCD上
            GaoJiBeng_XianShi();//高级示数显示在LCD上

            //函数读取电压显示在LCD上，未写。
            //函数读取电流显示在LCD上，未写。


            delay_s(60);//等待60s做工



            Res =USART_ReceiveData(USART3);	//做工后，电子秤读取接收到的数据。
            chazhi = (Res - zsch);          //计算出差值
            LCD_ShowString(60,70,200,16,16,&chazhi);	//电子秤示数显示在LCD上
            GaoJiBeng_XianShi();//高级示数读取后泵示数显示在LCD上，此程序为子程序调用


            //函数读取电压显示在LCD上，未写。
            //函数读取电流显示在LCD上，未写。	 

        }

        ///////////////////////////////8Mpa测试结束/////////////////////////////////////////////////	




        deyay_min(10);  //停机休息10分钟


        ////////////////////////////10Mp测试开始///////////////////////////////'///

        for(i=0;i<=3;i++)     //循环3次结束
        {
            Mpa_10 = 1; //2Mpa电磁阀打开
            delay_s(4);//等待4s等待稳定
            Mpa_8 = 1; //0Mpa电磁阀关闭
            delay_s(4);//等待4s等待稳定


            Res =USART_ReceiveData(USART3);	//电子秤读取接收到的数据
            Res = zsch;                      //记录电子秤初值在zsch里
            LCD_ShowString(60,70,200,16,16,&Res);	//电子秤示数显示在LCD上
            GaoJiBeng_XianShi();//高级示数显示在LCD上

            //函数读取电压显示在LCD上，未写。
            //函数读取电流显示在LCD上，未写。


            delay_s(60);//等待60s做工



            Res =USART_ReceiveData(USART3);	//做工后，电子秤读取接收到的数据。
            chazhi = (Res - zsch);          //计算出差值
            LCD_ShowString(60,70,200,16,16,&chazhi);	//电子秤示数显示在LCD上
            GaoJiBeng_XianShi();//高级示数读取后泵示数显示在LCD上，此程序为子程序调用


            //函数读取电压显示在LCD上，未写。
            //函数读取电流显示在LCD上，未写。	 

        }

        ///////////////////////////////10Mpa测试结束/////////////////////////////////////////////////	






        while(biaozhi);//暂停标志位

    }

}









