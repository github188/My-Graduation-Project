#include "eep.h"
//#include "delay.h"
#include "systick.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h" 
#define FLASH_NSS_PIN		GPIO_Pin_12
#define FLASH_MISO_PIN		GPIO_Pin_14
#define FLASH_MOSI_PIN		GPIO_Pin_15
#define FLASH_SCK_PIN		GPIO_Pin_13
#define FLASH_GPIO  	GPIOB
#define FLASH_SET_NSS 	GPIO_SetBits(FLASH_GPIO,FLASH_NSS_PIN)
#define FLASH_RESET_NSS GPIO_ResetBits(FLASH_GPIO,FLASH_NSS_PIN)
#define FLASH_SPI SPI2
#define FLASH_SPI_RCC RCC_APB1Periph_SPI2
#define FLASH_GPIO_RCC RCC_AHB1Periph_GPIOB
#define FLASH_NSS_PINSOURCE		GPIO_PinSource12
#define FLASH_MISO_PINSOURCE		GPIO_PinSource14
#define FLASH_MOSI_PINSOURCE		GPIO_PinSource15
#define FLASH_SCK_PINSOURCE		GPIO_PinSource13 
#define FLASH_AF					GPIO_AF_SPI2

void delay(unsigned long nCount)
{
	sysTickDelayus(nCount);
}
	//Flash2驱动函数
//u8 EEP_ReadByte(void)
//{
//	u8 retry=0;				 
//	while((SPI2->SR&1<<0)==0) //等待接收完一个byte  
//	{
//		retry++;
//		if(retry>200)return 0;
//	}	  						    
//	return SPI2->DR;          //返回收到的数据				    
//}
//void EEP_WriteByte(u8 TxData)
//{
//	u8 retry=0;	
//	vu8 tDat;
//	tDat=TxData;			 
//	while(((SPI2->SR)&(1<<1))==0);//等待发送区空	
////	{
////		retry++;
////		if(retry>200)break;
////	}			  
//	SPI2->DR=tDat;	 	  //发送一个byte 
//}
//
u8 EEP_ReadWriteByte(u8 TxData)
{		
	u8 retry=0;				 
	while((SPI2->SR&1<<1)==0)//等待发送区空	
	{
		retry++;
		if(retry>200)return 0;
	}			  
	SPI2->DR=TxData;	 	  //发送一个byte 
	retry=0;
	while((SPI2->SR&1<<0)==0) //等待接收完一个byte  
	{
		retry++;
		if(retry>200)return 0;
	}	  						    
	return SPI2->DR;          //返回收到的数据				    
}


//u8 Read_Data_byte(u16 address)
//{
//	u8 tDat;
//	EEP_CS_LOW();                            //使能器件   
//	EEP_ReadWriteByte(0x03);   //发送写取状态寄存器命令 
//	   
//	while();
//}
//片选线函数
void EEP_CS_HIGH(void)
{											 
	delay(200);
	GPIO_SetBits(GPIOB,GPIO_Pin_12);
}

void EEP_CS_LOW(void)
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_12);
	delay(200);
}


