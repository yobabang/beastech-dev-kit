#ifndef TestCase_CAN_Communication
#define TestCase_CAN_Communication

#include "test_case_diag_api.h"

////////////////////////////CAN DIAGNOSTIC SERVER APP///////////////////////////////////
s32 test_case_1021940() {

	strcpy(TestCaseName, "TC_1021940_Block size handling physical addressing - Default session");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - APP");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN();

	int i = 0, j = 0;

	u8 reqDataArray1[2] = { 0xF1,0X86 };
	TestReport_HTML_StepDescription("检查当前会话");
	CheckCurrentSessionTP(0x01);

	u8 reqDataArray2[3] = { 0x22,0xED,0X20 };

	TestReport_HTML_StepDescription("请求多帧响应");
	SendCanSingleFrame(3, reqDataArray2);

	if (0 == CheckCanFirstFrame(1000))
	{
		TestReport_HTML_StepDescription("发送流控帧");
		SendCanFlowControlFrame(0, 0, 0);

		CheckCanConsecutiveFrame(1000);
	}

	TestWait(100);

	TestReport_HTML_StepDescription("发送BS值不同的流控帧 检查ECU按照流控帧参数响应");

	for (int BS = 1; BS < 6; BS++)
	{
		// 发送单帧CAN消息
		SendCanSingleFrame(3, reqDataArray2);

		// 检查是否收到第一帧CAN消息
		if (CheckCanFirstFrame(1000) == 0)
		{
			// 根据第一帧消息的长度计算连续帧的数量
			CFcnt = (TScanfdResp.FDLC == 8) ? (FFLength - 6 + 6) / 7 : (FFLength - 6 + 62) / 63;
			printf("CFcnt: %d\n", CFcnt);

			// 根据BS值计算流控帧的数量
			FCcnt = (CFcnt + BS - 1) / BS;
			printf("FCcnt: %d\n", FCcnt);

			// 发送流控帧并等待响应
			for (int i = 0; i < FCcnt; i++)
			{
				SendCanFlowControlFrame(0, BS, 0);
				printf("Sent Flow Control Frame %d\n", i);

				// 处理最后一组消息
				remainingFrames = CFcnt - i * BS;
				printf("Remaining Frames: %d\n", remainingFrames);
				int transferedframe = 0;

				for (int j = 0; j < remainingFrames; j++)
				{
					// 缩短等待时间，确保及时检测到连续帧
					if (WaitMessage(1000) != 0)
					{
						issueflag = 1;
						printf("WaitMessage timeout or error at frame %d\n", j);
						break; // 提前退出循环以避免不必要的等待
					}
					else
					{
						printf("Received Continuous Frame %d\n", j);
						transferedframe++;
					
						if (transferedframe >= BS)
						{
							remainingFrames -= BS;
							break;
						}
					}
				}

				if (issueflag != 0) break; // 如果发现问题，提前退出外层循环
			}
		}

	}

	if (issueflag != 0)
	{
		strcpy(issuereason, "ECU未按照流控帧BS值返回连续帧！");
		strcpy(requestInfo, "发送BS值不同的流控帧");
		strcpy(expectedResponse, "验证DUT按照BS返回连续帧");
		strcpy(actualResponse, "DUT返回的连续帧数量与流控帧BS不符");
		TestReport_HTML_StepDescription("ECU未按照流控帧BS值返回连续帧！", Fail);
	}

	TestReport_HTML_StepDescription("检查当前会话");
	CheckCurrentSessionTP(0x01);

	TestPostCondition_TP_CAN();

	if (resultFlag != 0)
		return 1;
	else
		return 0;
}
//分开的必须是7FF，流控帧算不？
s32 test_case_1021941() {
	strcpy(TestCaseName, "TC_1021941_Block size handling functional addressing - Default session");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - APP");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN();

	int i = 0,j = 0;

	u8 reqDataArray1[2] = { 0xF1,0X86 };
	TestReport_HTML_StepDescription("检查当前会话");
	CheckCurrentSessionTP( 0x01,1);

	u8 reqDataArray2[3] = { 0x22,0xED,0X20 };

	TestReport_HTML_StepDescription("请求多帧响应");
	SendCanSingleFrame(3, reqDataArray2,1);

	CheckCanFirstFrame(1000);

	TestReport_HTML_StepDescription("发送流控帧");
	SendCanFlowControlFrame(0, 0, 0);

	CheckCanConsecutiveFrame(1000);

	TestWait(100);

	TestReport_HTML_StepDescription("发送BS值不同的流控帧 检查ECU按照流控帧参数响应");
	for (int BS = 1; BS < 6; BS++)
	{
		// 发送单帧CAN消息
		SendCanSingleFrame(3, reqDataArray2);

		// 检查是否收到第一帧CAN消息
		if (CheckCanFirstFrame(1000) == 0)
		{
			// 根据第一帧消息的长度计算连续帧的数量
			CFcnt = (TScanfdResp.FDLC == 8) ? (FFLength - 6 + 6) / 7 : (FFLength - 6 + 62) / 63;
			printf("CFcnt: %d\n", CFcnt);

			// 根据BS值计算流控帧的数量
			FCcnt = (CFcnt + BS - 1) / BS;
			printf("FCcnt: %d\n", FCcnt);

			// 发送流控帧并等待响应
			for (int i = 0; i < FCcnt; i++)
			{
				SendCanFlowControlFrame(0, BS, 0);
				printf("Sent Flow Control Frame %d\n", i);

				// 处理最后一组消息
				remainingFrames = CFcnt - i * BS;
				printf("Remaining Frames: %d\n", remainingFrames);
				int transferedframe = 0;

				for (int j = 0; j < remainingFrames; j++)
				{
					// 缩短等待时间，确保及时检测到连续帧
					if (WaitMessage(1000) != 0)
					{
						issueflag = 1;
						printf("WaitMessage timeout or error at frame %d\n", j);
						break; // 提前退出循环以避免不必要的等待
					}
					else
					{
						printf("Received Continuous Frame %d\n", j);
						transferedframe++;

						if (transferedframe >= BS)
						{
							remainingFrames -= BS;
							break;
						}
					}
				}

				if (issueflag != 0) break; // 如果发现问题，提前退出外层循环
			}
		}

	}

	if (issueflag != 0)
	{
		strcpy(issuereason, "ECU未按照流控帧BS值返回连续帧！");
		strcpy(requestInfo, "发送BS值不同的流控帧");
		strcpy(expectedResponse, "验证DUT按照BS返回连续帧");
		strcpy(actualResponse, "DUT返回的连续帧数量与流控帧BS不符");
		TestReport_HTML_StepDescription("ECU未按照流控帧BS值返回连续帧！", Fail);
	}

	TestReport_HTML_StepDescription("检查当前会话");
	CheckCurrentSessionTP( 0x01,1);

	TestPostCondition_TP_CAN();

	if (resultFlag != 0)
		return 1;
	else
		return 0;
}

s32 test_case_1021942() {
	strcpy(TestCaseName, "TC_1021942_Consecutive frame timeout physical addressing - Default session");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - APP");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN();
    
	u8 req1[2] = { 0xF1,0X86 };
	u8 req2[6] = { 0x10,0x01,0x02,0x03,0x04,0x05 };
	u8 req3[8] = { 0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c };
	TestReport_HTML_StepDescription("检查当前会话");
	CheckCurrentSessionTP( 0x01);

	TestReport_HTML_StepDescription("发送首帧请求，等待N_Cr*0.95后，发送连续帧");
	SendCanFirstFrame(0x0D, req2);
	CheckCanFlowControlMessage(1000);

	TestWait(Diag_TP_N_Cr * 0.95);

	SendCanConsecutiveFrame(1, req3);

	CheckCanSingleFrame(1000);
	if (TScanResp.FData[1] != 0x7f || TScanResp.FData[3] != 0x13)
	{
		TestReport_HTML_StepDescription("ECU回复了非预期响应", Fail);
	}

	TestReport_HTML_StepDescription("等待N_Cr+25ms后，发送连续帧，检查无响应");
	//SendCanFirstFrame(0x0D, req2);
	//CheckCanFlowControlMessage(1000);
	TestWait(Diag_TP_N_Cr +25);
	SendCanConsecutiveFrame(1, req3);
	//app.set_system_var_int32("Diagnostic0.诊断会话控制1_Start", 0);
	
	if (0 != CheckNonResponse(1000))
		issueflag = 1;
	else
		issueflag = 0;

	if (issueflag == 1)
	{
		strcpy(issuereason, "ECU在N_Cr超时后仍然发送响应！");
		strcpy(requestInfo, "Tester发送首帧，");
		strcpy(expectedResponse, "验证DUT按照BS返回连续帧");
		strcpy(actualResponse, "DUT返回的连续帧数量与流控帧BS不符");
		TestReport_HTML_StepDescription("ECU在N_Cr超时后仍然发送响应！", Fail);
	}

	TestReport_HTML_StepDescription("检查当前会话");
	CheckCurrentSessionTP( 0x01);

	TestPostCondition_TP_CAN();

	//return resultFlag;
	if (resultFlag != 0)
		return 1;
	else
		return 0;

}

s32 test_case_1021943() {
	strcpy(TestCaseName, "TC_1021943_Flow Control handling with wait frames test physical addressing - Default session");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - APP");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN();

	TestReport_HTML_StepDescription("检查当前会话");
	CheckCurrentSessionTP( 0x01);
    //----------------------------------------第1部分-------------------------------------------------
    u8 req_date[] = { 0x22,0xED ,0x20 };
    SendCanSingleFrame(3, req_date);
    CheckCanFirstFrame(1000);
    app.wait(Diag_TP_N_Br * 0.95, "");
    SendCanFlowControlFrame(1, 0, 0);
	//CheckNonResponse(Diag_TP_N_Bs);
	//if (WaitMessage(25) == 0)
	//{
	//	strcpy(issuereason, "ECU在收到等待流控帧后仍然发送响应！");
	//	strcpy(requestInfo, "Tester请求首帧响应，收到首帧后发送等待流控帧");
	//	strcpy(expectedResponse, "DUT不发送连续帧");
	//	strcpy(actualResponse, "DUT仍然发送连续帧");
	//	TestReport_HTML_StepDescription("ECU发送了连续帧！", Fail);
	//}
    SendCanFlowControlFrame(0, 0, 0);
	if (0 == CheckCanConsecutiveFrame(1000))
	{
		TestReport_HTML_StepDescription("ECU回复了连续帧", Pass);
	}
	else
	{
		strcpy(issuereason, "ECU在收到等待流控帧后收到发送流控帧，仍然不发送响应！");
		strcpy(requestInfo, "Tester请求首帧响应，收到首帧后发送等待流控帧");
		strcpy(expectedResponse, "DUT不发送连续帧，收到发送流控帧后，DUT发送连续帧");
		strcpy(actualResponse, "DUT不发送连续帧，收到发送流控帧后，DUT仍不发送连续帧");
		TestReport_HTML_StepDescription("ECU未回复连续帧", Fail);
	}

	TestWait(stepseptime);
	//6
    //----------------------------------------第2部分-------------------------------------------------
    SendCanSingleFrame(3, req_date, Physical);
    CheckCanFirstFrame(1000);
    app.wait(Diag_TP_N_Br * 1.05, "");
    SendCanFlowControlFrame(1, 0, 0);

	//if (WaitMessage(Diag_TP_N_Bs * 0.95) == 0)
	//{
	//	TestReport_HTML_StepDescription("ECU在N_Bs时间内回复了连续帧", Fail);
	//}
    
	SendCanFlowControlFrame(0, 0, 0);

	if (0 != CheckCanConsecutiveFrame(1000))
	{
		strcpy(issuereason, "ECU在收到N_Br超时的等待流控帧后，收到发送流控帧，仍然不发送响应！");
		strcpy(requestInfo, "Tester请求首帧响应，收到首帧后在N_Br超时后发送等待流控帧，然后发送继续发送流控帧");
		strcpy(expectedResponse, "DUT不发送连续帧，收到发送流控帧后，DUT发送连续帧");
		strcpy(actualResponse, "DUT不发送连续帧，收到发送流控帧后，DUT仍不发送连续帧");
		TestReport_HTML_StepDescription("ECU未回复连续帧", Fail);
	}
	TestWait(stepseptime);
    //11
	//----------------------------------------第3部分-------------------------------------------------
    SendCanSingleFrame(3, req_date, Physical);
    CheckCanFirstFrame(1000);

	TestReport_HTML_StepDescription("等待N_BS+25ms");
    app.wait(Diag_TP_N_Bs +25, "");
    SendCanFlowControlFrame(1, 0, 0);//step13,这里是发完31后，1s内没有收到响应，再等待50%N_Bs
	//if (WaitMessage(Diag_TP_N_Bs * 0.5) == 0)
	//{
	//	TestReport_HTML_StepDescription("ECU在N_Bs时间内回复了连续帧", Fail);
	//}
	TestWait(Diag_TP_N_Bs*0.5);
    SendCanFlowControlFrame(0, 0, 0);
	CheckNonResponse(Diag_TP_N_Bs);

	//if (WaitMessage(1000) == 0)
	//{
	//	TestReport_HTML_StepDescription("ECU在N_Bs超时后回复了连续帧", Fail);
	//}

	CheckCurrentSessionTP( 0x01);
	TestPostCondition_TP_CAN();

	if (resultFlag != 0)
		return 1;
	else
		return 0;
}

s32 test_case_1021944() {
	strcpy(TestCaseName, "TC_1021944_Flow Control Timeout physical addressing - Default session");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - APP");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN(); 

	u8 req1[2] = { 0xF1,0X86 };
	CheckCurrentSessionTP( 0x01);
    u8 req_date[] = { 0x22,0xED,0x20 };
	TestReport_HTML_StepDescription("请求多帧响应，等待N_Br*0.95后，发送流控帧");
    SendCanSingleFrame(3, req_date, Physical);
    CheckCanFirstFrame(1000);
    app.wait(Diag_TP_N_Br * 0.95, "");
    SendCanFlowControlFrame(0, 0, 0);
    CheckCanConsecutiveFrame( Diag_TP_N_Br);//这里应该是1000把

	TestReport_HTML_StepDescription("请求多帧响应，等待N_Br超时");
    SendCanSingleFrame(3, req_date, Physical);
    CheckCanFirstFrame(1000);
    app.wait(Diag_TP_N_Br + 25, "");
    SendCanFlowControlFrame(0, 0, 0);
	if (WaitMessage(1000) == 0)
	{
		strcpy(issuereason, "ECU在N_Br超时后仍然发送连续帧！");
		strcpy(requestInfo, "请求多帧响应，等待N_Br超时后发送流控帧");
		strcpy(expectedResponse, "DUT不发送连续帧");
		strcpy(actualResponse, "DUT在N_Br超时后，仍然发送连续帧");
		TestReport_HTML_StepDescription("在N_Br超时后ECU仍然发送连续帧", Fail);
	}
	else
		TestReport_HTML_StepDescription("ECU未响应", Pass);

	CheckCurrentSessionTP( 0x01);
	TestPostCondition_TP_CAN();

	if (resultFlag != 0)
		return 1;
	else
		return 0;
}

s32 test_case_1021945()
{
	strcpy(TestCaseName, "TC_1021945_Flow Control handling with max wait frames test physical addressing - Default session");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - APP");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN();
    int N_WFTmax = 255;
	u8 req1[2] = { 0xF1,0X86 };
	CheckCurrentSessionTP( 0x01,1);
    u8 req_date[3] = { 0x22,0xED ,0x20 };
    SendCanSingleFrame(3, req_date, 0);
    CheckCanFirstFrame(1000);

	TestReport_HTML_StepDescription("发送等待流控帧 N_WFTMax=255");
    for (int n = 0; n < N_WFTmax; n++)
    {
        SendCanFlowControlFrame(1, 0, 0);
    }
    SendCanFlowControlFrame(0, 0, 0);

	if (CheckCanConsecutiveFrame(1000) != 0)
	{
		strcpy(issuereason, "发送最大数量的等待流控帧后发送继续发送流控帧，DUT不发送连续帧！");
		strcpy(requestInfo, "Tester发送首帧，发送等待流控帧，N_WFTMax=255，然后发送继续发送流控帧");
		strcpy(expectedResponse, "DUT收到等待流控帧时不发送响应，收到继续发送流控帧时发送剩下的连续帧");
		strcpy(actualResponse, "DUT未发送连续帧");
		TestReport_HTML_StepDescription("发送最大数量的等待流控帧后发送继续发送流控帧，DUT不发送连续帧！", Fail);
	}
	else
		TestReport_HTML_StepDescription("发送最大数量的等待流控帧后发送继续发送流控帧，DUT发送连续帧！", Pass);

	CheckCurrentSessionTP( 0x01);
	
	TestPostCondition_TP_CAN();

	if (resultFlag != 0)
		return 1;
	else
		return 0;
}

