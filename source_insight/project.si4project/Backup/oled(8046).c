#include "oled.h"
#include "stdlib.h"
#include "oledfont.h"  	 
#include "drv_delay.h"
#include "bmp.h"

u8 OLED_GRAM[144][8];

//∑¥œ‘∫Ø ˝
void OLED_ColorTurn(u8 i)
{
	if(i==0)
		{
			OLED_WR_Byte(0xA6,OLED_CMD);//’˝≥£œ‘ æ
		}
	if(i==1)
		{
			OLED_WR_Byte(0xA7,OLED_CMD);//∑¥…´œ‘ æ
		}
}

//∆¡ƒª–˝◊™180∂»
void OLED_DisplayTurn(u8 i)
{
	if(i==0)
		{
			OLED_WR_Byte(0xC8,OLED_CMD);//’˝≥£œ‘ æ
			OLED_WR_Byte(0xA1,OLED_CMD);
		}
	if(i==1)
		{
			OLED_WR_Byte(0xC0,OLED_CMD);//∑¥◊™œ‘ æ
			OLED_WR_Byte(0xA0,OLED_CMD);
		}
}

//—” ±
void IIC_delay(void)
{
	u8 t=10;
	while(t--);
}

//∆ º–≈∫≈
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

//Ω· ¯–≈∫≈
void I2C_Stop(void)
{
	OLED_SDA_Clr();
	OLED_SCL_Set();
	IIC_delay();
	OLED_SDA_Set();
}

//µ»¥˝–≈∫≈œÏ”¶
void I2C_WaitAck(void) //≤‚ ˝æ›–≈∫≈µƒµÁ∆Ω
{
	OLED_SDA_Set();
	IIC_delay();
	OLED_SCL_Set();
	IIC_delay();
	OLED_SCL_Clr();
	IIC_delay();
}

//–¥»Î“ª∏ˆ◊÷Ω⁄
void Send_Byte(u8 dat)
{
	u8 i;
	for(i=0;i<8;i++)
	{
		if(dat&0x80)//Ω´datµƒ8Œª¥”◊Ó∏ﬂŒª“¿¥Œ–¥»Î
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
		OLED_SCL_Clr();//Ω´ ±÷”–≈∫≈…Ë÷√Œ™µÕµÁ∆Ω
		dat<<=1;
  }
}

//∑¢ÀÕ“ª∏ˆ◊÷Ω⁄
//mode: ˝æ›/√¸¡Ó±Í÷æ 0,±Ì æ√¸¡Ó;1,±Ì æ ˝æ›;
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

//ø™∆ÙOLEDœ‘ æ 
void OLED_DisPlay_On(void)
{
	OLED_WR_Byte(0x8D,OLED_CMD);//µÁ∫…±√ πƒ‹
	OLED_WR_Byte(0x14,OLED_CMD);//ø™∆ÙµÁ∫…±√
	OLED_WR_Byte(0xAF,OLED_CMD);//µ„¡¡∆¡ƒª
}

//πÿ±’OLEDœ‘ æ 
void OLED_DisPlay_Off(void)
{
	OLED_WR_Byte(0x8D,OLED_CMD);//µÁ∫…±√ πƒ‹
	OLED_WR_Byte(0x10,OLED_CMD);//πÿ±’µÁ∫…±√
	OLED_WR_Byte(0xAE,OLED_CMD);//πÿ±’∆¡ƒª
}

