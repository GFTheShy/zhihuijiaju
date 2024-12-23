#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"  	 
#include "drv_delay.h"
#include "bmp.h"

u8 OLED_GRAM[144][8];

//·´ÏÔº¯Êı
void OLED_ColorTurn(u8 i)
{
	if(i==0)
		{
			OLED_WR_Byte(0xA6,OLED_CMD);//Õı³£ÏÔÊ¾
		}
	if(i==1)
		{
			OLED_WR_Byte(0xA7,OLED_CMD);//·´É«ÏÔÊ¾
		}
}

//ÆÁÄ»Ğı×ª180¶È
void OLED_DisplayTurn(u8 i)
{
	if(i==0)
		{
			OLED_WR_Byte(0xC8,OLED_CMD);//Õı³£ÏÔÊ¾
			OLED_WR_Byte(0xA1,OLED_CMD);
		}
	if(i==1)
		{
			OLED_WR_Byte(0xC0,OLED_CMD);//·´×ªÏÔÊ¾
			OLED_WR_Byte(0xA0,OLED_CMD);
		}
}

//ÑÓÊ±
void IIC_delay(void)
{
	u8 t=10;
	while(t--);
}

//ÆğÊ¼ĞÅºÅ
void I2C_Start(void)
{
	OLED_SDA_Set();
	OLED_SCL_Set();
	IIC_delay();
	OLED_SDA_Clr();
	IIC_delay();
	OLED_SCL_Clr();
	IIC_delay();
}

//½áÊøĞÅºÅ
void I2C_Stop(void)
{
	OLED_SDA_Clr();
	OLED_SCL_Set();
	IIC_delay();
	OLED_SDA_Set();
}

//µÈ´ıĞÅºÅÏìÓ¦
void I2C_WaitAck(void) //²âÊı¾İĞÅºÅµÄµçÆ½
{
	OLED_SDA_Set();
	IIC_delay();
	OLED_SCL_Set();
	IIC_delay();
	OLED_SCL_Clr();
	IIC_delay();
}

//Ğ´ÈëÒ»¸ö×Ö½Ú
void Send_Byte(u8 dat)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		if(dat&0x80)//½«datµÄ8Î»´Ó×î¸ßÎ»ÒÀ´ÎĞ´Èë
		{
			OLED_SDA_Set();
    }
		else
		{
			OLED_SDA_Clr();
    }
		IIC_delay();
		OLED_SCL_Set();
		IIC_delay();
		OLED_SCL_Clr();//½«Ê±ÖÓĞÅºÅÉèÖÃÎªµÍµçÆ½
		dat<<=1;
  }
}

//·¢ËÍÒ»¸ö×Ö½Ú
//mode:Êı¾İ/ÃüÁî±êÖ¾ 0,±íÊ¾ÃüÁî;1,±íÊ¾Êı¾İ;
void OLED_WR_Byte(u8 dat,u8 mode)
{
	I2C_Start();
	Send_Byte(0x78);
	I2C_WaitAck();
	if(mode){Send_Byte(0x40);}
  else{Send_Byte(0x00);}
	I2C_WaitAck();
	Send_Byte(dat);
	I2C_WaitAck();
	I2C_Stop();
}

//¿ªÆôOLEDÏÔÊ¾ 
void OLED_DisPlay_On(void)
{
	OLED_WR_Byte(0x8D,OLED_CMD);//µçºÉ±ÃÊ¹ÄÜ
	OLED_WR_Byte(0x14,OLED_CMD);//¿ªÆôµçºÉ±Ã
	OLED_WR_Byte(0xAF,OLED_CMD);//µãÁÁÆÁÄ»
}

//¹Ø±ÕOLEDÏÔÊ¾ 
void OLED_DisPlay_Off(void)
{
	OLED_WR_Byte(0x8D,OLED_CMD);//µçºÉ±ÃÊ¹ÄÜ
	OLED_WR_Byte(0x10,OLED_CMD);//¹Ø±ÕµçºÉ±Ã
	OLED_WR_Byte(0xAE,OLED_CMD);//¹Ø±ÕÆÁÄ»
}

