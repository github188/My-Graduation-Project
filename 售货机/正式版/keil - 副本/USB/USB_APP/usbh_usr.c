#include "usbh_usr.h" 
#include "ff.h"

static u8 AppState;//usb状态<第一次用于初始化，之后运行用户测试程序>
extern USB_OTG_CORE_HANDLE  USB_OTG_Core;//usb内核结构体
extern 	u8 UPDATE_CHECK;
//USB OTG 中断服务函数
//处理所有USB中断
void OTG_FS_IRQHandler(void)
{
  	USBH_OTG_ISR_Handler(&USB_OTG_Core);
} 
//USB HOST 用户回调函数.
USBH_Usr_cb_TypeDef USR_Callbacks=
{
	USBH_USR_Init,
	USBH_USR_DeInit,
	USBH_USR_DeviceAttached,
	USBH_USR_ResetDevice,
	USBH_USR_DeviceDisconnected,
	USBH_USR_OverCurrentDetected,
	USBH_USR_DeviceSpeedDetected,
	USBH_USR_Device_DescAvailable,
	USBH_USR_DeviceAddressAssigned,
	USBH_USR_Configuration_DescAvailable,
	USBH_USR_Manufacturer_String,
	USBH_USR_Product_String,
	USBH_USR_SerialNum_String,
	USBH_USR_EnumerationDone,
	USBH_USR_UserInput,
	USBH_USR_MSC_Application,
	USBH_USR_DeviceNotSupported,
	USBH_USR_UnrecoveredError
};
/////////////////////////////////////////////////////////////////////////////////
//以下为各回调函数实现.

//USB HOST 初始化 
void USBH_USR_Init(void)
{
}
//检测到U盘插入
void USBH_USR_DeviceAttached(void)//U盘插入
{
}
//检测到U盘拔出
void USBH_USR_DeviceDisconnected (void)//U盘移除
{
}  
//复位从机
void USBH_USR_ResetDevice(void)
{
}
//检测到从机速度
//DeviceSpeed:从机速度(0,1,2 / 其他)
void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed)
{
	if(DeviceSpeed==HPRT0_PRTSPD_HIGH_SPEED)
	{
 	}  
	else if(DeviceSpeed==HPRT0_PRTSPD_FULL_SPEED)
	{
	}
	else if(DeviceSpeed==HPRT0_PRTSPD_LOW_SPEED)
	{
	}
	else
	{
	}
}
//检测到从机的描述符
//DeviceDesc:设备描述符指针
void USBH_USR_Device_DescAvailable(void *DeviceDesc)
{ 
}
//从机地址分配成功
void USBH_USR_DeviceAddressAssigned(void)
{
}
//配置描述符获有效
void USBH_USR_Configuration_DescAvailable(USBH_CfgDesc_TypeDef * cfgDesc,
                                          USBH_InterfaceDesc_TypeDef *itfDesc,
                                          USBH_EpDesc_TypeDef *epDesc)
{
	USBH_InterfaceDesc_TypeDef *id; 
	id = itfDesc;   
	if((*id).bInterfaceClass==0x08)
	{
//		printf("可移动存储器设备!\r\n"); 
	}else if((*id).bInterfaceClass==0x03)
	{
//		printf("HID 设备!\r\n"); 
	}    
}
//获取到设备Manufacturer String
void USBH_USR_Manufacturer_String(void *ManufacturerString)
{
//	printf("Manufacturer: %s\r\n",(char *)ManufacturerString);
}
//获取到设备Product String 
void USBH_USR_Product_String(void *ProductString)
{
//	printf("Product: %s\r\n",(char *)ProductString);  
}
//获取到设备SerialNum String 
void USBH_USR_SerialNum_String(void *SerialNumString)
{
//	printf("Serial Number: %s\r\n",(char *)SerialNumString);    
} 
//设备USB枚举完成
void USBH_USR_EnumerationDone(void)
{ 
//	printf("设备枚举完成!\r\n\r\n");    
} 
//无法识别的USB设备
void USBH_USR_DeviceNotSupported(void)
{
//	printf("无法识别的USB设备!\r\n\r\n");    
}  
//等待用户输入按键,执行下一步操作
USBH_USR_Status USBH_USR_UserInput(void)
{ 
//	printf("跳过用户确认步骤!\r\n");
	return USBH_USR_RESP_OK;
} 
//USB接口电流过载
void USBH_USR_OverCurrentDetected (void)
{
//	printf("端口电流过大!!!\r\n");
} 

