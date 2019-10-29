#include "control.h"
#include "filter.h"

u8 Flag_Target, Flag_Change;	//?????��
u8 temp1;			//???????
float Voltage_Count, Voltage_All;	//?????????????
float Gyro_K = 0.004;		//????????????
int j;
#define a_PARAMETER          (0.311f)	//(0.25f)
#define b_PARAMETER          (0.3075f)	//(0.225f)
/**************************************************************************
?????????��???????????
????????X Y Z ??????????��??
????  ?????
**************************************************************************/
void
Kinematic_Analysis(float Vx, float Vy, float Vz)
{
	Target_A = -Vx + Vy - Vz * (a_PARAMETER + b_PARAMETER);
	Target_B = +Vx + Vy - Vz * (a_PARAMETER + b_PARAMETER);
	Target_C = -Vx + Vy + Vz * (a_PARAMETER + b_PARAMETER);
	Target_D = +Vx + Vy + Vz * (a_PARAMETER + b_PARAMETER);
}

/**************************************************************************
????????????��????????????
????????X Y Z ????��??��??
????  ?????
**************************************************************************/
void
Kinematic_Analysis2(float Vx, float Vy, float Vz)
{
	Rate_A = -Vx + Vy - Vz * (a_PARAMETER + b_PARAMETER);
	Rate_B = +Vx + Vy - Vz * (a_PARAMETER + b_PARAMETER);
	Rate_C = -Vx + Vy + Vz * (a_PARAMETER + b_PARAMETER);
	Rate_D = +Vx + Vy + Vz * (a_PARAMETER + b_PARAMETER);
}

/**************************************************************************
????????????��???????????????
         5ms????��???MPU6050??INT???????
         ?????????????????????????
**************************************************************************/
int
EXTI15_10_IRQHandler(void)
{
	if (INT == 0) {
		EXTI->PR = 1 << 15;	//???LINE5????��???��
		Flag_Target = !Flag_Target;
		if (delay_flag == 1) {
			if (++delay_50 == 10)
				delay_50 = 0, delay_flag = 0;	//??????????50ms???????
		}
		if (Flag_Target == 1)	//5ms????????????????????
		{
			if (Usart_Flag == 0 && PS2_ON_Flag == 0
			    && Usart_ON_Flag == 1)
				memcpy(rxbuf, Urxbuf, 8 * sizeof (u8));	//?????????????????��?????????????
			Read_DMP();	//===???????
			Key();	//??Ǵ???��
			return 0;
		}		//===10ms??????��??????M??????????????????????????????
		Encoder_A = Read_Encoder(2) / 20;	//===????????????(????)
		Position_A += Encoder_A;	//===?????????
		Encoder_B = Read_Encoder(3) / 20;	//===????????????
		Position_B += Encoder_B;	//===?????????
		Encoder_C = -Read_Encoder(4) / 20;	//===????????????
		Position_C += Encoder_C;	//===?????????
		Encoder_D = -Read_Encoder(5) / 20;	//===????????????
		Position_D += Encoder_D;	//===?????????
		Read_DMP();	//===???????
		Led_Flash(100);	//===LED???;?????? 1s?????????????
		Voltage_All += Get_battery_volt();	//??��??????
		if (++Voltage_Count == 100)
			Voltage = Voltage_All / 100, Voltage_All = 0, Voltage_Count = 0;	//?????? ????????
		if (PS2_KEY == 4)
			PS2_ON_Flag = 1, CAN_ON_Flag = 0, Usart_ON_Flag =
			    0;
		if (CAN_ON_Flag == 1 || Usart_ON_Flag == 1
		    || PS2_ON_Flag == 1)
			CAN_N_Usart_Control();	//??????????CAN???????????????CAN????????????
		if (RC_Velocity > 0 && RC_Velocity < 15)
			RC_Velocity = 15;	//????????????????????

		if (Turn_Off(Voltage) == 0)	//===????????????????
		{
			if (Run_Flag == 0)	//?????
			{
				if (CAN_ON_Flag == 0 && Usart_ON_Flag == 0
				    && PS2_ON_Flag == 0)
					Get_RC(Run_Flag);	//===?????CAN?????��?????????????????
				Motor_A = Incremental_PI_A(Encoder_A, Target_A);	//===??????????????A????PWM
				Motor_B = Incremental_PI_B(Encoder_B, Target_B);	//===??????????????B????PWM
				Motor_C = Incremental_PI_C(Encoder_C, Target_C);	//===??????????????C????PWM
				Motor_D = Incremental_PI_D(Encoder_D, Target_D);	//===??????????????C????PWM
			} else	//��????
			{
				if (CAN_ON_Flag == 0 && Usart_ON_Flag == 0 && PS2_ON_Flag == 0)	//===?????CAN?????��???????????????????
				{
					if (Turn_Flag == 0)
						Flag_Direction =
						    click_RC();
					Get_RC(Run_Flag);
				}
				Motor_A = Position_PID_A(Position_A, Target_A) >> 8;	//��???????????????A??????????????
				Motor_B = Position_PID_B(Position_B, Target_B) >> 8;	//��???????????????B??????????????
				Motor_C = Position_PID_C(Position_C, Target_C) >> 8;	//��???????????????C??????????????
				Motor_D = Position_PID_D(Position_D, Target_D) >> 8;	//��???????????????C??????????????

				if (rxbuf[0] != 2)
					Count_Velocity();	//???????��?????????????��
				else
					Xianfu_Velocity(RC_Velocity,
							RC_Velocity,
							RC_Velocity,
							RC_Velocity);
				Motor_A = Incremental_PI_A(Encoder_A, -Motor_A);	//===??????????????A????PWM
				Motor_B = Incremental_PI_B(Encoder_B, -Motor_B);	//===??????????????B????PWM
				Motor_C = Incremental_PI_C(Encoder_C, -Motor_C);	//===??????????????C????PWM
				Motor_D = Incremental_PI_D(Encoder_D, -Motor_D);	//===??????????????C????PWM
			}
			Xianfu_Pwm(6900);	//===PWM???
			Set_Pwm(Motor_A, Motor_B, Motor_C, Motor_D);	//===?????PWM?????  
		}
	}
	return 0;
}

