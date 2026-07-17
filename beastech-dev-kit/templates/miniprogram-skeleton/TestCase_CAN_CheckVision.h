#ifndef TestCase_CAN_CheckVision
#define TestCase_CAN_CheckVision

#include "test_case_diag_api.h"

/////////////////////////////////////////////////////////////APP////////////////////////////////////////////////////////////

s32 TC_Check_Sofeware_Version() {
    try {
        strcpy(TestCaseName, "TC_Check_Sofeware_Version");
        strcpy(TestScopeName, "Software Version");
        strcpy(testPurpose_CH, "测试目的：");

        TestPrecondition();

        TestReport_HTML_StepDescription("切换至编程会话");

        SID10_SessionControl(ProgrammingSession);
        CheckPosResponse_SID10(ProgrammingSession);

        CheckCurrentSession(ProgrammingSession);

        TestReport_HTML_StepDescription("检查Boot版本号");

        u8 DID_D94B[2] = { 0xD9,0x4B };
        u8 D94B_Version[13] = { 0 };
        SID22_ReadDataByIdentifier(DID_D94B);
        CheckPosResponse_SID22(DID_D94B);
        StoreArray(D94B_Version, TpRxData, 12, 3, 0);
        D94B_Version[12] = '\0';
        snprintf(StepDespriction, sizeof(StepDespriction), "当前版本号为：%s", D94B_Version);
        TestReport_HTML_StepDescription(StepDespriction);


        TestReport_HTML_StepDescription("读取ECU Core Assembly Part Number");
        u8 DID_F1AA[2] = { 0xF1,0xAA };
        u8 F1AA_Version[8] = { 0 };
        SID22_ReadDataByIdentifier(DID_F1AA);
        CheckPosResponse_SID22(DID_F1AA);

        TestReport_HTML_StepDescription("读取PBL Software Part Number");
        u8 DID_F1A5[2] = { 0xF1,0xA5 };
        u8 F1A5_Version[8] = { 0 };
        SID22_ReadDataByIdentifier(DID_F1A5);
        CheckPosResponse_SID22(DID_F1A5);

        TestReport_HTML_StepDescription("切换至默认会话");

        SID10_SessionControl(DefaultSession);
        CheckPosResponse_SID10(DefaultSession);

        CheckCurrentSession(DefaultSession);

        TestReport_HTML_StepDescription("检查APP版本号");


        SID22_ReadDataByIdentifier(DID_D94B);
        if (0 == CheckPosResponse_SID22(DID_D94B)) {
            StoreArray(D94B_Version, TpRxData, 12, 3, 0);
            D94B_Version[12] = '\0';
            snprintf(StepDespriction, sizeof(StepDespriction), "当前版本号为：%s", D94B_Version);
            TestReport_HTML_StepDescription(StepDespriction);
        }

        char APP_File[100] = "APP0Path";
        //解析softVersion
        APP_File[3] = '0';
        for (u8 i = 0; i < APPFileNum; i++) {
            vbfparser(APP_File);
            APP_File[3]++;
        }

        TestReport_HTML_StepDescription("检查Software版本号");

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

#endif // !TestCase_CAN_CheckVision