extern u8 USH_User_App(void);		//用户测试主程序
//USB HOST MSC类用户应用程序
int USBH_USR_MSC_Application(void)
{
	u8 res=0;
  	switch(AppState)
  	{
    	case USH_USR_FS_INIT://初始化文件系统 
			AppState=USH_USR_FS_TEST;
      		break;
    	case USH_USR_FS_TEST:	//执行USB OTG 测试主程序	
			USH_User_App(); //用户主程序
			res=0;
			if(res)AppState=USH_USR_FS_INIT;//这里用FATFS，所以文件系统初始化不在这里进行
      		break;
    	default:break;
  	} 
	return res;
}
//用户要求重新初始化设备
void USBH_USR_DeInit(void)
{
  	AppState=USH_USR_FS_INIT;//USH_USR_FS_TEST
}
//无法恢复的错误!!  
void USBH_USR_UnrecoveredError (void)
{
//	printf("无法恢复的错误!!!\r\n\r\n");	
}
////////////////////////////////////////////////////////////////////////////////////////
//用户定义函数,实现fatfs diskio的接口函数 
extern USBH_HOST              USB_Host;

//获取U盘状态
//返回值:0,U盘未就绪
//      1,就绪
u8 USBH_UDISK_Status(void)
{
	return HCD_IsDeviceConnected(&USB_OTG_Core);//返回U盘状态
}

//读U盘
//buf:读数据缓存区
//sector:扇区地址
//cnt:扇区个数	
//返回值:错误状态;0,正常;其他,错误代码;		 
u8 USBH_UDISK_Read(u8* buf,u32 sector,u32 cnt)
{
	u8 res=1;
	if(HCD_IsDeviceConnected(&USB_OTG_Core)&&AppState==USH_USR_FS_TEST)//连接还存在,且是APP测试状态
	{  		    
		do
		{
			res=USBH_MSC_Read10(&USB_OTG_Core,buf,sector,512*cnt);
			USBH_MSC_HandleBOTXfer(&USB_OTG_Core ,&USB_Host);		      
			if(!HCD_IsDeviceConnected(&USB_OTG_Core))
			{
				res=1;//读写错误
				break;
			};   
		}while(res==USBH_MSC_BUSY);
	}else res=1;		  
	if(res==USBH_MSC_OK)res=0;	
	return res;
}

//写U盘
//buf:写数据缓存区
//sector:扇区地址
//cnt:扇区个数	
//返回值:错误状态;0,正常;其他,错误代码;		 
u8 USBH_UDISK_Write(u8* buf,u32 sector,u32 cnt)
{
	u8 res=1;
	if(HCD_IsDeviceConnected(&USB_OTG_Core)&&AppState==USH_USR_FS_TEST)//连接还存在,且是APP测试状态
	{  		    
		do
		{
			res=USBH_MSC_Write10(&USB_OTG_Core,buf,sector,512*cnt); 
			USBH_MSC_HandleBOTXfer(&USB_OTG_Core ,&USB_Host);		      
			if(!HCD_IsDeviceConnected(&USB_OTG_Core))
			{
				res=1;//读写错误
				break;
			};   
		}while(res==USBH_MSC_BUSY);
	}else res=1;		  
	if(res==USBH_MSC_OK)res=0;	
	return res;
}












































