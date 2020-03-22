#include "usart3.h"

#include <string.h>
#include <stdio.h>

extern u8 Res; 

#define dma_buf_size 128
#define VOLTAGE_ADDR 0x01
#define CURRENT_ADDR 0x02

static u8 RxBuffer[dma_buf_size];
u8 recv_data[dma_buf_size];
u8 current_data[dma_buf_size];


//USART2???ڳ?ʼ??
void uart2_init(u32 bound)
{
    //GPIO?˿?????
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    DMA_InitTypeDef DMA_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//ʹ??USART2??GPIOAʱ??
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//ʹ??USART2??GPIOAʱ??



    //USART2_TX   GPIOA.9
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//????????????
    GPIO_Init(GPIOA, &GPIO_InitStructure);//??ʼ??GPIOA.9

    //USART2_RX	  GPIOA.10??ʼ??
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA3
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//????????
    GPIO_Init(GPIOA, &GPIO_InitStructure);//??ʼ??GPIOA.10  

    //Usart1 NVIC ????
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
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

    USART_Init(USART2, &USART_InitStructure); //??ʼ??????1
    //USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//???����ڽ????ж?
    USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);  //?? USART2 ??????
    USART_Cmd(USART2, ENABLE);                    //ʹ?ܴ???1 


    DMA_DeInit(DMA1_Channel6);
    DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)(&USART2->DR);    //??--->??
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
    DMA_Init(DMA1_Channel6, &DMA_InitStruct);

    DMA_Cmd(DMA1_Channel6, ENABLE);
    USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);              // ?? USART2??DMA
}
void request_voltage(void)
{
    u8 send_data[] = {VOLTAGE_ADDR, 0x03, 0x00, 0x2A, 0x00, 0x02, 0xE5, 0xC3};
    int i;
    int cnt = sizeof(send_data);
    for(i = 0; i < cnt; i++)
    {
        USART_SendData(USART2, send_data[i]);
    }
}
void request_current(void)
{
    u8 send_data[] = {CURRENT_ADDR, 0x03, 0x00, 0x2A, 0x00, 0x02, 0xE5, 0xC3};
    int i;
    int cnt = sizeof(send_data);
    for(i = 0; i < cnt; i++)
    {
        USART_SendData(USART2, send_data[i]);
    }
}
float byte2float(u8 *data, int size)
{
    float result;
    u8 *p;

    p = (u8 *)&result;

    *(p+1) = *(data+1);
    *(p+2) = *(data+2);
    *(p+3) = *(data+3);

    return result;
}
void parse_recv_data(void)
{
    if(recv_data[0] == VOLTAGE_ADDR)
    {
        float voltage = byte2float(&recv_data[3], 4);
        printf("recv voltage: %fV\r\n", voltage);
    }
    else if(recv_data[0] == CURRENT_ADDR)
    {
        float current = byte2float(&recv_data[3], 4);
        printf("recv current: %fA\r\n", current);
    }
    else
    {
        printf("recv unkown data from 485\r\n");
    }
}
void USART2_IRQHandler(void)                	//????1?жϷ???????
{
    extern u8 Res; 
    //Res =USART_ReceiveData(USART2);	//??ȡ???յ???????
    //USART_SendData(USART2, Res);//?򴮿?1???????? USART2_RX_BUF[t]
    uint8_t clear = clear;  // ?????????????????
    uint8_t RxCounter;
    int i;
    if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
    {
        DMA_Cmd(DMA1_Channel6, DISABLE);                // ??DMA,??DMA??

        clear = USART2->SR;
        clear = USART2->DR;

        //      RxCounter = BufferSize - DMA1_Channel6->CNDTR;//???????
        RxCounter = dma_buf_size - DMA_GetCurrDataCounter(DMA1_Channel6);//???????

        //      USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);

        // RxStatus = 1;   //???????
        USART_ClearITPendingBit(USART2, USART_IT_IDLE); // ??????

        memset(recv_data, 0, dma_buf_size);
        for(i = 0; i < RxCounter; i++)
        {
            recv_data[i] = RxBuffer[i];
        }
        parse_recv_data();
        DMA_SetCurrDataCounter(DMA1_Channel6, dma_buf_size);
        DMA_Cmd(DMA1_Channel6, ENABLE);
    }

}

