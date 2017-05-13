#ifndef __TRANSACTION_H__
#define __TRANSACTION_H__	 


//
#define		NO		0	//
#define		YES		1	//
//
#define		COMMODITY_NOLACK		0	//不缺货
#define		COMMODITY_LACK			1	//缺货
//
#define		PAY_SUCCESSED			1	//付款成功
#define		PAY_FAILED				0	//付款失败
//
#define		REQUEST_SUCCESSED			1	//请求成功
#define		REQUEST_FAILED			0	//请求失败
//
#define		PAY_WAITTIME			10	//付款等待时间-10S
#define		MB_RECWAITTIME			50	//付款等待时间-10S




//
#define		KEY_NULL		0	//空按键
#define		KEY_CD1			1 //商品1
#define		KEY_CD2			2 //商品2
#define		KEY_CD3			3 //商品3
#define		KEY_CD4			4 //商品4
#define		KEY_CD5			5 //商品5


//商品信息结构体
typedef struct	
{
	unsigned char Transaction_ID;//商品编号
	unsigned char Transaction_Num;//商品数量
	unsigned char Transaction_Price;//商品单价
}transaction_info_struct;

//商品状态结构体
typedef struct	
{
	unsigned char Transaction_Status;//商品状态
	unsigned char Transaction_BuyRequest;//商品购买请求
	unsigned char Transaction_PayStatus;//商品支付状态
}transaction_status_struct;






//
extern unsigned char Key_Scan(void);
extern unsigned char Key2CDID(unsigned char key);
extern unsigned char Commodity_ReserveCheck(unsigned char Commodity_Id);
extern unsigned char CheckPay_Status(unsigned char Commodity_Id);
extern void ResetDevice2InitSellMode(void);
extern void LcdShow_WaitPay(void);
extern void LcdShow_PayOutTime(void);
extern void LcdShow_PayComplete(void);
extern void Transaction_Process(unsigned char id);
extern void NoticeDevice_Action(void);
extern void Part_Init(void);
//
extern unsigned char Input_Key;//输入键值
extern unsigned char Transaction_Finished;//交易完成标志
extern unsigned char Commodity_Id;//商品ID
extern unsigned char Commodity_Lack;//商品缺货标志
extern unsigned char Pay_Status;//支付状态
extern unsigned char PayWaitTime;//支付等待时间


//
extern transaction_info_struct Device_Transaction_Info;  //该设备商品信息
extern transaction_status_struct Device_Transaction_Status;  //该设备商品状态























#endif
