#include "led.h"
/**************************************************************************
�������ܣ�LED�ӿڳ�ʼ��
��ڲ������� 
����  ֵ����
��   �ţ� PB.13
**************************************************************************/
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//ʹ�ܶ˿�ʱ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				//�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//50M
	GPIO_Init(GPIOB, &GPIO_InitStructure);					//�����趨������ʼ��GPIOB
}

/**************************************************************************
�������ܣ�LED��˸
��ڲ�������˸Ƶ�� 
����  ֵ����
**************************************************************************/
void Led_Flash(u16 time)
{
    static int temp;
    if (0 == time)
        LED = 0;
    else if (++temp == time)
    {
        LED = ~LED;
        temp = 0;
    }
}
