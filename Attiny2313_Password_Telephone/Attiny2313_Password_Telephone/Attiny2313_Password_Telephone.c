/*
 * Attiny2313_Password_Telephone.c
 *
 * Created: 13.10.2015 13:23:33
 *  Author: Vic
 */ 

#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>

/*==========���� ������==========*/
//���� ������
#define OUT_DDR DDRD
#define OUT_PORT PORTD
#define OUT_SIG_PIN PD0
//�������������
#define INIT_OUT_PORT OUT_DDR = _BV(OUT_SIG_PIN);
//������� ������������
#define OUT_SIG_ON OUT_PORT |= _BV(OUT_SIG_PIN)
#define OUT_SIG_OFF OUT_PORT &= ~_BV(OUT_SIG_PIN)
/*==========���� ������==========*/

/*==========������==========*/
//����� �����
#define BTNS_DDR DDRB
#define BTNS_PORT PORTB
#define BTNS_PIN PINB

//���������� ������
#define BTN1 PB0 //2
#define BTN2 PB1 //3
#define BTN3 PB2 //4
#define BTN4 PB3 //8
#define BTN5 PB4 //9
//�� ���������� ������
#define BTN6 PB5 // "������"

#define SET_BTNS_ON _BV(BTN1)|_BV(BTN2)|_BV(BTN3)|_BV(BTN4)|_BV(BTN5)|_BV(BTN6);//��������� � ��������� HIGH
/*==========������==========*/

//������ ������
#define MAXBTN 6//������ ������
//int CorrectButtons[MAXBTN] = {BTN3,BTN2,BTN1,BTN2,BTN5,BTN4};//������ 431375 (����)
int CorrectButtons[MAXBTN] = {BTN3,BTN4,BTN2,BTN4,BTN1,BTN5};//������ 483829 (1408)
int NotCorrectButton = BTN6;//� ��� "������" ������
int ToCheckNow=0;//������, ������� �� ������ ���������
//������� ������������� ������ �� ����
void InitInput();
//������� ������ ������
void ReadBtns();
//������� ��������� ������� ������
void ReadHandle();

int main(void)
{
	InitInput();//�������������� ������
	INIT_OUT_PORT;//�������������� �����
    while(1)
    {
		ReadHandle();//��������� �����
    }
}

void ReadHandle()
{
	if(ToCheckNow==MAXBTN)
	{
		OUT_SIG_ON;
		for(int i=0;i<15;i++){_delay_ms(100);}
		OUT_SIG_OFF;
		ToCheckNow=0;
	}
	else
	{
		ReadBtns();
	}
}

void InitInput()
{
	BTNS_DDR |= SET_BTNS_ON;
	BTNS_PORT |= SET_BTNS_ON;
}

void ReadBtns()
{
	//�������� ��� ������ �� �����
	for(int i=0;i!=MAXBTN;i++)
	{
		//���� ������� ������ == ���, ��� ����� ������
		if(i==ToCheckNow)
		{
			//��������� �� ���������
			if(bit_is_set(BTNS_PIN,CorrectButtons[ToCheckNow]))
			{
				//���� ���� �� ������� �������� ������
				while(bit_is_set(BTNS_PIN,CorrectButtons[ToCheckNow]));
				ToCheckNow++;
			}
		}
		else
		{
			//������� �� �� ��������� ������� ������
			if(bit_is_set(BTNS_PIN,CorrectButtons[ToCheckNow]))
			{
				//���� ���� �� ������� �������� ������
				while(bit_is_set(BTNS_PIN,CorrectButtons[ToCheckNow]));
				ToCheckNow=0;
			}
			//������� �� �� ���������� ������
			else if(bit_is_set(BTNS_PIN,BTN6))
			{
				while(bit_is_set(BTNS_PIN,BTN6));
				ToCheckNow=0;
			}
		}
	}
}
