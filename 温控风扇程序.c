#include<reg51.h>
#include<intrins.h>			 //����ͷ�ļ�
#define uchar unsigned char
#define uint unsigned int		 //�궨��
////////////////////
sbit dj=P1^0;//������ƶ˽ӿ�
sbit DQ=P1^6;//�¶ȴ������ӿ�
//////////�����ӿ�/////////////////////////////////
sbit key1=P3^5;//�����¶�
sbit key2=P3^6;//�¶ȼ�
sbit key3=P3^7;//�¶ȼ�
//////////////////////////////////////////////////////
sbit w1=P2^4;
sbit w2=P2^5;
sbit w3=P2^6;
sbit w4=P2^7;			  //����ܵ��ĸ�λ
/////��������ܶ�ѡ//////////////////////////////////////////////
uchar table[22]=
{0x3F,0x06,0x5B,0x4F,0x66,
0x6D,0x7D,0x07,0x7F,0x6F,
0x77,0x7C,0x39,0x5E,0x79,0x71,
0x40,0x38,0x76,0x00,0xff,0x37};//'-',L,H,��,ȫ����n	 16-21
uint wen_du;						//�¶ȱ���  
uint shang,xia; //�Ա��¶��ݴ����
uchar dang;//��λ��ʾ
uchar flag;
uchar d1,d2,d3;//��ʾ�����ݴ����
void delay(uint ms)		//��ʱ��������Լ��ʱ25us
{
	uchar x;
	for(ms;ms>0;ms--)
		for(x=10;x>0;x--);
}
/***********ds18b20�ӳ��Ӻ���������12MHz ��*******/ 
void delay_18B20(uint i)
{
	while(i--);
}
/**********ds18b20��ʼ������**********************/
void Init_DS18B20() 
{
	 uchar x=0;
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
uchar ReadOneChar()
{
	uchar i=0;
	uchar dat=0;
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
void WriteOneChar(uchar dat)
{
 	uchar i=0;
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
	uchar a=0;
	uchar b=0;
	uchar t=0;
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
void display()//��ʾ�¶�
{
	w1=0;P0=table[d1];delay(10); //��1λ
	P0=0x00;w1=1;delay(1);
	 
	w2=0;P0=table[16];delay(10);//��2λ
	P0=0x00;w2=1;delay(1);

	w3=0;P0=table[d2]; delay(10);//��3λ
	P0=0x00;w3=1;delay(1);

	w4=0;P0=table[d3];delay(10);//��4λ
	P0=0x00;w4=1;delay(1);
}
void zi_keyscan()//�Զ�ģʽ����ɨ�躯��
{
	if(key1==0)							  //���ü�����
	{
		delay(30);						  //��ʱȥ��
		if(key1==0)flag=1;				  //�ٴ��жϰ��������µĻ���������״̬
		while(key1==0);//���ּ��		  //�����ͷ�
	}
	while(flag==1)						  //������������״̬
	{
		d1=18;d2=shang/10;d3=shang%10;	  //��ʾ��ĸH �������¶�ֵ
		display();						  //������ʾ����
		if(key1==0)						  //�ж����ü��Ƿ���
		{
			delay(30);					  //��ʱȥ��
			if(key1==0)flag=2;			  //�������£�������������ģʽ
			while(key1==0);//���ּ��
		}
		if(key2==0)						  //�Ӽ�����
		{
			delay(30);					  //��ʱȥ��
			if(key2==0)					  //�Ӽ�����
			{
				shang+=5;				  //���޼�5
				if(shang>=100)shang=100;  //�������ӵ�100
			}while(key2==0);//���ּ��
		}
		if(key3==0)						  //��������
		{
			delay(30);					  //��ʱȥ��
			if(key3==0)					  //��������
			{
				shang-=1;				  //���޼�1
				if(shang<=10)shang=10;	  //������С����10
			}while(key3==0);//���ּ��
		}		
	}
	while(flag==2)						  //��������
	{
		d1=17;d2=xia/10;d3=xia%10;		  //��ʾ��ĸL ��ʾ�����¶�ֵ
		display();						  //����ע��ͬ��
		if(key1==0)
		{
			delay(30);
			if(key1==0)flag=0;
			while(key1==0);//���ּ��
		}
		if(key2==0)
		{
			delay(30);
			if(key2==0)
			{
				xia+=5;
				if(xia>=95)xia=95;	
			}while(key2==0);//���ּ��
		}
		if(key3==0)
		{
			delay(30);
			if(key3==0)
			{
				xia-=1;
				if(xia<=0)xia=0;	
			}while(key3==0);//���ּ��
		}		
	}
}
void zi_dong()//�Զ��¿�ģʽ
{
	uchar i;
	d1=dang;d2=wen_du/10;d3=wen_du%10;		//��ʾ��λ����ʾ��ǰ�¶�ֵ
	zi_keyscan();//����ɨ�躯��
	display();	 //������ʾ����
	if(wen_du<xia){dj=0;dang=0;}//��������  ��λΪ0 ���ֹͣ
	if((wen_du>=xia)&&(wen_du<=shang))//�¶ȴ������ޣ�С������  1��
	{
		dang=1;											//��λ��1
		for(i=0;i<3;i++){dj=0;display();zi_keyscan();}	//PWMͨ����ʱ�õ�
		for(i=0;i<6;i++){dj=1;display();zi_keyscan();}		
	}
	if(wen_du>shang){dj=1;dang=2;}//����ȫ��
}
void main()		  //������
{
	uchar j;
	dj=0;		  //�����
	shang=30;
	xia=20;		  //��ʼ������ֵ
	for(j=0;j<80;j++)	  //�ȶ�ȡ�¶�ֵ����ֹ������ʾ85
	ReadTemperature();
	while(1)			  //����whileѭ��
	{	
		ReadTemperature();	//��ȡ�¶�ֵ
		for(j=0;j<100;j++)zi_dong();//�Զ��¿�ģʽ
	}
}