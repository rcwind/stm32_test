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
  ALIENTEKս��STM32������ʵ��1
  �����ʵ�� 
  ����֧�֣�www.openedv.com
  �Ա����̣�http://eboard.taobao.com 
  ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
  ������������ӿƼ����޹�˾  
  ���ߣ�����ԭ�� @ALIENTEK
 ************************************************/
u8 zhuanhuanzhi;
u8 GJBZ;  //�߼���ֵ��ȡ
u8 Res;
u16 adcx;	 
u16 dacval;	
float temp; 
u8 biaozhi=0;//��ͣ��־λ
u8  i;  //������ʱ��������2Mpaѭ��

int main(void)
{

    u8 zsch; //���ζ�����ʱ�洢ֵ
    u8 chazhi;
    u8 t = 0;
    u8 key;
    u8 Get ; //��ȡ��ȡADC�ķ���ֵ
    JiDdianQi_Init();
    delay_init();
    Adc_Init();

    delay_init();	    	 //��ʱ������ʼ��	  
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
    uart_init(115200);	 	//����1��ʼ��Ϊ115200
    KEY_Init();			  //��ʼ����������
    LED_Init();			     //LED�˿ڳ�ʼ��
    LCD_Init();			 	 //LCD��ʼ��
    usmart_dev.init(72);	//��ʼ��USMART	
    Adc_Init();		  		//ADC��ʼ��
    Dac1_Init();				//DAC��ʼ��
    uart2_init(9600);     //����3��ʼ��
    uart3_init(9600);     //����3��ʼ��
    TIM3_Int_Init(4999,7199);//10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms  
    EXTIX_Init();

    printf("init...\r\n");
    //////////////////������ʾ��ʼ��////////////////////////////////




    /////////////////������ʾ��ʼ��////////////////////////


    POINT_COLOR=RED;//��������Ϊ��ɫ 
    //	LCD_ShowString(60,50,200,16,16,"ZhuSaiBeng");	
    LCD_ShowString(60,70,200,16,16,"ZiDongKongZhi");	
    LCD_ShowString(60,90,200,16,16,"GongZhuang");
    LCD_ShowString(60,110,200,16,16,&Res);	


    //��ʾ��ʾ��Ϣ											      
    POINT_COLOR=BLUE;//��������Ϊ��ɫ
    LCD_ShowString(60,150,200,16,24,"ZhuanSu:0.000V");
    //	LCD_ShowString(60,180,200,16,24,&Res);





    while(1)
    {
        double dzc_init, dzc_after, dzc_chazhi;

        Mpa_0 = 1;//0MPA��ŷ���
        ChuJiBeng =1;//�����ô�
        //	 delay_s(30);//��ʱ30��ȴ��ȶ�

        //////////////////////////////������///////////////////////////////////////		

        Dac1_Set_Vol(800);//�߼��ô� 
        //   delay_s(4);//��ʱ4��ȴ��ȶ����ȸ߼����ȶ�

        delay_s(1);//��������ʱ

        /////////////////////////////�߼���////////////////////////////////////////

        //////////////////////////// ��¼ת�١���ѹ�����������ӳӳ�ʼֵ		

        LED0=!LED0;//��������ָʾ��

        /* Res =USART_ReceiveData(USART3);	//���ӳӶ�ȡ���յ������� */
        /* Res = zsch;                      //��¼���ӳӳ�ֵ��zsch�� */
        dzc_init = get_kg_data();
        LCD_ShowString(60,70,200,16,16,&Res);	//���ӳ�ʾ����ʾ��LCD��
        GaoJiBeng_XianShi();//�߼�ʾ����ʾ��LCD��

        //������ȡ��ѹ��ʾ��LCD�ϣ�δд��
        //������ȡ������ʾ��LCD�ϣ�δд��


        delay_s(60);//�ȴ�60s����



        dzc_after = get_kg_data();//USART_ReceiveData(USART3);	//�����󣬵��ӳӶ�ȡ���յ������ݡ�
        dzc_chazhi = (dzc_after - dzc_init);          //�������ֵ
        printf("chazhi: %f\r\n", dzc_chazhi);
        LCD_ShowString(60,70,200,16,16,&chazhi);	//���ӳ�ʾ����ʾ��LCD��
        GaoJiBeng_XianShi();//�߼�ʾ����ȡ���ʾ����ʾ��LCD�ϣ��˳���Ϊ�ӳ������


        //������ȡ��ѹ��ʾ��LCD�ϣ�δд��
        //������ȡ������ʾ��LCD�ϣ�δд��



        ////////////////////////////0Mpa�Լ����//////////////////////////////////

        ////////////////////////////2Mp���Կ�ʼ///////////////////////////////'///


        for(i=0;i<=3;i++)     //ѭ��3�ν���
        {
            Mpa_2 = 1; //2Mpa��ŷ���
            delay_s(4);//�ȴ�4s�ȴ��ȶ�
            Mpa_0 = 0; //0Mpa��ŷ��ر�
            delay_s(4);//�ȴ�4s�ȴ��ȶ�


            Res =USART_ReceiveData(USART3);	//���ӳӶ�ȡ���յ�������
            Res = zsch;                      //��¼���ӳӳ�ֵ��zsch��
            LCD_ShowString(60,70,200,16,16,&Res);	//���ӳ�ʾ����ʾ��LCD��
            GaoJiBeng_XianShi();//�߼�ʾ����ʾ��LCD��

            //������ȡ��ѹ��ʾ��LCD�ϣ�δд��
            //������ȡ������ʾ��LCD�ϣ�δд��


            delay_s(60);//�ȴ�60s����



            Res =USART_ReceiveData(USART3);	//�����󣬵��ӳӶ�ȡ���յ������ݡ�
            chazhi = (Res - zsch);          //�������ֵ
            LCD_ShowString(60,70,200,16,16,&chazhi);	//���ӳ�ʾ����ʾ��LCD��
            GaoJiBeng_XianShi();//�߼�ʾ����ȡ���ʾ����ʾ��LCD�ϣ��˳���Ϊ�ӳ������


            //������ȡ��ѹ��ʾ��LCD�ϣ�δд��
            //������ȡ������ʾ��LCD�ϣ�δд��	 

        }

        ///////////////////////////////2Mpa���Խ���/////////////////////////////////////////////////	

        ////////////////////////////4Mp���Կ�ʼ///////////////////////////////'///

        for(i=0;i<=3;i++)     //ѭ��3�ν���
        {
            Mpa_4 = 1; //2Mpa��ŷ���
            delay_s(4);//�ȴ�4s�ȴ��ȶ�
            Mpa_2 = 0; //0Mpa��ŷ��ر�
            delay_s(4);//�ȴ�4s�ȴ��ȶ�


            Res =USART_ReceiveData(USART3);	//���ӳӶ�ȡ���յ�������
            Res = zsch;                      //��¼���ӳӳ�ֵ��zsch��
            LCD_ShowString(60,70,200,16,16,&Res);	//���ӳ�ʾ����ʾ��LCD��
            GaoJiBeng_XianShi();//�߼�ʾ����ʾ��LCD��

            //������ȡ��ѹ��ʾ��LCD�ϣ�δд��
            //������ȡ������ʾ��LCD�ϣ�δд��


            delay_s(60);//�ȴ�60s����



            Res =USART_ReceiveData(USART3);	//�����󣬵��ӳӶ�ȡ���յ������ݡ�
            chazhi = (Res - zsch);          //�������ֵ
            LCD_ShowString(60,70,200,16,16,&chazhi);	//���ӳ�ʾ����ʾ��LCD��
            GaoJiBeng_XianShi();//�߼�ʾ����ȡ���ʾ����ʾ��LCD�ϣ��˳���Ϊ�ӳ������


            //������ȡ��ѹ��ʾ��LCD�ϣ�δд��
            //������ȡ������ʾ��LCD�ϣ�δд��	 

        }

        ///////////////////////////////4Mpa���Խ���/////////////////////////////////////////////////	


        ////////////////////////////6Mp���Կ�ʼ///////////////////////////////'///

        for(i=0;i<=3;i++)     //ѭ��3�ν���
        {
            Mpa_6 = 1; //2Mpa��ŷ���
            delay_s(4);//�ȴ�4s�ȴ��ȶ�
            Mpa_4 = 1; //0Mpa��ŷ��ر�
            delay_s(4);//�ȴ�4s�ȴ��ȶ�


            Res =USART_ReceiveData(USART3);	//���ӳӶ�ȡ���յ�������
            Res = zsch;                      //��¼���ӳӳ�ֵ��zsch��
            LCD_ShowString(60,70,200,16,16,&Res);	//���ӳ�ʾ����ʾ��LCD��
            GaoJiBeng_XianShi();//�߼�ʾ����ʾ��LCD��

            //������ȡ��ѹ��ʾ��LCD�ϣ�δд��
            //������ȡ������ʾ��LCD�ϣ�δд��


            delay_s(60);//�ȴ�60s����



            Res =USART_ReceiveData(USART3);	//�����󣬵��ӳӶ�ȡ���յ������ݡ�
            chazhi = (Res - zsch);          //�������ֵ
            LCD_ShowString(60,70,200,16,16,&chazhi);	//���ӳ�ʾ����ʾ��LCD��
            GaoJiBeng_XianShi();//�߼�ʾ����ȡ���ʾ����ʾ��LCD�ϣ��˳���Ϊ�ӳ������


            //������ȡ��ѹ��ʾ��LCD�ϣ�δд��
            //������ȡ������ʾ��LCD�ϣ�δд��	 

        }

        ///////////////////////////////6Mpa���Խ���/////////////////////////////////////////////////	



        deyay_min(10);  //ͣ����Ϣ10����


        ////////////////////////////8Mp���Կ�ʼ///////////////////////////////'///

        for(i=0;i<=3;i++)     //ѭ��3�ν���
        {
            Mpa_6 = 1; //2Mpa��ŷ���
            delay_s(4);//�ȴ�4s�ȴ��ȶ�
            Mpa_4 = 1; //0Mpa��ŷ��ر�
            delay_s(4);//�ȴ�4s�ȴ��ȶ�


            Res =USART_ReceiveData(USART3);	//���ӳӶ�ȡ���յ�������
            Res = zsch;                      //��¼���ӳӳ�ֵ��zsch��
            LCD_ShowString(60,70,200,16,16,&Res);	//���ӳ�ʾ����ʾ��LCD��
            GaoJiBeng_XianShi();//�߼�ʾ����ʾ��LCD��

            //������ȡ��ѹ��ʾ��LCD�ϣ�δд��
            //������ȡ������ʾ��LCD�ϣ�δд��


            delay_s(60);//�ȴ�60s����



            Res =USART_ReceiveData(USART3);	//�����󣬵��ӳӶ�ȡ���յ������ݡ�
            chazhi = (Res - zsch);          //�������ֵ
            LCD_ShowString(60,70,200,16,16,&chazhi);	//���ӳ�ʾ����ʾ��LCD��
            GaoJiBeng_XianShi();//�߼�ʾ����ȡ���ʾ����ʾ��LCD�ϣ��˳���Ϊ�ӳ������


            //������ȡ��ѹ��ʾ��LCD�ϣ�δд��
            //������ȡ������ʾ��LCD�ϣ�δд��	 

        }

        ///////////////////////////////8Mpa���Խ���/////////////////////////////////////////////////	




        deyay_min(10);  //ͣ����Ϣ10����


        ////////////////////////////10Mp���Կ�ʼ///////////////////////////////'///

        for(i=0;i<=3;i++)     //ѭ��3�ν���
        {
            Mpa_10 = 1; //2Mpa��ŷ���
            delay_s(4);//�ȴ�4s�ȴ��ȶ�
            Mpa_8 = 1; //0Mpa��ŷ��ر�
            delay_s(4);//�ȴ�4s�ȴ��ȶ�


            Res =USART_ReceiveData(USART3);	//���ӳӶ�ȡ���յ�������
            Res = zsch;                      //��¼���ӳӳ�ֵ��zsch��
            LCD_ShowString(60,70,200,16,16,&Res);	//���ӳ�ʾ����ʾ��LCD��
            GaoJiBeng_XianShi();//�߼�ʾ����ʾ��LCD��

            //������ȡ��ѹ��ʾ��LCD�ϣ�δд��
            //������ȡ������ʾ��LCD�ϣ�δд��


            delay_s(60);//�ȴ�60s����



            Res =USART_ReceiveData(USART3);	//�����󣬵��ӳӶ�ȡ���յ������ݡ�
            chazhi = (Res - zsch);          //�������ֵ
            LCD_ShowString(60,70,200,16,16,&chazhi);	//���ӳ�ʾ����ʾ��LCD��
            GaoJiBeng_XianShi();//�߼�ʾ����ȡ���ʾ����ʾ��LCD�ϣ��˳���Ϊ�ӳ������


            //������ȡ��ѹ��ʾ��LCD�ϣ�δд��
            //������ȡ������ʾ��LCD�ϣ�δд��	 

        }

        ///////////////////////////////10Mpa���Խ���/////////////////////////////////////////////////	






        while(biaozhi);//��ͣ��־λ

    }

}









