#include <reg52.h>
typedef unsigned char uint8;
typedef unsigned int  uint16;
void showNum(uint16 num);
void keyAction(uint8 keyCode);
void init();

/**********************************
				函数名称：main
				函数功能：加法计算器
				输入：无
				返回：无
				
**********************************/

void main()
{
	
}

/*********************************
					中断服务函数
					函数功能：矩阵按键消痘
					输入输出无
*********************************/
void tmr0interrupt() interrupt 1;
{
}