//¸üĞÂÏÔ´æµ½OLED	
void OLED_Refresh(void)
{
	u8 i,n;
	for(i=0;i<8;i++)
	{
		OLED_WR_Byte(0xb0+i,OLED_CMD); //ÉèÖÃĞĞÆğÊ¼µØÖ·
		OLED_WR_Byte(0x00,OLED_CMD);   //ÉèÖÃµÍÁĞÆğÊ¼µØÖ·
		OLED_WR_Byte(0x10,OLED_CMD);   //ÉèÖÃ¸ßÁĞÆğÊ¼µØÖ·
		I2C_Start();
		Send_Byte(0x78);
		I2C_WaitAck();
		Send_Byte(0x40);
		I2C_WaitAck();
		for(n=0;n<128;n++)
		{
			Send_Byte(OLED_GRAM[n][i]);
			I2C_WaitAck();
		}
		I2C_Stop();
  }
}
//ÇåÆÁº¯Êı
void OLED_Clear(void)
{
	u8 i,n;
	for(i=0;i<8;i++)
	{
	   for(n=0;n<128;n++)
			{
			 OLED_GRAM[n][i]=0;//Çå³ıËùÓĞÊı¾İ
			}
  }
	OLED_Refresh();//¸üĞÂÏÔÊ¾
}

//»­µã 
//x:0~127
//y:0~63
//t:1 Ìî³ä 0,Çå¿Õ	
void OLED_DrawPoint(u8 x,u8 y,u8 t)
{
	u8 i,m,n;
	i=y/8;
	m=y%8;
	n=1<<m;
	if(t){OLED_GRAM[x][i]|=n;}
	else
	{
		OLED_GRAM[x][i]=~OLED_GRAM[x][i];
		OLED_GRAM[x][i]|=n;
		OLED_GRAM[x][i]=~OLED_GRAM[x][i];
	}
}

//»­Ïß
//x1,y1:Æğµã×ø±ê
//x2,y2:½áÊø×ø±ê
void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2,u8 mode)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	delta_x=x2-x1; //¼ÆËã×ø±êÔöÁ¿ 
	delta_y=y2-y1;
	uRow=x1;//»­ÏßÆğµã×ø±ê
	uCol=y1;
	if(delta_x>0)incx=1; //ÉèÖÃµ¥²½·½Ïò 
	else if (delta_x==0)incx=0;//´¹Ö±Ïß 
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if (delta_y==0)incy=0;//Ë®Æ½Ïß 
	else {incy=-1;delta_y=-delta_x;}
	if(delta_x>delta_y)distance=delta_x; //Ñ¡È¡»ù±¾ÔöÁ¿×ø±êÖá 
	else distance=delta_y;
	for(t=0;t<distance+1;t++)
	{
		OLED_DrawPoint(uRow,uCol,mode);//»­µã
		xerr+=delta_x;
		yerr+=delta_y;
		if(xerr>distance)
		{
			xerr-=distance;
			uRow+=incx;
		}
		if(yerr>distance)
		{
			yerr-=distance;
			uCol+=incy;
		}
	}
}
//x,y:Ô²ĞÄ×ø±ê
//r:Ô²µÄ°ë¾¶
void OLED_DrawCircle(u8 x,u8 y,u8 r)
{
	int a, b,num;
    a = 0;
    b = r;
    while(2 * b * b >= r * r)      
    {
        OLED_DrawPoint(x + a, y - b,1);
        OLED_DrawPoint(x - a, y - b,1);
        OLED_DrawPoint(x - a, y + b,1);
        OLED_DrawPoint(x + a, y + b,1);
 
        OLED_DrawPoint(x + b, y + a,1);
        OLED_DrawPoint(x + b, y - a,1);
        OLED_DrawPoint(x - b, y - a,1);
        OLED_DrawPoint(x - b, y + a,1);
        
        a++;
        num = (a * a + b * b) - r*r;//¼ÆËã»­µÄµãÀëÔ²ĞÄµÄ¾àÀë
        if(num > 0)
        {
            b--;
            a--;
        }
    }
}



