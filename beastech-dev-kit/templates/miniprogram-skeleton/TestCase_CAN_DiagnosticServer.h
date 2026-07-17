#ifndef TestCase_CAN_DiagnosticServer
#define TestCase_CAN_DiagnosticServer

#include "test_case_diag_api.h"

/////////////////////////////////////////////////////////////APP////////////////////////////////////////////////////////////
s32 test_case_1350868()
{
    strcpy(TestCaseName, "TC_1350868_Service_0x10_Valid_physical_all_sessions_supported_service");
    strcpy(TestScopeName, "Diagnostic Service APP");
    strcpy(testPurpose_CH, "测试目的：");

    TestPrecondition();

    SID10_SessionControl(0x01);
    CheckPosResponse_SID10(0x01);
    CheckCurrentSession(0x01);


    SID10_SessionControl(0x81);
    CheckNonResponse();
    CheckCurrentSession(0x01);

    SID10_SessionControl(0x02);
    CheckPosResponse_SID10(0x02);
    TestWait(Session02_Time);
    CheckCurrentSession(0x02);

    SID10_SessionControl(0x01);
    CheckPosResponse_SID10(0x01);
    TestWait(ECUResetTime);
    CheckCurrentSession(0x01);

    SID10_SessionControl(0x82);
    CheckNonResponse();
    TestWait(Session02_Time);
    CheckCurrentSession(0x02);

    SID10_SessionControl(0x81);
    CheckNonResponse();
    TestWait(Session02_Time);
    CheckCurrentSession(0x01);

    SID10_SessionControl(0x03);
    CheckPosResponse_SID10(0x03);
    TestWait(Session02_Time);
    CheckCurrentSession(0x03);

    SID10_SessionControl(0x81);
    CheckNonResponse();
    TestWait(Session02_Time);
    CheckCurrentSession(0x01);

    SID10_SessionControl(0x83);
    CheckNonResponse();
    TestWait(Session02_Time);
    CheckCurrentSession(0x03);

    SID10_SessionControl(0x03);
    CheckPosResponse_SID10(0x03);
    TestWait(Session02_Time);
    CheckCurrentSession(0x03);

    SID10_SessionControl(0x83);
    CheckNonResponse();
    TestWait(Session02_Time);
    CheckCurrentSession(0x03);

    SID10_SessionControl(0x02);
    CheckPosResponse_SID10(0x02);
    TestWait(Session02_Time);
    CheckCurrentSession(0x02);

    SID10_SessionControl(0x02);
    CheckPosResponse_SID10(0x02);
    TestWait(Session02_Time);
    CheckCurrentSession(0x02);

    SID10_SessionControl(0x82);
    CheckNonResponse();
    TestWait(Session02_Time);
    CheckCurrentSession(0x02);

    SID10_SessionControl(0x03);
    u8 nrc[2] = { 0x12,0x7E };
    CheckNegResponse(nrc);
    CheckCurrentSession(0x02);

    SID10_SessionControl(0x83);
    CheckNegResponse(nrc);
    TestWait(Session02_Time);
    CheckCurrentSession(0x02);

    SID10_SessionControl(0x01);
    CheckPosResponse_SID10(0x01);
    TestWait(Session02_Time);
    CheckCurrentSession(0x01);

    SID10_SessionControl(0x03);
    CheckPosResponse_SID10(0x03);
    TestWait(Session02_Time);
    CheckCurrentSession(0x03);

    SID10_SessionControl(0x82);
    CheckNonResponse();
    TestWait(Session02_Time);
    CheckCurrentSession(0x02);

    SID10_SessionControl(0x81);
    CheckNonResponse();
    TestWait(Session02_Time);
    CheckCurrentSession(0x01);

    SID10_SessionControl(0x01);
    CheckPosResponse_SID10(0x01);
    TestWait(Session02_Time);
    CheckCurrentSession(0x01);

    SID10_SessionControl(0x03);
    CheckPosResponse_SID10(0x03);
    CheckCurrentSession(0x03);

    SID10_SessionControl(0x01);
    CheckPosResponse_SID10(0x01);
    CheckCurrentSession(0x01);

    TestPostCondition();

    return Test_Result;

}
s32 test_case_1350870()
{
    strcpy(TestCaseName, "TC_1350870_Service (0x11) Valid physical defaultSession supported service");
    strcpy(TestScopeName, "Diagnostic Service APP");
    strcpy(testPurpose_CH, "测试目的：");

    TestPrecondition();

    SID11_ECUReset(0x01);
    CheckPosResponse_SID11(0x01);

    TestWait(ECUResetTime);

    CheckCurrentSession(0x01);

    SID11_ECUReset(0x81);
    CheckNonResponse();

    TestWait(ECUResetTime);

    CheckCurrentSession(0x01);

    TestPostCondition();

    return Test_Result;
}
s32 test_case_1022421()
{
    strcpy(TestCaseName, "TC_1022421_Service (0x14) Valid physical defaultSession supported service");
    strcpy(TestScopeName, "Diagnostic Service APP");
    strcpy(testPurpose_CH, "测试目的：");

    TestPrecondition();

    SID10_SessionControl(DefaultSession);
    CheckPosResponse_SID10(DefaultSession);

    //清除所有DTC
    SID14_ClearAllDTC();
    CheckPosResponse_SID14();

    TestWait(ECUResetTime);

    CheckCurrentSession(DefaultSession);

    TestPostCondition();

    return Test_Result;
}
s32 test_case_1350878() {

    strcpy(TestCaseName, "TC_1350878_Service (0x19) with sf (0x02) Valid physical defaultSession supported service");
    strcpy(TestScopeName, "Diagnostic Service APP");
    strcpy(testPurpose_CH, "测试目的：");

    u8 sub19[2] = { 0x02,0 };

    TestPrecondition();

    SID10_SessionControl(0x01, Physical);
    CheckPosResponse_SID10(0x01);
    CheckCurrentSession(0x01);

    //test_report.test_pass(HTMLHandle, "", "与DTCList不匹配的为：0x048511，0x048512，0x048513，0x0a1b11，0x0a1b12，0x0a1c11，0x0a1c12，0x130e09，0x130e96，0x161a86，0x454600，0x504d11，0x504d12，0x504d13，0x511a11，0x511a12，0x511a13，0x511b11，0x511b12，0x511b13，0x51a096，0x5a568f，0x5a588f，0x5a608f，0x5a628f，0x901c11，0x901c12，0x905b11，0x905b12，0x905b13，0x905c11，0x905c12，0x905c13，0x906111，0x906112，0x906113，0x906211，0x906212，0x906213，0x906371，0x906571，0x909a4a，0x90a211，0x90a215，0x90a228，0x90be96，0x90f311，0x90f312，0x90f313，0x90f511，0x90f512，0x90f513，0x911511，0x911512，0x912711，0x912715，0x912771，0x912811，0x912815，0x912871，0x912911，0x912915，0x912971，0x913611，0x913615，0x913671，0x913711，0x913715，0x913771，0x913811，0x913815，0x913871，0x913911，0x913915，0x913971，0x913c11，0x913c15，0x913e23，0x914711，0x914712，0x914713，0x915911，0x915915，0x915b11，0x915b12，0x915b13，0x919b13，0x91d011，0x91d012，0x91d013，0x91db87，0x91db96，0x924d77，0x924e41，0x924e55，0x92e31e，0x92e324，0x932324，0x946f11，0x946f12，0x947012，0x947014，0x947112，0x947114，0x947212，0x947412，0x947414，0x947512，0x947514，0x947612，0x947614，0x948324，0x948423，0x948624，0x948723，0x948a11，0x948a12，0x948b12，0x948b14，0x948f12，0x948f14，0x949511，0x949512，0x949612");

    for (size_t i = 0; i <= 0xff; i++) {
        TestReport_HTML_StepDescription(("当前测试的状态码为" + intToHexStr(i, 2) + "-----------------").c_str(), Info);
        sub19[1] = i;
        SID19_ReadDTC(sub19, 2);
        CheckPosResponse_SID19(sub19);
        TestWait(stepseptime);
    }

    CheckCurrentSession(DefaultSession);
    TestPostCondition();

    return Test_Result;

}
s32 test_case_1350898()
{
    u8 ExpectedNRC[3] = { 0x13, 0x31,0x33 };
    u8 DID[2] = {};
    u8 TempData[1000] = {};
    u8 StoreDID[1000] = {};
    u8 AfterDIDData[1000] = {};
    strcpy(TestCaseName, "TC_1350898_Service 0x22 and 0x2E Valid physical defaultSession supported service");
    strcpy(TestScopeName, "Diagnostic Service APP");
    strcpy(testPurpose_CH, "测试目的：");

    TestPrecondition();


    TestReport_HTML_StepDescription("Step: Enter default session ", Info);
    SID10_SessionControl(DefaultSession);
    CheckPosResponse_SID10(DefaultSession);

    TestReport_HTML_StepDescription("Step: Check current session is default session", Info);
    CheckCurrentSession(DefaultSession);

    for (int i = 0; i < readdidsize; i++)
    {
        if (ReadDIDList[i].Sup_Def == 1)
        {
            DID[0] = ReadDIDList[i].DID >> 8;
            DID[1] = ReadDIDList[i].DID & 0xFF;

            TestReport_HTML_StepDescription(("Step: Read DID " + dec2hex(DID[0]) + dec2hex(DID[1])).c_str(), Info);
            printf("[%d] DIDLength is %d,", i+1, ReadDIDList[i].DIDLength);
            SID22_ReadDataByIdentifier(DID);
            CheckPosResponse_SID22(DID);

            if (ReadDIDList[i].Writable == 1)
            {
                memcpy(TempData, TpRxData, TpRxLength);

                StoreDID[0] = DID[0];
                StoreDID[1] = DID[1];
                for (int k = 0; k < ReadDIDList[i].DIDLength; k++)
                {
                    StoreDID[k + 2] = TempData[k];
                    AfterDIDData[k] = StoreDID[k + 2];
                }
                TestReport_HTML_StepDescription(("Step: Write DID " + dec2hex(DID[0]) + dec2hex(DID[1])).c_str(), Info);
                SID2E_WriteDataById(StoreDID, ReadDIDList[i].DIDLength + 2);
                if (ReadDIDList[i].Writable == 1)
                    CheckPosResponse_SID2E(DID);
                else
                    CheckNegResponse(ExpectedNRC);
            }

        }

    }

    TestReport_HTML_StepDescription("Step: Check current session is default session", Info);
    CheckCurrentSession(DefaultSession);

    TestPostCondition();

    return Test_Result;


    //u8 ExpectedNRC[2] = { 0x31,0x33 };
    //u8 DID[2] = {};
    //u8 TempData[100] = {};
    //u8 StoreDID[100] = {};
    //u8 AfterDIDData[100] = {};
    //strcpy(TestCaseName, "TC_1350898_Service 0x22 and 0x2E Valid physical defaultSession supported service");
    //strcpy(TestScopeName, "Diagnostic Service APP");
    //strcpy(testPurpose_CH, "测试目的：");

    //TestPrecondition();


    //TestReport_HTML_StepDescription("Step: Enter default session ", Info);
    //SID10_SessionControl(DefaultSession);
    //CheckPosResponse_SID10(DefaultSession);

    //TestReport_HTML_StepDescription("Step: Check current session is default session", Info);
    //CheckCurrentSession(DefaultSession);

    //for (int i = 0; i < readdidsize; i++)
    //{
    //    int readawritenum = 0;
    //    if (ReadDIDList[i].Sup_Def == 1)
    //    {
    //        DID[0] = ReadDIDList[i].DID >> 8;
    //        DID[1] = ReadDIDList[i].DID & 0xFF;

    //        TestReport_HTML_StepDescription(("Step: Read DID " + dec2hex(DID[0]) + " " + dec2hex(DID[1])).c_str(), Info);
    //        SID22_ReadDataByIdentifier(DID);
    //        //CheckPosResponse_SID22(DID, ReadDIDList[i].DIDLength);
    //        CheckPosResponse_SID22(DID);

    //        int flaggg = 0;
    //        for (int j = 0; j < servicesize; j++)
    //        {
    //            if (SupportServices[j].service == 0x2E)
    //            {
    //                flaggg = j;
    //            }
    //            //else
    //            //    flaggg = 0;//有这个的话一直不能发2E
    //        }

    //        if (flaggg != 0)
    //        {
    //            memcpy(TempData, TpRxData, TpRxLength);

    //            StoreDID[0] = DID[0];
    //            StoreDID[1] = DID[1];
    //            for (int k = 0; k < ReadDIDList[i].DIDLength; k++)
    //            {
    //                StoreDID[k + 2] = TempData[k];
    //                AfterDIDData[k] = StoreDID[k + 2];
    //            }
    //            TestReport_HTML_StepDescription(("Step: Write DID " + dec2hex(DID[0]) + " " + dec2hex(DID[1])).c_str(), Info);
    //            if (readawritenum == 0)
    //                SID2E_WriteDataById(StoreDID, 9);  //mc
    //            else
    //                CheckNegResponse(ExpectedNRC);
    //            //SID2E_WriteDataById(StoreDID, ReadDIDList[i].DIDLength+2);
    //            /*if (ReadDIDList[i].Writable == 1)
    //                CheckPosResponse_SID2E(DID);
    //            else {
    //                printf("进入else中");
    //                CheckNegResponse(ExpectedNRC);
    //            }*/
    //            CheckNegResponse(ExpectedNRC);  //上述判断未能正常执行 ZCUD

    //            //TestReport_HTML_StepDescription(("Step: Read DID " + dec2hex(DID[0]) + " " + dec2hex(DID[1])).c_str(), Info);
    //            //SID22_ReadDataByIdentifier(DID);
    //            //CheckPosResponse_SID22(DID, ReadDIDList[i].DIDLength, AfterDIDData);
    //            if (readawritenum == 0)
    //                SID2E_WriteDataById(StoreDID, 9);  //mc
    //            else
    //                CheckNegResponse(ExpectedNRC);

    //            readawritenum++;
    //        }
    //    }
    //}

    //TestReport_HTML_StepDescription("Step: Check current session is default session", Info);
    //CheckCurrentSession(DefaultSession);

    //TestPostCondition();

    //if (resultFlag == 0)
    //    return 1;
    //else
    //    return 0;
    
}
s32 test_case_1088261()
{
    strcpy(TestCaseName, "TC_1088261_Service (0x28) Phy_PositiveResponse");
    strcpy(TestScopeName, "Diagnostic Service APP");
    strcpy(testPurpose_CH, "测试目的：");

    TestPrecondition();
    try {
        SID10_SessionControl(0x01);
        CheckPosResponse_SID10(0x01);

        SID10_SessionControl(0x03);
        CheckPosResponse_SID10(0x03);

        CheckCurrentSession(0x03);

        for (u8 CoT = 0x1; CoT < 4; CoT++)
        {
            for (u8 CT = 0; CT < 4; CT++)
            {
                SID28_CommunicationControl(CT, CoT);
                CheckPosResponse_SID28(CT);
            }
        }

        for (u8 CoT = 0x1; CoT < 4; CoT++)
        {
            for (u8 CT = 0x80; CT < 0x84; CT++)
            {
                SID28_CommunicationControl(CT, CoT);
                CheckNonResponse();
            }
        }

        //while (CoT < 0x04)
        //{
        //    do
        //    {
        //        SID28_CommunicationControl(CT, CoT);
        //        CheckPosResponse_SID28(CT);
        //        CT = CT + 1;
        //    } while (CT < 0x04);

        //    CoT = CoT + 1;
        //    CT = 0;
        //}

        //CT = 0x80, CoT = 0x01;

        //while (CoT < 0x04)
        //{
        //    do
        //    {
        //        SID28_CommunicationControl(CT, CoT);
        //        CheckNonResponse();
        //        CT = CT + 1;
        //    } while (CT < 0x84);

        //    CoT = CoT + 1;
        //    CT = 0x80;
        //}
    }
    catch (...)
    {
        log("error");
    }
    TestPostCondition();

    return Test_Result;

}
s32 test_case_1350943()
{
    strcpy(TestCaseName, "TC_1350943_Service (0x3E) Valid physical defaultSession supported service");
    strcpy(TestScopeName, "Diagnostic Service APP");
    strcpy(testPurpose_CH, "测试目的：");

    TestPrecondition();

    TestReport_HTML_StepDescription("Step: Enter default session", Info);
    SID10_SessionControl(DefaultSession);
    CheckPosResponse_SID10(DefaultSession);

    TestReport_HTML_StepDescription("Step: Check current session is default session", Info);
    CheckCurrentSession(DefaultSession);

    TestReport_HTML_StepDescription("Step: Request serivce 3E with subfunction 0x00", Info);
    SID3E_TesterPresent(0x00);
    CheckPosResponse_SID3E(0x00);

    TestReport_HTML_StepDescription("Step: Request service 3E ，SPRMIB = True", Info);
    SID3E_TesterPresent(0x80);
    CheckNonResponse();

    TestReport_HTML_StepDescription("Step: Check current session is default session", Info);
    CheckCurrentSession(DefaultSession);

    TestPostCondition();

    return Test_Result;
}
s32 test_case_1350947()
{
    strcpy(TestCaseName, "TC_1350947_Service (0x2F)  Valid physical extendedDiagnosticSession supported service");
    strcpy(TestScopeName, "Diagnostic Service APP");
    strcpy(testPurpose_CH, "测试目的：");

    TestPrecondition();
    int level = 0;
    int levelx = 0;
    TestReport_HTML_StepDescription("Step: Enter Extended Session", Info);
    SID10_SessionControl(0x03);
    CheckPosResponse_SID10(0x03);

    TestReport_HTML_StepDescription("Step: Check Current Session", Info);
    TestWait(1000);
    CheckCurrentSession(0x03);

    u8 DID[3] = { 0 };
    u16 Z1Z2_count = 1;
    u8 req_date1[] = { 0x20,0x2f, 0x00 }; //实际数据字节待确认 mc   3 4 3 5
    u8 req_date2[] = { 0x20,0x2f, 0x03, 0x00 }; //实际数据字节待确认
    //u8 req_date3[] = { 0x40,0x28, 0x00 }; //实际数据字节待确认
    //u8 req_date4[] = { 0x40,0x28, 0x03, 0x00, 0x00 }; //实际数据字节待确认
    SID10_SessionControl(0x01, Physical);
    CheckPosResponse_SID10(0x01);
    TestWait(3000);
    CheckCurrentSession(0x01);
    u32 responsize = 0;
    CheckCurrentSession(0x01);
    SID10_SessionControl(0x03, Physical);
    CheckPosResponse_SID10(0x03);
    TestWait(3000);
    CheckCurrentSession(0x03);

    TestReport_HTML_StepDescription("Step: Request seed", Info);
    SID27_SecurityAccess_RequestSeed(5);
    CheckPosResponse_SID27_RequestSeed(5);
    if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
    {
        TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
    }
    GenSAKeyExOpt(5);
    TestReport_HTML_StepDescription("Step: Send valid key", Info);
    SID27_SecurityAccess_SendKey(5);
    CheckPosResponse_SID27_SendKey(5);

    // 遍历所有2F服务支持的DID
    for (int i = 0; i < iodidsize; i++)
    {
        u8 current_did_high = (IODIDList[i].DID >> 8) & 0xFF;
        u8 current_did_low = IODIDList[i].DID & 0xFF;

        // 构造一个足够大的 buffer，UDS通常 2F DID(2) Param(1) Data(n)
        u8 requestBuffer[50] = { 0 };
        requestBuffer[0] = current_did_high;
        requestBuffer[1] = current_did_low;

        // --- 测试 Return Control to ECU (0x00) ---
        // 检查掩码 Bit0
        if ((IODIDList[i].SupportedIOParams & 0x01))
        {
            TestInfoPrint("Step: 2F DID 0x%X Param 0x00 (Return Control)", IODIDList[i].DID);
            requestBuffer[2] = 0x00;
            // 0x00 通常不需要额外数据，长度为 3 (DID_H + DID_L + Param)
            SID2F_IO_Control_byid(requestBuffer, 3);
            CheckPosResponse_SID2F(requestBuffer, 3);
            TestWait(50);
        }

        // --- 测试 Reset to Default (0x01) ---
        // 检查掩码 Bit1
        if ((IODIDList[i].SupportedIOParams & 0x02))
        {
            TestInfoPrint("Step: 2F DID 0x%X Param 0x01 (Reset to Default)", IODIDList[i].DID);
            requestBuffer[2] = 0x01;
            SID2F_IO_Control_byid(requestBuffer, 3);
            CheckPosResponse_SID2F(requestBuffer, 3);
            TestWait(50);
        }

        // --- 测试 Freeze Current State (0x02) ---
        // 检查掩码 Bit2
        if ((IODIDList[i].SupportedIOParams & 0x04))
        {
            TestInfoPrint("Step: 2F DID 0x%X Param 0x02 (Freeze)", IODIDList[i].DID);
            requestBuffer[2] = 0x02;
            SID2F_IO_Control_byid(requestBuffer, 3);
            CheckPosResponse_SID2F(requestBuffer, 3);
            TestWait(50);
        }

        // --- 测试 Short Term Adjustment (0x03) ---
        // 检查掩码 Bit3
        if ((IODIDList[i].SupportedIOParams & 0x08))
        {
            u8 dataSize = IODIDList[i].STA_DataSize;
            TestInfoPrint("Step: 2F DID 0x%X Param 0x03 (STA) Size= %d", IODIDList[i].DID, dataSize);

            requestBuffer[2] = 0x03;

            // 填充控制数据，通常填 0xFF (全开/最大) 或 0x01 等，视具体测试需求
            // 这里简单地全部填充 0xFF
            for (int k = 0; k < dataSize; k++) {
                requestBuffer[3 + k] = 0x00;
            }

            // 发送长度 = DID(2) + Param(1) + Data(dataSize)
            int totalLength = 2 + 1 + dataSize;

            SID2F_IO_Control_byid(requestBuffer, totalLength);

            // 校验时注意：正响应可能回显控制数据，也可能只回显到 Param，
            // 具体要看 CheckPosResponse_SID2F 内部实现。
            // 如果它只比较前几个字节，传 requestBuffer 即可。
            CheckPosResponse_SID2F(requestBuffer, totalLength);
            TestWait(50);
        }
    }

    CheckCurrentSession(0x03);

    TestPostCondition();

    return Test_Result;
}

