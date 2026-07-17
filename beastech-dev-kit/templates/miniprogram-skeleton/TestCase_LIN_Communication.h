#ifndef TestCase_LIN_Communication
#define TestCase_LIN_Communication

#include "test_case_diag_api.h"

/// <summary>
/// /////////////////////////////////////////////////LIN_APP//////////////////////////////////////////////////
/// </summary>
s32 test_case_1022527()
{
	strcpy(TestCaseName, "TC_1022527_LIN DiagServ defaultSession - Test correct sequence");
	strcpy(TestScopeName, "Diagnostic Server - default session");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_LIN();

	CheckCurrentSession(0x01);

	u8 req[20] = { 0x10,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,0x10,0x11 };

	diag_serve_sent(req, 17);

	CheckNegResponse(0x13);

	u8 did[2] = { 0xF1,0x2B };
	SID22_ReadDataByIdentifier(did);
	CheckPosResponse_SID22(did);

	TestPostCondition_LIN();

	if (resultFlag == 0)
		return 1;
	else
		return 0;
}

s32 test_case_1022528()
{
	strcpy(TestCaseName, "TC_1022528_LIN DiagServ defaultSession - Test device only responds on correct NAD");
	strcpy(TestScopeName, "Diagnostic Server - default session");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_LIN();

	u8 DID[2] = { 0xF1,0x86 };
	s32 temp_NAD = 0;

	for(u8 i=1;;++i)
	{
		if (i == 0)
			break;

		if (i == 0x7E)
			i = 0x80;

		if (rtlUIDiagnostics.tsdiag_lin_create(&udsHandle, Diag_ch, temp_NAD) == 0x00)
		{
			SID22_ReadDataByIdentifier(DID);
			if (temp_NAD == NAD)
			{
				CheckPosResponse_SID22(DID);
			}
			else
			{
				CheckNonResponse();
			}
		}
	}

	TestPostCondition_LIN();

	if (resultFlag == 0)
		return 1;
	else
		return 0;
}