/**************************************************************************
??????????????PWM?????
????????PWM
????  ?????
**************************************************************************/
void
Set_Pwm(int motor_a, int motor_b, int motor_c, int motor_d)
{
	int siqu = 0;
	if (motor_a > 0)
		PWMA = motor_a + siqu, INA = 0;
	else
		PWMA = 7200 + motor_a - siqu, INA = 1;

	if (motor_b > 0)
		PWMB = 7200 - motor_b - siqu, INB = 1;
	else
		PWMB = -motor_b + siqu, INB = 0;

	if (motor_c > 0)
		PWMC = 7200 - motor_c - siqu, INC = 1;
	else
		PWMC = -motor_c + siqu, INC = 0;

	if (motor_d > 0)
		PWMD = motor_d + siqu, IND = 0;
	else
		PWMD = 7200 + motor_d - siqu, IND = 1;
}

/**************************************************************************
?????????????PWM??? 
???????????
????  ?????
**************************************************************************/
void
Xianfu_Pwm(int amplitude)
{
	if (Motor_A < -amplitude)
		Motor_A = -amplitude;
	if (Motor_A > amplitude)
		Motor_A = amplitude;
	if (Motor_B < -amplitude)
		Motor_B = -amplitude;
	if (Motor_B > amplitude)
		Motor_B = amplitude;
	if (Motor_C < -amplitude)
		Motor_C = -amplitude;
	if (Motor_C > amplitude)
		Motor_C = amplitude;
	if (Motor_D < -amplitude)
		Motor_D = -amplitude;
	if (Motor_D > amplitude)
		Motor_D = amplitude;
}