s32 test_case_1350949()
{
    strcpy(TestCaseName, "TC_1350949_Service (0x31) Valid physical defaultSession supported service");
    strcpy(TestScopeName, "Diagnostic Service APP");
    strcpy(testPurpose_CH, "测试目的：");

    TestPrecondition();
    u8 nrc1[3] = { 0x31,0x33,0x7E };
    SID10_SessionControl(0x01, Physical);
    CheckPosResponse_SID10(0x01);

    CheckCurrentSession(0x01);

    u16 temprid = 0x0206;
    SID31_RoutineControl(0x01, temprid);
    CheckPosResponse_SID31(0x01, temprid);

    TestWait(2000);
    SID11_ECUReset(0x01);
    CheckPosResponse_SID11(0x01);

    TestWait(3000);
    CheckCurrentSession(0x01);

    
    SID10_SessionControl(0x01, Physical);
    CheckPosResponse_SID10(0x01);

    CheckCurrentSession(0x01);

    for (int i = 0; i < ridsize_default; i++)
    {
        u8 nrc1[3] = { 0x31,0x33,0x7E };
        SID31_RoutineControl(0x01, RIDList_Phy_Default[i].RID);
        if (i == 0)
        {
            CheckPosResponse_SID31(0x01, RIDList_Phy_Default[i].RID);
        }
        else CheckNegResponse(nrc1);

        TestWait(2000);
    }

    TestWait(2000);
    SID11_ECUReset(0x01);
    CheckPosResponse_SID11(0x01);

    TestWait(3000);
    CheckCurrentSession(0x01);
    

    TestPostCondition();

    return Test_Result;
}
s32 test_case_1351011()
{

    strcpy(TestCaseName, "TC_1351011_Service 0x34 valid physical programmingSession supported service ");
    strcpy(TestScopeName, "Diagnostic Service APP");
    strcpy(testPurpose_CH, "测试目的：");

    TestPrecondition();

    SID10_SessionControl(0x02, Physical);
    CheckPosResponse_SID10(0x02);
    TestWait(ECUResetTime);

    CheckCurrentSession(0x02);

    u8 req[10] = { 0x00,0x44,0x11,0x11,0x11,0x11,0x22,0x22,0x22,0x22 };
    SID34_Request_Download(req, 10);
    CheckNegResponse(0x33);

    CheckCurrentSession(0x02);

    SID10_SessionControl(0x81);
    CheckNonResponse();
    TestWait(Session02_Time);

    CheckCurrentSession(0x01);

    SID10_SessionControl(0x82);
    CheckNonResponse();
    TestWait(Session02_Time);

    CheckCurrentSession(0x02);

    TestPostCondition();

    return Test_Result;
}