s32 test_case_1022529()
{
	strcpy(TestCaseName, "TC_1022529_LIN DiagServ defaultSession - Consecutive Frame Timeout Test");
	strcpy(TestScopeName, "Diagnostic Server - default session");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_LIN();

	TSlinReq.FData[1] = 0x10;
	TSlinReq.FData[2] = 0x11;
	TSlinReq.FData[3] = 0x10;
	TSlinReq.FData[4] = 0x01;
	TSlinReq.FData[5] = 0x03;
	TSlinReq.FData[6] = 0x04;
	TSlinReq.FData[7] = 0x05;
	TP_SendMessage();

	TSlinReq.FData[1] = 0x21;
	TSlinReq.FData[2] = 0x06;
	TSlinReq.FData[3] = 0x07;
	TSlinReq.FData[4] = 0x08;
	TSlinReq.FData[5] = 0x09;
	TSlinReq.FData[6] = 0x0A;
	TSlinReq.FData[7] = 0x0B;
	TP_SendMessage();

	TSlinReq.FData[1] = 0x22;
	TSlinReq.FData[2] = 0x0C;
	TSlinReq.FData[3] = 0x0D;
	TSlinReq.FData[4] = 0x0E;
	TSlinReq.FData[5] = 0x0F;
	TSlinReq.FData[6] = 0x10;
	TSlinReq.FData[7] = 0x11;
	TP_SendMessage();

	CheckCanSingleFrame(1000);
//////////////////////////////////////
	TSlinReq.FData[1] = 0x10;
	TSlinReq.FData[2] = 0x11;
	TSlinReq.FData[3] = 0x10;
	TSlinReq.FData[4] = 0x01;
	TSlinReq.FData[5] = 0x03;
	TSlinReq.FData[6] = 0x04;
	TSlinReq.FData[7] = 0x05;
	TP_SendMessage();

	TestWait(Diag_TP_N_Cr);

	TSlinReq.FData[1] = 0x21;
	TSlinReq.FData[2] = 0x06;
	TSlinReq.FData[3] = 0x07;
	TSlinReq.FData[4] = 0x08;
	TSlinReq.FData[5] = 0x09;
	TSlinReq.FData[6] = 0x0A;
	TSlinReq.FData[7] = 0x0B;
	TP_SendMessage();

	TSlinReq.FData[1] = 0x22;
	TSlinReq.FData[2] = 0x0C;
	TSlinReq.FData[3] = 0x0D;
	TSlinReq.FData[4] = 0x0E;
	TSlinReq.FData[5] = 0x0F;
	TSlinReq.FData[6] = 0x10;
	TSlinReq.FData[7] = 0x11;
	TP_SendMessage();

	CheckNoResp(1000);
///////////////////////////////////////
	TSlinReq.FData[1] = 0x10;
	TSlinReq.FData[2] = 0x11;
	TSlinReq.FData[3] = 0x10;
	TSlinReq.FData[4] = 0x01;
	TSlinReq.FData[5] = 0x03;
	TSlinReq.FData[6] = 0x04;
	TSlinReq.FData[7] = 0x05;
	TP_SendMessage();

	TSlinReq.FData[1] = 0x21;
	TSlinReq.FData[2] = 0x06;
	TSlinReq.FData[3] = 0x07;
	TSlinReq.FData[4] = 0x08;
	TSlinReq.FData[5] = 0x09;
	TSlinReq.FData[6] = 0x0A;
	TSlinReq.FData[7] = 0x0B;
	TP_SendMessage();

	TSlinReq.FData[1] = 0x22;
	TSlinReq.FData[2] = 0x0C;
	TSlinReq.FData[3] = 0x0D;
	TSlinReq.FData[4] = 0x0E;
	TSlinReq.FData[5] = 0x0F;
	TSlinReq.FData[6] = 0x10;
	TSlinReq.FData[7] = 0x11;
	TP_SendMessage();

	CheckCanSingleFrame(1000);

	TestPostCondition_LIN();

	if (resultFlag == 0)
		return 1;
	else
		return 0;
}

s32 test_case_1022530()
{
	strcpy(TestCaseName, "TC_1022530_LIN DiagServ defaultSession - Handling of Unexpected Valid CF Frames During Idle Test");
	strcpy(TestScopeName, "Diagnostic Server - default session");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_LIN();

	TSlinReq.FData[1] = 0x21;
	TSlinReq.FData[2] = 0x06;
	TSlinReq.FData[3] = 0x07;
	TSlinReq.FData[4] = 0x08;
	TSlinReq.FData[5] = 0x09;
	TSlinReq.FData[6] = 0x0A;
	TSlinReq.FData[7] = 0x0B;
	TP_SendMessage();

	CheckNoResp(1000);


	TSlinReq.FData[1] = 0x10;
	TSlinReq.FData[2] = 0x11;
	TSlinReq.FData[3] = 0x10;
	TSlinReq.FData[4] = 0x01;
	TSlinReq.FData[5] = 0x03;
	TSlinReq.FData[6] = 0x04;
	TSlinReq.FData[7] = 0x05;
	TP_SendMessage();

	TSlinReq.FData[1] = 0x21;
	TSlinReq.FData[2] = 0x06;
	TSlinReq.FData[3] = 0x07;
	TSlinReq.FData[4] = 0x08;
	TSlinReq.FData[5] = 0x09;
	TSlinReq.FData[6] = 0x0A;
	TSlinReq.FData[7] = 0x0B;
	TP_SendMessage();

	TSlinReq.FData[1] = 0x22;
	TSlinReq.FData[2] = 0x0C;
	TSlinReq.FData[3] = 0x0D;
	TSlinReq.FData[4] = 0x0E;
	TSlinReq.FData[5] = 0x0F;
	TSlinReq.FData[6] = 0x10;
	TSlinReq.FData[7] = 0x11;
	TP_SendMessage();

	CheckCanSingleFrame(1000);

	TestPostCondition_LIN();

	if (resultFlag == 0)
		return 1;
	else
		return 0;
}

