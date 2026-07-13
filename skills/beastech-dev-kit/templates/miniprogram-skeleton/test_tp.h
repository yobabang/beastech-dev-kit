#ifndef test_can_tp_api
#define test_can_tp_api
#include "test_common.h"
#include "TSMaster.h"
#include <sstream>
#include "TestReport.h"

extern s32 CheckNonResponse(u16 WaitTime);

//总线类型定义
enum TP { TP_ON = 1, TP_OFF = 0 };
enum BusType { CAN = 1, CANFD = 2, LIN = 3, Flexray = 4, Ethernet = 5 };
enum BusType Bustype;
u8 BusTypeNow = CANFD;

//CAN帧定义
TCAN TScanReq = { Diag_ch,1,8,0,Diag_Request_ID,0,{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0} };
TCAN TScanResp = { Diag_ch,0,8,0,Diag_Response_ID,0,{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0} };
TCAN TScanInvalid = { Diag_ch,1,8,0,-1,0,{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}};
TCAN TesterPresent = { Diag_ch,1,8,0,Diag_Function_ID,0,{0x02,0x3E,0x80,0xAA,0xAA,0xAA,0xAA,0xAA} };
//CANFD帧定义
TCANFD TScanfdReq = { Diag_ch,1,8,3,Diag_Request_ID,0,{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0} };; //第四个参数3是开启BRS
TCANFD TScanfdResp = { Diag_ch,0,8,1,Diag_Response_ID,0,{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0} };
TCANFD TScanfdInvalid = { Diag_ch,0,8,1,-1,0,{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0} };
TCANFD TesterPresentFD = { Diag_ch,0,8,3,Diag_Function_ID,0,{0x02,0x3E,0x80,0xAA,0xAA,0xAA,0xAA,0xAA} };
//CAN/FD首帧流控帧参数
u16 FFLength = 0,CFLength = 0;
u8 CFcnt = 0;
u8 FCcnt = 0;
u8 remainingFrames = 0;
s64 firstCFtime = 0;

TLIN TSlinReq = { Diag_ch,0,1,8,0x3C,0,0,0,{NAD,0,0,0,0,0,0,0} };
TLIN TSlinResp = { Diag_ch,0,0,8,0x3D,0,0,0,{NAD,0,0,0,0,0,0,0} };

//NM唤醒报文
//u8 NMWakeUpChn = NetworkCluster["ZCU_CANFD1"];
TCAN NMWakeUpMsg = { Diag_ch,1,8,0,0x53f,0,{0x3F,0x40,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF} };
TCAN ZCUDWakeUpMsg = { NetworkCluster["ZCU_CANFD1"],1,8,0,0x53f,0,{0x3F,0x40,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}};
TCAN DUTTestBusWakeUpMsg = { 3,1,8,0,0x53f,0,{0x3F,0x40,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF} };

s32 NMWakeUp(u8 wakeup,u8 channel = -1)
{
	if (wakeup == 1)
	{
		ZCUDWakeUpMsg.FIdxChn = NetworkCluster["ZCU_CANFD1"];
		//log("NetworkCluster[ZCU_CANFD1] is %d", NetworkCluster["ZCU_CANFD1"]);
		TestReport_HTML_StepDescription("Start Simulate NM WakeUp Message", Info);
		com.add_cyclic_message_can(&NMWakeUpMsg, 1000);
		//log("ZCU NM WakeUp Message Chn is %d", ZCUDWakeUpMsg.FIdxChn);
		com.add_cyclic_message_can(&ZCUDWakeUpMsg, 1000);
		DUTTestBusWakeUpMsg.FIdxChn = channel;
		//log("DUTTestBusWakeUpMsg Chn is %d", DUTTestBusWakeUpMsg.FIdxChn);
		com.add_cyclic_message_can(&DUTTestBusWakeUpMsg, 1000);

		return 0;
	}
	else
	{
		TestReport_HTML_StepDescription("Stop Simulate NM WakeUp Message", Info);
		com.del_cyclic_message_can(&NMWakeUpMsg);
		com.del_cyclic_message_can(&ZCUDWakeUpMsg);
		com.del_cyclic_message_can(&DUTTestBusWakeUpMsg);
		return -1;
	}
}
/*------------variables of FDC10 test--------------*/
//TMPTimerMS T_StepUpTest;
TMPTimerMS T_StepDownTest;
u8 gtestDTC[3];
int gstepValue, gtestIndex;
int gfdc1, gfdc2;
int gUnCofrmLmt = 0;
int gsnapshotFlag = 0;
int gLastIndex, gReadEndFlag;

