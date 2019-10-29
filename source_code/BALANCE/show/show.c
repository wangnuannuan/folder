#include "show.h"

unsigned char i;		//��������
unsigned char Send_Count;	//������Ҫ���͵����ݸ���
float Vol;
/**************************************************************************
�������ܣ�OLED��ʾ
��ڲ�������
����  ֵ����
**************************************************************************/
void
oled_show(void)
{
	//=============��1����ʾ3��Ƕ�===============//    
	OLED_ShowString(0, 0, "X:");
	if (Pitch < 0)
		OLED_ShowNumber(15, 0, Pitch + 360, 3, 12);
	else
		OLED_ShowNumber(15, 0, Pitch, 3, 12);

	OLED_ShowString(40, 0, "Y:");
	if (Roll < 0)
		OLED_ShowNumber(55, 0, Roll + 360, 3, 12);
	else
		OLED_ShowNumber(55, 0, Roll, 3, 12);

	OLED_ShowString(80, 0, "Z:");
	if (Yaw < 0)
		OLED_ShowNumber(95, 0, Yaw + 360, 3, 12);
	else
		OLED_ShowNumber(95, 0, Yaw, 3, 12);

	if (Run_Flag == 0)	//�ٶ�ģʽ
	{
		//=============��ʾ���A��״̬=======================// 
		if (Target_A < 0)
			OLED_ShowString(00, 10, "-"),
			    OLED_ShowNumber(15, 10, -Target_A, 5, 12);
		else
			OLED_ShowString(0, 10, "+"),
			    OLED_ShowNumber(15, 10, Target_A, 5, 12);

		if (Encoder_A < 0)
			OLED_ShowString(80, 10, "-"),
			    OLED_ShowNumber(95, 10, -Encoder_A, 4, 12);
		else
			OLED_ShowString(80, 10, "+"),
			    OLED_ShowNumber(95, 10, Encoder_A, 4, 12);
		//=============��ʾ���B��״̬=======================// 
		if (Target_B < 0)
			OLED_ShowString(00, 20, "-"),
			    OLED_ShowNumber(15, 20, -Target_B, 5, 12);
		else
			OLED_ShowString(0, 20, "+"),
			    OLED_ShowNumber(15, 20, Target_B, 5, 12);

		if (Encoder_B < 0)
			OLED_ShowString(80, 20, "-"),
			    OLED_ShowNumber(95, 20, -Encoder_B, 4, 12);
		else
			OLED_ShowString(80, 20, "+"),
			    OLED_ShowNumber(95, 20, Encoder_B, 4, 12);
		//=============��ʾ���C��״̬=======================//        
		if (Target_C < 0)
			OLED_ShowString(00, 30, "-"),
			    OLED_ShowNumber(15, 30, -Target_C, 5, 12);
		else
			OLED_ShowString(0, 30, "+"),
			    OLED_ShowNumber(15, 30, Target_C, 5, 12);

		if (Encoder_C < 0)
			OLED_ShowString(80, 30, "-"),
			    OLED_ShowNumber(95, 30, -Encoder_C, 4, 12);
		else
			OLED_ShowString(80, 30, "+"),
			    OLED_ShowNumber(95, 30, Encoder_C, 4, 12);
		//=============��ʾ���D��״̬=======================// 
		if (Target_D < 0)
			OLED_ShowString(00, 40, "-"),
			    OLED_ShowNumber(15, 40, -Target_D, 5, 12);
		else
			OLED_ShowString(0, 40, "+"),
			    OLED_ShowNumber(15, 40, Target_D, 5, 12);

		if (Encoder_D < 0)
			OLED_ShowString(80, 40, "-"),
			    OLED_ShowNumber(95, 40, -Encoder_D, 4, 12);
		else
			OLED_ShowString(80, 40, "+"),
			    OLED_ShowNumber(95, 40, Encoder_D, 4, 12);
	} else if (Run_Flag == 1)	//λ��ģʽ
	{
		//=============��ʾ���A��״̬=======================//     
		if (Target_A < 0)
			OLED_ShowString(00, 10, "-"),
			    OLED_ShowNumber(15, 10, -Target_A, 5, 12);
		else
			OLED_ShowString(0, 10, "+"),
			    OLED_ShowNumber(15, 10, Target_A, 5, 12);

		if (Position_A < 0)
			OLED_ShowString(80, 10, "-"),
			    OLED_ShowNumber(95, 10, -Position_A, 4, 12);
		else
			OLED_ShowString(80, 10, "+"),
			    OLED_ShowNumber(95, 10, Position_A, 4, 12);
		//=============��ʾ���B��״̬=======================// 
		if (Target_B < 0)
			OLED_ShowString(00, 20, "-"),
			    OLED_ShowNumber(15, 20, -Target_B, 5, 12);
		else
			OLED_ShowString(0, 20, "+"),
			    OLED_ShowNumber(15, 20, Target_B, 5, 12);

		if (Position_B < 0)
			OLED_ShowString(80, 20, "-"),
			    OLED_ShowNumber(95, 20, -Position_B, 4, 12);
		else
			OLED_ShowString(80, 20, "+"),
			    OLED_ShowNumber(95, 20, Position_B, 4, 12);
		//=============��ʾ���C��״̬=======================//        
		if (Target_C < 0)
			OLED_ShowString(00, 30, "-"),
			    OLED_ShowNumber(15, 30, -Target_C, 5, 12);
		else
			OLED_ShowString(0, 30, "+"),
			    OLED_ShowNumber(15, 30, Target_C, 5, 12);

		if (Position_C < 0)
			OLED_ShowString(80, 30, "-"),
			    OLED_ShowNumber(95, 30, -Position_C, 4, 12);
		else
			OLED_ShowString(80, 30, "+"),
			    OLED_ShowNumber(95, 30, Position_C, 4, 12);
		//=============��ʾ���D��״̬=======================// 
		if (Target_D < 0)
			OLED_ShowString(00, 40, "-"),
			    OLED_ShowNumber(15, 40, -Target_D, 5, 12);
		else
			OLED_ShowString(0, 40, "+"),
			    OLED_ShowNumber(15, 40, Target_D, 5, 12);

		if (Position_D < 0)
			OLED_ShowString(80, 40, "-"),
			    OLED_ShowNumber(95, 40, -Position_D, 4, 12);
		else
			OLED_ShowString(80, 40, "+"),
			    OLED_ShowNumber(95, 40, Position_D, 4, 12);
	}
	//=============��������ʾ��ѹ=======================//
	if (Run_Flag == 0)
		OLED_ShowString(00, 50, "VELOCITY");
	else if (Run_Flag == 1)
		OLED_ShowString(00, 50, "POSITION");
	OLED_ShowString(88, 50, ".");
	OLED_ShowString(110, 50, "V");
	OLED_ShowNumber(75, 50, Voltage / 100, 2, 12);
	OLED_ShowNumber(98, 50, Voltage % 100, 2, 12);
	if (Voltage % 100 < 10)
		OLED_ShowNumber(92, 50, 0, 2, 12);
	//=============ˢ��=======================//
	OLED_Refresh_Gram();
}