//EEPROM SPI初始化
//void FlashHardwareConfig(void)
void EEP_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef  SPI_InitStruct;

	//F407的配置
	RCC_APB1PeriphClockCmd(FLASH_SPI_RCC , ENABLE);	  //SPI时钟配置
	RCC_AHB1PeriphClockCmd(FLASH_GPIO_RCC, ENABLE);
	SPI_Cmd(FLASH_SPI,DISABLE);

  	GPIO_PinAFConfig(FLASH_GPIO, FLASH_MISO_PINSOURCE, FLASH_AF);
  	GPIO_PinAFConfig(FLASH_GPIO, FLASH_MOSI_PINSOURCE, FLASH_AF);
  	GPIO_PinAFConfig(FLASH_GPIO, FLASH_SCK_PINSOURCE, FLASH_AF);

	GPIO_InitStructure.GPIO_Pin = FLASH_MISO_PIN |FLASH_MOSI_PIN |FLASH_SCK_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(FLASH_GPIO, &GPIO_InitStructure);


	GPIO_InitStructure.GPIO_Pin = FLASH_NSS_PIN;           //SPI NSS配置成片选口
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(FLASH_GPIO, &GPIO_InitStructure);

	//初始化SPI结构体
	//SPI_StructInit(&SPI_InitStruct);
	//SPI_Init(FLASH_SPI,&SPI_InitStruct);//初始化SPI
	SPI_InitStruct.SPI_Direction =SPI_Direction_2Lines_FullDuplex;  //两线全双工
	SPI_InitStruct.SPI_Mode =SPI_Mode_Master;       //主
	SPI_InitStruct.SPI_DataSize =SPI_DataSize_8b;      //8位
	SPI_InitStruct.SPI_CPOL =SPI_CPOL_High;        //CPOL=1时钟悬空高
	//   SPI_InitStruct.SPI_CPOL =SPI_CPOL_Low;        //CPOL=1时钟悬空高
	SPI_InitStruct.SPI_CPHA =SPI_CPHA_2Edge;       //CPHA=1 数据捕获第2个
	//   SPI_InitStruct.SPI_CPHA =SPI_CPHA_1Edge;       //CPHA=1 数据捕获第2个
	SPI_InitStruct.SPI_NSS =SPI_NSS_Soft;        //软件NSS
	SPI_InitStruct.SPI_BaudRatePrescaler =SPI_BaudRatePrescaler_8;  //2分频
	//   SPI_InitStruct.SPI_BaudRatePrescaler =SPI_BaudRatePrescaler_64;  //2分频
	SPI_InitStruct.SPI_FirstBit =SPI_FirstBit_MSB;      //高位在前
	SPI_InitStruct.SPI_CRCPolynomial =7;        //CRC7
	SPI_SSOutputCmd(FLASH_SPI, ENABLE);//使能NSS输出

	SPI_Init(FLASH_SPI,&SPI_InitStruct);

	SPI_Cmd(FLASH_SPI,ENABLE);

	FLASH_SET_NSS;
}

unsigned char SPI_MSTRS(unsigned char cData)
{
	u16 retry=0;				 
	while((SPI2->SR&1<<1)==0)//等待发送区空	
	{
		retry++;
		if(retry>200)return 0;
	}			  
	SPI2->DR=cData;	 	  //发送一个byte 
	retry=0;
	while((SPI2->SR&1<<0)==0) //等待接收完一个byte  
	{
		retry++;
		if(retry>200)return 0;
	}	  						    
	return SPI2->DR;          //返回收到的数据				    
////启动数据传输
//  SPDR = cData;
////等待数据传输结束
//  while(!(SPSR&(1<<SPIF)));
////发送完成后，清除发送标志
////  SPSR&=~(1<<SPIF);
//  return SPDR;
////每次发送数据的同时，也会接收到数据。
}

//写使能
void EEPROM_WREN()
{
	EEP_CS_LOW();
	SPI_MSTRS(0x06);
	EEP_CS_HIGH();
}
//写禁止
void EEPROM_WRDI()
{
	EEP_CS_LOW();
	SPI_MSTRS(0x04);
	EEP_CS_HIGH();
}
//写状态
void EEPROM_WRSR()
{
  EEPROM_WREN();
  EEP_CS_LOW(); //使能
  SPI_MSTRS(0x01);  //0x01为写状态寄存器的指令
  SPI_MSTRS(0x82);  //把0x82写入状态寄存器
  EEP_CS_HIGH(); //抬高片选，结束
}

/*
读状态寄存器。读SPI从机数据时，主机要给从机发送任意数据，才能收到从机返回的数据.
即0x00可以是任何数据（但是不能是25256的指令，例如0x01,0x04之类的）
*/
unsigned char EEPROM_RDSR()
{
  unsigned char StatusReg;
  EEPROM_WREN();
//  EEPROM_WRDI();  //如使用此句，不使用EEPROM_WREN()，则读出0x80
  EEP_CS_LOW(); //使能
  SPI_MSTRS(0x05); //0x05为读状态寄存器指令
  StatusReg=SPI_MSTRS(0x00); //给25256发送任意数据
/*
此处原为：
SPDR = 0x00;
while(!(SPSR&(1<<SPIF)));
StatusReg=SPDR;
时出错，读出数据都为0xff。想不清楚为什么
*/
  EEP_CS_HIGH();
  return StatusReg;
}
//25256字节写
void EEPROM_ByteWrite(unsigned int Address,unsigned char Data)
{
	u8 ti;
 	EEPROM_WREN();
  EEP_CS_LOW(); //使能
  SPI_MSTRS(0x02); //0x02为写指令
  SPI_MSTRS(Address/256); //地址高位
  SPI_MSTRS(Address%256); //地址低位
  SPI_MSTRS(Data); //写入数据  
  EEP_CS_HIGH();
	do{
	ti=EEPROM_RDSR();
	 }while((ti&0x01)==0x01);
	//delay(5000);
}