//如果等到了报文，ret=0
s32 WaitMessage( u32 WaitTime,bool IS_TX=false)
{
	u8 ret = 0;

	if(BusTypeNow==CAN)
	{
		TScanResp.is_tx = IS_TX;
		ret = com.wait_can_message(&TScanInvalid, &TScanResp, WaitTime);//qq:这句话会自动打印 CAN报文接收超时等
	}
	else if(BusTypeNow==CANFD)
	{
		TScanfdResp.is_tx = IS_TX;
		ret = com.wait_canfd_message(&TScanfdInvalid, &TScanfdResp, WaitTime);
	}
	//else if (BusTypeNow == LIN)
	//{
	//	TSlinResp.is_tx = IS_TX;
	//	ret = com.wait
	//}

	if (ret == 0)
		return 0;
	else
		return -1;
}

s32 CheckNoResp(u32 time)
{
	u8 ret = 0;
	
	ret = WaitMessage(time);

	if (ret == 0)
	{
		log("ECU resp");
		return -1;
	}
	else
	{
		log("ECU no resp");
		return 0;
	}

	app.wait(50, "");
}

s32 TP_SendMessage(u8 AddressMode = 0)
{
	//log("222");

	if(BusTypeNow==CAN)
	{
		if (AddressMode == 0)
		{
			TScanReq.FIdentifier = Diag_Request_ID;

			if (0 == com.transmit_can_async(&TScanReq))
			{
				log("发送报文 ID：0x%x", TScanResp.FIdentifier);
			}
			else
			{
				log("未发送报文");
				return -1;
			}
		}
		else
		{
			TScanReq.FIdentifier = Diag_Function_ID;
			if (0 == com.transmit_can_async(&TScanReq))
			{
				//printf("收到报文 ID：0x%x", TScanResp.FIdentifier);

			}
			else
			{
				//printf("未收到报文");
				return -1;
			}
		}
	}
	else if(BusTypeNow==CANFD)
	{
		if (AddressMode == 0)
		{
			TScanfdReq.FIdentifier = Diag_Request_ID;

			if (0 == com.transmit_canfd_async(&TScanfdReq))
			{
				//log("收到报文 ID：0x%x", TScanfdResp.FIdentifier);
			}
			else
			{
				//log("未收到报文");
				return -1;
			}
		}
		else
		{
			TScanfdReq.FIdentifier = Diag_Request_ID;
			if (0 == com.transmit_canfd_async(&TScanfdReq))
			{
				//printf("收到报文 ID：0x%x", TScanfdResp.FIdentifier);
			}
			else
			{
				//printf("未收到报文");
				return -1;
			}
		}
	}
	else if (BusTypeNow==LIN)
	{
		if (0 != app.connect()) return -1;
		if (0 == com.transmit_lin_sync(&TSlinReq, 1000)) {
			log("LIN message 0x%02x has been transmitted on LIN Channel 1", TSlinReq.FIdentifier);
		}

	}

	return 0;
}

s32 CheckCurrentSessionTP(u8 ActiveSession=0,u8 AddressMode = 0)
{
	//log("111");
	if(BusTypeNow==CAN)
	{
		TScanReq.FData[0] = 0x03;
		TScanReq.FData[1] = 0x22;
		TScanReq.FData[2] = 0xF1;
		TScanReq.FData[3] = 0x86;
		TScanReq.FData[4] = 0x00;
		TScanReq.FData[5] = 0x00;
		TScanReq.FData[6] = 0x00;
		TScanReq.FData[7] = 0x00;

		TP_SendMessage(AddressMode);
		if (0 == WaitMessage(1000) && TScanResp.FData[4] == ActiveSession)
			return 0;
		else
			return -1;
	}
	else if (BusTypeNow == CANFD)
	{

		TScanfdReq.FDLC = 8;
		TScanfdReq.FData[0] = 0x03;
		TScanfdReq.FData[1] = 0x22;
		TScanfdReq.FData[2] = 0xF1;
		TScanfdReq.FData[3] = 0x86;
		TScanfdReq.FData[4] = 0x00;
		TScanfdReq.FData[5] = 0x00;
		TScanfdReq.FData[6] = 0x00;
		TScanfdReq.FData[7] = 0x00;

		TP_SendMessage(AddressMode);
		if (0 == WaitMessage(1000) && TScanfdResp.FData[4] == ActiveSession)
			return 0;
		else
			return -1;
	}

	return 0;
}