s32 test_case_1022531()
{
	strcpy(TestCaseName, "TC_1022531_LIN DiagServ defaultSession - Handling of Unexpected Frames During Segmented Receive Test");
	strcpy(TestScopeName, "Diagnostic Server - default session");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_LIN();

	TSlinReq.FData[1] = 0x10;
	TSlinReq.FData[2] = 0x11;
	TSlinReq.FData[3] = 0x10;
	TSlinReq.FData[4] = 0x01;
	TSlinReq.FData[5] = 0x03;
	TSlinReq.FData[6] = 0x04;
	TSlinReq.FData[7] = 0x05;
	TP_SendMessage();

	TSlinReq.FData[1] = 0x10;
	TSlinReq.FData[2] = 0x11;
	TSlinReq.FData[3] = 0x10;
	TSlinReq.FData[4] = 0x01;
	TSlinReq.FData[5] = 0x03;
	TSlinReq.FData[6] = 0x04;
	TSlinReq.FData[7] = 0x05;
	TP_SendMessage();

	TSlinReq.FData[1] = 0x21;
	TSlinReq.FData[2] = 0x06;
	TSlinReq.FData[3] = 0x07;
	TSlinReq.FData[4] = 0x08;
	TSlinReq.FData[5] = 0x09;
	TSlinReq.FData[6] = 0x0A;
	TSlinReq.FData[7] = 0x0B;
	TP_SendMessage();

	TSlinReq.FData[1] = 0x22;
	TSlinReq.FData[2] = 0x0C;
	TSlinReq.FData[3] = 0x0D;
	TSlinReq.FData[4] = 0x0E;
	TSlinReq.FData[5] = 0x0F;
	TSlinReq.FData[6] = 0x10;
	TSlinReq.FData[7] = 0x11;
	TP_SendMessage();

	CheckCanSingleFrame(1000);
///////////////////////

	TSlinReq.FData[1] = 0x10;
	TSlinReq.FData[2] = 0x11;
	TSlinReq.FData[3] = 0x10;
	TSlinReq.FData[4] = 0x01;
	TSlinReq.FData[5] = 0x03;
	TSlinReq.FData[6] = 0x04;
	TSlinReq.FData[7] = 0x05;
	TP_SendMessage();

	TSlinReq.FData[1] = 0x21;
	TSlinReq.FData[2] = 0x06;
	TSlinReq.FData[3] = 0x07;
	TSlinReq.FData[4] = 0x08;
	TSlinReq.FData[5] = 0x09;
	TSlinReq.FData[6] = 0x0A;
	TSlinReq.FData[7] = 0x0B;
	TP_SendMessage();

	TSlinReq.FData[1] = 0x10;
	TSlinReq.FData[2] = 0x11;
	TSlinReq.FData[3] = 0x10;
	TSlinReq.FData[4] = 0x01;
	TSlinReq.FData[5] = 0x03;
	TSlinReq.FData[6] = 0x04;
	TSlinReq.FData[7] = 0x05;
	TP_SendMessage();

	TSlinReq.FData[1] = 0x21;
	TSlinReq.FData[2] = 0x06;
	TSlinReq.FData[3] = 0x07;
	TSlinReq.FData[4] = 0x08;
	TSlinReq.FData[5] = 0x09;
	TSlinReq.FData[6] = 0x0A;
	TSlinReq.FData[7] = 0x0B;
	TP_SendMessage();

	TSlinReq.FData[1] = 0x22;
	TSlinReq.FData[2] = 0x0C;
	TSlinReq.FData[3] = 0x0D;
	TSlinReq.FData[4] = 0x0E;
	TSlinReq.FData[5] = 0x0F;
	TSlinReq.FData[6] = 0x10;
	TSlinReq.FData[7] = 0x11;
	TP_SendMessage();

	CheckCanFirstFrame(1000);
	///////////////////////

	TSlinReq.FData[1] = 0x10;
	TSlinReq.FData[2] = 0x11;
	TSlinReq.FData[3] = 0x10;
	TSlinReq.FData[4] = 0x01;
	TSlinReq.FData[5] = 0x03;
	TSlinReq.FData[6] = 0x04;
	TSlinReq.FData[7] = 0x05;
	TP_SendMessage();

	TSlinReq.FData[1] = 0x06;
	TSlinReq.FData[2] = 0x10;
	TSlinReq.FData[3] = 0x01;
	TSlinReq.FData[4] = 0x03;
	TSlinReq.FData[5] = 0x04;
	TSlinReq.FData[6] = 0x05;
	TSlinReq.FData[7] = 0x06;
	TP_SendMessage();

	CheckCanFirstFrame(1000);

	TSlinReq.FData[1] = 0x21;
	TSlinReq.FData[2] = 0x06;
	TSlinReq.FData[3] = 0x07;
	TSlinReq.FData[4] = 0x08;
	TSlinReq.FData[5] = 0x09;
	TSlinReq.FData[6] = 0x0A;
	TSlinReq.FData[7] = 0x0B;
	TP_SendMessage();

	TSlinReq.FData[1] = 0x22;
	TSlinReq.FData[2] = 0x0C;
	TSlinReq.FData[3] = 0x0D;
	TSlinReq.FData[4] = 0x0E;
	TSlinReq.FData[5] = 0x0F;
	TSlinReq.FData[6] = 0x10;
	TSlinReq.FData[7] = 0x11;
	TP_SendMessage();

	CheckNoResp(1000);
	/////////////
	TSlinReq.FData[1] = 0x10;
	TSlinReq.FData[2] = 0x11;
	TSlinReq.FData[3] = 0x10;
	TSlinReq.FData[4] = 0x01;
	TSlinReq.FData[5] = 0x03;
	TSlinReq.FData[6] = 0x04;
	TSlinReq.FData[7] = 0x05;
	TP_SendMessage();

	TSlinReq.FData[1] = 0x21;
	TSlinReq.FData[2] = 0x06;
	TSlinReq.FData[3] = 0x07;
	TSlinReq.FData[4] = 0x08;
	TSlinReq.FData[5] = 0x09;
	TSlinReq.FData[6] = 0x0A;
	TSlinReq.FData[7] = 0x0B;
	TP_SendMessage();

	TSlinReq.FData[1] = 0x06;
	TSlinReq.FData[2] = 0x10;
	TSlinReq.FData[3] = 0x01;
	TSlinReq.FData[4] = 0x03;
	TSlinReq.FData[5] = 0x04;
	TSlinReq.FData[6] = 0x05;
	TSlinReq.FData[7] = 0x06;
	TP_SendMessage();

	CheckCanFirstFrame(1000);

	/////////////////
	TSlinReq.FData[1] = 0x22;
	TSlinReq.FData[2] = 0x0C;
	TSlinReq.FData[3] = 0x0D;
	TSlinReq.FData[4] = 0x0E;
	TSlinReq.FData[5] = 0x0F;
	TSlinReq.FData[6] = 0x10;
	TSlinReq.FData[7] = 0x11;
	TP_SendMessage();

	CheckNoResp(1000);

	TestPostCondition_LIN();

	if (resultFlag == 0)
		return 1;
	else
		return 0;
}

