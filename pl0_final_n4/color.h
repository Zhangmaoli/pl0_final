#include <stdio.h>
#include <windows.h>
#define COLOR_RED 12
#define COLOR_WHITE 7
#define COLOR_BLACK 7

#define COLOR_GREEN 10
#define COLOR_BLUE 9
#define COLOR_PURPLE 13
#define COLOR_YELLOW 14
#define COLOR_BRIGHT 15

/*��ɫ��Ӧֵ�� 
����0=��ɫ                8=��ɫ����
  �� 1=��ɫ                9=����ɫ                ����                        
����2=��ɫ                10=����ɫ       0xa        ����
����3=����ɫ          11=��ǳ��ɫ        0xb��
����4=��ɫ                12=����ɫ        0xc����
����5=��ɫ                13=����ɫ        0xd        ����
����6=��ɫ                14=����ɫ        0xe        ����
����7=��ɫ                15=����ɫ        0xf 
����Ҳ���԰���Щֵ���óɳ�����
*/
void color(const unsigned short color1)
{        /*���޸ı�0-15����ɫ;�����0-15��ôʵ��������ɫ   ��Ϊ�������15����ĸı�����ı�����ɫ��*/
        if(color1>=0&&color1<=15 || 1)
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color1);
        /*�������0-15�ķ�Χ��ɫ����ô��ΪĬ�ϵ���ɫ��ɫ��*/
    else
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}
void printColor()
{
	int i;
	for (i=0;i<32;i++){
		printf("%d ",i);
	}
	printf("\n");
}
