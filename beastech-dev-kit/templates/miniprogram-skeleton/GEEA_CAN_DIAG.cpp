// GEN BLOCK BEGIN Include
#define TSMP_IMPL
#include "TSMaster.h"
#include "MPLibrary.h"
#include "Database.h"
#include "TSMasterBaseInclude.h"
#include "Configuration.h"
// GEN BLOCK END Include

// CODE BLOCK BEGIN Timer NewTimer1 MTAw
// 定时器文档 "NewTimer1"
TMPTimerMS NewTimer1;
// CODE BLOCK END Timer NewTimer1

// CODE BLOCK BEGIN Timer CCPTimer MTAw
// 定时器文档 "CCPTimer"
TMPTimerMS CCPTimer;
// CODE BLOCK END Timer CCPTimer

// CODE BLOCK BEGIN Global_Definitions 
//#include <TestCase_CAN_Communication.h>
//#include <TestCase_CANFD_Communication.h>
//#include <TestCase_LIN_Communication.h>
#include <TestCase_CAN_DiagnosticServer.h>
#include <TestCase_CAN_SWDL.h>
#include <TestCase_CAN_CheckVision.h>
#include <TestCase_CAN_E2E.h>
#include <TestCase_CAN_NM.h>
#include <TestCase_CAN_NVM.h>
#include <TestCase_CAN_SecurityUnlock.h>
#include <TestCase_CAN_CS.h>
TCANFD TempMessageFD;
TCANFD callbackMessage;
// CODE BLOCK END Global_Definitions 

// CODE BLOCK BEGIN On_CAN_Rx NewOn_CAN_Rx1 MCwtMSwtMQ__
// CAN报文接收事件 "NewOn_CAN_Rx1" 针对标识符 = 任意
void on_can_rx_NewOn_CAN_Rx1(const TCAN* ACAN) {
	try {  // 针对标识符 = 任意
		if (BusTypeNow == CAN)
		{
			if (BoardCH == ACAN->FIdxChn)
				return;
			if (ACAN->FIdentifier < 0x400)
			{
				APP_Flag = 1;
			}
			else if (ACAN->FIdentifier < 0x600 && ACAN->FIdentifier >= 0x400)
			{
				NM_Flag = 1;
			}
			else if (ACAN->FIdentifier == Diag_Response_ID)
			{
				if (ReportDescriptionFlag == 1)
				{
					strcpy(actualResponse, "收到报文：");
					for (int i = 0; i < GetDLCLength(ACAN->FDLC); i++)
					{
						strcat(actualResponse, " ");
						strcat(actualResponse, dec2hex(ACAN->FData[i]).c_str());
					}
					StepNumber++;
					TestReport_HTML_Step(StepNumber, actualResponse, Info);
					app.log_text(actualResponse, lvlInfo);
				}
			}
		}

	}
	catch (...) { log_nok("CRASH detected"); app.terminate_application(); }
}
// CODE BLOCK END On_CAN_Rx NewOn_CAN_Rx1