s32 test_case_1022532()
{
	strcpy(TestCaseName, "TC_1022532_LIN DiagServ defaultSession - Handling of Unexpected Frames During Segmented Send Test");
	strcpy(TestScopeName, "Diagnostic Server - default session");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_LIN();

	TSlinReq.FData[1] = 0x03;
	TSlinReq.FData[2] = 0x22;
	TSlinReq.FData[3] = 0xF1;
	TSlinReq.FData[4] = 0x2A;
	TSlinReq.FData[5] = 0xFF;
	TSlinReq.FData[6] = 0xFF;
	TSlinReq.FData[7] = 0xFF;
	TP_SendMessage();

	TSlinReq.FData[1] = 0x03;
	TSlinReq.FData[2] = 0x22;
	TSlinReq.FData[3] = 0xF1;
	TSlinReq.FData[4] = 0x2B;
	TSlinReq.FData[5] = 0xFF;
	TSlinReq.FData[6] = 0xFF;
	TSlinReq.FData[7] = 0xFF;
	TP_SendMessage();

	CheckCanFirstFrame(1000);

	CheckCanConsecutiveFrame(1000);
	
	CheckNoResp(1000);

	TSlinReq.FData[1] = 0x03;
	TSlinReq.FData[2] = 0x22;
	TSlinReq.FData[3] = 0xF1;
	TSlinReq.FData[4] = 0x2A;
	TSlinReq.FData[5] = 0xFF;
	TSlinReq.FData[6] = 0xFF;
	TSlinReq.FData[7] = 0xFF;
	TP_SendMessage();

	CheckCanFirstFrame(1000);

	TSlinReq.FData[1] = 0x03;
	TSlinReq.FData[2] = 0x22;
	TSlinReq.FData[3] = 0xF1;
	TSlinReq.FData[4] = 0x2B;
	TSlinReq.FData[5] = 0xFF;
	TSlinReq.FData[6] = 0xFF;
	TSlinReq.FData[7] = 0xFF;
	TP_SendMessage();

	CheckCanFirstFrame(1000);

	CheckCanConsecutiveFrame(1000);

	CheckNoResp(1000);

	TSlinReq.FData[1] = 0x03;
	TSlinReq.FData[2] = 0x22;
	TSlinReq.FData[3] = 0xF1;
	TSlinReq.FData[4] = 0x2A;
	TSlinReq.FData[5] = 0xFF;
	TSlinReq.FData[6] = 0xFF;
	TSlinReq.FData[7] = 0xFF;
	TP_SendMessage();

	TSlinReq.FData[1] = 0x03;
	TSlinReq.FData[2] = 0x22;
	TSlinReq.FData[3] = 0xF1;
	TSlinReq.FData[4] = 0x86;
	TSlinReq.FData[5] = 0xFF;
	TSlinReq.FData[6] = 0xFF;
	TSlinReq.FData[7] = 0xFF;
	TP_SendMessage();

	CheckCanSingleFrame(1000);

	CheckNoResp(1000);

	TSlinReq.FData[1] = 0x03;
	TSlinReq.FData[2] = 0x22;
	TSlinReq.FData[3] = 0xF1;
	TSlinReq.FData[4] = 0x2A;
	TSlinReq.FData[5] = 0xFF;
	TSlinReq.FData[6] = 0xFF;
	TSlinReq.FData[7] = 0xFF;
	TP_SendMessage();

	CheckCanFirstFrame(1000);

	TSlinReq.FData[1] = 0x03;
	TSlinReq.FData[2] = 0x22;
	TSlinReq.FData[3] = 0xF1;
	TSlinReq.FData[4] = 0x86;
	TSlinReq.FData[5] = 0xFF;
	TSlinReq.FData[6] = 0xFF;
	TSlinReq.FData[7] = 0xFF;
	TP_SendMessage();

	CheckCanSingleFrame(1000);

	CheckNoResp(1000);

	TestPostCondition_LIN();

	if (resultFlag == 0)
		return 1;
	else
		return 0;
}

