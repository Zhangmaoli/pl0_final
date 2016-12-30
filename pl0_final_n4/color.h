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

/*颜色对应值： 
　　0=黑色                8=灰色　　
  　 1=蓝色                9=淡蓝色                　　                        
　　2=绿色                10=淡绿色       0xa        　　
　　3=湖蓝色          11=淡浅绿色        0xb　
　　4=红色                12=淡红色        0xc　　
　　5=紫色                13=淡紫色        0xd        　　
　　6=黄色                14=淡黄色        0xe        　　
　　7=白色                15=亮白色        0xf 
　　也可以吧这些值设置成常量。
*/
void color(const unsigned short color1)
{        /*仅限改变0-15的颜色;如果在0-15那么实现他的颜色   因为如果超过15后面的改变的是文本背景色。*/
        if(color1>=0&&color1<=15 || 1)
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color1);
        /*如果不在0-15的范围颜色，那么改为默认的颜色白色；*/
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