// CODE BLOCK BEGIN On_CAN_FD_Rx NewOn_CAN_FD_Rx1 LTEsLTEsLTE_
// CAN报文接收事件 "NewOn_CAN_FD_Rx1" 针对标识符 = 任意 (FD)
void on_canfd_rx_NewOn_CAN_FD_Rx1(const TCANFD* ACANFD) {
	try {  // 针对标识符 = 任意 (FD)
		if (checkFirstMessageFlag) {
			callbackMessage = *ACANFD;
			if (!callbackMessage.is_err && callbackMessage.FIdxChn != CH1) {
				currentMessageTime = callbackMessage.FTimeUs;
				checkFirstMessageFlag = false;
			}
		}
		if (BusTypeNow == CANFD)
		{
			if (BoardCH == ACANFD->FIdxChn)
				return;
			if (ACANFD->FIdentifier < 0x400)
			{
				APP_Flag = 1;
				//printf("APP_Flag %d", APP_Flag);
			}
			else if (ACANFD->FIdentifier < 0x600 && ACANFD->FIdentifier >= 0x400)
			{
				NM_Flag = 1;
				//printf("NM_Flag %d", NM_Flag);
			}
			else if (ACANFD->FIdentifier == Diag_Response_ID)
			{
				static u8 count_rx = 0;
				if (CheckFlag) {
					rx[count_rx].FIdentifier = ACANFD->FIdentifier;
					rx[count_rx].FDLC = ACANFD->FDLC;
					if (ACANFD->FData[1] == 0x76) {
						rx[count_rx].FData[0] = ACANFD->FData[1];
						rx[count_rx].FData[1] = ACANFD->FData[2];
					}
					else if (ACANFD->FData[1] == 0x7F && ACANFD->FData[2] == 0x36) {
						rx[count_rx].FData[0] = ACANFD->FData[1];
						rx[count_rx].FData[1] = ACANFD->FData[2];
						rx[count_rx].FData[3] = ACANFD->FData[3];
					}
					count_rx++;
					rx[0].Num = count_rx;
					if (count_rx >= 1000)
						count_rx = 0;
				}
				//for (int i = 0; i < GetDLCLength(ACANFD->FDLC); i++)
				//{
				//	if (rx[count_rx].FDLC <= 8) {
				//		rx[count_rx].FData[i] = ACANFD->FData[i];
				//	}
				//	else if (rx[count_rx].FDLC > 8 && rx[count_rx].FDLC < 15) {
				//		rx[count_rx].FData[i] = ACANFD->FData[i + 1];
				//	}
				//	
				//	//log("FData is 0x%02X", rx[count_rx].FData[i]);
				//}

				if (ReportDescriptionFlag == 1)
				{
					strcpy(actualResponse, "收到报文：");
					for (int i = 0; i < GetDLCLength(ACANFD->FDLC); i++)
					{
						strcat(actualResponse, " ");
						strcat(actualResponse, dec2hex(ACANFD->FData[i]).c_str());
					}
					StepNumber++;
					TestReport_HTML_Step(StepNumber, actualResponse, Info);
					app.log_text(actualResponse, lvlInfo);
				}


			}
		}
		if (ACANFD->FIdxChn != NetworkCluster[TpBusName_Temp])
			return;
		TempMessageFD = *ACANFD;
		if (TempMessageFD.get_is_err()) {
			//log("This is a error");
			errorFrameCount++;
			return;
		}


	}
	catch (...) { log_nok("CRASH detected"); app.terminate_application(); }
}
// CODE BLOCK END On_CAN_FD_Rx NewOn_CAN_FD_Rx1

// CODE BLOCK BEGIN On_CAN_Tx NewOn_CAN_Tx1 MCwtMSwtMQ__
// CAN报文发送成功事件 "NewOn_CAN_Tx1" 针对标识符 = 任意
void on_can_tx_NewOn_CAN_Tx1(const TCAN* ACAN) {
	try {  // 针对标识符 = 任意
		if (BusTypeNow == CAN)
		{
			if (ACAN->FIdentifier == Diag_Request_ID)
			{
				if (ReportDescriptionFlag == 1)
				{
					strcpy(requestInfo, "发送报文：");
					for (int i = 0; i < GetDLCLength(ACAN->FDLC); i++)
					{
						strcat(requestInfo, " ");
						strcat(requestInfo, dec2hex(ACAN->FData[i]).c_str());
					}
					StepNumber++;
					TestReport_HTML_Step(StepNumber, requestInfo, Info);
					app.log_text(requestInfo, lvlInfo);
				}
			}
			if (FirstNMMessageFlg && ACAN->FIdentifier == NM_WakeUp_ID && ACAN->FIdxChn == NetworkCluster[TpNMBusName_Temp]) {
				FirstNMMessageTime = ACAN->FTimeUs;
				FirstNMMessageFlg = false;
			}
		}

	}
	catch (...) { log_nok("CRASH detected"); app.terminate_application(); }
}
// CODE BLOCK END On_CAN_Tx NewOn_CAN_Tx1

// CODE BLOCK BEGIN On_CAN_FD_Tx NewOn_CAN_FD_Tx1 LTEsLTEsLTE_
// CAN报文发送成功事件 "NewOn_CAN_FD_Tx1" 针对标识符 = 任意 (FD)
void on_canfd_tx_NewOn_CAN_FD_Tx1(const TCANFD* ACANFD) {
	try {  // 针对标识符 = 任意 (FD)

		if (BusTypeNow == CANFD)
		{
			if (ACANFD->FIdentifier == Diag_Request_ID)
			{
				if (ReportDescriptionFlag == 1)
				{
					strcpy(requestInfo, "发送报文：");
					for (int i = 0; i < GetDLCLength(ACANFD->FDLC); i++)
					{
						strcat(requestInfo, " ");
						strcat(requestInfo, dec2hex(ACANFD->FData[i]).c_str());
					}
					StepNumber++;
					TestReport_HTML_Step(StepNumber, requestInfo, Info);
					app.log_text(requestInfo, lvlInfo);
				}
			}
			if (FirstNMMessageFlg && ACANFD->FIdentifier == NM_WakeUp_ID && ACANFD->FIdxChn == NetworkCluster[TpNMBusName_Temp]) {
				FirstNMMessageTime = ACANFD->FTimeUs;
				FirstNMMessageFlg = false;
			}
		}

	}
	catch (...) { log_nok("CRASH detected"); app.terminate_application(); }
}
// CODE BLOCK END On_CAN_FD_Tx NewOn_CAN_FD_Tx1