s32 test_case_1022533()
{
	strcpy(TestCaseName, "TC_1022533_LIN DiagServ defaultSession - Handling of frame with faulty checksum");
	strcpy(TestScopeName, "Diagnostic Server - default session");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_LIN();




	TestPostCondition_LIN();

	if (resultFlag == 0)
		return 1;
	else
		return 0;
}

s32 test_case_1022534()
{
	strcpy(TestCaseName, "TC_1022534_LIN DiagServ defaultSession - Functional addressed request shall not disturb message transmission");
	strcpy(TestScopeName, "Diagnostic Server - default session");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_LIN();

	TSlinReq.FData[1] = 0x03;
	TSlinReq.FData[2] = 0x22;
	TSlinReq.FData[3] = 0xF1;
	TSlinReq.FData[4] = 0x2A;
	TSlinReq.FData[5] = 0xFF;
	TSlinReq.FData[6] = 0xFF;
	TSlinReq.FData[7] = 0xFF;
	TP_SendMessage();


	TSlinReq.FData[0] = 0x7E;
	TSlinReq.FData[1] = 0x02;
	TSlinReq.FData[2] = 0x3E;
	TSlinReq.FData[3] = 0x80;
	TSlinReq.FData[4] = 0xFF;
	TSlinReq.FData[5] = 0xFF;
	TSlinReq.FData[6] = 0xFF;
	TSlinReq.FData[7] = 0xFF;
	TP_SendMessage();

	CheckCanFirstFrame(1000);

	TSlinReq.FData[0] = 0x7E;
	TSlinReq.FData[1] = 0x02;
	TSlinReq.FData[2] = 0x3E;
	TSlinReq.FData[3] = 0x80;
	TSlinReq.FData[4] = 0xFF;
	TSlinReq.FData[5] = 0xFF;
	TSlinReq.FData[6] = 0xFF;
	TSlinReq.FData[7] = 0xFF;
	TP_SendMessage();

	CheckCanConsecutiveFrame(1000);

	TestPostCondition_LIN();

	if (resultFlag == 0)
		return 1;
	else
		return 0;
}

