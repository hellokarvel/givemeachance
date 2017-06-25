#include <intrins.h>
#include <reg52.h>
sbit LED = P1^7;
sbit LED_gear1=P2^1;
sbit LED_gear2=P2^2;
sbit LED_gear3=P2^3;														//LED������ʾ���ٴ�С
sbit dj=P1^0;														//����˿�					  												 
sbit DQ = P1^6;					  								//�¶ȴ��������ݶ�				
char table[11]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};	//��������0-9	  		
int d2,d3,d4,d5,time,wen_du;										//4������+��ʱʱ��+�¶ȱ���
sbit key2=P1^2;														//ʱ���
sbit key3=P1^3;														//ʱ���
sbit key4=P1^4;														//ʱ�俪��														 
sbit w1=P2^4;	  													//����ܵ�һ�Σ��͵�ƽ��Ч
sbit w2=P2^5;														//����ܵڶ��Σ��͵�ƽ��Ч
sbit w3=P2^6;														//����ܵ����Σ��͵�ƽ��Ч	
sbit w4=P2^7;														//����ܵ��ĶΣ��͵�ƽ��Ч
int time=0;															
void DHT11_DATA();
void zi_keyscan();
void Timer1();
static unsigned char flag;
 ///////////////////////////////////////////////////////////////////////////////////////

void delay(unsigned int ms)																//��ʱ��������Լ��ʱ
{
	unsigned int x;
	while(ms--)
		for(x=10;x>0;x--); }
void delay_18B20(unsigned int i)
{
	while(i--);
}
/**********ds18b20��ʼ������**********************/
void Init_DS18B20() 
{
	 unsigned char x=0;
	 DQ=1;          //DQ��λ
	 delay_18B20(8);  //������ʱ
	 DQ=0;          //��Ƭ����DQ����
	 delay_18B20(80); //��ȷ��ʱ ���� 480us
	 DQ=1;          //��������
	 delay_18B20(14);
	 x=DQ;            //������ʱ�� ���x=0���ʼ���ɹ� x=1���ʼ��ʧ��
	 delay_18B20(20);
}
/***********ds18b20��һ���ֽ�**************/  
unsigned char ReadOneChar()
{
	unsigned char i=0;
	unsigned char dat=0;
	for (i=8;i>0;i--)
	 {
		  DQ=0; // �������ź�
		  dat>>=1;
		  DQ=1; // �������ź�
		  if(DQ)
		  dat|=0x80;
		  delay_18B20(4);
	 }
 	return(dat);
}
/*************ds18b20дһ���ֽ�****************/  
void WriteOneChar(unsigned char dat)
{
 	unsigned char i=0;
 	for (i=8;i>0;i--)
 	{
  		DQ=0;
 		DQ=dat&0x01;
    	delay_18B20(5);
 		DQ=1;
    	dat>>=1;
	}
}
/**************��ȡds18b20��ǰ�¶�************/
void ReadTemperature()
{
	char a=0;
	char b=0;
	char t=0;
	Init_DS18B20();
	WriteOneChar(0xCC);    	// ����������кŵĲ���
	WriteOneChar(0x44); 	// �����¶�ת��
	delay_18B20(100);       // this message is wery important
	Init_DS18B20();
	WriteOneChar(0xCC); 	//����������кŵĲ���
	WriteOneChar(0xBE); 	//��ȡ�¶ȼĴ����ȣ����ɶ�9���Ĵ����� ǰ���������¶�
	delay_18B20(100);
	a=ReadOneChar();    	//��ȡ�¶�ֵ��λ
	b=ReadOneChar();   		//��ȡ�¶�ֵ��λ
	wen_du=((b*256+a)>>4);    //��ǰ�ɼ��¶�ֵ��16��ʵ���¶�ֵ
}


																			   
void display()																	//�������ʾ
{ if(wen_du==8)wen_du=24;
 if(wen_du==9)wen_du=25;
 if(wen_du==10)wen_du=26;
 if(wen_du==11)wen_du=27;
 if(wen_du==12)wen_du=28;
 if(wen_du==13)wen_du=29;
 if(wen_du==14)wen_du=30;
 if(wen_du==15)wen_du=31;
d2=wen_du/10;d3=wen_du%10;														//ת���¶ȸ�ʽ
 d4=time/10;d5=time%10;															//ת��ʱ���ʽ

	w1=0;P0=table[d2];delay(10); 												//��1λ
	P0=0x00;w1=1;delay(1);		
								  
	w2=0;P0=table[d3];delay(10);												//��2λ
	P0=0x00;w2=1;delay(1);

	w3=0;P0=table[d4]; delay(10);												//��3λ
	P0=0x00;w3=1;delay(1);

	w4=0;P0=table[d5];delay(10);												//��4λ
	P0=0x00;w4=1;delay(1);
							}



void zi_dong()																	//�Զ��¿�ģʽ
{
	unsigned char i;
	zi_keyscan();																//����ɨ�躯��
																//������ʾ����
	if(wen_du<20||(time==0)){dj=0;}														//��������  ��λΪ0 ���ֹͣ
	if((wen_du>=20)&&(wen_du<=25)&&(time!=0))												//�¶ȴ������ޣ�С������  1��
	{ LED_gear1=1;LED_gear2=0;LED_gear3=0;
	for(i=0;i<3;i++){dj=0;display();zi_keyscan();}								//PWMͨ����ʱ�õ�
	 for(i=0;i<6;i++){dj=1;display();zi_keyscan();
	  }		
	}
	if((wen_du>25)&&(wen_du<=30)&&(time!=0))												//�¶ȴ������ޣ�С������  2��
	{ LED_gear1=1;LED_gear2=1;LED_gear3=0;
	for(i=0;i<3;i++){dj=0;display();zi_keyscan(); }								//PWMͨ����ʱ�õ�
	 for(i=0;i<9;i++){dj=1;display();zi_keyscan();}		
	}
	if(wen_du>30&&(time!=0)){dj=1; LED_gear1=3;LED_gear1=1;LED_gear2=1;LED_gear3=1;}
	}																			//����ȫ��
void zi_keyscan()	                  					 						//������ʱ�ر�
{ 	 	
	display();
		 
	if(key2==0){LED=1;										 						//key2 ��ʱ��
	 delay(20);																    //����
	  if(key2==0)time=time+1;}
	while(!key2);		
					  	
    if(key3==0){										 						 //key3��ʱ��
	 delay(20);
	  if(key3==0)time=time-1;}
 	while(!key3);

	if(key4==0){										                         //��ʼ����ʱ����λ��
	 delay(20);
	  if(key4==0){ 
	     for(;time>0;time--) {
		 	display();delay(5800); }
	              }
	            }	
	while(!key4);
															    
	if(time==0){LED=0;
	  LED_gear1=0;
	  LED_gear2=0;
	  LED_gear3=0;};										                 //��ʱ��Ϊ0ʱ�򣬹رյ��
		 }

			
 void main()		  														//������
{	 
	unsigned char j;
	time=10;dj=0;		 														      //�����
	  LED_gear1=0;
	  LED_gear2=0;
	  LED_gear3=0;
	
	while(1)			  													//����whileѭ��
	{ReadTemperature();															//��ȡ�¶�ֵ
	zi_dong();										//�Զ��¿�ģʽ
		}
}													