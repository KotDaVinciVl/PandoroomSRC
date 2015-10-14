/*
 * ������� ���� ��� ��������������� ����������.
 * TODO: ����������� � ������� � ����
 * Program Memory Usage 	:	886 bytes   43,3 % Full
 * Data Memory Usage 		:	30 bytes   23,4 % Full
 * �����: ���������� ������
 */ 
#define F_CPU 8000000
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//�������� ��� ������ "�����" �� �������������� �������
//NODOT -> ��� �����
//DOT -> � ������
char SEGMENT_NODOT[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F};
char SEGMENT_DOT[] = {0xBF, 0x86, 0xDB, 0xCF, 0xE6, 0xED, 0xFD, 0x87, 0xFF, 0xEF};

//======�������======//
void setup_output();//��������� �������� ������
void setup_input();//��������� ������ �����(������ ��������� �������)
void show_number(int num,int dig);//����� ����� �� �������
void check_time();//�������, ����� ������� �� ���� ������ 60, ������ ������ 60 � ���� ������ 24
void ReadInput();//������ ����� � ������

int i=0;//����� �� ��� ���?
int ms=0;//���� ��� ������, ����� ������ 1000, ����� +1 �������
int mode=0;//����� �����
int time_seconds=0;//�������
int time_minutes=0;//������
int time_hr=0;//����

//��� ��������
#define BTN_DDR DDRD
#define BTN_PORT PORTD
#define BTN_PIN PIND
#define BTN0 PD0
#define BTN1 PD1
#define BTN2 PD2

#define SEGMENT_DDR DDRB
#define SEGMENT_PORT PORTB
#define DIGIT_DDR DDRD
#define DIGIT_PORT PORTD

#define DIG_0 PD5
#define DIG_1 PD6
#define DIG_2 PD3
#define DIG_3 PD4

ISR(TIMER0_OVF_vect)//���������� �� �������, ��� ������ �������
{
	ms++;
	i++;
	check_time();
	//show_number(time_minutes%10,DIG_0);
	//show_number(time_minutes/10,DIG_1);
	//show_number(time_hr%10,DIG_2);
	//show_number(time_hr/10,DIG_3);
}
ISR (TIMER1_OVF_vect)//���������� �� ������� ��� ����� �������
{
	if(ms>1000)
	{
		time_seconds++;
		ms=0;
	}
}

int main(void)
{
	//�������� �������
	TIMSK = 1 << TOIE0 | 1 << TOIE1;
	TCCR1B = 1 << CS01;
	TCCR0B = 1 << CS00;
	//��������� ����������
	sei();
	setup_output();//����������� ���������� �������
	setup_input();//����������� ������ �����
	show_number(0,DIG_0);
    while(1)
    {
		ReadInput();//���� � ��� � ���� ��������� ����� � ������� ����� ������ ��������� ������
    }
}
void setup_output()//��������� ������
{
	SEGMENT_DDR = 0xFF;//��� ����� ��������� �� �����
	DIGIT_DDR |= (1<<DIG_0)|(1<<DIG_1)|(1<<DIG_2)|(1<<DIG_3);//��� �� ��� ����� �������� �� �����
	DIGIT_PORT = 0xFF;//��������� ��� �������
}
void setup_input()//����������� ������ ��� �����
{
	BTN_DDR |= (0<<BTN0)|(0<<BTN1)|(0<<BTN2);
	BTN_PORT |= (1<<BTN0)|(1<<BTN1)|(1<<BTN2);
}
void show_number(int num,int dig)//����� ����� �� ��������
{
	SEGMENT_PORT = 0xFF;
	DIGIT_PORT = (1<<BTN0)|(1<<BTN1)|(1<<BTN2);
	DIGIT_PORT &= ~(1<<dig);
	if(dig==DIG_2)
	{
		SEGMENT_PORT = SEGMENT_DOT[num];//~SEGMENT_DOT[num];
		
	}
	else
	{
		SEGMENT_PORT = SEGMENT_NODOT[num];//~SEGMENT_NODOT[num];
	}
	DIGIT_PORT |= _BV(dig);
}
void check_time()//�������� ������������ �������
{
	if(time_seconds>59)
	{
		time_seconds=0;
		time_minutes++;
	}
	else if(time_seconds<0)
	{
		time_seconds=0;
	}
	if(time_minutes>59)
	{
		time_minutes=0;
		time_hr++;
	}
	else if(time_minutes<0)
	{
		time_minutes=0;
	}
	if(time_hr>23)
	{
		time_hr=0;
		time_seconds=0;
		time_minutes=0;
	}
	else if(time_hr<0)
	{
		time_hr=0;
	}
	
}
void ReadInput()//������ �����
{
	switch(mode)//��� ������ +- ������ � +- ����
	{
		case 0:
			if(bit_is_clear(BTN_PIN,BTN0)==1)//������ 0 ������
			{
				while(bit_is_clear(BTN_PIN,BTN0)==1);//���, ���� �������� ������
				time_minutes++;//+1 ������
			}
			else if(bit_is_clear(BTN_PIN,BTN1)==1)//������ 1 ������
			{
				while(bit_is_clear(BTN_PIN,BTN1)==1);//���, ���� �������� ������
				time_minutes--;//-1 ������
			}
			else if(bit_is_clear(BTN_PIN,BTN2)==1)//������ 2 ������
			{
				while(bit_is_clear(BTN_PIN,BTN2)==1);//���, ���� �������� ������
				mode=1;//����������� �����
			}
		break;
		case 1://��������� ������� case, ������ +- ������ ����� +- ���
			if(bit_is_clear(BTN_PIN,BTN0)==1)
			{
				while(bit_is_clear(BTN_PIN,BTN0)==1);
				time_hr++;
			}
			else if(bit_is_clear(BTN_PIN,BTN1)==1)
			{
				while(bit_is_clear(BTN_PIN,BTN1)==1);
				time_hr--;
			}
			else if(bit_is_clear(BTN_PIN,BTN2)==1)
			{
				while(bit_is_clear(BTN_PIN,BTN2)==1);
				mode=0;
			}
		break;
	}
}