// CODE BLOCK BEGIN On_CAN_PreTx NewOn_CAN_PreTx1 MCwtMSwtMQ__
// CAN报文预发送事件 "NewOn_CAN_PreTx1" 针对标识符 = 任意
void on_can_pretx_NewOn_CAN_PreTx1(const PCAN ACAN) {
	try {  // 针对标识符 = 任意

		/*if (ACAN->FIdentifier == EngSts[0].slotID)
		{
			SimEngStsCAN();
			*ACAN = Sim_EngSts_CAN;
		}
		else if (ACAN->FIdentifier == EngSts[1].slotID)
		{
			SimIDcDcActLoSideCAN();
			*ACAN = Sim_IDcDcActLoSide_CAN;
		}
		else if (ACAN->FIdentifier == VehBattU.slotID) {
			if (gTRCfdFlag == 0) {
				SimVehBatOnCAN();
				*ACAN = Sim_VehBat_CAN;
			}
		}*/

	}
	catch (...) { log_nok("CRASH detected"); app.terminate_application(); }
}
// CODE BLOCK END On_CAN_PreTx NewOn_CAN_PreTx1

// CODE BLOCK BEGIN On_CAN_FD_PreTx NewOn_CAN_FD_PreTx1 LTEsLTEsLTE_
// CAN报文预发送事件 "NewOn_CAN_FD_PreTx1" 针对标识符 = 任意 (FD)
void on_canfd_pretx_NewOn_CAN_FD_PreTx1(const PCANFD ACANFD) {
	try {  // 针对标识符 = 任意 (FD)

		//if (ACANFD->FIdentifier == VehSpdLgt.slotID)
		//{
		//	//SimVehMtnSpdCANFD();
		//	//*ACANFD = Sim_VehSpd_CANFD;
		//	SimVehMtnSpd();
		//	*ACANFD = Sim_VehSpd;
		//}
		//else if (ACANFD->FIdentifier == VehGlbSafe.slotID)
		//{
		//	SimVehMtnGlbSafeCANFD();
		//	*ACANFD = Sim_UsgMode_CANFD;
		//}
		//else if (ACANFD->FIdentifier == VehBattU.slotID)
		//{
		//	if (gTRCfdFlag == 1) {
		//		SimVehBatOnCANFD();
		//		*ACANFD = Sim_VehBat_CANFD;
		//	}
		//}

	}
	catch (...) { log_nok("CRASH detected"); app.terminate_application(); }
}
// CODE BLOCK END On_CAN_FD_PreTx NewOn_CAN_FD_PreTx1

// CODE BLOCK BEGIN On_LIN_Rx NewOn_LIN_Rx1 LTE_
// LIN报文接收事件 "NewOn_LIN_Rx1" 针对标识符 = 任意
void on_lin_rx_NewOn_LIN_Rx1(const TLIN* ALIN) {
	try {  // 针对标识符 = 任意

		if (CheckLINMessageFlg) {
			if (FirstLINMessageTime[ALIN->FIdxChn] == 0) {
				FirstLINMessageTime[ALIN->FIdxChn] = ALIN->FTimeUs;
			}
		}
		TLIN frame = *ALIN;
		if (ReportDescriptionFlagLin == 1 && IfListenLIN)
		{
			//printf("Diag_RX_Lin_V");
			if (frame.FIdentifier != 0x3F)  Diag_RX_Lin_V.push_back(frame);

		}


	}
	catch (...) { log_nok("CRASH detected"); app.terminate_application(); }
}
// CODE BLOCK END On_LIN_Rx NewOn_LIN_Rx1

// CODE BLOCK BEGIN On_LIN_Tx NewOn_LIN_Tx1 LTE_
// LIN报文发送成功事件 "NewOn_LIN_Tx1" 针对标识符 = 任意
void on_lin_tx_NewOn_LIN_Tx1(const TLIN* ALIN) {
	try {  // 针对标识符 = 任意

		TLIN frame = *ALIN;
		if (ReportDescriptionFlagLin == 1 && IfListenLIN)
		{
			//printf("Diag_RX_Lin_V");
			if (frame.FIdentifier != 0x3F) Diag_TX_Lin_V.push_back(frame);

		}


	}
	catch (...) { log_nok("CRASH detected"); app.terminate_application(); }
}
// CODE BLOCK END On_LIN_Tx NewOn_LIN_Tx1