s32 test_case_1021946()
{
	strcpy(TestCaseName, "TC_1021946_Handling of Unexpected Frames During Segmented Receive Test physical addressing - Default session");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - APP");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN();
	u8 req1[2] = { 0xF1,0X86 };
	CheckCurrentSessionTP( 0x01);
   
	u8 req2[6] = { 0x10,0x01,0x02,0x03,0x04,0x05 };

	TestReport_HTML_StepDescription("CASE1 多帧接收被单帧打断");
	SendCanFirstFrame(0x0D, req2);
	CheckCanFlowControlMessage(1000);

	CheckCurrentSessionTP( 0x01);

	u8 req3[7] = { 0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c };

	SendCanConsecutiveFrame(1, req3);

	if (0 != CheckNoResp(25))
	{
		strcpy(issuereason, "多帧接收过程中收到单帧仍然对多帧发送应答！");
		strcpy(requestInfo, "Tester发送首帧，ECU回复流控帧后Tester发送单帧后发送连续帧");
		strcpy(expectedResponse, "DUT回复单帧请求并忽略后续连续帧");
		strcpy(actualResponse, "DUT仍然发送对多帧的响应");
		TestReport_HTML_StepDescription("多帧接收过程中收到单帧仍然对多帧发送应答！", Fail);
	}
	else
		TestReport_HTML_StepDescription("无响应！",Pass);

	/////////////
	TestReport_HTML_StepDescription("CASE2 多帧接受被首帧打断");

	u8 req4[8] = { 0x13,0x00,0x00,0x00,0x00,0x00,0x00 };
	SendCanFirstFrame(0x13, req4);
	CheckCanFlowControlMessage(1000);
	u8 req5[8] = { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };
	SendCanConsecutiveFrame(1, req5);
	CheckNoResp(1000);

	SendCanFirstFrame(0x0D, req2);
	CheckCanFlowControlMessage(1000);

	SendCanConsecutiveFrame(1, req3);
	CheckCanSingleFrame(1000);
	if (TScanResp.FData[1] != 0x7f || TScanResp.FData[3] != 0x13)
	{
		TestReport_HTML_StepDescription("收到了非预期响应", Fail);
	}
	/////////////
	TestReport_HTML_StepDescription("CASE3 多帧接收忽略非预期流控帧");
	SendCanFirstFrame(0x0D, req2);
	CheckCanFlowControlMessage(1000);

	SendCanFlowControlFrame(0, 0, 0);
	CheckNoResp(25);

	SendCanConsecutiveFrame(1, req3);
	CheckCanSingleFrame(1000);
	if (TScanResp.FData[1] != 0x7f || TScanResp.FData[3] != 0x13)
	{
		TestReport_HTML_StepDescription("收到了非预期响应", Fail);
	}
	///////////////
	TestReport_HTML_StepDescription("CASE4 多帧接受忽略非预期帧");
	SendCanFirstFrame(0x0D, req2);
	CheckCanFlowControlMessage(1000);

	TCAN canReq = { Diag_ch,1,8,0,Diag_Request_ID,0,{0x40,0x0,0x0,0x0,0x0,0x0,0x0,0x0} };
	com.transmit_can_async(&TScanReq);

	CheckNoResp(25);

	SendCanConsecutiveFrame(1, req3);
	CheckCanSingleFrame(1000);
	if (TScanResp.FData[1] != 0x7f || TScanResp.FData[3] != 0x13)
	{
		TestReport_HTML_StepDescription("收到了非预期响应", Fail);
	}

	CheckCurrentSessionTP( 0x01);

	TestPostCondition_TP_CAN();

	if (resultFlag != 0)
		return 1;
	else
		return 0;
}
//这个case的报告的报文有些错乱
s32 test_case_1021947()
{
	strcpy(TestCaseName, "TC_1021947_Half-duplex - Handling of Unexpected Frames During Segmented Transmit Test physical addressing");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - APP");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN();

	u8 req1[2] = { 0xF1,0X86 };
	CheckCurrentSessionTP( 0x01);

	TestReport_HTML_StepDescription("CASE1 多帧传输忽略非预期单帧");
    u8 req_date[3] = { 0x22,0xED ,0x20 };
    SendCanSingleFrame(3, req_date, 0);
    CheckCanFirstFrame(1000);

	SendCanSingleFrame(2, req1);

    SendCanFlowControlFrame(0, 0, 0);
    app.wait(Diag_TP_N_Ar + 10, "");

	TestReport_HTML_StepDescription("CASE2 多帧传输忽略非预期首帧");
    SendCanSingleFrame(3, req_date, 0);
    CheckCanFirstFrame(1000);
    TCAN f3 = { Diag_ch,0x1,8,0,Diag_Request_ID,0,{0x10, 0x0D, 0x10, 0x01, 0x02, 0x03, 0x04, 0x05} };
    com.transmit_can_async(&f3);
    SendCanFlowControlFrame(0, 0, 0);
    app.wait(Diag_TP_N_Bs + 10, "");//9

	TestReport_HTML_StepDescription("CASE3 多帧传输忽略非预期连续帧");
    SendCanSingleFrame(3, req_date, 0);//10
    CheckCanFirstFrame(1000);
    TCAN f5 = { Diag_ch,0x1,8,0,Diag_Request_ID,0,{0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} };
    com.transmit_can_async(&f5);
    SendCanFlowControlFrame(0, 0, 0);//12

	TestReport_HTML_StepDescription("CASE4 多帧传输忽略非预期流控帧");
    SendCanSingleFrame(3, req_date, 0);
    CheckCanFirstFrame(1000);
    SendCanFlowControlFrame(0, 0, 0);
    SendCanFlowControlFrame(0, 1, 0);

	TestReport_HTML_StepDescription("CASE5 多帧传输忽略非预期帧");
    SendCanSingleFrame(3, req_date, 0);
    CheckCanFirstFrame(1000);
    TCAN f9 = { Diag_ch,0x1,8,0,Diag_Request_ID,0,{0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} };
    com.transmit_can_async(&f9);
    SendCanFlowControlFrame(0, 0, 0);

	CheckCurrentSessionTP( 0x01);
	
	TestPostCondition_TP_CAN();

	if (resultFlag != 0)
		return 1;
	else
		return 0;
}
//未分段消息的请求必须按功能寻址，7FF不支持多帧,30FC需要用7FF吗？
s32 test_case_1021948()
{
	strcpy(TestCaseName, "TC_1021948_Half-duplex - Handling of Unexpected Frames During Segmented Transmit Test functional addressing");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - APP");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN();

	u8 req1[2] = { 0xF1,0X86 };
	CheckCurrentSessionTP( 0x01,1);

	TestReport_HTML_StepDescription("CASE1 多帧传输忽略非预期单帧");
    u8 req_date[3] = { 0x22,0xED ,0x20 };
    SendCanSingleFrame(3, req_date, 1);
    CheckCanFirstFrame(1000);
	CheckCurrentSessionTP( 0x01);
    SendCanFlowControlFrame(0, 0, 0, 1);
    app.wait(Diag_TP_N_Ar + 10, "");


	TestReport_HTML_StepDescription("CASE2 多帧传输忽略非预期首帧");
    SendCanSingleFrame(3, req_date, 1);
    CheckCanFirstFrame(1000);
    TCAN f3 = { Diag_ch,0x1,8,0,Diag_Request_ID,0,{0x10, 0x0D, 0x10, 0x01, 0x02, 0x03, 0x04, 0x05} };
    com.transmit_can_async(&f3);
    SendCanFlowControlFrame(0, 0, 0, 1);
    app.wait(Diag_TP_N_Bs + 10, "");


	TestReport_HTML_StepDescription("CASE3 多帧传输忽略非预期连续帧");
    SendCanSingleFrame(3, req_date, 1);
    CheckCanFirstFrame(1000);
    TCAN f5 = { Diag_ch,0x1,8,0,Diag_Request_ID,0,{0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} };
    com.transmit_can_async(&f5);
    SendCanFlowControlFrame(0, 0, 0, 1);


	TestReport_HTML_StepDescription("CASE4 多帧传输忽略非预期流控帧");
    SendCanSingleFrame(3, req_date, 1);
    CheckCanFirstFrame(1000);
    SendCanFlowControlFrame(0, 0, 0, 1);
    SendCanFlowControlFrame(0, 1, 0, 1);


	TestReport_HTML_StepDescription("CASE5 多帧传输忽略非预期帧");
    SendCanSingleFrame(3, req_date, 1);
    CheckCanFirstFrame(1000);
    TCAN f9 = { Diag_ch,0x1,8,0,Diag_Request_ID,0,{0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} };
    com.transmit_can_async(&f9);
    SendCanFlowControlFrame(0, 0, 0, 1);
	CheckCurrentSessionTP( 0x01,1);
	TestPostCondition_TP_CAN();

	if (resultFlag != 0)
		return 1;
	else
		return 0;
}

s32 test_case_1021949()
{
	strcpy(TestCaseName, "TC_1021949_Valid Separation Time Parameter Test physical addressing - Default session");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - APP");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN();

	u8 SFdata1[2] = {0xF1,0x86 };
	u8 SFdata2[3] = { 0x22,0xED,0x20 };
	s64 t1=0, t2=0, t3=0;
	u16 STminTimeArr[7] = {0x19,0x32,0x64,0x7F,0xF1,0xF5,0xF9};

	//检查当前会话
	CheckCurrentSessionTP(0x01);

	for (int i = 0; i < 7; i++)
	{
		u32 STminTime = STminTimeArr[i];

		//请求多帧响应
		char sinfo[200] = "";
		sprintf(sinfo, "CASE%d 当前STMin=0x%x", i, STminTime);
		TestReport_HTML_StepDescription(sinfo);
		SendCanSingleFrame(3, SFdata2);
		TestReport_HTML_StepDescription("Step: Request Multi Response !");

		if (0 == CheckCanFirstFrame( Diag_TP_N_As))
		{
			TestReport_HTML_StepDescription("Step: Send Flow Control Frame !");
			SendCanFlowControlFrame(0, 3, STminTime);
			//等待第一帧连续帧
			s32 ret = WaitMessage(Diag_TP_N_Cs);
			if (0 != ret)
			{
				TestReport_HTML_StepDescription("ECU does not send first CF within N_Cs !",Fail);
			}
			else
			{
				t1 = TScanResp.FTimeUs;
			}

			//等待第二帧连续帧
			if (0 == WaitMessage(Diag_TP_N_Cr))
			{
				t2 = TScanResp.FTimeUs;
			}
			else
			{
				TestReport_HTML_StepDescription("ECU does not send CF within N_Cr !",Fail);
			}

			//等待第三帧连续帧
			if (0 == WaitMessage(Diag_TP_N_Cr))
			{
				t3 = TScanResp.FTimeUs;
			}
			else
			{
				TestReport_HTML_StepDescription("ECU does not send CF within N_Cr !",Fail);
			}
			//检查两个连续帧之间的时间间隔是否大于STmin
			if (((t2 - t1) / 1000) > STminTime && ((t3 - t2)) / 1000 > STminTime)
			{

			}
			else
			{
				strcpy(issuereason, "ECU发送的连续帧间隔与STmin不符");
				strcpy(requestInfo, "Tester请求多帧响应");
				strcpy(expectedResponse, "DUT回复的连续帧之间的时间间隔大于STMin");
				strcpy(actualResponse, "DUT回复的连续帧之间的时间间隔小于STMin");
				TestReport_HTML_StepDescription("Time between two consecutive frame is less than STmin !", Fail);
			}
			//检查两个连续帧之间的时间间隔是否小于N_As+N_Cs
			if (((t2 - t1) / 1000) < (Diag_TP_N_As + Diag_TP_N_Cs) && ((t3 - t2)) / 1000 < (Diag_TP_N_As + Diag_TP_N_Cs))
			{

			}
			else
			{
				strcpy(issuereason, "ECU发送的连续帧间隔与STmin不符");
				strcpy(requestInfo, "Tester请求多帧响应");
				strcpy(expectedResponse, "DUT回复的连续帧之间的时间间隔大于STMin");
				strcpy(actualResponse, "DUT回复的连续帧之间的时间间隔小于STMin");
				TestReport_HTML_StepDescription("Time between two consecutive frame is less than STmin !", Fail);
			}

			TestWait(ECUResetTime);
		}
		else
		{
			app.log_text("Test condition not fulfilled, test aborted!", lvlError);
			TestPostCondition_TP_CAN();
			return 0;
		}

		TestWait(stepseptime);
	}
	//检查当前会话
	CheckCurrentSessionTP(0x01);
	
	TestPostCondition_TP_CAN();

	if (resultFlag != 0)
		return 1;
	else
		return 0;
}

s32 test_case_1021950()
{
	strcpy(TestCaseName, "TC_1021950_Valid Separation Time Parameter Test functional addressing - Default session");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - APP");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN();
	//Valid Separation Time Parameter Test functional addressing - Default session

	//检查当前会话
	u8 SFdata1[2] = { 0xF1,0x86 };
	u8 SFdata2[3] = { 0x22,0xED,0x20 };
	s64 t1=0, t2=0, t3=0;
	u16 STminTimeArr[7] = { 0x19,0x32,0x64,0x7F,0xF1,0xF5,0xF9 };

	//检查当前会话
	CheckCurrentSessionTP(0x01,1);

	for (int i = 0; i < 7; i++)
	{
		u32 STminTime = STminTimeArr[i];
		//请求多帧响应
		char sinfo[200] = "";
		sprintf(sinfo, "CASE%d 当前STMin=0x%x", i, STminTime);
		TestReport_HTML_StepDescription(sinfo);
		SendCanSingleFrame(3, SFdata2,1);
		TestReport_HTML_StepDescription("Step: Request Multi Response !");

		if (0 == CheckCanFirstFrame(Diag_TP_N_As))
		{
			TestReport_HTML_StepDescription("Step: Send Flow Control Frame !");
			SendCanFlowControlFrame(0, 3, STminTime);
			//等待第一帧连续帧
			s32 ret = WaitMessage(Diag_TP_N_Cs);
			if (0 != ret)
			{
				TestReport_HTML_StepDescription("ECU does not send first CF within N_Cs !", Fail);
			}
			else
			{
				t1 = TScanResp.FTimeUs;
			}

			//等待第二帧连续帧
			if (0 == WaitMessage( Diag_TP_N_Cr))
			{
				t2 = TScanResp.FTimeUs;
			}
			else
			{
				TestReport_HTML_StepDescription("ECU does not send CF within N_Cr !", Fail);
			}

			//等待第三帧连续帧
			if (0 == WaitMessage( Diag_TP_N_Cr))
			{
				t3 = TScanResp.FTimeUs;
			}
			else
			{
				TestReport_HTML_StepDescription("ECU does not send CF within N_Cr !", Fail);
			}
			//检查两个连续帧之间的时间间隔是否大于STmin
			if (((t2 - t1) / 1000) > STminTime && ((t3 - t2)) / 1000 > STminTime)
			{

			}
			else
			{
				strcpy(issuereason, "ECU发送的连续帧间隔与STmin不符");
				strcpy(requestInfo, "Tester请求多帧响应");
				strcpy(expectedResponse, "DUT回复的连续帧之间的时间间隔大于STMin");
				strcpy(actualResponse, "DUT回复的连续帧之间的时间间隔小于STMin");
				TestReport_HTML_StepDescription("Time between two consecutive frame is less than STmin !", Fail);
			}
			//检查两个连续帧之间的时间间隔是否小于N_As+N_Cs
			if (((t2 - t1) / 1000) < (Diag_TP_N_As + Diag_TP_N_Cs) && ((t3 - t2)) / 1000 < (Diag_TP_N_As + Diag_TP_N_Cs))
			{

			}
			else
			{
				strcpy(issuereason, "ECU发送的连续帧间隔与STmin不符");
				strcpy(requestInfo, "Tester请求多帧响应");
				strcpy(expectedResponse, "DUT回复的连续帧之间的时间间隔大于STMin");
				strcpy(actualResponse, "DUT回复的连续帧之间的时间间隔小于STMin");
				TestReport_HTML_StepDescription("Time between two consecutive frame is less than STmin !", Fail);
			}

			TestWait(stepseptime);
		}
		else
		{
			TestReport_HTML_StepDescription("Test condition not fulfilled, test aborted!", Fail);
			TestPostCondition_TP_CAN();
			continue;
		}

		TestWait(stepseptime);
	}
	//检查当前会话
	CheckCurrentSessionTP(0x01,1);

	TestPostCondition_TP_CAN();

	if (resultFlag != 0)
		return 1;
	else
		return 0;
}

s32 test_case_1021951()
{
	strcpy(TestCaseName, "TC_1021951_Wrong Sequence Number Test physical addressing - Default session");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - APP");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN();
	//Wrong Sequence Number Test physical addressing - Default session
	u8 SFdata[2] = { 0xF1,0x86 };
	u8 FFdata[6] = { 0x10,0x01,0x12,0x13,0x14,0x15 };
	u8 CFdata1[7] = { 0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C };
	u8 CFdata2[7] = { 0x1D,0x1E,0x1F,0x00,0x00,0x00,0x00 };
	//检查当前会话
	CheckCurrentSessionTP( 0x01);

	//发送首帧
	SendCanFirstFrame(0x10, FFdata);
	app.log_text("Step: Send First Frame !", lvlInfo);
	//检查流控帧
	CheckCanFlowControlMessage(Diag_TP_N_Bs);
	//发送连续帧
	TestReport_HTML_StepDescription("发送序列号错误的首帧请求");
	app.log_text("Step: Send Consecutive Frame with Incorrect Sequence Number!", lvlInfo);
	app.wait(Diag_TP_FC_STmin + 1, "");
	SendCanConsecutiveFrame(0x02, CFdata2);
	//发送第二帧连续帧
	TestReport_HTML_StepDescription("发送序列号正确的首帧请求");
	app.log_text("Step: Send Consecutive Frame with Correct Sequence Number!", lvlInfo);
	app.wait(Diag_TP_FC_STmin + 1, "");
	SendCanConsecutiveFrame(0x01, CFdata1);
	//检查是否有响应
	if (0 == WaitMessage(Diag_UDS_P2Client))
	{
		strcpy(issuereason, "连续帧序列号错误，ECU仍然发送响应！");
		strcpy(requestInfo, "Tester请求多帧响应,发送序列号错误的第一帧连续帧");
		strcpy(expectedResponse, "DUT不回复响应");
		strcpy(actualResponse, "DUT收到序列号错误的连续帧后仍然回复响应");
		TestReport_HTML_StepDescription("连续帧序列号错误，ECU仍然发送响应！", Fail);
	}

	//发送首帧
	SendCanFirstFrame(0x10, FFdata);
	app.log_text("Step: Send First Frame !", lvlInfo);
	//检查流控帧
	CheckCanFlowControlMessage(Diag_TP_N_Bs);
	//发送连续帧
	app.log_text("Step: Send Consecutive Frame with Correct Sequence Number!", lvlInfo);
	app.wait(Diag_TP_FC_STmin + 1, "");
	SendCanConsecutiveFrame(0x01, CFdata1);
	//发送第二帧连续帧
	app.log_text("Step: Send Consecutive Frame with Inorrect Sequence Number!", lvlInfo);
	app.wait(Diag_TP_FC_STmin + 1, "");
	TestReport_HTML_StepDescription("发送序列号错误的连续帧");
	SendCanConsecutiveFrame(0x03, CFdata2);
	//发送第三帧连续帧
	app.log_text("Step: Send Consecutive Frame with Correct Sequence Number!", lvlInfo);
	app.wait(Diag_TP_FC_STmin + 1, "");
	TestReport_HTML_StepDescription("发送序列号错误的连续帧");
	SendCanConsecutiveFrame(0x02, CFdata2);
	//检查是否有响应
	if (0 == WaitMessage(Diag_UDS_P2Client))
	{
		strcpy(issuereason, "连续帧序列号错误，ECU仍然发送响应！");
		strcpy(requestInfo, "Tester请求多帧响应,发送序列号错误的第二帧连续帧");
		strcpy(expectedResponse, "DUT不回复响应");
		strcpy(actualResponse, "DUT收到序列号错误的连续帧后仍然回复响应"); 
		TestReport_HTML_StepDescription("连续帧序列号错误，ECU仍然发送响应！", Fail);
	}

	//发送首帧
	SendCanFirstFrame(0x10, FFdata);
	app.log_text("Step: Send First Frame !", lvlInfo);
	//检查流控帧
	CheckCanFlowControlMessage(Diag_TP_N_Bs);
	//发送连续帧
	app.log_text("Step: Send Consecutive Frame with Correct Sequence Number!", lvlInfo);
	app.wait(Diag_TP_FC_STmin + 1, "");
	TestReport_HTML_StepDescription("发送连续帧");
	SendCanConsecutiveFrame(0x01, CFdata1);
	//发送第二帧连续帧
	app.log_text("Step: Send Consecutive Frame with Correct Sequence Number!", lvlInfo);
	app.wait(Diag_TP_FC_STmin + 1, "");
	SendCanConsecutiveFrame(0x02, CFdata2);
	
	CheckCanSingleFrame(1000);
	if (TScanResp.FData[1] != 0x7f || TScanResp.FData[3] != 0x13)
	{
		TestReport_HTML_StepDescription("ECU回复了非预期请求", Fail);
	}

	CheckCurrentSessionTP( 0x01);

	TestPostCondition_TP_CAN();

	if (resultFlag != 0)
		return 1;
	else
		return 0;
}

s32 test_case_1021952()
{
	strcpy(TestCaseName, "TC_1021952_N_As test physical addressing - Default session");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - APP");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN();
	u8 SFdata1[2] = { 0xF1,0x86 };
	u8 SFdata2[3] = { 0x22,0xED,0x20 };
	TCAN InteruptMsg1 = { Diag_ch,1,8,0,1,0,{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0} };
	TCAN InteruptMsg2 = { Diag_ch,1,8,0,2,0,{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0} };
	TCAN InteruptMsg3 = { Diag_ch,1,8,0,3,0,{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0} };
	//检查当前会话
	CheckCurrentSessionTP( 0x01);

	//请求多帧响应
	TestReport_HTML_StepDescription("请求多帧响应 并立即发送高优先级报文，等到N_As*0.9，取消发送高优先级报文");

	//发送高优先级报文使N_As超时
	com.add_cyclic_message_can(&InteruptMsg1, 1);
	com.add_cyclic_message_can(&InteruptMsg2, 1);
	com.add_cyclic_message_can(&InteruptMsg3, 1);

	SendCanSingleFrame(3, SFdata2);
	//等待N_As未超时
	//检查首帧
	app.wait(Diag_TP_N_As*0.9,"");

	//取消周期报文
	com.del_cyclic_messages();
	
	//发送流控帧
	if (0 == CheckCanFirstFrame(Diag_TP_N_As))
	{
		TestReport_HTML_StepDescription("发送流控帧");
		SendCanFlowControlFrame(0x0, 0x0, 0x0);

		CheckCanConsecutiveFrame(Diag_TP_N_Cs);
	}
	else
	{
		TestReport_HTML_StepDescription("未收到ECU响应", Fail);
	}


	//请求多帧响应
	TestReport_HTML_StepDescription("请求多帧响应 并立即发送高优先级报文，等到N_As*1.1，取消发送高优先级报文");
	

	//发送高优先级报文使N_As超时
	com.add_cyclic_message_can(&InteruptMsg1, 1);
	com.add_cyclic_message_can(&InteruptMsg2, 1);
	com.add_cyclic_message_can(&InteruptMsg3, 1);
	SendCanSingleFrame(3, SFdata2);
	//等待N_As未超时
	//检查首帧
	app.wait(Diag_TP_N_As*1.1,"");
	//取消周期报文
	com.del_cyclic_messages();
	
	//发送流控帧
	if (0 == CheckCanFirstFrame(Diag_TP_N_As))
	{
		TestReport_HTML_StepDescription("发送流控帧");
		SendCanFlowControlFrame(0x0, 0x0, 0x0);

		if (0 != CheckNoResp(1000))
		{
			strcpy(issuereason, "N_As超时后ECU仍然发送连续帧");
			strcpy(requestInfo, "请求多帧响应后立即发送高优先级报文干扰总线");
			strcpy(expectedResponse, "ECU回复首帧或不响应");
			strcpy(actualResponse, "ECU在N_As超时后仍然响应");
			TestReport_HTML_StepDescription("N_As超时后ECU仍然发送连续帧", Fail);
		}
	}
	else
	{
		TestReport_HTML_StepDescription("未收到ECU响应", Pass);
	}

	//检查当前会话
	CheckCurrentSessionTP( 0x01);

	TestPostCondition_TP_CAN();

	if (resultFlag != 0)
		return 1;
	else
		return 0;
}