s32 test_case_1022535()
{
	strcpy(TestCaseName, "TC_1022535_LIN DiagServ defaultSession - Functional addressed request shall not disturb message reception");
	strcpy(TestScopeName, "Diagnostic Server - default session");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_LIN();

	TSlinReq.FData[1] = 0x10;
	TSlinReq.FData[2] = 0x11;
	TSlinReq.FData[3] = 0x10;
	TSlinReq.FData[4] = 0x01;
	TSlinReq.FData[5] = 0x03;
	TSlinReq.FData[6] = 0x04;
	TSlinReq.FData[7] = 0x05;
	TP_SendMessage();

	TSlinReq.FData[0] = 0x7E;
	TSlinReq.FData[1] = 0x02;
	TSlinReq.FData[2] = 0x3E;
	TSlinReq.FData[3] = 0x80;
	TSlinReq.FData[4] = 0xFF;
	TSlinReq.FData[5] = 0xFF;
	TSlinReq.FData[6] = 0xFF;
	TSlinReq.FData[7] = 0xFF;
	TP_SendMessage();

	TSlinReq.FData[1] = 0x21;
	TSlinReq.FData[2] = 0x06;
	TSlinReq.FData[3] = 0x07;
	TSlinReq.FData[4] = 0x08;
	TSlinReq.FData[5] = 0x09;
	TSlinReq.FData[6] = 0x0A;
	TSlinReq.FData[7] = 0x0B;
	TP_SendMessage();

	TSlinReq.FData[0] = 0x7E;
	TSlinReq.FData[1] = 0x02;
	TSlinReq.FData[2] = 0x3E;
	TSlinReq.FData[3] = 0x80;
	TSlinReq.FData[4] = 0xFF;
	TSlinReq.FData[5] = 0xFF;
	TSlinReq.FData[6] = 0xFF;
	TSlinReq.FData[7] = 0xFF;
	TP_SendMessage();

	TSlinReq.FData[1] = 0x22;
	TSlinReq.FData[2] = 0x0C;
	TSlinReq.FData[3] = 0x0D;
	TSlinReq.FData[4] = 0x0E;
	TSlinReq.FData[5] = 0x0F;
	TSlinReq.FData[6] = 0x10;
	TSlinReq.FData[7] = 0x11;
	TP_SendMessage();

	CheckCanSingleFrame(1000);


	TestPostCondition_LIN();

	if (resultFlag == 0)
		return 1;
	else
		return 0;
}