// CODE BLOCK BEGIN On_LIN_PreTx NewOn_LIN_PreTx1 LTE_
// LIN报文预发送事件 "NewOn_LIN_PreTx1" 针对标识符 = 任意
void on_lin_pretx_NewOn_LIN_PreTx1(const PLIN ALIN) {
	try {  // 针对标识符 = 任意


	}
	catch (...) { log_nok("CRASH detected"); app.terminate_application(); }
}
// CODE BLOCK END On_LIN_PreTx NewOn_LIN_PreTx1

// CODE BLOCK BEGIN On_Ethernet_Rx NewOn_Ethernet_Rx1 LTE_
// Ethernet报文接收事件 "NewOn_Ethernet_Rx1" 针对报文 = 任意
void on_ethernet_rx_NewOn_Ethernet_Rx1(const TEthernetHeader* AEthernetHeader) {
	try {  // 针对报文 = 任意


	}
	catch (...) { log_nok("CRASH detected"); app.terminate_application(); }
}
// CODE BLOCK END On_Ethernet_Rx NewOn_Ethernet_Rx1

// CODE BLOCK BEGIN On_Ethernet_Tx NewOn_Ethernet_Tx1 LTE_
// Ethernet报文发送成功事件 "NewOn_Ethernet_Tx1" 针对报文 = 任意
void on_ethernet_tx_NewOn_Ethernet_Tx1(const TEthernetHeader* AEthernetHeader) {
	try {  // 针对报文 = 任意


	}
	catch (...) { log_nok("CRASH detected"); app.terminate_application(); }
}
// CODE BLOCK END On_Ethernet_Tx NewOn_Ethernet_Tx1

// CODE BLOCK BEGIN On_Ethernet_PreTx NewOn_Ethernet_PreTx1 LTE_
// Ethernet报文预发送事件 "NewOn_Ethernet_PreTx1" 针对报文 = 任意
void on_ethernet_pretx_NewOn_Ethernet_PreTx1(const PEthernetHeader AEthernetHeader) {
	try {  // 针对报文 = 任意


	}
	catch (...) { log_nok("CRASH detected"); app.terminate_application(); }
}
// CODE BLOCK END On_Ethernet_PreTx NewOn_Ethernet_PreTx1

// CODE BLOCK BEGIN On_TCP_Listen NewOn_TCP_Listen1 MCwxOTIuMTY4LjEuMTExOjEyMzQ_
// CH1: 192.168.1.111:1234
void on_tcp_listen_NewOn_TCP_Listen1(const native_int ASocket, native_int AClientSocket, const native_int AResult) {
	try {  // CH1: 192.168.1.111:1234


	}
	catch (...) { log_nok("CRASH detected"); app.terminate_application(); }
}
// CODE BLOCK END On_TCP_Listen NewOn_TCP_Listen1

// CODE BLOCK BEGIN On_TCP_Connect NewOn_TCP_Connect1 MCwxOTIuMTY4LjEuMTExOjEyMzQ_
// CH1: 192.168.1.111:1234
void on_tcp_connect_NewOn_TCP_Connect1(const native_int ASocket, const native_int AResult) {
	try {  // CH1: 192.168.1.111:1234


	}
	catch (...) { log_nok("CRASH detected"); app.terminate_application(); }
}
// CODE BLOCK END On_TCP_Connect NewOn_TCP_Connect1

// CODE BLOCK BEGIN On_TCP_Close NewOn_TCP_Close1 MCwxOTIuMTY4LjEuMTExOjEyMzQ_
// CH1: 192.168.1.111:1234
void on_tcp_close_NewOn_TCP_Close1(const native_int ASocket, const native_int AResult) {
	try {  // CH1: 192.168.1.111:1234

	}
	catch (...) { log_nok("CRASH detected"); app.terminate_application(); }
}
// CODE BLOCK END On_TCP_Close NewOn_TCP_Close1

// CODE BLOCK BEGIN On_TCP_Receive NewOn_TCP_Receive1 MCwxOTIuMTY4LjEuMTExOjEyMzQ_
// CH1: 192.168.1.111:1234
void on_tcp_receive_NewOn_TCP_Receive1(const native_int ASocket, const native_int AResult, const u32 AAddr, const u32 APort, const pu8 AData, const native_int ASize) {
	try {  // CH1: 192.168.1.111:1234


	}
	catch (...) { log_nok("CRASH detected"); app.terminate_application(); }
}
// CODE BLOCK END On_TCP_Receive NewOn_TCP_Receive1