/**************************************************************************
?????????��??PID?????????????????
??????????????
????  ?????
**************************************************************************/
void
Xianfu_Velocity(int amplitude_A, int amplitude_B, int amplitude_C,
		int amplitude_D)
{
	if (Motor_A < -amplitude_A)
		Motor_A = -amplitude_A;	//��????????��?A????????????
	if (Motor_A > amplitude_A)
		Motor_A = amplitude_A;	//��????????��?A????????????
	if (Motor_B < -amplitude_B)
		Motor_B = -amplitude_B;	//��????????��?B????????????
	if (Motor_B > amplitude_B)
		Motor_B = amplitude_B;	//��????????��?B????????????
	if (Motor_C < -amplitude_C)
		Motor_C = -amplitude_C;	//��????????��?C????????????
	if (Motor_C > amplitude_C)
		Motor_C = amplitude_C;	//��????????��?C????????????
	if (Motor_D < -amplitude_D)
		Motor_D = -amplitude_D;	//��????????��?C????????????
	if (Motor_D > amplitude_D)
		Motor_D = amplitude_D;	//��????????��?C????????????
}

/**************************************************************************
????????????????��???????? 
??????????
????  ?????
**************************************************************************/
void
Key(void)
{
	u8 tmp;
	tmp = click_N_Double(100);
	if (tmp == 2)
		Flag_Show = !Flag_Show;	//????????????
}

/**************************************************************************
????????????????
???????????
????  ???1????  0??????
**************************************************************************/
u8
Turn_Off(int voltage)
{
	u8 temp;
	if (voltage < 2220)	//?????????22.2V?????
	{
		temp = 1;
		PWMA = 0;
		PWMB = 0;
		PWMC = 0;
		PWMD = 0;
		INA = 0;
		INB = 0;
		INC = 0;
		IND = 0;
	} else
		temp = 0;
	return temp;
}

/**************************************************************************
??????????????????
????????long int
????  ???unsigned int
**************************************************************************/
u32
myabs(long int a)
{
	u32 temp;
	if (a < 0)
		temp = -a;
	else
		temp = a;
	return temp;
}

/**************************************************************************
?????????????PI??????
???????????????????????????
????  ??????PWM
????????????PID??? 
pwm+=Kp[e??k??-e(k-1)]+Ki*e(k)+Kd[e(k)-2e(k-1)+e(k-2)]
e(k)??????????? 
e(k-1)????????��????  ??????? 
pwm???????????
????????????????????��????PI????
pwm+=Kp[e??k??-e(k-1)]+Ki*e(k)
**************************************************************************/
int
Incremental_PI_A(int Encoder, int Target)
{
	static int Bias, Pwm, Last_bias;
	Bias = Encoder - Target;	//???????
	Pwm += Velocity_KP * (Bias - Last_bias) + Velocity_KI * Bias;	//?????PI??????
	if (Pwm > 7200)
		Pwm = 7200;
	if (Pwm < -7200)
		Pwm = -7200;
	Last_bias = Bias;	//???????????? 
	return Pwm;		//???????
}

int
Incremental_PI_B(int Encoder, int Target)
{
	static int Bias, Pwm, Last_bias;
	Bias = Encoder - Target;	//???????
	Pwm += Velocity_KP * (Bias - Last_bias) + Velocity_KI * Bias;	//?????PI??????
	if (Pwm > 7200)
		Pwm = 7200;
	if (Pwm < -7200)
		Pwm = -7200;
	Last_bias = Bias;	//???????????? 
	return Pwm;		//???????
}

int
Incremental_PI_C(int Encoder, int Target)
{
	static int Bias, Pwm, Last_bias;
	Bias = Encoder - Target;	//???????
	Pwm += Velocity_KP * (Bias - Last_bias) + Velocity_KI * Bias;	//?????PI??????
	if (Pwm > 7200)
		Pwm = 7200;
	if (Pwm < -7200)
		Pwm = -7200;
	Last_bias = Bias;	//???????????? 
	return Pwm;		//???????
}

int
Incremental_PI_D(int Encoder, int Target)
{
	static int Bias, Pwm, Last_bias;
	Bias = Encoder - Target;	//???????
	Pwm += Velocity_KP * (Bias - Last_bias) + Velocity_KI * Bias;	//?????PI??????
	if (Pwm > 7200)
		Pwm = 7200;
	if (Pwm < -7200)
		Pwm = -7200;
	Last_bias = Bias;	//???????????? 
	return Pwm;		//???????
}