//ÔÚÖ¸¶¨Î»ÖÃÏÔÊ¾Ò»¸ö×Ö·û,°üÀ¨²¿·Ö×Ö·û
//x:0~127
//y:0~63
//size1:Ñ¡Ôñ×ÖÌå 6x8/6x12/8x16/12x24
//mode:0,·´É«ÏÔÊ¾;1,Õı³£ÏÔÊ¾
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1,u8 mode)
{
	u8 i,m,temp,size2,chr1;
	u8 x0=x,y0=y;
	if(size1==8)size2=6;
	else size2=(size1/8+((size1%8)?1:0))*(size1/2);  //µÃµ½×ÖÌåÒ»¸ö×Ö·û¶ÔÓ¦µãÕó¼¯ËùÕ¼µÄ×Ö½ÚÊı
	chr1=chr-' ';  //¼ÆËãÆ«ÒÆºóµÄÖµ
	for(i=0;i<size2;i++)
	{
		if(size1==8)
			  {temp=asc2_0806[chr1][i];} //µ÷ÓÃ0806×ÖÌå
		else if(size1==12)
        {temp=asc2_1206[chr1][i];} //µ÷ÓÃ1206×ÖÌå
		else if(size1==16)
        {temp=asc2_1608[chr1][i];} //µ÷ÓÃ1608×ÖÌå
		else if(size1==24)
        {temp=asc2_2412[chr1][i];} //µ÷ÓÃ2412×ÖÌå
		else return;
		for(m=0;m<8;m++)
		{
			if(temp&0x01)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp>>=1;
			y++;
		}
		x++;
		if((size1!=8)&&((x-x0)==size1/2))
		{x=x0;y0=y0+8;}
		y=y0;
  }
}


//ÏÔÊ¾×Ö·û´®
//x,y:Æğµã×ø±ê  
//size1:×ÖÌå´óĞ¡ 
//*chr:×Ö·û´®ÆğÊ¼µØÖ· 
//mode:0,·´É«ÏÔÊ¾;1,Õı³£ÏÔÊ¾
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1,u8 mode)
{
	while((*chr>=' ')&&(*chr<='~'))//ÅĞ¶ÏÊÇ²»ÊÇ·Ç·¨×Ö·û!
	{
		OLED_ShowChar(x,y,*chr,size1,mode);
		if(size1==8)x+=6;
		else x+=size1/2;
		chr++;
  }
}

//m^n
u32 OLED_Pow(u8 m,u8 n)
{
	u32 result=1;
	while(n--)
	{
	  result*=m;
	}
	return result;
}

//ÏÔÊ¾Êı×Ö
//x,y :Æğµã×ø±ê
//num :ÒªÏÔÊ¾µÄÊı×Ö
//len :Êı×ÖµÄÎ»Êı
//size:×ÖÌå´óĞ¡
//mode:0,·´É«ÏÔÊ¾;1,Õı³£ÏÔÊ¾
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size1,u8 mode)
{
	u8 t,temp,m=0;
	if(size1==8)m=2;
	for(t=0;t<len;t++)
	{
		temp=(num/OLED_Pow(10,len-t-1))%10;
			if(temp==0)
			{
				OLED_ShowChar(x+(size1/2+m)*t,y,'0',size1,mode);
      }
			else 
			{
			  OLED_ShowChar(x+(size1/2+m)*t,y,temp+'0',size1,mode);
			}
  }
}

//ÏÔÊ¾ºº×Ö
//x,y:Æğµã×ø±ê
//num:ºº×Ö¶ÔÓ¦µÄĞòºÅ
//mode:0,·´É«ÏÔÊ¾;1,Õı³£ÏÔÊ¾
void OLED_ShowChinese(u8 x,u8 y,u8 num,u8 size1,u8 mode)
{
	u8 m,temp;
	u8 x0=x,y0=y;
	u16 i,size3=(size1/8+((size1%8)?1:0))*size1;  //µÃµ½×ÖÌåÒ»¸ö×Ö·û¶ÔÓ¦µãÕó¼¯ËùÕ¼µÄ×Ö½ÚÊı
	for(i=0;i<size3;i++)
	{
		if(size1==16)
				{temp=Hzk1[num][i];}//µ÷ÓÃ16*16×ÖÌå
		else if(size1==24)
				{temp=Hzk2[num][i];}//µ÷ÓÃ24*24×ÖÌå
		else if(size1==32)       
				{temp=Hzk3[num][i];}//µ÷ÓÃ32*32×ÖÌå
		else if(size1==64)
				{temp=Hzk4[num][i];}//µ÷ÓÃ64*64×ÖÌå
		else return;
		for(m=0;m<8;m++)
		{
			if(temp&0x01)OLED_DrawPoint(x,y,mode);
			else OLED_DrawPoint(x,y,!mode);
			temp>>=1;
			y++;
		}
		x++;
		if((x-x0)==size1)
		{x=x0;y0=y0+8;}
		y=y0;
	}
}

