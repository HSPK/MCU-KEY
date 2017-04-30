#include <reg52.h>
typedef unsigned char uint8;
typedef unsigned int  uint16;
void showNum(uint16 num);
void keyAction(uint8 keyCode);
void init();
void ledRefresh();

sbit keyIn1=P1^3;
sbit keyIn2=P1^2;
sbit keyIn3=P1^1;
sbit keyIn4=P1^0;
sbit keyOut1=P1^7;
sbit keyOut2=P1^6;
sbit keyOut3=P1^5;
sbit keyOut4=P1^4;

uint8 code keyCodeMap[][4]={{ 0x31, 0x32, 0x33, 0x26 }, //数字键1、数字键2、数字键3、向上键
{ 0x34, 0x35, 0x36, 0x25 }, //数字键4、数字键5、数字键6、向左键
{ 0x37, 0x38, 0x39, 0x28 }, //数字键7、数字键8、数字键9、向下键
{ 0x30, 0x1B, 0x0D, 0x27 } //数字键0、ESC键、 回车键、 向右键
}};
uint8 code ledChar[]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
uint8 ledNum[5];
bit keySta[][4]={1,1,1,1,
	                                  1,1,1,1,
	                                  1,1,1,1,
	                                  1,1,1,1	};

/**********************************
				函数名称：main
				函数功能：加法计算器
				输入：无
				返回：无
				
**********************************/

void main()
{
	
	init();
	while(1)
	{
		keyDriver();
	}
}


void init()
{
	P1=0xff;
	P0=0x00;
	TMOD=0X01;
	TH0=
	TL0=
	TR0=1;
	EA=1;
	ET0=1;
	showNum(0);
}

void keyDriver()
{
		uint8 i,j;
	static bit keyBac[][4]={1,1,1,1,                                  1,1,1,1,
	                                  1,1,1,1,
	                                  1,1,1,1};
			for(i=0;i<4,i++)
		{
			for(j=0;j=4;j++)
			{
				if(keySta[i][j]!=keyBac[i][j])
				{
					if(keyBac[i][j])
					{
						keyAction(keyCodeMap[i][j]);
					}
					keyBac[i][j]=keySta[i][j];
			    }
		    }
			
		}
}

void showNum(uint16 num)
{
	char i,j;
	for(i=0;i<5;i++)
	{
		ledNum[i]=num%10;
		num=num/10;
	}
	for(i=4;i>0;i--)
	{
		if(ledNum[i])
			break;
	}
	for(;i>=0;i--)
	{
		ledNum[i]=ledChar[ledNum[i]];
	}
}

void keyAction(uint8 keyCode)
{
	showNum(keyCode);
}


void keyScan()
{
	static uint8 keyBuff[]={0xff,0xff,0xff,0xff};
	static uint8 keyOut=0;
	uint8 i;
	keyBuff[0]=(0xff<<1)|keyIn1;
	keyBuff[1]=(0xff<<1)|keyIn2;
	keyBuff[2]=(0xff<<1)|keyIn3;
	keyBuff[3]=(0xff<<1)|keyIn4;
	for(i=0;i<4;i++)
	{
		if((keyBuff[i]&0x0f)==0x00)
		keySta[keyOut][i]=0;
		else
		keySta[keyOut][i]=1;
	}
	keyOut++;
	keyOut&=0x03;
	switch(keyOut)
	{
		case 0:keyOut4=0;keyOut1=1;break;
		case 1:keyOut1=0;keyOut2=1;break;
		case 2:keyOut2=0;keyOut3=1;break;
		case 3:keyOut3=0;keyOut4=1;break;
	}
}
		
		
void ledRefresh()
{
	uint8 i=0;
	P0=0x00;
	P2&=0xe3;
	P2|=(i<<2);
	P0=ledNum[i];
	if((++i)==5)
	i =0;
}


/*********************************
					中断服务函数
					函数功能：矩阵按键消抖,数码管刷新
					输入输出无
*********************************/
void tmr0interrupt() interrupt 1;
{
	keyScan();
	ledRefresh();
}