/**************************************************************************
?????????��???PID??????
??????????????????��??????????��??
????  ??????PWM
????��??????PID??? 
pwm=Kp*e(k)+Ki*??e(k)+Kd[e??k??-e(k-1)]
e(k)??????????? 
e(k-1)????????��????  
??e(k)????e(k)????????????????;????k?1,2,,k;
pwm???????
**************************************************************************/
int
Position_PID_A(int Encoder, int Target)
{
	static float Bias, Pwm, Integral_bias, Last_Bias;
	Bias = Encoder - Target;	//???????
	Integral_bias += Bias;	//??????????
	if (Integral_bias > 100000)
		Integral_bias = 10000;
	if (Integral_bias < -100000)
		Integral_bias = -10000;
	Pwm = Position_KP * Bias + Position_KI / 100 * Integral_bias + Position_KD * (Bias - Last_Bias);	//��???PID??????
	Last_Bias = Bias;	//???????????? 
	return Pwm;		//???????
}

int
Position_PID_B(int Encoder, int Target)
{
	static float Bias, Pwm, Integral_bias, Last_Bias;
	Bias = Encoder - Target;	//???????
	Integral_bias += Bias;	//??????????
	if (Integral_bias > 100000)
		Integral_bias = 10000;
	if (Integral_bias < -100000)
		Integral_bias = -10000;
	Pwm = Position_KP * Bias + Position_KI / 100 * Integral_bias + Position_KD * (Bias - Last_Bias);	//��???PID??????
	Last_Bias = Bias;	//???????????? 
	return Pwm;		//???????
}

int
Position_PID_C(int Encoder, int Target)
{
	static float Bias, Pwm, Integral_bias, Last_Bias;
	Bias = Encoder - Target;	//???????
	Integral_bias += Bias;	//??????????
	if (Integral_bias > 100000)
		Integral_bias = 10000;
	if (Integral_bias < -100000)
		Integral_bias = -10000;
	Pwm = Position_KP * Bias + Position_KI / 100 * Integral_bias + Position_KD * (Bias - Last_Bias);	//��???PID??????
	Last_Bias = Bias;	//???????????? 
	return Pwm;		//???????
}

int
Position_PID_D(int Encoder, int Target)
{
	static float Bias, Pwm, Integral_bias, Last_Bias;
	Bias = Encoder - Target;	//???????
	Integral_bias += Bias;	//??????????
	if (Integral_bias > 100000)
		Integral_bias = 10000;
	if (Integral_bias < -100000)
		Integral_bias = -10000;
	Pwm = Position_KP * Bias + Position_KI / 100 * Integral_bias + Position_KD * (Bias - Last_Bias);	//��???PID??????
	Last_Bias = Bias;	//???????????? 
	return Pwm;		//???????
}

