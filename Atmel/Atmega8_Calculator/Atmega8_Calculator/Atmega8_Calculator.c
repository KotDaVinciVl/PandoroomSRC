/*
��������� ������:
	LOW:E4
	��� �������� ������.
	
������������ ������:
	LCD1602 - �������
	���������� ��������� 4�4
	74HC595 - ��������� �������
	Atmega8 - ������
*/

#define F_CPU 8000000

#include "KEYPAD.h"
#include "LCD1602.h"

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdbool.h>

/*=======�������======*/
/**/#define CLEAR_ALL_DATA for(int i=0;i<3;i++){CALCULATION_DATA[i]=0;};MODE=0;LINE_L=0;DOT_COUNTER=0.1;AFTER_DOT=false;LCD_CLEAR;//������� ������
/**/float CALCULATION_DATA[3] = {0.0,0.0,0.0};//����� �� ������� ������ ����������, ����� �� � ���� ������ ��������.
/**/uint8_t OPERATION_VAL=0;//�������� */+-
/**/uint8_t MODE=0;//�����, ��� switch � ������� calculation.
/**/bool AFTER_DOT=false;//�� ��� ������ ����� ����� �����?
/**/float DOT_COUNTER=0.1;//���������� 0.�
/**/char RESULT_CH[15];//���������� ��������� � ������� char
/**/void calculation(char ch);//������� �������
/*=======�������======*/

/*=========1602LCD============*/
uint8_t LINE_L=0;
#define LCD_GOTO_SECOND_LINE lcd_write_cmd(0b11000000);

#define PRINT_INPUT_A lcd_write_str("[A]");LCD_GOTO_SECOND_LINE;
#define PRINT_INPUT_B lcd_write_str("[B]");LCD_GOTO_SECOND_LINE;
#define PRINT_INPUT_DOT lcd_write_str("[INPUT DOT]");LCD_GOTO_SECOND_LINE;

#define LCD_RESET_CURSOR lcd_write_cmd(0x2);
#define LCD_CLEAR lcd_write_cmd(0x1);_delay_ms(100);LINE_L=0;
#define LCD_BACKSPACE lcd_write_cmd(0x10);lcd_write_char(0x02);lcd_write_cmd(0x10);
#define LINE_L_MINUS LINE_L--;if(LINE_L<0){LINE_L=0;}
#define LINE_L_PLUS LINE_L++;if(LINE_L>15){LCD_CLEAR;}
/*=========1602LCD============*/

int main(void)
{
	lcd_init();//�������������� LCD � ����������� ��������� �������
	keypad_init();//����������� ��������� ����������
	PRINT_INPUT_A;
	char TempKeypadVar;//��������� ���������� ��� �������� ���������� ����� � ��������� ����������.
	while(1)
	{
		TempKeypadVar = keypad_read();//��������� �������
		if(TempKeypadVar!='_')//���� keypad_read ��������� ��� �� ����� ������� "_"
		{
			calculation(TempKeypadVar);//�������� � �����.
		}
	}
}
/*==========�������===========*/
void calculation(char ch)
{
	LINE_L_PLUS;//������� ����, ������� �� �����
	lcd_write_char(ch);//����� ������ �� �����
	if(MODE!=2 && ch=='.')
	{
		AFTER_DOT=true;
	}
	else if(MODE==2 && ch=='.')
	{
		CLEAR_ALL_DATA;
		PRINT_INPUT_A;
	}
	else if(ch=='+'||ch=='-'||ch=='/'||ch=='*')//���� ��������
	{
		LCD_CLEAR;//������� �������
		OPERATION_VAL = ch;//�������� ��������� "��������" � ����������
		MODE=1;//��������� � ������ ���������� ����� B
		DOT_COUNTER=0.1;//��������
		AFTER_DOT=false;//��������
		PRINT_INPUT_B;
	}
	else if(MODE>0 && ch == '=')
	{
		switch(OPERATION_VAL)
		{
		case '-':
			CALCULATION_DATA[2] = CALCULATION_DATA[0]-CALCULATION_DATA[1];
			break;
		case '+':
			CALCULATION_DATA[2] = CALCULATION_DATA[0]+CALCULATION_DATA[1];
			break;
		case '/':
			CALCULATION_DATA[2] = CALCULATION_DATA[0]/CALCULATION_DATA[1];
			break;
		case '*':
			CALCULATION_DATA[2] = CALCULATION_DATA[0]*CALCULATION_DATA[1];
			break;
		}
		LCD_CLEAR;
		PRINT_INPUT_DOT;
		sprintf(RESULT_CH,"%f",CALCULATION_DATA[2]);
		lcd_write_str(RESULT_CH);
		MODE=2;			
	}
	else//��� ������.
	{
		if(AFTER_DOT==false)//��������� ��� ���� ����� ������.
		{
			CALCULATION_DATA[MODE] = CALCULATION_DATA[MODE]*10+(ch-'0');
		}
		else//�������� ����� �����.
		{
			CALCULATION_DATA[MODE] = (CALCULATION_DATA[MODE])+((ch-'0')*DOT_COUNTER);
			DOT_COUNTER=DOT_COUNTER*0.10;
		}
	}
}
/*==========�������===========*/