s32 test_case_1021953()
{
	strcpy(TestCaseName, "TC_1021953_N_As test functional addressing - Default session");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - APP");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN();
	u8 SFdata1[2] = { 0xF1,0x86 };
	u8 SFdata2[3] = { 0x22,0xED,0x20 };
	TCAN InteruptMsg1 = { Diag_ch,1,8,0,1,0,{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0} };
	TCAN InteruptMsg2 = { Diag_ch,1,8,0,1,0,{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0} };
	TCAN InteruptMsg3 = { Diag_ch,1,8,0,1,0,{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0} };
	//检查当前会话
	CheckCurrentSessionTP( 0x01,1);

	//请求多帧响应
	TestReport_HTML_StepDescription("请求多帧响应 并立即发送高优先级报文，等到N_As*0.9，取消发送高优先级报文");

	//发送高优先级报文使N_As超时
	com.add_cyclic_message_can(&InteruptMsg1, 1);
	com.add_cyclic_message_can(&InteruptMsg2, 1);
	com.add_cyclic_message_can(&InteruptMsg3, 1);
	SendCanSingleFrame(3, SFdata2, 1);
	//等待N_As未超时
	//检查首帧
	app.wait(Diag_TP_N_As * 0.9, "");

	//取消周期报文
	com.del_cyclic_messages();

	//发送流控帧
	if (0 == CheckCanFirstFrame(Diag_TP_N_As))
	{
		TestReport_HTML_StepDescription("发送流控帧");
		SendCanFlowControlFrame(0x0, 0x0, 0x0);

		CheckCanConsecutiveFrame(Diag_TP_N_Cs);
	}
	else
	{
		TestReport_HTML_StepDescription("未收到ECU响应", Fail);
	}


	//请求多帧响应
	TestReport_HTML_StepDescription("请求多帧响应 并立即发送高优先级报文，等到N_As*1.1，取消发送高优先级报文");

	//发送高优先级报文使N_As超时
	com.add_cyclic_message_can(&InteruptMsg1, 1);
	com.add_cyclic_message_can(&InteruptMsg2, 1);
	com.add_cyclic_message_can(&InteruptMsg3, 1);
	SendCanSingleFrame(3, SFdata2, 1);
	//等待N_As未超时
	//检查首帧
	app.wait(Diag_TP_N_As * 1.1, "");
	//取消周期报文
	com.del_cyclic_messages();

	//发送流控帧
	if (0 == CheckCanFirstFrame(Diag_TP_N_As))
	{
		TestReport_HTML_StepDescription("发送流控帧");
		SendCanFlowControlFrame(0x0, 0x0, 0x0);

		if (0 != CheckNoResp(1000))
		{
			strcpy(issuereason, "N_As超时后ECU仍然发送连续帧");
			strcpy(requestInfo, "请求多帧响应后立即发送高优先级报文干扰总线");
			strcpy(expectedResponse, "ECU回复首帧或不响应");
			strcpy(actualResponse, "ECU在N_As超时后仍然响应");
			TestReport_HTML_StepDescription("N_As超时后ECU仍然发送连续帧", Fail);
		}
	}
	else
	{
		TestReport_HTML_StepDescription("未收到ECU响应", Pass);
	}

	//检查当前会话
	CheckCurrentSessionTP( 0x01,1);
	
	TestPostCondition_TP_CAN();

	if (resultFlag != 0)
		return 1;
	else
		return 0;
}

s32 test_case_1021954()
{
	strcpy(TestCaseName, "TC_1021954_N_Ar test physical addressing - Default session");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - APP");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN();
	u8 SFdata1[2] = { 0xF1,0x86 };
	u8 FFdata[6] = { 0x10,0x01,0x02,0x03,0x04,0x05 };
	u8 CFdata[7] = { 0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C };
	TCAN InteruptMsg1 = { Diag_ch,1,8,0,1,0,{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0} };
	TCAN InteruptMsg2 = { Diag_ch,1,8,0,1,0,{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0} };
	TCAN InteruptMsg3 = { Diag_ch,1,8,0,1,0,{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0} };
	//检查当前会话
	CheckCurrentSessionTP( 0x01);

	//发送首帧
	TestReport_HTML_StepDescription("发送首帧，并立即发送高优先级报文，等到N_Ar*0.9，取消发送高优先级报文");

	//发送高优先级报文
	com.add_cyclic_message_can(&InteruptMsg1, 1);
	com.add_cyclic_message_can(&InteruptMsg2, 1);
	com.add_cyclic_message_can(&InteruptMsg3, 1);
	SendCanFirstFrame(0x0D, FFdata);
	//等待N_Ar未超时
	//检查流控帧
	//等待N_Ar未超时
	app.wait(Diag_TP_N_Ar * 0.9, "");
	//取消周期报文
	com.del_cyclic_messages();
	
	CheckCanFlowControlMessage(1000);

	//发送连续帧
	SendCanConsecutiveFrame(0x01, CFdata);
	//期望否定响应
	if (0 == WaitMessage(Diag_UDS_P2Client))
	{
		if (TScanResp.FData[0] == 0x7F && TScanResp.FData[2] == 0x13)
		{

		}
		else
		{
			TestReport_HTML_StepDescription("收到了非预期响应", Fail);
		}
	}
	else 
	{
		TestReport_HTML_StepDescription("N_Ar未超时情况下未收到响应", Fail);
	}

	//发送多帧请求
	app.log_text("Step: Send Multi Request !", lvlInfo);

	//发送高优先级报文使N_Ar超时
	com.add_cyclic_message_can(&InteruptMsg1, 1);
	com.add_cyclic_message_can(&InteruptMsg2, 1);
	com.add_cyclic_message_can(&InteruptMsg3, 1);
	SendCanFirstFrame(0x0D, FFdata);
	//等待N_Ar超时
	//检查流控帧
	//等待N_Ar超时
	app.wait(Diag_TP_N_Ar * 1.1, "");
	//取消周期报文
	com.del_cyclic_messages();
	
	CheckCanFlowControlMessage(1000);
	
	//发送连续帧
	SendCanConsecutiveFrame(0x01, CFdata);
	if (0 != WaitMessage(1000))
	{

		TestReport_HTML_StepDescription("N_Ar超时情况下未收到响应", Pass);
	}
	else
	{
		TestReport_HTML_StepDescription("N_Ar超时情况下仍然收到响应", Fail);
	}
	

	//检查当前会话
	CheckCurrentSessionTP( 0x01);
	
	TestPostCondition_TP_CAN();

	if (resultFlag != 0)
		return 1;
	else
		return 0;
}

s32 test_case_1021955() {
	strcpy(TestCaseName, "TC_1021955_Check_Timeout_Bs Physical Addressing - Default session");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - APP");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN(); 

	u8 req1[2] = { 0xF1,0X86 };
	CheckCurrentSessionTP( 0x01);


	u8 req_date[40] = { 0x10,0x01,0x01,0x03,0x04,0x05 };
	s64 T1 = 0, T2 = 0;
	
	
	SendCanFirstFrame(0x22, req_date, Physical);
	T1 = TScanReq.FTimeUs;

	if (CheckCanFlowControlMessage(Diag_TP_FC_BS) == 0) {
		T2 = TScanResp.FTimeUs;
	}
	else
	{
		TestReport_HTML_StepDescription("未收到流控帧", Fail);
	}
	if ((T2 - T1) / 1000 < Diag_TP_N_Bs) {
		app.log_text(("DUT sends FC in " + std::to_string((T2 - T1) / 1000)).c_str(), lvlOK);
		char ss[200] = "";
		strcpy(ss, ("DUT sends FC in " + std::to_string((T2 - T1) / 1000)).c_str());
		TestReport_HTML_StepDescription(ss, Pass);
	}
	else
	{
		strcpy(issuereason, "未在N_Bs时间内收到流控帧");
		strcpy(requestInfo, "Tester发送首帧");
		strcpy(expectedResponse, "ECU在N_Bs时间内收到流控帧");
		strcpy(actualResponse, "ECU在N_Bs时间内未收到流控帧");
		TestReport_HTML_StepDescription("未在N_Bs时间内收到流控帧", Fail);
	}
	
	CheckCurrentSessionTP( 0x01);

	TestPostCondition_TP_CAN();

	if (resultFlag != 0)
		return 1;
	else
		return 0;
}

s32 test_case_1021956() {
	strcpy(TestCaseName, "TC_1021956_Check_Timeout_Cr Physical Addressing - Default session");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - APP");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN(); 
	
	u8 req1[2] = { 0xF1,0X86 };
	CheckCurrentSessionTP( 0x01);

	u8 req_date[40] = { 0x22,0xED,0x20 };
	s64 T1 = 0, T2 = 0;

	SendCanSingleFrame(3, req_date, Physical);
	CheckCanFirstFrame(1000);

	SendCanFlowControlFrame(0, 0, 0);
	T1 = TScanReq.FTimeUs;

	if (0 == CheckCanConsecutiveFrame(Diag_TP_N_Cr))
	{
		T2 = firstCFtime;
		if ((T2 - T1) / 1000 < Diag_TP_N_Cr) {
			app.log_text(("DUT sends CF in " + std::to_string((T2 - T1) / 1000)).c_str(), lvlOK);
		}
		else
		{
			strcpy(issuereason, "ECU未在N_Cr时间内发送连续帧");
			strcpy(requestInfo, "Tester请求多帧应答，ECU发送首帧后Tester发送连续帧");
			strcpy(expectedResponse, "ECU在N_Cr时间内发送连续帧");
			strcpy(actualResponse, "ECU在N_Cr时间内未发送连续帧");
			TestReport_HTML_StepDescription("未在N_Cr内收到连续帧！", Fail);
		}
	}
	else
	{
		TestReport_HTML_StepDescription("未收到连续帧！", Fail);
	}
	
	TestWait(1000);

	CheckCurrentSessionTP( 0x01);

	TestPostCondition_TP_CAN();

	if (resultFlag != 0)
		return 1;
	else
		return 0;
}

s32 test_case_1021957() {
	strcpy(TestCaseName, "TC_1021957_Check_Timeout_Cr Functional Addressing - Default session");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - APP");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN(); 
	
	u8 req1[2] = { 0xF1,0X86 };
	CheckCurrentSessionTP( 0x01,1);

	u8 req_date[] = { 0x22,0xED,0x20 };
	s64 T1 = 0, T2 = 0;

	SendCanSingleFrame(3, req_date, 1);
	CheckCanFirstFrame(1000);

	SendCanFlowControlFrame(0, 0, 0);
	T1 = TScanReq.FTimeUs;

	if (0 == CheckCanConsecutiveFrame(Diag_TP_N_Cr))
	{
		T2 = firstCFtime;
		if ((T2 - T1) / 1000 < Diag_TP_N_Cr) {
			app.log_text(("DUT sends CF in " + std::to_string((T2 - T1) / 1000)).c_str(), lvlOK);
		}
		else
		{
			strcpy(issuereason, "功能寻址下ECU未在N_Cr时间内发送连续帧");
			strcpy(requestInfo, "Tester请求多帧应答，ECU发送首帧后Tester发送连续帧");
			strcpy(expectedResponse, "ECU在N_Cr时间内发送连续帧");
			strcpy(actualResponse, "ECU在N_Cr时间内未发送连续帧");
			TestReport_HTML_StepDescription("未在N_Cr内收到连续帧！", Fail);
		}
	}
	else
	{
		TestReport_HTML_StepDescription("未收到连续帧！", Fail);
	}

	TestWait(1000);

	CheckCurrentSessionTP( 0x01,1);

	TestPostCondition_TP_CAN();

	if (resultFlag != 0)
		return 1;
	else
		return 0;
}

s32 test_case_1021958() {
	strcpy(TestCaseName, "TC_1021958_Invalid _CAN_DLC_SF Physical Addressing - Default session");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - APP");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN(); 
	
	u8 req1[2] = { 0xF1,0X86 };
	CheckCurrentSessionTP( 0x01);

	TCAN f0 = { Diag_ch,0x1,8,0,Diag_Request_ID,0,{0x03, 0x22, 0xF1, 0x86, 0x00, 0x00, 0x00, 0x00} };
	for (int i = 1; i < 8; i++) {
		f0.FDLC = i;

		com.transmit_can_async(&f0);
		app.log_text(("Send SF with incorrect DLC " + std::to_string(i)).c_str(), lvlInfo);
		if (0 == WaitMessage( Diag_UDS_P2Server)) {

			strcpy(issuereason, "Tester发送CAN_DL无效的单帧报文，ECU仍然响应");
			strcpy(requestInfo, "Tester发送CAN_DLC无效的单帧报文");
			strcpy(expectedResponse, "ECU忽略单帧，不回复响应");
			strcpy(actualResponse, "ECU对CAN_DLC无效的单帧回复响应");
			TestReport_HTML_StepDescription("CAN_DL无效仍然收到响应", Fail);
		}
		else {
			TestReport_HTML_StepDescription("CAN_DL无效未收到响应", Pass);
		};
	
	}


	CheckCurrentSessionTP( 0x01);

	TestPostCondition_TP_CAN();

	if (resultFlag != 0)
		return 1;
	else
		return 0;
}

s32 test_case_1021959() {
	strcpy(TestCaseName, "TC_1021959_Invalid _CAN_DLC_SF  Functional Addressing - Default session");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - APP");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN(); 
	
	u8 req1[2] = { 0xF1,0X86 };
	CheckCurrentSessionTP( 0x01,1);

	TCAN f0 = { Diag_ch,0x1,8,0,Diag_Function_ID,0,{0x03, 0x22, 0xF1, 0x86, 0x00, 0x00, 0x00, 0x00} };
	for (int i = 1; i < 8; i++) {
		f0.FDLC = i;

		app.log_text(("Send SF with incorrect DLC " + std::to_string(i)).c_str(), lvlInfo);

		char ss[200] = "";
		strcpy(ss, ("Send SF with incorrect DLC " + std::to_string(i)).c_str());
		TestReport_HTML_StepDescription(ss);

		com.transmit_can_async(&f0);

		if (0 == WaitMessage(Diag_UDS_P2Server)) {
			strcpy(issuereason, "Tester发送CAN_DL无效的功能寻址单帧报文，ECU仍然响应");
			strcpy(requestInfo, "Tester发送CAN_DLC无效的单帧报文");
			strcpy(expectedResponse, "ECU忽略单帧，不回复响应");
			strcpy(actualResponse, "ECU对CAN_DLC无效的单帧回复响应");
			TestReport_HTML_StepDescription("CAN_DL无效仍然收到响应", Fail);
		}
		else {
			TestReport_HTML_StepDescription("CAN_DL无效未收到响应", Pass);
		};
	}

	CheckCurrentSessionTP( 0x01,1);

	 TestPostCondition_TP_CAN();

	 if (resultFlag == 0)
		 return 1;
	 else
		 return 0;
}

s32 test_case_1021960()
{
	strcpy(TestCaseName, "TC_1021960_Invalid _CAN_DLC_FF Physical Addressing - Default session");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - APP");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN(); 

	TCAN FFFrame = { Diag_ch,0x1,8,0,Diag_Request_ID,0,{0x10, 0x22, 0x10, 0x01,0x02, 0x03, 0x04, 0x05} };
	for (int i = 1; i < 8; i++)
	{
		FFFrame.FDLC = i;
		app.log_text(("Send FF with incorrect DLC " + std::to_string(i)).c_str(), lvlInfo);
		char ss[200] = "";
		strcpy(ss, ("Send FF with incorrect DLC " + std::to_string(i)).c_str());
		TestReport_HTML_StepDescription(ss);

		com.transmit_can_async(&FFFrame);

		if (0 == WaitMessage( Diag_UDS_P2Server))
		{
			strcpy(issuereason, "Tester发送CAN_DL无效的首帧报文，ECU仍然响应");
			strcpy(requestInfo, "Tester发送CAN_DLC无效的首帧报文");
			strcpy(expectedResponse, "ECU忽略首帧，不回复响应");
			strcpy(actualResponse, "ECU对CAN_DLC无效的单帧回复响应");
			TestReport_HTML_StepDescription("CAN_DL无效仍然收到响应", Fail);
		}
		else
		{
			TestReport_HTML_StepDescription("CAN_DL无效未收到响应", Pass);
		};
	}

	
	TestPostCondition_TP_CAN();

	if (resultFlag != 0)
		return 1;
	else
		return 0;
}

s32 test_case_1021961()
{
	strcpy(TestCaseName, "TC_1021961_Invalid _CAN_DLC_FC Physical Addressing - Default session");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - APP");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN(); 


	TCAN SFFrame = { Diag_ch,0x1,8,0,Diag_Request_ID,0,{0x03, 0x22, 0xED, 0x20, 0x00, 0x00, 0x00, 0x00} };
	TCAN FCFrame = { Diag_ch,0x1,1,0,Diag_Request_ID,0,{0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} };

	for (int i = 1; i < 8; i++)
	{
		FCFrame.FDLC = i;

		com.transmit_can_async(&SFFrame);
		if (0 != WaitMessage(Diag_UDS_P2Server))
		{
			TestReport_HTML_StepDescription("未收到首帧响应", Fail);
		};

		com.transmit_can_async(&FCFrame);

		if (0 == WaitMessage(Diag_UDS_P2Server))
		{
			strcpy(issuereason, "Tester发送CAN_DL无效的流控帧报文，ECU仍然响应");
			strcpy(requestInfo, "Tester请求多帧应答，发送无效流控帧");
			strcpy(expectedResponse, "ECU忽略流控帧，不回复响应");
			strcpy(actualResponse, "ECU对CAN_DLC无效的流控帧回复响应");
			TestReport_HTML_StepDescription("发送无效流控帧 仍然收到响应", Fail);
		}
		else
		{
			TestReport_HTML_StepDescription("DUT does not respond", Pass);
		};
	}
	
	TestPostCondition_TP_CAN();

	if (resultFlag != 0)
		return 1;
	else
		return 0;
}

s32 test_case_1021962()
{
	strcpy(TestCaseName, "TC_1021962_Invalid _CAN_DLC_CF Physical Addressing - Default session");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - APP");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN();

	TCAN FFFrame = { Diag_ch,0x1,8,0,Diag_Request_ID,0,{0x10, 0x1C, 0x10, 0x01, 0x02, 0x03, 0x04, 0x05} };
	TCAN CFFrame1 = { Diag_ch,0x1,8,0,Diag_Request_ID,0,{0x21, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07} };
	TCAN CFFrame2 = { Diag_ch,0x1,8,0,Diag_Request_ID,0,{0x22, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07} };
	TCAN CFFrame3 = { Diag_ch,0x1,8,0,Diag_Request_ID,0,{0x23, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07} };
	TCAN CFFrame4 = { Diag_ch,0x1,1,0,Diag_Request_ID,0,{0x24, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} };
	

	TestReport_HTML_StepDescription("发送CAN_DL错误的连续帧");

	for (int i = 1; i <= 8; i++)
	{
		CFFrame4.FDLC = i;
		strcpy(printChar, "");
		sprintf(printChar, "发送的最后一帧连续帧CAN_DLC=1");
		TestReport_HTML_StepDescription(printChar);
		com.transmit_can_async(&FFFrame);
		if (0 == CheckCanFlowControlMessage(Diag_UDS_P2Server))
		{
			app.log_text("DUT responds", lvlOK);
		}
		else
		{
			app.log_text("DUT does not respond", lvlError);
		};

		com.transmit_can_async(&CFFrame1);
		com.transmit_can_async(&CFFrame2);
		com.transmit_can_async(&CFFrame3);
		com.transmit_can_async(&CFFrame4);

		if (i == 8)
		{
			CheckCanSingleFrame(1000);
			if (TScanResp.FData[1] != 0x7f || TScanResp.FData[3] != 0x13)
			{
				TestReport_HTML_StepDescription("ECU回复了非预期响应", Fail);
			}
		}
		else
		{
			if (0 != CheckNoResp(Diag_UDS_P2Server))
			{
				strcpy(issuereason, "Tester发送CAN_DL无效的连续帧报文，ECU仍然响应");
				strcpy(requestInfo, "Tester请求多帧传输，发送CAN_DLC无效的连续帧");
				strcpy(expectedResponse, "ECU忽略连续帧，不回复响应");
				strcpy(actualResponse, "ECU对CAN_DLC无效的连续帧帧回复响应");
				TestReport_HTML_StepDescription("连续帧CAN_DL无效情况下ECU仍然发送响应", Fail);
			}
		}
	}
	
	TestPostCondition_TP_CAN();

	if (resultFlag != 0)
		return 1;
	else
		return 0;
}