s32 test_case_1022536()
{
	strcpy(TestCaseName, "TC_1022536_LIN DiagServ defaultSession - Wrong Sequence Number Test");
	strcpy(TestScopeName, "Diagnostic Server - default session");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_LIN();

	TSlinReq.FData[1] = 0x10;
	TSlinReq.FData[2] = 0x0B;
	TSlinReq.FData[3] = 0x10;
	TSlinReq.FData[4] = 0x01;
	TSlinReq.FData[5] = 0x03;
	TSlinReq.FData[6] = 0x04;
	TSlinReq.FData[7] = 0x05;
	TP_SendMessage();

	TSlinReq.FData[1] = 0x20;
	TSlinReq.FData[2] = 0x0B;
	TSlinReq.FData[3] = 0x0A;
	TSlinReq.FData[4] = 0x10;
	TSlinReq.FData[5] = 0x09;
	TSlinReq.FData[6] = 0x08;
	TSlinReq.FData[7] = 0x07;
	TP_SendMessage();

	TSlinReq.FData[1] = 0x21;
	TSlinReq.FData[2] = 0x06;
	TSlinReq.FData[3] = 0x07;
	TSlinReq.FData[4] = 0x08;
	TSlinReq.FData[5] = 0x09;
	TSlinReq.FData[6] = 0x0A;
	TSlinReq.FData[7] = 0x0B;
	TP_SendMessage();

	CheckNoResp(1000);

	TSlinReq.FData[1] = 0x22;
	TSlinReq.FData[2] = 0x0C;
	TSlinReq.FData[3] = 0x0D;
	TSlinReq.FData[4] = 0x0E;
	TSlinReq.FData[5] = 0x0F;
	TSlinReq.FData[6] = 0x10;
	TSlinReq.FData[7] = 0x11;
	TP_SendMessage();

	TSlinReq.FData[1] = 0x10;
	TSlinReq.FData[2] = 0x11;
	TSlinReq.FData[3] = 0x10;
	TSlinReq.FData[4] = 0x01;
	TSlinReq.FData[5] = 0x03;
	TSlinReq.FData[6] = 0x04;
	TSlinReq.FData[7] = 0x05;
	TP_SendMessage();



	TSlinReq.FData[1] = 0x21;
	TSlinReq.FData[2] = 0x06;
	TSlinReq.FData[3] = 0x07;
	TSlinReq.FData[4] = 0x08;
	TSlinReq.FData[5] = 0x09;
	TSlinReq.FData[6] = 0x0A;
	TSlinReq.FData[7] = 0x0B;
	TP_SendMessage();

	TSlinReq.FData[1] = 0x21;
	TSlinReq.FData[2] = 0x0B;
	TSlinReq.FData[3] = 0x0A;
	TSlinReq.FData[4] = 0x10;
	TSlinReq.FData[5] = 0x09;
	TSlinReq.FData[6] = 0x08;
	TSlinReq.FData[7] = 0x07;
	TP_SendMessage();


	TSlinReq.FData[1] = 0x22;
	TSlinReq.FData[2] = 0x0C;
	TSlinReq.FData[3] = 0x0D;
	TSlinReq.FData[4] = 0x0E;
	TSlinReq.FData[5] = 0x0F;
	TSlinReq.FData[6] = 0x10;
	TSlinReq.FData[7] = 0x11;
	TP_SendMessage();


	CheckNoResp(1000);

	TSlinReq.FData[1] = 0x10;
	TSlinReq.FData[2] = 0x0B;
	TSlinReq.FData[3] = 0x10;
	TSlinReq.FData[4] = 0x01;
	TSlinReq.FData[5] = 0x03;
	TSlinReq.FData[6] = 0x04;
	TSlinReq.FData[7] = 0x05;
	TP_SendMessage();


	TSlinReq.FData[1] = 0x21;
	TSlinReq.FData[2] = 0x06;
	TSlinReq.FData[3] = 0x07;
	TSlinReq.FData[4] = 0x08;
	TSlinReq.FData[5] = 0x09;
	TSlinReq.FData[6] = 0x0A;
	TSlinReq.FData[7] = 0x0B;
	TP_SendMessage();

	CheckCanSingleFrame(1000);

	TestPostCondition_LIN();

	if (resultFlag == 0)
		return 1;
	else
		return 0;
}