s32 SendCanSingleFrame(u8 Length, u8* data, u8 AddressMode = 0)
{
	if(BusTypeNow==CAN)
	{
		if (Length <= 7)
		{
			TScanReq.FData[0] = Length;
			for (int i = 0; i < Length; i++)
			{
				TScanReq.FData[i + 1] = data[i];
			}
			for (int j = 7; j > Length; j--)
			{
				TScanReq.FData[j] = 0xAA;//填充0xAA
			}

			TP_SendMessage(AddressMode);
			return 0;
		}
		else
		{
			app.log_text("Warning! 单帧数据长度大于7！", lvlWarning);
			return -1;
		}
	}
	else if (BusTypeNow == CANFD)
	{
		if (Length <= 7)
		{
			TScanfdReq.FDLC = 8;
			TScanfdReq.FData[0] = Length;
			for (int i = 0; i < Length; i++)
			{
				TScanfdReq.FData[i + 1] = data[i];
			}
			TP_SendMessage(AddressMode);
			return 0;
		}
		else if (Length <= 62)
		{
			TScanfdReq.FDLC = 15;
			TScanfdReq.FData[0] = 0x00;
			TScanfdReq.FData[1] = Length;
			for (int i = 0; i < Length; i++)
			{
				TScanfdReq.FData[i + 2] = data[i];
			}
			TP_SendMessage(AddressMode);
			return 0;
		}
		else
		{
			app.log_text("Warning! 单帧数据长度大于62！", lvlWarning);
			return -1;
		}
	}
	return 0;
}

s32 SendCanFirstFrame(u32 TX_DL, u8* data ,u8 AddressMode = 0)
{
	if(BusTypeNow==CAN)
	{
		TScanReq.FData[0] = 0x10 + (TX_DL >> 8);
		TScanReq.FData[1] = TX_DL & 0xFF;
		for (int i = 0; i < 6; i++)
		{
			TScanReq.FData[i + 2] = data[i];
		}
	}
	else if (BusTypeNow == CANFD)
	{
		TScanfdReq.FDLC = 15;
		if (TX_DL <= 4095)
		{
			TScanfdReq.FData[0] = 0x10 + (TX_DL >> 8);
			TScanfdReq.FData[1] = TX_DL & 0xFF;
			for (int i = 0; i < 62; i++)
			{
				TScanfdReq.FData[i + 2] = data[i];
			}
			if (0 != TP_SendMessage(AddressMode))
				return -1;
			else
				return 0;
		}
		else
		{
			TScanfdReq.FData[0] = 0x10;
			TScanfdReq.FData[1] = 0x00;
			TScanfdReq.FData[2] = TX_DL >> 24;
			TScanfdReq.FData[3] = (TX_DL >> 16) & 0xff;
			TScanfdReq.FData[4] = (TX_DL >> 8) & 0xff;
			TScanfdReq.FData[5] = TX_DL & 0xff;

			for (int i = 0; i < 56; i++)
			{
				TScanfdReq.FData[i + 6] = data[i];
			}
			if (0 != TP_SendMessage(AddressMode))
				return -1;
			else
				return 0;
		}
	}

	if (0 != TP_SendMessage(AddressMode))
		return -1;
	else
		return 0;
}

s32 SendCanConsecutiveFrame(u8 SequenceNumber, u8* data)
{
	if(BusTypeNow==CAN)
	{
		if (SequenceNumber > 0x0F)
		{
			app.log_text("SequenceNumber of Consecutive Frame shall not greater than 0x0F", lvlError);
			return -1;
		}
		TScanReq.FData[0] = 0x20 + SequenceNumber;
		for (int i = 0; i < 7; i++)
		{
			TScanReq.FData[i + 1] = data[i];
		}
	}
	else if (BusTypeNow == CANFD)
	{
		if (SequenceNumber > 0x0F)
		{
			app.log_text("SequenceNumber of Consecutive Frame shall not greater than 0x0F", lvlError);
			return -1;
		}
		TScanfdReq.FData[0] = 0x20 + SequenceNumber;

		//if (DLC == 8)
		//{
		//	TScanfdReq.FDLC = 8;
		//	for (int i = 0; i < 7; i++)
		//	{
		//		TScanfdReq.FData[i + 1] = data[i];
		//	}
		//}
		//else
		//{
		TScanfdReq.FDLC = 15;
		for (int i = 0; i < 63; i++)
		{
			TScanfdReq.FData[i + 1] = data[i];
		}
		//}
	}

	if (0 != TP_SendMessage(0))
		return -1;
	else
		return 0;
}