s32 test_case_1351013()
{

    strcpy(TestCaseName, "TC_1351013_Service 0x36 valid physical programmingSession supported service ");
    strcpy(TestScopeName, "Diagnostic Service APP");
    strcpy(testPurpose_CH, "测试目的：");

    TestPrecondition();

    SID10_SessionControl(0x02, Physical);
    CheckPosResponse_SID10(0x02);
    TestWait(ECUResetTime);

    CheckCurrentSession(0x02);

    u8 data[10] = { 0x01 };
    SID36_Transfer_Data(0x01, data, 1);
    CheckNegResponse(0x24);

    CheckCurrentSession(0x02);

    TestPostCondition();

    return Test_Result;
}

s32 test_case_1351015()
{

    strcpy(TestCaseName, "TC_1351015_Service 0x37 valid physical programmingSession supported service ");
    strcpy(TestScopeName, "Diagnostic Service APP");
    strcpy(testPurpose_CH, "测试目的：");

    TestPrecondition();

    SID10_SessionControl(0x02, Physical);
    CheckPosResponse_SID10(0x02);
    TestWait(ECUResetTime);

    CheckCurrentSession(0x02);

    SID37_RequestTransferExit();
    CheckNegResponse(0x24);

    CheckCurrentSession(0x02);

    TestPostCondition();

    return Test_Result;
}