s32 test_case_1022537()
{
	strcpy(TestCaseName, "TC_1022537_LIN DiagServ defaultSession - Length of SF out of range Handling Test");
	strcpy(TestScopeName, "Diagnostic Server - default session");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_LIN();

	TSlinReq.FData[1] = 0x06;
	TSlinReq.FData[2] = 0x10;
	TSlinReq.FData[3] = 0x01;
	TSlinReq.FData[4] = 0x03;
	TSlinReq.FData[5] = 0x04;
	TSlinReq.FData[6] = 0x05;
	TSlinReq.FData[7] = 0x06;
	TP_SendMessage();

	CheckCanSingleFrame(1000);

	TSlinReq.FData[1] = 0x07;
	TSlinReq.FData[2] = 0x10;
	TSlinReq.FData[3] = 0x01;
	TSlinReq.FData[4] = 0x03;
	TSlinReq.FData[5] = 0x04;
	TSlinReq.FData[6] = 0x05;
	TSlinReq.FData[7] = 0x06;
	TP_SendMessage();

	CheckNoResp(1000);

	TSlinReq.FData[1] = 0x08;
	TSlinReq.FData[2] = 0x10;
	TSlinReq.FData[3] = 0x01;
	TSlinReq.FData[4] = 0x03;
	TSlinReq.FData[5] = 0x04;
	TSlinReq.FData[6] = 0x05;
	TSlinReq.FData[7] = 0x06;
	TP_SendMessage();

	CheckNoResp(1000);

	TestPostCondition_LIN();

	if (resultFlag == 0)
		return 1;
	else
		return 0;
}

s32 test_case_1022538()
{
	strcpy(TestCaseName, "TC_1022538_LIN DiagServ defaultSession - Length of FF out of range Handling Test");
	strcpy(TestScopeName, "Diagnostic Server - default session");
	strcpy(testPurpose_CH, "测试目的：");

	TestPrecondition_LIN();

	TSlinReq.FData[1] = 0x10;
	TSlinReq.FData[2] = 0x06;
	TSlinReq.FData[3] = 0x10;
	TSlinReq.FData[4] = 0x01;
	TSlinReq.FData[5] = 0x03;
	TSlinReq.FData[6] = 0x04;
	TSlinReq.FData[7] = 0x05;
	TP_SendMessage();
	CheckNoResp(1000);

	TSlinReq.FData[1] = 0x10;
	TSlinReq.FData[2] = 0x10;
	TSlinReq.FData[3] = 0x01;
	TSlinReq.FData[4] = 0x03;
	TSlinReq.FData[5] = 0x04;
	TSlinReq.FData[6] = 0x05;
	TSlinReq.FData[7] = 0x06;
	TP_SendMessage();

	TSlinReq.FData[1] = 0x21;
	TSlinReq.FData[2] = 0x07;
	TSlinReq.FData[3] = 0x08;
	TSlinReq.FData[4] = 0x09;
	TSlinReq.FData[5] = 0x0A;
	TSlinReq.FData[6] = 0x0B;
	TSlinReq.FData[7] = 0xFF;
	TP_SendMessage();

	CheckCanSingleFrame(1000);

	TSlinReq.FData[1] = 0x10;
	TSlinReq.FData[2] = 0x00;
	TSlinReq.FData[3] = 0x01;
	TSlinReq.FData[4] = 0x03;
	TSlinReq.FData[5] = 0x04;
	TSlinReq.FData[6] = 0x05;
	TSlinReq.FData[7] = 0x06;
	TP_SendMessage();
	CheckNoResp(1000);


	TestPostCondition_LIN();

	if (resultFlag == 0)
		return 1;
	else
		return 0;
}







#endif // !TestCase_LIN_Communication