//25256页写。试验结果，只能连续写入三个字节
void EEPROM_PageWrite(unsigned int Address,unsigned char *Data,unsigned char NData)
{
  unsigned char i;
  EEPROM_WREN();
  EEP_CS_LOW(); //使能
  SPI_MSTRS(0x02);//页写指令
  SPI_MSTRS(Address/256); //地址高位
  SPI_MSTRS(Address%256); //地址低位
  for(i=0;i<NData;i++)
   {
     SPI_MSTRS(*Data); //写入数据 
  		Data++;
   } 
  EEP_CS_HIGH();
}

//25256读
unsigned char EEPROM_Read(unsigned int Address)
{
  unsigned char dataa;
  EEP_CS_LOW(); //使能
  SPI_MSTRS(0x03); //0x03为读25256的指令
  SPI_MSTRS(Address/256); //地址高位
  SPI_MSTRS(Address%256); //地址低位
  dataa=SPI_MSTRS(0x00);//发送任意数据
  EEP_CS_HIGH();
  return dataa;  //返回读到的数据
}



//读取SPI_FLASH的状态寄存器
//BIT7  6   5   4   3   2   1   0
//SPR   RV  TB BP2 BP1 BP0 WEL BUSY
//SPR:默认0,状态寄存器保护位,配合WP使用
//TB,BP2,BP1,BP0:FLASH区域写保护设置
//WEL:写使能锁定
//BUSY:忙标记位(1,忙;0,空闲)
//默认:0x00
u8 EEP_Flash_ReadSR(void)   
{  
	u8 byte=0;   
	EEP_CS_LOW();                            //使能器件   
	EEP_ReadWriteByte(AT25X_ReadStatusReg);    //发送读取状态寄存器命令    
	byte=EEP_ReadWriteByte(0xFF);             //读取一个字节  
	EEP_CS_HIGH();                            //取消片选     
	return byte;   
} 
//写SPI_FLASH状态寄存器
//只有SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)可以写!!!
void EEP_FLASH_Write_SR(u8 sr)   
{   
	EEP_CS_LOW();                            //使能器件   
	EEP_ReadWriteByte(AT25X_WriteStatusReg);   //发送写取状态寄存器命令    
	EEP_ReadWriteByte(sr);               //写入一个字节  
	EEP_CS_HIGH();                            //取消片选     	      
}   
//SPI_FLASH写使能	
//将WEL置位   
void EEP_FLASH_Write_Enable(void)   
{
	EEP_CS_LOW();                            //使能器件   
  EEP_ReadWriteByte(AT25X_WriteEnable);      //发送写使能  
	EEP_CS_HIGH();                            //取消片选     	      
} 
//SPI_FLASH写禁止	
//将WEL清零  
void EEP_FLASH_Write_Disable(void)   
{  
	EEP_CS_LOW();                            //使能器件   
    EEP_ReadWriteByte(AT25X_WriteDisable);     //发送写禁止指令    
	EEP_CS_HIGH();                            //取消片选     	      
} 			    

void EEP_Flash_Wait_Busy(void)   
{   
	while ((EEP_Flash_ReadSR()&0x01)==0x01);   // 等待BUSY位清空
}  

unsigned char Read_Data_byte(unsigned int address)
{
	//read sequence 
	u8 tDat;
	u8 ri;
	EEP_CS_LOW();                            //使能器件   
	for(ri=0;ri<8;ri++);
	EEP_ReadWriteByte(AT25X_ReadData); // read instruction
 	EEP_ReadWriteByte(address >> 8); // send out 8-bit higher address 
 	EEP_ReadWriteByte(address&0xFF); // send out 8-bit lower address
 	tDat=	EEP_ReadWriteByte(0xFF);
	EEP_Flash_Wait_Busy();
	EEP_CS_HIGH();                            //取消片选     	      
 	return tDat;

}