/**************************************************************************
�������ܣ���APP��������
��ڲ�������
����  ֵ����
**************************************************************************/
void
APP_Show(void)
{
	static u8 flag;
	int app_2, app_3, app_4;
	app_4 = (Voltage / 2 - 1110) * 2 / 3;
	if (app_4 > 100)
		app_4 = 100;	//�Ե�ѹ���ݽ��д���
	app_2 = Move_X * 0.7;
	if (app_2 < 0)
		app_2 = -app_2;	//�Ա��������ݾ������ݴ�������ͼ�λ�
	app_3 = Move_Y * 0.7;
	if (app_3 < 0)
		app_3 = -app_3;
	if (Run_Flag == 1)
		app_2 = 0, app_3 = 0;
	flag = !flag;
	if (PID_Send == 1)	//����PID����
	{
		printf("{C%d:%d:%d:%d:%d:%d:%d:%d:%d}$", (int) RC_Velocity, (int) RC_Position, (int) Position_KP, (int) Position_KI, (int) Position_KD, (int) Velocity_KP, (int) Velocity_KI, 0, 0);	//��ӡ��APP����     
		PID_Send = 0;
	} else if (flag == 0)	// 
		printf("{A%d:%d:%d:%d}$", (u8) app_2, (u8) app_3, app_4, 0);	//��ӡ��APP����
	else
		printf("{B%d:%d:%d:%d}$", (int) Pitch, (int) Roll, (int) Yaw, app_4);	//��ӡ��APP���� ��ʾ����
}

void
DataScope(void)
{
	if (Run_Flag == 0)	//�ٶ�ģʽ
	{
		DataScope_Get_Channel_Data(Encoder_A, 1);	//��ʾĿ��ֵ
		DataScope_Get_Channel_Data(Encoder_B, 2);	//��ʾʵ��ֵ������PID��������  
		DataScope_Get_Channel_Data(Encoder_C, 3);
		DataScope_Get_Channel_Data(Encoder_D, 4);

	} else			//λ��ģʽ
	{
		DataScope_Get_Channel_Data(Position_A, 1);
		DataScope_Get_Channel_Data(Position_B, 2);
		DataScope_Get_Channel_Data(Position_C, 3);
		DataScope_Get_Channel_Data(Position_D, 4);
	}

//              DataScope_Get_Channel_Data(0, 9 );  
//              DataScope_Get_Channel_Data( 0 , 10);
	Send_Count = DataScope_Data_Generate(4);
	for (i = 0; i < Send_Count; i++) {
		while ((USART1->SR & 0X40) == 0) ;
		USART1->DR = DataScope_OutPut_Buffer[i];
	}
}