s32 SendCanFlowControlFrame(u8 FlowStatus,u8 BlockSize, u8 STmin,u8 AddressMode = 0)
{
	if(BusTypeNow==CAN)
	{
		TScanReq.FData[0] = 0x30 + FlowStatus;
		TScanReq.FData[1] = BlockSize;
		TScanReq.FData[2] = STmin;

		for (int j = 7; j > 2; j--)
		{
			TScanReq.FData[j] = 0x00;
		}
	}
	else
	{
		TScanfdReq.FData[0] = 0x30 + FlowStatus;
		TScanfdReq.FData[1] = BlockSize;
		TScanfdReq.FData[2] = STmin;

		for (int j = 7; j > 2; j--)
		{
			TScanfdReq.FData[j] = 0x00;
		}
	}


	if (0 != TP_SendMessage(AddressMode))
	{
		if (FlowStatus == 1)
			CheckNonResponse(Diag_TP_N_Bs*0.95);//如果是1，可能会超时，暂定0.95
		return -1;
	}
	else
	{
		if (FlowStatus == 1)
			CheckNonResponse(Diag_TP_N_Bs*0.95);
		return 0;
	}
	//return 0;

}

s32 CheckCanSingleFrame(u32 WaitTime)
{
	if (0 != WaitMessage(WaitTime))
	{
		//app.log_text("ECU does not response!", lvlError);
		strcpy(issuereason, "ECU does not response!");
		strcpy(requestInfo, "");
		strcpy(actualResponse, "");
		strcpy(expectedResponse, "");
		TestReport_HTML_Step(StepNumber, issuereason, Fail);
		resultFlag++;
		return -1;
	}

	if(BusTypeNow==CAN)
	{
		if (TScanResp.FDLC == 8 && ((TScanResp.FData[0] & 0xF0) == 0x00))
		{
			app.log_text("ECU responds a SF", lvlOK);
			return 0;
		}
	}
	else if (BusTypeNow == CANFD)
	{
		if ((TScanfdResp.FDLC == 8 || TScanfdResp.FDLC == 15) && ((TScanfdResp.FData[0] & 0xF0) == 0x00))
		{
			app.log_text("ECU responds a SF", lvlOK);
			return 0;
		}
	}
	else
	{
		app.log_text("ECU does not response a SF", lvlError);
		strcpy(issuereason, "ECU does not response a SF, ECU returns an unexpected frame.");
		TestReport_HTML_Step(StepNumber, issuereason, Fail);
		resultFlag++;
		return -1; 
	}

	return 0;
}

s32 CheckCanFirstFrame(u32 WaitTime)
{
	if (0 != WaitMessage(WaitTime))
	{
		app.log_text("ECU does not response!", lvlError);
		strcpy(issuereason, "ECU does not response!");
		TestReport_HTML_Step(StepNumber, issuereason, Fail);
		resultFlag++;
		return -1;
	}

	if(BusTypeNow==CAN)
	{
		FFLength = ((TScanResp.FData[0] & 0x0F) << 8) + TScanResp.FData[1];
		if (TScanResp.FDLC == 8 && ((TScanResp.FData[0] & 0xF0) == 0x10))
		{
			app.log_text(("ECU responds a FF, Datalength is " + std::to_string(FFLength)).c_str(), lvlOK);
			return 0;
		}
	}
	else if (BusTypeNow == CANFD)
	{
		if (TScanfdResp.FData[1] != 0x00 && ((TScanfdResp.FData[0] & 0xF0) == 0x10))
			FFLength = ((TScanfdResp.FData[0] & 0x0F) << 8) + TScanfdResp.FData[1];
		else
			FFLength = (TScanfdResp.FData[2] << 24) + (TScanfdResp.FData[3] << 16) + (TScanfdResp.FData[4] << 8) + TScanfdResp.FData[5];

		if ((TScanfdResp.FDLC == 8 || (TScanfdResp.FDLC == 15)))
		{
			app.log_text(("ECU responds a FF, Datalength is " + std::to_string(FFLength)).c_str(), lvlOK);
			return 0;
		}
	}

	else
	{
		app.log_text("ECU does not respond a FF", lvlError);
		strcpy(issuereason, "ECU does not response a SF, ECU returns an unexpected frame.");
		TestReport_HTML_Step(StepNumber, issuereason, Fail);
		resultFlag++;
		return -1;
	}

	return 0;
}

