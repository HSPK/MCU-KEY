#include <reg52.h>
#define SYM_OSC 12000000
typedef unsigned char uint8;
typedef unsigned int  uint16;
typedef char int8;
typedef int  int16;
typedef long  int32;

void KeyScan();
void KeyAction(uint8 keyCode);
void KeyDriver();
void StartMotor(int16 angle);
void StopMotor();
void ConfigTmr0(uint8 ms);
void Init();
void Delayms(uint8 ms);

				//定义输出引脚
sbit keyIn1=P3^0;				//定义输入引脚
sbit keyIn2=P3^1;
sbit keyIn3=P3^2;
sbit keyIn4=P3^3;

uint8 code keyCodeMap[]={0x01,0x02,0x03,0x04};   //键盘映射值
uint8 keySta[]={1,1,1,1};  //按键状态

uint8 T0RL,T0RH;
int32 beatNum=0;


void main()
{
	Init();
	while(1)
	{
		KeyDriver();
	}
}

void Init()
{
	    //按键初始
	keyIn1=1;
	keyIn2=1;
	keyIn3=1;
	keyIn4=1;
	EA=1;
	P2&=0xfe;
	ConfigTmr0(2);   //定时器0定时2ms
}

void ConfigTmr0(uint8 ms)
{
	uint16 load;
	load=65536-(SYM_OSC/12/1000)*ms;
	T0RH=(load>>8);
	T0RL=(load);
	TMOD|=0x01;
	TMOD&=0xfd;
	TH0=T0RH;
	TL0=T0RL;
	TR0=1;
	ET0=1;
}


//按键驱动函数
void KeyDriver()
{
	uint8 i;
	static uint8 keyBac[]={1,1,1,1};
	for(i=0;i<4;i++)
	{
		if(keySta[i]!=keyBac[i])
		{	
			keyBac[i]=keySta[i];
			if(keySta[i]==0)
			{
				KeyAction(keyCodeMap[i]);//按键动作
			}
		
		}
	}
}


void KeyAction(uint8 keyCode)
{
	switch(keyCode)
	{
		case 0x01:
			P2=0xfe;StartMotor(300);break;
		case 0x02:
			StartMotor(-180);P2=0xfd;break;
		case 0x03:
			StartMotor(180);P2=0xf8;break;
		case 0x04:
			StopMotor();break;
		default: StopMotor();
	}
}

void StartMotor(int16 angle)
{
	EA=0;
	beatNum=(angle*4076);
	EA=1;
}

void StopMotor()
{
	EA=0;
	beatNum=0;
	EA=1;
}

void KeyScan()
{
	uint8 i;
	static uint8 keyBuff[]={0xff,0xff,0xff,0xff};
	keyBuff[0]=(keyBuff[0]<<1)|keyIn1;
	keyBuff[1]=(keyBuff[1]<<1)|keyIn2;
	keyBuff[2]=(keyBuff[2]<<1)|keyIn3;
	keyBuff[3]=(keyBuff[3]<<1)|keyIn4;
	for(i=0;i<4;i++)
	{
		if((keyBuff[i])==0x00)
		{
			keySta[i]=0;
		}
		else
		{
			keySta[i]=1;
		}
	}
}

void TurnMotor()
{
	uint8 code motorStep[]={0x8,0xc,0x4,0x6,0x2,0x3,0x1,0x9};
	static int8 index=0;
	uint8 tmp;
	if(beatNum!=0)
	{	
	
		if(beatNum>0)
		{
			index++;
			index&=0x07;
			beatNum--;
		}
		else if(beatNum<0)
		{
			index--;
			index&=0x07;
			beatNum++;
		}		tmp=P1;
		tmp&=0xf0;
		tmp|=motorStep[index];
		P1=tmp;
	}
	else 
	{
		P1&=0xf0;
	}
	
}

//定时器0中断  扫描按键
void Tmr0Interrupt() interrupt 1
{
	TH0=T0RH;
	TL0=T0RL;
	KeyScan();
	TurnMotor();
}