void Write_Data_byte(unsigned int address, unsigned char input_byte) 
{
//	unsigned char ri;
	EEP_FLASH_Write_Enable();
	delay(100);
	EEP_Flash_ReadSR(); // this is necessary for write
	delay(100);

	//write byte sequence 
 	EEP_CS_LOW();
	//for(ri=0;ri<8;ri++);
 	delay(100);
	EEP_ReadWriteByte(0x02);   // write instruction
 	delay(100);
 	EEP_ReadWriteByte(address >> 8);  // send out high 8-bit address 
 	delay(100);
 	EEP_ReadWriteByte((u8)address);  // send out low 8-bit address 
 	delay(100);
 	EEP_ReadWriteByte(input_byte);
	delay(2000);
	//while(SSC0_CON_AREN_BSY)_srvwdt_(); // wait until transfer complete 

 	//EEP_Flash_Wait_Busy();
 	EEP_CS_HIGH();
 	delay(100);
	//EEP_FLASH_Write_Disable();

}
void EepRd(unsigned int ReadAddr,unsigned int NumByteToWrite)
{
	u16 i;
	for(i=0;i<NumByteToWrite;i++)
	{
	  EepRdBuf[i]=EEPROM_Read(ReadAddr+i);
	}
}
//void EEP_Read(char *pBuffer,u16 ReadAddr,u16 NumByteToRead) 
//{
//	u16 i;
//	for(i=0;i<NumByteToRead;i++)
//	{
//	 pBuffer[i]=Read_Data_byte(ReadAddr+i);
//	}
//
//}
void EEP_Read(char *pBuffer,u16 ReadAddr,u16 NumByteToRead) 
{
	u16 i;
	for(i=0;i<NumByteToRead;i++)
	{	 pBuffer[i]=Read_Data_byte(ReadAddr+i);	}

}
void EepWr(unsigned int ReadAddr,unsigned int NumByteToWrite)
{
	u16 i;
	for(i=0;i<NumByteToWrite;i++)
	{	 EEPROM_ByteWrite(ReadAddr+i,EepWrBuf[i]);	}
}
void EEP_Write(char *pBuffer,u16 ReadAddr,u16 NumByteToWrite) 
{
	u16 i;
	for(i=0;i<NumByteToWrite;i++)
	{  Write_Data_byte(ReadAddr+i,pBuffer[i]);	}
}
////连续读取SPI FLASH  
////在指定地址开始读取指定长度的数据
////pBuffer:数据存储区
////ReadAddr:开始读取的地址(15bit)
////NumByteToRead:要读取的字节数(最大65535)
//void EEP_Read(char *pBuffer,u16 ReadAddr,u16 NumByteToRead)   
//{ 
// 	u16 i;    												    
//	EEP_CS_LOW();                            //使能器件   
//	for(i=0;i<8;i++);
//	EEP_ReadWriteByte(AT25X_ReadData); // read instruction
// 	EEP_ReadWriteByte(ReadAddr >> 8); // send out 8-bit higher address 
// 	EEP_ReadWriteByte((u8)ReadAddr); // send out 8-bit lower address
// 	for(i=0;i<NumByteToRead;i++)
//	{
//		pBuffer[i]=	EEP_ReadWriteByte(0xFF);
//	}
//	EEP_CS_HIGH();                            //取消片选     	      
//	EEP_Flash_Wait_Busy();
//}  
//
//void EEP_Write(char *pBuffer,u16 ReadAddr,u16 NumByteToWrite) 
//{
//	u16 i;
//	char tBuf[1000];
// 	for(i=0;i<NumByteToWrite;i++)
//		tBuf[i]=pBuffer[i];
//	EEP_FLASH_Write_Enable();
//	EEP_Flash_ReadSR(); // this is necessary for write
//
//
//	//write byte sequence 
// 	EEP_CS_LOW();
//	for(i=0;i<8;i++);
//
// 	EEP_ReadWriteByte(0x02);   // write instruction
// 	EEP_ReadWriteByte(ReadAddr >> 8);  // send out high 8-bit address 
// 	EEP_ReadWriteByte((u8)ReadAddr);  // send out low 8-bit address 
// 	for(i=0;i<NumByteToWrite;i++)
// 	{	
//		EEP_ReadWriteByte(tBuf[i]);
// 		EEP_Flash_Wait_Busy();
// 	}
//	EEP_CS_HIGH();
//	delay(2000);
//
// 	EEP_Flash_Wait_Busy();
//}