s32 test_case_1350890() {
    strcpy(TestCaseName, "TC_1350890_Service 0x19 with sf 0x0A Valid physical defaultSession supported service");
    strcpy(TestScopeName, "Diagnostic Service");
    strcpy(testPurpose_CH, "测试目的：");

    TestPrecondition();
    app.set_system_var_int32("TestType", 1);
    RX_SID19_ReportSupportDTC(Physical);
    CheckPosResponse_SID19_0A();
    //int Ndtc = StorageDTC();
    CheckCurrentSession(DefaultSession, Physical);
    TestPostCondition();
    return Test_Result;
    //if (resultFlag == 0)
    //    return 1;
    //else
    //    return 0;
}

s32 test_case_1350891() {
    strcpy(TestCaseName, "TC_1350891_Service 0x19 with sf 0x0A Valid physical extendedDiagnosticSession supported service");
    strcpy(TestScopeName, "Diagnostic Service");
    strcpy(testPurpose_CH, "测试目的：");

    TestPrecondition();

    SID10_SessionControl(0x03, Physical);
    CheckPosResponse_SID10(0x03);
    CheckCurrentSession(0x03, Physical);
    RX_SID19_ReportSupportDTC(Physical);
    CheckPosResponse_SID19_0A();
    //int Ndtc = StorageDTC();
    CheckCurrentSession(0x03, Physical);
    TestPostCondition();
    return Test_Result;
    //if (resultFlag == 0)
    //    return 1;
    //else
    //    return 0;
}