////////////////////////////CAN DIAGNOSTIC SERVER PBL///////////////////////////////////
s32 test_case_1021963() {
	strcpy(TestCaseName, "TC_1021963_Block size handling physical addressing -Programming session");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - PBL");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN();

	int i = 0, j = 0;

	EnterProgrammingSesson();

	u8 reqDataArray1[2] = { 0xF1,0X86 };
	CheckCurrentSessionTP( 0x02);

	u8 reqDataArray2[3] = { 0x22,0xED,0X20 };

	SendCanSingleFrame(3, reqDataArray2);

	if(0==CheckCanFirstFrame(1000))
	{
		SendCanFlowControlFrame(0, 0, 0);

		CheckCanConsecutiveFrame(1000);
	}

	TestWait(100);

	TestReport_HTML_StepDescription("发送BS值不同的流控帧 检查ECU按照流控帧参数响应");
	for (int BS = 1; BS < 6; BS++)
	{
		// 发送单帧CAN消息
		SendCanSingleFrame(3, reqDataArray2);

		// 检查是否收到第一帧CAN消息
		if (CheckCanFirstFrame(1000) == 0)
		{
			// 根据第一帧消息的长度计算连续帧的数量
			CFcnt = (TScanfdResp.FDLC == 8) ? (FFLength - 6 + 6) / 7 : (FFLength - 6 + 62) / 63;
			printf("CFcnt: %d\n", CFcnt);

			// 根据BS值计算流控帧的数量
			FCcnt = (CFcnt + BS - 1) / BS;
			printf("FCcnt: %d\n", FCcnt);

			// 发送流控帧并等待响应
			for (int i = 0; i < FCcnt; i++)
			{
				SendCanFlowControlFrame(0, BS, 0);
				printf("Sent Flow Control Frame %d\n", i);

				// 处理最后一组消息
				remainingFrames = CFcnt - i * BS;
				printf("Remaining Frames: %d\n", remainingFrames);
				int transferedframe = 0;

				for (int j = 0; j < remainingFrames; j++)
				{
					// 缩短等待时间，确保及时检测到连续帧
					if (WaitMessage(1000) != 0)
					{
						issueflag = 1;
						printf("WaitMessage timeout or error at frame %d\n", j);
						break; // 提前退出循环以避免不必要的等待
					}
					else
					{
						printf("Received Continuous Frame %d\n", j);
						transferedframe++;

						if (transferedframe >= BS)
						{
							remainingFrames -= BS;
							break;
						}
					}
				}

				if (issueflag != 0) break; // 如果发现问题，提前退出外层循环
			}
		}
	}

	if (issueflag != 0)
	{
		strcpy(issuereason, "ECU未按照流控帧BS值返回连续帧！");
		strcpy(requestInfo, "发送BS值不同的流控帧");
		strcpy(expectedResponse, "验证DUT按照BS返回连续帧");
		strcpy(actualResponse, "DUT返回的连续帧数量与流控帧BS不符");
		TestReport_HTML_StepDescription("ECU未按照流控帧BS值返回连续帧！", Fail);
	}


	TestReport_HTML_StepDescription("检查当前会话");
	CheckCurrentSessionTP( 0x02);
	TestPostCondition_TP_CAN();

	if (resultFlag != 0)
		return 1;
	else
		return 0;
}

s32 test_case_1021964() {
	strcpy(TestCaseName, "TC_1021964_Consecutive frame timeout physical addressing - Programming session");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - PBL");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN();

	int i = 0, j = 0;

	EnterProgrammingSesson();

	u8 req1[2] = { 0xF1,0X86 };
	u8 req3[8] = { 0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c };
	u8 req2[6] = { 0x10,0x01,0x02,0x03,0x04,0x05 };
	CheckCurrentSessionTP( 0x02);

	TestReport_HTML_StepDescription("发送首帧请求，等待N_Cr*0.95后，发送连续帧");
	SendCanFirstFrame(0x0D, req2);
	CheckCanFlowControlMessage(1000);

	TestWait(Diag_TP_N_Cr * 0.95);

	SendCanConsecutiveFrame(1, req3);

	CheckCanSingleFrame(1000);
	if (TScanResp.FData[1] != 0x7f || TScanResp.FData[3] != 0x13)
	{
		TestReport_HTML_StepDescription("ECU回复了非预期响应", Fail);
	}

	TestReport_HTML_StepDescription("发送首帧请求，等待N_Cr+25ms后，发送连续帧");
	SendCanFirstFrame(0x0D, req2);
	CheckCanFlowControlMessage(1000);

	TestWait(Diag_TP_N_Cr + 25);

	SendCanConsecutiveFrame(1, req3);

	if (0 != WaitMessage(1000))
		issueflag = 1;
	else
		issueflag = 0;

	if (issueflag != 0)
	{
		strcpy(issuereason, "ECU在N_Cr超时后仍然发送响应！");
		strcpy(requestInfo, "Tester发送首帧，");
		strcpy(expectedResponse, "验证DUT按照BS返回连续帧");
		strcpy(actualResponse, "DUT返回的连续帧数量与流控帧BS不符");
		TestReport_HTML_StepDescription("ECU在N_Cr超时后仍然发送响应！");
	}

	TestReport_HTML_StepDescription("检查当前会话");
	CheckCurrentSessionTP( 0x02);

	TestPostCondition_TP_CAN();



	if (resultFlag != 0)
		return 1;
	else
		return 0;
}

s32 test_case_1021965() {
	strcpy(TestCaseName, "TC_1021965_Flow Control handling with wait frames test physical addressing - Programming session");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - PBL");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN();
	EnterProgrammingSesson();

	u8 req1[2] = { 0xF1,0X86 };
	CheckCurrentSessionTP( 0x02);
	//----------------------------------------第1部分-------------------------------------------------
	u8 req_date[] = { 0x22,0xED ,0x20 };
	SendCanSingleFrame(3, req_date);
	CheckCanFirstFrame(1000);
	app.wait(Diag_TP_N_Bs * 0.95, "");
	SendCanFlowControlFrame(1, 0, 0);
	if (WaitMessage(25) == 0)
	{
		strcpy(issuereason, "ECU在收到等待流控帧后仍然发送响应！");
		strcpy(requestInfo, "Tester请求首帧响应，收到首帧后发送等待流控帧");
		strcpy(expectedResponse, "DUT不发送连续帧");
		strcpy(actualResponse, "DUT仍然发送连续帧");
		TestReport_HTML_StepDescription("ECU发送了连续帧！", Fail);
	}
	SendCanFlowControlFrame(0, 0, 0); if (0 == CheckCanConsecutiveFrame(1000))
	{
		TestReport_HTML_StepDescription("ECU回复了连续帧", Pass);
	}
	else
	{
		strcpy(issuereason, "ECU在收到等待流控帧后收到发送流控帧，仍然不发送响应！");
		strcpy(requestInfo, "Tester请求首帧响应，收到首帧后发送等待流控帧");
		strcpy(expectedResponse, "DUT不发送连续帧，收到发送流控帧后，DUT发送连续帧");
		strcpy(actualResponse, "DUT不发送连续帧，收到发送流控帧后，DUT仍不发送连续帧");
		TestReport_HTML_StepDescription("ECU未回复连续帧", Fail);
	}

	TestWait(stepseptime);
	//----------------------------------------第2部分-------------------------------------------------
	SendCanSingleFrame(3, req_date, Physical);
	CheckCanFirstFrame(1000);
	TestReport_HTML_StepDescription("等待N_Br*1.05");
	app.wait(Diag_TP_N_Br * 1.05, "");
	SendCanFlowControlFrame(1, 0, 0);

	if (WaitMessage(Diag_TP_N_Bs * 0.95) == 0)
	{
		TestReport_HTML_StepDescription("ECU在N_Bs时间内回复了连续帧", Fail);
	}

	SendCanFlowControlFrame(0, 0, 0);

	if (0 != CheckCanConsecutiveFrame(1000))
	{
		strcpy(issuereason, "ECU在收到N_Br超时的等待流控帧后，收到发送流控帧，仍然不发送响应！");
		strcpy(requestInfo, "Tester请求首帧响应，收到首帧后在N_Br超时后发送等待流控帧，然后发送继续发送流控帧");
		strcpy(expectedResponse, "DUT不发送连续帧，收到发送流控帧后，DUT发送连续帧");
		strcpy(actualResponse, "DUT不发送连续帧，收到发送流控帧后，DUT仍不发送连续帧");
		TestReport_HTML_StepDescription("ECU未回复连续帧", Fail);
	}
	TestWait(stepseptime);
	//----------------------------------------第3部分-------------------------------------------------
	SendCanSingleFrame(3, req_date, Physical);
	CheckCanFirstFrame(1000);
	TestReport_HTML_StepDescription("等待N_BS+25ms");
	app.wait(Diag_TP_N_Bs + 25, "");
	SendCanFlowControlFrame(1, 0, 0);
	if (WaitMessage(Diag_TP_N_Bs * 0.5) == 0)
	{
		TestReport_HTML_StepDescription("ECU在N_Bs时间内回复了连续帧", Fail);
	}
	SendCanFlowControlFrame(0, 0, 0);
	if (WaitMessage(1000) == 0)
	{
		TestReport_HTML_StepDescription("ECU在N_Bs超时后回复了连续帧", Fail);
	}

	CheckCurrentSessionTP( 0x02);
	
	TestPostCondition_TP_CAN();

	if (resultFlag != 0)
		return 1;
	else
		return 0;
}

s32 test_case_1021966() {
	strcpy(TestCaseName, "TC_1021966_Flow Control Timeout physical addressing - Programming session");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - PBL");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN();
	EnterProgrammingSesson();

	u8 req1[2] = { 0xF1,0X86 };
	CheckCurrentSessionTP( 0x02);
	TestReport_HTML_StepDescription("请求多帧响应，等待N_Br*0.95后，发送流控帧");
	u8 req_date[] = { 0x22,0xED,0x20 };
	SendCanSingleFrame(3, req_date, Physical);
	CheckCanFirstFrame(1000);
	app.wait(Diag_TP_N_Br * 0.95, "");
	SendCanFlowControlFrame(0, 0, 0);
	CheckCanConsecutiveFrame(Diag_TP_N_Br);

	TestReport_HTML_StepDescription("请求多帧响应，等待N_Br超时");
	SendCanSingleFrame(3, req_date, Physical);
	CheckCanFirstFrame(1000);
	app.wait(Diag_TP_N_Br + 25, "");
	SendCanFlowControlFrame(0, 0, 0);
	if (WaitMessage(1000) == 0)
	{
		strcpy(issuereason, "ECU在N_Br超时后仍然发送连续帧！");
		strcpy(requestInfo, "请求多帧响应，等待N_Br超时后发送流控帧");
		strcpy(expectedResponse, "DUT不发送连续帧");
		strcpy(actualResponse, "DUT在N_Br超时后，仍然发送连续帧");
		TestReport_HTML_StepDescription("在N_Br超时后ECU仍然发送连续帧", Fail);
	}

	CheckCurrentSessionTP( 0x02);

	TestPostCondition_TP_CAN();


	if (resultFlag != 0)
		return 1;
	else
		return 0;
}

s32 test_case_1021967() {
	strcpy(TestCaseName, "TC_1021967_Flow Control handling with max wait frames test physical addressing - Programming session");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - PBL");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN();
	EnterProgrammingSesson();

	int N_WFTmax = 255;
	u8 req1[2] = { 0xF1,0X86 };
	CheckCurrentSessionTP( 0x02, 1);
	u8 req_date[3] = { 0x22,0xED ,0x20 };
	SendCanSingleFrame(3, req_date, 0);
	CheckCanFirstFrame(1000);

	TestReport_HTML_StepDescription("发送等待流控帧 N_WFTMax=255");
	for (int n = 0; n < N_WFTmax; n++)
	{
		SendCanFlowControlFrame(1, 0, 0);
	}
	SendCanFlowControlFrame(0, 0, 0);

	if (CheckCanConsecutiveFrame(1000) != 0)
	{
		strcpy(issuereason, "发送最大数量的等待流控帧后发送继续发送流控帧，DUT不发送连续帧！");
		strcpy(requestInfo, "Tester发送首帧，发送等待流控帧，N_WFTMax=255，然后发送继续发送流控帧");
		strcpy(expectedResponse, "DUT收到等待流控帧时不发送响应，收到继续发送流控帧时发送剩下的连续帧");
		strcpy(actualResponse, "DUT未发送连续帧");
		TestReport_HTML_StepDescription("发送最大数量的等待流控帧后发送继续发送流控帧，DUT不发送连续帧！", Fail);
	}

	CheckCurrentSessionTP( 0x02); 
	
	TestPostCondition_TP_CAN();


	if (resultFlag != 0)
		return 1;
	else
		return 0;
}

s32 test_case_1021968()
{
	strcpy(TestCaseName, "TC_1021968_Handling of Unexpected Frames During Segmented Receive Test physical addressing - Programming sessio");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - PBL");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN();
	EnterProgrammingSesson();

	u8 req1[2] = { 0xF1,0X86 };
	CheckCurrentSessionTP( 0x02);

	u8 req2[6] = { 0x10,0x01,0x02,0x03,0x04,0x05 };

	TestReport_HTML_StepDescription("CASE1 多帧接收被单帧打断");
	SendCanFirstFrame(0x0D, req2);
	CheckCanFlowControlMessage(1000);

	CheckCurrentSessionTP( 0x02);

	u8 req3[7] = { 0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c };

	SendCanConsecutiveFrame(1, req3);

	if (0 != CheckNoResp(25))
	{
		strcpy(issuereason, "多帧接收过程中收到单帧仍然对多帧发送应答！");
		strcpy(requestInfo, "Tester发送首帧，ECU回复流控帧后Tester发送单帧后发送连续帧");
		strcpy(expectedResponse, "DUT回复单帧请求并忽略后续连续帧");
		strcpy(actualResponse, "DUT仍然发送对多帧的响应");
		TestReport_HTML_StepDescription("多帧接收过程中收到单帧仍然对多帧发送应答！", Fail);
	}

	/////////////
	TestReport_HTML_StepDescription("CASE2 多帧接受被首帧打断");
	u8 req4[8] = { 0x13,0x00,0x00,0x00,0x00,0x00,0x00 };
	SendCanFirstFrame(0x13, req4);
	CheckCanFlowControlMessage(1000);
	u8 req5[8] = { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };
	SendCanConsecutiveFrame(1, req5);
	CheckNoResp(1000);
	SendCanFirstFrame(0x0D, req2);
	CheckCanFlowControlMessage(1000);

	SendCanConsecutiveFrame(1, req3);
	CheckCanSingleFrame(1000);
	if (TScanResp.FData[1] != 0x7f || TScanResp.FData[3] != 0x13)
	{
		TestReport_HTML_StepDescription("收到了非预期响应", Fail);
	}
	/////////////
	TestReport_HTML_StepDescription("CASE3 多帧接收忽略非预期流控帧");
	SendCanFirstFrame(0x0D, req2);
	CheckCanFlowControlMessage(1000);

	SendCanFlowControlFrame(0, 0, 0);
	CheckNoResp(25);

	SendCanConsecutiveFrame(1, req3);
	CheckCanSingleFrame(1000);
	if (TScanResp.FData[1] != 0x7f || TScanResp.FData[3] != 0x13)
	{
		resultFlag++;
	}
	///////////////
	TestReport_HTML_StepDescription("CASE4 多帧接受忽略非预期帧");
	SendCanFirstFrame(0x0D, req2);
	CheckCanFlowControlMessage(1000);

	TCAN canReq = { Diag_ch,1,8,0,Diag_Request_ID,0,{0x40,0x0,0x0,0x0,0x0,0x0,0x0,0x0} };
	com.transmit_can_async(&TScanReq);

	CheckNoResp(25);

	SendCanConsecutiveFrame(1, req3);
	CheckCanSingleFrame(1000);
	if (TScanResp.FData[1] != 0x7f || TScanResp.FData[3] != 0x13)
	{
		TestReport_HTML_StepDescription("收到了非预期响应！", Fail);
	}

	CheckCurrentSessionTP( 0x02);
	TestPostCondition_TP_CAN();

	if (resultFlag != 0)
		return 1;
	else
		return 0;
}

s32 test_case_1021969()
{
	strcpy(TestCaseName, "TC_1021969_Full-duplex Handling of unexpected frames during segmented transmission transmit test physical");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - PBL");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN();

	EnterProgrammingSesson();

	u8 req1[2] = { 0xF1,0X86 };
	CheckCurrentSessionTP( 0x02);

	TestReport_HTML_StepDescription("CASE1 多帧传输忽略非预期单帧");
	u8 req_date[3] = { 0x22,0xED ,0x20 };
	SendCanSingleFrame(3, req_date, 0);
	CheckCanFirstFrame(1000);

	CheckCurrentSessionTP( 0x02);

	SendCanFlowControlFrame(0, 0, 0);
	CheckCanConsecutiveFrame(1000);
	CheckCanSingleFrame(1000);



	TestReport_HTML_StepDescription("CASE2 多帧传输忽略非预期首帧");
	SendCanSingleFrame(3, req_date, 0);
	CheckCanFirstFrame(1000);
	TCAN f3 = { Diag_ch,0x1,8,0,Diag_Request_ID,0,{0x10, 0x0D, 0x13, 0x00, 0x00, 0x00, 0x00, 0x00} };
	com.transmit_can_async(&f3);

	SendCanFlowControlFrame(0, 0, 0);
	CheckCanConsecutiveFrame(1000);

	CheckCanFlowControlMessage(1000);

	u8 req2[8] = {};
	SendCanConsecutiveFrame(1, req2);
	CheckCanSingleFrame(1000);
	if (TScanReq.FData[1] != 0x7f || TScanResp.FData[3] != 0x11)
	{
		resultFlag++;
	}

	TestReport_HTML_StepDescription("CASE3 多帧传输忽略非预期连续帧");
	SendCanSingleFrame(3, req_date, 0);
	CheckCanFirstFrame(1000);
	TCAN f5 = { Diag_ch,0x1,8,0,Diag_Request_ID,0,{0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} };
	com.transmit_can_async(&f5);
	SendCanFlowControlFrame(0, 0, 0);
	CheckCanConsecutiveFrame(1000);

	TestReport_HTML_StepDescription("CASE4 多帧传输忽略非预期流控帧");
	SendCanSingleFrame(3, req_date, 0);
	CheckCanFirstFrame(1000);
	SendCanFlowControlFrame(0, 0, 0);
	SendCanFlowControlFrame(0, 1, 0);
	CheckCanConsecutiveFrame(1000);

	TestReport_HTML_StepDescription("CASE5 多帧传输忽略非预期帧");
	SendCanSingleFrame(3, req_date, 0);
	CheckCanFirstFrame(1000);
	TCAN f9 = { Diag_ch,0x1,8,0,Diag_Request_ID,0,{0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} };
	com.transmit_can_async(&f9);
	SendCanFlowControlFrame(0, 0, 0);
	CheckCanConsecutiveFrame(1000);

	CheckCurrentSessionTP( 0x02);
	TestPostCondition_TP_CAN();

	if (resultFlag != 0)
		return 1;
	else
		return 0;
}

s32 test_case_1021970()
{
	strcpy(TestCaseName, "TC_1021970_Full-duplex Handling of unexpected frames during segmented transmission transmit test functional");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - PBL");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN();

	EnterProgrammingSesson();

	u8 req1[2] = { 0xF1,0X86 };
	CheckCurrentSessionTP( 0x02,1);

	TestReport_HTML_StepDescription("CASE1 多帧传输忽略非预期单帧");
	u8 req_date[3] = { 0x22,0xED ,0x20 };
	SendCanSingleFrame(3, req_date, 1);
	CheckCanFirstFrame(1000);

	CheckCurrentSessionTP( 0x02,1);

	SendCanFlowControlFrame(0, 0, 0);
	CheckCanConsecutiveFrame(1000);
	CheckCanSingleFrame(1000);



	TestReport_HTML_StepDescription("CASE2 多帧传输忽略非预期首帧");
	SendCanSingleFrame(3, req_date, 1);
	CheckCanFirstFrame(1000);
	TCAN f3 = { Diag_ch,0x1,8,0,Diag_Function_ID,0,{0x10, 0x0D, 0x13, 0x00, 0x00, 0x00, 0x00, 0x00} };
	com.transmit_can_async(&f3);

	SendCanFlowControlFrame(0, 0, 0);
	CheckCanConsecutiveFrame(1000);

	CheckCanFlowControlMessage(1000);

	u8 req2[8] = {};
	SendCanConsecutiveFrame(1, req2);
	CheckCanSingleFrame(1000);
	if (TScanReq.FData[1] != 0x7f || TScanResp.FData[3] != 0x11)
	{
		resultFlag++;
	}

	TestReport_HTML_StepDescription("CASE3 多帧传输忽略非预期连续帧");
	SendCanSingleFrame(3, req_date, 1);
	CheckCanFirstFrame(1000);
	TCAN f5 = { Diag_ch,0x1,8,0,Diag_Request_ID,0,{0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} };
	com.transmit_can_async(&f5);
	SendCanFlowControlFrame(0, 0, 0);
	CheckCanConsecutiveFrame(1000);

	TestReport_HTML_StepDescription("CASE4 多帧传输忽略非预期流控帧");
	SendCanSingleFrame(3, req_date, 1);
	CheckCanFirstFrame(1000);
	SendCanFlowControlFrame(0, 0, 0);
	SendCanFlowControlFrame(0, 1, 0);
	CheckCanConsecutiveFrame(1000);

	TestReport_HTML_StepDescription("CASE5 多帧传输忽略非预期帧");
	SendCanSingleFrame(3, req_date, 1);
	CheckCanFirstFrame(1000);
	TCAN f9 = { Diag_ch,0x1,8,0,Diag_Request_ID,0,{0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} };
	com.transmit_can_async(&f9);
	SendCanFlowControlFrame(0, 0, 0);
	CheckCanConsecutiveFrame(1000);

	CheckCurrentSessionTP( 0x02,1);
	
	TestPostCondition_TP_CAN();

	if (resultFlag != 0)
		return 1;
	else
		return 0;
}

