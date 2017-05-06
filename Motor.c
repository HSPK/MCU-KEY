#include <reg52.h>
typedef unsigned char uint8;
typedef unsigned int  uint16;
typedef char int8;
typedef int  int16;

void KeyScan();
void KeyAction(uint8 keyCode);
void KeyDriver();
void StartMotor(int16 angle);
void StopMotor();
void ConfigTmr0(uint8 ms);
void Init();
void Delayms(uint8 ms);

sbit keyOut1=P;				//定义输出引脚
sbit KeyIn1=P;				//定义输入引脚

uint8 code keyCodeMap[]={0x01,0x02,0x03,0x04};   //键盘映射值
uint8 keySta[]={1,1,1,1};  //按键状态

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
	KeyOut1=0;     //按键初始
	KeyIn1=1;
	KeyIn2=1;
	KeyIn3=1;
	KeyIn4=1;
	EA=1;
	ConfigTmr0(4);   //定时器0定时4ms
}

void ConfigTmr0(uint8 ms)
{
	uint16 load;
	load=65536-(12000000/12/1000)*ms;
	TMOD|=0x01;
	TMOD&=0xfd;
	TH0=(uint8)(load>>8);
	TL0=(uint8)load;
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
			if(keyBac[i]==1)
			{
				KeyAction(keyCodeMap[i]);//按键动作
			}
			keyBac[i]=keySta[i]);
		}
	}
}


void KeyAction(uint8 keyCode)
{
	switch(keyCode)
	{
		case 0x01:StartMotor(90);break;
		case 0x02:StartMotor(-90);break;
		case 0x03:StartMotor(0);break;
		case 0x04:StopMotor();break;
		default: StopMotor();
	}
}

void StartMotor(int16 angle)
{
	
}
void StopMotor()
{

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
		if((keyBuff[i]&0x0f)==0x00)
		{
			keySta[i]=0;
		}
		else
		{
			keySta[i]=1;
		}
	}
}

//定时器0中断  扫描按键
void Tmr0Interrupt() interrupt 1
{
	KeyScan();
}