s32 test_case_1350892() {
    strcpy(TestCaseName, "TC_1350892_Service 0x19 with sf 0x0A Valid functional defaultSession supported service");
    strcpy(TestScopeName, "Diagnostic Service");
    strcpy(testPurpose_CH, "测试目的：");

    TestPrecondition();

    RX_SID19_ReportSupportDTC(Functional);
    CheckPosResponse_SID19_0A();
    //int Ndtc = StorageDTC();
    TestWait(100);  //ZCUP目前会非正常转发
    CheckCurrentSession(DefaultSession, Physical);
    TestPostCondition();
    return Test_Result;
    //if (resultFlag == 0)
    //    return 1;
    //else
    //    return 0;
}

s32 test_case_1350893() {
    strcpy(TestCaseName, "TC_1350893_Service 0x19 with sf 0x0A Valid functional extendedDiagnosticSession supported service");
    strcpy(TestScopeName, "Diagnostic Service");
    strcpy(testPurpose_CH, "测试目的：");

    TestPrecondition();

    SID10_SessionControl(0x03, 1);
    CheckPosResponse_SID10(0x03);
    CheckCurrentSession(0x03, 1);
    RX_SID19_ReportSupportDTC(Functional);
    CheckPosResponse_SID19_0A();
    //int Ndtc = StorageDTC();
    CheckCurrentSession(0x03, 1);
    TestPostCondition();
    return Test_Result;
    //if (resultFlag == 0)
    //    return 1;
    //else
    //    return 0;
}
#endif // !TestCase_CAN_DiagnosticServer