s32 CheckCanConsecutiveFrame(u32 WaitTime)
// 检查是否可以连续帧（CF）发送的函数。根据当前总线类型（CAN或CANFD）和等待时间，计算并等待连续帧的数量，然后根据响应判断是否成功。
{
	u8 ret;

	if (BusTypeNow == CAN)
		// 如果当前总线类型是CAN
	{
		CFcnt = ((FFLength - 6) + 6) / 7; // 优化计算
		for (int j = 0; j < CFcnt; j++)
			// 循环等待每帧消息
		{
			ret = WaitMessage(WaitTime);
			if (ret != 0) break;
			else if(j==0)
				firstCFtime = TScanResp.FTimeUs;
		}

		if ((TScanResp.FData[0] & 0xF0) == 0x20)
			// 如果接收到的数据帧的第一个字节的高四位是0x20
		{
			app.log_text("ECU responds a CF", lvlOK);
			return 0;
		}
		else
			return 1;
	}
	else if (BusTypeNow == CANFD)
		// 如果当前总线类型是CANFD
	{
		if (TScanfdResp.FDLC == 8)
			// 如果数据长度码（DLC）是8
		{
			CFcnt = ((FFLength - 6) + 6) / 7; // 优化计算
			for (int j = 1; j < CFcnt; j++)
				// 循环等待每帧消息
			{
				ret = WaitMessage(WaitTime);
				if (ret != 0) break;
				else if (j == 1)
					firstCFtime = TScanResp.FTimeUs;
			}
		}
		else
		{
			CFcnt = ((FFLength - 6) + 57) / 63; // 优化计算
			for (int j = 1; j < CFcnt; j++)
				// 循环等待每帧消息
			{
				ret = WaitMessage(WaitTime);
				if (ret != 0) break;
				else if (j == 1)
					firstCFtime = TScanResp.FTimeUs;
			}
		}

		if ((TScanfdResp.FData[0] & 0xF0) == 0x20)
			// 如果接收到的数据帧的第一个字节的高四位是0x20
		{
			app.log_text("ECU responds a CF", lvlOK);
			return 0;
		}
	}
	else
		// 如果当前总线类型既不是CAN也不是CANFD
	{
		app.log_text("ECU does not respond a CF", lvlError);
		strcpy(issuereason, "ECU does not respond a CF, ECU returns an unexpected frame.");
		TestReport_HTML_Step(StepNumber, issuereason, Fail);
		resultFlag++;
		return -1;
	}

	return 0;
}