s32 test_case_1021971()
{
	strcpy(TestCaseName, "TC_1021971_Valid Separation Time Parameter Test physical addressing - Programming session");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - PBL");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN();
	EnterProgrammingSesson();

	u8 SFdata1[2] = { 0xF1,0x86 };
	u8 SFdata2[3] = { 0x22,0xED,0x20 };
	s64 t1=0, t2=0, t3=0;
	u16 STminTime;

	//检查当前会话
	CheckCurrentSessionTP( 0x02,1);

	for (int i = 0; i < 7; i++)
	{
		switch (i)
		{
		case 0:
			STminTime = 0x19;
			break;
		case 1:
			STminTime = 0x32;
			break;
		case 2:
			STminTime = 0x64;
			break;
		case 3:
			STminTime = 0x7F;
			break;
		case 4:
			STminTime = 0xF1;
			break;
		case 5:
			STminTime = 0xF5;
			break;
		case 6:
			STminTime = 0xF9;
			break;
		default:
			break;
		}
		//请求多帧响应
		char sinfo[200] = "";
		sprintf(sinfo, "CASE%d 当前STMin=0x%x", i, STminTime);
		TestReport_HTML_StepDescription(sinfo);
		SendCanSingleFrame(3, SFdata2,1);
		app.log_text("Step: Request Multi Response !", lvlInfo);

		if (0 == CheckCanFirstFrame(Diag_TP_N_As))
		{
			app.log_text("Step: Send Flow Control Frame !", lvlInfo);
			SendCanFlowControlFrame(0, 3, STminTime);
			//等待第一帧连续帧
			s32 ret = WaitMessage(Diag_TP_N_Cs);
			if (0 != ret)
			{
				app.log_text("ECU does not send first CF within N_Cs !", lvlError);
			}
			else
			{
				t1 = TScanResp.FTimeUs;
			}

			//等待第二帧连续帧
			if (0 == WaitMessage( Diag_TP_N_Cr))
			{
				t2 = TScanResp.FTimeUs;
			}
			else
			{
				app.log_text("ECU does not send CF within N_Cr !", lvlError);
			}

			//等待第三帧连续帧
			if (0 == WaitMessage( Diag_TP_N_Cr))
			{
				t3 = TScanResp.FTimeUs;
			}
			else
			{
				app.log_text("ECU does not 1111 send CF within N_Cr !", lvlError);
			}
			//检查两个连续帧之间的时间间隔是否大于STmin
			if (((t2 - t1) / 1000) > STminTime && ((t3 - t2)) / 1000 > STminTime)
			{

			}
			else
			{
				strcpy(issuereason, "ECU发送的连续帧间隔与STmin不符");
				strcpy(requestInfo, "Tester请求多帧响应");
				strcpy(expectedResponse, "DUT回复的连续帧之间的时间间隔大于STMin");
				strcpy(actualResponse, "DUT回复的连续帧之间的时间间隔小于STMin");
				TestReport_HTML_StepDescription("Time between two consecutive frame is less than STmin !", Fail);
			}
			//检查两个连续帧之间的时间间隔是否小于N_As+N_Cs
			if (((t2 - t1) / 1000) < (Diag_TP_N_As + Diag_TP_N_Cs) && ((t3 - t2)) / 1000 < (Diag_TP_N_As + Diag_TP_N_Cs))
			{

			}
			else
			{
				strcpy(issuereason, "ECU发送的连续帧间隔与STmin不符");
				strcpy(requestInfo, "Tester请求多帧响应");
				strcpy(expectedResponse, "DUT回复的连续帧之间的时间间隔大于STMin");
				strcpy(actualResponse, "DUT回复的连续帧之间的时间间隔小于STMin");
				TestReport_HTML_StepDescription("Time between two consecutive frame is less than STmin !", Fail);
			}
			TestWait(stepseptime);
		}
		else
		{
			app.log_text("Test condition not fulfilled, test aborted!", lvlError);
			TestPostCondition_TP_CAN();
			return 0;
		}
	}
	TestWait(stepseptime);
	//检查当前会话
	CheckCurrentSessionTP( 0x02,1);
	
	TestPostCondition_TP_CAN();

	if (resultFlag != 0)
		return 1;
	else
		return 0;
}

s32 test_case_1021972()
{
	strcpy(TestCaseName, "TC_1021972_Wrong Sequence Number Test physical addressing - Programming session");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - PBL");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN();
	EnterProgrammingSesson();

	//Wrong Sequence Number Test physical addressing - Default session
	u8 SFdata[2] = { 0xF1,0x86 };
	u8 FFdata[6] = { 0x10,0x01,0x12,0x13,0x14,0x15 };
	u8 CFdata1[7] = { 0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C };
	u8 CFdata2[7] = { 0x1D,0x1E,0x1F,0x00,0x00,0x00,0x00 };
	//检查当前会话
	CheckCurrentSessionTP( 0x02);

	//发送首帧
	SendCanFirstFrame(0x10, FFdata);
	app.log_text("Step: Send First Frame !", lvlInfo);
	//检查流控帧
	CheckCanFlowControlMessage(Diag_TP_N_Bs);
	//发送连续帧
	TestReport_HTML_StepDescription("发送序列号错误的首帧请求");
	app.log_text("Step: Send Consecutive Frame with Incorrect Sequence Number!", lvlInfo);
	app.wait(Diag_TP_FC_STmin + 1, "");
	SendCanConsecutiveFrame(0x02, CFdata2);
	//发送第二帧连续帧
	TestReport_HTML_StepDescription("发送序列号正确的首帧请求");
	app.log_text("Step: Send Consecutive Frame with Correct Sequence Number!", lvlInfo);
	app.wait(Diag_TP_FC_STmin + 1, "");
	SendCanConsecutiveFrame(0x01, CFdata1);
	//检查是否有响应
	if (0 == WaitMessage(Diag_UDS_P2Client))
	{
		strcpy(issuereason, "连续帧序列号错误，ECU仍然发送响应！");
		strcpy(requestInfo, "Tester请求多帧响应,发送序列号错误的第一帧连续帧");
		strcpy(expectedResponse, "DUT不回复响应");
		strcpy(actualResponse, "DUT收到序列号错误的连续帧后仍然回复响应");
		TestReport_HTML_StepDescription("连续帧序列号错误，ECU仍然发送响应！", Fail);
	}

	//发送首帧
	SendCanFirstFrame(0x10, FFdata);
	app.log_text("Step: Send First Frame !", lvlInfo);
	//检查流控帧
	CheckCanFlowControlMessage(Diag_TP_N_Bs);
	//发送连续帧
	TestReport_HTML_StepDescription("发送连续帧");
	app.log_text("Step: Send Consecutive Frame with Correct Sequence Number!", lvlInfo);
	app.wait(Diag_TP_FC_STmin + 1, "");
	SendCanConsecutiveFrame(0x01, CFdata1);
	//发送第二帧连续帧
	TestReport_HTML_StepDescription("发送序列号错误的连续帧");
	app.log_text("Step: Send Consecutive Frame with Inorrect Sequence Number!", lvlInfo);
	app.wait(Diag_TP_FC_STmin + 1, "");
	SendCanConsecutiveFrame(0x03, CFdata2);
	//发送第三帧连续帧
	TestReport_HTML_StepDescription("发送序列号错误的连续帧");
	app.log_text("Step: Send Consecutive Frame with Correct Sequence Number!", lvlInfo);
	app.wait(Diag_TP_FC_STmin + 1, "");
	SendCanConsecutiveFrame(0x02, CFdata2);
	//检查是否有响应
	if (0 == WaitMessage(Diag_UDS_P2Client))
	{
		strcpy(issuereason, "连续帧序列号错误，ECU仍然发送响应！");
		strcpy(requestInfo, "Tester请求多帧响应,发送序列号错误的第二帧连续帧");
		strcpy(expectedResponse, "DUT不回复响应");
		strcpy(actualResponse, "DUT收到序列号错误的连续帧后仍然回复响应");
		TestReport_HTML_StepDescription("连续帧序列号错误，ECU仍然发送响应！", Fail);
	}

	//发送首帧
	SendCanFirstFrame(0x10, FFdata);
	app.log_text("Step: Send First Frame !", lvlInfo);
	//检查流控帧
	CheckCanFlowControlMessage(Diag_TP_N_Bs);
	//发送连续帧
	TestReport_HTML_StepDescription("发送连续帧");
	app.log_text("Step: Send Consecutive Frame with Correct Sequence Number!", lvlInfo);
	app.wait(Diag_TP_FC_STmin + 1, "");
	SendCanConsecutiveFrame(0x01, CFdata1);
	//发送第二帧连续帧
	app.log_text("Step: Send Consecutive Frame with Correct Sequence Number!", lvlInfo);
	app.wait(Diag_TP_FC_STmin + 1, "");
	SendCanConsecutiveFrame(0x02, CFdata2);

	CheckCanSingleFrame(1000);
	if (TScanResp.FData[1] != 0x7f || TScanResp.FData[3] != 0x13)
	{
		TestReport_HTML_StepDescription("ECU回复了非预期请求", Fail);
	}

	CheckCurrentSessionTP( 0x02);

	TestPostCondition_TP_CAN();

	if (resultFlag != 0)
		return 1;
	else
		return 0;
}

s32 test_case_1021973()
{
	strcpy(TestCaseName, "TC_1021973_N_As test physical addressing - Programming session");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - PBL");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN();
	EnterProgrammingSesson();

	u8 SFdata1[2] = { 0xF1,0x86 };
	u8 SFdata2[3] = { 0x22,0xED,0x20 };
	TCAN InteruptMsg1 = { Diag_ch,1,8,0,1,0,{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0} };
	TCAN InteruptMsg2 = { Diag_ch,1,8,0,1,0,{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0} };
	TCAN InteruptMsg3 = { Diag_ch,1,8,0,1,0,{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0} };
	//检查当前会话
	CheckCurrentSessionTP( 0x02);

	//请求多帧响应
	TestReport_HTML_StepDescription("请求多帧响应 并立即发送高优先级报文，等到N_As*0.9，取消发送高优先级报文");
	SendCanSingleFrame(3, SFdata2);
	app.log_text("Step: Request Multi Resposne !", lvlInfo);

	//发送高优先级报文使N_As超时
	com.add_cyclic_message_can(&InteruptMsg1, 1);
	com.add_cyclic_message_can(&InteruptMsg2, 1);
	com.add_cyclic_message_can(&InteruptMsg3, 1);
	//等待N_As未超时
	app.wait(Diag_TP_N_As * 0.9, "");
	//取消周期报文
	com.del_cyclic_messages();
	//检查首帧
	//发送流控帧
	if (0 == CheckCanFirstFrame(Diag_TP_N_As))
	{
		TestReport_HTML_StepDescription("发送流控帧");
		SendCanFlowControlFrame(0x0, 0x0, 0x0);

		CheckCanConsecutiveFrame(Diag_TP_N_Cs);
	}
	else
	{
		TestReport_HTML_StepDescription("未收到ECU响应", Fail);
	}


	//请求多帧响应
	TestReport_HTML_StepDescription("请求多帧响应 并立即发送高优先级报文，等到N_As*1.1，取消发送高优先级报文");
	SendCanSingleFrame(3, SFdata2);
	app.log_text("Step: Request Multi Resposne !", lvlInfo);

	//发送高优先级报文使N_As超时
	com.add_cyclic_message_can(&InteruptMsg1, 1);
	com.add_cyclic_message_can(&InteruptMsg2, 1);
	com.add_cyclic_message_can(&InteruptMsg3, 1);
	//等待N_As未超时
	app.wait(Diag_TP_N_As * 1.1, "");
	//取消周期报文
	com.del_cyclic_messages();
	//检查首帧
	if (0 == CheckCanFirstFrame(Diag_TP_N_As))
	{
		TestReport_HTML_StepDescription("发送流控帧");
		SendCanFlowControlFrame(0x0, 0x0, 0x0);

		if (0 != CheckNoResp(1000))
		{
			strcpy(issuereason, "N_As超时后ECU仍然发送连续帧");
			strcpy(requestInfo, "请求多帧响应后立即发送高优先级报文干扰总线");
			strcpy(expectedResponse, "ECU回复首帧或不响应");
			strcpy(actualResponse, "ECU在N_As超时后仍然响应");
			TestReport_HTML_StepDescription("N_As超时后ECU仍然发送连续帧", Fail);
		}
	}
	else
	{
		TestReport_HTML_StepDescription("未收到ECU响应", Pass);
	}

	//检查当前会话
	CheckCurrentSessionTP( 0x02);
	
	TestPostCondition_TP_CAN();

	if (resultFlag != 0)
		return 1;
	else
		return 0;
}

s32 test_case_1021974()
{
	strcpy(TestCaseName, "TC_1021974_N_Ar test physical addressing - Programming session");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - PBL");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN();
	EnterProgrammingSesson();

	u8 SFdata1[2] = { 0xF1,0x86 };
	u8 FFdata[6] = { 0x10,0x01,0x02,0x03,0x04,0x05 };
	u8 CFdata[7] = { 0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C };
	TCAN InteruptMsg1 = { Diag_ch,1,8,0,1,0,{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0} };
	TCAN InteruptMsg2 = { Diag_ch,1,8,0,1,0,{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0} };
	TCAN InteruptMsg3 = { Diag_ch,1,8,0,1,0,{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0} };
	//检查当前会话
	CheckCurrentSessionTP( 0x02);

	//发送首帧
	TestReport_HTML_StepDescription("发送首帧，并立即发送高优先级报文，等到N_Ar*0.9，取消发送高优先级报文");
	SendCanFirstFrame(0x0D, FFdata);
	app.log_text("Step: Send First Frame !", lvlInfo);

	//发送高优先级报文
	com.add_cyclic_message_can(&InteruptMsg1, 1);
	com.add_cyclic_message_can(&InteruptMsg2, 1);
	com.add_cyclic_message_can(&InteruptMsg3, 1);
	//等待N_Ar未超时
	app.wait(Diag_TP_N_Ar * 0.9, "");
	//取消周期报文
	com.del_cyclic_messages();
	//检查流控帧
	if (0 == CheckCanFirstFrame(Diag_TP_N_As))
	{
		TestReport_HTML_StepDescription("发送流控帧");
		SendCanFlowControlFrame(0x0, 0x0, 0x0);

		CheckCanConsecutiveFrame(Diag_TP_N_Cs);
	}
	else
	{
		TestReport_HTML_StepDescription("未收到ECU响应", Fail);
	}

	//发送多帧请求
	TestReport_HTML_StepDescription("发送首帧，并立即发送高优先级报文，等到N_Ar*1.1，取消发送高优先级报文");
	SendCanFirstFrame(0x0D, FFdata);
	app.log_text("Step: Send Multi Request !", lvlInfo);

	//发送高优先级报文使N_Ar超时
	com.add_cyclic_message_can(&InteruptMsg1, 1);
	com.add_cyclic_message_can(&InteruptMsg2, 1);
	com.add_cyclic_message_can(&InteruptMsg3, 1);
	//等待N_Ar超时
	app.wait(Diag_TP_N_Ar * 1.1, "");
	//取消周期报文
	com.del_cyclic_messages();
	//检查流控帧
	if (0 == CheckCanFirstFrame(Diag_TP_N_As))
	{
		TestReport_HTML_StepDescription("发送流控帧");
		SendCanFlowControlFrame(0x0, 0x0, 0x0);

		if (0 != CheckNoResp(1000))
		{
			strcpy(issuereason, "N_As超时后ECU仍然发送连续帧");
			strcpy(requestInfo, "请求多帧响应后立即发送高优先级报文干扰总线");
			strcpy(expectedResponse, "ECU回复首帧或不响应");
			strcpy(actualResponse, "ECU在N_As超时后仍然响应");
			TestReport_HTML_StepDescription("N_As超时后ECU仍然发送连续帧", Fail);
		}
	}
	else
	{
		TestReport_HTML_StepDescription("未收到ECU响应", Pass);
	}

	//检查当前会话
	CheckCurrentSessionTP( 0x02);
	
	TestPostCondition_TP_CAN();

	if (resultFlag != 0)
		return 1;
	else
		return 0;
}

s32 test_case_1021975() {
	strcpy(TestCaseName, "TC_1021975_Check_Timeout_Bs- Programming session");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - PBL");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN();
	EnterProgrammingSesson();

	u8 req1[2] = { 0xF1,0X86 };
	CheckCurrentSessionTP( 0x02);


	u8 req_date[] = { 0x10,0x01,0x01,0x03,0x04,0x05 };
	s64 T1 = 0, T2 = 0;


	SendCanFirstFrame(0x22, req_date, Physical);
	T1 = TScanReq.FTimeUs;

	if (CheckCanFlowControlMessage(Diag_TP_FC_BS) == 0) {
		T2 = TScanResp.FTimeUs;
	}
	else
	{
		TestReport_HTML_StepDescription("未收到流控帧", Fail);
	}
	if ((T2 - T1) / 1000 < Diag_TP_N_Bs) {
		app.log_text(("DUT sends FC in " + std::to_string((T2 - T1) / 1000)).c_str(), lvlOK);
		char ss[200] = "";
		strcpy(ss, ("DUT sends FC in " + std::to_string((T2 - T1) / 1000)).c_str());
		TestReport_HTML_StepDescription(ss, Pass);
	}
	else
	{
		strcpy(issuereason, "未在N_Bs时间内收到流控帧");
		strcpy(requestInfo, "Tester发送首帧");
		strcpy(expectedResponse, "ECU在N_Bs时间内收到流控帧");
		strcpy(actualResponse, "ECU在N_Bs时间内未收到流控帧");
		TestReport_HTML_StepDescription("未在N_Bs时间内收到流控帧", Fail);
	}

	CheckCurrentSessionTP( 0x02);
	TestPostCondition_TP_CAN();

	if (resultFlag != 0)
		return 1;
	else
		return 0;
}

s32 test_case_1021976() {
	strcpy(TestCaseName, "TC_1021976_Check_Timeout_Cr- Programming session");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - PBL");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN();
	EnterProgrammingSesson();

	u8 req1[2] = { 0xF1,0X86 };
	CheckCurrentSessionTP( 0x02);

	u8 req_date[] = { 0x22,0xED,0x20 };
	s64 T1 = 0, T2 = 0;

	SendCanSingleFrame(3, req_date, Physical);
	CheckCanFirstFrame(1000);

	SendCanFlowControlFrame(0, 0, 0);
	T1 = TScanReq.FTimeUs;

	if (0 == CheckCanConsecutiveFrame(Diag_TP_N_Cr))
	{
		T2 = firstCFtime;
		if ((T2 - T1) / 1000 < Diag_TP_N_Cr) {
			app.log_text(("DUT sends FC in " + std::to_string((T2 - T1) / 1000)).c_str(), lvlOK);
		}
		else
		{
			strcpy(issuereason, "ECU未在N_Cr时间内发送连续帧");
			strcpy(requestInfo, "Tester请求多帧应答，ECU发送首帧后Tester发送连续帧");
			strcpy(expectedResponse, "ECU在N_Cr时间内发送连续帧");
			strcpy(actualResponse, "ECU在N_Cr时间内未发送连续帧");
			TestReport_HTML_StepDescription("未在N_Cr内收到连续帧！", Fail);
		}
	}
	else
	{
		TestReport_HTML_StepDescription("未收到连续帧！", Fail);
	}

	CheckCurrentSessionTP( 0x02);
	TestPostCondition_TP_CAN();

	if (resultFlag != 0)
		return 1;
	else
		return 0;
}

s32 test_case_1021977() {
	strcpy(TestCaseName, "TC_1021977_Invalid _CAN_DLC_SF- Programming session");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - PBL");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN();
	EnterProgrammingSesson();

	u8 req1[2] = { 0xF1,0X86 };
	CheckCurrentSessionTP( 0x02);

	TCAN f0 = { Diag_ch,0x1,8,0,Diag_Request_ID,0,{0x03, 0x22, 0xF1, 0x86, 0x00, 0x00, 0x00, 0x00} };
	for (int i = 1; i < 8; i++) {
		f0.FDLC = i;

		com.transmit_can_async(&f0);
		app.log_text(("Send SF with incorrect DLC " + std::to_string(i)).c_str(), lvlInfo);
		if (0 == WaitMessage(Diag_UDS_P2Server)) {
			strcpy(issuereason, "Tester发送CAN_DL无效的单帧报文，ECU仍然响应");
			strcpy(requestInfo, "Tester发送CAN_DLC无效的单帧报文");
			strcpy(expectedResponse, "ECU忽略单帧，不回复响应");
			strcpy(actualResponse, "ECU对CAN_DLC无效的单帧回复响应");
			TestReport_HTML_StepDescription("CAN_DL无效仍然收到响应", Fail);
		}
		else {
			TestReport_HTML_StepDescription("CAN_DL无效未收到响应", Pass);
		};
		app.wait(1000, "");
	}


	CheckCurrentSessionTP( 0x02);
	TestPostCondition_TP_CAN();

	if (resultFlag != 0)
		return 1;
	else
		return 0;
}