// CODE BLOCK BEGIN On_TCP_Transmit NewOn_TCP_Transmit1 MCwxOTIuMTY4LjEuMTExOjEyMzQ_
// CH1: 192.168.1.111:1234
void on_tcp_transmit_NewOn_TCP_Transmit1(const native_int ASocket, const native_int AResult, const pu8 AData, const native_int ASize) {
	try {  // CH1: 192.168.1.111:1234


	}
	catch (...) { log_nok("CRASH detected"); app.terminate_application(); }
}
// CODE BLOCK END On_TCP_Transmit NewOn_TCP_Transmit1

// CODE BLOCK BEGIN On_UDP_Receive_From NewOn_UDP_Receive_From1 MCwxOTIuMTY4LjEuMTExOjEyMzQ_
// CH1: 192.168.1.111:1234
void on_udp_receive_from_NewOn_UDP_Receive_From1(const native_int ASocket, const native_int AResult, const u32 AAddr, const u32 APort, const pu8 AData, const native_int ASize) {
	try {  // CH1: 192.168.1.111:1234


	}
	catch (...) { log_nok("CRASH detected"); app.terminate_application(); }
}
// CODE BLOCK END On_UDP_Receive_From NewOn_UDP_Receive_From1

// CODE BLOCK BEGIN On_UDP_Transmit_To NewOn_UDP_Transmit_To1 MCwxOTIuMTY4LjEuMTExOjEyMzQ_
// CH1: 192.168.1.111:1234
void on_udp_transmit_to_NewOn_UDP_Transmit_To1(const native_int ASocket, const native_int AResult, const pu8 AData, const native_int ASize) {
	try {  // CH1: 192.168.1.111:1234

	}
	catch (...) { log_nok("CRASH detected"); app.terminate_application(); }
}
// CODE BLOCK END On_UDP_Transmit_To NewOn_UDP_Transmit_To1

// CODE BLOCK BEGIN On_Var_Change On_TestSystem_CsvStatus VGVzdFN5c3RlbS5Dc3ZTdGF0dXMsLTE_
// 变量变化事件 "On_TestSystem_CsvStatus" 针对变量 "TestSystem.CsvStatus" [On Written]
void on_var_change_On_TestSystem_CsvStatus(void) {
	try { // 变量 = TestSystem.CsvStatus


	}
	catch (...) { log_nok("CRASH detected"); app.terminate_application(); }
}
// CODE BLOCK END On_Var_Change On_TestSystem_CsvStatus

// CODE BLOCK BEGIN On_Timer NewOn_Timer1 TmV3VGltZXIx
// 定时器触发事件 "NewOn_Timer1" for Timer NewTimer1
void on_timer_NewOn_Timer1(void) {
	try { // 定时器 = NewTimer1
		gCarTiGlb_Sim += 0.1;
		UpdateCarTiGlbRBS(&g_CarTiGlb_path, gCarTiGlb_Sim);

	}
	catch (...) { log_nok("CRASH detected"); app.terminate_application(); }
}
// CODE BLOCK END On_Timer NewOn_Timer1

// CODE BLOCK BEGIN On_Timer On_CCPTimer Q0NQVGltZXI_
// 定时器触发事件 "On_CCPTimer" for Timer CCPTimer
void on_timer_On_CCPTimer(void) {
	try { // 定时器 = CCPTimer
		signCarConfigData();
	}
	catch (...) { log_nok("CRASH detected"); app.terminate_application(); }
}
// CODE BLOCK END On_Timer On_CCPTimer

// CODE BLOCK BEGIN On_Start NewOn_Start1
// 启动事件 "NewOn_Start1"
void on_start_NewOn_Start1(void) {
	try { // 程序启动事件


	}
	catch (...) { log_nok("CRASH detected"); app.terminate_application(); }
}
// CODE BLOCK END On_Start NewOn_Start1

// CODE BLOCK BEGIN On_Stop NewOn_Stop1
// 停止事件 "NewOn_Stop1"
void on_stop_NewOn_Stop1(void) {
	try { // 程序停止事件


	}
	catch (...) { log_nok("CRASH detected"); app.terminate_application(); }
}
// CODE BLOCK END On_Stop NewOn_Stop1

// CODE BLOCK BEGIN Step_Function  NQ__
// 主step函数，执行周期 5 ms
void step(void) {
	try { // 周期 = 5 ms


	}
	catch (...) { log_nok("CRASH detected"); app.terminate_application(); }
}
// CODE BLOCK END Step_Function 

