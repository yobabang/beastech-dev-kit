#ifndef TestCase_CAN_CS
#define TestCase_CAN_CS

#include "test_case_diag_api.h"

s32 CS_Test_001() {
    try {
        strcpy(TestCaseName, "CS_Test_001");
        strcpy(TestScopeName, "Secure Flash");
        strcpy(testPurpose_CH, "测试目的：");

        u8 ED20[2] = { 0xED,0x20 };
        u8 F1AA[2] = { 0xF1,0xAA };

        TestPrecondition();

        //step1
        TestStep("TestSeq_EnterProgrammingSession");
        TestSeq_EnterProgrammingSession();

        //step6
        stepnumber = 5;
        TestStep("Complete ECU Part/Serial Number(s)");
        SID22_ReadDataByIdentifier(ED20);
        CheckPosResponse_SID22(ED20);

        //step7
        TestStep("ECU Core Assembly Number");
        SID22_ReadDataByIdentifier(F1AA);
        CheckPosResponse_SID22(F1AA);

        //step18
        TestStep("Enter programming session unlock ECU and if SBL is supported, SBL should be downloaded and actived");
        SID10_SessionControl(ProgrammingSession);
        CheckPosResponse_SID10(ProgrammingSession);

        TestWait(5000);

        TestSeq_DownloadSBL();

        //step19
        TestStep("Execute software download sequence， Erase and Download ALL P1 data Note : when downloading P1, Flip the last byte of the first data bolck of the non - VBT bolck");
        TestReport_HTML_StepDescription("----Download APP----");

        char keyvalue[20] = "APP0Path_Production";

        for (int i = 0; i < APPFileNum; i++)
        {
            char StepDespriction[256];
            char filename[300] = {};
            vbfparser(keyvalue);
            filename[0] = '\0';
            if (app.get_system_var_string("Diagnostic0.下载文件_DataFile", 512, filename) != 0 || filename[0] == '\0') {
                log("Error: Failed to get filename for %s", keyvalue);
                snprintf(StepDespriction, sizeof(StepDespriction), "Download APP%d (no filename)", i);
            }
            else {
                size_t len = strlen(filename);
                if (len >= 9) {
                    filename[len - 9] = '\0';
                    snprintf(StepDespriction, sizeof(StepDespriction), "Download APP is %s", filename);
                }
                else {
                    log("Warning: Filename '%s' too short (len=%zu)", filename, len);
                    snprintf(StepDespriction, sizeof(StepDespriction), "Download APP%d (short filename)", i);
                }
            }
            TestReport_HTML_StepDescription(StepDespriction);
            EraseAddress();
            FileDownload_with_VBT_Hash(0, 1);

            TestStep("Check sw_signature");
            CheckSignature();

            keyvalue[3]++;
        }
        //step24
        TestStep("Check Complete And Compatible");
        CheckCompleteAndCompatible();

        //step26
        stepnumber = 25;
        TestStep("ECU Reset");
        SID11_ECUReset(0x81, Functional);

        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_002() {
    try {
        strcpy(TestCaseName, "CS_Test_002");
        strcpy(TestScopeName, "Secure Flash");
        strcpy(testPurpose_CH, "测试目的：");

        TestPrecondition();
        TestSeq_EnterProgrammingSession();

        u8 DID1[2] = { 0xED,0x20 };
        SID22_ReadDataByIdentifier(DID1);
        CheckPosResponse_SID22(DID1);

        u8 DID2[2] = { 0xF1,0xAA };
        SID22_ReadDataByIdentifier(DID2);
        CheckPosResponse_SID22(DID2);

        TestSeq_DownloadSBL(0);

        u8 DID3[2] = { 0xF1,0xA2 };
        //SID22_ReadDataByIdentifier(DID3);
        //CheckNegResponse(0x31);

        if (0 != ActivateSBL()) {
            com.del_cyclic_messages();
            TestPostCondition();
            return Test_Result;
        }

        SID22_ReadDataByIdentifier(DID1);
        CheckPosResponse_SID22(DID1);

        SID22_ReadDataByIdentifier(DID3);
        CheckPosResponse_SID22(DID3, 8);

        char keyvalue[20] = "APP0Path_Production";
        TestReport_HTML_StepDescription("----Download APP----");
        vbfparser(keyvalue);
        for (int i = 0; i < 1; i++)
        {
            char StepDespriction[256];
            char filename[300] = "";
            vbfparser(keyvalue);
            app.get_system_var_string("Diagnostic0.下载文件_DataFile", 512, &filename[0]);
            filename[strlen(filename) - 9] = '\0';
            snprintf(StepDespriction, sizeof(StepDespriction), "Download File is %s ", filename);
            TestReport_HTML_StepDescription(StepDespriction);
            EraseAddress();
            FileDownload();
            TestReport_HTML_StepDescription("----Check InCorrect Signature----");
            CheckSignatureInvalid();
            keyvalue[3]++;
        }

        SID11_ECUReset(0x81);
        CheckNonResponse();
        TestWait(ECUResetTime);

        CheckCurrentSession(0x01);

        TestPostCondition();

        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_003() {
    try {
        strcpy(TestCaseName, "CS_Test_003");
        strcpy(TestScopeName, "Secure Flash");
        strcpy(testPurpose_CH, "测试目的：");

        TestPrecondition();
        TestSeq_EnterProgrammingSession();

        u8 DID1[2] = { 0xED,0x20 };
        SID22_ReadDataByIdentifier(DID1);
        CheckPosResponse_SID22(DID1);

        u8 DID2[2] = { 0xF1,0xAA };
        SID22_ReadDataByIdentifier(DID2);
        CheckPosResponse_SID22(DID2);

        TestSeq_DownloadSBL(0);

        u8 DID3[2] = { 0xF1,0xA2 };
        //SID22_ReadDataByIdentifier(DID3);
        //CheckNegResponse(0x31);

        if (0 != ActivateSBL()) {
            com.del_cyclic_messages();
            TestPostCondition();
            return Test_Result;
        }

        SID22_ReadDataByIdentifier(DID1);
        CheckPosResponse_SID22(DID1);

        SID22_ReadDataByIdentifier(DID3);
        CheckPosResponse_SID22(DID3, 8);

        char keyvalue[20] = "APP0Path_Production";
        TestReport_HTML_StepDescription("----Download APP----");
        vbfparser(keyvalue);
        for (int i = 0; i < 1; i++)
        {
            char StepDespriction[256];
            char filename[300] = "";
            vbfparser(keyvalue);
            app.get_system_var_string("Diagnostic0.下载文件_DataFile", 512, &filename[0]);
            filename[strlen(filename) - 9] = '\0';
            snprintf(StepDespriction, sizeof(StepDespriction), "Download File is %s ", filename);
            TestReport_HTML_StepDescription(StepDespriction);
            EraseAddress();
            FileDownload();
            TestReport_HTML_StepDescription("----Check Null Signature----");
            CheckSignatureBlank();
            keyvalue[3]++;
        }

        SID11_ECUReset(0x81);
        CheckNonResponse();
        TestWait(ECUResetTime);

        CheckCurrentSession(0x01);

        TestPostCondition();

        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_004() {
    try {
        strcpy(TestCaseName, "CS_Test_004");
        strcpy(TestScopeName, "Secure Flash");
        strcpy(testPurpose_CH, "测试目的：");

        TestPrecondition();
        TestSeq_EnterProgrammingSession();

        u8 DID1[2] = { 0xED,0x20 };
        SID22_ReadDataByIdentifier(DID1);
        CheckPosResponse_SID22(DID1);

        u8 DID2[2] = { 0xF1,0xAA };
        SID22_ReadDataByIdentifier(DID2);
        CheckPosResponse_SID22(DID2);

        TestSeq_DownloadSBL(0);

        u8 DID3[2] = { 0xF1,0xA2 };
        //SID22_ReadDataByIdentifier(DID3);
        //CheckNegResponse(0x31);

        if (0 != ActivateSBL()) {
            com.del_cyclic_messages();
            TestPostCondition();
            return Test_Result;
        }

        u8 D01C[2] = { 0XD0,0X1C };

        SID22_ReadDataByIdentifier(D01C);
        CheckPosResponse_SID22(D01C);

        //if (0 == CheckNegResponse(0x22)) {
        //    TestReport_HTML_StepDescription("ECU未写入公钥！");
        //}
        //else if (TpRxData[2] = 0x62) {
        //    TestReport_HTML_StepDescription("ECU已写入公钥！");
        //}

        SID11_ECUReset(0x81);
        CheckNonResponse();
        TestWait(ECUResetTime);

        CheckCurrentSession(0x01);

        TestPostCondition();

        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_005() {
    try {
        strcpy(TestCaseName, "CS_Test_005");
        strcpy(TestScopeName, "Secure Flash");
        strcpy(testPurpose_CH, "测试目的：");

        TestPrecondition();
        TestStep("Enter Programming Session");
        SID10_SessionControl(ProgrammingSession);

        TestStep("Check Programming Session");
        CheckCurrentSession(ProgrammingSession);

        TestStep("Read Public Key");
        u8 D01C[2] = { 0XD0,0X1C };
        //如果文件包含公钥
        SID22_ReadDataByIdentifier(D01C);
        //如果未写入公钥
        if (0 == Raw_CheckNegResponse(0x22)) {
            TestReport_StepInfo(Pass);
        }
        else if (TpRxData[2] = 0x62) {
            TestReport_HTML_StepDescription("ECU已写入公钥！", Pass);
        }

        TestStep("Downlaod SBL");
        u8 DID1[2] = { 0xED,0x20 };
        SID22_ReadDataByIdentifier(DID1);
        CheckPosResponse_SID22(DID1);

        u8 DID2[2] = { 0xF1,0xAA };
        SID22_ReadDataByIdentifier(DID2);
        CheckPosResponse_SID22(DID2);

        TestSeq_DownloadSBL(0);

        u8 DID3[2] = { 0xF1,0xA2 };
        SID22_ReadDataByIdentifier(DID3);
        CheckNegResponse(0x31);

        ActivateSBL();
        SID22_ReadDataByIdentifier(DID1);
        CheckPosResponse_SID22(DID1);

        SID22_ReadDataByIdentifier(DID3);
        CheckPosResponse_SID22(DID3, 8);

        //SendTesterPresent(TP_OFF);

        TestWait(6000);

        //TestStep("Enter Programming Session");
        //SID10_SessionControl(ProgrammingSession);

        //TestStep("Check Programming Session");
        //CheckCurrentSession(ProgrammingSession);

        TestStep("Read Public Key");
        //如果文件包含公钥
        SID22_ReadDataByIdentifier(D01C);
        //如果未写入公钥
        if (0 == Raw_CheckNegResponse(0x22)) {
            TestReport_StepInfo(Pass);
        }
        else if (TpRxData[2] = 0x62) {
            TestReport_HTML_StepDescription("ECU已写入公钥！", Pass);
        }

        TestStep("Write Public Key");
        u8 D01C_Key[295] = { 0xD0,0x1C,0xD2,0x1E,0x37,0x2A,0x9B,0x64,0xF1,0x8E,0xC6,0x53,0x43,0xE0,0x2A,0x2D,0x48,0x06,
                         0x19,0x56,0x70,0x17,0xC8,0x85,0xD1,0x7B,0x5B,0xD4,0x8A,0x41,0xCB,0x53,0xB4,0x68,
                         0xDF,0xED,0xB9,0xD0,0x0A,0xD7,0xE0,0xDB,0x4B,0x1D,0x9B,0x7F,0x44,0x3F,0xC0,0xE7,
                         0xBB,0xDB,0x82,0x83,0xF1,0x9E,0x2B,0x7E,0x2B,0x0A,0x6C,0x13,0x97,0x6A,0xB8,0xEF,
                         0x11,0x6F,0x26,0x46,0x20,0x00,0x58,0x85,0x6C,0xD8,0xE0,0xF0,0xE3,0x8F,0xA0,0xD5,
                         0xB2,0x8B,0x02,0x3D,0xDA,0x64,0x98,0x8B,0x4D,0xC9,0xD7,0xD2,0x6A,0x0F,0x8C,0x11,
                         0x77,0x9F,0x20,0xE4,0xAC,0xD4,0x92,0x8E,0x3B,0x70,0x66,0x4D,0x5B,0x5A,0x13,0xB7,
                         0x6A,0x36,0x9E,0xD9,0x34,0x1C,0x8C,0x56,0xD4,0x1C,0x30,0x91,0x44,0x83,0xCE,0x1A,
                         0xC5,0x5F,0xCE,0x75,0x98,0x50,0x3B,0x7F,0x09,0xC7,0xD2,0xC9,0xA7,0x14,0xF5,0x80,
                         0xEC,0x00,0x16,0x83,0xF2,0x28,0x38,0x64,0x25,0x0E,0xA0,0x45,0xBE,0x33,0xDE,0x18,
                         0x66,0x2F,0x26,0x8B,0xF1,0xA6,0x1E,0x12,0xFD,0x44,0x2B,0x18,0x86,0x40,0x65,0xCE,
                         0x22,0xB3,0xF4,0xD6,0x1A,0x7A,0xBB,0x83,0x65,0x90,0x67,0xD3,0x37,0x77,0xE4,0x99,
                         0x50,0x48,0x23,0x7F,0xDA,0xCF,0xB3,0x22,0xF9,0x13,0x40,0x4B,0x18,0xC2,0x87,0xE8,
                         0x25,0x39,0x0A,0x53,0x66,0x2B,0x6A,0x53,0x9D,0x75,0x61,0x7C,0x78,0xE6,0xD3,0xDE,
                         0x08,0xFB,0x2E,0xF5,0x26,0xE2,0xC0,0xBD,0xCE,0x65,0xD6,0x9A,0xC0,0x01,0xF4,0xB3,
                         0x10,0x64,0x9C,0xEE,0x5A,0x84,0x8F,0x71,0xBB,0x01,0xBD,0x16,0xEA,0xDD,0x9C,0xF7,
                         0x00,0x01,0x00,0x01,0x02,0x9F,0x26,0x24,0xE7,0x65,0xE7,0x3F,0xF3,0xCC,0x98,0x20,
                         0x9C,0xE4,0x37,0x06,0xB0,0x75,0xD5,0x9F,0x55,0x88,0x2F,0x3B,0x1B,0xC2,0xA3,0x7E,
                         0xE3,0x4F,0x17,0xE9,0x00 };
        SID2E_WriteDataById(D01C_Key, 295);
        CheckNegResponse(0X31);

        TestStep("Security Unlock");
        SecurityUnlock(0x01);

        TestStep("Read Public Key");
        //如果文件包含公钥
        SID22_ReadDataByIdentifier(D01C);
        //如果未写入公钥
        if (0 == Raw_CheckNegResponse(0x22)) {
            TestReport_StepInfo(Pass);
        }
        else if (TpRxData[2] = 0x62) {
            TestReport_HTML_StepDescription("ECU已写入公钥！", Pass);
        }

        TestStep("Write Public Key Lack 1 Byte");
        SID2E_WriteDataById(D01C_Key, 293);
        CheckNegResponse(0X31);

        TestStep("Write Public Key Change 1 Byte");
        D01C_Key[293] = ~D01C_Key[293];
        SID2E_WriteDataById(D01C_Key, 294);
        CheckNegResponse(0X31);

        D01C_Key[293] = ~D01C_Key[293];

        TestStep("Write Public Key Add 1 Byte");
        SID2E_WriteDataById(D01C_Key, 295);
        CheckNegResponse(0X31);

        TestStep("Write Public Correct");
        SID2E_WriteDataById(D01C_Key, 294);
        CheckPosResponse_SID2E(D01C);

        TestStep("Read Public Key");
        //如果文件包含公钥
        SID22_ReadDataByIdentifier(D01C);
        CheckPosResponse_SID22(D01C);

        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_005_007_052_059() {
    try {
        strcpy(TestCaseName, "CS_Test_005_006_007_052_059");
        strcpy(TestScopeName, "Secure Flash");
        strcpy(testPurpose_CH, "测试目的：");

        TestPrecondition();
        u32 DTC = 0xD14C51;
        u8 DID_C00E[2] = { 0xC0,0x0E };
        u8 DID_C00F[2] = { 0xC0,0x0F };
        u8 DID_Store[148] = { 0x0 };

        const int recordCount = 20;
        const int recordSize = 7;
        const int recordCount_C00F = 20;
        const int recordSize_C00F = 9;
        u32 lastCount = 0, lastCount_C00F = 0, currentCount = 0, currentCount_C00F = 0;
        u8 lastRecordTime[recordSize] = { 0 };
        u8 currentRecordTime[recordSize] = { 0 };
        u8 lastRecordTime_C00F[recordSize_C00F] = { 0 };
        u8 currentRecordTime_C00F[recordSize_C00F] = { 0 };

        TestStep("Enter Extended Session");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        SecurityUnlock(0x19);

        ReadAndRecordC00E(&lastCount, recordCount, recordSize,lastRecordTime);
        ReadAndRecordC00F(&lastCount_C00F, recordCount_C00F, recordSize_C00F, lastRecordTime_C00F);

        TestStep("Clear DTCs");
        SID14_ClearAllDTC();
        CheckPosResponse_SID14();
        SID11_ECUReset(0x01);
        CheckPosResponse_SID11(0x01);

        TestStep("Read DTC from the DUT with status mask 0x08");
        RX_SID19_ReportDTCByStatusMask(0x08);
        RX_CheckResponse_DTC_SID19_02(0x08, &DTC, 1);

        TestStep("Enter Default Session");
        SID10_SessionControl(DefaultSession);
        CheckPosResponse_SID10(DefaultSession);

        TestStep("Enter Programming Session");
        SID10_SessionControl(ProgrammingSession);
        CheckPosResponse_SID10(ProgrammingSession);

        TestStep("Check Programming Session");
        CheckCurrentSession(ProgrammingSession);

        TestStep("Read Public Key");
        u8 D01C[2] = { 0XD0,0X1C };
        //如果文件包含公钥
        SID22_ReadDataByIdentifier(D01C);
        //如果未写入公钥
        if (0 == Raw_CheckNegResponse(0x22)) {
            TestReport_StepInfo(Pass);
        }
        else if (TpRxData[2] = 0x62) {
            TestReport_HTML_StepDescription("ECU已写入公钥！", Pass);
        }

        TestStep("Downlaod SBL");
        u8 DID1[2] = { 0xED,0x20 };
        SID22_ReadDataByIdentifier(DID1);
        CheckPosResponse_SID22(DID1);

        u8 DID2[2] = { 0xF1,0xAA };
        SID22_ReadDataByIdentifier(DID2);
        CheckPosResponse_SID22(DID2);

        TestSeq_DownloadSBL(0);

        u8 DID3[2] = { 0xF1,0xA2 };
        SID22_ReadDataByIdentifier(DID3);
        CheckNegResponse(0x31);

        ActivateSBL();
        SID22_ReadDataByIdentifier(DID1);
        CheckPosResponse_SID22(DID1);

        SID22_ReadDataByIdentifier(DID3);
        CheckPosResponse_SID22(DID3, 8);

        //SendTesterPresent(TP_OFF);

        TestWait(6000);

        //TestStep("Enter Programming Session");
        //SID10_SessionControl(ProgrammingSession);

        //TestStep("Check Programming Session");
        //CheckCurrentSession(ProgrammingSession);

        TestStep("Read Public Key");
        //如果文件包含公钥
        SID22_ReadDataByIdentifier(D01C);
        //如果未写入公钥
        if (0 == Raw_CheckNegResponse(0x22)) {
            TestReport_StepInfo(Pass);
        }
        else if (TpRxData[2] = 0x62) {
            TestReport_HTML_StepDescription("ECU已写入公钥！", Pass);
        }

        TestStep("Write Public Key");
        u8 D01C_Key[295] = { 0xD0,0x1C,0xD2,0x1E,0x37,0x2A,0x9B,0x64,0xF1,0x8E,0xC6,0x53,0x43,0xE0,0x2A,0x2D,0x48,0x06,
                         0x19,0x56,0x70,0x17,0xC8,0x85,0xD1,0x7B,0x5B,0xD4,0x8A,0x41,0xCB,0x53,0xB4,0x68,
                         0xDF,0xED,0xB9,0xD0,0x0A,0xD7,0xE0,0xDB,0x4B,0x1D,0x9B,0x7F,0x44,0x3F,0xC0,0xE7,
                         0xBB,0xDB,0x82,0x83,0xF1,0x9E,0x2B,0x7E,0x2B,0x0A,0x6C,0x13,0x97,0x6A,0xB8,0xEF,
                         0x11,0x6F,0x26,0x46,0x20,0x00,0x58,0x85,0x6C,0xD8,0xE0,0xF0,0xE3,0x8F,0xA0,0xD5,
                         0xB2,0x8B,0x02,0x3D,0xDA,0x64,0x98,0x8B,0x4D,0xC9,0xD7,0xD2,0x6A,0x0F,0x8C,0x11,
                         0x77,0x9F,0x20,0xE4,0xAC,0xD4,0x92,0x8E,0x3B,0x70,0x66,0x4D,0x5B,0x5A,0x13,0xB7,
                         0x6A,0x36,0x9E,0xD9,0x34,0x1C,0x8C,0x56,0xD4,0x1C,0x30,0x91,0x44,0x83,0xCE,0x1A,
                         0xC5,0x5F,0xCE,0x75,0x98,0x50,0x3B,0x7F,0x09,0xC7,0xD2,0xC9,0xA7,0x14,0xF5,0x80,
                         0xEC,0x00,0x16,0x83,0xF2,0x28,0x38,0x64,0x25,0x0E,0xA0,0x45,0xBE,0x33,0xDE,0x18,
                         0x66,0x2F,0x26,0x8B,0xF1,0xA6,0x1E,0x12,0xFD,0x44,0x2B,0x18,0x86,0x40,0x65,0xCE,
                         0x22,0xB3,0xF4,0xD6,0x1A,0x7A,0xBB,0x83,0x65,0x90,0x67,0xD3,0x37,0x77,0xE4,0x99,
                         0x50,0x48,0x23,0x7F,0xDA,0xCF,0xB3,0x22,0xF9,0x13,0x40,0x4B,0x18,0xC2,0x87,0xE8,
                         0x25,0x39,0x0A,0x53,0x66,0x2B,0x6A,0x53,0x9D,0x75,0x61,0x7C,0x78,0xE6,0xD3,0xDE,
                         0x08,0xFB,0x2E,0xF5,0x26,0xE2,0xC0,0xBD,0xCE,0x65,0xD6,0x9A,0xC0,0x01,0xF4,0xB3,
                         0x10,0x64,0x9C,0xEE,0x5A,0x84,0x8F,0x71,0xBB,0x01,0xBD,0x16,0xEA,0xDD,0x9C,0xF7,
                         0x00,0x01,0x00,0x01,0x02,0x9F,0x26,0x24,0xE7,0x65,0xE7,0x3F,0xF3,0xCC,0x98,0x20,
                         0x9C,0xE4,0x37,0x06,0xB0,0x75,0xD5,0x9F,0x55,0x88,0x2F,0x3B,0x1B,0xC2,0xA3,0x7E,
                         0xE3,0x4F,0x17,0xE9,0x00 };
        SID2E_WriteDataById(D01C_Key, 295);
        CheckNegResponse(0X31);

        TestStep("Security Unlock");
        SecurityUnlock(0x01);

        TestStep("Read Public Key");
        //如果文件包含公钥
        SID22_ReadDataByIdentifier(D01C);
        //如果未写入公钥
        if (0 == Raw_CheckNegResponse(0x22)) {
            TestReport_StepInfo(Pass);
        }
        else if (TpRxData[2] = 0x62) {
            TestReport_HTML_StepDescription("ECU已写入公钥！", Pass);
        }

        TestStep("Write Public Key Lack 1 Byte");
        SID2E_WriteDataById(D01C_Key, 293);
        CheckNegResponse(0X31);

        TestStep("Write Public Key Change 1 Byte");
        D01C_Key[293] = ~D01C_Key[293];
        SID2E_WriteDataById(D01C_Key, 294);
        CheckNegResponse(0X31);

        D01C_Key[293] = ~D01C_Key[293];

        TestStep("Write Public Key Add 1 Byte");
        SID2E_WriteDataById(D01C_Key, 295);
        CheckNegResponse(0X31);

        TestStep("Write Public Correct");
        SID2E_WriteDataById(D01C_Key, 294);
        CheckPosResponse_SID2E(D01C);

        TestStep("Read Public Key");
        //如果文件包含公钥
        SID22_ReadDataByIdentifier(D01C);
        CheckPosResponse_SID22(D01C);

        //-----C00E------

        TestStep("ECU Reset");
        SID11_ECUReset(0x01);
        CheckPosResponse_SID11(0x01);

        TestStep("Enter Extended Session");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        SecurityUnlock(0x19);
        ReadAndRecordC00E(&currentCount, recordCount, recordSize, currentRecordTime);
        ReadAndRecordC00F(&currentCount_C00F, recordCount_C00F, recordSize_C00F, currentRecordTime_C00F);
        checkInCrease(currentCount, lastCount, currentRecordTime, lastRecordTime, recordSize);
        checkInCrease(currentCount_C00F, lastCount_C00F, currentRecordTime_C00F, lastRecordTime_C00F, recordSize_C00F);

        //---------DTC--------

        TestStep("ECU Reset");
        SID11_ECUReset(0x01);
        CheckPosResponse_SID11(0x01);

        TestStep("Read DTC from the DUT with status mask 0x08");
        RX_SID19_ReportDTCByStatusMask(0x08);
        RX_CheckResponse_DTC_SID19_02(0x08, &DTC, 0);

        TestStep("DUT Enter Next Cycle");
        NM_NextOperation_NoSwitch();

        TestStep("Read DTC Extended from the DUT");
        u8 DTCArray[3];
        DTCArray[0] = (DTC & 0xFF0000) >> 16;
        DTCArray[1] = (DTC & 0x00FF00) >> 8;
        DTCArray[2] = (DTC & 0x0000FF);
        RX_SID19_ReportDTCExtendedDataRecordByDTCNumber(DTCArray, 0x10);
        Rx_CheckPosResponse_SID19_06(DTCArray);

        SID11_ECUReset(0x81);
        CheckNonResponse();
        TestWait(ECUResetTime);

        CheckCurrentSession(0x01);


        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_006() {
    try {
        strcpy(TestCaseName, "CS_Test_006");
        strcpy(TestScopeName, "Secure Flash");
        strcpy(testPurpose_CH, "测试目的：");


        TestPrecondition();
        TestSeq_EnterProgrammingSession();

        u8 DID1[2] = { 0xED,0x20 };
        SID22_ReadDataByIdentifier(DID1);
        CheckPosResponse_SID22(DID1);

        u8 DID2[2] = { 0xF1,0xAA };
        SID22_ReadDataByIdentifier(DID2);
        CheckPosResponse_SID22(DID2);

        TestSeq_DownloadSBL(0);

        u8 DID3[2] = { 0xF1,0xA2 };
        //SID22_ReadDataByIdentifier(DID3);
        //CheckNegResponse(0x31);

        if (0 != ActivateSBL()) {
            com.del_cyclic_messages();
            TestPostCondition();
            return Test_Result;
        }

        SID22_ReadDataByIdentifier(DID1);
        CheckPosResponse_SID22(DID1);

        SID22_ReadDataByIdentifier(DID3);
        CheckPosResponse_SID22(DID3, 8);

        char RDkeyvalue[20] = "APP0_Production";
        TestReport_HTML_StepDescription("----Download APP With Production Private Key----");
        vbfparser(RDkeyvalue);
        for (int i = 0; i < APPFileNum; i++)
        {
            char StepDespriction[256];
            char filename[300] = "";
            vbfparser(RDkeyvalue);
            app.get_system_var_string("Diagnostic0.下载文件_DataFile", 512, &filename[0]);
            filename[strlen(filename) - 9] = '\0';
            snprintf(StepDespriction, sizeof(StepDespriction), "Download File is %s ", filename);
            TestReport_HTML_StepDescription(StepDespriction);
            EraseAddress();
            FileDownload();
            TestReport_HTML_StepDescription("----Check Signature----");
            CheckSignature();
            RDkeyvalue[3]++;
        }

        char keyvalue[20] = "APP0_Path";
        TestReport_HTML_StepDescription("----Download APP With RD Private Key----");
        vbfparser(keyvalue);
        for (int i = 0; i < APPFileNum; i++)
        {
            char StepDespriction[256];
            char filename[300] = "";
            vbfparser(keyvalue);
            app.get_system_var_string("Diagnostic0.下载文件_DataFile", 512, &filename[0]);
            filename[strlen(filename) - 9] = '\0';
            snprintf(StepDespriction, sizeof(StepDespriction), "Download File is %s ", filename);
            TestReport_HTML_StepDescription(StepDespriction);
            EraseAddress();
            FileDownload();
            TestReport_HTML_StepDescription("----Check Signature----");
            CheckSignature();
            keyvalue[3]++;
        }

        SID11_ECUReset(0x81);
        CheckNonResponse();
        TestWait(ECUResetTime);

        CheckCurrentSession(0x01);

        TestPostCondition();

        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_007() {
    try {
        strcpy(TestCaseName, "CS_Test_007");
        strcpy(TestScopeName, "Secure Flash");
        strcpy(testPurpose_CH, "测试目的：");

        TestPrecondition();
        u32 DTC = 0xD14C51;

        TestStep("Enter Extended Session");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        TestStep("Clear DTCs");
        SID14_ClearAllDTC();
        CheckPosResponse_SID14();
        SID11_ECUReset(0x01);
        CheckPosResponse_SID11(0x01);

        TestStep("Read DTC from the DUT with status mask 0x08");
        RX_SID19_ReportDTCByStatusMask(0x08);
        RX_CheckResponse_DTC_SID19_02(0x08, &DTC, 1);

        TestStep("Enter Program Session");
        SID10_SessionControl(ProgrammingSession);
        CheckPosResponse_SID10(ProgrammingSession);


        TestSeq_EnterProgrammingSession();
        TestSeq_DownloadSBL();


        TestStep("Write Public Key");
        u8 D01C[2] = { 0xD0,0x1C };
        u8 D01C_Key[295] = { 0xD0,0x1C,0xD2,0x1E,0x37,0x2A,0x9B,0x64,0xF1,0x8E,0xC6,0x53,0x43,0xE0,0x2A,0x2D,0x48,0x06,
                         0x19,0x56,0x70,0x17,0xC8,0x85,0xD1,0x7B,0x5B,0xD4,0x8A,0x41,0xCB,0x53,0xB4,0x68,
                         0xDF,0xED,0xB9,0xD0,0x0A,0xD7,0xE0,0xDB,0x4B,0x1D,0x9B,0x7F,0x44,0x3F,0xC0,0xE7,
                         0xBB,0xDB,0x82,0x83,0xF1,0x9E,0x2B,0x7E,0x2B,0x0A,0x6C,0x13,0x97,0x6A,0xB8,0xEF,
                         0x11,0x6F,0x26,0x46,0x20,0x00,0x58,0x85,0x6C,0xD8,0xE0,0xF0,0xE3,0x8F,0xA0,0xD5,
                         0xB2,0x8B,0x02,0x3D,0xDA,0x64,0x98,0x8B,0x4D,0xC9,0xD7,0xD2,0x6A,0x0F,0x8C,0x11,
                         0x77,0x9F,0x20,0xE4,0xAC,0xD4,0x92,0x8E,0x3B,0x70,0x66,0x4D,0x5B,0x5A,0x13,0xB7,
                         0x6A,0x36,0x9E,0xD9,0x34,0x1C,0x8C,0x56,0xD4,0x1C,0x30,0x91,0x44,0x83,0xCE,0x1A,
                         0xC5,0x5F,0xCE,0x75,0x98,0x50,0x3B,0x7F,0x09,0xC7,0xD2,0xC9,0xA7,0x14,0xF5,0x80,
                         0xEC,0x00,0x16,0x83,0xF2,0x28,0x38,0x64,0x25,0x0E,0xA0,0x45,0xBE,0x33,0xDE,0x18,
                         0x66,0x2F,0x26,0x8B,0xF1,0xA6,0x1E,0x12,0xFD,0x44,0x2B,0x18,0x86,0x40,0x65,0xCE,
                         0x22,0xB3,0xF4,0xD6,0x1A,0x7A,0xBB,0x83,0x65,0x90,0x67,0xD3,0x37,0x77,0xE4,0x99,
                         0x50,0x48,0x23,0x7F,0xDA,0xCF,0xB3,0x22,0xF9,0x13,0x40,0x4B,0x18,0xC2,0x87,0xE8,
                         0x25,0x39,0x0A,0x53,0x66,0x2B,0x6A,0x53,0x9D,0x75,0x61,0x7C,0x78,0xE6,0xD3,0xDE,
                         0x08,0xFB,0x2E,0xF5,0x26,0xE2,0xC0,0xBD,0xCE,0x65,0xD6,0x9A,0xC0,0x01,0xF4,0xB3,
                         0x10,0x64,0x9C,0xEE,0x5A,0x84,0x8F,0x71,0xBB,0x01,0xBD,0x16,0xEA,0xDD,0x9C,0xF7,
                         0x00,0x01,0x00,0x01,0x02,0x9F,0x26,0x24,0xE7,0x65,0xE7,0x3F,0xF3,0xCC,0x98,0x20,
                         0x9C,0xE4,0x37,0x06,0xB0,0x75,0xD5,0x9F,0x55,0x88,0x2F,0x3B,0x1B,0xC2,0xA3,0x7E,
                         0xE3,0x4F,0x17,0xE9,0x00 };

        TestStep("Write Public Correct");
        SID2E_WriteDataById(D01C_Key, 294);
        CheckPosResponse_SID2E(D01C);

        TestStep("ECU Reset");
        SID11_ECUReset(0x01);
        CheckPosResponse_SID11(0x01);

        TestStep("Read DTC from the DUT with status mask 0x08");
        RX_SID19_ReportDTCByStatusMask(0x08);
        RX_CheckResponse_DTC_SID19_02(0x08, &DTC, 0);

        TestStep("DUT Enter Next Cycle");
        NM_NextOperation_NoSwitch();

        TestStep("Read DTC Extended from the DUT");
        u8 DTCArray[3];
        DTCArray[0] = (DTC & 0xFF0000) >> 16;
        DTCArray[1] = (DTC & 0x00FF00) >> 8;
        DTCArray[2] = (DTC & 0x0000FF);
        RX_SID19_ReportDTCExtendedDataRecordByDTCNumber(DTCArray,0x10);
        Rx_CheckPosResponse_SID19_06(DTCArray);

        SID11_ECUReset(0x81);
        CheckNonResponse();
        TestWait(ECUResetTime);

        CheckCurrentSession(0x01);

        TestPostCondition();

        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_008() {
    try {
        strcpy(TestCaseName, "CS_Test_008");
        strcpy(TestScopeName, "Secure Access");
        strcpy(testPurpose_CH, "测试目的：");

        pincode0x03[0] = 0x5E;
        pincode0x03[1] = 0x40;
        pincode0x03[2] = 0xDC;
        pincode0x03[3] = 0x91;
        pincode0x03[4] = 0xEC;
        pincode0x03[5] = 0xA1;
        pincode0x03[6] = 0x44;
        pincode0x03[7] = 0xEC;
        pincode0x03[8] = 0x66;
        pincode0x03[9] = 0x3F;
        pincode0x03[10] = 0xCC;
        pincode0x03[11] = 0x2E;
        pincode0x03[12] = 0x6E;
        pincode0x03[13] = 0xD3;
        pincode0x03[14] = 0x46;
        pincode0x03[15] = 0xD8;
        if (Diag_Request_ID == 0x701) {
            pincode0x05[0] = 0x10;
            pincode0x05[1] = 0x54;
            pincode0x05[2] = 0xA8;
            pincode0x05[3] = 0x09;
            pincode0x05[4] = 0x8E;
            pincode0x05[5] = 0xA0;
            pincode0x05[6] = 0x8C;
            pincode0x05[7] = 0x6F;
            pincode0x05[8] = 0x49;
            pincode0x05[9] = 0x25;
            pincode0x05[10] = 0x78;
            pincode0x05[11] = 0x38;
            pincode0x05[12] = 0x2A;
            pincode0x05[13] = 0x11;
            pincode0x05[14] = 0x84;
            pincode0x05[15] = 0x9B;
        }
        else {
            pincode0x05[0] = 0x57;
            pincode0x05[1] = 0xEA;
            pincode0x05[2] = 0xFE;
            pincode0x05[3] = 0xD7;
            pincode0x05[4] = 0x93;
            pincode0x05[5] = 0xDE;
            pincode0x05[6] = 0x70;
            pincode0x05[7] = 0xA8;
            pincode0x05[8] = 0x87;
            pincode0x05[9] = 0xF3;
            pincode0x05[10] = 0xB8;
            pincode0x05[11] = 0x09;
            pincode0x05[12] = 0x1A;
            pincode0x05[13] = 0xDD;
            pincode0x05[14] = 0xFA;
            pincode0x05[15] = 0x75;
        }
        TestPrecondition();
        u8 TestLevel = 0x03;
        if (0 == gCEMFlg) {
            TestInfoPrintWarning("ZCUP Not Applicable");
            TestPostCondition();
            return Test_Result;
        }

        TestInfoPrint("-----Test Level 0x%X-----", TestLevel);
        stepnumber = 2;
        TestStep("Enter ExtendedSession");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        TestStep("Check ExtendedSession");
        CheckCurrentSession(ExtendedSession);

        for (int j = 0; j < 3; j++) {
            TestStep("Wait 2s,Send Incorrect Key");
            TestWait(2000);
            if (j >= 2) {
                SID27_SecurityAccess_RequestSeed(TestLevel);
                CheckNegResponse(0x37);
                break;
            }
            SID27_SecurityAccess_RequestSeed(TestLevel);
            CheckPosResponse_SID27_RequestSeed(TestLevel);
            if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
            {
                TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
            }

            GenSAKeyExOpt(TestLevel);
            KeyArray[15]++;

            TestStep("Send Incorrect Key");
            SID27_SecurityAccess_SendKey(TestLevel);
            CheckNegResponse(0x35 + j);
        }

        TestStep("Wait 10s,Send Incorrect Key");
        TestWait(10000);
        SID27_SecurityAccess_RequestSeed(TestLevel);
        CheckPosResponse_SID27_RequestSeed(TestLevel);
        if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
        {
            TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
        }

        GenSAKeyExOpt(TestLevel);
        KeyArray[15]++;
        SID27_SecurityAccess_SendKey(TestLevel);
        CheckNegResponse(0x35);


        TestStep("Wait 2s,Send Correct Key");
        TestWait(2000);
        SID27_SecurityAccess_RequestSeed(TestLevel);
        CheckPosResponse_SID27_RequestSeed(TestLevel);
        if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
        {
            TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
        }

        GenSAKeyExOpt(TestLevel);

        SID27_SecurityAccess_SendKey(TestLevel);
        CheckPosResponse_SID27_SendKey(TestLevel);

        TestStep("Wait 1s,Request Seed");
        TestWait(1000);
        SID27_SecurityAccess_RequestSeed(TestLevel);
        CheckPosResponse_SID27_RequestSeed(TestLevel);
        if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
        {
            TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Pass);
        }

        TestStep("Enter ExtendedSession");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        TestStep("Check ExtendedSession");
        CheckCurrentSession(ExtendedSession);

        TestStep("Wait 1s,Request Seed");
        TestWait(1000);
        SID27_SecurityAccess_RequestSeed(TestLevel);
        CheckPosResponse_SID27_RequestSeed(TestLevel);
        if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
        {
            TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
        }
        else {
            TestReport_HTML_StepDescription("Seed not equals to 0x000000!", Pass);
        }

        SID11_ECUReset(0x81);
        CheckNonResponse();

        TestPostCondition();


        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_009() {
    try {
        strcpy(TestCaseName, "CS_Test_009");
        strcpy(TestScopeName, "Secure Access");
        strcpy(testPurpose_CH, "测试目的：");

        pincode0x03[0] = 0x5E;
        pincode0x03[1] = 0x40;
        pincode0x03[2] = 0xDC;
        pincode0x03[3] = 0x91;
        pincode0x03[4] = 0xEC;
        pincode0x03[5] = 0xA1;
        pincode0x03[6] = 0x44;
        pincode0x03[7] = 0xEC;
        pincode0x03[8] = 0x66;
        pincode0x03[9] = 0x3F;
        pincode0x03[10] = 0xCC;
        pincode0x03[11] = 0x2E;
        pincode0x03[12] = 0x6E;
        pincode0x03[13] = 0xD3;
        pincode0x03[14] = 0x46;
        pincode0x03[15] = 0xD8;
        if (Diag_Request_ID == 0x701) {
            pincode0x05[0] = 0x10;
            pincode0x05[1] = 0x54;
            pincode0x05[2] = 0xA8;
            pincode0x05[3] = 0x09;
            pincode0x05[4] = 0x8E;
            pincode0x05[5] = 0xA0;
            pincode0x05[6] = 0x8C;
            pincode0x05[7] = 0x6F;
            pincode0x05[8] = 0x49;
            pincode0x05[9] = 0x25;
            pincode0x05[10] = 0x78;
            pincode0x05[11] = 0x38;
            pincode0x05[12] = 0x2A;
            pincode0x05[13] = 0x11;
            pincode0x05[14] = 0x84;
            pincode0x05[15] = 0x9B;
        }
        else {
            pincode0x05[0] = 0x57;
            pincode0x05[1] = 0xEA;
            pincode0x05[2] = 0xFE;
            pincode0x05[3] = 0xD7;
            pincode0x05[4] = 0x93;
            pincode0x05[5] = 0xDE;
            pincode0x05[6] = 0x70;
            pincode0x05[7] = 0xA8;
            pincode0x05[8] = 0x87;
            pincode0x05[9] = 0xF3;
            pincode0x05[10] = 0xB8;
            pincode0x05[11] = 0x09;
            pincode0x05[12] = 0x1A;
            pincode0x05[13] = 0xDD;
            pincode0x05[14] = 0xFA;
            pincode0x05[15] = 0x75;
        }
        TestPrecondition();
        u8 TestLevel = 0x05;

        TestInfoPrint("-----Test Level 0x%X-----", TestLevel);
        stepnumber = 2;
        TestStep("Enter ExtendedSession");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        TestStep("Check ExtendedSession");
        CheckCurrentSession(ExtendedSession);

        for (int j = 0; j < 3; j++) {
            TestStep("Wait 2s,Send Incorrect Key");
            TestWait(2000);
            if (j >= 2) {
                SID27_SecurityAccess_RequestSeed(TestLevel);
                CheckNegResponse(0x37);
                break;
            }
            SID27_SecurityAccess_RequestSeed(TestLevel);
            CheckPosResponse_SID27_RequestSeed(TestLevel);
            if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
            {
                TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
            }

            GenSAKeyExOpt(TestLevel);
            KeyArray[15]++;

            TestStep("Send Incorrect Key");
            SID27_SecurityAccess_SendKey(TestLevel);
            CheckNegResponse(0x35 + j);
        }

        TestStep("Wait 10s,Send Incorrect Key");
        TestWait(10000);
        SID27_SecurityAccess_RequestSeed(TestLevel);
        CheckPosResponse_SID27_RequestSeed(TestLevel);
        if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
        {
            TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
        }

        GenSAKeyExOpt(TestLevel);
        KeyArray[15]++;
        SID27_SecurityAccess_SendKey(TestLevel);
        CheckNegResponse(0x35);


        TestStep("Wait 2s,Send Correct Key");
        TestWait(2000);
        SID27_SecurityAccess_RequestSeed(TestLevel);
        CheckPosResponse_SID27_RequestSeed(TestLevel);
        if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
        {
            TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
        }

        GenSAKeyExOpt(TestLevel);

        SID27_SecurityAccess_SendKey(TestLevel);
        CheckPosResponse_SID27_SendKey(TestLevel);

        TestStep("Wait 1s,Request Seed");
        TestWait(1000);
        SID27_SecurityAccess_RequestSeed(TestLevel);
        CheckPosResponse_SID27_RequestSeed(TestLevel);
        if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
        {
            TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Pass);
        }

        TestStep("Enter ExtendedSession");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        TestStep("Check ExtendedSession");
        CheckCurrentSession(ExtendedSession);

        TestStep("Wait 1s,Request Seed");
        TestWait(1000);
        SID27_SecurityAccess_RequestSeed(TestLevel);
        CheckPosResponse_SID27_RequestSeed(TestLevel);
        if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
        {
            TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
        }
        else {
            TestReport_HTML_StepDescription("Seed not equals to 0x000000!", Pass);
        }

        SID11_ECUReset(0x81);
        CheckNonResponse();

        TestPostCondition();


        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_010() {
    try {
        strcpy(TestCaseName, "CS_Test_010");
        strcpy(TestScopeName, "Secure Access");
        strcpy(testPurpose_CH, "测试目的：");

        pincode0x03[0] = 0x5E;
        pincode0x03[1] = 0x40;
        pincode0x03[2] = 0xDC;
        pincode0x03[3] = 0x91;
        pincode0x03[4] = 0xEC;
        pincode0x03[5] = 0xA1;
        pincode0x03[6] = 0x44;
        pincode0x03[7] = 0xEC;
        pincode0x03[8] = 0x66;
        pincode0x03[9] = 0x3F;
        pincode0x03[10] = 0xCC;
        pincode0x03[11] = 0x2E;
        pincode0x03[12] = 0x6E;
        pincode0x03[13] = 0xD3;
        pincode0x03[14] = 0x46;
        pincode0x03[15] = 0xD8;
        if (Diag_Request_ID == 0x701) {
            pincode0x05[0] = 0x10;
            pincode0x05[1] = 0x54;
            pincode0x05[2] = 0xA8;
            pincode0x05[3] = 0x09;
            pincode0x05[4] = 0x8E;
            pincode0x05[5] = 0xA0;
            pincode0x05[6] = 0x8C;
            pincode0x05[7] = 0x6F;
            pincode0x05[8] = 0x49;
            pincode0x05[9] = 0x25;
            pincode0x05[10] = 0x78;
            pincode0x05[11] = 0x38;
            pincode0x05[12] = 0x2A;
            pincode0x05[13] = 0x11;
            pincode0x05[14] = 0x84;
            pincode0x05[15] = 0x9B;
        }
        else {
            pincode0x05[0] = 0x57;
            pincode0x05[1] = 0xEA;
            pincode0x05[2] = 0xFE;
            pincode0x05[3] = 0xD7;
            pincode0x05[4] = 0x93;
            pincode0x05[5] = 0xDE;
            pincode0x05[6] = 0x70;
            pincode0x05[7] = 0xA8;
            pincode0x05[8] = 0x87;
            pincode0x05[9] = 0xF3;
            pincode0x05[10] = 0xB8;
            pincode0x05[11] = 0x09;
            pincode0x05[12] = 0x1A;
            pincode0x05[13] = 0xDD;
            pincode0x05[14] = 0xFA;
            pincode0x05[15] = 0x75;
        }
        TestPrecondition();
        u8 TestLevel = 0x11;

        TestInfoPrint("-----Test Level 0x%X-----", TestLevel);
        stepnumber = 2;
        TestStep("Enter ExtendedSession");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        TestStep("Check ExtendedSession");
        CheckCurrentSession(ExtendedSession);

        for (int j = 0; j < 3; j++) {
            TestStep("Wait 2s,Send Incorrect Key");
            TestWait(2000);
            if (j >= 2) {
                SID27_SecurityAccess_RequestSeed(TestLevel);
                CheckNegResponse(0x37);
                break;
            }
            SID27_SecurityAccess_RequestSeed(TestLevel);
            CheckPosResponse_SID27_RequestSeed(TestLevel);
            if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
            {
                TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
            }

            GenSAKeyExOpt(TestLevel);
            KeyArray[15]++;

            TestStep("Send Incorrect Key");
            SID27_SecurityAccess_SendKey(TestLevel);
            CheckNegResponse(0x35 + j);
        }

        TestStep("Wait 10s,Send Incorrect Key");
        TestWait(10000);
        SID27_SecurityAccess_RequestSeed(TestLevel);
        CheckPosResponse_SID27_RequestSeed(TestLevel);
        if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
        {
            TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
        }

        GenSAKeyExOpt(TestLevel);
        KeyArray[15]++;
        SID27_SecurityAccess_SendKey(TestLevel);
        CheckNegResponse(0x35);


        TestStep("Wait 2s,Send Correct Key");
        TestWait(2000);
        SID27_SecurityAccess_RequestSeed(TestLevel);
        CheckPosResponse_SID27_RequestSeed(TestLevel);
        if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
        {
            TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
        }

        GenSAKeyExOpt(TestLevel);

        SID27_SecurityAccess_SendKey(TestLevel);
        CheckPosResponse_SID27_SendKey(TestLevel);

        TestStep("Wait 1s,Request Seed");
        TestWait(1000);
        SID27_SecurityAccess_RequestSeed(TestLevel);
        CheckPosResponse_SID27_RequestSeed(TestLevel);
        if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
        {
            TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Pass);
        }

        TestStep("Enter ExtendedSession");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        TestStep("Check ExtendedSession");
        CheckCurrentSession(ExtendedSession);

        TestStep("Wait 1s,Request Seed");
        TestWait(1000);
        SID27_SecurityAccess_RequestSeed(TestLevel);
        CheckPosResponse_SID27_RequestSeed(TestLevel);
        if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
        {
            TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
        }
        else {
            TestReport_HTML_StepDescription("Seed not equals to 0x000000!", Pass);
        }

        SID11_ECUReset(0x81);
        CheckNonResponse();

        TestPostCondition();


        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_011() {
    try {
        strcpy(TestCaseName, "CS_Test_011");
        strcpy(TestScopeName, "Secure Access");
        strcpy(testPurpose_CH, "测试目的：");

        pincode0x03[0] = 0x5E;
        pincode0x03[1] = 0x40;
        pincode0x03[2] = 0xDC;
        pincode0x03[3] = 0x91;
        pincode0x03[4] = 0xEC;
        pincode0x03[5] = 0xA1;
        pincode0x03[6] = 0x44;
        pincode0x03[7] = 0xEC;
        pincode0x03[8] = 0x66;
        pincode0x03[9] = 0x3F;
        pincode0x03[10] = 0xCC;
        pincode0x03[11] = 0x2E;
        pincode0x03[12] = 0x6E;
        pincode0x03[13] = 0xD3;
        pincode0x03[14] = 0x46;
        pincode0x03[15] = 0xD8;
        if (Diag_Request_ID == 0x701) {
            pincode0x05[0] = 0x10;
            pincode0x05[1] = 0x54;
            pincode0x05[2] = 0xA8;
            pincode0x05[3] = 0x09;
            pincode0x05[4] = 0x8E;
            pincode0x05[5] = 0xA0;
            pincode0x05[6] = 0x8C;
            pincode0x05[7] = 0x6F;
            pincode0x05[8] = 0x49;
            pincode0x05[9] = 0x25;
            pincode0x05[10] = 0x78;
            pincode0x05[11] = 0x38;
            pincode0x05[12] = 0x2A;
            pincode0x05[13] = 0x11;
            pincode0x05[14] = 0x84;
            pincode0x05[15] = 0x9B;
        }
        else {
            pincode0x05[0] = 0x57;
            pincode0x05[1] = 0xEA;
            pincode0x05[2] = 0xFE;
            pincode0x05[3] = 0xD7;
            pincode0x05[4] = 0x93;
            pincode0x05[5] = 0xDE;
            pincode0x05[6] = 0x70;
            pincode0x05[7] = 0xA8;
            pincode0x05[8] = 0x87;
            pincode0x05[9] = 0xF3;
            pincode0x05[10] = 0xB8;
            pincode0x05[11] = 0x09;
            pincode0x05[12] = 0x1A;
            pincode0x05[13] = 0xDD;
            pincode0x05[14] = 0xFA;
            pincode0x05[15] = 0x75;
        }
        TestPrecondition();
        u8 TestLevel = 0x19;

        TestInfoPrint("-----Test Level 0x%X-----", TestLevel);
        stepnumber = 2;
        TestStep("Enter ExtendedSession");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        TestStep("Check ExtendedSession");
        CheckCurrentSession(ExtendedSession);

        for (int j = 0; j < 3; j++) {
            TestStep("Wait 2s,Send Incorrect Key");
            TestWait(2000);
            if (j >= 2) {
                SID27_SecurityAccess_RequestSeed(TestLevel);
                CheckNegResponse(0x37);
                break;
            }
            SID27_SecurityAccess_RequestSeed(TestLevel);
            CheckPosResponse_SID27_RequestSeed(TestLevel);
            if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
            {
                TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
            }

            GenSAKeyExOpt(TestLevel);
            KeyArray[15]++;

            TestStep("Send Incorrect Key");
            SID27_SecurityAccess_SendKey(TestLevel);
            CheckNegResponse(0x35 + j);
        }

        TestStep("Wait 10s,Send Incorrect Key");
        TestWait(10000);
        SID27_SecurityAccess_RequestSeed(TestLevel);
        CheckPosResponse_SID27_RequestSeed(TestLevel);
        if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
        {
            TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
        }

        GenSAKeyExOpt(TestLevel);
        KeyArray[15]++;
        SID27_SecurityAccess_SendKey(TestLevel);
        CheckNegResponse(0x35);


        TestStep("Wait 2s,Send Correct Key");
        TestWait(2000);
        SID27_SecurityAccess_RequestSeed(TestLevel);
        CheckPosResponse_SID27_RequestSeed(TestLevel);
        if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
        {
            TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
        }

        GenSAKeyExOpt(TestLevel);

        SID27_SecurityAccess_SendKey(TestLevel);
        CheckPosResponse_SID27_SendKey(TestLevel);

        TestStep("Wait 1s,Request Seed");
        TestWait(1000);
        SID27_SecurityAccess_RequestSeed(TestLevel);
        CheckPosResponse_SID27_RequestSeed(TestLevel);
        if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
        {
            TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Pass);
        }

        TestStep("Enter ExtendedSession");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        TestStep("Check ExtendedSession");
        CheckCurrentSession(ExtendedSession);

        TestStep("Wait 1s,Request Seed");
        TestWait(1000);
        SID27_SecurityAccess_RequestSeed(TestLevel);
        CheckPosResponse_SID27_RequestSeed(TestLevel);
        if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
        {
            TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
        }
        else {
            TestReport_HTML_StepDescription("Seed not equals to 0x000000!", Pass);
        }

        SID11_ECUReset(0x81);
        CheckNonResponse();

        TestPostCondition();


        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_012() {
    try {
        strcpy(TestCaseName, "CS_Test_012");
        strcpy(TestScopeName, "Secure Access");
        strcpy(testPurpose_CH, "测试目的：");


        TestPrecondition();

        //1
        SID10_SessionControl(0x03);
        CheckPosResponse_SID10(0x03);
        TestWait(2000);
        CheckCurrentSession(0x03);
        if (0 == gCEMFlg) {
            TestInfoPrintWarning("ZCUP Not Applicable");
            TestPostCondition();
            return Test_Result;
        }

        char* SeedLogPath = nullptr;
        app.get_configuration_file_path(&SeedLogPath);
        for (int count = 0; count < SecurityUnlockCycle; count++) {
            TestInfoPrint("------Step: Request seed %d cycle", count + 1);
            SendTesterPresent(TP_ON);
            SID10_SessionControl(0x03);
            CheckPosResponse_SID10(0x03);
            CheckCurrentSession(0x03);
            char* SeedLogPath = nullptr;
            app.get_configuration_file_path(&SeedLogPath);
            fs::path baseDir = fs::path(SeedLogPath) / "ToolBox/NM/common/data_0x03";
            fs::create_directories(baseDir);
            for (int count = 0; count < SecurityUnlockCycle; count++) {
                TestInfoPrint("------Step: Request seed %d cycle", count + 1);
                SendTesterPresent(TP_ON);
                SID10_SessionControl(0x03);
                CheckPosResponse_SID10(0x03);
                CheckCurrentSession(0x03);
                TextFileManager manager;
                fs::path fullPath = baseDir / (std::to_string(count + 1) + ".txt");
                std::string filename = fullPath.string();
                printf("Record filename is %s", filename.c_str());
                std::ofstream outFile;
                if (manager.openFile(outFile, filename)) {
                    for (int i = 0; i < SecurityUnlockTimes; i++) {
                        TestInfoPrint("------Step: Request seed %d times", i + 1);
                        SID27_SecurityAccess_RequestSeed(0x03);
                        CheckPosResponse_SID27_RequestSeed(0x03);
                        std::stringstream ss;
                        for (int i = 0; i < 16; i++) {
                            ss << std::hex << std::setfill('0') << std::setw(2) << (int)SeedArray[i];
                        }
                        std::string hexData = ss.str();
                        if (i == SecurityUnlockTimes - 1) {
                            manager.write(outFile, hexData);
                        }
                        else {
                            manager.writeLine(outFile, hexData);
                        }
                    }
                    outFile.close();
                    calculateNIST(SeedLogPath, filename.c_str());
                }
                //manager.deleteFile(filename);
            }
        }
        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_013() {
    try {
        strcpy(TestCaseName, "CS_Test_013");
        strcpy(TestScopeName, "Secure Access");
        strcpy(testPurpose_CH, "测试目的：");


        TestPrecondition();

        //1
        SID10_SessionControl(0x03);
        CheckPosResponse_SID10(0x03);
        TestWait(2000);
        CheckCurrentSession(0x03);
        char* SeedLogPath = nullptr;
        app.get_configuration_file_path(&SeedLogPath);
        fs::path baseDir = fs::path(SeedLogPath) / "ToolBox/NM/common/data_0x05";
        fs::create_directories(baseDir);
        for (int count = 0; count < SecurityUnlockCycle; count++) {
            TestInfoPrint("------Step: Request seed %d cycle", count + 1);
            SendTesterPresent(TP_ON);
            SID10_SessionControl(0x03);
            CheckPosResponse_SID10(0x03);
            CheckCurrentSession(0x03);
            TextFileManager manager;
            fs::path fullPath = baseDir / (std::to_string(count + 1) + ".txt");
            std::string filename = fullPath.string();
            printf("Record filename is %s", filename.c_str());
            std::ofstream outFile;
            if (manager.openFile(outFile, filename)) {
                for (int i = 0; i < SecurityUnlockTimes; i++) {
                    TestInfoPrint("------Step: Request seed %d times", i + 1);
                    SID27_SecurityAccess_RequestSeed(0x05);
                    CheckPosResponse_SID27_RequestSeed(0x05);
                    std::stringstream ss;
                    for (int i = 0; i < 16; i++) {
                        ss << std::hex << std::setfill('0') << std::setw(2) << (int)SeedArray[i];
                    }
                    std::string hexData = ss.str();
                    if (i == SecurityUnlockTimes - 1) {
                        manager.write(outFile, hexData);
                    }
                    else {
                        manager.writeLine(outFile, hexData);
                    }
                }
                outFile.close();
                calculateNIST(SeedLogPath, filename.c_str());
            }
            //manager.deleteFile(filename);
        }
        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_014() {
    try {
        strcpy(TestCaseName, "CS_Test_014");
        strcpy(TestScopeName, "Secure Access");
        strcpy(testPurpose_CH, "测试目的：");


        TestPrecondition();

        //1
        SID10_SessionControl(0x03);
        CheckPosResponse_SID10(0x03);
        TestWait(2000);
        CheckCurrentSession(0x03);
        char* SeedLogPath = nullptr;
        app.get_configuration_file_path(&SeedLogPath);
        fs::path baseDir = fs::path(SeedLogPath) / "ToolBox/NM/common/data_0x11";
        fs::create_directories(baseDir);
        for (int count = 0; count < SecurityUnlockCycle; count++) {
            TestInfoPrint("------Step: Request seed %d cycle", count + 1);
            SendTesterPresent(TP_ON);
            SID10_SessionControl(0x03);
            CheckPosResponse_SID10(0x03);
            CheckCurrentSession(0x03);
            TextFileManager manager;
            fs::path fullPath = baseDir / (std::to_string(count + 1) + ".txt");
            std::string filename = fullPath.string();
            printf("Record filename is %s", filename.c_str());
            std::ofstream outFile;
            if (manager.openFile(outFile, filename)) {
                for (int i = 0; i < SecurityUnlockTimes; i++) {
                    TestInfoPrint("------Step: Request seed %d times", i + 1);
                    SID27_SecurityAccess_RequestSeed(0x11);
                    CheckPosResponse_SID27_RequestSeed(0x11);
                    std::stringstream ss;
                    for (int i = 0; i < 16; i++) {
                        ss << std::hex << std::setfill('0') << std::setw(2) << (int)SeedArray[i];
                    }
                    std::string hexData = ss.str();
                    if (i == SecurityUnlockTimes - 1) {
                        manager.write(outFile, hexData);
                    }
                    else {
                        manager.writeLine(outFile, hexData);
                    }
                }
                outFile.close();
                calculateNIST(SeedLogPath, filename.c_str());
            }
            //manager.deleteFile(filename);
        }
        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_015() {
    try {
        strcpy(TestCaseName, "CS_Test_015");
        strcpy(TestScopeName, "Secure Access");
        strcpy(testPurpose_CH, "测试目的：");


        TestPrecondition();

        //1
        SID10_SessionControl(0x03);
        CheckPosResponse_SID10(0x03);
        TestWait(2000);
        CheckCurrentSession(0x03);
        char* SeedLogPath = nullptr;
        app.get_configuration_file_path(&SeedLogPath);
        fs::path baseDir = fs::path(SeedLogPath) / "ToolBox/NM/common/data_0x19";
        fs::create_directories(baseDir);
        for (int count = 0; count < SecurityUnlockCycle; count++) {
            TestInfoPrint("------Step: Request seed %d cycle", count + 1);
            SendTesterPresent(TP_ON);
            SID10_SessionControl(0x03);
            CheckPosResponse_SID10(0x03);
            CheckCurrentSession(0x03);
            TextFileManager manager;
            fs::path fullPath = baseDir / (std::to_string(count + 1) + ".txt");
            std::string filename = fullPath.string();
            printf("Record filename is %s", filename.c_str());
            std::ofstream outFile;
            if (manager.openFile(outFile, filename)) {
                for (int i = 0; i < SecurityUnlockTimes; i++) {
                    TestInfoPrint("------Step: Request seed %d times", i + 1);
                    SID27_SecurityAccess_RequestSeed(0x19);
                    CheckPosResponse_SID27_RequestSeed(0x19);
                    std::stringstream ss;
                    for (int i = 0; i < 16; i++) {
                        ss << std::hex << std::setfill('0') << std::setw(2) << (int)SeedArray[i];
                    }
                    std::string hexData = ss.str();
                    if (i == SecurityUnlockTimes - 1) {
                        manager.write(outFile, hexData);
                    }
                    else {
                        manager.writeLine(outFile, hexData);
                    }
                }
                outFile.close();
                calculateNIST(SeedLogPath, filename.c_str());
            }
            //manager.deleteFile(filename);
        }
        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_016() {
    try {
        strcpy(TestCaseName, "CS_Test_016");
        strcpy(TestScopeName, "Secure Access");
        strcpy(testPurpose_CH, "测试目的：");


        TestPrecondition();

        //1
        SID10_SessionControl(0x03);
        CheckPosResponse_SID10(0x03);
        TestWait(2000);
        CheckCurrentSession(0x03);
        if (0 == gCEMFlg) {
            TestInfoPrintWarning("ZCUP Not Applicable");
            TestPostCondition();
            return Test_Result;
        }

        char* SeedLogPath = nullptr;
        app.get_configuration_file_path(&SeedLogPath);
        fs::path baseDir = fs::path(SeedLogPath) / "ToolBox/NM/common/NM_data_0x03";
        fs::create_directories(baseDir);
        for (int count = 0; count < SecurityUnlockCycle; count++) {
            TestInfoPrint("------Step: Request seed %d cycle", count + 1);
            SendTesterPresent(TP_ON);
            SID10_SessionControl(0x03);
            CheckPosResponse_SID10(0x03);
            CheckCurrentSession(0x03);
            TextFileManager manager;
            fs::path fullPath = baseDir / (std::to_string(count + 1) + ".txt");
            std::string filename = fullPath.string();
            printf("Record filename is %s", filename.c_str());
            std::ofstream outFile;
            if (manager.openFile(outFile, filename)) {
                for (int i = 0; i < SecurityUnlockTimes; i++) {
                    TestInfoPrint("------Step: Request seed %d times", i + 1);
                    SID27_SecurityAccess_RequestSeed(0x03);
                    CheckPosResponse_SID27_RequestSeed(0x03);
                    std::stringstream ss;
                    for (int i = 0; i < 16; i++) {
                        ss << std::hex << std::setfill('0') << std::setw(2) << (int)SeedArray[i];
                    }
                    std::string hexData = ss.str();
                    if (i == SecurityUnlockTimes - 1) {
                        manager.write(outFile, hexData);
                    }
                    else {
                        manager.writeLine(outFile, hexData);
                    }
                }
                outFile.close();
                calculateNIST(SeedLogPath, filename.c_str());
                NM_NextOperation_NoSwitch();
            }
            //manager.deleteFile(filename);
        }

        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_017() {
    try {
        strcpy(TestCaseName, "CS_Test_017");
        strcpy(TestScopeName, "Secure Access");
        strcpy(testPurpose_CH, "测试目的：");


        TestPrecondition();

        //1
        SID10_SessionControl(0x03);
        CheckPosResponse_SID10(0x03);
        TestWait(2000);
        CheckCurrentSession(0x03);
        char* SeedLogPath = nullptr;
        app.get_configuration_file_path(&SeedLogPath);
        fs::path baseDir = fs::path(SeedLogPath) / "ToolBox/NM/common/NM_data_0x05";
        fs::create_directories(baseDir);
        for (int count = 0; count < SecurityUnlockCycle; count++) {
            TestInfoPrint("------Step: Request seed %d cycle", count + 1);
            SendTesterPresent(TP_ON);
            SID10_SessionControl(0x03);
            CheckPosResponse_SID10(0x03);
            CheckCurrentSession(0x03);
            TextFileManager manager;
            fs::path fullPath = baseDir / (std::to_string(count + 1) + ".txt");
            std::string filename = fullPath.string();
            printf("Record filename is %s", filename.c_str());
            std::ofstream outFile;
            if (manager.openFile(outFile, filename)) {
                for (int i = 0; i < SecurityUnlockTimes; i++) {
                    TestInfoPrint("------Step: Request seed %d times", i + 1);
                    SID27_SecurityAccess_RequestSeed(0x05);
                    CheckPosResponse_SID27_RequestSeed(0x05);
                    std::stringstream ss;
                    for (int i = 0; i < 16; i++) {
                        ss << std::hex << std::setfill('0') << std::setw(2) << (int)SeedArray[i];
                    }
                    std::string hexData = ss.str();
                    if (i == SecurityUnlockTimes - 1) {
                        manager.write(outFile, hexData);
                    }
                    else {
                        manager.writeLine(outFile, hexData);
                    }
                }
                outFile.close();
                calculateNIST(SeedLogPath, filename.c_str());
                NM_NextOperation_NoSwitch();
            }
            //manager.deleteFile(filename);
        }


        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_018() {
    try {
        strcpy(TestCaseName, "CS_Test_018");
        strcpy(TestScopeName, "Secure Access");
        strcpy(testPurpose_CH, "测试目的：");


        TestPrecondition();

        //1
        SID10_SessionControl(0x03);
        CheckPosResponse_SID10(0x03);
        TestWait(2000);
        CheckCurrentSession(0x03);
        char* SeedLogPath = nullptr;
        app.get_configuration_file_path(&SeedLogPath);
        fs::path baseDir = fs::path(SeedLogPath) / "ToolBox/NM/common/NM_data_0x11";
        fs::create_directories(baseDir);
        for (int count = 0; count < SecurityUnlockCycle; count++) {
            TestInfoPrint("------Step: Request seed %d cycle", count + 1);
            SendTesterPresent(TP_ON);
            SID10_SessionControl(0x03);
            CheckPosResponse_SID10(0x03);
            CheckCurrentSession(0x03);
            TextFileManager manager;
            fs::path fullPath = baseDir / (std::to_string(count + 1) + ".txt");
            std::string filename = fullPath.string();
            printf("Record filename is %s", filename.c_str());
            std::ofstream outFile;
            if (manager.openFile(outFile, filename)) {
                for (int i = 0; i < SecurityUnlockTimes; i++) {
                    TestInfoPrint("------Step: Request seed %d times", i + 1);
                    SID27_SecurityAccess_RequestSeed(0x11);
                    CheckPosResponse_SID27_RequestSeed(0x11);
                    std::stringstream ss;
                    for (int i = 0; i < 16; i++) {
                        ss << std::hex << std::setfill('0') << std::setw(2) << (int)SeedArray[i];
                    }
                    std::string hexData = ss.str();
                    if (i == SecurityUnlockTimes - 1) {
                        manager.write(outFile, hexData);
                    }
                    else {
                        manager.writeLine(outFile, hexData);
                    }
                }
                outFile.close();
                calculateNIST(SeedLogPath, filename.c_str());
                NM_NextOperation_NoSwitch();
            }
            //manager.deleteFile(filename);
        }


        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_019() {
    try {
        strcpy(TestCaseName, "CS_Test_019");
        strcpy(TestScopeName, "Secure Access");
        strcpy(testPurpose_CH, "测试目的：");


        TestPrecondition();

        //1
        SID10_SessionControl(0x03);
        CheckPosResponse_SID10(0x03);
        TestWait(2000);
        CheckCurrentSession(0x03);
        char* SeedLogPath = nullptr;
        app.get_configuration_file_path(&SeedLogPath);
        fs::path baseDir = fs::path(SeedLogPath) / "ToolBox/NM/common/NM_data_0x19";
        fs::create_directories(baseDir);
        for (int count = 0; count < SecurityUnlockCycle; count++) {
            TestInfoPrint("------Step: Request seed %d cycle", count + 1);
            SendTesterPresent(TP_ON);
            SID10_SessionControl(0x03);
            CheckPosResponse_SID10(0x03);
            CheckCurrentSession(0x03);
            TextFileManager manager;
            fs::path fullPath = baseDir / (std::to_string(count + 1) + ".txt");
            std::string filename = fullPath.string();
            printf("Record filename is %s", filename.c_str());
            std::ofstream outFile;
            if (manager.openFile(outFile, filename)) {
                for (int i = 0; i < SecurityUnlockTimes; i++) {
                    TestInfoPrint("------Step: Request seed %d times", i + 1);
                    SID27_SecurityAccess_RequestSeed(0x19);
                    CheckPosResponse_SID27_RequestSeed(0x19);
                    std::stringstream ss;
                    for (int i = 0; i < 16; i++) {
                        ss << std::hex << std::setfill('0') << std::setw(2) << (int)SeedArray[i];
                    }
                    std::string hexData = ss.str();
                    if (i == SecurityUnlockTimes - 1) {
                        manager.write(outFile, hexData);
                    }
                    else {
                        manager.writeLine(outFile, hexData);
                    }
                }
                outFile.close();
                calculateNIST(SeedLogPath, filename.c_str());
                NM_NextOperation_NoSwitch();
            }
            //manager.deleteFile(filename);
        }

        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_020() {
    try {
        strcpy(TestCaseName, "CS_Test_020");
        strcpy(TestScopeName, "Secure Access");
        strcpy(testPurpose_CH, "测试目的：");


        TestPrecondition();

        //1
        SID10_SessionControl(0x03);
        CheckPosResponse_SID10(0x03);
        TestWait(2000);
        CheckCurrentSession(0x03);
        if (0 == gCEMFlg) {
            TestInfoPrintWarning("ZCUP Not Applicable");
            TestPostCondition();
            return Test_Result;
        }

        char* SeedLogPath = nullptr;
        app.get_configuration_file_path(&SeedLogPath);
        fs::path baseDir = fs::path(SeedLogPath) / "ToolBox/NM/common/PwrReset_data_0x03";
        fs::create_directories(baseDir);
        for (int count = 0; count < SecurityUnlockCycle; count++) {
            TestInfoPrint("------Step: Request seed %d cycle", count + 1);
            SendTesterPresent(TP_ON);
            SID10_SessionControl(0x03);
            CheckPosResponse_SID10(0x03);
            CheckCurrentSession(0x03);
            TextFileManager manager;
            fs::path fullPath = baseDir / (std::to_string(count + 1) + ".txt");
            std::string filename = fullPath.string();
            printf("Record filename is %s", filename.c_str());
            std::ofstream outFile;
            if (manager.openFile(outFile, filename)) {
                for (int i = 0; i < SecurityUnlockTimes; i++) {
                    TestInfoPrint("------Step: Request seed %d times", i + 1);
                    SID27_SecurityAccess_RequestSeed(0x03);
                    CheckPosResponse_SID27_RequestSeed(0x03);
                    std::stringstream ss;
                    for (int i = 0; i < 16; i++) {
                        ss << std::hex << std::setfill('0') << std::setw(2) << (int)SeedArray[i];
                    }
                    std::string hexData = ss.str();
                    if (i == SecurityUnlockTimes - 1) {
                        manager.write(outFile, hexData);
                    }
                    else {
                        manager.writeLine(outFile, hexData);
                    }
                }
                outFile.close();
                calculateNIST(SeedLogPath, filename.c_str());
                SendTesterPresent(TP_OFF);
                NMWakeUp(0);
                KL15(off);
                KL30(off);
                TestWait(30000);
                KL30(on);
                KL15(on);
                TestWait(10000);
            }
            //manager.deleteFile(filename);
        }

        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_021() {
    try {
        strcpy(TestCaseName, "CS_Test_021");
        strcpy(TestScopeName, "Secure Access");
        strcpy(testPurpose_CH, "测试目的：");


        TestPrecondition();

        //1
        SID10_SessionControl(0x03);
        CheckPosResponse_SID10(0x03);
        TestWait(2000);
        CheckCurrentSession(0x03);
        char* SeedLogPath = nullptr;
        app.get_configuration_file_path(&SeedLogPath);
        fs::path baseDir = fs::path(SeedLogPath) / "ToolBox/NM/common/PwrReset_data_0x05";
        fs::create_directories(baseDir);
        for (int count = 0; count < SecurityUnlockCycle; count++) {
            TestInfoPrint("------Step: Request seed %d cycle", count + 1);
            SendTesterPresent(TP_ON);
            SID10_SessionControl(0x03);
            CheckPosResponse_SID10(0x03);
            CheckCurrentSession(0x03);
            TextFileManager manager;
            fs::path fullPath = baseDir / (std::to_string(count + 1) + ".txt");
            std::string filename = fullPath.string();
            printf("Record filename is %s", filename.c_str());
            std::ofstream outFile;
            if (manager.openFile(outFile, filename)) {
                for (int i = 0; i < SecurityUnlockTimes; i++) {
                    TestInfoPrint("------Step: Request seed %d times", i + 1);
                    SID27_SecurityAccess_RequestSeed(0x05);
                    CheckPosResponse_SID27_RequestSeed(0x05);
                    std::stringstream ss;
                    for (int i = 0; i < 16; i++) {
                        ss << std::hex << std::setfill('0') << std::setw(2) << (int)SeedArray[i];
                    }
                    std::string hexData = ss.str();
                    if (i == SecurityUnlockTimes - 1) {
                        manager.write(outFile, hexData);
                    }
                    else {
                        manager.writeLine(outFile, hexData);
                    }
                }
                outFile.close();
                calculateNIST(SeedLogPath, filename.c_str());
                SendTesterPresent(TP_OFF);
                NMWakeUp(0);
                KL15(off);
                KL30(off);
                TestWait(30000);
                KL30(on);
                KL15(on);
                TestWait(10000);
            }
            //manager.deleteFile(filename);
        }

        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_022() {
    try {
        strcpy(TestCaseName, "CS_Test_022");
        strcpy(TestScopeName, "Secure Access");
        strcpy(testPurpose_CH, "测试目的：");


        TestPrecondition();

        //1
        SID10_SessionControl(0x03);
        CheckPosResponse_SID10(0x03);
        TestWait(2000);
        CheckCurrentSession(0x03);
        char* SeedLogPath = nullptr;
        app.get_configuration_file_path(&SeedLogPath);
        fs::path baseDir = fs::path(SeedLogPath) / "ToolBox/NM/common/PwrReset_data_0x11";
        fs::create_directories(baseDir);
        for (int count = 0; count < SecurityUnlockCycle; count++) {
            TestInfoPrint("------Step: Request seed %d cycle", count + 1);
            SendTesterPresent(TP_ON);
            SID10_SessionControl(0x03);
            CheckPosResponse_SID10(0x03);
            CheckCurrentSession(0x03);
            TextFileManager manager;
            fs::path fullPath = baseDir / (std::to_string(count + 1) + ".txt");
            std::string filename = fullPath.string();
            printf("Record filename is %s", filename.c_str());
            std::ofstream outFile;
            if (manager.openFile(outFile, filename)) {
                for (int i = 0; i < SecurityUnlockTimes; i++) {
                    TestInfoPrint("------Step: Request seed %d times", i + 1);
                    SID27_SecurityAccess_RequestSeed(0x11);
                    CheckPosResponse_SID27_RequestSeed(0x11);
                    std::stringstream ss;
                    for (int i = 0; i < 16; i++) {
                        ss << std::hex << std::setfill('0') << std::setw(2) << (int)SeedArray[i];
                    }
                    std::string hexData = ss.str();
                    if (i == SecurityUnlockTimes - 1) {
                        manager.write(outFile, hexData);
                    }
                    else {
                        manager.writeLine(outFile, hexData);
                    }
                }
                outFile.close();
                calculateNIST(SeedLogPath, filename.c_str());
                SendTesterPresent(TP_OFF);
                NMWakeUp(0);
                KL15(off);
                KL30(off);
                TestWait(30000);
                KL30(on);
                KL15(on);
                TestWait(10000);
            }
            //manager.deleteFile(filename);
        }

        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_023() {
    try {
        strcpy(TestCaseName, "CS_Test_023");
        strcpy(TestScopeName, "Secure Access");
        strcpy(testPurpose_CH, "测试目的：");


        TestPrecondition();

        //1
        SID10_SessionControl(0x03);
        CheckPosResponse_SID10(0x03);
        TestWait(2000);
        CheckCurrentSession(0x03);
        char* SeedLogPath = nullptr;
        app.get_configuration_file_path(&SeedLogPath);
        fs::path baseDir = fs::path(SeedLogPath) / "ToolBox/NM/common/PwrReset_data_0x19";
        fs::create_directories(baseDir);
        for (int count = 0; count < SecurityUnlockCycle; count++) {
            TestInfoPrint("------Step: Request seed %d cycle", count + 1);
            SendTesterPresent(TP_ON);
            SID10_SessionControl(0x03);
            CheckPosResponse_SID10(0x03);
            CheckCurrentSession(0x03);
            TextFileManager manager;
            fs::path fullPath = baseDir / (std::to_string(count + 1) + ".txt");
            std::string filename = fullPath.string();
            printf("Record filename is %s", filename.c_str());
            std::ofstream outFile;
            if (manager.openFile(outFile, filename)) {
                for (int i = 0; i < SecurityUnlockTimes; i++) {
                    TestInfoPrint("------Step: Request seed %d times", i + 1);
                    SID27_SecurityAccess_RequestSeed(0x19);
                    CheckPosResponse_SID27_RequestSeed(0x19);
                    std::stringstream ss;
                    for (int i = 0; i < 16; i++) {
                        ss << std::hex << std::setfill('0') << std::setw(2) << (int)SeedArray[i];
                    }
                    std::string hexData = ss.str();
                    if (i == SecurityUnlockTimes - 1) {
                        manager.write(outFile, hexData);
                    }
                    else {
                        manager.writeLine(outFile, hexData);
                    }
                }
                outFile.close();
                calculateNIST(SeedLogPath, filename.c_str());
                SendTesterPresent(TP_OFF);
                NMWakeUp(0);
                KL15(off);
                KL30(off);
                TestWait(30000);
                KL30(on);
                KL15(on);
                TestWait(10000);
            }
            //manager.deleteFile(filename);
        }

        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_024() {
    try {
        strcpy(TestCaseName, "CS_Test_024");
        strcpy(TestScopeName, "Secure Access");
        strcpy(testPurpose_CH, "测试目的：");


        TestStep("Enter DefaultSession");
        SID10_SessionControl(DefaultSession);
        CheckPosResponse_SID10(DefaultSession);

        TestStep("Check DefaultSession");
        CheckCurrentSession(DefaultSession);

        TestInfoPrint("-----Test Level 0x01-----");
        stepnumber = 2;
        TestStep("Enter ProgrammingSession");
        SID10_SessionControl(ProgrammingSession);
        CheckPosResponse_SID10(ProgrammingSession);

        TestStep("Check ProgrammingSession");
        CheckCurrentSession(ProgrammingSession);

        for (int j = 0; j < 3; j++) {
            TestStep("Wait 2s,Send Incorrect Key");
            TestWait(2000);
            if (j >= 2) {
                SID27_SecurityAccess_RequestSeed(0x01);
                CheckNegResponse(0x37);
                break;
            }
            SID27_SecurityAccess_RequestSeed(0x01);
            if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
            {
                TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
            }

            GenSAKeyExOpt(0x01);
            KeyArray[15]++;

            SID27_SecurityAccess_SendKey(0x01);
            CheckNegResponse(0x35 + j);
        }

        TestStep("Wait 60s,Send Incorrect Key");
        TestWait(60000);
        SID27_SecurityAccess_RequestSeed(0x01);
        CheckPosResponse_SID27_RequestSeed(0x01);
        if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
        {
            TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
        }

        GenSAKeyExOpt(0x01);
        KeyArray[15]++;
        SID27_SecurityAccess_SendKey(0x01);
        CheckNegResponse(0x35);


        TestStep("Wait 2s,Send Incorrect Key");
        TestWait(2000);
        SID27_SecurityAccess_RequestSeed(0x01);
        CheckPosResponse_SID27_RequestSeed(0x01);
        if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
        {
            TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
        }

        GenSAKeyExOpt(0x01);

        SID27_SecurityAccess_SendKey(0x01);
        CheckPosResponse_SID27_SendKey(0x01);

        TestStep("Wait 1s,Request Seed");
        TestWait(1000);
        SID27_SecurityAccess_RequestSeed(0x01);
        CheckPosResponse_SID27_RequestSeed(0x01);
        if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
        {
            TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Pass);
        }

        TestStep("Enter ProgrammingSession");
        SID10_SessionControl(ProgrammingSession);
        CheckPosResponse_SID10(ProgrammingSession);

        TestStep("Check ProgrammingSession");
        CheckCurrentSession(ProgrammingSession);

        TestStep("Wait 1s,Request Seed");
        TestWait(1000);
        SID27_SecurityAccess_RequestSeed(0x01);
        CheckPosResponse_SID27_RequestSeed(0x01);
        if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
        {
            TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
        }
        else {
            TestReport_HTML_StepDescription("Seed not equals to 0x000000!", Pass);
        }

        SID11_ECUReset(0x81);
        CheckNonResponse();

        TestPostCondition();

        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_025() {
    try {
        strcpy(TestCaseName, "CS_Test_025");
        strcpy(TestScopeName, "Secure Access");
        strcpy(testPurpose_CH, "测试目的：");


        TestPrecondition();

        //1
        SID10_SessionControl(0x02);
        CheckPosResponse_SID10(0x02);
        TestWait(2000);
        CheckCurrentSession(0x02);
        char* SeedLogPath = nullptr;
        app.get_configuration_file_path(&SeedLogPath);
        fs::path baseDir = fs::path(SeedLogPath) / "ToolBox/NM/common/data_0x01";
        fs::create_directories(baseDir);
        for (int count = 0; count < SecurityUnlockCycle; count++) {
            TestInfoPrint("------Step: Request seed %d cycle", count + 1);
            SendTesterPresent(TP_ON);
            SID10_SessionControl(0x03);
            CheckPosResponse_SID10(0x03);
            CheckCurrentSession(0x03);
            TextFileManager manager;
            fs::path fullPath = baseDir / (std::to_string(count + 1) + ".txt");
            std::string filename = fullPath.string();
            printf("Record filename is %s", filename.c_str());
            std::ofstream outFile;
            if (manager.openFile(outFile, filename)) {
                for (int i = 0; i < SecurityUnlockTimes; i++) {
                    TestInfoPrint("------Step: Request seed %d times", i + 1);
                    SID27_SecurityAccess_RequestSeed(0x01);
                    CheckPosResponse_SID27_RequestSeed(0x01);
                    std::stringstream ss;
                    for (int i = 0; i < 16; i++) {
                        ss << std::hex << std::setfill('0') << std::setw(2) << (int)SeedArray[i];
                    }
                    std::string hexData = ss.str();
                    if (i == SecurityUnlockTimes - 1) {
                        manager.write(outFile, hexData);
                    }
                    else {
                        manager.writeLine(outFile, hexData);
                    }
                }
                outFile.close();
                calculateNIST(SeedLogPath, filename.c_str());
            }
            //manager.deleteFile(filename);
        }

        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_026() {
    try {
        strcpy(TestCaseName, "CS_Test_026");
        strcpy(TestScopeName, "Secure Access");
        strcpy(testPurpose_CH, "测试目的：");


        TestPrecondition();

        SID10_SessionControl(0x02);
        CheckPosResponse_SID10(0x02);
        TestWait(2000);
        CheckCurrentSession(0x02);
        char* SeedLogPath = nullptr;
        app.get_configuration_file_path(&SeedLogPath);
        fs::path baseDir = fs::path(SeedLogPath) / "ToolBox/NM/common/NM_data_0x01";
        fs::create_directories(baseDir);
        for (int count = 0; count < SecurityUnlockCycle; count++) {
            TestInfoPrint("------Step: Request seed %d cycle", count + 1);
            SendTesterPresent(TP_ON);
            SID10_SessionControl(0x03);
            CheckPosResponse_SID10(0x03);
            CheckCurrentSession(0x03);
            TextFileManager manager;
            fs::path fullPath = baseDir / (std::to_string(count + 1) + ".txt");
            std::string filename = fullPath.string();
            printf("Record filename is %s", filename.c_str());
            std::ofstream outFile;
            if (manager.openFile(outFile, filename)) {
                for (int i = 0; i < SecurityUnlockTimes; i++) {
                    TestInfoPrint("------Step: Request seed %d times", i + 1);
                    SID27_SecurityAccess_RequestSeed(0x01);
                    CheckPosResponse_SID27_RequestSeed(0x01);
                    std::stringstream ss;
                    for (int i = 0; i < 16; i++) {
                        ss << std::hex << std::setfill('0') << std::setw(2) << (int)SeedArray[i];
                    }
                    std::string hexData = ss.str();
                    if (i == SecurityUnlockTimes - 1) {
                        manager.write(outFile, hexData);
                    }
                    else {
                        manager.writeLine(outFile, hexData);
                    }
                }
                outFile.close();
                calculateNIST(SeedLogPath, filename.c_str());
                NM_NextOperation_NoSwitch();
            }
            //manager.deleteFile(filename);
        }

        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_027() {
    try {
        strcpy(TestCaseName, "CS_Test_027");
        strcpy(TestScopeName, "Secure Access");
        strcpy(testPurpose_CH, "测试目的：");

        TestPrecondition();

        SID10_SessionControl(0x02);
        CheckPosResponse_SID10(0x02);
        TestWait(2000);
        CheckCurrentSession(0x02);

        char* SeedLogPath = nullptr;
        app.get_configuration_file_path(&SeedLogPath);
        fs::path baseDir = fs::path(SeedLogPath) / "ToolBox/NM/common/PwrReset_data_0x01";
        fs::create_directories(baseDir);
        for (int count = 0; count < SecurityUnlockCycle; count++) {
            TestInfoPrint("------Step: Request seed %d cycle", count + 1);
            SendTesterPresent(TP_ON);
            SID10_SessionControl(0x03);
            CheckPosResponse_SID10(0x03);
            CheckCurrentSession(0x03);
            TextFileManager manager;
            fs::path fullPath = baseDir / (std::to_string(count + 1) + ".txt");
            std::string filename = fullPath.string();
            printf("Record filename is %s", filename.c_str());
            std::ofstream outFile;
            if (manager.openFile(outFile, filename)) {
                for (int i = 0; i < SecurityUnlockTimes; i++) {
                    TestInfoPrint("------Step: Request seed %d times", i + 1);
                    SID27_SecurityAccess_RequestSeed(0x01);
                    CheckPosResponse_SID27_RequestSeed(0x01);
                    std::stringstream ss;
                    for (int i = 0; i < 16; i++) {
                        ss << std::hex << std::setfill('0') << std::setw(2) << (int)SeedArray[i];
                    }
                    std::string hexData = ss.str();
                    if (i == SecurityUnlockTimes - 1) {
                        manager.write(outFile, hexData);
                    }
                    else {
                        manager.writeLine(outFile, hexData);
                    }
                }
                outFile.close();
                calculateNIST(SeedLogPath, filename.c_str());
                SendTesterPresent(TP_OFF);
                NMWakeUp(0);
                KL15(off);
                KL30(off);
                TestWait(30000);
                KL30(on);
                KL15(on);
                TestWait(10000);
            }
            //manager.deleteFile(filename);
        }

        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_028() {
    try {
        strcpy(TestCaseName, "CS_Test_028");
        strcpy(TestScopeName, "Secure Boot");
        strcpy(testPurpose_CH, "测试目的：");

        TestPrecondition();
        const int TIMEOUT_MS = 5000;  // 设置5秒超时
        int timeoutCounter = 0;
        s64 timeStamp = 0;
        s64 currentTime = 0;
        for (int count = 0; count < SecurityUnlockCycle; count++) {
            TestInfoPrint("------Step: PowerReset %d cycle", count + 1);
            KL15(off);
            KL30(off);
            com.del_cyclic_messages();
            TestWait(10000);
            checkFirstMessageFlag = true;
            KL30(on);
            app.get_timestamp(&timeStamp);
            printf("The WakeUp Time is %.6lfms", timeStamp / 1000.0);
            if (!checkFirstMessageFlag) {
                printf("The First Message Time is %.6lfms", currentMessageTime / 1000.0);
            }
            else {
                while (checkFirstMessageFlag && timeoutCounter < TIMEOUT_MS) {
                    TestWait(1);
                    timeoutCounter++;
                }
                printf("The First Message Time is %.6lfms", currentMessageTime / 1000.0);
            }
            TestInfoPrint("The Delta Time is %.6lfms", (currentMessageTime - timeStamp) / 1000.0);
            TestWait(10000);
        }
        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_029() {
    try {
        strcpy(TestCaseName, "CS_Test_029");
        strcpy(TestScopeName, "Secure Boot");
        strcpy(testPurpose_CH, "测试目的：");

        TestPrecondition();
        const int TIMEOUT_MS = 5000;  // 设置5秒超时
        int timeoutCounter = 0;
        s64 timeStamp = 0;
        s64 currentTime = 0;
        for (int count = 0; count < SecurityUnlockCycle; count++) {
            TestInfoPrint("------Step: ECUReset %d cycle", count + 1);
            com.del_cyclic_messages();
            TestWait(10000);
            checkFirstMessageFlag = true;
            SID11_ECUReset(0x01);
            CheckPosResponse_SID11(0x01);
            app.get_timestamp(&timeStamp);
            printf("The WakeUp Time is %.6lfms", timeStamp / 1000.0);
            if (!checkFirstMessageFlag) {
                printf("The First Message Time is %.6lfms", currentMessageTime / 1000.0);
            }
            else {
                while (checkFirstMessageFlag && timeoutCounter < TIMEOUT_MS) {
                    TestWait(1);
                    timeoutCounter++;
                }
                printf("The First Message Time is %.6lfms", currentMessageTime / 1000.0);
            }
            TestInfoPrint("The Delta Time is %.6lfms", (currentMessageTime - timeStamp) / 1000.0);
            TestWait(10000);
        }
        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_030() {
    try {
        strcpy(TestCaseName, "CS_Test_030");
        strcpy(TestScopeName, "Secure Boot");
        strcpy(testPurpose_CH, "测试目的：");

        TestPrecondition();
        const int TIMEOUT_MS = 5000;  // 设置5秒超时
        int timeoutCounter = 0;
        s64 timeStamp = 0;
        for (int count = 0; count < SecurityUnlockCycle; count++) {
            TestInfoPrint("------Step: NetworkWakeup %d cycle", count + 1);
            NM_NextOperation_NoSwitch();
            checkFirstMessageFlag = true;
            NMWakeUp(1);
            app.get_timestamp(&timeStamp);
            printf("The WakeUp Time is %.6lfms", timeStamp / 1000.0);
            if (!checkFirstMessageFlag) {
                printf("The First Message Time is %.6lfms", currentMessageTime / 1000.0);
            }
            else {
                while (checkFirstMessageFlag && timeoutCounter < TIMEOUT_MS) {
                    TestWait(1);
                    timeoutCounter++;
                }
                printf("The First Message Time is %.6lfms", currentMessageTime / 1000.0);
            }
            TestInfoPrint("The Delta Time is %.6lfms", (currentMessageTime - timeStamp) / 1000.0);
            TestWait(10000);
        }
        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}
//未完成，Debug工具调试
s32 CS_Test_031() {
    try {
        strcpy(TestCaseName, "CS_Test_031");
        strcpy(TestScopeName, "Secure Boot");
        strcpy(testPurpose_CH, "测试目的：");

        TestPrecondition();

        TestInfoPrintWarning("A Debug tool is needed");

        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}
//未完成，Debug工具调试
s32 CS_Test_032() {
    try {
        strcpy(TestCaseName, "CS_Test_032");
        strcpy(TestScopeName, "Secure Boot");
        strcpy(testPurpose_CH, "测试目的：");

        TestPrecondition();
        TestInfoPrintWarning("A Debug tool is needed");
        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}
//未完成，Debug工具调试
s32 CS_Test_033() {
    try {
        strcpy(TestCaseName, "CS_Test_033");
        strcpy(TestScopeName, "Secure Boot");
        strcpy(testPurpose_CH, "测试目的：");

        TestPrecondition();
        TestInfoPrintWarning("A Debug tool is needed");
        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}
//未完成，Debug工具调试
s32 CS_Test_034() {
    try {
        strcpy(TestCaseName, "CS_Test_034");
        strcpy(TestScopeName, "Secure Debug");
        strcpy(testPurpose_CH, "测试目的：");

        TestPrecondition();
        TestInfoPrintWarning("A Debug tool is needed");
        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}
//未完成，Debug工具调试
s32 CS_Test_035() {
    try {
        strcpy(TestCaseName, "CS_Test_035");
        strcpy(TestScopeName, "Secure Debug");
        strcpy(testPurpose_CH, "测试目的：");

        TestPrecondition();
        TestInfoPrintWarning("A Debug tool is needed");
        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_036() {
    try {
        strcpy(TestCaseName, "CS_Test_036");
        strcpy(TestScopeName, "Secure Audit Log");
        strcpy(testPurpose_CH, "测试目的：");

        u8 DID_C00A[2] = { 0xC0,0x0A };
        u8 DID_Store[192] = { 0x0 };
        u32 lastCount = 0;
        u8 levelCount = 0;

        u32 currentCountPass = 0, currentCountFail = 0, lastCountPass = 0, lastCountFail = 0;
        const int recordCount = C00A_Count;
        const int recordSize = C00A_Size;
        u8 currentTimePass[recordSize] = { 0 };
        u8 currentTimeFail[recordSize] = { 0 };
        u8 lastTimePass[recordSize] = { 0 };
        u8 lastTimeFail[recordSize] = { 0 };
        TestPrecondition();

        TestStep("Unlock 0x19 Read 0xC00A");
        SID10_SessionControl(DefaultSession);
        CheckPosResponse_SID10(DefaultSession);
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);
        SecurityUnlock(0x19);

        PassReadAndRecordC00A(&lastCountPass, recordCount, recordSize, lastTimePass);
        FailReadAndRecordC00A(&lastCountFail, recordCount, recordSize, lastTimeFail);

        SID10_SessionControl(DefaultSession);
        CheckPosResponse_SID10(DefaultSession);
        SID10_SessionControl(ProgrammingSession);
        CheckPosResponse_SID10(ProgrammingSession);

        AppWait(5000, "");

        SecurityUnlock_Invalid(0x01);
        SecurityUnlock(0x01);

        SID10_SessionControl(DefaultSession);
        CheckPosResponse_SID10(DefaultSession);

        AppWait(5000, "");

        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        AppWait(5000, "");

        SecurityUnlock(0x19);

        PassReadAndRecordC00A(&currentCountPass, recordCount, recordSize, currentTimePass);
        FailReadAndRecordC00A(&currentCountFail, recordCount, recordSize, currentTimeFail);

        TestInfoPrint("Check Pass Info");
        checkInCrease(currentCountPass, lastCountPass, currentTimePass, lastTimePass, recordSize,2);
        TestInfoPrint("Check Fail Info");
        checkInCrease(currentCountFail, lastCountFail, currentTimeFail, lastTimeFail, recordSize);

        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_037() {
    try {
        strcpy(TestCaseName, "CS_Test_037");
        strcpy(TestScopeName, "Secure Audit Log");
        strcpy(testPurpose_CH, "测试目的：");

        u8 DID_C00A[2] = { 0xC0,0x0A };
        u8 DID_Store[192] = { 0x0 };
        u32 lastCount = 0;
        u8 levelCount = 0;

        u32 currentCountPass = 0, currentCountFail = 0, lastCountPass = 0, lastCountFail = 0;
        const int recordCount = C00A_Count;
        const int recordSize = C00A_Size;
        u8 currentTimePass[recordSize] = { 0 };
        u8 currentTimeFail[recordSize] = { 0 };
        u8 lastTimePass[recordSize] = { 0 };
        u8 lastTimeFail[recordSize] = { 0 };
        TestPrecondition();

        if (0 == gCEMFlg) {
            TestInfoPrintWarning("ZCUP Not Applicable");
            TestPostCondition();
            return Test_Result;
        }

        TestStep("Unlock 0x19 Read Fail 0xC00A");
        SID10_SessionControl(DefaultSession);
        CheckPosResponse_SID10(DefaultSession);
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);
        SecurityUnlock(0x19);

        PassReadAndRecordC00A(&lastCountPass, recordCount, recordSize, lastTimePass);
        FailReadAndRecordC00A(&lastCountFail, recordCount, recordSize, lastTimeFail);

        SecurityUnlock_Invalid(0x03);
        SecurityUnlock(0x03);

        SID10_SessionControl(DefaultSession);
        CheckPosResponse_SID10(DefaultSession);
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);
        SecurityUnlock(0x19);

        PassReadAndRecordC00A(&currentCountPass, recordCount, recordSize, currentTimePass);
        FailReadAndRecordC00A(&currentCountFail, recordCount, recordSize, currentTimeFail);

        TestInfoPrint("Check Pass Info");
        checkInCrease(currentCountPass, lastCountPass, currentTimePass, lastTimePass, recordSize, 2);
        TestInfoPrint("Check Fail Info");
        checkInCrease(currentCountFail, lastCountFail, currentTimeFail, lastTimeFail, recordSize);

        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_038() {
    try {
        strcpy(TestCaseName, "CS_Test_038");
        strcpy(TestScopeName, "Secure Audit Log");
        strcpy(testPurpose_CH, "测试目的：");

        u8 DID_C00A[2] = { 0xC0,0x0A };
        u8 DID_Store[192] = { 0x0 };
        u32 lastCount = 0;
        u8 levelCount = 0;

        u32 currentCountPass = 0, currentCountFail = 0, lastCountPass = 0, lastCountFail = 0;
        const int recordCount = C00A_Count;
        const int recordSize = C00A_Size;
        u8 currentTimePass[recordSize] = { 0 };
        u8 currentTimeFail[recordSize] = { 0 };
        u8 lastTimePass[recordSize] = { 0 };
        u8 lastTimeFail[recordSize] = { 0 };
        TestPrecondition();

        TestStep("Unlock 0x19 Read Fail 0xC00A");
        SID10_SessionControl(DefaultSession);
        CheckPosResponse_SID10(DefaultSession);
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);
        SecurityUnlock(0x19);

        PassReadAndRecordC00A(&lastCountPass, recordCount, recordSize, lastTimePass);
        FailReadAndRecordC00A(&lastCountFail, recordCount, recordSize, lastTimeFail);

        SecurityUnlock_Invalid(0x05);
        SecurityUnlock(0x05);

        SID10_SessionControl(DefaultSession);
        CheckPosResponse_SID10(DefaultSession);
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);
        SecurityUnlock(0x19);

        PassReadAndRecordC00A(&currentCountPass, recordCount, recordSize, currentTimePass);
        FailReadAndRecordC00A(&currentCountFail, recordCount, recordSize, currentTimeFail);

        TestInfoPrint("Check Pass Info");
        checkInCrease(currentCountPass, lastCountPass, currentTimePass, lastTimePass, recordSize, 2);
        TestInfoPrint("Check Fail Info");
        checkInCrease(currentCountFail, lastCountFail, currentTimeFail, lastTimeFail, recordSize);

        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_039() {
    try {
        strcpy(TestCaseName, "CS_Test_039");
        strcpy(TestScopeName, "Secure Audit Log");
        strcpy(testPurpose_CH, "测试目的：");

        u8 DID_C00A[2] = { 0xC0,0x0A };
        u8 DID_Store[192] = { 0x0 };
        u32 lastCount = 0;
        u8 levelCount = 0;

        u32 currentCountPass = 0, currentCountFail = 0, lastCountPass = 0, lastCountFail = 0;
        const int recordCount = C00A_Count;
        const int recordSize = C00A_Size;
        u8 currentTimePass[recordSize] = { 0 };
        u8 currentTimeFail[recordSize] = { 0 };
        u8 lastTimePass[recordSize] = { 0 };
        u8 lastTimeFail[recordSize] = { 0 };
        TestPrecondition();

        TestStep("Unlock 0x19 Read Fail 0xC00A");
        SID10_SessionControl(DefaultSession);
        CheckPosResponse_SID10(DefaultSession);
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);
        SecurityUnlock(0x19);

        PassReadAndRecordC00A(&lastCountPass, recordCount, recordSize, lastTimePass);
        FailReadAndRecordC00A(&lastCountFail, recordCount, recordSize, lastTimeFail);

        SecurityUnlock_Invalid(0x11);
        SecurityUnlock(0x11);

        SID10_SessionControl(DefaultSession);
        CheckPosResponse_SID10(DefaultSession);
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);
        SecurityUnlock(0x19);

        PassReadAndRecordC00A(&currentCountPass, recordCount, recordSize, currentTimePass);
        FailReadAndRecordC00A(&currentCountFail, recordCount, recordSize, currentTimeFail);

        TestInfoPrint("Check Pass Info");
        checkInCrease(currentCountPass, lastCountPass, currentTimePass, lastTimePass, recordSize, 2);
        TestInfoPrint("Check Fail Info");
        checkInCrease(currentCountFail, lastCountFail, currentTimeFail, lastTimeFail, recordSize);

        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_040() {
    try {
        strcpy(TestCaseName, "CS_Test_040");
        strcpy(TestScopeName, "Secure Audit Log");
        strcpy(testPurpose_CH, "测试目的：");

        u8 DID_C00A[2] = { 0xC0,0x0A };
        u8 DID_Store[192] = { 0x0 };
        u32 lastCount = 0;
        u8 levelCount = 0;

        u32 currentCountPass = 0, currentCountFail = 0, lastCountPass = 0, lastCountFail = 0;
        const int recordCount = C00A_Count;
        const int recordSize = C00A_Size;
        u8 currentTimePass[recordSize] = { 0 };
        u8 currentTimeFail[recordSize] = { 0 };
        u8 lastTimePass[recordSize] = { 0 };
        u8 lastTimeFail[recordSize] = { 0 };
        TestPrecondition();

        TestStep("Unlock 0x19 Read Fail 0xC00A");
        SID10_SessionControl(DefaultSession);
        CheckPosResponse_SID10(DefaultSession);
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);
        SecurityUnlock(0x19);

        PassReadAndRecordC00A(&lastCountPass, recordCount, recordSize, lastTimePass);
        FailReadAndRecordC00A(&lastCountFail, recordCount, recordSize, lastTimeFail);
        AppWait(3000, "");
        SID10_SessionControl(DefaultSession);
        CheckPosResponse_SID10(DefaultSession);
        AppWait(3000, "");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);
        AppWait(3000, "");

        SecurityUnlock_Invalid(0x19);
        SecurityUnlock(0x19);

        PassReadAndRecordC00A(&currentCountPass, recordCount, recordSize, currentTimePass);
        FailReadAndRecordC00A(&currentCountFail, recordCount, recordSize, currentTimeFail);

        TestInfoPrint("Check Pass Info");
        checkInCrease(currentCountPass, lastCountPass, currentTimePass, lastTimePass, recordSize);
        TestInfoPrint("Check Fail Info");
        checkInCrease(currentCountFail, lastCountFail, currentTimeFail, lastTimeFail, recordSize);

        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_041() {
    try {
        strcpy(TestCaseName, "CS_Test_041");
        strcpy(TestScopeName, "Secure Audit Log");
        strcpy(testPurpose_CH, "测试目的：");

        u8 DID_C00A[2] = { 0xC0,0x0A };
        u8 DID_Store[192] = { 0x0 };
        u32 lastCount = 0;
        u8 levelCount = 0;
        int testCount = 12;

        u32 currentCountPass = 0, currentCountFail = 0, lastCountPass = 0, lastCountFail = 0;
        const int recordCount = C00A_Count;
        const int recordSize = C00A_Size;
        u8 currentTimePass[recordSize] = { 0 };
        u8 currentTimeFail[recordSize] = { 0 };
        u8 lastTimePass[recordSize] = { 0 };
        u8 lastTimeFail[recordSize] = { 0 };
        TestPrecondition();

        TestStep("Unlock 0x19 Read Fail 0xC00A");
        SID10_SessionControl(DefaultSession);
        CheckPosResponse_SID10(DefaultSession);
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);
        SecurityUnlock(0x19);

        PassReadAndRecordC00A(&lastCountPass, recordCount, recordSize, lastTimePass);
        FailReadAndRecordC00A(&lastCountFail, recordCount, recordSize, lastTimeFail);
        for (int i = 0; i < testCount; i++) {
            SecurityUnlock_Invalid(0x11);
            //SID10_SessionControl(DefaultSession);
            //CheckPosResponse_SID10(DefaultSession);
            SID10_SessionControl(ExtendedSession);
            CheckPosResponse_SID10(ExtendedSession);
            SecurityUnlock(0x11);
            SID10_SessionControl(ExtendedSession);
            CheckPosResponse_SID10(ExtendedSession);
        }

        SID10_SessionControl(DefaultSession);
        CheckPosResponse_SID10(DefaultSession);
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);
        SecurityUnlock(0x19);

        //PassReadAndRecordC00A(&currentCountPass, recordCount, recordSize, currentTimePass);
        //FailReadAndRecordC00A(&currentCountFail, recordCount, recordSize, currentTimeFail);
        //u8 DID_C00A[2] = { 0xC0,0x0A };
        //u8 DID_Store[192] = { 0x0 };

        SID22_ReadDataByIdentifier(DID_C00A);
        if (0 == CheckPosResponse_SID22(DID_C00A)) {
            StoreArray(DID_Store, TpRxData, 192, 0, 0);
        }
        if (0 == checkC00A(DID_Store, true)) {
            TestInfoPrintPass("C00A Pass Check is Pass");
        }
        else {
            TestInfoPrintFail("C00A Pass Check is Fail");
        }
        if (0 == checkC00A(DID_Store, false)) {
            TestInfoPrintPass("C00A Fail Check is Pass");
        }
        else {
            TestInfoPrintFail("C00A Fail Check is Fail");
        }

        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_042() {
    try {
        strcpy(TestCaseName, "CS_Test_042");
        strcpy(TestScopeName, "Secure Audit Log");
        strcpy(testPurpose_CH, "测试目的：");

        u8 DID_C00B[2] = { 0xC0,0x0B };
        u8 DID_Store[168] = { 0x0 };
        u8 levelCount = 0;
        u8 NRC[2] = {0x22,0xF0};

        u32 currentCount = 0, lastCount = 0;
        const int recordCount = C00B_Count;
        const int recordSize = C00B_Size;
        u8 currentTime[recordSize] = { 0 };
        u8 lastTime[recordSize] = { 0 };
        TestPrecondition();
        gVehSpd = 10;
        SimulateTRC(true);

        TestStep("Unlock 0x19 Read Fail 0xC00B");
        SID10_SessionControl(DefaultSession);
        CheckPosResponse_SID10(DefaultSession);
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);
        SecurityUnlock(0x19);

        ReadAndRecordC00B(&lastCount, recordCount, recordSize, lastTime);

        SID10_SessionControl(ProgrammingSession);
        CheckNegResponse(NRC,2);

        SID10_SessionControl(DefaultSession);
        CheckPosResponse_SID10(DefaultSession);
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);
        SecurityUnlock(0x19);

        ReadAndRecordC00B(&currentCount, recordCount, recordSize, currentTime);

        TestInfoPrint("Check C00B Info");
        checkInCrease(currentCount, lastCount, currentTime, lastTime, recordSize);

        SimulateTRC(false);
        TestPostCondition();

        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_043() {
    try {
        strcpy(TestCaseName, "CS_Test_043");
        strcpy(TestScopeName, "Secure Audit Log");
        strcpy(testPurpose_CH, "测试目的：");

        u8 DID_C00B[2] = { 0xC0,0x0B };
        u8 DID_Store[168] = { 0x0 };
        u8 levelCount = 0;
        u8 NRC[2] = { 0x22,0xF0 };

        u32 currentCount = 0, lastCount = 0;
        const int recordCount = C00B_Count;
        const int recordSize = C00B_Size;
        u8 currentTime[recordSize] = { 0 };
        u8 lastTime[recordSize] = { 0 };
        TestPrecondition();
        gVehSpd = 10;
        SimulateTRC(true);

        TestStep("Unlock 0x19 Read Fail 0xC00B");
        SID10_SessionControl(DefaultSession);
        CheckPosResponse_SID10(DefaultSession);
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);
        SecurityUnlock(0x19);

        ReadAndRecordC00B(&lastCount, recordCount, recordSize, lastTime);

        SID10_SessionControl(0x82);
        CheckNegResponse(NRC, 2);

        SID10_SessionControl(DefaultSession);
        CheckPosResponse_SID10(DefaultSession);
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);
        SecurityUnlock(0x19);

        ReadAndRecordC00B(&currentCount, recordCount, recordSize, currentTime);

        TestInfoPrint("Check C00B Info");
        checkInCrease(currentCount, lastCount, currentTime, lastTime, recordSize);

        SimulateTRC(false);
        TestPostCondition();

        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_044() {
    try {
        strcpy(TestCaseName, "CS_Test_044");
        strcpy(TestScopeName, "Secure Audit Log");
        strcpy(testPurpose_CH, "测试目的：");

        u8 DID_C00B[2] = { 0xC0,0x0B };
        u8 DID_Store[168] = { 0x0 };
        u8 levelCount = 0;
        u8 NRC[2] = { 0x22,0xF0 };

        u32 currentCount = 0, lastCount = 0;
        const int recordCount = C00B_Count;
        const int recordSize = C00B_Size;
        u8 currentTime[recordSize] = { 0 };
        u8 lastTime[recordSize] = { 0 };
        TestPrecondition();
        gVehSpd = 10;
        SimulateTRC(true);

        TestStep("Unlock 0x19 Read Fail 0xC00B");
        SID10_SessionControl(DefaultSession);
        CheckPosResponse_SID10(DefaultSession);
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);
        SecurityUnlock(0x19);

        ReadAndRecordC00B(&lastCount, recordCount, recordSize, lastTime);

        SID11_ECUReset(0x01);
        CheckNegResponse(NRC, 2);

        SID10_SessionControl(DefaultSession);
        CheckPosResponse_SID10(DefaultSession);
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);
        SecurityUnlock(0x19);

        ReadAndRecordC00B(&currentCount, recordCount, recordSize, currentTime);

        TestInfoPrint("Check C00B Info");
        checkInCrease(currentCount, lastCount, currentTime, lastTime, recordSize);

        SimulateTRC(false);
        TestPostCondition();

        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_045() {
    try {
        strcpy(TestCaseName, "CS_Test_045");
        strcpy(TestScopeName, "Secure Audit Log");
        strcpy(testPurpose_CH, "测试目的：");

        u8 DID_C00B[2] = { 0xC0,0x0B };
        u8 DID_Store[168] = { 0x0 };
         
        u8 levelCount = 0;
        u8 NRC[2] = { 0x22,0xF0 };

        u32 currentCount = 0, lastCount = 0;
        const int recordCount = C00B_Count;
        const int recordSize = C00B_Size;
        u8 currentTime[recordSize] = { 0 };
        u8 lastTime[recordSize] = { 0 };
        TestPrecondition();
        gVehSpd = 10;
        SimulateTRC(true);

        TestStep("Unlock 0x19 Read Fail 0xC00B");
        SID10_SessionControl(DefaultSession);
        CheckPosResponse_SID10(DefaultSession);
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);
        SecurityUnlock(0x19);

        ReadAndRecordC00B(&lastCount, recordCount, recordSize, lastTime);

        SID11_ECUReset(0x81);
        CheckNegResponse(NRC, 2);

        SID10_SessionControl(DefaultSession);
        CheckPosResponse_SID10(DefaultSession);
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);
        SecurityUnlock(0x19);

        ReadAndRecordC00B(&currentCount, recordCount, recordSize, currentTime);

        TestInfoPrint("Check C00B Info");
        checkInCrease(currentCount, lastCount, currentTime, lastTime, recordSize);

        SimulateTRC(false);
        TestPostCondition();

        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_046() {
    try {
        strcpy(TestCaseName, "CS_Test_046");
        strcpy(TestScopeName, "Secure Audit Log");
        strcpy(testPurpose_CH, "测试目的：");

        u8 DID_C00B[2] = { 0xC0,0x0B };
        u8 DID_Store[168] = { 0x0 };
         
        u8 levelCount = 0;
        u8 NRC[2] = { 0x22,0xF0 };

        u32 currentCount = 0, lastCount = 0;
        const int recordCount = C00B_Count;
        const int recordSize = C00B_Size;
        u8 currentTime[recordSize] = { 0 };
        u8 lastTime[recordSize] = { 0 };
        TestPrecondition();
        gVehSpd = 10;
        SimulateTRC(true);

        TestStep("Unlock 0x19 Read Fail 0xC00B");
        SID10_SessionControl(DefaultSession);
        CheckPosResponse_SID10(DefaultSession);
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);
        SecurityUnlock(0x19);

        ReadAndRecordC00B(&lastCount, recordCount, recordSize, lastTime);

        for (int i = 0; i < 22; i++) {
            SID10_SessionControl(ProgrammingSession);
            CheckNegResponse(NRC, 2);
        }

        SID10_SessionControl(DefaultSession);
        CheckPosResponse_SID10(DefaultSession);
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);
        SecurityUnlock(0x19);

        SID22_ReadDataByIdentifier(DID_C00B);
        if (0 == CheckPosResponse_SID22(DID_C00B)) {
            StoreArray(DID_Store, TpRxData, 168, 0, 0);
        }
        TestInfoPrint("Check C00B Info");
        if (0 == checkC00B(DID_Store, recordCount, recordSize)) {
            TestInfoPrintPass("Check C00B Loop Pass");
        }
        else {
            TestInfoPrintFail("Check C00B Loop Fail");
        }
        SimulateTRC(false);
        TestPostCondition();

        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_047() {
    try {
        strcpy(TestCaseName, "CS_Test_047");
        strcpy(TestScopeName, "Secure Audit Log");
        strcpy(testPurpose_CH, "测试目的：");

        u8 DID_C00C[2] = { 0xC0,0x0C };
        u8 DID_Store[272] = { 0x0 };
         
        u8 levelCount = 0;
        u8 NRC[2] = { 0x22,0xF0 };

        u32 currentCount = 0, lastCount = 0;
        const int recordCount = C00C_Count;
        const int recordSize = C00C_Size;
        u8 currentTime[recordSize] = { 0 };
        u8 lastTime[recordSize] = { 0 };
        TestPrecondition();

        TestStep("Unlock 0x19 Read Fail 0xC00C");
        SID10_SessionControl(DefaultSession);
        CheckPosResponse_SID10(DefaultSession);
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);
        SecurityUnlock(0x19);

        ReadAndRecordC00C(&lastCount, recordCount, recordSize, lastTime);

        NM_NextOperation_NoSwitch();

        CheckCurrentSession(DefaultSession);

        SID10_SessionControl(DefaultSession);
        CheckPosResponse_SID10(DefaultSession);
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);
        SecurityUnlock(0x19);

        ReadAndRecordC00C(&currentCount, recordCount, recordSize, currentTime);

        TestInfoPrint("Check C00C Info");
        checkInCrease(currentCount, lastCount, currentTime, lastTime, recordSize);
        
        TestPostCondition();

        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_048() {
    try {
        strcpy(TestCaseName, "CS_Test_048");
        strcpy(TestScopeName, "Secure Audit Log");
        strcpy(testPurpose_CH, "测试目的：");

        u8 DID_C00C[2] = { 0xC0,0x0C };
        u8 DID_Store[272] = { 0x0 };
         
        u8 levelCount = 0;
        u8 NRC[2] = { 0x22,0xF0 };

        u32 currentCount = 0, lastCount = 0;
        const int recordCount = C00C_Count;
        const int recordSize = C00C_Size;
        u8 currentTime[recordSize] = { 0 };
        u8 lastTime[recordSize] = { 0 };
        TestPrecondition();

        TestStep("Unlock 0x19 Read Fail 0xC00C");
        SID10_SessionControl(DefaultSession);
        CheckPosResponse_SID10(DefaultSession);
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);
        SecurityUnlock(0x19);

        ReadAndRecordC00C(&lastCount, recordCount, recordSize, lastTime);

        MainPower_OFF();
        AppWait(5000, "");
        MainPower_ON();
        AppWait(5000, "");
        CheckCurrentSession(DefaultSession);

        SID10_SessionControl(DefaultSession);
        CheckPosResponse_SID10(DefaultSession);
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);
        SecurityUnlock(0x19);

        ReadAndRecordC00C(&currentCount, recordCount, recordSize, currentTime);

        TestInfoPrint("Check C00C Info");
        checkInCrease(currentCount, lastCount, currentTime, lastTime, recordSize);

        TestPostCondition();

        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_049() {
    try {
        strcpy(TestCaseName, "CS_Test_049");
        strcpy(TestScopeName, "Secure Audit Log");
        strcpy(testPurpose_CH, "测试目的：");

        u8 DID_C00C[2] = { 0xC0,0x0C };
        u8 DID_Store[272] = { 0x0 };
         
        u8 levelCount = 0;
        u8 NRC[2] = { 0x22,0xF0 };

        u32 currentCount = 0, lastCount = 0;
        const int recordCount = C00C_Count;
        const int recordSize = C00C_Size;
        u8 currentTime[recordSize] = { 0 };
        u8 lastTime[recordSize] = { 0 };
        TestPrecondition();

        TestStep("Unlock 0x19 Read Fail 0xC00C");
        SID10_SessionControl(DefaultSession);
        CheckPosResponse_SID10(DefaultSession);
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);
        SecurityUnlock(0x19);

        ReadAndRecordC00C(&lastCount, recordCount, recordSize, lastTime);

        SID11_ECUReset(0x01);
        CheckPosResponse_SID11(0x01);

        CheckCurrentSession(DefaultSession);

        SID10_SessionControl(DefaultSession);
        CheckPosResponse_SID10(DefaultSession);
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);
        SecurityUnlock(0x19);

        ReadAndRecordC00C(&currentCount, recordCount, recordSize, currentTime);

        TestInfoPrint("Check C00C Info");
        checkInCrease(currentCount, lastCount, currentTime, lastTime, recordSize);

        TestPostCondition();

        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_050() {
    try {
        strcpy(TestCaseName, "CS_Test_050");
        strcpy(TestScopeName, "Secure Audit Log");
        strcpy(testPurpose_CH, "测试目的：");

        u8 DID_C00C[2] = { 0xC0,0x0C };
        u8 DID_Store[272] = { 0x0 };
         
        u8 levelCount = 0;
        u8 NRC[2] = { 0x22,0xF0 };

        u32 currentCount = 0, lastCount = 0;
        const int recordCount = C00C_Count;
        const int recordSize = C00C_Size;
        u8 currentTime[recordSize] = { 0 };
        u8 lastTime[recordSize] = { 0 };
        TestPrecondition();

        TestStep("Unlock 0x19 Read Fail 0xC00C");
        SID10_SessionControl(DefaultSession);
        CheckPosResponse_SID10(DefaultSession);
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);
        SecurityUnlock(0x19);

        ReadAndRecordC00C(&lastCount, recordCount, recordSize, lastTime);

        SID11_ECUReset(0x81);
        CheckNonResponse();

        CheckCurrentSession(DefaultSession);

        SID10_SessionControl(DefaultSession);
        CheckPosResponse_SID10(DefaultSession);
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);
        SecurityUnlock(0x19);

        ReadAndRecordC00C(&currentCount, recordCount, recordSize, currentTime);

        TestInfoPrint("Check C00C Info");
        checkInCrease(currentCount, lastCount, currentTime, lastTime, recordSize);

        TestPostCondition();

        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_051() {
    try {
        strcpy(TestCaseName, "CS_Test_051");
        strcpy(TestScopeName, "Secure Audit Log");
        strcpy(testPurpose_CH, "测试目的：");

        u8 DID_C00C[2] = { 0xC0,0x0C };
        u8 DID_Store[272] = { 0x0 };
         
        u8 levelCount = 0;
        u8 NRC[2] = { 0x22,0xF0 };

        u32 currentCount = 0, lastCount = 0;
        const int recordCount = C00C_Count;
        const int recordSize = C00C_Size;
        u8 currentTime[recordSize] = { 0 };
        u8 lastTime[recordSize] = { 0 };
        TestPrecondition();

        TestStep("Unlock 0x19 Read Fail 0xC00C");
        SID10_SessionControl(DefaultSession);
        CheckPosResponse_SID10(DefaultSession);
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);
        SecurityUnlock(0x19);

        ReadAndRecordC00C(&lastCount, recordCount, recordSize, lastTime);

        for (int i = 0; i < 12; i++) {
            SID11_ECUReset(0x01);
            CheckPosResponse_SID11(0x01);
            AppWait(5000, "");
            CheckCurrentSession(DefaultSession);
            AppWait(60000, "");
        }

        SID10_SessionControl(DefaultSession);
        CheckPosResponse_SID10(DefaultSession);
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);
        SecurityUnlock(0x19);

        SID22_ReadDataByIdentifier(DID_C00C);
        if (0 == CheckPosResponse_SID22(DID_C00C)) {
            StoreArray(DID_Store, TpRxData, 272, 0, 0);
        }
        TestInfoPrint("Check C00C Info");
        if (0 == checkC00C(DID_Store, recordCount, recordSize)) {
            TestInfoPrintPass("Check C00C Loop Pass");
        }
        else {
            TestInfoPrintFail("Check C00C Loop Fail");
        }
        TestPostCondition();

        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_056() {
    try {
        strcpy(TestCaseName, "CS_Test_056");
        strcpy(TestScopeName, "Secure Audit Log");
        strcpy(testPurpose_CH, "测试目的：");

        TestPrecondition();
        u8 DID_C00E[2] = { 0xC0,0x0E };
        u8 DID_Store[148] = { 0x0 };

        const int recordCount = C00E_Count;
        const int recordSize = C00E_Size;
        u32 lastCount = 0,  currentCount = 0;
        u8 lastRecordTime[recordSize] = { 0 };
        u8 currentRecordTime[recordSize] = { 0 };

        TestStep("Enter Extended Session");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        SecurityUnlock(0x19);

        ReadAndRecordC00E(&lastCount, recordCount, recordSize, lastRecordTime);

        u8 D01C[2] = { 0XD0,0X1C };
        u8 C014[2] = { 0XC0,0X14 };
        u8 C01E[2] = { 0XC0,0X1E };

        TestStep("Write Public Key");
        u8 C014_Key[18] = { 0xC0,0x14,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFA };
        u8 C01E_Key[18] = { 0xC0,0x1E,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFA };

        TestStep("Read Public Key");
        //如果文件包含公钥
        SID22_ReadDataByIdentifier(D01C);
        //如果未写入公钥
        if (0 == Raw_CheckNegResponse(0x22)) {
            TestReport_StepInfo(Pass);
        }
        else if (TpRxData[2] = 0x62) {
            TestReport_HTML_StepDescription("ECU已写入公钥！", Pass);
        }
        AppWait(5000, "");
        TestStep("Write C020 Correct");
        SID2E_WriteDataById(C01E_Key, 18);
        CheckPosResponse_SID2E(C01E);
        AppWait(3000, "");
        TestStep("Write Public Correct");
        SID2E_WriteDataById(C014_Key, 18);
        CheckPosResponse_SID2E(C014);

        TestStep("Read Public Key");
        //如果文件包含公钥
        SID22_ReadDataByIdentifier(D01C);
        CheckPosResponse_SID22(D01C);

        TestStep("Enter Extended Session");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        SecurityUnlock(0x19);
        ReadAndRecordC00E(&currentCount, recordCount, recordSize, currentRecordTime);
        checkInCrease(currentCount, lastCount, currentRecordTime, lastRecordTime, recordSize);

        TestStep("Write Public Correct");
        SID2E_WriteDataById(C01E_Key, 18);
        CheckPosResponse_SID2E(C01E);

        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_057() {
    try {
        strcpy(TestCaseName, "CS_Test_057");
        strcpy(TestScopeName, "Secure Audit Log");
        strcpy(testPurpose_CH, "测试目的：");

        TestPrecondition();
        u8 DID_C00E[2] = { 0xC0,0x0E };
        u8 DID_Store[148] = { 0x0 };

        const int recordCount = C00E_Count;
        const int recordSize = C00E_Size;
        u32 lastCount = 0, currentCount = 0;
        u8 lastRecordTime[recordSize] = { 0 };
        u8 currentRecordTime[recordSize] = { 0 };

        TestStep("Enter Extended Session");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        SecurityUnlock(0x19);

        ReadAndRecordC00E(&lastCount, recordCount, recordSize, lastRecordTime);

        u8 D01C[2] = { 0XD0,0X1C };
        u8 C015[2] = { 0XC0,0X15 };
        u8 C01E[2] = { 0XC0,0X1E };

        TestStep("Write Public Key");
        u8 C015_Key[18] = { 0xC0,0x15,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFA };
        u8 C01E_Key[18] = { 0xC0,0x1E,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF };

        TestStep("Read Public Key");
        //如果文件包含公钥
        SID22_ReadDataByIdentifier(D01C);
        //如果未写入公钥
        if (0 == Raw_CheckNegResponse(0x22)) {
            TestReport_StepInfo(Pass);
        }
        else if (TpRxData[2] = 0x62) {
            TestReport_HTML_StepDescription("ECU已写入公钥！", Pass);
        }
        AppWait(5000, "");
        TestStep("Write C01E Correct");
        SID2E_WriteDataById(C01E_Key, 18);
        CheckPosResponse_SID2E(C01E);
        AppWait(3000, "");
        TestStep("Write Public Correct");
        SID2E_WriteDataById(C015_Key, 18);
        CheckPosResponse_SID2E(C015);

        TestStep("Read Public Key");
        //如果文件包含公钥
        SID22_ReadDataByIdentifier(D01C);
        CheckPosResponse_SID22(D01C);

        TestStep("Enter Extended Session");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        SecurityUnlock(0x19);
        ReadAndRecordC00E(&currentCount, recordCount, recordSize, currentRecordTime);
        checkInCrease(currentCount, lastCount, currentRecordTime, lastRecordTime, recordSize);

        //TestStep("Write Public Correct");
        //SID2E_WriteDataById(C020_Key, 18);
        //CheckPosResponse_SID2E(C020);

        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_058() {
    try {
        strcpy(TestCaseName, "CS_Test_058");
        strcpy(TestScopeName, "Secure Audit Log");
        strcpy(testPurpose_CH, "测试目的：");

        TestPrecondition();
        u8 DID_C00E[2] = { 0xC0,0x0E };
        u8 DID_Store[148] = { 0x0 };

        const int recordCount = C00E_Count;
        const int recordSize = C00E_Size;
        u32 lastCount = 0, currentCount = 0;
        u8 lastRecordTime[recordSize] = { 0 };
        u8 currentRecordTime[recordSize] = { 0 };

        TestStep("Enter Extended Session");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        SecurityUnlock(0x19);

        ReadAndRecordC00E(&lastCount, recordCount, recordSize, lastRecordTime);

        u8 D01C[2] = { 0XD0,0X1C };
        u8 C014[2] = { 0XC0,0X14 };
        u8 C01E[2] = { 0XC0,0X1E };

        TestStep("Write Public Key");
        u8 C014_Key[18] = { 0xC0,0x14,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFA };
        u8 C01E_Key[18] = { 0xC0,0x1E,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFA };

        TestStep("Read Public Key");
        //如果文件包含公钥
        SID22_ReadDataByIdentifier(D01C);
        //如果未写入公钥
        if (0 == Raw_CheckNegResponse(0x22)) {
            TestReport_StepInfo(Pass);
        }
        else if (TpRxData[2] = 0x62) {
            TestReport_HTML_StepDescription("ECU已写入公钥！", Pass);
        }
        AppWait(5000, "");
        for (int i = 0; i < 12; i++) {
            TestStep("Write C01E Correct");
            SID2E_WriteDataById(C01E_Key, 18);
            CheckPosResponse_SID2E(C01E);
            //AppWait(3000, "");
            TestStep("Write Public Correct");
            SID2E_WriteDataById(C014_Key, 18);
            CheckPosResponse_SID2E(C014);
        }

        TestStep("Read Public Key");
        //如果文件包含公钥
        SID22_ReadDataByIdentifier(D01C);
        CheckPosResponse_SID22(D01C);

        TestStep("Enter Extended Session");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        SecurityUnlock(0x19);

        if (0 == checkC00E(recordCount, recordSize)) {
            TestInfoPrintPass("Check C00E Pass");
        }
        else {
            TestInfoPrintFail("Check C00E Fail");
        }


        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_053_060() {
    try {
        strcpy(TestCaseName, "CS_Test_053_060");
        strcpy(TestScopeName, "Secure Audit Log");
        strcpy(testPurpose_CH, "测试目的：");

        TestPrecondition();
        u32 DTC = 0xD14C51;
        u8 DID_C00E[2] = { 0xC0,0x0E };
        u8 DID_C00F[2] = { 0xC0,0x0F };
        u8 DID_Store[148] = { 0x0 };

        const int recordCount = C00E_Count;
        const int recordSize = C00E_Size;
        const int recordCount_C00F = C00F_Count;
        const int recordSize_C00F = C00F_Size;
        u32 lastCount = 0, lastCount_C00F = 0, currentCount = 0, currentCount_C00F = 0;
        u8 lastRecordTime[recordSize] = { 0 };
        u8 currentRecordTime[recordSize] = { 0 };
        u8 lastRecordTime_C00F[recordSize_C00F] = { 0 };
        u8 currentRecordTime_C00F[recordSize_C00F] = { 0 };

        TestStep("Enter Extended Session");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        SecurityUnlock(0x19);

        ReadAndRecordC00E(&lastCount, recordCount, recordSize, lastRecordTime);
        ReadAndRecordC00F(&lastCount_C00F, recordCount_C00F, recordSize_C00F, lastRecordTime_C00F);

        TestStep("Enter Default Session");
        SID10_SessionControl(DefaultSession);
        CheckPosResponse_SID10(DefaultSession);

        TestStep("Enter Programming Session");
        SID10_SessionControl(ProgrammingSession);
        CheckPosResponse_SID10(ProgrammingSession);

        TestStep("Check Programming Session");
        CheckCurrentSession(ProgrammingSession);

        TestStep("Read Public Key");
        u8 D01C[2] = { 0XD0,0X1C };
        u8 F109[2] = { 0XF1,0X09 };
        //如果文件包含公钥
        SID22_ReadDataByIdentifier(D01C);
        //如果未写入公钥
        if (0 == Raw_CheckNegResponse(0x22)) {
            TestReport_StepInfo(Pass);
        }
        else if (TpRxData[2] = 0x62) {
            TestReport_HTML_StepDescription("ECU已写入公钥！", Pass);
        }

        TestStep("Downlaod SBL");
        u8 DID1[2] = { 0xED,0x20 };
        SID22_ReadDataByIdentifier(DID1);
        CheckPosResponse_SID22(DID1);

        u8 DID2[2] = { 0xF1,0xAA };
        SID22_ReadDataByIdentifier(DID2);
        CheckPosResponse_SID22(DID2);

        TestSeq_DownloadSBL(0);

        u8 DID3[2] = { 0xF1,0xA2 };
        SID22_ReadDataByIdentifier(DID3);
        CheckNegResponse(0x31);

        ActivateSBL();
        SID22_ReadDataByIdentifier(DID1);
        CheckPosResponse_SID22(DID1);

        SID22_ReadDataByIdentifier(DID3);
        CheckPosResponse_SID22(DID3, 8);

        //SendTesterPresent(TP_OFF);

        TestWait(6000);

        TestStep("Read Public Key");
        //如果文件包含公钥
        SID22_ReadDataByIdentifier(D01C);
        //如果未写入公钥
        if (0 == Raw_CheckNegResponse(0x22)) {
            TestReport_StepInfo(Pass);
        }
        else if (TpRxData[2] = 0x62) {
            TestReport_HTML_StepDescription("ECU已写入公钥！", Pass);
        }

        TestStep("Write Public Key");
        u8 F109_Key[18] = { 0xF1,0x09,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFA};

        TestStep("Security Unlock");
        SecurityUnlock(0x01);

        TestStep("Read Public Key");
        //如果文件包含公钥
        SID22_ReadDataByIdentifier(D01C);
        //如果未写入公钥
        if (0 == Raw_CheckNegResponse(0x22)) {
            TestReport_StepInfo(Pass);
        }
        else if (TpRxData[2] = 0x62) {
            TestReport_HTML_StepDescription("ECU已写入公钥！", Pass);
        }

        TestStep("Write Public Correct");
        SID2E_WriteDataById(F109_Key, 18);
        CheckPosResponse_SID2E(F109);

        TestStep("Read Public Key");
        //如果文件包含公钥
        SID22_ReadDataByIdentifier(D01C);
        CheckPosResponse_SID22(D01C);

        //-----C00E------

        TestStep("ECU Reset");
        SID11_ECUReset(0x01);
        CheckPosResponse_SID11(0x01);

        TestStep("Enter Extended Session");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        SecurityUnlock(0x19);
        ReadAndRecordC00E(&currentCount, recordCount, recordSize, currentRecordTime);
        ReadAndRecordC00F(&currentCount_C00F, recordCount_C00F, recordSize_C00F, currentRecordTime_C00F);
        checkInCrease(currentCount, lastCount, currentRecordTime, lastRecordTime, recordSize);
        checkInCrease(currentCount_C00F, lastCount_C00F, currentRecordTime_C00F, lastRecordTime_C00F, recordSize_C00F);

        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_054_061() {
    try {
        strcpy(TestCaseName, "CS_Test_054_061");
        strcpy(TestScopeName, "Secure Audit Log");
        strcpy(testPurpose_CH, "测试目的：");

        TestPrecondition();
        u32 DTC = 0xD14C51;
        u8 DID_C00E[2] = { 0xC0,0x0E };
        u8 DID_C00F[2] = { 0xC0,0x0F };
        u8 DID_Store[148] = { 0x0 };

        const int recordCount = C00E_Count;
        const int recordSize = C00E_Size;
        const int recordCount_C00F = C00F_Count;
        const int recordSize_C00F = C00F_Size;
        u32 lastCount = 0, lastCount_C00F = 0, currentCount = 0, currentCount_C00F = 0;
        u8 lastRecordTime[recordSize] = { 0 };
        u8 currentRecordTime[recordSize] = { 0 };
        u8 lastRecordTime_C00F[recordSize_C00F] = { 0 };
        u8 currentRecordTime_C00F[recordSize_C00F] = { 0 };

        TestStep("Enter Extended Session");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        SecurityUnlock(0x19);

        ReadAndRecordC00E(&lastCount, recordCount, recordSize, lastRecordTime);
        ReadAndRecordC00F(&lastCount_C00F, recordCount_C00F, recordSize_C00F, lastRecordTime_C00F);

        u8 D01C[2] = { 0XD0,0X1C };
        u8 F10A[2] = { 0XF1,0X0A };

        TestStep("Write Public Key");
        u8 F10A_Key[18] = { 0xF1,0x0A,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFA };

        TestStep("Security Unlock");
        SecurityUnlock(0x11);

        TestStep("Read Public Key");
        //如果文件包含公钥
        SID22_ReadDataByIdentifier(D01C);
        //如果未写入公钥
        if (0 == Raw_CheckNegResponse(0x22)) {
            TestReport_StepInfo(Pass);
        }
        else if (TpRxData[2] = 0x62) {
            TestReport_HTML_StepDescription("ECU已写入公钥！", Pass);
        }

        TestStep("Write Public Correct");
        SID2E_WriteDataById(F10A_Key, 18);
        CheckPosResponse_SID2E(F10A);

        TestStep("Read Public Key");
        //如果文件包含公钥
        SID22_ReadDataByIdentifier(D01C);
        CheckPosResponse_SID22(D01C);

        //-----C00E------

        TestStep("ECU Reset");
        SID11_ECUReset(0x01);
        CheckPosResponse_SID11(0x01);

        TestStep("Enter Extended Session");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        SecurityUnlock(0x19);
        ReadAndRecordC00E(&currentCount, recordCount, recordSize, currentRecordTime);
        ReadAndRecordC00F(&currentCount_C00F, recordCount_C00F, recordSize_C00F, currentRecordTime_C00F);
        checkInCrease(currentCount, lastCount, currentRecordTime, lastRecordTime, recordSize);
        checkInCrease(currentCount_C00F, lastCount_C00F, currentRecordTime_C00F, lastRecordTime_C00F, recordSize_C00F);

        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_055_062() {
    try {
        strcpy(TestCaseName, "CS_Test_055_062");
        strcpy(TestScopeName, "Secure Audit Log");
        strcpy(testPurpose_CH, "测试目的：");

        TestPrecondition();
        u32 DTC = 0xD14C51;
        u8 DID_C00E[2] = { 0xC0,0x0E };
        u8 DID_C00F[2] = { 0xC0,0x0F };
        u8 DID_Store[148] = { 0x0 };

        const int recordCount = C00E_Count;
        const int recordSize = C00E_Size;
        const int recordCount_C00F = C00F_Count;
        const int recordSize_C00F = C00F_Size;
        u32 lastCount = 0, lastCount_C00F = 0, currentCount = 0, currentCount_C00F = 0;
        u8 lastRecordTime[recordSize] = { 0 };
        u8 currentRecordTime[recordSize] = { 0 };
        u8 lastRecordTime_C00F[recordSize_C00F] = { 0 };
        u8 currentRecordTime_C00F[recordSize_C00F] = { 0 };

        TestStep("Enter Extended Session");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        SecurityUnlock(0x19);

        ReadAndRecordC00E(&lastCount, recordCount, recordSize, lastRecordTime);
        ReadAndRecordC00F(&lastCount_C00F, recordCount_C00F, recordSize_C00F, lastRecordTime_C00F);

        u8 D01C[2] = { 0XD0,0X1C };
        u8 F10B[2] = { 0XF1,0X0B };

        TestStep("Write Public Key");
        u8 F10B_Key[18] = { 0xF1,0x0B,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFA };

        TestStep("Security Unlock");
        SecurityUnlock(0x19);

        TestStep("Read Public Key");
        //如果文件包含公钥
        SID22_ReadDataByIdentifier(D01C);
        //如果未写入公钥
        if (0 == Raw_CheckNegResponse(0x22)) {
            TestReport_StepInfo(Pass);
        }
        else if (TpRxData[2] = 0x62) {
            TestReport_HTML_StepDescription("ECU已写入公钥！", Pass);
        }

        TestStep("Write Public Correct");
        SID2E_WriteDataById(F10B_Key, 18);
        CheckPosResponse_SID2E(F10B);

        TestStep("Read Public Key");
        //如果文件包含公钥
        SID22_ReadDataByIdentifier(D01C);
        CheckPosResponse_SID22(D01C);

        //-----C00E------

        TestStep("ECU Reset");
        SID11_ECUReset(0x01);
        CheckPosResponse_SID11(0x01);

        TestStep("Enter Extended Session");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        SecurityUnlock(0x19);
        ReadAndRecordC00E(&currentCount, recordCount, recordSize, currentRecordTime);
        ReadAndRecordC00F(&currentCount_C00F, recordCount_C00F, recordSize_C00F, currentRecordTime_C00F);
        checkInCrease(currentCount, lastCount, currentRecordTime, lastRecordTime, recordSize);
        checkInCrease(currentCount_C00F, lastCount_C00F, currentRecordTime_C00F, lastRecordTime_C00F, recordSize_C00F);

        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_063() {
    try {
        strcpy(TestCaseName, "CS_Test_063");
        strcpy(TestScopeName, "Secure Audit Log");
        strcpy(testPurpose_CH, "测试目的：");

        TestPrecondition();
        u8 DID_C00E[2] = { 0xC0,0x0E };
        u8 DID_Store[148] = { 0x0 };

        const int recordCount = 20;
        const int recordSize = 7;
        u32 lastCount = 0, currentCount = 0;
        u8 lastRecordTime[recordSize] = { 0 };
        u8 currentRecordTime[recordSize] = { 0 };

        TestStep("Enter Extended Session");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        SecurityUnlock(0x19);

        ReadAndRecordC00F(&lastCount, recordCount, recordSize, lastRecordTime);

        u8 D01C[2] = { 0XD0,0X1C };
        u8 C014[2] = { 0XC0,0X14 };
        u8 C01E[2] = { 0XC0,0X1E };

        TestStep("Write Public Key");
        u8 C014_Key[18] = { 0xC0,0x14,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFA };
        u8 C01E_Key[18] = { 0xC0,0x1E,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFA };

        TestStep("Read Public Key");
        //如果文件包含公钥
        SID22_ReadDataByIdentifier(D01C);
        //如果未写入公钥
        if (0 == Raw_CheckNegResponse(0x22)) {
            TestReport_StepInfo(Pass);
        }
        else if (TpRxData[2] = 0x62) {
            TestReport_HTML_StepDescription("ECU已写入公钥！", Pass);
        }
        AppWait(5000, "");
        TestStep("Write C01E Correct");
        SID2E_WriteDataById(C01E_Key, 18);
        CheckPosResponse_SID2E(C01E);
        AppWait(3000, "");
        TestStep("Write Public Correct");
        SID2E_WriteDataById(C014_Key, 18);
        CheckPosResponse_SID2E(C014);

        TestStep("Read Public Key");
        //如果文件包含公钥
        SID22_ReadDataByIdentifier(D01C);
        CheckPosResponse_SID22(D01C);

        TestStep("Enter Extended Session");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        SecurityUnlock(0x19);
        ReadAndRecordC00F(&currentCount, recordCount, recordSize, currentRecordTime);
        checkInCrease(currentCount, lastCount, currentRecordTime, lastRecordTime, recordSize);

        TestStep("Write Public Correct");
        SID2E_WriteDataById(C01E_Key, 18);
        CheckPosResponse_SID2E(C01E);

        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_064() {
    try {
        strcpy(TestCaseName, "CS_Test_064");
        strcpy(TestScopeName, "Secure Audit Log");
        strcpy(testPurpose_CH, "测试目的：");

        TestPrecondition();
        u8 DID_C00F[2] = { 0xC0,0x0F };
        u8 DID_Store[188] = { 0x0 };

        const int recordCount = 20;
        const int recordSize = 9;
        u32 lastCount = 0, currentCount = 0;
        u8 lastRecordTime[recordSize] = { 0 };
        u8 currentRecordTime[recordSize] = { 0 };

        TestStep("Enter Extended Session");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        SecurityUnlock(0x19);

        ReadAndRecordC00F(&lastCount, recordCount, recordSize, lastRecordTime);

        u8 D01C[2] = { 0XD0,0X1C };
        u8 C015[2] = { 0XC0,0X15 };
        u8 C01E[2] = { 0XC0,0X1E };

        TestStep("Write Public Key");
        u8 C015_Key[18] = { 0xC0,0x15,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFA };
        u8 C01E_Key[18] = { 0xC0,0x1E,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF };

        TestStep("Read Public Key");
        //如果文件包含公钥
        SID22_ReadDataByIdentifier(D01C);
        //如果未写入公钥
        if (0 == Raw_CheckNegResponse(0x22)) {
            TestReport_StepInfo(Pass);
        }
        else if (TpRxData[2] = 0x62) {
            TestReport_HTML_StepDescription("ECU已写入公钥！", Pass);
        }
        AppWait(5000, "");
        TestStep("Write C01E Correct");
        SID2E_WriteDataById(C01E_Key, 18);
        CheckPosResponse_SID2E(C01E);
        AppWait(3000, "");
        TestStep("Write Public Correct");
        SID2E_WriteDataById(C015_Key, 18);
        CheckPosResponse_SID2E(C015);

        TestStep("Read Public Key");
        //如果文件包含公钥
        SID22_ReadDataByIdentifier(D01C);
        CheckPosResponse_SID22(D01C);

        TestStep("Enter Extended Session");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        SecurityUnlock(0x19);
        ReadAndRecordC00F(&currentCount, recordCount, recordSize, currentRecordTime);
        checkInCrease(currentCount, lastCount, currentRecordTime, lastRecordTime, recordSize);

        //TestStep("Write Public Correct");
        //SID2E_WriteDataById(C020_Key, 18);
        //CheckPosResponse_SID2E(C020);

        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_065() {
    try {
        strcpy(TestCaseName, "CS_Test_065");
        strcpy(TestScopeName, "Secure Audit Log");
        strcpy(testPurpose_CH, "测试目的：");

        TestPrecondition();
        u8 DID_C00F[2] = { 0xC0,0x0F };
        u8 DID_Store[188] = { 0x0 };

        const int recordSize = C00E_Size;
        const int recordCount = C012_Count;
        u32 lastCount = 0, currentCount = 0;
        u8 lastRecordTime[recordSize] = { 0 };
        u8 currentRecordTime[recordSize] = { 0 };

        TestStep("Enter Extended Session");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        SecurityUnlock(0x19);

        ReadAndRecordC00F(&lastCount, recordCount, recordSize, lastRecordTime);

        u8 D01C[2] = { 0XD0,0X1C };
        u8 C014[2] = { 0XC0,0X14 };
        u8 C01E[2] = { 0XC0,0X1E };

        TestStep("Write Public Key");
        u8 C014_Key[18] = { 0xC0,0x14,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFA };
        u8 C01E_Key[18] = { 0xC0,0x1E,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFA };


        TestStep("Read Public Key");
        //如果文件包含公钥
        SID22_ReadDataByIdentifier(D01C);
        //如果未写入公钥
        if (0 == Raw_CheckNegResponse(0x22)) {
            TestReport_StepInfo(Pass);
        }
        else if (TpRxData[2] = 0x62) {
            TestReport_HTML_StepDescription("ECU已写入公钥！", Pass);
        }
        AppWait(5000, "");
        for (int i = 0; i < 12; i++) {
            TestStep("Write C01E Correct");
            SID2E_WriteDataById(C01E_Key, 18);
            CheckPosResponse_SID2E(C01E);
            //AppWait(3000, "");
            TestStep("Write Public Correct");
            SID2E_WriteDataById(C014_Key, 18);
            CheckPosResponse_SID2E(C014);
        }

        TestStep("Read Public Key");
        //如果文件包含公钥
        SID22_ReadDataByIdentifier(D01C);
        CheckPosResponse_SID22(D01C);

        if (0 == checkC00F(recordCount, recordSize)) {
            TestInfoPrintPass("Check C00F Pass");
        }
        else {
            TestInfoPrintFail("Check C00F Fail");
        }


        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_066() {
    try {
        strcpy(TestCaseName, "CS_Test_066");
        strcpy(TestScopeName, "Secure Audit Log");
        strcpy(testPurpose_CH, "测试目的：");

        u8 ED20[2] = { 0xED,0x20 };
        u8 F1AA[2] = { 0xF1,0xAA };
        u8 DID_C012[2] = { 0xC0,0x12 };

        u32 currentCountPass = 0, currentCountFail = 0, lastCountPass = 0, lastCountFail = 0;
        const int recordSize = C012_Size;
        const int recordCount = C012_Count;
        u8 currentTimePass[recordSize] = { 0 };
        u8 currentTimeFail[recordSize] = { 0 };
        u8 lastTimePass[recordSize] = { 0 };
        u8 lastTimeFail[recordSize] = { 0 };

        TestPrecondition();

        TestStep("Enter Extended Session");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        SecurityUnlock(0x19);

        PassReadAndRecordC012(&lastCountPass, recordCount, recordSize, lastTimePass);
        FailReadAndRecordC012(&lastCountFail, recordCount, recordSize, lastTimeFail);

        //step18
        TestStep("Enter programming session unlock ECU and if SBL is supported, SBL should be downloaded and actived");
        SID10_SessionControl(ProgrammingSession);
        CheckPosResponse_SID10(ProgrammingSession);

        TestWait(5000);

        TestSeq_DownloadSBL();

        //step19
        TestReport_HTML_StepDescription("----Download APP----");

        char keyvalue[20] = "APP0Path_Production";

        for (int i = 0; i < APPFileNum; i++)
        {
            char StepDespriction[256];
            char filename[300] = {};
            vbfparser(keyvalue);
            filename[0] = '\0';
            if (app.get_system_var_string("Diagnostic0.下载文件_DataFile", 512, filename) != 0 || filename[0] == '\0') {
                log("Error: Failed to get filename for %s", keyvalue);
                snprintf(StepDespriction, sizeof(StepDespriction), "Download APP%d (no filename)", i);
            }
            else {
                size_t len = strlen(filename);
                if (len >= 9) {
                    filename[len - 9] = '\0';
                    snprintf(StepDespriction, sizeof(StepDespriction), "Download APP is %s", filename);
                }
                else {
                    log("Warning: Filename '%s' too short (len=%zu)", filename, len);
                    snprintf(StepDespriction, sizeof(StepDespriction), "Download APP%d (short filename)", i);
                }
            }
            TestReport_HTML_StepDescription(StepDespriction);
            EraseAddress();
            FileDownload_with_VBT_Hash(0, 1);

            //step23
            TestStep("Check sw_signature");
            CheckSignature();

            keyvalue[3]++;
        }
        //step24
        TestStep("Check Complete And Compatible");
        CheckCompleteAndCompatible();

        SID11_ECUReset(0x81, 1);
        AppWait(5000, "");
        SID10_SessionControl(ProgrammingSession);
        CheckPosResponse_SID10(ProgrammingSession);
        TestSeq_DownloadSBL();

        keyvalue[3] = '0';

        for (int i = 0; i < APPFileNum; i++)
        {
            char StepDespriction[256];
            char filename[300] = {};
            vbfparser(keyvalue);
            filename[0] = '\0';
            if (app.get_system_var_string("Diagnostic0.下载文件_DataFile", 512, filename) != 0 || filename[0] == '\0') {
                log("Error: Failed to get filename for %s", keyvalue);
                snprintf(StepDespriction, sizeof(StepDespriction), "Download APP%d (no filename)", i);
            }
            else {
                size_t len = strlen(filename);
                if (len >= 9) {
                    filename[len - 9] = '\0';
                    snprintf(StepDespriction, sizeof(StepDespriction), "Download APP is %s", filename);
                }
                else {
                    log("Warning: Filename '%s' too short (len=%zu)", filename, len);
                    snprintf(StepDespriction, sizeof(StepDespriction), "Download APP%d (short filename)", i);
                }
            }
            TestReport_HTML_StepDescription(StepDespriction);
            EraseAddress();
            FileDownload();

            //step23
            TestStep("Check sw_signature");
            CheckSignature();

            keyvalue[3]++;
        }
        //step24
        TestStep("Check Complete And Compatible");
        CheckCompleteAndCompatible();

        SID11_ECUReset(0x81, Functional);
        CheckNonResponse();

        TestStep("Enter Extended Session");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        SecurityUnlock(0x19);

        PassReadAndRecordC012(&currentCountPass, recordCount, recordSize, currentTimePass);
        FailReadAndRecordC012(&currentCountFail, recordCount, recordSize, currentTimeFail);

        TestInfoPrint("Check Pass Info");
        checkInCrease(currentCountPass, lastCountPass, currentTimePass, lastTimePass, recordSize,0);
        TestInfoPrint("Check Fail Info");
        checkInCrease(currentCountFail, lastCountFail, currentTimeFail, lastTimeFail, recordSize);

        //step26
        stepnumber = 25;
        TestStep("ECU Reset");
        SID11_ECUReset(0x81, Functional);

        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_067() {
    try {
        strcpy(TestCaseName, "CS_Test_067");
        strcpy(TestScopeName, "Secure Audit Log");
        strcpy(testPurpose_CH, "测试目的：");

        u8 ED20[2] = { 0xED,0x20 };
        u8 F1AA[2] = { 0xF1,0xAA };
        u8 DID_C012[2] = { 0xC0,0x12 };

        u32 currentCountPass = 0, currentCountFail = 0, lastCountPass = 0, lastCountFail = 0;
        const int recordSize = C012_Size;
        const int recordCount = C012_Count;
        u8 currentTimePass[recordSize] = { 0 };
        u8 currentTimeFail[recordSize] = { 0 };
        u8 lastTimePass[recordSize] = { 0 };
        u8 lastTimeFail[recordSize] = { 0 };

        TestPrecondition();

        TestStep("Enter Extended Session");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        SecurityUnlock(0x19);

        PassReadAndRecordC012(&lastCountPass, recordCount, recordSize, lastTimePass);
        FailReadAndRecordC012(&lastCountFail, recordCount, recordSize, lastTimeFail);

        //step18
        TestStep("Enter programming session unlock ECU and if SBL is supported, SBL should be downloaded and actived");
        SID10_SessionControl(ProgrammingSession);
        CheckPosResponse_SID10(ProgrammingSession);

        TestWait(5000);

        TestSeq_DownloadSBL();

        //step19
        TestReport_HTML_StepDescription("----Download APP----");

        char keyvalue[30] = "APP0Path_Production";

        for (int i = 0; i < APPFileNum; i++)
        {
            char StepDespriction[256];
            char filename[300] = {};
            vbfparser(keyvalue);
            filename[0] = '\0';
            if (app.get_system_var_string("Diagnostic0.下载文件_DataFile", 512, filename) != 0 || filename[0] == '\0') {
                log("Error: Failed to get filename for %s", keyvalue);
                snprintf(StepDespriction, sizeof(StepDespriction), "Download APP%d (no filename)", i);
            }
            else {
                size_t len = strlen(filename);
                if (len >= 9) {
                    filename[len - 9] = '\0';
                    snprintf(StepDespriction, sizeof(StepDespriction), "Download APP is %s", filename);
                }
                else {
                    log("Warning: Filename '%s' too short (len=%zu)", filename, len);
                    snprintf(StepDespriction, sizeof(StepDespriction), "Download APP%d (short filename)", i);
                }
            }
            TestReport_HTML_StepDescription(StepDespriction);
            EraseAddress();
            FileDownload();

            //step23
            TestStep("Check InValid sw_signature");
            CheckSignatureInvalid();

            keyvalue[3]++;
        }

        SID11_ECUReset(0x81, 1);
        AppWait(5000, "");
        SID10_SessionControl(ProgrammingSession);
        CheckPosResponse_SID10(ProgrammingSession);
        TestSeq_DownloadSBL();

        //TestSeq_DownloadAPP();
                //step19
        TestReport_HTML_StepDescription("----Download APP----");

        //char keyvalue[30] = "APP0Path_Production";
        keyvalue[3] = '0';
        for (int i = 0; i < APPFileNum; i++)
        {
            char StepDespriction[256];
            char filename[300] = {};
            vbfparser(keyvalue);
            filename[0] = '\0';
            if (app.get_system_var_string("Diagnostic0.下载文件_DataFile", 512, filename) != 0 || filename[0] == '\0') {
                log("Error: Failed to get filename for %s", keyvalue);
                snprintf(StepDespriction, sizeof(StepDespriction), "Download APP%d (no filename)", i);
            }
            else {
                size_t len = strlen(filename);
                if (len >= 9) {
                    filename[len - 9] = '\0';
                    snprintf(StepDespriction, sizeof(StepDespriction), "Download APP is %s", filename);
                }
                else {
                    log("Warning: Filename '%s' too short (len=%zu)", filename, len);
                    snprintf(StepDespriction, sizeof(StepDespriction), "Download APP%d (short filename)", i);
                }
            }
            TestReport_HTML_StepDescription(StepDespriction);
            EraseAddress();
            FileDownload();

            //step23
            TestStep("Check sw_signature");
            CheckSignature();
            keyvalue[3]++;
        }

        TestStep("Check Complete And Compatible");
        CheckCompleteAndCompatible();

        SID10_SessionControl(DefaultSession);
        CheckPosResponse_SID10(DefaultSession);
        AppWait(1000, "");
        TestStep("Enter Extended Session");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        SecurityUnlock(0x19);

        PassReadAndRecordC012(&currentCountPass, recordCount, recordSize, currentTimePass);
        FailReadAndRecordC012(&currentCountFail, recordCount, recordSize, currentTimeFail);

        TestInfoPrint("Check Pass Info");
        checkInCrease(currentCountPass, lastCountPass, currentTimePass, lastTimePass, recordSize);
        TestInfoPrint("Check Fail Info");
        checkInCrease(currentCountFail, lastCountFail, currentTimeFail, lastTimeFail, recordSize);

        //step26
        stepnumber = 25;
        TestStep("ECU Reset");
        SID11_ECUReset(0x81, Functional);

        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_068() {
    try {
        strcpy(TestCaseName, "CS_Test_068");
        strcpy(TestScopeName, "Secure Audit Log");
        strcpy(testPurpose_CH, "测试目的：");

        u8 ED20[2] = { 0xED,0x20 };
        u8 F1AA[2] = { 0xF1,0xAA };
        u8 DID_C012[2] = { 0xC0,0x12 };

        u32 currentCountPass = 0, currentCountFail = 0, lastCountPass = 0, lastCountFail = 0;
        const int recordSize = C012_Size;
        const int recordCount = C012_Count;
        u8 currentTimePass[recordSize] = { 0 };
        u8 currentTimeFail[recordSize] = { 0 };
        u8 lastTimePass[recordSize] = { 0 };
        u8 lastTimeFail[recordSize] = { 0 };

        TestPrecondition();

        TestStep("Enter Extended Session");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        SecurityUnlock(0x19);

        PassReadAndRecordC012(&lastCountPass, recordCount, recordSize, lastTimePass);
        FailReadAndRecordC012(&lastCountFail, recordCount, recordSize, lastTimeFail);

        //step18
        TestStep("Enter programming session unlock ECU and if SBL is supported, SBL should be downloaded and actived");
        SID10_SessionControl(ProgrammingSession);
        CheckPosResponse_SID10(ProgrammingSession);

        TestWait(5000);

        TestSeq_DownloadSBL();

        //step19
        TestReport_HTML_StepDescription("----Download APP----");

        char keyvalue[20] = "APP0Path_Production";

        for (int i = 0; i < APPFileNum; i++)
        {
            char StepDespriction[256];
            char filename[300] = {};
            vbfparser(keyvalue);
            filename[0] = '\0';
            if (app.get_system_var_string("Diagnostic0.下载文件_DataFile", 512, filename) != 0 || filename[0] == '\0') {
                log("Error: Failed to get filename for %s", keyvalue);
                snprintf(StepDespriction, sizeof(StepDespriction), "Download APP%d (no filename)", i);
            }
            else {
                size_t len = strlen(filename);
                if (len >= 9) {
                    filename[len - 9] = '\0';
                    snprintf(StepDespriction, sizeof(StepDespriction), "Download APP is %s", filename);
                }
                else {
                    log("Warning: Filename '%s' too short (len=%zu)", filename, len);
                    snprintf(StepDespriction, sizeof(StepDespriction), "Download APP%d (short filename)", i);
                }
            }
            TestReport_HTML_StepDescription(StepDespriction);
            EraseAddress();
            FileDownload();

            //step23
            TestStep("Check sw_signature");
            CheckSignature();

            keyvalue[3]++;
        }
        //step24
        TestStep("Check Complete And Compatible");
        CheckCompleteAndCompatible();

        SID10_SessionControl(DefaultSession);
        CheckPosResponse_SID10(DefaultSession);
        AppWait(1000, "");
        TestStep("Enter Extended Session");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        SecurityUnlock(0x19);

        PassReadAndRecordC012(&currentCountPass, recordCount, recordSize, currentTimePass);
        FailReadAndRecordC012(&currentCountFail, recordCount, recordSize, currentTimeFail);

        TestInfoPrint("Check Pass Info");
        checkInCrease(currentCountPass, lastCountPass, currentTimePass, lastTimePass, recordSize);
        TestInfoPrint("Check Fail Info");
        checkInCrease(currentCountFail, lastCountFail, currentTimeFail, lastTimeFail, recordSize,0);

        //step26
        stepnumber = 25;
        TestStep("ECU Reset");
        SID11_ECUReset(0x81, Functional);

        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_069() {
    try {
        strcpy(TestCaseName, "CS_Test_069");
        strcpy(TestScopeName, "Secure Audit Log");
        strcpy(testPurpose_CH, "测试目的：");

        u8 ED20[2] = { 0xED,0x20 };
        u8 F1AA[2] = { 0xF1,0xAA };
        u8 DID_C012[2] = { 0xC0,0x12 };

        u32 currentCountPass = 0, currentCountFail = 0, lastCountPass = 0, lastCountFail = 0;
        const int recordSize = C012_Size;
        const int recordCount = C012_Count;
        u8 currentTimePass[recordSize] = { 0 };
        u8 currentTimeFail[recordSize] = { 0 };
        u8 lastTimePass[recordSize] = { 0 };
        u8 lastTimeFail[recordSize] = { 0 };

        TestPrecondition();

        TestStep("Enter Extended Session");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        SecurityUnlock(0x19);

        PassReadAndRecordC012(&lastCountPass, recordCount, recordSize, lastTimePass);
        FailReadAndRecordC012(&lastCountFail, recordCount, recordSize, lastTimeFail);

        for (int i = 0; i < 12; i++) {
            //step18
            TestStep("Enter programming session unlock ECU and if SBL is supported, SBL should be downloaded and actived");
            SID10_SessionControl(ProgrammingSession);
            CheckPosResponse_SID10(ProgrammingSession);

            TestWait(5000);

            TestSeq_DownloadSBL();

            //step19
            TestReport_HTML_StepDescription("----Download APP----");

            char keyvalue[20] = "APP0Path_Production";

            for (int i = 0; i < APPFileNum; i++)
            {
                char StepDespriction[256];
                char filename[300] = {};
                vbfparser(keyvalue);
                filename[0] = '\0';
                if (app.get_system_var_string("Diagnostic0.下载文件_DataFile", 512, filename) != 0 || filename[0] == '\0') {
                    log("Error: Failed to get filename for %s", keyvalue);
                    snprintf(StepDespriction, sizeof(StepDespriction), "Download APP%d (no filename)", i);
                }
                else {
                    size_t len = strlen(filename);
                    if (len >= 9) {
                        filename[len - 9] = '\0';
                        snprintf(StepDespriction, sizeof(StepDespriction), "Abnormal Download APP is %s", filename);
                    }
                    else {
                        log("Warning: Filename '%s' too short (len=%zu)", filename, len);
                        snprintf(StepDespriction, sizeof(StepDespriction), "Download APP%d (short filename)", i);
                    }
                }
                TestReport_HTML_StepDescription(StepDespriction);
                EraseAddress();
                FileDownload();
                TestStep("Check InValid sw_signature");
                CheckSignatureInvalid();
                keyvalue[3]++;
            }
            keyvalue[3] = '0';
            for (int i = 0; i < APPFileNum; i++)
            {
                char StepDespriction[256];
                char filename[300] = {};
                vbfparser(keyvalue);
                filename[0] = '\0';
                if (app.get_system_var_string("Diagnostic0.下载文件_DataFile", 512, filename) != 0 || filename[0] == '\0') {
                    log("Error: Failed to get filename for %s", keyvalue);
                    snprintf(StepDespriction, sizeof(StepDespriction), "Download APP%d (no filename)", i);
                }
                else {
                    size_t len = strlen(filename);
                    if (len >= 9) {
                        filename[len - 9] = '\0';
                        snprintf(StepDespriction, sizeof(StepDespriction), "Normal Download APP is %s", filename);
                    }
                    else {
                        log("Warning: Filename '%s' too short (len=%zu)", filename, len);
                        snprintf(StepDespriction, sizeof(StepDespriction), "Download APP%d (short filename)", i);
                    }
                }
                TestReport_HTML_StepDescription(StepDespriction);
                EraseAddress();
                FileDownload();

                //step23
                TestStep("Check sw_signature");
                CheckSignature();

                keyvalue[3]++;
            }

            TestStep("Check Complete And Compatible");
            CheckCompleteAndCompatible();

            TestStep("ECU Reset");
            SID11_ECUReset(0x81, Functional);
            TestWait(6000);
        }
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        SecurityUnlock(0x19);

        PassReadAndRecordC012(&currentCountPass, recordCount, recordSize, currentTimePass);
        FailReadAndRecordC012(&currentCountFail, recordCount, recordSize, currentTimeFail);

        TestInfoPrint("Check Pass Info");
        if (0 == checkC012(true)) {
            TestInfoPrintPass("Check Pass Info Pass");
        }
        else {
            TestInfoPrintFail("Check Pass Info Fail");
        }
        TestInfoPrint("Check Fail Info");
        if (0 == checkC012(false)) {
            TestInfoPrintPass("Check Fail Info Pass");
        }
        else {
            TestInfoPrintFail("Check Fail Info Fail");
        }

        //step26
        stepnumber = 25;
        TestStep("ECU Reset");
        SID11_ECUReset(0x81, Functional);

        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_070() {
    try {
        strcpy(TestCaseName, "CS_Test_070");
        strcpy(TestScopeName, "Secure Audit Log");
        strcpy(testPurpose_CH, "测试目的：");

        u8 ED20[2] = { 0xED,0x20 };
        u8 F1AA[2] = { 0xF1,0xAA };
        u8 DID_C012[2] = { 0xC0,0x12 };

        u32 currentCountPass = 0, currentCountFail = 0, lastCountPass = 0, lastCountFail = 0;
        const int recordSize = C00A_Size;
        const int recordCount = C00A_Count;
        u8 currentTimePass[recordSize] = { 0 };
        u8 currentTimeFail[recordSize] = { 0 };
        u8 lastTimePass[recordSize] = { 0 };
        u8 lastTimeFail[recordSize] = { 0 };

        TestPrecondition();

        TestStep("Enter Extended Session");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        SecurityUnlock(0x19);

        PassReadAndRecordC00A(&lastCountPass, recordCount, recordSize, lastTimePass);
        FailReadAndRecordC00A(&lastCountFail, recordCount, recordSize, lastTimeFail);

        NM_NextOperation_NoSwitch();

        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        SecurityUnlock(0x19);

        PassReadAndRecordC00A(&currentCountPass, recordCount, recordSize, currentTimePass);
        FailReadAndRecordC00A(&currentCountFail, recordCount, recordSize, currentTimeFail);

        TestInfoPrint("Check Pass Info");
        checkInCrease(currentCountPass, lastCountPass, currentTimePass, lastTimePass, recordSize,0);
        TestInfoPrint("Check Fail Info");
        checkInCrease(currentCountFail, lastCountFail, currentTimeFail, lastTimeFail, recordSize,0);

        MainPower_OFF();
        AppWait(5000, "");
        MainPower_ON();
        AppWait(3000, "");

        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        SecurityUnlock(0x19);

        PassReadAndRecordC00A(&lastCountPass, recordCount, recordSize, lastTimePass);
        FailReadAndRecordC00A(&lastCountFail, recordCount, recordSize, lastTimeFail);

        TestInfoPrint("Check Pass Info");
        checkInCrease(lastCountPass, currentCountPass, lastTimePass, currentTimePass , recordSize, 0);
        TestInfoPrint("Check Fail Info");
        checkInCrease(lastCountFail , currentCountFail, lastTimeFail, currentTimeFail , recordSize, 0);

        //step26
        stepnumber = 25;
        TestStep("ECU Reset");
        SID11_ECUReset(0x81, Functional);

        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_071() {
    try {
        strcpy(TestCaseName, "CS_Test_071");
        strcpy(TestScopeName, "Secure Audit Log");
        strcpy(testPurpose_CH, "测试目的：");

        u8 ED20[2] = { 0xED,0x20 };
        u8 F1AA[2] = { 0xF1,0xAA };
        u8 DID_C012[2] = { 0xC0,0x12 };

        u32 currentCount = 0,lastCount = 0;
        const int recordSize = C00B_Size;
        const int recordCount = C00B_Count;
        u8 currentTime[recordSize] = { 0 };
        u8 lastTime[recordSize] = { 0 };

        TestPrecondition();

        TestStep("Enter Extended Session");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        SecurityUnlock(0x19);

        ReadAndRecordC00B(&lastCount, recordCount, recordSize, lastTime);

        NM_NextOperation_NoSwitch();

        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        SecurityUnlock(0x19);

        ReadAndRecordC00B(&currentCount, recordCount, recordSize, currentTime);

        TestInfoPrint("Check Info");
        checkInCrease(currentCount, lastCount, currentTime, lastTime, recordSize, 0);

        MainPower_OFF();
        AppWait(5000, "");
        MainPower_ON();
        AppWait(3000, "");

        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        SecurityUnlock(0x19);

        ReadAndRecordC00B(&lastCount, recordCount, recordSize, lastTime);

        TestInfoPrint("Check Info");
        checkInCrease(lastCount, currentCount, lastTime, currentTime, recordSize, 0);

        //step26
        stepnumber = 25;
        TestStep("ECU Reset");
        SID11_ECUReset(0x81, Functional);

        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_072() {
    try {
        strcpy(TestCaseName, "CS_Test_072");
        strcpy(TestScopeName, "Secure Audit Log");
        strcpy(testPurpose_CH, "测试目的：");

        u8 ED20[2] = { 0xED,0x20 };
        u8 F1AA[2] = { 0xF1,0xAA };
        u8 DID_C012[2] = { 0xC0,0x12 };

        u32 currentCount = 0, lastCount = 0;
        const int recordSize = C00C_Size;
        const int recordCount = C00C_Count;
        u8 currentTime[recordSize] = { 0 };
        u8 lastTime[recordSize] = { 0 };

        TestPrecondition();

        TestStep("Enter Extended Session");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        SecurityUnlock(0x19);

        ReadAndRecordC00C(&lastCount, recordCount, recordSize, lastTime);

        NM_NextOperation_NoSwitch();

        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        SecurityUnlock(0x19);

        ReadAndRecordC00C(&currentCount, recordCount, recordSize, currentTime);

        TestInfoPrint("Check Info");
        checkInCrease(currentCount, lastCount, currentTime, lastTime, recordSize, 0);

        MainPower_OFF();
        AppWait(5000, "");
        MainPower_ON();
        AppWait(3000, "");

        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        SecurityUnlock(0x19);

        ReadAndRecordC00C(&lastCount, recordCount, recordSize, lastTime);

        TestInfoPrint("Check Info");
        checkInCrease(lastCount, currentCount, lastTime, currentTime, recordSize, 0);

        //step26
        stepnumber = 25;
        TestStep("ECU Reset");
        SID11_ECUReset(0x81, Functional);

        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_073() {
    try {
        strcpy(TestCaseName, "CS_Test_073");
        strcpy(TestScopeName, "Secure Audit Log");
        strcpy(testPurpose_CH, "测试目的：");

        u8 ED20[2] = { 0xED,0x20 };
        u8 F1AA[2] = { 0xF1,0xAA };
        u8 DID_C012[2] = { 0xC0,0x12 };

        u32 currentCount = 0, lastCount = 0;
        const int recordSize = C00E_Size;
        const int recordCount = C00E_Count;
        u8 currentTime[recordSize] = { 0 };
        u8 lastTime[recordSize] = { 0 };

        TestPrecondition();

        TestStep("Enter Extended Session");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        SecurityUnlock(0x19);

        ReadAndRecordC00E(&lastCount, recordCount, recordSize, lastTime);

        NM_NextOperation_NoSwitch();

        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        SecurityUnlock(0x19);

        ReadAndRecordC00E(&currentCount, recordCount, recordSize, currentTime);

        TestInfoPrint("Check Info");
        checkInCrease(currentCount, lastCount, currentTime, lastTime, recordSize, 0);

        MainPower_OFF();
        AppWait(5000, "");
        MainPower_ON();
        AppWait(3000, "");

        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        SecurityUnlock(0x19);

        ReadAndRecordC00E(&lastCount, recordCount, recordSize, lastTime);

        TestInfoPrint("Check Info");
        checkInCrease(lastCount, currentCount, lastTime, currentTime, recordSize, 0);

        //step26
        stepnumber = 25;
        TestStep("ECU Reset");
        SID11_ECUReset(0x81, Functional);

        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_074() {
    try {
        strcpy(TestCaseName, "CS_Test_074");
        strcpy(TestScopeName, "Secure Audit Log");
        strcpy(testPurpose_CH, "测试目的：");

        u8 ED20[2] = { 0xED,0x20 };
        u8 F1AA[2] = { 0xF1,0xAA };
        u8 DID_C012[2] = { 0xC0,0x12 };

        u32 currentCount = 0, lastCount = 0;
        const int recordSize = C00F_Size;
        const int recordCount = C00F_Count;
        u8 currentTime[recordSize] = { 0 };
        u8 lastTime[recordSize] = { 0 };

        TestPrecondition();

        TestStep("Enter Extended Session");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        SecurityUnlock(0x19);

        ReadAndRecordC00F(&lastCount, recordCount, recordSize, lastTime);

        NM_NextOperation_NoSwitch();

        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        SecurityUnlock(0x19);

        ReadAndRecordC00F(&currentCount, recordCount, recordSize, currentTime);

        TestInfoPrint("Check Info");
        checkInCrease(currentCount, lastCount, currentTime, lastTime, recordSize, 0);

        MainPower_OFF();
        AppWait(5000, "");
        MainPower_ON();
        AppWait(3000, "");

        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        SecurityUnlock(0x19);

        ReadAndRecordC00F(&lastCount, recordCount, recordSize, lastTime);

        TestInfoPrint("Check Info");
        checkInCrease(lastCount, currentCount, lastTime, currentTime, recordSize, 0);

        //step26
        stepnumber = 25;
        TestStep("ECU Reset");
        SID11_ECUReset(0x81, Functional);

        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_075() {
    try {
        strcpy(TestCaseName, "CS_Test_075");
        strcpy(TestScopeName, "Secure Audit Log");
        strcpy(testPurpose_CH, "测试目的：");

        u8 ED20[2] = { 0xED,0x20 };
        u8 F1AA[2] = { 0xF1,0xAA };
        u8 DID_C012[2] = { 0xC0,0x12 };

        u32 currentCountPass = 0, currentCountFail = 0, lastCountPass = 0, lastCountFail = 0;
        const int recordSize = C012_Size;
        const int recordCount = C012_Count;
        u8 currentTimePass[recordSize] = { 0 };
        u8 currentTimeFail[recordSize] = { 0 };
        u8 lastTimePass[recordSize] = { 0 };
        u8 lastTimeFail[recordSize] = { 0 };

        TestPrecondition();

        TestStep("Enter Extended Session");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        SecurityUnlock(0x19);

        PassReadAndRecordC012(&lastCountPass, recordCount, recordSize, lastTimePass);
        FailReadAndRecordC012(&lastCountFail, recordCount, recordSize, lastTimeFail);

        NM_NextOperation_NoSwitch();

        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        SecurityUnlock(0x19);

        PassReadAndRecordC012(&currentCountPass, recordCount, recordSize, currentTimePass);
        FailReadAndRecordC012(&currentCountFail, recordCount, recordSize, currentTimeFail);

        TestInfoPrint("Check Pass Info");
        checkInCrease(currentCountPass, lastCountPass, currentTimePass, lastTimePass, recordSize, 0);
        TestInfoPrint("Check Fail Info");
        checkInCrease(currentCountFail, lastCountFail, currentTimeFail, lastTimeFail, recordSize, 0);

        MainPower_OFF();
        AppWait(5000, "");
        MainPower_ON();
        AppWait(3000, "");

        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        SecurityUnlock(0x19);

        PassReadAndRecordC012(&lastCountPass, recordCount, recordSize, lastTimePass);
        FailReadAndRecordC012(&lastCountFail, recordCount, recordSize, lastTimeFail);

        TestInfoPrint("Check Pass Info");
        checkInCrease(lastCountPass, currentCountPass, lastTimePass, currentTimePass, recordSize, 0);
        TestInfoPrint("Check Fail Info");
        checkInCrease(lastCountFail, currentCountFail, lastTimeFail, currentTimeFail, recordSize, 0);

        //step26
        stepnumber = 25;
        TestStep("ECU Reset");
        SID11_ECUReset(0x81, Functional);

        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_076() {
    try {
        strcpy(TestCaseName, "CS_Test_076");
        strcpy(TestScopeName, "Secure Audit Log");
        strcpy(testPurpose_CH, "测试目的：");

        u8 ED20[2] = { 0xED,0x20 };
        u8 F1AA[2] = { 0xF1,0xAA };
        u8 DID_C00A[2] = { 0xC0,0x0A };

        u8 NRC[2] = { 0x7F,0x7E };

        TestPrecondition();

        TestStep("Enter Programming Session");
        SID10_SessionControl(ProgrammingSession);
        CheckPosResponse_SID10(ProgrammingSession);

        for(int i = 0;i < 12;i++)
        {
            SID22_ReadDataByIdentifier(DID_C00A);
            CheckNegResponse(NRC, 2);
        }

        TestStep("Enter Programming Session");
        SID10_SessionControl(ProgrammingSession);
        CheckPosResponse_SID10(ProgrammingSession);

        SecurityUnlock(0x01);

        for (int i = 0; i < 12; i++)
        {
            SID22_ReadDataByIdentifier(DID_C00A);
            CheckNegResponse(NRC, 2);
        }


        TestStep("ECU Reset");
        SID11_ECUReset(0x81, Functional);

        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_077() {
    try {
        strcpy(TestCaseName, "CS_Test_077");
        strcpy(TestScopeName, "Secure Audit Log");
        strcpy(testPurpose_CH, "测试目的：");

        u8 ED20[2] = { 0xED,0x20 };
        u8 F1AA[2] = { 0xF1,0xAA };
        u8 DID_C00B[2] = { 0xC0,0x0B };

        u8 NRC[2] = { 0x7F,0x7E };

        TestPrecondition();

        TestStep("Enter Programming Session");
        SID10_SessionControl(ProgrammingSession);
        CheckPosResponse_SID10(ProgrammingSession);

        for (int i = 0; i < 12; i++)
        {
            SID22_ReadDataByIdentifier(DID_C00B);
            CheckNegResponse(NRC, 2);
        }

        TestStep("Enter Programming Session");
        SID10_SessionControl(ProgrammingSession);
        CheckPosResponse_SID10(ProgrammingSession);

        SecurityUnlock(0x01);

        for (int i = 0; i < 12; i++)
        {
            SID22_ReadDataByIdentifier(DID_C00B);
            CheckNegResponse(NRC, 2);
        }


        TestStep("ECU Reset");
        SID11_ECUReset(0x81, Functional);

        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_078() {
    try {
        strcpy(TestCaseName, "CS_Test_078");
        strcpy(TestScopeName, "Secure Audit Log");
        strcpy(testPurpose_CH, "测试目的：");

        u8 ED20[2] = { 0xED,0x20 };
        u8 F1AA[2] = { 0xF1,0xAA };
        u8 DID_C00C[2] = { 0xC0,0x0C };

        u8 NRC[2] = { 0x7F,0x7E };

        TestPrecondition();

        TestStep("Enter Programming Session");
        SID10_SessionControl(ProgrammingSession);
        CheckPosResponse_SID10(ProgrammingSession);

        for (int i = 0; i < 12; i++)
        {
            SID22_ReadDataByIdentifier(DID_C00C);
            CheckNegResponse(NRC, 2);
        }

        TestStep("Enter Programming Session");
        SID10_SessionControl(ProgrammingSession);
        CheckPosResponse_SID10(ProgrammingSession);

        SecurityUnlock(0x01);

        for (int i = 0; i < 12; i++)
        {
            SID22_ReadDataByIdentifier(DID_C00C);
            CheckNegResponse(NRC, 2);
        }


        TestStep("ECU Reset");
        SID11_ECUReset(0x81, Functional);

        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_079() {
    try {
        strcpy(TestCaseName, "CS_Test_079");
        strcpy(TestScopeName, "Secure Audit Log");
        strcpy(testPurpose_CH, "测试目的：");

        u8 ED20[2] = { 0xED,0x20 };
        u8 F1AA[2] = { 0xF1,0xAA };
        u8 DID_C00E[2] = { 0xC0,0x0E };

        u8 NRC[2] = { 0x7F,0x7E };

        TestPrecondition();

        TestStep("Enter Programming Session");
        SID10_SessionControl(ProgrammingSession);
        CheckPosResponse_SID10(ProgrammingSession);

        for (int i = 0; i < 12; i++)
        {
            SID22_ReadDataByIdentifier(DID_C00E);
            CheckNegResponse(NRC, 2);
        }

        TestStep("Enter Programming Session");
        SID10_SessionControl(ProgrammingSession);
        CheckPosResponse_SID10(ProgrammingSession);

        SecurityUnlock(0x01);

        for (int i = 0; i < 12; i++)
        {
            SID22_ReadDataByIdentifier(DID_C00E);
            CheckNegResponse(NRC, 2);
        }


        TestStep("ECU Reset");
        SID11_ECUReset(0x81, Functional);

        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_080() {
    try {
        strcpy(TestCaseName, "CS_Test_080");
        strcpy(TestScopeName, "Secure Audit Log");
        strcpy(testPurpose_CH, "测试目的：");

        u8 ED20[2] = { 0xED,0x20 };
        u8 F1AA[2] = { 0xF1,0xAA };
        u8 DID_C00F[2] = { 0xC0,0x0F };

        u8 NRC[2] = { 0x7F,0x7E };

        TestPrecondition();

        TestStep("Enter Programming Session");
        SID10_SessionControl(ProgrammingSession);
        CheckPosResponse_SID10(ProgrammingSession);

        for (int i = 0; i < 12; i++)
        {
            SID22_ReadDataByIdentifier(DID_C00F);
            CheckNegResponse(NRC, 2);
        }

        TestStep("Enter Programming Session");
        SID10_SessionControl(ProgrammingSession);
        CheckPosResponse_SID10(ProgrammingSession);

        SecurityUnlock(0x01);

        for (int i = 0; i < 12; i++)
        {
            SID22_ReadDataByIdentifier(DID_C00F);
            CheckNegResponse(NRC, 2);
        }


        TestStep("ECU Reset");
        SID11_ECUReset(0x81, Functional);

        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_081() {
    try {
        strcpy(TestCaseName, "CS_Test_081");
        strcpy(TestScopeName, "Secure Audit Log");
        strcpy(testPurpose_CH, "测试目的：");

        u8 ED20[2] = { 0xED,0x20 };
        u8 F1AA[2] = { 0xF1,0xAA };
        u8 DID_C012[2] = { 0xC0,0x12 };

        u8 NRC[2] = { 0x7F,0x7E };

        TestPrecondition();

        TestStep("Enter Programming Session");
        SID10_SessionControl(ProgrammingSession);
        CheckPosResponse_SID10(ProgrammingSession);

        for (int i = 0; i < 12; i++)
        {
            SID22_ReadDataByIdentifier(DID_C012);
            CheckNegResponse(NRC, 2);
        }

        TestStep("Enter Programming Session");
        SID10_SessionControl(ProgrammingSession);
        CheckPosResponse_SID10(ProgrammingSession);

        SecurityUnlock(0x01);

        for (int i = 0; i < 12; i++)
        {
            SID22_ReadDataByIdentifier(DID_C012);
            CheckNegResponse(NRC, 2);
        }


        TestStep("ECU Reset");
        SID11_ECUReset(0x81, Functional);

        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_082() {
    try {
        strcpy(TestCaseName, "CS_Test_082");
        strcpy(TestScopeName, "Secure Audit Log");
        strcpy(testPurpose_CH, "测试目的：");

        u8 ED20[2] = { 0xED,0x20 };
        u8 F1AA[2] = { 0xF1,0xAA };
        u8 DID_C00A[2] = { 0xC0,0x0A };

        u8 NRC[2] = { 0x31,0x33 };

        TestPrecondition();

        TestStep("Enter Default Session");
        SID10_SessionControl(DefaultSession);
        CheckPosResponse_SID10(DefaultSession);

        for (int i = 0; i < 12; i++)
        {
            SID22_ReadDataByIdentifier(DID_C00A);
            CheckNegResponse(NRC, 2);
        }

        TestStep("Enter Extended Session");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        for (int i = 0; i < 12; i++)
        {
            SID22_ReadDataByIdentifier(DID_C00A);
            CheckNegResponse(NRC, 2);
        }


        TestStep("ECU Reset");
        SID11_ECUReset(0x81, Functional);

        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_083() {
    try {
        strcpy(TestCaseName, "CS_Test_083");
        strcpy(TestScopeName, "Secure Audit Log");
        strcpy(testPurpose_CH, "测试目的：");

        u8 ED20[2] = { 0xED,0x20 };
        u8 F1AA[2] = { 0xF1,0xAA };
        u8 DID_C00B[2] = { 0xC0,0x0B };

        u8 NRC[2] = { 0x31,0x33 };

        TestPrecondition();

        TestStep("Enter Default Session");
        SID10_SessionControl(DefaultSession);
        CheckPosResponse_SID10(DefaultSession);

        for (int i = 0; i < 12; i++)
        {
            SID22_ReadDataByIdentifier(DID_C00B);
            CheckNegResponse(NRC, 2);
        }

        TestStep("Enter Extended Session");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        for (int i = 0; i < 12; i++)
        {
            SID22_ReadDataByIdentifier(DID_C00B);
            CheckNegResponse(NRC, 2);
        }


        TestStep("ECU Reset");
        SID11_ECUReset(0x81, Functional);

        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_084() {
    try {
        strcpy(TestCaseName, "CS_Test_084");
        strcpy(TestScopeName, "Secure Audit Log");
        strcpy(testPurpose_CH, "测试目的：");

        u8 ED20[2] = { 0xED,0x20 };
        u8 F1AA[2] = { 0xF1,0xAA };
        u8 DID_C00C[2] = { 0xC0,0x0C };

        u8 NRC[2] = { 0x31,0x33 };

        TestPrecondition();
        TestStep("Enter Default Session");
        SID10_SessionControl(DefaultSession);
        CheckPosResponse_SID10(DefaultSession);

        for (int i = 0; i < 12; i++)
        {
            SID22_ReadDataByIdentifier(DID_C00C);
            CheckNegResponse(NRC, 2);
        }

        TestStep("Enter Extended Session");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        for (int i = 0; i < 12; i++)
        {
            SID22_ReadDataByIdentifier(DID_C00C);
            CheckNegResponse(NRC, 2);
        }


        TestStep("ECU Reset");
        SID11_ECUReset(0x81, Functional);

        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_085() {
    try {
        strcpy(TestCaseName, "CS_Test_085");
        strcpy(TestScopeName, "Secure Audit Log");
        strcpy(testPurpose_CH, "测试目的：");

        u8 ED20[2] = { 0xED,0x20 };
        u8 F1AA[2] = { 0xF1,0xAA };
        u8 DID_C00E[2] = { 0xC0,0x0E };

        u8 NRC[2] = { 0x31,0x33 };

        TestPrecondition();

        TestStep("Enter Default Session");
        SID10_SessionControl(DefaultSession);
        CheckPosResponse_SID10(DefaultSession);

        for (int i = 0; i < 12; i++)
        {
            SID22_ReadDataByIdentifier(DID_C00E);
            CheckNegResponse(NRC, 2);
        }

        TestStep("Enter Extended Session");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        for (int i = 0; i < 12; i++)
        {
            SID22_ReadDataByIdentifier(DID_C00E);
            CheckNegResponse(NRC, 2);
        }


        TestStep("ECU Reset");
        SID11_ECUReset(0x81, Functional);

        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_086() {
    try {
        strcpy(TestCaseName, "CS_Test_086");
        strcpy(TestScopeName, "Secure Audit Log");
        strcpy(testPurpose_CH, "测试目的：");

        u8 ED20[2] = { 0xED,0x20 };
        u8 F1AA[2] = { 0xF1,0xAA };
        u8 DID_C00F[2] = { 0xC0,0x0F };

        u8 NRC[2] = { 0x31,0x33 };

        TestPrecondition();

        TestStep("Enter Default Session");
        SID10_SessionControl(DefaultSession);
        CheckPosResponse_SID10(DefaultSession);

        for (int i = 0; i < 12; i++)
        {
            SID22_ReadDataByIdentifier(DID_C00F);
            CheckNegResponse(NRC, 2);
        }

        TestStep("Enter Extended Session");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        for (int i = 0; i < 12; i++)
        {
            SID22_ReadDataByIdentifier(DID_C00F);
            CheckNegResponse(NRC, 2);
        }



        TestStep("ECU Reset");
        SID11_ECUReset(0x81, Functional);

        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_087() {
    try {
        strcpy(TestCaseName, "CS_Test_087");
        strcpy(TestScopeName, "Secure Audit Log");
        strcpy(testPurpose_CH, "测试目的：");

        u8 ED20[2] = { 0xED,0x20 };
        u8 F1AA[2] = { 0xF1,0xAA };
        u8 DID_C012[2] = { 0xC0,0x12 };
        u8 NRC[2] = { 0x31,0x33 };

        TestPrecondition();

        TestStep("Enter Default Session");
        SID10_SessionControl(DefaultSession);
        CheckPosResponse_SID10(DefaultSession);

        for (int i = 0; i < 12; i++)
        {
            SID22_ReadDataByIdentifier(DID_C012);
            CheckNegResponse(NRC, 2);
        }

        TestStep("Enter Extended Session");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        for (int i = 0; i < 12; i++)
        {
            SID22_ReadDataByIdentifier(DID_C012);
            CheckNegResponse(NRC, 2);
        }

        TestStep("ECU Reset");
        SID11_ECUReset(0x81, Functional);

        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_088() {
    try {
        strcpy(TestCaseName, "CS_Test_088");
        strcpy(TestScopeName, "Secure Audit Log");
        strcpy(testPurpose_CH, "测试目的：");

        u8 ED20[2] = { 0xED,0x20 };
        u8 F1AA[2] = { 0xF1,0xAA };
        u8 DID_C00A[2] = { 0xC0,0x0A };

        u8 NRC[2] = { 0x33,0x33 };
        const int levelCOunt = 4;
        int level[levelCOunt] = { 0x03,0x05,0x11,0x19 };

        TestPrecondition();

        TestStep("Enter Extended Session");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);
        for (int unlock = 0; unlock < levelCOunt; unlock++) {
            if (0 == gCEMFlg) {
                TestInfoPrintWarning("ZCUP Not Applicable Level 0x03");
                continue;
            }
            SecurityUnlock(level[unlock]);
            if (0x19 == level[unlock]) {
                for (int i = 0; i < 12; i++)
                {
                    SID22_ReadDataByIdentifier(DID_C00A);
                    CheckPosResponse_SID22(DID_C00A);
                }
            }
            else {
                for (int i = 0; i < 12; i++)
                {
                    SID22_ReadDataByIdentifier(DID_C00A);
                    CheckNegResponse(NRC, 2);
                }
            }
        }

        TestStep("ECU Reset");
        SID11_ECUReset(0x81, Functional);

        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_089() {
    try {
        strcpy(TestCaseName, "CS_Test_089");
        strcpy(TestScopeName, "Secure Audit Log");
        strcpy(testPurpose_CH, "测试目的：");

        u8 ED20[2] = { 0xED,0x20 };
        u8 F1AA[2] = { 0xF1,0xAA };
        u8 DID_C00B[2] = { 0xC0,0x0B };

        u8 NRC[2] = { 0x33,0x33 };

        const int levelCOunt = 4;
        int level[levelCOunt] = { 0x03,0x05,0x11,0x19 };

        TestPrecondition();

        TestStep("Enter Extended Session");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);
        for (int unlock = 0; unlock < levelCOunt; unlock++) {
            if (0 == gCEMFlg) {
                TestInfoPrintWarning("ZCUP Not Applicable Level 0x03");
                continue;
            }
            SecurityUnlock(level[unlock]);
            if (0x19 == level[unlock]) {
                for (int i = 0; i < 12; i++)
                {
                    SID22_ReadDataByIdentifier(DID_C00B);
                    CheckPosResponse_SID22(DID_C00B);
                }
            }
            else {
                for (int i = 0; i < 12; i++)
                {
                    SID22_ReadDataByIdentifier(DID_C00B);
                    CheckNegResponse(NRC, 2);
                }
            }
        }

        TestStep("ECU Reset");
        SID11_ECUReset(0x81, Functional);

        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_090() {
    try {
        strcpy(TestCaseName, "CS_Test_090");
        strcpy(TestScopeName, "Secure Audit Log");
        strcpy(testPurpose_CH, "测试目的：");

        u8 ED20[2] = { 0xED,0x20 };
        u8 F1AA[2] = { 0xF1,0xAA };
        u8 DID_C00C[2] = { 0xC0,0x0C };

        u8 NRC[2] = { 0x33,0x33 };

        const int levelCOunt = 4;
        int level[levelCOunt] = { 0x03,0x05,0x11,0x19 };

        TestPrecondition();

        TestStep("Enter Extended Session");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);
        for (int unlock = 0; unlock < levelCOunt; unlock++) {
            if (0 == gCEMFlg) {
                TestInfoPrintWarning("ZCUP Not Applicable Level 0x03");
                continue;
            }
            SecurityUnlock(level[unlock]);
            if (0x19 == level[unlock]) {
                for (int i = 0; i < 12; i++)
                {
                    SID22_ReadDataByIdentifier(DID_C00C);
                    CheckPosResponse_SID22(DID_C00C);
                }
            }
            else {
                for (int i = 0; i < 12; i++)
                {
                    SID22_ReadDataByIdentifier(DID_C00C);
                    CheckNegResponse(NRC, 2);
                }
            }
        }

        TestStep("ECU Reset");
        SID11_ECUReset(0x81, Functional);

        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_091() {
    try {
        strcpy(TestCaseName, "CS_Test_091");
        strcpy(TestScopeName, "Secure Audit Log");
        strcpy(testPurpose_CH, "测试目的：");

        u8 ED20[2] = { 0xED,0x20 };
        u8 F1AA[2] = { 0xF1,0xAA };
        u8 DID_C00E[2] = { 0xC0,0x0E };

        u8 NRC[2] = { 0x33,0x33 };

        const int levelCOunt = 4;
        int level[levelCOunt] = { 0x03,0x05,0x11,0x19 };

        TestPrecondition();

        TestStep("Enter Extended Session");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);
        for (int unlock = 0; unlock < levelCOunt; unlock++) {
            if (0 == gCEMFlg) {
                TestInfoPrintWarning("ZCUP Not Applicable Level 0x03");
                continue;
            }
            SecurityUnlock(level[unlock]);
            if (0x19 == level[unlock]) {
                for (int i = 0; i < 12; i++)
                {
                    SID22_ReadDataByIdentifier(DID_C00E);
                    CheckPosResponse_SID22(DID_C00E);
                }
            }
            else {
                for (int i = 0; i < 12; i++)
                {
                    SID22_ReadDataByIdentifier(DID_C00E);
                    CheckNegResponse(NRC, 2);
                }
            }
        }

        TestStep("ECU Reset");
        SID11_ECUReset(0x81, Functional);

        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_092() {
    try {
        strcpy(TestCaseName, "CS_Test_092");
        strcpy(TestScopeName, "Secure Audit Log");
        strcpy(testPurpose_CH, "测试目的：");

        u8 ED20[2] = { 0xED,0x20 };
        u8 F1AA[2] = { 0xF1,0xAA };
        u8 DID_C00F[2] = { 0xC0,0x0F };

        u8 NRC[2] = { 0x33,0x33 };

        const int levelCOunt = 4;
        int level[levelCOunt] = { 0x03,0x05,0x11,0x19 };

        TestPrecondition();

        TestStep("Enter Extended Session");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);
        for (int unlock = 0; unlock < levelCOunt; unlock++) {
            if (0 == gCEMFlg) {
                TestInfoPrintWarning("ZCUP Not Applicable Level 0x03");
                continue;
            }
            SecurityUnlock(level[unlock]);
            if (0x19 == level[unlock]) {
                for (int i = 0; i < 12; i++)
                {
                    SID22_ReadDataByIdentifier(DID_C00F);
                    CheckPosResponse_SID22(DID_C00F);
                }
            }
            else {
                for (int i = 0; i < 12; i++)
                {
                    SID22_ReadDataByIdentifier(DID_C00F);
                    CheckNegResponse(NRC, 2);
                }
            }
        }


        TestStep("ECU Reset");
        SID11_ECUReset(0x81, Functional);

        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 CS_Test_093() {
    try {
        strcpy(TestCaseName, "CS_Test_093");
        strcpy(TestScopeName, "Secure Audit Log");
        strcpy(testPurpose_CH, "测试目的：");

        u8 ED20[2] = { 0xED,0x20 };
        u8 F1AA[2] = { 0xF1,0xAA };
        u8 DID_C012[2] = { 0xC0,0x12 };

        u8 NRC[2] = { 0x33,0x33 };

        const int levelCOunt = 4;
        int level[levelCOunt] = { 0x03,0x05,0x11,0x19 };

        TestPrecondition();

        TestStep("Enter Extended Session");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);
        for (int unlock = 0; unlock < levelCOunt; unlock++) {
            if (0 == gCEMFlg) {
                TestInfoPrintWarning("ZCUP Not Applicable Level 0x03");
                continue;
            }
            SecurityUnlock(level[unlock]);
            if (0x19 == level[unlock]) {
                for (int i = 0; i < 12; i++)
                {
                    SID22_ReadDataByIdentifier(DID_C012);
                    CheckPosResponse_SID22(DID_C012);
                }
            }
            else {
                for (int i = 0; i < 12; i++)
                {
                    SID22_ReadDataByIdentifier(DID_C012);
                    CheckNegResponse(NRC, 2);
                }
            }
        }
        TestStep("ECU Reset");
        SID11_ECUReset(0x81, Functional);

        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}



s32 test_case_request_seed_NextCycle() {
    try {
        strcpy(TestCaseName, "TC_001_test_case_request_seed_NextCycle");
        strcpy(TestScopeName, "CS");
        strcpy(testPurpose_CH, "测试目的：");

        TestPrecondition();

        //1
        SID10_SessionControl(0x03);
        CheckPosResponse_SID10(0x03);
        TestWait(2000);
        CheckCurrentSession(0x03);
        for (int count = 0; count < SecurityUnlockCycle; count++) {
            TestInfoPrint("------Step: Request seed %d cycle", count + 1);
            SendTesterPresent(TP_ON);
            SID10_SessionControl(0x03);
            CheckPosResponse_SID10(0x03);
            CheckCurrentSession(0x03);
            for (int i = 0; i < SecurityUnlockTimes; i++) {
                TestInfoPrint("------Step: Request seed %d times", i + 1);
                //TestReport_HTML_StepDescription("Step: Request seed", Info);
                if (0x702 != Diag_Request_ID) {
                    printf("test level 0x03");
                    SID27_SecurityAccess_RequestSeed(0x03);
                    CheckPosResponse_SID27_RequestSeed(0x03);
                }

                SID27_SecurityAccess_RequestSeed(0x05);
                CheckPosResponse_SID27_RequestSeed(0x05);

                SID27_SecurityAccess_RequestSeed(0x11);
                CheckPosResponse_SID27_RequestSeed(0x11);

                SID27_SecurityAccess_RequestSeed(0x19);
                CheckPosResponse_SID27_RequestSeed(0x19);
            }
            NM_NextOperation_NoSwitch();
        }
        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 test_case_request_seed_PowerReset() {
    try {
        strcpy(TestCaseName, "TC_002_test_case_request_seed_PowerReset");
        strcpy(TestScopeName, "CS");
        strcpy(testPurpose_CH, "测试目的：");

        TestPrecondition();

        //1
        SID10_SessionControl(0x03);
        CheckPosResponse_SID10(0x03);
        TestWait(2000);
        CheckCurrentSession(0x03);
        for (int count = 0; count < SecurityUnlockCycle; count++) {
            TestInfoPrint("------Step: Request seed %d cycle", count + 1);
            NMWakeUp(1);
            SendTesterPresent(TP_ON);
            SID10_SessionControl(0x03);
            CheckPosResponse_SID10(0x03);
            CheckCurrentSession(0x03);
            for (int i = 0; i < SecurityUnlockTimes; i++) {
                TestInfoPrint("------Step: Request seed %d times", i + 1);
                //TestReport_HTML_StepDescription("Step: Request seed", Info);
                if (0x702 != Diag_Request_ID) {
                    printf("test level 0x03");
                    SID27_SecurityAccess_RequestSeed(0x03);
                    CheckPosResponse_SID27_RequestSeed(0x03);
                }

                SID27_SecurityAccess_RequestSeed(0x05);
                CheckPosResponse_SID27_RequestSeed(0x05);

                SID27_SecurityAccess_RequestSeed(0x11);
                CheckPosResponse_SID27_RequestSeed(0x11);

                SID27_SecurityAccess_RequestSeed(0x19);
                CheckPosResponse_SID27_RequestSeed(0x19);
            }
            SendTesterPresent(TP_OFF);
            NMWakeUp(0);
            KL15(off);
            KL30(off);
            TestWait(30000);
            KL30(on);
            KL15(on);
            TestWait(10000);
        }
        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 test_case_request_seed() {
    try {
        strcpy(TestCaseName, "TC_000_test_case_request_seed");
        strcpy(TestScopeName, "CS");
        strcpy(testPurpose_CH, "测试目的：");

        TestPrecondition();

        //1
        SID10_SessionControl(0x03);
        CheckPosResponse_SID10(0x03);
        TestWait(2000);
        CheckCurrentSession(0x03);
        for (int count = 0; count < SecurityUnlockCycle; count++) {
            TestInfoPrint("------Step: Request seed %d cycle", count + 1);
            SendTesterPresent(TP_ON);
            SID10_SessionControl(0x03);
            CheckPosResponse_SID10(0x03);
            CheckCurrentSession(0x03);
            for (int i = 0; i < SecurityUnlockTimes; i++) {
                TestInfoPrint("------Step: Request seed %d times", i + 1);
                //TestReport_HTML_StepDescription("Step: Request seed", Info);
                if (0x702 != Diag_Request_ID) {
                    printf("test level 0x03");
                    SID27_SecurityAccess_RequestSeed(0x03);
                    CheckPosResponse_SID27_RequestSeed(0x03);
                }

                SID27_SecurityAccess_RequestSeed(0x05);
                CheckPosResponse_SID27_RequestSeed(0x05);

                SID27_SecurityAccess_RequestSeed(0x11);
                CheckPosResponse_SID27_RequestSeed(0x11);

                SID27_SecurityAccess_RequestSeed(0x19);
                CheckPosResponse_SID27_RequestSeed(0x19);
            }
            //SendTesterPresent(TP_OFF);
            //KL15(off);
            //KL30(off);
            //TestWait(30000);
            //KL30(on);
            //KL15(on);
            //TestWait(10000);
        }
        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 test_case_request_seed_NextCycle_Program() {
    try {
        strcpy(TestCaseName, "TC_001_test_case_request_seed_NextCycle_Program");
        strcpy(TestScopeName, "CS");
        strcpy(testPurpose_CH, "测试目的：");

        TestPrecondition();

        //1
        SID10_SessionControl(0x02);
        CheckPosResponse_SID10(0x02);
        TestWait(2000);
        CheckCurrentSession(0x02);
        for (int count = 0; count < SecurityUnlockCycle; count++) {

            TestInfoPrint("------Step: Request seed %d cycle", count + 1);
            SendTesterPresent(TP_ON);
            SID10_SessionControl(0x02);
            CheckPosResponse_SID10(0x02);
            CheckCurrentSession(0x02);
            for (int i = 0; i < SecurityUnlockTimes; i++) {
                TestInfoPrint("------Step: Request seed %d times", i + 1);
                SID27_SecurityAccess_RequestSeed(0x01);
                CheckPosResponse_SID27_RequestSeed(0x01);
            }
            NM_NextOperation_NoSwitch();
        }
        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 test_case_request_seed_PowerReset_Program() {
    try {
        strcpy(TestCaseName, "TC_002_test_case_request_seed_PowerReset_Program");
        strcpy(TestScopeName, "CS");
        strcpy(testPurpose_CH, "测试目的：");

        TestPrecondition();

        //1
        SID10_SessionControl(0x02);
        CheckPosResponse_SID10(0x02);
        TestWait(2000);
        CheckCurrentSession(0x02);
        for (int count = 0; count < SecurityUnlockCycle; count++) {

            TestInfoPrint("------Step: Request seed %d cycle", count + 1);
            SendTesterPresent(TP_ON);
            SID10_SessionControl(0x02);
            CheckPosResponse_SID10(0x02);
            CheckCurrentSession(0x02);
            for (int i = 0; i < SecurityUnlockTimes; i++) {
                TestInfoPrint("------Step: Request seed %d times", i + 1);
                //TestReport_HTML_StepDescription("Step: Request seed", Info);
                SID27_SecurityAccess_RequestSeed(0x01);
                CheckPosResponse_SID27_RequestSeed(0x01);
            }
            SendTesterPresent(TP_OFF);
            KL15(off);
            KL30(off);
            TestWait(30000);
            KL30(on);
            KL15(on);
            TestWait(10000);
        }
        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 test_case_Check_Wakeup_Time_NetworkWakeup() {
    try {
        strcpy(TestCaseName, "TC_003_test_case_Check_Wakeup_Time_NetworkWakeup");
        strcpy(TestScopeName, "CS");
        strcpy(testPurpose_CH, "测试目的：");

        TestPrecondition();
        const int TIMEOUT_MS = 5000;  // 设置5秒超时
        int timeoutCounter = 0;
        s64 timeStamp = 0;
        for (int count = 0; count < SecurityUnlockCycle; count++) {
            TestInfoPrint("------Step: NetworkWakeup %d cycle", count + 1);
            NM_NextOperation_NoSwitch();
            checkFirstMessageFlag = true;
            NMWakeUp(1);
            app.get_timestamp(&timeStamp);
            printf("The WakeUp Time is %.6lfms", timeStamp / 1000.0);
            if (!checkFirstMessageFlag) {
                printf("The First Message Time is %.6lfms", currentMessageTime / 1000.0);
            }
            else {
                while (checkFirstMessageFlag && timeoutCounter < TIMEOUT_MS) {
                    TestWait(1);
                    timeoutCounter++;
                }
                printf("The First Message Time is %.6lfms", currentMessageTime / 1000.0);
            }
            TestInfoPrint("The Delta Time is %.6lfms", (currentMessageTime - timeStamp) / 1000.0);
            TestWait(10000);
        }
        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 test_case_Check_Wakeup_Time_PowerReset() {
    try {
        strcpy(TestCaseName, "TC_004_test_case_Check_Wakeup_Time_PowerReset");
        strcpy(TestScopeName, "CS");
        strcpy(testPurpose_CH, "测试目的：");

        TestPrecondition();
        const int TIMEOUT_MS = 5000;  // 设置5秒超时
        int timeoutCounter = 0;
        s64 timeStamp = 0;
        s64 currentTime = 0;
        for (int count = 0; count < SecurityUnlockCycle; count++) {
            TestInfoPrint("------Step: PowerReset %d cycle", count + 1);
            KL15(off);
            KL30(off);
            com.del_cyclic_messages();
            TestWait(10000);
            checkFirstMessageFlag = true;
            KL30(on);
            app.get_timestamp(&timeStamp);
            printf("The WakeUp Time is %.6lfms", timeStamp / 1000.0);
            if (!checkFirstMessageFlag) {
                printf("The First Message Time is %.6lfms", currentMessageTime / 1000.0);
            }
            else {
                while (checkFirstMessageFlag && timeoutCounter < TIMEOUT_MS) {
                    TestWait(1);
                    timeoutCounter++;
                }
                printf("The First Message Time is %.6lfms", currentMessageTime / 1000.0);
            }
            TestInfoPrint("The Delta Time is %.6lfms", (currentMessageTime - timeStamp) / 1000.0);
            TestWait(10000);
        }
        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 test_case_Seed_Incorrect_Key() {
    try {
        strcpy(TestCaseName, "TC_005_test_case_Seed_Incorrect_Key");
        strcpy(TestScopeName, "CS");
        strcpy(testPurpose_CH, "测试目的：");
        pincode0x03[0] = 0x5E;
        pincode0x03[1] = 0x40;
        pincode0x03[2] = 0xDC;
        pincode0x03[3] = 0x91;
        pincode0x03[4] = 0xEC;
        pincode0x03[5] = 0xA1;
        pincode0x03[6] = 0x44;
        pincode0x03[7] = 0xEC;
        pincode0x03[8] = 0x66;
        pincode0x03[9] = 0x3F;
        pincode0x03[10] = 0xCC;
        pincode0x03[11] = 0x2E;
        pincode0x03[12] = 0x6E;
        pincode0x03[13] = 0xD3;
        pincode0x03[14] = 0x46;
        pincode0x03[15] = 0xD8;
        if (Diag_Request_ID == 0x701) {
            pincode0x05[0] = 0x10;
            pincode0x05[1] = 0x54;
            pincode0x05[2] = 0xA8;
            pincode0x05[3] = 0x09;
            pincode0x05[4] = 0x8E;
            pincode0x05[5] = 0xA0;
            pincode0x05[6] = 0x8C;
            pincode0x05[7] = 0x6F;
            pincode0x05[8] = 0x49;
            pincode0x05[9] = 0x25;
            pincode0x05[10] = 0x78;
            pincode0x05[11] = 0x38;
            pincode0x05[12] = 0x2A;
            pincode0x05[13] = 0x11;
            pincode0x05[14] = 0x84;
            pincode0x05[15] = 0x9B;
        }
        else {
            pincode0x05[0] = 0x57;
            pincode0x05[1] = 0xEA;
            pincode0x05[2] = 0xFE;
            pincode0x05[3] = 0xD7;
            pincode0x05[4] = 0x93;
            pincode0x05[5] = 0xDE;
            pincode0x05[6] = 0x70;
            pincode0x05[7] = 0xA8;
            pincode0x05[8] = 0x87;
            pincode0x05[9] = 0xF3;
            pincode0x05[10] = 0xB8;
            pincode0x05[11] = 0x09;
            pincode0x05[12] = 0x1A;
            pincode0x05[13] = 0xDD;
            pincode0x05[14] = 0xFA;
            pincode0x05[15] = 0x75;
        }
        TestPrecondition();
        int level = 1;
        u8 ExtendedSessionLength = 0;
        for (u8 i = 0; i < 5; i++) {
            if (ExtendedSession == securityUnlock[i].session) {
                ExtendedSessionLength++;
            }
        }

        TestStep("Enter DefaultSession");
        SID10_SessionControl(DefaultSession);
        CheckPosResponse_SID10(DefaultSession);

        TestStep("Check DefaultSession");
        CheckCurrentSession(DefaultSession);

        for (int i = 0; i < ExtendedSessionLength; i++)
        {
            if (Diag_Request_ID == 0x702 && securityUnlock[i].level == 0x03) {
                continue;
            }
            TestInfoPrint("-----Test Level 0x%X-----", securityUnlock[i].level);
            stepnumber = 2;
            TestStep("Enter ExtendedSession");
            SID10_SessionControl(ExtendedSession);
            CheckPosResponse_SID10(ExtendedSession);

            TestStep("Check ExtendedSession");
            CheckCurrentSession(ExtendedSession);

            for (int j = 0; j < 3; j++) {
                TestStep("Wait 2s,Send Incorrect Key");
                TestWait(2000);
                if (j >= 2) {
                    SID27_SecurityAccess_RequestSeed(securityUnlock[i].level);
                    CheckNegResponse(0x37);
                    break;
                }
                SID27_SecurityAccess_RequestSeed(securityUnlock[i].level);
                CheckPosResponse_SID27_RequestSeed(securityUnlock[i].level);
                if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
                {
                    TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
                }

                GenSAKeyExOpt(securityUnlock[i].level);
                KeyArray[15]++;

                TestStep("Send Incorrect Key");
                SID27_SecurityAccess_SendKey(securityUnlock[i].level);
                CheckNegResponse(0x35 + j);
            }

            TestStep("Wait 15s,Send Incorrect Key");
            TestWait(15000);
            SID27_SecurityAccess_RequestSeed(securityUnlock[i].level);
            CheckPosResponse_SID27_RequestSeed(securityUnlock[i].level);
            if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
            {
                TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
            }

            GenSAKeyExOpt(securityUnlock[i].level);
            KeyArray[15]++;
            SID27_SecurityAccess_SendKey(securityUnlock[i].level);
            CheckNegResponse(0x35);


            TestStep("Wait 2s,Send Correct Key");
            TestWait(2000);
            SID27_SecurityAccess_RequestSeed(securityUnlock[i].level);
            CheckPosResponse_SID27_RequestSeed(securityUnlock[i].level);
            if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
            {
                TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
            }

            GenSAKeyExOpt(securityUnlock[i].level);

            SID27_SecurityAccess_SendKey(securityUnlock[i].level);
            CheckPosResponse_SID27_SendKey(securityUnlock[i].level);

            TestStep("Wait 1s,Request Seed");
            TestWait(1000);
            SID27_SecurityAccess_RequestSeed(securityUnlock[i].level);
            CheckPosResponse_SID27_RequestSeed(securityUnlock[i].level);
            if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
            {
                TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Pass);
            }

            TestStep("Enter ExtendedSession");
            SID10_SessionControl(ExtendedSession);
            CheckPosResponse_SID10(ExtendedSession);

            TestStep("Check ExtendedSession");
            CheckCurrentSession(ExtendedSession);

            TestStep("Wait 1s,Request Seed");
            TestWait(1000);
            SID27_SecurityAccess_RequestSeed(securityUnlock[i].level);
            CheckPosResponse_SID27_RequestSeed(securityUnlock[i].level);
            if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
            {
                TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
            }
            else {
                TestReport_HTML_StepDescription("Seed not equals to 0x000000!", Pass);
            }
        }

        TestStep("Enter DefaultSession");
        SID10_SessionControl(DefaultSession);
        CheckPosResponse_SID10(DefaultSession);

        TestStep("Check DefaultSession");
        CheckCurrentSession(DefaultSession);

        TestInfoPrint("-----Test Level 0x01-----");
        stepnumber = 2;
        TestStep("Enter ProgrammingSession");
        SID10_SessionControl(ProgrammingSession);
        CheckPosResponse_SID10(ProgrammingSession);

        TestStep("Check ProgrammingSession");
        CheckCurrentSession(ProgrammingSession);

        for (int j = 0; j < 3; j++) {
            TestStep("Wait 2s,Send Incorrect Key");
            TestWait(2000);
            if (j >= 2) {
                SID27_SecurityAccess_RequestSeed(0x01);
                CheckNegResponse(0x37);
                break;
            }
            SID27_SecurityAccess_RequestSeed(0x01);
            if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
            {
                TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
            }

            GenSAKeyExOpt(0x01);
            KeyArray[15]++;

            SID27_SecurityAccess_SendKey(0x01);
            CheckNegResponse(0x35 + j);
        }

        TestStep("Wait 60s,Send Incorrect Key");
        TestWait(60000);
        SID27_SecurityAccess_RequestSeed(0x01);
        CheckPosResponse_SID27_RequestSeed(0x01);
        if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
        {
            TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
        }

        GenSAKeyExOpt(0x01);
        KeyArray[15]++;
        SID27_SecurityAccess_SendKey(0x01);
        CheckNegResponse(0x35);


        TestStep("Wait 2s,Send Incorrect Key");
        TestWait(2000);
        SID27_SecurityAccess_RequestSeed(0x01);
        CheckPosResponse_SID27_RequestSeed(0x01);
        if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
        {
            TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
        }

        GenSAKeyExOpt(0x01);

        SID27_SecurityAccess_SendKey(0x01);
        CheckPosResponse_SID27_SendKey(0x01);

        TestStep("Wait 1s,Request Seed");
        TestWait(1000);
        SID27_SecurityAccess_RequestSeed(0x01);
        CheckPosResponse_SID27_RequestSeed(0x01);
        if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
        {
            TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Pass);
        }

        TestStep("Enter ProgrammingSession");
        SID10_SessionControl(ProgrammingSession);
        CheckPosResponse_SID10(ProgrammingSession);

        TestStep("Check ProgrammingSession");
        CheckCurrentSession(ProgrammingSession);

        TestStep("Wait 1s,Request Seed");
        TestWait(1000);
        SID27_SecurityAccess_RequestSeed(0x01);
        CheckPosResponse_SID27_RequestSeed(0x01);
        if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
        {
            TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
        }
        else {
            TestReport_HTML_StepDescription("Seed not equals to 0x000000!", Pass);
        }

        SID11_ECUReset(0x81);
        CheckNonResponse();

        TestPostCondition();

        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 test_case_Seed_D01C() {
    try {
        strcpy(TestCaseName, "TC_006_test_case_Seed_D01C");
        strcpy(TestScopeName, "CS");
        strcpy(testPurpose_CH, "测试目的：");

        TestPrecondition();
        TestStep("Enter Programming Session");
        SID10_SessionControl(ProgrammingSession);

        TestStep("Check Programming Session");
        CheckCurrentSession(ProgrammingSession);

        TestStep("Read Public Key");
        u8 D01C[2] = { 0XD0,0X1C };
        //如果文件包含公钥
        SID22_ReadDataByIdentifier(D01C);
        //如果未写入公钥
        if (0 == Raw_CheckNegResponse(0x22)) {
            TestReport_StepInfo(Pass);
        }
        else if (TpRxData[2] = 0x62) {
            TestReport_HTML_StepDescription("ECU已写入公钥！", Pass);
        }

        TestStep("Downlaod SBL");
        u8 DID1[2] = { 0xED,0x20 };
        SID22_ReadDataByIdentifier(DID1);
        CheckPosResponse_SID22(DID1);

        u8 DID2[2] = { 0xF1,0xAA };
        SID22_ReadDataByIdentifier(DID2);
        CheckPosResponse_SID22(DID2);

        TestSeq_DownloadSBL(0);

        u8 DID3[2] = { 0xF1,0xA2 };
        SID22_ReadDataByIdentifier(DID3);
        CheckNegResponse(0x31);

        ActivateSBL();
        SID22_ReadDataByIdentifier(DID1);
        CheckPosResponse_SID22(DID1);

        SID22_ReadDataByIdentifier(DID3);
        CheckPosResponse_SID22(DID3, 8);

        //SendTesterPresent(TP_OFF);

        TestWait(6000);

        //TestStep("Enter Programming Session");
        //SID10_SessionControl(ProgrammingSession);

        //TestStep("Check Programming Session");
        //CheckCurrentSession(ProgrammingSession);

        TestStep("Read Public Key");
        //如果文件包含公钥
        SID22_ReadDataByIdentifier(D01C);
        //如果未写入公钥
        if (0 == Raw_CheckNegResponse(0x22)) {
            TestReport_StepInfo(Pass);
        }
        else if (TpRxData[2] = 0x62) {
            TestReport_HTML_StepDescription("ECU已写入公钥！", Pass);
        }

        TestStep("Write Public Key");
        u8 D01C_Key[295] = { 0xD0,0x1C,0xD2,0x1E,0x37,0x2A,0x9B,0x64,0xF1,0x8E,0xC6,0x53,0x43,0xE0,0x2A,0x2D,0x48,0x06,
                         0x19,0x56,0x70,0x17,0xC8,0x85,0xD1,0x7B,0x5B,0xD4,0x8A,0x41,0xCB,0x53,0xB4,0x68,
                         0xDF,0xED,0xB9,0xD0,0x0A,0xD7,0xE0,0xDB,0x4B,0x1D,0x9B,0x7F,0x44,0x3F,0xC0,0xE7,
                         0xBB,0xDB,0x82,0x83,0xF1,0x9E,0x2B,0x7E,0x2B,0x0A,0x6C,0x13,0x97,0x6A,0xB8,0xEF,
                         0x11,0x6F,0x26,0x46,0x20,0x00,0x58,0x85,0x6C,0xD8,0xE0,0xF0,0xE3,0x8F,0xA0,0xD5,
                         0xB2,0x8B,0x02,0x3D,0xDA,0x64,0x98,0x8B,0x4D,0xC9,0xD7,0xD2,0x6A,0x0F,0x8C,0x11,
                         0x77,0x9F,0x20,0xE4,0xAC,0xD4,0x92,0x8E,0x3B,0x70,0x66,0x4D,0x5B,0x5A,0x13,0xB7,
                         0x6A,0x36,0x9E,0xD9,0x34,0x1C,0x8C,0x56,0xD4,0x1C,0x30,0x91,0x44,0x83,0xCE,0x1A,
                         0xC5,0x5F,0xCE,0x75,0x98,0x50,0x3B,0x7F,0x09,0xC7,0xD2,0xC9,0xA7,0x14,0xF5,0x80,
                         0xEC,0x00,0x16,0x83,0xF2,0x28,0x38,0x64,0x25,0x0E,0xA0,0x45,0xBE,0x33,0xDE,0x18,
                         0x66,0x2F,0x26,0x8B,0xF1,0xA6,0x1E,0x12,0xFD,0x44,0x2B,0x18,0x86,0x40,0x65,0xCE,
                         0x22,0xB3,0xF4,0xD6,0x1A,0x7A,0xBB,0x83,0x65,0x90,0x67,0xD3,0x37,0x77,0xE4,0x99,
                         0x50,0x48,0x23,0x7F,0xDA,0xCF,0xB3,0x22,0xF9,0x13,0x40,0x4B,0x18,0xC2,0x87,0xE8,
                         0x25,0x39,0x0A,0x53,0x66,0x2B,0x6A,0x53,0x9D,0x75,0x61,0x7C,0x78,0xE6,0xD3,0xDE,
                         0x08,0xFB,0x2E,0xF5,0x26,0xE2,0xC0,0xBD,0xCE,0x65,0xD6,0x9A,0xC0,0x01,0xF4,0xB3,
                         0x10,0x64,0x9C,0xEE,0x5A,0x84,0x8F,0x71,0xBB,0x01,0xBD,0x16,0xEA,0xDD,0x9C,0xF7,
                         0x00,0x01,0x00,0x01,0x02,0x9F,0x26,0x24,0xE7,0x65,0xE7,0x3F,0xF3,0xCC,0x98,0x20,
                         0x9C,0xE4,0x37,0x06,0xB0,0x75,0xD5,0x9F,0x55,0x88,0x2F,0x3B,0x1B,0xC2,0xA3,0x7E,
                         0xE3,0x4F,0x17,0xE9,0x00 };
        SID2E_WriteDataById(D01C_Key, 295);
        CheckNegResponse(0X31);

        TestStep("Security Unlock");
        SecurityUnlock(0x01);

        TestStep("Read Public Key");
        //如果文件包含公钥
        SID22_ReadDataByIdentifier(D01C);
        //如果未写入公钥
        if (0 == Raw_CheckNegResponse(0x22)) {
            TestReport_StepInfo(Pass);
        }
        else if (TpRxData[2] = 0x62) {
            TestReport_HTML_StepDescription("ECU已写入公钥！", Pass);
        }

        TestStep("Write Public Key Lack 1 Byte");
        SID2E_WriteDataById(D01C_Key, 293);
        CheckNegResponse(0X31);

        TestStep("Write Public Key Change 1 Byte");
        D01C_Key[293] = ~D01C_Key[293];
        SID2E_WriteDataById(D01C_Key, 294);
        CheckNegResponse(0X31);

        D01C_Key[293] = ~D01C_Key[293];

        TestStep("Write Public Key Add 1 Byte");
        SID2E_WriteDataById(D01C_Key, 295);
        CheckNegResponse(0X31);

        TestStep("Write Public Correct");
        SID2E_WriteDataById(D01C_Key, 294);
        CheckPosResponse_SID2E(D01C);

        TestStep("Read Public Key");
        //如果文件包含公钥
        SID22_ReadDataByIdentifier(D01C);
        CheckPosResponse_SID22(D01C);

        TestPostCondition();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 test_case_Check_Signature()
{
    try {
        //test刷写全流程
        //strcpy(TestCaseName, "TC_1021823_File Download Full Sequence Test - Physical Addressing");
        strcpy(TestCaseName, "TC_007_test_case_Check_Signature");
        strcpy(TestScopeName, "CS");
        strcpy(testPurpose_CH, "测试目的：");

        TestPrecondition();

        TestSeq_EnterProgrammingSession();

        u8 DID1[2] = { 0xED,0x20 };
        SID22_ReadDataByIdentifier(DID1);
        CheckPosResponse_SID22(DID1);

        u8 DID2[2] = { 0xF1,0xAA };
        SID22_ReadDataByIdentifier(DID2);
        CheckPosResponse_SID22(DID2);

        TestSeq_DownloadSBL(0);

        u8 DID3[2] = { 0xF1,0xA2 };
        SID22_ReadDataByIdentifier(DID3);
        CheckNegResponse(0x31);

        ActivateSBL();

        SID22_ReadDataByIdentifier(DID1);
        CheckPosResponse_SID22(DID1);

        SID22_ReadDataByIdentifier(DID3);
        CheckPosResponse_SID22(DID3, 8);

        char keyvalue[20] = "APP0Path";
        TestReport_HTML_StepDescription("----Download APP Change_Data----");
        vbfparser(keyvalue);
        TestReport_HTML_StepDescription("----Download APP Signature Invalid----");
        for (int i = 0; i < APPFileNum; i++)
        {
            if (2 != CheckCurrentSession_Return(ProgrammingSession)) {
                SID10_SessionControl(ProgrammingSession);
                CheckCurrentSession_Return(ProgrammingSession);

                SecurityUnlock(0x01);
            }
            char StepDespriction[256];
            char filename[300] = "";
            vbfparser(keyvalue);
            app.get_system_var_string("Diagnostic0.下载文件_DataFile", 512, &filename[0]);
            filename[strlen(filename) - 9] = '\0';
            snprintf(StepDespriction, sizeof(StepDespriction), "Download File is %s ", filename);
            TestReport_HTML_StepDescription(StepDespriction);
            EraseAddress();
            FileDownload();
            CheckSignatureInvalid();
            keyvalue[3]++;
        }


        keyvalue[3] = '0';
        TestReport_HTML_StepDescription("----Download APP Signature Blank----");
        for (int i = 0; i < APPFileNum; i++)
        {
            if (2 != CheckCurrentSession_Return(ProgrammingSession)) {
                SID10_SessionControl(ProgrammingSession);
                CheckCurrentSession_Return(ProgrammingSession);

                SecurityUnlock(0x01);
            }
            char StepDespriction[256];
            char filename[300] = "";
            vbfparser(keyvalue);
            app.get_system_var_string("Diagnostic0.下载文件_DataFile", 512, &filename[0]);
            filename[strlen(filename) - 9] = '\0';
            snprintf(StepDespriction, sizeof(StepDespriction), "Download File is %s ", filename);
            TestReport_HTML_StepDescription(StepDespriction);
            EraseAddress();
            FileDownload();
            CheckSignatureBlank();
            keyvalue[3]++;
        }
        keyvalue[3] = '0';
        TestReport_HTML_StepDescription("----Download APP----");
        for (int i = 0; i < APPFileNum; i++)
        {
            if (2 != CheckCurrentSession_Return(ProgrammingSession)) {
                SID10_SessionControl(ProgrammingSession);
                CheckCurrentSession_Return(ProgrammingSession);

                SecurityUnlock(0x01);
            }
            char StepDespriction[256];
            char filename[300] = "";
            vbfparser(keyvalue);
            app.get_system_var_string("Diagnostic0.下载文件_DataFile", 512, &filename[0]);
            filename[strlen(filename) - 9] = '\0';
            snprintf(StepDespriction, sizeof(StepDespriction), "Download File is %s ", filename);
            TestReport_HTML_StepDescription(StepDespriction);
            EraseAddress();
            FileDownload();
            CheckSignature();
            keyvalue[3]++;
        }

        keyvalue[3] = '0';
        for (int i = 0; i < APPFileNum; i++)
        {
            if (2 != CheckCurrentSession_Return(ProgrammingSession)) {
                SID10_SessionControl(ProgrammingSession);
                CheckCurrentSession_Return(ProgrammingSession);

                SecurityUnlock(0x01);
            }

            char StepDespriction[256];
            char filename[300] = "";
            vbfparser(keyvalue);
            app.get_system_var_string("Diagnostic0.下载文件_DataFile", 512, &filename[0]);
            filename[strlen(filename) - 9] = '\0';
            snprintf(StepDespriction, sizeof(StepDespriction), "Download File is %s ", filename);
            TestReport_HTML_StepDescription(StepDespriction);
            EraseAddress();
            FileDownload_with_Change_Data(1, 0);
            CheckSignature();
            keyvalue[3]++;
        }

        if (0 != CheckCompleteAndCompatible()) {
            strcpy(issuereason, "");
            TestReport_HTML_StepDescription("检查完整性和兼容性不通过！", Fail);
        }

        SID11_ECUReset(0x81);
        CheckNonResponse();
        TestWait(ECUResetTime);

        CheckCurrentSession(0x01);

        RX_SID19_ReportDTCByStatusMask(0x08);
        RX_CheckPosResponse_SID19_02(0xFF);

        CheckSoftwareVersion();

        CheckCurrentSession(0x01);


        TestPostCondition();

        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 test_case_Check_Read_C00X()
{
    try {
        strcpy(TestCaseName, "TC_008_test_case_Check_Read_C00X");
        strcpy(TestScopeName, "CS");
        strcpy(testPurpose_CH, "测试目的：");
        const u8 arryLength = 7;
        u8 DID_Array[arryLength][2] = { {0xC0,0x0A},{0xC0,0x0B},{0xC0,0x0C},{0xC0,0x0D},{0xC0,0x0E},{0xC0,0x0F},{0xC0,0x12} };
        u8 NRC[4] = { 0x7F,0x31,0x12,0x33 };
        TestPrecondition();
        FulfilTRC();
        TRCSwitch(0X0, 0X1, 0X0);
        TestSeq_EnterProgrammingSession();

        TestStep("Programming Session Request 0x05-0x11-0x19");

        SID27_SecurityAccess_RequestSeed(0x05);
        CheckNegResponse(NRC);
        SID27_SecurityAccess_RequestSeed(0x11);
        CheckNegResponse(NRC);
        SID27_SecurityAccess_RequestSeed(0x19);
        CheckNegResponse(NRC);

        TestStep("Default Session Request 0x05-0x11-0x19");
        SID10_SessionControl(DefaultSession);
        CheckPosResponse_SID10(DefaultSession);

        TestStep("Check DefaultSession");
        CheckCurrentSession(DefaultSession);

        SID27_SecurityAccess_RequestSeed(0x05);
        CheckNegResponse(NRC);
        SID27_SecurityAccess_RequestSeed(0x11);
        CheckNegResponse(NRC);
        SID27_SecurityAccess_RequestSeed(0x19);
        CheckNegResponse(NRC);

        TestStep("Programming Session 'Not Unlcok' Read C00A-C012");
        SID10_SessionControl(ProgrammingSession);
        CheckPosResponse_SID10(ProgrammingSession);

        TestStep("Check ProgrammingSession");
        CheckCurrentSession(ProgrammingSession);

        for (int i = 0; i < arryLength; i++) {
            SID22_ReadDataByIdentifier(DID_Array[i]);
            CheckNegResponse(NRC, 4);
        }

        TestStep("Programming Session 'Unlcok' Read C00A-C012");
        SID10_SessionControl(ProgrammingSession);
        CheckPosResponse_SID10(ProgrammingSession);

        TestStep("Check ProgrammingSession");
        CheckCurrentSession(ProgrammingSession);

        SecurityUnlock(0x01);

        for (int i = 0; i < arryLength; i++) {
            SID22_ReadDataByIdentifier(DID_Array[i]);
            CheckNegResponse(NRC);
        }

        TestStep("Default Session 'Not Unlcok' Read C00A-C012");
        SID10_SessionControl(DefaultSession);
        CheckPosResponse_SID10(DefaultSession);

        TestStep("Check Default");
        CheckCurrentSession(DefaultSession);

        for (int i = 0; i < arryLength; i++) {
            SID22_ReadDataByIdentifier(DID_Array[i]);
            CheckNegResponse(NRC);
        }

        TestStep("Extended Session 'Not Unlcok' Read C00A-C012");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        TestStep("Check ExtendedSession");
        CheckCurrentSession(ExtendedSession);

        for (int i = 0; i < arryLength; i++) {
            SID22_ReadDataByIdentifier(DID_Array[i]);
            CheckNegResponse(NRC);
        }

        TestStep("Extended Session 'Unlcok 0x05' Read C00A-C012");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        TestStep("Check ExtendedSession");
        CheckCurrentSession(ExtendedSession);
        SecurityUnlock(0x05);
        for (int i = 0; i < arryLength; i++) {
            SID22_ReadDataByIdentifier(DID_Array[i]);
            CheckNegResponse(NRC);
        }

        TestStep("Extended Session 'Unlcok 0x011' Read C00A-C012");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        TestStep("Check ExtendedSession");
        CheckCurrentSession(ExtendedSession);
        SecurityUnlock(0x11);
        for (int i = 0; i < arryLength; i++) {
            SID22_ReadDataByIdentifier(DID_Array[i]);
            CheckNegResponse(NRC);
        }

        TestStep("Extended Session 'Unlcok 0x19' Read C00A-C012");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        TestStep("Check ExtendedSession");
        CheckCurrentSession(ExtendedSession);
        SecurityUnlock(0x19);
        for (int i = 0; i < arryLength; i++) {
            SID22_ReadDataByIdentifier(DID_Array[i]);
            CheckPosResponse_SID22(DID_Array[i]);
        }

        TestStep("Wait Enter Low Power");

        NM_NextOperation();

        TestStep("Extended Session 'Unlcok 0x19' Read C00A-C012");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        TestStep("Check ExtendedSession");
        CheckCurrentSession(ExtendedSession);
        SecurityUnlock(0x19);
        for (int i = 0; i < arryLength; i++) {
            SID22_ReadDataByIdentifier(DID_Array[i]);
            CheckPosResponse_SID22(DID_Array[i]);
        }

        TestStep("Reset Power");
        PowerReset();

        TestStep("Extended Session 'Unlcok 0x19' Read C00A-C012");
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);

        TestStep("Check ExtendedSession");
        CheckCurrentSession(ExtendedSession);
        SecurityUnlock(0x19);
        for (int i = 0; i < arryLength; i++) {
            SID22_ReadDataByIdentifier(DID_Array[i]);
            CheckPosResponse_SID22(DID_Array[i]);
        }

        TestPostCondition();

        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 test_case_Check_Read_C00A()
{
    try {
        //test刷写全流程
        //strcpy(TestCaseName, "TC_1021823_File Download Full Sequence Test - Physical Addressing");
        strcpy(TestCaseName, "TC_009_test_case_Check_Read_C00A");
        strcpy(TestScopeName, "CS");
        strcpy(testPurpose_CH, "测试目的：");
        u8 DID_C00A[2] = { 0xC0,0x0A };
        u8 DID_Store[192] = { 0x0 };
        u32 lastCount = 0;
        TestPrecondition();

        TestStep("Unlock 0x19 Read Pass 0xC00A");
        SID10_SessionControl(DefaultSession);
        CheckPosResponse_SID10(DefaultSession);
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);
        SecurityUnlock(0x19);
        SID22_ReadDataByIdentifier(DID_C00A);
        if (0 == CheckPosResponse_SID22(DID_C00A)) {
            StoreArray(DID_Store, TpRxData, 192, 0, 0);
        }
        lastCount = ((u32)DID_Store[3] << 24) | ((u32)DID_Store[4] << 16) | ((u32)DID_Store[5] << 8) | (u32)DID_Store[6];
        TestInfoPrint("Current Read Pass Count is %d", lastCount);

        TestStep("Unlock Level 0x01 0x05 0x11 0x19");
        for (int cycleCount = 0; cycleCount < 3; cycleCount++) {
            SID10_SessionControl(ProgrammingSession);
            CheckPosResponse_SID10(ProgrammingSession);
            CheckCurrentSession(ProgrammingSession);
            if (0 != SecurityUnlock(0x01)) {
                TestWait(60000);
                SecurityUnlock(0x01);
            }
            SID10_SessionControl(DefaultSession);
            CheckPosResponse_SID10(DefaultSession);
            CheckCurrentSession(DefaultSession);
            SID10_SessionControl(ExtendedSession);
            CheckPosResponse_SID10(ExtendedSession);
            CheckCurrentSession(ExtendedSession);
            if (0 != SecurityUnlock(0x05)) {
                TestWait(15000);
                SecurityUnlock(0x05);
            }
            if (0 != SecurityUnlock(0x11)) {
                TestWait(15000);
                SecurityUnlock(0x11);
            }
            if (0 != SecurityUnlock(0x19)) {
                TestWait(15000);
                SecurityUnlock(0x19);
            }
            TestWait(3000);
        }
        TestStep("Calculate Pass Correct Data");
        SID22_ReadDataByIdentifier(DID_C00A);
        if (0 == CheckPosResponse_SID22(DID_C00A)) {
            StoreArray(DID_Store, TpRxData, 192, 0, 0);
        }
        if (0 == calculateResult_C00A(DID_Store, true, lastCount, 12)) {
            TestInfoPrintPass("Record Pass Data Is Correct");
        }
        else {
            TestInfoPrintFail("Record Pass Data Is Not Correct");
        }


        TestStep("Unlock 0x19 Read Fail 0xC00A");
        SID10_SessionControl(DefaultSession);
        CheckPosResponse_SID10(DefaultSession);
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);
        SecurityUnlock(0x19);
        SID22_ReadDataByIdentifier(DID_C00A);
        if (0 == CheckPosResponse_SID22(DID_C00A)) {
            StoreArray(DID_Store, TpRxData, 192, 0, 0);
        }
        lastCount = ((u32)DID_Store[7] << 24) | ((u32)DID_Store[8] << 16) | ((u32)DID_Store[9] << 8) | (u32)DID_Store[10];
        TestInfoPrint("Current Read Fail Count is %d", lastCount);

        TestStep("Unlock Level 0x01 0x05 0x11 0x19");
        for (int cycleCount = 0; cycleCount < 3; cycleCount++) {
            SID10_SessionControl(ProgrammingSession);
            CheckPosResponse_SID10(ProgrammingSession);
            CheckCurrentSession(ProgrammingSession);
            if (0 != SecurityUnlock_Invalid(0x01)) {
                TestWait(60000);
                SecurityUnlock_Invalid(0x01);
            }
            SID10_SessionControl(DefaultSession);
            CheckPosResponse_SID10(DefaultSession);
            CheckCurrentSession(DefaultSession);
            SID10_SessionControl(ExtendedSession);
            CheckPosResponse_SID10(ExtendedSession);
            CheckCurrentSession(ExtendedSession);
            if (0 != SecurityUnlock_Invalid(0x05)) {
                TestWait(15000);
                SecurityUnlock_Invalid(0x05);
            }
            if (0 != SecurityUnlock_Invalid(0x11)) {
                TestWait(15000);
                SecurityUnlock_Invalid(0x11);
            }
            if (0 != SecurityUnlock_Invalid(0x19)) {
                TestWait(15000);
                SecurityUnlock_Invalid(0x19);
            }
            TestWait(3000);
        }
        TestStep("Calculate Fail Correct Data");
        SID22_ReadDataByIdentifier(DID_C00A);
        if (0 == CheckPosResponse_SID22(DID_C00A)) {
            StoreArray(DID_Store, TpRxData, 192, 0, 0);
        }
        if (0 == calculateResult_C00A(DID_Store, false, lastCount, 12)) {
            TestInfoPrintPass("Record Fail Data Is Correct");
        }
        else {
            TestInfoPrintFail("Record Fail Data Is Not Correct");
        }


        TestPostCondition();

        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 test_case_Check_Read_C00B()
{
    try {
        strcpy(TestCaseName, "TC_009_test_case_Check_Read_C00B");
        strcpy(TestScopeName, "CS");
        strcpy(testPurpose_CH, "测试目的：");
        u8 DID_C00B[2] = { 0xC0,0x0B };
        u8 DID_Store[168] = { 0x0 };
        u32 lastCount = 0;
        gVehSpd = 0x1000;
        TestPrecondition();
        TestInfoPrint("仿真车速信号gVehSpd = %d", gVehSpd);
        FulfilTRC();
        TestWait(5000);

        TestStep("Unlock 0x19 Read 0xC00B");
        SID10_SessionControl(DefaultSession);
        CheckPosResponse_SID10(DefaultSession);
        SID10_SessionControl(ExtendedSession);
        CheckPosResponse_SID10(ExtendedSession);
        SecurityUnlock(0x19);
        SID22_ReadDataByIdentifier(DID_C00B);
        if (0 == CheckPosResponse_SID22(DID_C00B)) {
            StoreArray(DID_Store, TpRxData, 168, 0, 0);
        }
        lastCount = ((u32)DID_Store[3] << 24) | ((u32)DID_Store[4] << 16) | ((u32)DID_Store[5] << 8) | (u32)DID_Store[6];
        TestInfoPrint("Current Read Count is %d", lastCount);

        TestStep("Cycle Excute 10 02 - 10 82");
        for (int cycleCount = 0; cycleCount < 11; cycleCount++) {
            SID10_SessionControl(ProgrammingSession);
            CheckNegResponse(0x22);
            TestWait(1000);
            SID10_SessionControl(0x82);
            CheckNegResponse(0x22);
            TestWait(3000);
        }
        TestStep("Verify 0xC00B Data");
        SID22_ReadDataByIdentifier(DID_C00B);
        if (0 == CheckPosResponse_SID22(DID_C00B)) {
            StoreArray(DID_Store, TpRxData, 168, 0, 0);
        }
        if (0 == calculateResult_C00B(DID_Store, lastCount, 22)) {
            TestInfoPrintPass("Record Pass Data Is Correct");
        }
        else {
            TestInfoPrintFail("Record Pass Data Is Not Correct");
        }
        gVehSpd = 0xAA;
        TestPostCondition();

        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

#endif // !TestCase_CAN_CS