s32 test_case_1021978()
{
	strcpy(TestCaseName, "TC_1021978_Invalid _CAN_DLC_FF- Programming session");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - PBL");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN();

	EnterProgrammingSesson();

	TCAN FFFrame = { Diag_ch,0x1,8,0,Diag_Function_ID,0,{0x10, 0x22, 0x10, 0x01,0x02, 0x03, 0x04, 0x05} };
	for (int i = 1; i < 8; i++)
	{
		FFFrame.FDLC = i;
		com.transmit_can_async(&FFFrame);
		app.log_text(("Send FF with incorrect DLC " + std::to_string(i)).c_str(), lvlInfo);

		if (0 == WaitMessage(Diag_UDS_P2Server))
		{
			strcpy(issuereason, "Tester发送CAN_DL无效的首帧报文，ECU仍然响应");
			strcpy(requestInfo, "Tester发送CAN_DLC无效的首帧报文");
			strcpy(expectedResponse, "ECU忽略首帧，不回复响应");
			strcpy(actualResponse, "ECU对CAN_DLC无效的单帧回复响应");
			TestReport_HTML_StepDescription("CAN_DL无效仍然收到响应", Fail);
		}
		else {
			TestReport_HTML_StepDescription("CAN_DL无效未收到响应", Pass);
		};
	}
	
	TestPostCondition_TP_CAN();

	if (resultFlag != 0)
		return 1;
	else
		return 0;
}

s32 test_case_1021979()
{
	strcpy(TestCaseName, "TC_1021979_Invalid _CAN_DLC_FC- Programming session");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - PBL");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN();
	EnterProgrammingSesson();

	TCAN SFFrame = { Diag_ch,0x1,8,0,Diag_Request_ID,0,{0x03, 0x22, 0xED, 0x20, 0x00, 0x00, 0x00, 0x00} };
	TCAN FCFrame = { Diag_ch,0x1,1,0,Diag_Request_ID,0,{0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} };

	for (int i = 1; i < 8; i++)
	{
		FCFrame.FDLC = i;
		com.transmit_can_async(&SFFrame);

		app.log_text(("Send SF/FC with incorrect DLC " + std::to_string(i)).c_str(), lvlInfo);
		if (0 == CheckCanSingleFrame(Diag_UDS_P2Server))
		{
			app.log_text("DUT responds", lvlOK);
		}
		else
		{
			TestReport_HTML_StepDescription("未收到首帧响应", Fail);
		};

		com.transmit_can_async(&FCFrame);

		if (0 == WaitMessage(Diag_UDS_P2Server))
		{
			strcpy(issuereason, "Tester发送CAN_DL无效的流控帧报文，ECU仍然响应");
			strcpy(requestInfo, "Tester请求多帧应答，发送无效流控帧");
			strcpy(expectedResponse, "ECU忽略流控帧，不回复响应");
			strcpy(actualResponse, "ECU对CAN_DLC无效的流控帧回复响应");
			TestReport_HTML_StepDescription("发送无效流控帧 仍然收到响应", Fail);
		}
		else
		{
			app.log_text("DUT does not respond", lvlOK);
		};
	}
	
	TestPostCondition_TP_CAN();

	if (resultFlag != 0)
		return 1;
	else
		return 0;
}

s32 test_case_1021980()
{
	strcpy(TestCaseName, "TC_1021980_Invalid _CAN_DLC_CF- Programming session");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - PBL");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN();
	EnterProgrammingSesson();

	TCAN FFFrame = { Diag_ch,0x1,8,0,Diag_Request_ID,0,{0x10, 0x1C, 0x10, 0x01, 0x02, 0x03, 0x04, 0x05} };
	TCAN CFFrame1 = { Diag_ch,0x1,8,0,Diag_Request_ID,0,{0x21, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07} };
	TCAN CFFrame2 = { Diag_ch,0x1,8,0,Diag_Request_ID,0,{0x22, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07} };
	TCAN CFFrame3 = { Diag_ch,0x1,8,0,Diag_Request_ID,0,{0x23, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07} };
	TCAN CFFrame4 = { Diag_ch,0x1,1,0,Diag_Request_ID,0,{0x24, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} };


	for (int i = 1; i <= 8; i++)
	{
		CFFrame4.FDLC = i;
		com.transmit_can_async(&FFFrame);

		app.log_text(("Send FF/CF with incorrect DLC " + std::to_string(i)).c_str(), lvlInfo);
		if (0 == CheckCanFlowControlMessage(Diag_UDS_P2Server))
		{
			TestReport_HTML_StepDescription("发送CAN_DL错误的连续帧");
		}
		else
		{
			app.log_text("DUT does not respond", lvlError);
		};

		com.transmit_can_async(&CFFrame1);
		com.transmit_can_async(&CFFrame2);
		com.transmit_can_async(&CFFrame3);
		com.transmit_can_async(&CFFrame4);

		if (i == 8)
		{
			CheckCanSingleFrame(1000);
			if (TScanResp.FData[1] != 0x7f || TScanResp.FData[3] != 0x13)
			{
				resultFlag++;
			}
		}
		else
		{
			if (0 != CheckNoResp(Diag_UDS_P2Server))
			{
				strcpy(issuereason, "Tester发送CAN_DL无效的连续帧报文，ECU仍然响应");
				strcpy(requestInfo, "Tester请求多帧传输，发送CAN_DLC无效的连续帧");
				strcpy(expectedResponse, "ECU忽略连续帧，不回复响应");
				strcpy(actualResponse, "ECU对CAN_DLC无效的连续帧帧回复响应");
				TestReport_HTML_StepDescription("连续帧CAN_DL无效情况下ECU仍然发送响应", Fail);
			}
		}
	}
	
	TestPostCondition_TP_CAN();

	if (resultFlag != 0)
		return 1;
	else
		return 0;
}

s32 test_case_1021981()
{
	strcpy(TestCaseName, "TC_1021981_Separation _time_SF_ProgrammingSession Physical Addressing");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - PBL");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN();

	EnterProgrammingSesson();

	u8 req1[2] = { 0xF1,0X86 };
	u8 req2[3] = { 10,01 };
	CheckCurrentSessionTP( 0x02);
	SendCanSingleFrame(0x03, req2);
	if (0 != CheckCanSingleFrame(1000))
	{
		TestReport_HTML_StepDescription("ECU未回复响应");
	}
	if (TScanResp.FData[2] != 0xf1 || TScanResp.FData[3] != 0x86 || TScanResp.FData[4] != 0x02)
	{
		TestReport_HTML_StepDescription("ECU回复非预期报文");
	}
	if (0 != CheckCanSingleFrame(1000))
	{
		TestReport_HTML_StepDescription("ECU未回复响应");
	}
	if (TScanResp.FData[2] != 0xf1 || TScanResp.FData[3] != 0x86 || TScanResp.FData[4] != 0x02)
	{
		TestReport_HTML_StepDescription("ECU回复非预期报文");
	}

	TestPostCondition_TP_CAN();

	if (resultFlag != 0)
		return 1;
	else
		return 0;
}

////////////////////////////CAN DIAGNOSTIC SERVER SBL///////////////////////////////////
//此case会挂死
s32 test_case_1021982()
{
	strcpy(TestCaseName, "TC_1021982_Full-duplex - Handling of Unexpected Frames During Segmented Transmit Test physical");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - SBL");
	strcpy(testPurpose_CH, "测试目的：");

	//vbfparser();

	TestPrecondition_TP_CAN();
	EnterProgrammingSesson();

	u8 req1[2] = { 0xF1,0X86 };
	CheckCurrentSessionTP( 0x02);

	TestReport_HTML_StepDescription("下载SBL");
	ReportDescriptionFlag = 0;
	uds_create();
	DownloadSBL();
	uds_delete();
	ReportDescriptionFlag = 1;

	TestReport_HTML_StepDescription("CASE1 多帧传输忽略非预期单帧");
	u8 req_date[3] = { 0x22,0xED ,0x20 };
	SendCanSingleFrame(3, req_date, 0);
	CheckCanFirstFrame(1000);

	CheckCurrentSessionTP( 0x02);

	SendCanFlowControlFrame(0, 0, 0);
	CheckCanConsecutiveFrame(1000);
	CheckCanSingleFrame(1000);

	TestReport_HTML_StepDescription("CASE2 多帧传输忽略非预期首帧");
	SendCanSingleFrame(3, req_date, 0);
	CheckCanFirstFrame(1000);
	TCAN f3 = { Diag_ch,0x1,8,0,Diag_Request_ID,0,{0x10, 0x0D, 0x13, 0x00, 0x00, 0x00, 0x00, 0x00} };
	com.transmit_can_async(&f3);

	SendCanFlowControlFrame(0, 0, 0);
	CheckCanConsecutiveFrame(1000);

	CheckCanFlowControlMessage(1000);

	u8 req2[8] = {};
	SendCanConsecutiveFrame(1, req2);
	CheckCanSingleFrame(1000);
	if (TScanReq.FData[1] != 0x7f || TScanResp.FData[3] != 0x11)
	{
		resultFlag++;
	}

	TestReport_HTML_StepDescription("CASE3 多帧传输忽略非预期连续帧");
	SendCanSingleFrame(3, req_date, 0);
	CheckCanFirstFrame(1000);
	TCAN f5 = { Diag_ch,0x1,8,0,Diag_Request_ID,0,{0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} };
	com.transmit_can_async(&f5);
	SendCanFlowControlFrame(0, 0, 0);
	CheckCanConsecutiveFrame(1000);

	TestReport_HTML_StepDescription("CASE4 多帧传输忽略非预期流控帧");
	SendCanSingleFrame(3, req_date, 0);
	CheckCanFirstFrame(1000);
	SendCanFlowControlFrame(0, 0, 0);
	SendCanFlowControlFrame(0, 1, 0);
	CheckCanConsecutiveFrame(1000);

	TestReport_HTML_StepDescription("CASE5 多帧传输忽略非预期帧");
	SendCanSingleFrame(3, req_date, 0);
	CheckCanFirstFrame(1000);
	TCAN f9 = { Diag_ch,0x1,8,0,Diag_Request_ID,0,{0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} };
	com.transmit_can_async(&f9);
	SendCanFlowControlFrame(0, 0, 0);
	CheckCanConsecutiveFrame(1000);

	CheckCurrentSessionTP( 0x02);
	
	TestPostCondition_TP_CAN();

	if (resultFlag != 0)
		return 1;
	else
		return 0;
}
			   
s32 test_case_1021983()
{
	strcpy(TestCaseName, "TC_1021983_Full-duplex Handling of unexpected frames during segmented transmission transmit test functional");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - SBL");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN();
	EnterProgrammingSesson();

	u8 req1[2] = { 0xF1,0X86 };
	CheckCurrentSessionTP( 0x02, 1); 
	
	TestReport_HTML_StepDescription("下载SBL");
	ReportDescriptionFlag = 0;
	uds_create();
	DownloadSBL();
	uds_delete();
	ReportDescriptionFlag = 1;

	TestReport_HTML_StepDescription("CASE1 多帧传输忽略非预期单帧");
	u8 req_date[3] = { 0x22,0xED ,0x20 };
	SendCanSingleFrame(3, req_date, 1);
	CheckCanFirstFrame(1000);

	CheckCurrentSessionTP( 0x02, 1);

	SendCanFlowControlFrame(0, 0, 0);
	CheckCanConsecutiveFrame(1000);
	CheckCanSingleFrame(1000);



	TestReport_HTML_StepDescription("CASE2 多帧传输忽略非预期首帧");
	SendCanSingleFrame(3, req_date, 1);
	CheckCanFirstFrame(1000);
	TCAN f3 = { Diag_ch,0x1,8,0,Diag_Function_ID,0,{0x10, 0x0D, 0x13, 0x00, 0x00, 0x00, 0x00, 0x00} };
	com.transmit_can_async(&f3);

	SendCanFlowControlFrame(0, 0, 0);
	CheckCanConsecutiveFrame(1000);

	CheckCanFlowControlMessage(1000);

	u8 req2[8] = {};
	SendCanConsecutiveFrame(1, req2);
	CheckCanSingleFrame(1000);
	if (TScanReq.FData[1] != 0x7f || TScanResp.FData[3] != 0x11)
	{
		resultFlag++;
	}

	TestReport_HTML_StepDescription("CASE3 多帧传输忽略非预期连续帧");
	SendCanSingleFrame(3, req_date, 1);
	CheckCanFirstFrame(1000);
	TCAN f5 = { Diag_ch,0x1,8,0,Diag_Request_ID,0,{0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} };
	com.transmit_can_async(&f5);
	SendCanFlowControlFrame(0, 0, 0);
	CheckCanConsecutiveFrame(1000);

	TestReport_HTML_StepDescription("CASE4 多帧传输忽略非预期流控帧");
	SendCanSingleFrame(3, req_date, 1);
	CheckCanFirstFrame(1000);
	SendCanFlowControlFrame(0, 0, 0);
	SendCanFlowControlFrame(0, 1, 0);
	CheckCanConsecutiveFrame(1000);

	TestReport_HTML_StepDescription("CASE5 多帧传输忽略非预期帧");
	SendCanSingleFrame(3, req_date, 1);
	CheckCanFirstFrame(1000);
	TCAN f9 = { Diag_ch,0x1,8,0,Diag_Request_ID,0,{0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} };
	com.transmit_can_async(&f9);
	SendCanFlowControlFrame(0, 0, 0);
	CheckCanConsecutiveFrame(1000);

	CheckCurrentSessionTP( 0x02, 1);
	
	TestPostCondition_TP_CAN();

	if (resultFlag != 0)
		return 1;
	else
		return 0;
}
			   
s32 test_case_1021984() {

	strcpy(TestCaseName, "TC_1021984_Block size handling physical addressing -Programming session");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - SBL");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN();

	int i = 0, j = 0;

	EnterProgrammingSesson();

	u8 reqDataArray1[2] = { 0xF1,0X86 };
	CheckCurrentSessionTP( 0x02);

	TestReport_HTML_StepDescription("下载SBL");
	ReportDescriptionFlag = 0;
	uds_create();
	DownloadSBL();
	uds_delete();
	ReportDescriptionFlag = 1;

	u8 reqDataArray2[3] = { 0x22,0xED,0X20 };

	SendCanSingleFrame(3, reqDataArray2);

	if(0==CheckCanFirstFrame(1000))
	{
		SendCanFlowControlFrame(0, 0, 0);

		CheckCanConsecutiveFrame(1000);
	}

	TestWait(100);

	TestReport_HTML_StepDescription("发送BS值不同的流控帧 检查ECU按照流控帧参数响应");
	for (int BS = 1; BS < 6; BS++)
	{
		// 发送单帧CAN消息
		SendCanSingleFrame(3, reqDataArray2);

		// 检查是否收到第一帧CAN消息
		if (CheckCanFirstFrame(1000) == 0)
		{
			// 根据第一帧消息的长度计算连续帧的数量
			CFcnt = (TScanfdResp.FDLC == 8) ? (FFLength - 6 + 6) / 7 : (FFLength - 6 + 62) / 63;
			printf("CFcnt: %d\n", CFcnt);

			// 根据BS值计算流控帧的数量
			FCcnt = (CFcnt + BS - 1) / BS;
			printf("FCcnt: %d\n", FCcnt);

			// 发送流控帧并等待响应
			for (int i = 0; i < FCcnt; i++)
			{
				SendCanFlowControlFrame(0, BS, 0);
				printf("Sent Flow Control Frame %d\n", i);

				// 处理最后一组消息
				remainingFrames = CFcnt - i * BS;
				printf("Remaining Frames: %d\n", remainingFrames);
				int transferedframe = 0;

				for (int j = 0; j < remainingFrames; j++)
				{
					// 缩短等待时间，确保及时检测到连续帧
					if (WaitMessage(1000) != 0)
					{
						issueflag = 1;
						printf("WaitMessage timeout or error at frame %d\n", j);
						break; // 提前退出循环以避免不必要的等待
					}
					else
					{
						printf("Received Continuous Frame %d\n", j);
						transferedframe++;

						if (transferedframe >= BS)
						{
							remainingFrames -= BS;
							break;
						}
					}
				}

				if (issueflag != 0) break; // 如果发现问题，提前退出外层循环
			}
		}

	}

	if (issueflag != 0)
	{
		strcpy(issuereason, "ECU未按照流控帧BS值返回连续帧！");
		strcpy(requestInfo, "发送BS值不同的流控帧");
		strcpy(expectedResponse, "验证DUT按照BS返回连续帧");
		strcpy(actualResponse, "DUT返回的连续帧数量与流控帧BS不符");
		TestReport_HTML_StepDescription("ECU未按照流控帧BS值返回连续帧！");
	}

	CheckCurrentSessionTP( 0x02);
	TestPostCondition_TP_CAN();

	if (resultFlag != 0)
		return 1;
	else
		return 0;
}
			   
s32 test_case_1021985() {
	strcpy(TestCaseName, "TC_1021985_Consecutive frame timeout physical addressing - Programming session");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - SBL");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN();
	
	EnterProgrammingSesson();

	u8 req1[2] = { 0xF1,0X86 };
	CheckCurrentSessionTP( 0x02);

	TestReport_HTML_StepDescription("下载SBL");
	ReportDescriptionFlag = 0;
	uds_create();
	DownloadSBL();
	uds_delete();
	ReportDescriptionFlag = 1;

	u8 req2[6] = { 0x10,0x01,0x02,0x03,0x04,0x05 };

	TestReport_HTML_StepDescription("发送首帧请求，等待N_Cr*0.95后，发送连续帧");
	SendCanFirstFrame(0x0D, req2);
	CheckCanFlowControlMessage(1000);

	TestWait(Diag_TP_N_Cr * 0.95);

	u8 req3[8] = { 0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c };

	SendCanConsecutiveFrame(1, req3);

	CheckCanSingleFrame(1000);
	if (TScanResp.FData[1] != 0x7f || TScanResp.FData[3] != 0x13)
	{
		TestReport_HTML_StepDescription("ECU回复了非预期响应", Fail);
	}


	TestReport_HTML_StepDescription("发送首帧请求，等待N_Cr*1.1后，发送连续帧");
	SendCanFirstFrame(0x0D, req2);
	CheckCanFlowControlMessage(1000);

	TestWait(Diag_TP_N_Cr + 25);

	SendCanConsecutiveFrame(1, req3);

	if (0 != WaitMessage(1000))
		issueflag = 1;
	else
		issueflag = 0;

	if (issueflag != 0)
	{
		strcpy(issuereason, "ECU在N_Cr超时后仍然发送响应！");
		strcpy(requestInfo, "Tester发送首帧，");
		strcpy(expectedResponse, "验证DUT按照BS返回连续帧");
		strcpy(actualResponse, "DUT返回的连续帧数量与流控帧BS不符");
		TestReport_HTML_StepDescription("ECU在N_Cr超时后仍然发送响应！", Fail);
	}

	CheckCurrentSessionTP( 0x02);


	TestPostCondition_TP_CAN();

	if (resultFlag != 0)
		return 1;
	else
		return 0;

}
			   