//∏¸–¬œ‘¥ÊµΩOLED	
void OLED_Refresh(void)
{
	u8 i,n;
	for(i=0;i<8;i++)
	{
		OLED_WR_Byte(0xb0+i,OLED_CMD); //…Ë÷√––∆ ºµÿ÷∑
		OLED_WR_Byte(0x00,OLED_CMD);   //…Ë÷√µÕ¡–∆ ºµÿ÷∑
		OLED_WR_Byte(0x10,OLED_CMD);   //…Ë÷√∏ﬂ¡–∆ ºµÿ÷∑
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
//«Â∆¡∫Ø ˝
void OLED_Clear(void)
{
	u8 i,n;
	for(i=0;i<8;i++)
	{
	   for(n=0;n<128;n++)
			{
			 OLED_GRAM[n][i]=0;//«Â≥˝À˘”– ˝æ›
			}
  }
	OLED_Refresh();//∏¸–¬œ‘ æ
}

//ª≠µ„ 
//x:0~127
//y:0~63
//t:1 ÃÓ≥‰ 0,«Âø’	
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

//ª≠œﬂ
//x1,y1:∆µ„◊¯±Í
//x2,y2:Ω· ¯◊¯±Í
void OLED_DrawLine(u8 x1,u8 y1,u8 x2,u8 y2,u8 mode)
{
	u16 t; 
	int xerr=0,yerr=0,delta_x,delta_y,distance;
	int incx,incy,uRow,uCol;
	delta_x=x2-x1; //º∆À„◊¯±Í‘ˆ¡ø 
	delta_y=y2-y1;
	uRow=x1;//ª≠œﬂ∆µ„◊¯±Í
	uCol=y1;
	if(delta_x>0)incx=1; //…Ë÷√µ•≤Ω∑ΩœÚ 
	else if (delta_x==0)incx=0;//¥π÷±œﬂ 
	else {incx=-1;delta_x=-delta_x;}
	if(delta_y>0)incy=1;
	else if (delta_y==0)incy=0;//ÀÆ∆Ωœﬂ 
	else {incy=-1;delta_y=-delta_x;}
	if(delta_x>delta_y)distance=delta_x; //—°»°ª˘±æ‘ˆ¡ø◊¯±Í÷· 
	else distance=delta_y;
	for(t=0;t<distance+1;t++)
	{
		OLED_DrawPoint(uRow,uCol,mode);//ª≠µ„
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
//x,y:‘≤–ƒ◊¯±Í
//r:‘≤µƒ∞Îæ∂
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
        num = (a * a + b * b) - r*r;//º∆À„ª≠µƒµ„¿Î‘≤–ƒµƒæ‡¿Î
        if(num > 0)
        {
            b--;
            a--;
        }
    }
}



//‘⁄÷∏∂®Œª÷√œ‘ æ“ª∏ˆ◊÷∑˚,∞¸¿®≤ø∑÷◊÷∑˚
//x:0~127
//y:0~63
//size1:—°‘Ò◊÷ÃÂ 6x8/6x12/8x16/12x24
//mode:0,∑¥…´œ‘ æ;1,’˝≥£œ‘ æ
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size1,u8 mode)
{
	u8 i,m,temp,size2,chr1;
	u8 x0=x,y0=y;
	if(size1==8)size2=6;
	else size2=(size1/8+((size1%8)?1:0))*(size1/2);  //µ√µΩ◊÷ÃÂ“ª∏ˆ◊÷∑˚∂‘”¶µ„’ÛºØÀ˘’ºµƒ◊÷Ω⁄ ˝
	chr1=chr-' ';  //º∆À„∆´“∆∫Ûµƒ÷µ
	for(i=0;i<size2;i++)
	{
		if(size1==8)
			  {temp=asc2_0806[chr1][i];} //µ˜”√0806◊÷ÃÂ
		else if(size1==12)
        {temp=asc2_1206[chr1][i];} //µ˜”√1206◊÷ÃÂ
		else if(size1==16)
        {temp=asc2_1608[chr1][i];} //µ˜”√1608◊÷ÃÂ
		else if(size1==24)
        {temp=asc2_2412[chr1][i];} //µ˜”√2412◊÷ÃÂ
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


//œ‘ æ◊÷∑˚¥Æ
//x,y:∆µ„◊¯±Í  
//size1:◊÷ÃÂ¥Û–° 
//*chr:◊÷∑˚¥Æ∆ ºµÿ÷∑ 
//mode:0,∑¥…´œ‘ æ;1,’˝≥£œ‘ æ
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 size1,u8 mode)
{
	while((*chr>=' ')&&(*chr<='~'))//≈–∂œ «≤ª «∑«∑®◊÷∑˚!
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

//œ‘ æ ˝◊÷
//x,y :∆µ„◊¯±Í
//num :“™œ‘ æµƒ ˝◊÷
//len : ˝◊÷µƒŒª ˝
//size:◊÷ÃÂ¥Û–°
//mode:0,∑¥…´œ‘ æ;1,’˝≥£œ‘ æ
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

//œ‘ æ∫∫◊÷
//x,y:∆µ„◊¯±Í
//num:∫∫◊÷∂‘”¶µƒ–Ú∫≈
//mode:0,∑¥…´œ‘ æ;1,’˝≥£œ‘ æ
void OLED_ShowChinese(u8 x,u8 y,u8 num,u8 size1,u8 mode)
{
	u8 m,temp;
	u8 x0=x,y0=y;
	u16 i,size3=(size1/8+((size1%8)?1:0))*size1;  //µ√µΩ◊÷ÃÂ“ª∏ˆ◊÷∑˚∂‘”¶µ„’ÛºØÀ˘’ºµƒ◊÷Ω⁄ ˝
	for(i=0;i<size3;i++)
	{
		if(size1==16)
				{temp=Hzk1[num][i];}//µ˜”√16*16◊÷ÃÂ
		else if(size1==24)
				{temp=Hzk2[num][i];}//µ˜”√24*24◊÷ÃÂ
		else if(size1==32)       
				{temp=Hzk3[num][i];}//µ˜”√32*32◊÷ÃÂ
		else if(size1==64)
				{temp=Hzk4[num][i];}//µ˜”√64*64◊÷ÃÂ
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

//num œ‘ æ∫∫◊÷µƒ∏ˆ ˝
//space √ø“ª±Èœ‘ æµƒº‰∏Ù
//mode:0,∑¥…´œ‘ æ;1,’˝≥£œ‘ æ
void OLED_ScrollDisplay(u8 num,u8 space,u8 mode)
{
	u8 i,n,t=0,m=0,r;
	while(1)
	{
		if(m==0)
		{
	    OLED_ShowChinese(128,24,t,16,mode); //–¥»Î“ª∏ˆ∫∫◊÷±£¥Ê‘⁄OLED_GRAM[][] ˝◊È÷–
			t++;
		}
		if(t==num)
			{
				for(r=0;r<16*space;r++)      //œ‘ æº‰∏Ù
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
		for(i=1;i<144;i++)   // µœ÷◊Û“∆
		{
			for(n=0;n<8;n++)
			{
				OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
			}
		}
		OLED_Refresh();
	}
}

//x,y£∫∆µ„◊¯±Í
//sizex,sizey,Õº∆¨≥§øÌ
//BMP[]£∫“™–¥»ÎµƒÕº∆¨ ˝◊È
//mode:0,∑¥…´œ‘ æ;1,’˝≥£œ‘ æ
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
//OLEDµƒ≥ı ºªØ
void OLED_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);// πƒ‹PORTA~E,PORTG ±÷”
  	
	//GPIO≥ı ºªØ…Ë÷√
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//∆’Õ® ‰≥ˆƒ£ Ω
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//Õ∆ÕÏ ‰≥ˆ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//…œ¿≠
  GPIO_Init(GPIOB, &GPIO_InitStructure);//≥ı ºªØ

	
	//GPIO≥ı ºªØ…Ë÷√
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//∆’Õ® ‰≥ˆƒ£ Ω
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//Õ∆ÕÏ ‰≥ˆ
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100MHz
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;//…œ¿≠
  GPIO_Init(GPIOB, &GPIO_InitStructure);//≥ı ºªØ
	
	OLED_RES_Clr();
	drv_Systick_delayms(200);
	OLED_RES_Set();
	
	OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
	OLED_WR_Byte(0xCF,OLED_CMD);// Set SEG Output Current Brightness
	OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0◊Û”“∑¥÷√ 0xa1’˝≥£
	OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0…œœ¬∑¥÷√ 0xc8’˝≥£
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
	OLED_ShowChinese(0,0,0,16,1);//Âçé
	OLED_ShowChinese(18,0,1,16,1);//Ê∏Ö
	OLED_ShowChinese(36,0,2,16,1);//Ëøú
	OLED_ShowChinese(54,0,3,16,1);//ËßÅ
	OLED_ShowString(8,16,"hello stm32",16,1);
	OLED_Refresh();
	drv_Systick_delayms(2000);
	OLED_Clear();
	OLED_ShowChinese(0,0,4,16,1);//Êùé
	OLED_ShowChinese(18,0,5,16,1);//Èôà
	OLED_ShowChinese(36,0,6,16,1);//Âàö
	OLED_Refresh();
	drv_Systick_delayms(2000);
	OLED_Clear();
	OLED_Refresh();
	drv_Systick_delayms(2000);
}		
char text[20];
extern u8 key1_num,key2_num,key3_num,key1_true,key2_true,key3_true,Cnt_Error;

void oled_func()
{
	if(oled_ui==1)
	{

		OLED_ShowChinese(8,0,7,16,1);   //Êô∫
		OLED_ShowChinese(22,0,8,16,1);  //ÊÖß
		OLED_ShowChinese(36,0,9,16,1);  //ÂÆ∂
		OLED_ShowChinese(50,0,10,16,1);  //Â±Ö
		OLED_ShowChinese(64,0,11,16,1); //Êéß
		OLED_ShowChinese(80,0,12,16,1); //Âà∂
		OLED_ShowChinese(94,0,13,16,1); //Á≥ª
		OLED_ShowChinese(108,0,14,16,1);//Áªü
		
		OLED_ShowString(48,16,"K1:",16,1);
		sprintf(text,"%c",key1_num);
		OLED_ShowString(73,16,(u8 *)text,16,1);
		
		OLED_ShowString(48,32,"K2:",16,1);
		sprintf(text,"%c",key2_num);
		OLED_ShowString(73,32,(u8 *)text,16,1);
		
		OLED_ShowString(48,48,"K3:",16,1);
		sprintf(text,"%c",key3_num);
		OLED_ShowString(73,48,(u8 *)text,16,1);
		
		OLED_Refresh();
		
	}
	else if(oled_ui==2)
	{
		OLED_ShowString(0,16,"tmp:",16,1);
		sprintf(text,"%d C",tmp);
		OLED_ShowString(40,16,(u8 *)text,16,1);
		
		OLED_ShowString(0,32,"humi:",16,1);
		sprintf(text,"%d RH",humi);
		OLED_ShowString(40,32,(u8 *)text,16,1);

		OLED_ShowString(0,64,"light:",16,1);
		sprintf(text,"%d Lux",light);
		OLED_ShowString(50,64,(u8 *)text,16,1);
		
		OLED_ShowString(0,50,"1/2",8,1);
		OLED_Refresh();
	}
	else if(oled_ui==3)
	{
		if(led1_flag)
		{
			GPIO_ResetBits(GPIOE, GPIO_Pin_8);
			OLED_ShowString(0,16,"LED1:ON",16,1);
		}
		else
		{	
			GPIO_SetBits(GPIOE, GPIO_Pin_8);
			OLED_ShowString(0,16,"LED1:OFF",16,1);
		}
		if(beep_flag)
		{
			beep_open();
			OLED_ShowString(0,32,"BEEP:ON",16,1);
		}
		else
		{
			beep_close();
			OLED_ShowString(0,32,"BEEP:OFF",16,1);
		}
		if(fan_flag)
		{
			OLED_ShowString(0,64,"FAN:ON",16,1);
		}
		else
		{
			OLED_ShowString(0,64,"FAN:OFF",16,1);
		}
		
		OLED_ShowString(0,50,"2/2",8,1);
		OLED_Refresh();
	}
}