//num ÏÔÊ¾ºº×ÖµÄ¸öÊı
//space Ã¿Ò»±éÏÔÊ¾µÄ¼ä¸ô
//mode:0,·´É«ÏÔÊ¾;1,Õı³£ÏÔÊ¾
void OLED_ScrollDisplay(u8 num,u8 space,u8 mode)
{
	u8 i,n,t=0,m=0,r;
	while(1)
	{
		if(m==0)
		{
	    OLED_ShowChinese(128,24,t,16,mode); //Ğ´ÈëÒ»¸öºº×Ö±£´æÔÚOLED_GRAM[][]Êı×éÖĞ
			t++;
		}
		if(t==num)
			{
				for(r=0;r<16*space;r++)      //ÏÔÊ¾¼ä¸ô
				 {
					for(i=1;i<144;i++)
						{
							for(n=0;n<8;n++)
							{
								OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
							}
						}
           OLED_Refresh();
				 }
        t=0;
      }
		m++;
		if(m==16){m=0;}
		for(i=1;i<144;i++)   //ÊµÏÖ×óÒÆ
		{
			for(n=0;n<8;n++)
			{
				OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
			}
		}
		OLED_Refresh();
	}
}

//x,y£ºÆğµã×ø±ê
//sizex,sizey,Í¼Æ¬³¤¿í
//BMP[]£ºÒªĞ´ÈëµÄÍ¼Æ¬Êı×é
//mode:0,·´É«ÏÔÊ¾;1,Õı³£ÏÔÊ¾
void OLED_ShowPicture(u8 x,u8 y,u8 sizex,u8 sizey,u8 BMP[],u8 mode)
{
	u16 j=0;
	u8 i,n,temp,m;
	u8 x0=x,y0=y;
	sizey=sizey/8+((sizey%8)?1:0);
	for(n=0;n<sizey;n++)
	{
		 for(i=0;i<sizex;i++)
		 {
				temp=BMP[j];
				j++;
				for(m=0;m<8;m++)
				{
					if(temp&0x01)OLED_DrawPoint(x,y,mode);
					else OLED_DrawPoint(x,y,!mode);
					temp>>=1;
					y++;
				}
				x++;
				if((x-x0)==sizex)
				{
					x=x0;
					y0=y0+8;
				}
				y=y0;
     }
	 }
}
//OLEDµÄ³õÊ¼»¯
void OLED_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);//Ê¹ÄÜPORTA~E,PORTGÊ±ÖÓ
  	
	//GPIO³õÊ¼»¯ÉèÖÃ
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//ÆÕÍ¨Êä³öÄ£Ê½
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//ÍÆÍìÊä³ö
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//ÉÏÀ­
  GPIO_Init(GPIOB, &GPIO_InitStructure);//³õÊ¼»¯

	
	//GPIO³õÊ¼»¯ÉèÖÃ
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//ÆÕÍ¨Êä³öÄ£Ê½
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//ÍÆÍìÊä³ö
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//ÉÏÀ­
  GPIO_Init(GPIOB, &GPIO_InitStructure);//³õÊ¼»¯
	
	OLED_RES_Clr();
	drv_Systick_delayms(200);
	OLED_RES_Set();
	
	OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
	OLED_WR_Byte(0xCF,OLED_CMD);// Set SEG Output Current Brightness
	OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0×óÓÒ·´ÖÃ 0xa1Õı³£
	OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0ÉÏÏÂ·´ÖÃ 0xc8Õı³£
	OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WR_Byte(0x00,OLED_CMD);//-not offset
	OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
	OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
	OLED_WR_Byte(0x12,OLED_CMD);
	OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
	OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
	OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WR_Byte(0x02,OLED_CMD);//
	OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
	OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
	OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	OLED_Clear();
	OLED_WR_Byte(0xAF,OLED_CMD);
}

int oled_begin_mv()
{
	OLED_Refresh();
	drv_Systick_delayms(2000);
	OLED_Clear();
	OLED_ShowChinese(0,0,0,16,1);//å
	OLED_ShowChinese(18,0,1,16,1);//æ¸…
	OLED_ShowChinese(36,0,2,16,1);//è¿œ
	OLED_ShowChinese(54,0,3,16,1);//è§
	OLED_ShowString(8,16,"hello stm32",16,1);
	OLED_Refresh();
	drv_Systick_delayms(2000);
	OLED_Clear();
	OLED_ShowChinese(0,0,4,16,1);//æ
	OLED_ShowChinese(18,0,5,16,1);//é™ˆ
	OLED_ShowChinese(36,0,6,16,1);//åˆš
	OLED_Refresh();
	drv_Systick_delayms(2000);
	OLED_Clear();
	OLED_Refresh();
	drv_Systick_delayms(2000);
}