s32 test_case_1021986() {
	strcpy(TestCaseName, "TC_1021986_Flow Control handling with wait frames test physical addressing - Programming session");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - SBL");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN();
	EnterProgrammingSesson();

	u8 req1[2] = { 0xF1,0X86 };
	CheckCurrentSessionTP( 0x02);

	TestReport_HTML_StepDescription("下载SBL");
	ReportDescriptionFlag = 0;
	uds_create();
	DownloadSBL();
	uds_delete();
	ReportDescriptionFlag = 1;
	//----------------------------------------第1部分-------------------------------------------------
	u8 req_date[] = { 0x22,0xED ,0x20 };
	SendCanSingleFrame(3, req_date);
	CheckCanFirstFrame(1000);
	app.wait(Diag_TP_N_Bs * 0.95, "");
	SendCanFlowControlFrame(1, 0, 0);
	if (WaitMessage(25) == 0)
	{
		strcpy(issuereason, "ECU在收到等待流控帧后仍然发送响应！");
		strcpy(requestInfo, "Tester请求首帧响应，收到首帧后发送等待流控帧");
		strcpy(expectedResponse, "DUT不发送连续帧");
		strcpy(actualResponse, "DUT仍然发送连续帧");
		TestReport_HTML_StepDescription("ECU发送了连续帧！", Fail);
	}
	SendCanFlowControlFrame(0, 0, 0);
	if (0 == CheckCanConsecutiveFrame(1000))
	{
		TestReport_HTML_StepDescription("ECU回复了连续帧", Pass);
	}
	else
	{
		strcpy(issuereason, "ECU在收到等待流控帧后收到发送流控帧，仍然不发送响应！");
		strcpy(requestInfo, "Tester请求首帧响应，收到首帧后发送等待流控帧");
		strcpy(expectedResponse, "DUT不发送连续帧，收到发送流控帧后，DUT发送连续帧");
		strcpy(actualResponse, "DUT不发送连续帧，收到发送流控帧后，DUT仍不发送连续帧");
		TestReport_HTML_StepDescription("ECU未回复连续帧", Fail);
	}

	TestWait(stepseptime);
	//----------------------------------------第2部分-------------------------------------------------
	SendCanSingleFrame(3, req_date, Physical);
	CheckCanFirstFrame(1000);
	TestReport_HTML_StepDescription("等待N_Br*1.05");
	app.wait(Diag_TP_N_Br * 1.05, "");
	SendCanFlowControlFrame(1, 0, 0);

	if (WaitMessage(Diag_TP_N_Bs * 0.95) == 0)
	{
		TestReport_HTML_StepDescription("ECU在N_Bs时间内回复了连续帧", Fail);
	}

	SendCanFlowControlFrame(0, 0, 0);
	if (0 != CheckCanConsecutiveFrame(1000))
	{
		strcpy(issuereason, "ECU在收到N_Br超时的等待流控帧后，收到发送流控帧，仍然不发送响应！");
		strcpy(requestInfo, "Tester请求首帧响应，收到首帧后在N_Br超时后发送等待流控帧，然后发送继续发送流控帧");
		strcpy(expectedResponse, "DUT不发送连续帧，收到发送流控帧后，DUT发送连续帧");
		strcpy(actualResponse, "DUT不发送连续帧，收到发送流控帧后，DUT仍不发送连续帧");
		TestReport_HTML_StepDescription("ECU未回复连续帧", Fail);
	}
	TestWait(stepseptime);
	//----------------------------------------第3部分-------------------------------------------------
	SendCanSingleFrame(3, req_date, Physical);
	CheckCanFirstFrame(1000);
	TestReport_HTML_StepDescription("等待N_BS+25ms");
	app.wait(Diag_TP_N_Bs + 25, "");
	SendCanFlowControlFrame(1, 0, 0);
	if (WaitMessage(Diag_TP_N_Bs * 0.5) == 0)
	{
		TestReport_HTML_StepDescription("ECU在N_Bs时间内回复了连续帧", Fail);
	}
	SendCanFlowControlFrame(0, 0, 0);
	if (WaitMessage(1000) == 0)
	{
		TestReport_HTML_StepDescription("ECU在N_Bs超时后回复了连续帧", Fail);
	}

	CheckCurrentSessionTP( 0x02); 
	
	TestPostCondition_TP_CAN();

	if (resultFlag != 0)
		return 1;
	else
		return 0;
}
			   
s32 test_case_1021987() {
	strcpy(TestCaseName, "TC_1021987_Flow Control Timeout physical addressing - Programming session");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - SBL");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN();
	EnterProgrammingSesson();

	u8 req1[2] = { 0xF1,0X86 };
	CheckCurrentSessionTP( 0x02);

	TestReport_HTML_StepDescription("下载SBL");
	ReportDescriptionFlag = 0;
	uds_create();
	DownloadSBL();
	uds_delete();
	ReportDescriptionFlag = 1;

	u8 req_date[] = { 0x22,0xED,0x20 };
	TestReport_HTML_StepDescription("请求多帧响应，等待N_Br*0.95后，发送流控帧");
	SendCanSingleFrame(3, req_date, Physical);
	CheckCanFirstFrame(1000);
	app.wait(Diag_TP_N_Br * 0.95, "");
	SendCanFlowControlFrame(0, 0, 0);
	CheckCanConsecutiveFrame(Diag_TP_N_Br);



	TestReport_HTML_StepDescription("请求多帧响应，等待N_Br超时");
	SendCanSingleFrame(3, req_date, Physical);
	CheckCanFirstFrame(1000);
	app.wait(Diag_TP_N_Br + 25, "");
	SendCanFlowControlFrame(0, 0, 0);
	if (WaitMessage(1000) == 0)
	{
		strcpy(issuereason, "ECU在N_Br超时后仍然发送连续帧！");
		strcpy(requestInfo, "请求多帧响应，等待N_Br超时后发送流控帧");
		strcpy(expectedResponse, "DUT不发送连续帧");
		strcpy(actualResponse, "DUT在N_Br超时后，仍然发送连续帧");
		TestReport_HTML_StepDescription("在N_Br超时后ECU仍然发送连续帧", Fail);
	}

	CheckCurrentSessionTP( 0x02);
	
	TestPostCondition_TP_CAN();

	if (resultFlag != 0)
		return 1;
	else
		return 0;
}
			   
s32 test_case_1021988()
{
	strcpy(TestCaseName, "TC_1021988_Flow Control handling with max wait frames test physical addressing - Programming session");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - SBL");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN();
	EnterProgrammingSesson();

	int N_WFTmax = 255;
	u8 req1[2] = { 0xF1,0X86 };
	CheckCurrentSessionTP( 0x02, 1);

	TestReport_HTML_StepDescription("下载SBL");
	ReportDescriptionFlag = 0;
	uds_create();
	DownloadSBL();
	uds_delete();
	ReportDescriptionFlag = 1;

	u8 req_date[3] = { 0x22,0xED ,0x20 };
	SendCanSingleFrame(3, req_date, 0);
	CheckCanFirstFrame(1000);

	TestReport_HTML_StepDescription("发送等待流控帧 N_WFTMax=255");
	for (int n = 0; n < N_WFTmax; n++)
	{
		SendCanFlowControlFrame(1, 0, 0);
	}
	
	if (CheckCanConsecutiveFrame(1000) != 0)
	{
		strcpy(issuereason, "发送最大数量的等待流控帧后发送继续发送流控帧，DUT不发送连续帧！");
		strcpy(requestInfo, "Tester发送首帧，发送等待流控帧，N_WFTMax=255，然后发送继续发送流控帧");
		strcpy(expectedResponse, "DUT收到等待流控帧时不发送响应，收到继续发送流控帧时发送剩下的连续帧");
		strcpy(actualResponse, "DUT未发送连续帧");
		TestReport_HTML_StepDescription("发送最大数量的等待流控帧后发送继续发送流控帧，DUT不发送连续帧！");
	}

	CheckCurrentSessionTP( 0x02); 
	
	TestPostCondition_TP_CAN();

	if (resultFlag != 0)
		return 1;
	else
		return 0;
}
			   
s32 test_case_1021989()
{
	strcpy(TestCaseName, "TC_1021989_Handling of Unexpected Frames During Segmented Receive Test physical addressing - Programming sessio");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - SBL");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN();
	EnterProgrammingSesson();

	u8 req1[2] = { 0xF1,0X86 };
	CheckCurrentSessionTP( 0x02);

	TestReport_HTML_StepDescription("下载SBL");
	ReportDescriptionFlag = 0;
	uds_create();
	DownloadSBL();
	uds_delete();
	ReportDescriptionFlag = 1;


	u8 req2[6] = { 0x10,0x01,0x02,0x03,0x04,0x05 };

	TestReport_HTML_StepDescription("CASE1 多帧接收被单帧打断");
	SendCanFirstFrame(0x0D, req2);
	CheckCanFlowControlMessage(1000);

	CheckCurrentSessionTP( 0x02);

	u8 req3[7] = { 0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c };

	SendCanConsecutiveFrame(1, req3);

	if (0 != CheckNoResp(25))
	{
		strcpy(issuereason, "多帧接收过程中收到单帧仍然对多帧发送应答！");
		strcpy(requestInfo, "Tester发送首帧，ECU回复流控帧后Tester发送单帧后发送连续帧");
		strcpy(expectedResponse, "DUT回复单帧请求并忽略后续连续帧");
		strcpy(actualResponse, "DUT仍然发送对多帧的响应");
		TestReport_HTML_StepDescription("多帧接收过程中收到单帧仍然对多帧发送应答！", Fail);
	}

	u8 req4[8] = { 0x13,0x00,0x00,0x00,0x00,0x00,0x00 };
	SendCanFirstFrame(0x13, req4);
	CheckCanFlowControlMessage(1000);
	u8 req5[8] = { 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };
	SendCanConsecutiveFrame(1, req5);
	CheckNoResp(1000);
	/////////////
	TestReport_HTML_StepDescription("CASE2 多帧接受被首帧打断");
	SendCanFirstFrame(0x0D, req2);
	CheckCanFlowControlMessage(1000);

	SendCanConsecutiveFrame(1, req3);
	CheckCanSingleFrame(1000);
	if (TScanResp.FData[1] != 0x7f || TScanResp.FData[3] != 0x13)
	{
		TestReport_HTML_StepDescription("收到了非预期响应", Fail);
	}
	/////////////
	TestReport_HTML_StepDescription("CASE3 多帧接收忽略非预期流控帧");
	SendCanFirstFrame(0x0D, req2);
	CheckCanFlowControlMessage(1000);

	SendCanFlowControlFrame(0, 0, 0);
	CheckNoResp(25);

	SendCanConsecutiveFrame(1, req3);
	CheckCanSingleFrame(1000);
	if (TScanResp.FData[1] != 0x7f || TScanResp.FData[3] != 0x13)
	{
		TestReport_HTML_StepDescription("收到了非预期响应", Fail);
	}
	///////////////
	TestReport_HTML_StepDescription("CASE4 多帧接受忽略非预期帧");
	SendCanFirstFrame(0x0D, req2);
	CheckCanFlowControlMessage(1000);

	TCAN canReq = { Diag_ch,1,8,0,Diag_Request_ID,0,{0x40,0x0,0x0,0x0,0x0,0x0,0x0,0x0} };
	com.transmit_can_async(&TScanReq);

	CheckNoResp(25);

	SendCanConsecutiveFrame(1, req3);
	CheckCanSingleFrame(1000);
	if (TScanResp.FData[1] != 0x7f || TScanResp.FData[3] != 0x13)
	{
		TestReport_HTML_StepDescription("收到了非预期响应！", Fail);
	}

	CheckCurrentSessionTP( 0x02);
	TestPostCondition_TP_CAN();

	if (resultFlag != 0)
		return 1;
	else
		return 0;
}
			   
s32 test_case_1021990()
{
	strcpy(TestCaseName, "TC_1021990_Valid Separation Time Parameter Test physical addressing - Programming session");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - SBL");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN();
	EnterProgrammingSesson();


	u8 SFdata1[2] = { 0xF1,0x86 };
	u8 SFdata2[3] = { 0x22,0xED,0x20 };
	s64 t1=0, t2=0, t3=0;
	u16 STminTime;

	//检查当前会话
	CheckCurrentSessionTP( 0x02,1);

	TestReport_HTML_StepDescription("下载SBL");
	ReportDescriptionFlag = 0;
	uds_create();
	DownloadSBL();
	uds_delete();
	ReportDescriptionFlag = 1;


	for (int i = 0; i < 7; i++)
	{
		switch (i)
		{
		case 0:
			STminTime = 0x19;
			break;
		case 1:
			STminTime = 0x32;
			break;
		case 2:
			STminTime = 0x64;
			break;
		case 3:
			STminTime = 0x7F;
			break;
		case 4:
			STminTime = 0xF1;
			break;
		case 5:
			STminTime = 0xF5;
			break;
		case 6:
			STminTime = 0xF9;
			break;
		default:
			break;
		}
		//请求多帧响应
		char sinfo[200] = "";
		sprintf(sinfo, "CASE%d 当前STMin=0x%x", i, STminTime);
		TestReport_HTML_StepDescription(sinfo);
		SendCanSingleFrame(5, SFdata2);
		app.log_text("Step: Request Multi Response !", lvlInfo);

		if (0 == CheckCanFirstFrame(Diag_TP_N_As))
		{
			app.log_text("Step: Send Flow Control Frame !", lvlInfo);
			SendCanFlowControlFrame(0, 3, STminTime);
			//等待第一帧连续帧
			s32 ret = WaitMessage(Diag_TP_N_Cs);
			if (0 != ret)
			{
				app.log_text("ECU does not send first CF within N_Cs !", lvlError);
			}
			else
			{
				t1 = TScanResp.FTimeUs;
			}

			//等待第二帧连续帧
			if (0 == WaitMessage( Diag_TP_N_Cr))
			{
				t2 = TScanResp.FTimeUs;
			}
			else
			{
				app.log_text("ECU does not send CF within N_Cr !", lvlError);
			}

			//等待第三帧连续帧
			if (0 == WaitMessage(Diag_TP_N_Cr))
			{
				t3 = TScanResp.FTimeUs;
			}
			else
			{
				app.log_text("ECU does not 1111 send CF within N_Cr !", lvlError);
			}
			//检查两个连续帧之间的时间间隔是否大于STmin
			if (((t2 - t1) / 1000) > STminTime && ((t3 - t2)) / 1000 > STminTime)
			{

			}
			else
			{
				strcpy(issuereason, "ECU发送的连续帧间隔与STmin不符");
				strcpy(requestInfo, "Tester请求多帧响应");
				strcpy(expectedResponse, "DUT回复的连续帧之间的时间间隔大于STMin");
				strcpy(actualResponse, "DUT回复的连续帧之间的时间间隔小于STMin");
				TestReport_HTML_StepDescription("Time between two consecutive frame is less than STmin !", Fail);
			}
			//检查两个连续帧之间的时间间隔是否小于N_As+N_Cs
			if (((t2 - t1) / 1000) < (Diag_TP_N_As + Diag_TP_N_Cs) && ((t3 - t2)) / 1000 < (Diag_TP_N_As + Diag_TP_N_Cs))
			{

			}
			else
			{
				strcpy(issuereason, "ECU发送的连续帧间隔与STmin不符");
				strcpy(requestInfo, "Tester请求多帧响应");
				strcpy(expectedResponse, "DUT回复的连续帧之间的时间间隔大于STMin");
				strcpy(actualResponse, "DUT回复的连续帧之间的时间间隔小于STMin");
				TestReport_HTML_StepDescription("Time between two consecutive frame is less than STmin !", Fail);
			}
			TestWait(stepseptime);
		}
		else
		{
			app.log_text("Test condition not fulfilled, test aborted!", lvlError);
			TestPostCondition_TP_CAN();
			return 0;
		}
	}
	TestWait(stepseptime);
	//检查当前会话
	CheckCurrentSessionTP( 0x02,1); 
	
	TestPostCondition_TP_CAN();

	if (resultFlag != 0)
		return 1;
	else
		return 0;
}
			   
s32 test_case_1021991()
{
	strcpy(TestCaseName, "TC_1021991_Wrong Sequence Number Test physical addressing - Programming session");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - SBL");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN();
	EnterProgrammingSesson();

	//Wrong Sequence Number Test physical addressing - Default session
	u8 SFdata[2] = { 0xF1,0x86 };
	u8 FFdata[6] = { 0x10,0x01,0x12,0x13,0x14,0x15 };
	u8 CFdata1[7] = { 0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C };
	u8 CFdata2[7] = { 0x1D,0x1E,0x1F,0x00,0x00,0x00,0x00 };
	//检查当前会话
	CheckCurrentSessionTP( 0x02);

	TestReport_HTML_StepDescription("下载SBL");
	ReportDescriptionFlag = 0;
	uds_create();
	DownloadSBL();
	uds_delete();
	ReportDescriptionFlag = 1;


	//发送首帧
	SendCanFirstFrame(0x10, FFdata);
	app.log_text("Step: Send First Frame !", lvlInfo);
	//检查流控帧
	CheckCanFlowControlMessage(Diag_TP_N_Bs);
	//发送连续帧
	TestReport_HTML_StepDescription("发送序列号错误的首帧请求");
	app.log_text("Step: Send Consecutive Frame with Incorrect Sequence Number!", lvlInfo);
	app.wait(Diag_TP_FC_STmin + 1, "");
	SendCanConsecutiveFrame(0x02, CFdata2);
	//发送第二帧连续帧
	TestReport_HTML_StepDescription("发送序列号正确的首帧请求");
	app.log_text("Step: Send Consecutive Frame with Correct Sequence Number!", lvlInfo);
	app.wait(Diag_TP_FC_STmin + 1, "");
	SendCanConsecutiveFrame(0x01, CFdata1);
	//检查是否有响应
	if (0 == WaitMessage(Diag_UDS_P2Client))
	{
		strcpy(issuereason, "连续帧序列号错误，ECU仍然发送响应！");
		strcpy(requestInfo, "Tester请求多帧响应,发送序列号错误的第一帧连续帧");
		strcpy(expectedResponse, "DUT不回复响应");
		strcpy(actualResponse, "DUT收到序列号错误的连续帧后仍然回复响应");
		TestReport_HTML_StepDescription("连续帧序列号错误，ECU仍然发送响应！", Fail);
	}

	//发送首帧
	SendCanFirstFrame(0x10, FFdata);
	app.log_text("Step: Send First Frame !", lvlInfo);
	//检查流控帧
	CheckCanFlowControlMessage(Diag_TP_N_Bs);
	//发送连续帧
	app.log_text("Step: Send Consecutive Frame with Correct Sequence Number!", lvlInfo);
	app.wait(Diag_TP_FC_STmin + 1, "");
	SendCanConsecutiveFrame(0x01, CFdata1);
	//发送第二帧连续帧
	TestReport_HTML_StepDescription("发送序列号错误的连续帧");
	app.log_text("Step: Send Consecutive Frame with Inorrect Sequence Number!", lvlInfo);
	app.wait(Diag_TP_FC_STmin + 1, "");
	SendCanConsecutiveFrame(0x03, CFdata2);
	//发送第三帧连续帧
	TestReport_HTML_StepDescription("发送序列号错误的连续帧");
	app.log_text("Step: Send Consecutive Frame with Correct Sequence Number!", lvlInfo);
	app.wait(Diag_TP_FC_STmin + 1, "");
	SendCanConsecutiveFrame(0x02, CFdata2);
	//检查是否有响应
	if (0 == WaitMessage(Diag_UDS_P2Client))
	{
		strcpy(issuereason, "连续帧序列号错误，ECU仍然发送响应！");
		strcpy(requestInfo, "Tester请求多帧响应,发送序列号错误的第二帧连续帧");
		strcpy(expectedResponse, "DUT不回复响应");
		strcpy(actualResponse, "DUT收到序列号错误的连续帧后仍然回复响应");
		TestReport_HTML_StepDescription("连续帧序列号错误，ECU仍然发送响应！", Fail);
	}

	//发送首帧
	SendCanFirstFrame(0x10, FFdata);
	app.log_text("Step: Send First Frame !", lvlInfo);
	//检查流控帧
	CheckCanFlowControlMessage(Diag_TP_N_Bs);
	//发送连续帧
	TestReport_HTML_StepDescription("发送连续帧");
	app.log_text("Step: Send Consecutive Frame with Correct Sequence Number!", lvlInfo);
	app.wait(Diag_TP_FC_STmin + 1, "");
	SendCanConsecutiveFrame(0x01, CFdata1);
	//发送第二帧连续帧
	app.log_text("Step: Send Consecutive Frame with Correct Sequence Number!", lvlInfo);
	app.wait(Diag_TP_FC_STmin + 1, "");
	SendCanConsecutiveFrame(0x02, CFdata2);

	CheckCanSingleFrame(1000);
	if (TScanResp.FData[1] != 0x7f || TScanResp.FData[3] != 0x13)
	{
		TestReport_HTML_StepDescription("ECU回复了非预期请求", Fail);
	}

	CheckCurrentSessionTP( 0x02);
	TestPostCondition_TP_CAN();

	if (resultFlag != 0)
		return 1;
	else
		return 0;
}
			   
