#include "usart3.h"

#include <string.h>
#include <stdio.h>


u8 USART3_RX_BUF[USART3_REC_LEN];     //???ջ???,????USART_REC_LEN???ֽ?.

u16 USART3_RX_STA=0;       //????״̬????	  

extern u8 Res; 

#define dma_buf_size 128

static u8 RxBuffer[dma_buf_size];
static u8 dianzichen_data[dma_buf_size];
double kg_data = 0.0;


//USART3???ڳ?ʼ??
void uart3_init(u32 bound)
{
    //GPIO?˿?????
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    DMA_InitTypeDef DMA_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//ʹ??USART3??GPIOAʱ??
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//ʹ??USART3??GPIOAʱ??



    //USART3_TX   GPIOA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PA.9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//????????????
    GPIO_Init(GPIOB, &GPIO_InitStructure);//??ʼ??GPIOA.9

    //USART3_RX	  GPIOA.10??ʼ??
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PA10
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//????????
    GPIO_Init(GPIOB, &GPIO_InitStructure);//??ʼ??GPIOA.10  

    //Usart1 NVIC ????
    NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//??ռ???ȼ?3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//?????ȼ?3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ??ʹ??
    NVIC_Init(&NVIC_InitStructure);	//????ָ???Ĳ?????ʼ??VIC?Ĵ???

    //USART ??ʼ??????

    USART_InitStructure.USART_BaudRate = bound;//???ڲ?????
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;//?ֳ?Ϊ8λ???ݸ?ʽ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ??ֹͣλ
    USART_InitStructure.USART_Parity = USART_Parity_No;//????żУ??λ
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//??Ӳ????????????
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//?շ?ģʽ

    USART_Init(USART3, &USART_InitStructure); //??ʼ??????1
    //USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//???����ڽ????ж?
    USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);  //?? USART3 ??????
    USART_Cmd(USART3, ENABLE);                    //ʹ?ܴ???1 


    DMA_DeInit(DMA1_Channel3);
    DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)(&USART3->DR);    //??--->??
    DMA_InitStruct.DMA_MemoryBaseAddr = (uint32_t)RxBuffer;
    DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStruct.DMA_BufferSize = dma_buf_size;
    DMA_InitStruct.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStruct.DMA_M2M = DMA_M2M_Disable;
    DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStruct.DMA_Priority = DMA_Priority_Medium;
    DMA_InitStruct.DMA_Mode = DMA_Mode_Normal;
    DMA_Init(DMA1_Channel3, &DMA_InitStruct);

    DMA_Cmd(DMA1_Channel3, ENABLE);
    USART_DMACmd(USART3, USART_DMAReq_Rx, ENABLE);              // ?? USART3??DMA
}
double get_kg_data() 
{
    return kg_data;
}
void parse_dianzichen_data(void)
{
    int matched;
    double kg;
    matched = sscanf((const char *)dianzichen_data, "%*s,%*s,+ %lfkg", &kg);
    if(matched == 1)
    {
        printf("dianzichen_data:%lfkg\r\n", kg);
        kg_data = kg;
    }
    else
    {
        kg_data = 0.0;
        printf("parse dianzichen data error!!!\r\n", kg);
    }
}
void USART3_IRQHandler(void)                	//????1?жϷ???????
{
    extern u8 Res; 
    //Res =USART_ReceiveData(USART3);	//??ȡ???յ???????
    //USART_SendData(USART3, Res);//?򴮿?1???????? USART3_RX_BUF[t]
    uint8_t clear = clear;  // ?????????????????
    uint8_t RxCounter;
    int i;
    if(USART_GetITStatus(USART3, USART_IT_IDLE) != RESET)
    {
        DMA_Cmd(DMA1_Channel3, DISABLE);                // ??DMA,??DMA??

        clear =USART3->SR;
        clear = USART3->DR;

        //      RxCounter = BufferSize - DMA1_Channel3->CNDTR;//???????
        RxCounter = dma_buf_size - DMA_GetCurrDataCounter(DMA1_Channel3);//???????

        //      USART_ITConfig(USART3, USART_IT_IDLE, ENABLE);

        // RxStatus = 1;   //???????
        USART_ClearITPendingBit(USART3, USART_IT_IDLE); // ??????

        memset(dianzichen_data, 0, dma_buf_size);
        for(i = 0; i < RxCounter; i++)
        {
            dianzichen_data[i] = RxBuffer[i];
        }
        parse_dianzichen_data();
        DMA_SetCurrDataCounter(DMA1_Channel3, dma_buf_size);
        DMA_Cmd(DMA1_Channel3, ENABLE);
    }
}