/**************************************************************************
????????????????????��?????????
???????????????
????  ?????
**************************************************************************/
void
Get_RC(u8 mode)
{

	float step = 0.5;	//????????????????
	u8 Flag_Move = 1;
	if (mode == 0)		//???
	{
		switch (Flag_Direction)	//???????
		{
		case 1:
			Move_X = 0;
			Move_Y += step;
			Flag_Move = 1;
			break;
		case 2:
			Move_X += step;
			Move_Y += step;
			Flag_Move = 1;
			break;
		case 3:
			Move_X += step;
			Move_Y = 0;
			Flag_Move = 1;
			break;
		case 4:
			Move_X += step;
			Move_Y -= step;
			Flag_Move = 1;
			break;
		case 5:
			Move_X = 0;
			Move_Y -= step;
			Flag_Move = 1;
			break;
		case 6:
			Move_X -= step;
			Move_Y -= step;
			Flag_Move = 1;
			break;
		case 7:
			Move_X -= step;
			Move_Y = 0;
			Flag_Move = 1;
			break;
		case 8:
			Move_X -= step;
			Move_Y += step;
			Flag_Move = 1;
			break;
		default:
			Flag_Move = 0;
			Move_X = Move_X / 1.05;
			Move_Y = Move_Y / 1.05;
			break;
		}

		if (Flag_Move == 0)	//??????????????         ?????????????
		{
			if (Flag_Left == 1)
				Move_Z -= step, Gyro_K = 0;	//??????   
			else if (Flag_Right == 1)
				Move_Z += step, Gyro_K = 0;	//??????               
			else
				Move_Z = 0, Gyro_K = 0.004;	//??
		}
		if (Flag_Move == 1)
			Flag_Left = 0, Flag_Right = 0, Move_Z = 0;
		if (Move_X < -RC_Velocity)
			Move_X = -RC_Velocity;	//?????????
		if (Move_X > RC_Velocity)
			Move_X = RC_Velocity;
		if (Move_Y < -RC_Velocity)
			Move_Y = -RC_Velocity;
		if (Move_Y > RC_Velocity)
			Move_Y = RC_Velocity;
		if (Move_Z < -RC_Velocity)
			Move_Z = -RC_Velocity;
		if (Move_Z > RC_Velocity)
			Move_Z = RC_Velocity;

	} else if (mode == 1)	//��????
	{
		switch (Flag_Direction)	//???????
		{
		case 1:
			Move_Y += RC_Position;
			Flag_Change = 1;
			break;
		case 2:
			Move_X += RC_Position;
			Flag_Change = 2;
			Move_Y += RC_Position;
			break;
		case 3:
			Move_X += RC_Position;
			Flag_Change = 3;
			break;
		case 4:
			Move_X += RC_Position;
			Flag_Change = 4;
			Move_Y -= RC_Position;
			break;
		case 5:
			Move_Y -= RC_Position;
			Flag_Change = 5;
			break;
		case 6:
			Move_X -= RC_Position;
			Flag_Change = 6;
			Move_Y -= RC_Position;
			break;
		case 7:
			Move_X -= RC_Position;
			Flag_Change = 7;
			break;
		case 8:
			Move_X -= RC_Position;
			Flag_Change = 8;
			Move_Y += RC_Position;
			break;
		case 9:
			Move_Z -= RC_Position;
			Flag_Change = 9;
			break;
		case 10:
			Move_Z += RC_Position;
			Flag_Change = 10;
			break;
		default:
			break;
		}
	}
	Kinematic_Analysis(Move_X, Move_Y, Move_Z);	//?????????????????????????
}

/**************************************************************************
???????????????��??????????????
??????????
????  ?????
**************************************************************************/
void
Count_Velocity(void)
{
	static double Last_Target_X, Last_Target_Y, Last_Target_Z, Divider;
	double Bias_X, Bias_Y, Bias_Z;
	Bias_X = (Move_X - Last_Target_X);	//??X??��????
	Bias_Y = (Move_Y - Last_Target_Y);	//??Y??��????
	Bias_Z = (Move_Z - Last_Target_Z);	//??Z??��????
	if (Bias_X != 0 || Bias_Y != 0 || Bias_Z != 0)
		Divider =
		    sqrt(Bias_X * Bias_X + Bias_Y * Bias_Y +
			 Bias_Z * Bias_Z);
	if (Bias_X != 0 || Bias_Y != 0 || Bias_Z != 0)
		Kinematic_Analysis2(Bias_X, Bias_Y, Bias_Z);

	Xianfu_Velocity(RC_Velocity * myabs(Rate_A) / Divider,
			RC_Velocity * myabs(Rate_B) / Divider,
			RC_Velocity * myabs(Rate_C) / Divider,
			RC_Velocity * myabs(Rate_D) / Divider);
	Last_Target_X = Move_X;	//????X??????��?��??????????????
	Last_Target_Y = Move_Y;	//????Y??????��?��??????????????
	Last_Target_Z = Move_Z;	//????Z??????��?��??????????????
}