s32 test_case_1021992()
{
	strcpy(TestCaseName, "TC_1021992_N_As test physical addressing - Programming session");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - SBL");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN();
	EnterProgrammingSesson();

	u8 SFdata1[2] = { 0xF1,0x86 };
	u8 SFdata2[3] = { 0x22,0xED,0x20 };
	TCAN InteruptMsg1 = { Diag_ch,1,8,0,1,0,{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0} };
	TCAN InteruptMsg2 = { Diag_ch,1,8,0,1,0,{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0} };
	TCAN InteruptMsg3 = { Diag_ch,1,8,0,1,0,{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0} };
	//检查当前会话
	CheckCurrentSessionTP( 0x02);

	TestReport_HTML_StepDescription("下载SBL");
	ReportDescriptionFlag = 0;
	uds_create();
	DownloadSBL();
	uds_delete();
	ReportDescriptionFlag = 1;


	//请求多帧响应
	TestReport_HTML_StepDescription("请求多帧响应 并立即发送高优先级报文，等到N_As*0.9，取消发送高优先级报文");
	SendCanSingleFrame(3, SFdata2);
	app.log_text("Step: Request Multi Resposne !", lvlInfo);

	//发送高优先级报文使N_As超时
	com.add_cyclic_message_can(&InteruptMsg1, 1);
	com.add_cyclic_message_can(&InteruptMsg2, 1);
	com.add_cyclic_message_can(&InteruptMsg3, 1);
	//等待N_As未超时
	app.wait(Diag_TP_N_As * 0.9, "");
	//取消周期报文
	com.del_cyclic_messages();
	//检查首帧
	if (0 == CheckCanFirstFrame(Diag_TP_N_As))
	{
		TestReport_HTML_StepDescription("发送流控帧");
		SendCanFlowControlFrame(0x0, 0x0, 0x0);

		CheckCanConsecutiveFrame(Diag_TP_N_Cs);
	}
	else
	{
		TestReport_HTML_StepDescription("未收到ECU响应", Fail);
	}


	//请求多帧响应
	TestReport_HTML_StepDescription("请求多帧响应 并立即发送高优先级报文，等到N_As*1.1，取消发送高优先级报文");
	SendCanSingleFrame(3, SFdata2);
	app.log_text("Step: Request Multi Resposne !", lvlInfo);

	//发送高优先级报文使N_As超时
	com.add_cyclic_message_can(&InteruptMsg1, 1);
	com.add_cyclic_message_can(&InteruptMsg2, 1);
	com.add_cyclic_message_can(&InteruptMsg3, 1);
	//等待N_As未超时
	app.wait(Diag_TP_N_As * 1.1, "");
	//取消周期报文
	com.del_cyclic_messages();
	//检查首帧
	if (0 == CheckCanFirstFrame(Diag_TP_N_As))
	{
		TestReport_HTML_StepDescription("发送流控帧");
		SendCanFlowControlFrame(0x0, 0x0, 0x0);

		if (0 != CheckNoResp(1000))
		{
			strcpy(issuereason, "N_As超时后ECU仍然发送连续帧");
			strcpy(requestInfo, "请求多帧响应后立即发送高优先级报文干扰总线");
			strcpy(expectedResponse, "ECU回复首帧或不响应");
			strcpy(actualResponse, "ECU在N_As超时后仍然响应");
			TestReport_HTML_StepDescription("N_As超时后ECU仍然发送连续帧", Fail);
		}
	}
	else
	{
		TestReport_HTML_StepDescription("未收到ECU响应", Pass);
	}

	//检查当前会话
	CheckCurrentSessionTP( 0x02);
	
	TestPostCondition_TP_CAN();

	if (resultFlag != 0)
		return 1;
	else
		return 0;
}
			   
s32 test_case_1021993()
{
	strcpy(TestCaseName, "TC_1021993_N_Ar test physical addressing - Programming session");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - SBL");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN();
	EnterProgrammingSesson();

	u8 SFdata1[2] = { 0xF1,0x86 };
	u8 FFdata[6] = { 0x10,0x01,0x02,0x03,0x04,0x05 };
	u8 CFdata[7] = { 0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C };
	TCAN InteruptMsg1 = { Diag_ch,1,8,0,1,0,{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0} };
	TCAN InteruptMsg2 = { Diag_ch,1,8,0,1,0,{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0} };
	TCAN InteruptMsg3 = { Diag_ch,1,8,0,1,0,{0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0} };
	//检查当前会话
	CheckCurrentSessionTP( 0x02);

	TestReport_HTML_StepDescription("下载SBL");
	ReportDescriptionFlag = 0;
	uds_create();
	DownloadSBL();
	uds_delete();
	ReportDescriptionFlag = 1;


	//发送首帧
	TestReport_HTML_StepDescription("发送首帧，并立即发送高优先级报文，等到N_Ar*0.9，取消发送高优先级报文");
	SendCanFirstFrame(0x0D, FFdata);
	app.log_text("Step: Send First Frame !", lvlInfo);

	//发送高优先级报文
	com.add_cyclic_message_can(&InteruptMsg1, 1);
	com.add_cyclic_message_can(&InteruptMsg2, 1);
	com.add_cyclic_message_can(&InteruptMsg3, 1);
	//等待N_Ar未超时
	app.wait(Diag_TP_N_Ar * 0.9, "");
	//取消周期报文
	com.del_cyclic_messages();
	//检查流控帧
	CheckCanFlowControlMessage(Diag_TP_N_Ar);
	//发送连续帧
	SendCanConsecutiveFrame(0x01, CFdata);
	//期望否定响应
	if (0 == WaitMessage(Diag_UDS_P2Client))
	{
		if (TScanResp.FData[0] == 0x7F && TScanResp.FData[2] == 0x13)
		{
			app.log_text("ECU send negative response with NRC13 !", lvlOK);
		}
		else
		{
			TestReport_HTML_StepDescription("收到了非预期响应", Fail);
		}
	}
	else
	{
		TestReport_HTML_StepDescription("N_Ar未超时情况下未收到响应", Fail);
	}

	//发送多帧请求
	TestReport_HTML_StepDescription("发送首帧，并立即发送高优先级报文，等到N_Ar*1.1，取消发送高优先级报文");
	SendCanFirstFrame(0x0D, FFdata);
	app.log_text("Step: Send Multi Request !", lvlInfo);

	//发送高优先级报文使N_Ar超时
	com.add_cyclic_message_can(&InteruptMsg1, 1);
	com.add_cyclic_message_can(&InteruptMsg2, 1);
	com.add_cyclic_message_can(&InteruptMsg3, 1);
	//等待N_Ar超时
	app.wait(Diag_TP_N_Ar * 1.1, "");
	//取消周期报文
	com.del_cyclic_messages();
	//检查流控帧
	CheckCanFlowControlMessage(Diag_TP_N_Ar);
	//发送连续帧
	SendCanConsecutiveFrame(0x01, CFdata);
	if (0 != CheckCanConsecutiveFrame(Diag_TP_N_Cs))
	{
		app.log_text("ECU does not response when N_Ar timeout", lvlOK);
	}
	else
	{
		strcpy(issuereason, "N_Ar超时后ECU仍然发送响应");
		strcpy(requestInfo, "请求多帧传输后立即发送高优先级报文干扰总线");
		strcpy(expectedResponse, "ECU回复首帧或不响应");
		strcpy(actualResponse, "ECU在N_Ar超时后仍然响应");
		TestReport_HTML_StepDescription("N_Ar超时后ECU仍然发送响应", Fail);
	}

	//检查当前会话
	CheckCurrentSessionTP( 0x02);
	
	TestPostCondition_TP_CAN();

	if (resultFlag != 0)
		return 1;
	else
		return 0;
}
			   
s32 test_case_1021994() {
	
	strcpy(TestCaseName, "TC_1021994_Check_Timeout_Bs- Programming session");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - SBL");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN(); 
	EnterProgrammingSesson();

	u8 req1[2] = { 0xF1,0X86 };
	CheckCurrentSessionTP( 0x02);

	TestReport_HTML_StepDescription("下载SBL");
	ReportDescriptionFlag = 0;
	uds_create();
	DownloadSBL();
	uds_delete();
	ReportDescriptionFlag = 1;



	u8 req_date[] = { 0x10,0x01,0x01,0x03,0x04,0x05 };
	s64 T1 = 0, T2 = 0;


	SendCanFirstFrame(0x22, req_date, Physical);
	T1 = TScanReq.FTimeUs;

	if (CheckCanFlowControlMessage(Diag_TP_FC_BS) == 0) {
		T2 = TScanResp.FTimeUs;
	}
	else
	{
		TestReport_HTML_StepDescription("未收到流控帧", Fail);
	}
	if ((T2 - T1) / 1000 < Diag_TP_N_Bs) {
		app.log_text(("DUT sends FC in " + std::to_string((T2 - T1) / 1000)).c_str(), lvlOK);
	}
	else
	{
		strcpy(issuereason, "未在N_Bs时间内收到流控帧");
		strcpy(requestInfo, "Tester发送首帧");
		strcpy(expectedResponse, "ECU在N_Bs时间内收到流控帧");
		strcpy(actualResponse, "ECU在N_Bs时间内未收到流控帧");
		TestReport_HTML_StepDescription("未在N_Bs时间内收到流控帧", Fail);
	}

	CheckCurrentSessionTP( 0x02);

	TestPostCondition_TP_CAN();

	if (resultFlag != 0)
		return 1;
	else
		return 0;
}
			   
s32 test_case_1021995() {
	strcpy(TestCaseName, "TC_1021995_Check_Timeout_Cr- Programming session");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - SBL");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN(); 
	EnterProgrammingSesson();

	u8 req1[2] = { 0xF1,0X86 };
	CheckCurrentSessionTP( 0x02);

	TestReport_HTML_StepDescription("下载SBL");
	ReportDescriptionFlag = 0;
	uds_create();
	DownloadSBL();
	uds_delete();
	ReportDescriptionFlag = 1;


	u8 req_date[] = { 0x22,0xED,0x20 };
	s64 T1 = 0, T2 = 0;

	SendCanSingleFrame(3, req_date, Physical);
	CheckCanFirstFrame(1000);

	SendCanFlowControlFrame(0, 0, 0);
	T1 = TScanReq.FTimeUs;

	if (0 == CheckCanConsecutiveFrame(Diag_TP_N_Cr))
	{
		T2 = firstCFtime;
		if ((T2 - T1) / 1000 < Diag_TP_N_Cr) {
			app.log_text(("DUT sends FC in " + std::to_string((T2 - T1) / 1000)).c_str(), lvlOK);
		}
		else
		{
			strcpy(issuereason, "ECU未在N_Cr时间内发送连续帧");
			strcpy(requestInfo, "Tester请求多帧应答，ECU发送首帧后Tester发送连续帧");
			strcpy(expectedResponse, "ECU在N_Cr时间内发送连续帧");
			strcpy(actualResponse, "ECU在N_Cr时间内未发送连续帧");
			TestReport_HTML_StepDescription("未在N_Cr内收到连续帧！", Fail);
		}
	}
	else
	{
		TestReport_HTML_StepDescription("未收到连续帧！", Fail);
	}

	CheckCurrentSessionTP( 0x02);
	TestPostCondition_TP_CAN();

	if (resultFlag != 0)
		return 1;
	else
		return 0;
}
			   
s32 test_case_1021996() {
	strcpy(TestCaseName, "TC_1021996_Invalid _CAN_DLC_SF- Programming session");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - SBL");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN(); 
	EnterProgrammingSesson();

	u8 req1[2] = { 0xF1,0X86 };
	CheckCurrentSessionTP( 0x02);

	TestReport_HTML_StepDescription("下载SBL");
	ReportDescriptionFlag = 0;
	uds_create();
	DownloadSBL();
	uds_delete();
	ReportDescriptionFlag = 1;


	TCAN f0 = { Diag_ch,0x1,8,0,Diag_Request_ID,0,{0x03, 0x22, 0xF1, 0x86, 0x00, 0x00, 0x00, 0x00} };
	for (int i = 1; i < 8; i++) {
		f0.FDLC = i;

		char ss[200] = "";
		strcpy(ss, ("Send SF with incorrect DLC " + std::to_string(i)).c_str());
		TestReport_HTML_StepDescription(ss);

		com.transmit_can_async(&f0);
		app.log_text(("Send SF with incorrect DLC " + std::to_string(i)).c_str(), lvlInfo);
		if (0 == WaitMessage(Diag_UDS_P2Server)) {
			strcpy(issuereason, "Tester发送CAN_DL无效的单帧报文，ECU仍然响应");
			strcpy(requestInfo, "Tester发送CAN_DLC无效的单帧报文");
			strcpy(expectedResponse, "ECU忽略单帧，不回复响应");
			strcpy(actualResponse, "ECU对CAN_DLC无效的单帧回复响应");
			TestReport_HTML_StepDescription("CAN_DL无效仍然收到响应", Fail);
		}
		else {
			TestReport_HTML_StepDescription("CAN_DL无效未收到响应", Pass);
		};
		app.wait(1000, "");
	}


	CheckCurrentSessionTP( 0x02);

	TestPostCondition_TP_CAN();

	if (resultFlag != 0)
		return 1;
	else
		return 0;
}
			   
s32 test_case_1021997()
{
	strcpy(TestCaseName, "TC_1021997_Invalid _CAN_DLC_FF- Programming session");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - SBL");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN();
	EnterProgrammingSesson();

	TestReport_HTML_StepDescription("下载SBL");
	ReportDescriptionFlag = 0;
	uds_create();
	DownloadSBL();
	uds_delete();
	ReportDescriptionFlag = 1;


	TCAN FFFrame = { Diag_ch,0x1,8,0,Diag_Function_ID,0,{0x10, 0x22, 0x10, 0x01,0x02, 0x03, 0x04, 0x05} };
	for (int i = 1; i < 8; i++)
	{
		FFFrame.FDLC = i;
		char ss[200] = "";
		strcpy(ss, ("Send FF with incorrect DLC " + std::to_string(i)).c_str());
		TestReport_HTML_StepDescription(ss);

		com.transmit_can_async(&FFFrame);
		app.log_text(("Send FF with incorrect DLC " + std::to_string(i)).c_str(), lvlInfo);
		if (0 == WaitMessage(Diag_UDS_P2Server))
		{
			strcpy(issuereason, "Tester发送CAN_DL无效的首帧报文，ECU仍然响应");
			strcpy(requestInfo, "Tester发送CAN_DLC无效的首帧报文");
			strcpy(expectedResponse, "ECU忽略首帧，不回复响应");
			strcpy(actualResponse, "ECU对CAN_DLC无效的单帧回复响应");
			TestReport_HTML_StepDescription("CAN_DL无效仍然收到响应", Fail);
		}
		else {
			TestReport_HTML_StepDescription("CAN_DL无效未收到响应", Pass);
		};
	}
	
	TestPostCondition_TP_CAN();

	if (resultFlag != 0)
		return 1;
	else
		return 0;
}
			   
s32 test_case_1021998()
{
	strcpy(TestCaseName, "TC_1021998_Invalid _CAN_DLC_FC- Programming session");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - SBL");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN();
	EnterProgrammingSesson();

	TestReport_HTML_StepDescription("下载SBL");
	ReportDescriptionFlag = 0;
	uds_create();
	DownloadSBL();
	uds_delete();
	ReportDescriptionFlag = 1;


	TCAN SFFrame = { Diag_ch,0x1,8,0,Diag_Request_ID,0,{0x03, 0x22, 0xED, 0x20, 0x00, 0x00, 0x00, 0x00} };
	TCAN FCFrame = { Diag_ch,0x1,1,0,Diag_Request_ID,0,{0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} };

	for (int i = 1; i < 8; i++)
	{
		FCFrame.FDLC = i;
		com.transmit_can_async(&SFFrame);

		app.log_text(("Send SF/FC with incorrect DLC " + std::to_string(i)).c_str(), lvlInfo);
		if (0 == CheckCanSingleFrame(Diag_UDS_P2Server))
		{
			app.log_text("DUT responds", lvlOK);
		}
		else
		{
			TestReport_HTML_StepDescription("未收到首帧响应", Fail);
		};

		com.transmit_can_async(&FCFrame);

		if (0 == WaitMessage(Diag_UDS_P2Server))
		{
			strcpy(issuereason, "Tester发送CAN_DL无效的流控帧报文，ECU仍然响应");
			strcpy(requestInfo, "Tester请求多帧应答，发送无效流控帧");
			strcpy(expectedResponse, "ECU忽略流控帧，不回复响应");
			strcpy(actualResponse, "ECU对CAN_DLC无效的流控帧回复响应");
			TestReport_HTML_StepDescription("发送无效流控帧 仍然收到响应", Fail);
		}
		else
		{
			app.log_text("DUT does not respond", lvlOK);
		};
	}
	TestPostCondition_TP_CAN();

	if (resultFlag != 0)
		return 1;
	else
		return 0;
}
			   
s32 test_case_1021999()
{
	strcpy(TestCaseName, "TC_1021999_Invalid _CAN_DLC_CF- Programming session");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - SBL");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN();
	EnterProgrammingSesson();

	TestReport_HTML_StepDescription("下载SBL");
	ReportDescriptionFlag = 0;
	uds_create();
	DownloadSBL();
	uds_delete();
	ReportDescriptionFlag = 1;


	TCAN FFFrame = { Diag_ch,0x1,8,0,Diag_Request_ID,0,{0x10, 0x1C, 0x10, 0x01, 0x02, 0x03, 0x04, 0x05} };
	TCAN CFFrame1 = { Diag_ch,0x1,8,0,Diag_Request_ID,0,{0x21, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07} };
	TCAN CFFrame2 = { Diag_ch,0x1,8,0,Diag_Request_ID,0,{0x22, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07} };
	TCAN CFFrame3 = { Diag_ch,0x1,8,0,Diag_Request_ID,0,{0x23, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07} };
	TCAN CFFrame4 = { Diag_ch,0x1,1,0,Diag_Request_ID,0,{0x24, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} };


	for (int i = 1; i <= 8; i++)
	{
		CFFrame4.FDLC = i;
		com.transmit_can_async(&FFFrame);

		app.log_text(("Send FF/CF with incorrect DLC " + std::to_string(i)).c_str(), lvlInfo);
		if (0 == CheckCanFlowControlMessage(Diag_UDS_P2Server))
		{
			TestReport_HTML_StepDescription("发送CAN_DL错误的连续帧");
		}
		else
		{
			app.log_text("DUT does not respond", lvlError);
		};

		com.transmit_can_async(&CFFrame1);
		com.transmit_can_async(&CFFrame2);
		com.transmit_can_async(&CFFrame3);
		com.transmit_can_async(&CFFrame4);

		if (i == 8)
		{
			CheckCanSingleFrame(1000);
			if (TScanResp.FData[1] != 0x7f || TScanResp.FData[3] != 0x13)
			{
				TestReport_HTML_StepDescription("ECU回复了非预期响应", Fail);
			}
		}
		else
		{
			if (0 != CheckNoResp(Diag_UDS_P2Server))
			{
				strcpy(issuereason, "Tester发送CAN_DL无效的连续帧报文，ECU仍然响应");
				strcpy(requestInfo, "Tester请求多帧传输，发送CAN_DLC无效的连续帧");
				strcpy(expectedResponse, "ECU忽略连续帧，不回复响应");
				strcpy(actualResponse, "ECU对CAN_DLC无效的连续帧帧回复响应");
				TestReport_HTML_StepDescription("连续帧CAN_DL无效情况下ECU仍然发送响应", Fail);
			}
		}
	}
	TestPostCondition_TP_CAN();

	if (resultFlag != 0)
		return 1;
	else
		return 0;
}
			   
s32 test_case_1022000()
{
	strcpy(TestCaseName, "TC_1022000_Separation _time_SF_ProgrammingSession Physical Addressing");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - SBL");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_TP_CAN();
	EnterProgrammingSesson();

	TestReport_HTML_StepDescription("下载SBL");
	ReportDescriptionFlag = 0;
	uds_create();
	DownloadSBL();
	uds_delete();
	ReportDescriptionFlag = 1;

	u8 req1[3] = { 0x22,0xF1,0X86 };
	u8 req2[3] = { 0x10,01 };
	
	SendCanSingleFrame(0x03, req1);
	SendCanSingleFrame(0x03, req2);
	if (0 != CheckCanSingleFrame(1000))
	{
		TestReport_HTML_StepDescription("ECU未回复响应");
	}
	if (TScanResp.FData[2] != 0xf1 || TScanResp.FData[3] != 0x86 || TScanResp.FData[4] != 0x02)
	{
		TestReport_HTML_StepDescription("ECU回复非预期报文");
	}
	if (0 != CheckCanSingleFrame(1000))
	{
		TestReport_HTML_StepDescription("ECU未回复响应");
	}
	if (TScanResp.FData[2] != 0xf1 || TScanResp.FData[3] != 0x86 || TScanResp.FData[4] != 0x02)
	{
		TestReport_HTML_StepDescription("ECU回复非预期报文");
	}

	TestPostCondition_TP_CAN();

	if (resultFlag != 0)
		return 1;
	else
		return 0;
}
			   
s32 test_case_1325129()
{
	strcpy(TestCaseName, "TC_1325129_Check_FC- Programming session");
	strcpy(TestScopeName, "Diagnostic Communication on CAN - SBL");
	strcpy(testPurpose_CH, "测试目的：");
	//下载所有等级的vbf，验证ECU回复的流控帧为30 00 00
	u8 data[100] = { 0x11,0x2F };

	//进入编程会话
	EnterProgrammingSesson();

	TestReport_HTML_StepDescription("下载SBL");
	ReportDescriptionFlag = 0;
	uds_create();
	DownloadSBL();
	uds_delete();
	ReportDescriptionFlag = 1;

	SendCanFirstFrame(0x12F, data);

	CheckCanFlowControlMessage(Diag_TP_N_Ar);

	TestPostCondition_TP_CANFD();

	if (resultFlag != 0)
		return 1;
	else
		return 0;
}













#endif // !TestCase_CAN_Communication
