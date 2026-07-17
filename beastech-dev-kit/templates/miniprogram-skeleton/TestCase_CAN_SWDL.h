#ifndef TestCase_CAN_SWDL
#define TestCase_CAN_SWDL

#include "test_case_diag_api.h"



////////////////////////////////////////////////////////////////////////////////////////CAN_SWDL/////////////////////////////////////////////////////////////////////////////////

//Software Download  (Device)
s32 test_case_1021823()
{
    try {
        //test刷写全流程
        //strcpy(TestCaseName, "TC_1021823_File Download Full Sequence Test - Physical Addressing");
        strcpy(TestCaseName, "TC_001_正向刷写及启动跳转");
        strcpy(TestScopeName, "Software Download");
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

        char keyvalue[20] = "APP0Path";
        TestReport_HTML_StepDescription("----Download APP----");
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

        if (0 != CheckCompleteAndCompatible()) {
            strcpy(issuereason, "");
            TestReport_HTML_StepDescription("检查完整性和兼容性不通过！", Fail);
        }

        AppWait(5000, "");

        SID11_ECUReset(0x81,1);
        CheckNonResponse();
        TestWait(ECUResetTime);

        CheckCurrentSession(0x01);

        RX_SID19_ReportDTCByStatusMask(0x08);
        RX_CheckPosResponse_SID19_02(0xFF);

        CheckSoftwareVersion();

        CheckCurrentSession(0x01);

        PowerReset(30000);
        TestWait(ECUResetTime);

        CheckCurrentSession(0x01);

        RX_SID19_ReportDTCByStatusMask(0x09);     //vector这里发的是19 02 09
        RX_CheckPosResponse_SID19_02(0xFF);

        CheckSoftwareVersion();

        CheckCurrentSession(0x01);

        CheckSoftVersion();

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

s32 test_case_1021932() {
    try {
        strcpy(TestCaseName, "TC_002_APP复刷");
        strcpy(TestScopeName, "Software Download");
        strcpy(testPurpose_CH, "测试目的：");


        char APP_File_VersionA[20] = "APP0Path_VersionA";
        char SBL_File_VersionA[20] = "SBL_Path_VersionA";
        char APP_File[20] = "APP0Path";
        char StepDespriction[256];
        char filename[300] = {};
        u16 FileSignature = 0x0212;
        u8 ExpectedResponse[2] = { 0x10,0x00 };

        TestPrecondition();

        //Download Soft Version A
        TestSeq_EnterProgrammingSession();
        TestReport_HTML_StepDescription("----Download SBL VersionA----");
        ReadPublicKey();
        SecurityUnlock(0x01);
        vbfparser(SBL_File_VersionA);
        app.get_system_var_string("Diagnostic0.下载文件_DataFile", 512, &filename[0]);
        filename[strlen(filename) - 9] = '\0';
        snprintf(StepDespriction, sizeof(StepDespriction), "Download SBL_Filename is %s ", filename);
        TestReport_HTML_StepDescription(StepDespriction);
        FileDownload(0);
        CheckSignature();
        ActivateSBL();
        TestReport_HTML_StepDescription("----Download APP VersionA----");
        productionSign = 0;
        //vbfparser(APP_File_VersionA);
        for (int i = 0; i < APPFileNum; i++)
        {
            vbfparser(APP_File_VersionA);
            filename[0] = '\0';
            if (app.get_system_var_string("Diagnostic0.下载文件_DataFile", 512, filename) != 0 || filename[0] == '\0') {
                log("Error: Failed to get filename for %s", APP_File_VersionA);
                snprintf(StepDespriction, sizeof(StepDespriction), "Download APP%d (no filename)", i);
            }
            else {
                // 安全截断文件名
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

            SID31_RoutineControl(0x01, FileSignature, VbfFileInfo.sw_signature_dev.count, VbfFileInfo.sw_signature_dev.array);
            CheckPosResponse_SID31(0x01, FileSignature, 2, ExpectedResponse);
            APP_File_VersionA[3]++;
        }
        CheckCompleteAndCompatible();


        //step12
        TestStep("ECU Reset");
        SID11_ECUReset(0x01, Functional);
        TestWait(5000);


        //解析softVersion
        APP_File_VersionA[3] = '0';
        for (u8 i = 0; i < APPFileNum; i++) {
            vbfparser(APP_File_VersionA);
            APP_File_VersionA[3]++;
        }


        //step1
        stepnumber = 0;
        TestStep("Check Current Session");
        CheckCurrentSession(0x01);

        //step2
        TestStep("Check Software Version");
        CheckSoftwareVersion();

        //step3
        TestStep("EnterProgrammingSession");
        TestSeq_EnterProgrammingSession();

        //step4
        TestStep("Enter programming session unlock ECU and if SBL is supported, SBL should be downloaded and actived");
        TestSeq_DownloadSBL(1, 0, 1, 0);
        APP_File[3] = '0';
        vbfparser(APP_File);
        for (int i = 0; i < APPFileNum; i++)
        {

            vbfparser(APP_File);
            filename[0] = '\0';
            if (app.get_system_var_string("Diagnostic0.下载文件_DataFile", 512, filename) != 0 || filename[0] == '\0') {
                printf("Error: Failed to get filename for %s", APP_File);
                snprintf(StepDespriction, sizeof(StepDespriction), "Download APP%d (no filename)", i);
            }
            else {
                // 安全截断文件名
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

            SID31_RoutineControl(0x01, FileSignature, VbfFileInfo.sw_signature_dev.count, VbfFileInfo.sw_signature_dev.array);
            CheckPosResponse_SID31(0x01, FileSignature, 2, ExpectedResponse);

            APP_File[3]++;
        }

        //step11
        stepnumber = 10;
        TestStep("Check Complete And Compatible");
        //TD文档内描述不清晰，期望结果未知
        CheckCompleteAndCompatible();

        //step12
        TestStep("ECU Reset");
        SID11_ECUReset(0x01, Functional);

        TestStep("Check Current Session");
        CheckCurrentSession(0x01);

        //step13
        TestStep("Check Software Version");
        CheckSoftwareVersion();

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

s32 test_case_boot_selfUpdate()
{
    try {
        //test刷写全流程
        //strcpy(TestCaseName, "TC_1021823_File Download Full Sequence Test - Physical Addressing");
        strcpy(TestCaseName, "TC_003_Boot自更新");
        strcpy(TestScopeName, "Software Download");
        strcpy(testPurpose_CH, "测试目的：");

        TestPrecondition();

        TestSeq_EnterProgrammingSession();

        u8 DID1[2] = { 0xED,0x20 };
        SID22_ReadDataByIdentifier(DID1);
        CheckPosResponse_SID22(DID1);

        u8 DID2[2] = { 0xF1,0xAA };
        SID22_ReadDataByIdentifier(DID2);
        CheckPosResponse_SID22(DID2);

        u8 DID_D94B[2] = { 0xD9,0x4B };
        u8 D94B_Version[13] = { 0 };
        SID22_ReadDataByIdentifier(DID_D94B);
        StoreArray(D94B_Version, TpRxData, 12, 4, 0);
        D94B_Version[12] = '\0';
        snprintf(StepDespriction, sizeof(StepDespriction), "当前版本号为：%s", D94B_Version);
        TestReport_HTML_StepDescription(StepDespriction);

        TestSeq_DownloadSBL(0);

        u8 DID3[2] = { 0xF1,0xA2 };
        SID22_ReadDataByIdentifier(DID3);
        CheckNegResponse(0x31);

        ActivateSBL();

        SID22_ReadDataByIdentifier(DID1);
        CheckPosResponse_SID22(DID1);

        SID22_ReadDataByIdentifier(DID3);
        CheckPosResponse_SID22(DID3, 8);

        char keyvalue[20] = "Boot";
        TestReport_HTML_StepDescription("----Download SelfUpdate APP----");
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

        NMWakeUp(0);
        SendTesterPresent(TP_OFF);
        TestStep("Wait DUT Recovery Communcation");
        TestWait(60000);

        SID11_ECUReset(0x81,1);
        CheckNonResponse();
        TestWait(ECUResetTime);

        NMWakeUp(1);
        SendTesterPresent(TP_ON);

        CheckCurrentSession(0x02);

        //u8 DID_D94B[2] = { 0xD9,0x4B };
        //u8 D94B_Version[13] = { 0 };
        SID22_ReadDataByIdentifier(DID_D94B);
        StoreArray(D94B_Version, TpRxData, 12, 4, 0);
        D94B_Version[12] = '\0';
        snprintf(StepDespriction, sizeof(StepDespriction), "当前版本号为：%s", D94B_Version);
        TestReport_HTML_StepDescription(StepDespriction);


        TestReport_HTML_StepDescription("--------开始正向刷写--------");

        TestSeq_EnterProgrammingSession();

        SID22_ReadDataByIdentifier(DID1);
        CheckPosResponse_SID22(DID1);


        SID22_ReadDataByIdentifier(DID2);
        CheckPosResponse_SID22(DID2);

        TestSeq_DownloadSBL(0);

        SID22_ReadDataByIdentifier(DID3);
        CheckNegResponse(0x31);

        ActivateSBL();

        SID22_ReadDataByIdentifier(DID1);
        CheckPosResponse_SID22(DID1);

        SID22_ReadDataByIdentifier(DID3);
        CheckPosResponse_SID22(DID3, 8);

        char keyvalue_APP[20] = "APP0Path";
        TestReport_HTML_StepDescription("----Download APP----");
        vbfparser(keyvalue_APP);
        for (int i = 0; i < APPFileNum; i++)
        {
            char StepDespriction[256];
            char filename[300] = "";
            vbfparser(keyvalue_APP);
            app.get_system_var_string("Diagnostic0.下载文件_DataFile", 512, &filename[0]);
            filename[strlen(filename) - 9] = '\0';
            snprintf(StepDespriction, sizeof(StepDespriction), "Download File is %s ", filename);
            TestReport_HTML_StepDescription(StepDespriction);
            EraseAddress();
            FileDownload();
            TestReport_HTML_StepDescription("----Check Signature----");
            CheckSignature();
            keyvalue_APP[3]++;
        }

        if (0 != CheckCompleteAndCompatible()) {
            strcpy(issuereason, "");
            TestReport_HTML_StepDescription("检查完整性和兼容性不通过！", Fail);
        }

        SID11_ECUReset(0x81,1);
        CheckNonResponse();
        TestWait(ECUResetTime);

        CheckCurrentSession(0x01);

        RX_SID19_ReportDTCByStatusMask(0x08);
        RX_CheckPosResponse_SID19_02(0xFF);

        CheckSoftwareVersion();

        CheckCurrentSession(0x01);

        PowerReset(30000);
        TestWait(ECUResetTime);

        CheckCurrentSession(0x01);

        RX_SID19_ReportDTCByStatusMask(0x09);     //vector这里发的是19 02 09
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

s32 test_case_1021825()
{
    try {
        strcpy(TestCaseName, "TC_1021825_File Download Duplicate Block Sequence Counter - Physical Addressing");
        strcpy(TestScopeName, "Software Download");
        strcpy(testPurpose_CH, "测试目的：");

        TestPrecondition();
        ECUResetTime = 6000;
        char keyvalue[20] = "SBL_Path";

        //SID11_ECUReset(0x81,1);
        //CheckNonResponse();
        //TestWait(ECUResetTime);

        TestSeq_EnterProgrammingSession();

        //CheckCurrentSession(0x01);

        SID10_SessionControl(0x82);
        CheckNonResponse();
        TestWait(ECUResetTime);

        SID10_SessionControl(0x02);
        CheckPosResponse_SID10(0x02);

        TestWait(ECUResetTime);

        CheckCurrentSession(0x02);

        TestSeq_DownloadSBL(1, 1);

        TestSeq_DownloadAPP(1);

        CheckCompleteAndCompatible();
        //AppWait(5000, "");
        SID11_ECUReset(0x81,1);
        CheckNonResponse();
        TestWait(ECUResetTime);

        CheckCurrentSession(0x01);

        RX_SID19_ReportDTCByStatusMask(0x08);
        RX_CheckPosResponse_SID19_02(0xFF);

        CheckCurrentSession(0x01);

        CheckSoftVersion();

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

s32 test_case_1021841()
{
    try {

        snprintf(StepDespriction, sizeof(StepDespriction), "TC_1021841_Robustness of File Download Full Sequence Test with Configurable Tester Present Periodicity - Physic");
        strcpy(TestCaseName, StepDespriction);
        strcpy(TestScopeName, "Robustness");
        strcpy(testPurpose_CH, "测试目的：");

        TestPrecondition();

        for (int i = 0; i < robustnessTimes; i++)
        {
            SendTesterPresent(TP_OFF);
            SendTesterPresent(TP_ON, 100);
            snprintf(StepDespriction, sizeof(StepDespriction), "Robustness %dth Times", i + 1);
            TestReport_HTML_StepDescription(StepDespriction);
            TestSeq_EnterProgrammingSession();

            u8 DID[2] = { 0xed,0x20 };
            SID22_ReadDataByIdentifier(DID);
            CheckPosResponse_SID22(DID);

            TestSeq_DownloadSBL();

            TestSeq_DownloadAPP();

            CheckCompleteAndCompatible();
            SID11_ECUReset(0x81, 1);
            CheckNonResponse();
            TestWait(ECUResetTime);

            CheckCurrentSession(0x01);

            RX_SID19_ReportDTCByStatusMask(0x08);
            RX_CheckPosResponse_SID19_02(0xFF);

            CheckSoftVersion();

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

s32 test_case_1365394()
{
    try {
        //FileDownloadFullSequenceForOTA_OTANormalInstallationSequence
        strcpy(TestCaseName, "TC_1365394_FileDownloadFullSequenceForOTA_OTANormalInstallationSequence");
        strcpy(TestScopeName, "OTA");
        strcpy(testPurpose_CH, "测试目的：");
        TestPrecondition();
        OTA_Init();
        CheckCurrentSession(0x01, Functional);//1
        OTA_SessionControl(0x83);//2
        OTA_ControlDTCSetting(0x82, Functional);//3
        OTA_CommunicationControl(0x83, 0x01, Functional);//3.5
        CheckProgrammingPrecondition();//4
        OTA_SessionControl(0x82, Functional, 1000, 16);//5
        TestWait(ECUResetTime);
        CheckCurrentSession(0x02);//6
        SID10_SessionControl(0x02);//7
        CheckPosResponse_SID10(0x02);
        DownloadSBL();//8-13
        //步骤13，14疑似重复
        ActivateSBL();//15
        TestSeq_DownloadAPP();//16-25
        CheckCompleteAndCompatible();//26
        SID11_ECUReset(0x81, Functional);//27
        CheckNonResponse();
        TestWait(70000);//28
        CheckCurrentSession(0x01);//29
        CheckSoftwareVersion();//30
        SID14_ClearAllDTC(Functional);//31

        CheckSoftVersion();

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


#endif // !TestCase_CAN_SWDL