s32 CheckCanFlowControlMessage(u32 WaitTime,u8 STMin=0x00)
{
	if (0 != WaitMessage(WaitTime))
	{
		app.log_text("ECU does not response!", lvlError);
		strcpy(issuereason, "ECU does not response!");
		TestReport_HTML_Step(StepNumber, issuereason, Fail);
		resultFlag++;
		return -1;
	}

	if(BusTypeNow==CAN)
	{
		if (TScanResp.FDLC == 8 && (TScanResp.FData[0] & 0xF0) == 0x30)
		{
			if (TScanResp.FData[3] == 0x00 && TScanResp.FData[4] == 0x00 && TScanResp.FData[5] == 0x00 && TScanResp.FData[6] == 0x00 && TScanResp.FData[7] == 0x00)
			{
				if (TScanResp.FData[1] != 0x00)
				{
					app.log_text("The BS of FC is not correct!", lvlError);
					strcpy(issuereason, "The BS of FC is not correct!");
					TestReport_HTML_Step(StepNumber, issuereason, Fail);
					resultFlag++;
					return -1;
				}

				if (TScanResp.FData[2] != STMin)
				{
					app.log_text("The STmin of FC is not correct!", lvlError);
					strcpy(issuereason, "The STmin of FC is not correct!");
					TestReport_HTML_Step(StepNumber, issuereason, Fail);
					resultFlag++;
					return -1;
				}
				return 0;
			}
			else
			{
				app.log_text("The padding of FC is not correct!", lvlError);
				strcpy(issuereason, "The padding of FC is not correct!");
				TestReport_HTML_Step(StepNumber, issuereason, Fail);
				resultFlag++;
				return -1;
			}
		}
	}
	else if (BusTypeNow==CANFD)
	{
		if (TScanfdResp.FDLC == 8 && (TScanfdResp.FData[0] & 0xF0) == 0x30)
		{
			if (TScanfdResp.FData[3] == 0x00 && TScanfdResp.FData[4] == 0x00 && TScanfdResp.FData[5] == 0x00 && TScanfdResp.FData[6] == 0x00 && TScanfdResp.FData[7] == 0x00)
			{
				if (TScanfdResp.FData[1] != 0x00)
				{
					app.log_text("The BS of FC is not correct!", lvlError);
					strcpy(issuereason, "The BS of FC is not correct!");
					TestReport_HTML_Step(StepNumber, issuereason, Fail);
					resultFlag++;
					return -1;
				}

				if (TScanfdResp.FData[2] != STMin)
				{
					app.log_text("The STmin of FC is not correct!", lvlError);
					strcpy(issuereason, "The STmin of FC is not correct!");
					TestReport_HTML_Step(StepNumber, issuereason, Fail);
					resultFlag++;
					return -1;
				}
				return 0;
			}
			else
			{
				app.log_text("The padding of FC is not correct!", lvlError);
				strcpy(issuereason, "The padding of FC is not correct!");
				TestReport_HTML_Step(StepNumber, issuereason, Fail);
				resultFlag++;
				return -1;
			}
			return 0;
		}
	}
	else
	{
		app.log_text("DUT does not response a FC!", lvlError);
		strcpy(issuereason, "ECU does not response a FC, ECU returns an unexpected frame.");
		TestReport_HTML_Step(StepNumber, issuereason, Fail);
		resultFlag++;
		return -1;
	}

	return 0;
}



s32 CAN_TP_TesterPresent(u8 Testpresent, u64 PeriodTime = Diag_UDS_S3Client)
{
	TesterPresent.FIdxChn = NetworkCluster[TpBusName_Temp];
	TesterPresentFD.FIdxChn = NetworkCluster[TpBusName_Temp];
	TesterPresentFD.set_is_brs(false);
	if (Testpresent == 1)
	{
		if(BusTypeNow==CAN)
		{
			if (0 == com.add_cyclic_message_can(&TesterPresent, PeriodTime))
			{
				//TestReport_HTML_StepDescription("功能寻址发送诊断仪在线");
				return 0;
			}
			else
				return -1;
		}
		else if (BusTypeNow == CANFD)
		{
			if (0 == com.add_cyclic_message_canfd(&TesterPresentFD, PeriodTime))
			{
				//TestReport_HTML_StepDescription("功能寻址发送诊断仪在线");
				return 0;
			}
			else
				return -1;
		}
	}
	else
	{
		if (BusTypeNow == CAN)
		{
			if (0 == com.del_cyclic_message_can(&TesterPresent))
			{

				//TestReport_HTML_StepDescription("停止发送功能寻址诊断仪在线");
				return 0;
			}
			else
				return -1;
		}
		else if (BusTypeNow == CANFD)
		{
			if (0 == com.del_cyclic_message_canfd(&TesterPresentFD))
			{
				//TestReport_HTML_StepDescription("停止发送功能寻址诊断仪在线");
				return 0;
			}
			else
				return -1;
		}
		
	}
	return 0;
}

s32 EnterProgrammingSesson(u8 AddressType = 0) {

	u8 req[2] = { 0x10,0x02 };
	SendCanSingleFrame(2, req, AddressType);
	CheckCanSingleFrame(1000);

	if (0 != CAN_TP_TesterPresent(1)) {
		return -1;
	}

	app.wait(Diag_ECUInitTime, "");

	return 0;
}

s32 EnterProgWithPrecondition()
{
	u8 req1[2] = { 0x10,0x02 };
	u8 req2[4] = { 0x31,0x01,0x02,0x06 };
	u8 req3[2] = { 0x10,0x82 };

	SendCanSingleFrame(4, req2);
	CheckCanSingleFrame(1000);
	if (TScanResp.FData[5] != 0x10 || TScanResp.FData[6] != 0x01)
	{

	}
	SendCanSingleFrame(2, req3, 1);
	app.wait(1500,"");

	SendCanSingleFrame(2, req1);
	CheckCanSingleFrame(1000);
	if (TScanResp.FData[1] != 0x50 || TScanResp.FData[2] != 0x02)
	{

	}

	app.wait(25,"");

	return 0;
}



#endif