/**************************************************************************
?????????????CAN???????????????��???
??????????
????  ?????
**************************************************************************/
void
CAN_N_Usart_Control(void)
{
	int flag_1, flag_2, flag_3, flag_4;
	int Yuzhi = 20;
	int LX, LY, RX;
	if (Run_Flag == 0)	//?????
	{
		if (CAN_ON_Flag == 1 || Usart_ON_Flag == 1) {
			if (rxbuf[0] == 1) {
				if ((rxbuf[7] & 0x04) == 0)
					flag_1 = 1;
				else
					flag_1 = -1;	//???????��
				if ((rxbuf[7] & 0x02) == 0)
					flag_2 = 1;
				else
					flag_2 = -1;	//???????��
				if ((rxbuf[7] & 0x01) == 0)
					flag_3 = 1;
				else
					flag_3 = -1;	//???????��
				Move_X =
				    flag_1 * (rxbuf[1] * 256 + rxbuf[2]);
				Move_Y =
				    flag_2 * (rxbuf[3] * 256 + rxbuf[4]);
				Move_Z =
				    flag_3 * (rxbuf[5] * 256 + rxbuf[6]);
				Kinematic_Analysis(Move_X, Move_Y, Move_Z);	//????????????
				Gyro_K = 0;
			}
			if (rxbuf[0] == 2) {
				if ((rxbuf[7] & 0x08) == 0)
					flag_1 = 1;
				else
					flag_1 = -1;	//???????��
				if ((rxbuf[7] & 0x04) == 0)
					flag_2 = 1;
				else
					flag_2 = -1;	//???????��
				if ((rxbuf[7] & 0x02) == 0)
					flag_3 = 1;
				else
					flag_3 = -1;	//???????��
				if ((rxbuf[7] & 0x01) == 0)
					flag_4 = 1;
				else
					flag_4 = -1;	//???????��

				Target_A = flag_1 * rxbuf[1];
				Target_B = flag_2 * rxbuf[2];
				Target_C = flag_3 * rxbuf[3];
				Target_D = flag_4 * rxbuf[4];
			}
		} else if (PS2_ON_Flag == 1) {
			LX = PS2_LX - 128;
			LY = PS2_LY - 128;
			RX = PS2_RX - 128;
			if (LX > -Yuzhi && LX < Yuzhi)
				LX = 0;
			if (LY > -Yuzhi && LY < Yuzhi)
				LY = 0;
			if (RX > -Yuzhi && RX < Yuzhi)
				RX = 0;
			Move_X = LX * RC_Velocity / 200;
			Move_Y = -LY * RC_Velocity / 200;
			Move_Z = -RX * RC_Velocity / 200;
//               if(Move_X<-RC_Velocity) Move_X=-RC_Velocity;      //?????????
//              if(Move_X>RC_Velocity)  Move_X=RC_Velocity;          
//              if(Move_Y<-RC_Velocity) Move_Y=-RC_Velocity;    
//              if(Move_Y>RC_Velocity)  Move_Y=RC_Velocity;      
//              if(Move_Z<-RC_Velocity) Move_Z=-RC_Velocity;    
//              if(Move_Z>RC_Velocity)  Move_Z=RC_Velocity;      
			Kinematic_Analysis(Move_X, Move_Y, Move_Z), Gyro_K = 0;	//???????????? 
		}

	} else if (Run_Flag == 1)	//��????
	{
		if (rxbuf[0] == 1) {
			if ((rxbuf[7] & 0x04) == 0)
				flag_1 = 1;
			else
				flag_1 = -1;	//???????��
			if ((rxbuf[7] & 0x02) == 0)
				flag_2 = 1;
			else
				flag_2 = -1;	//???????��
			if ((rxbuf[7] & 0x01) == 0)
				flag_3 = 1;
			else
				flag_3 = -1;	//???????��
			Move_X = flag_1 * (rxbuf[1] * 256 + rxbuf[2]);
			Move_Y = flag_2 * (rxbuf[3] * 256 + rxbuf[4]);
			Move_Z = flag_3 * (rxbuf[5] * 256 + rxbuf[6]);
			Kinematic_Analysis(Move_X, Move_Y, Move_Z);	//????????????
			Gyro_K = 0;
		}
	}
}
