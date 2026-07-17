#ifndef TestCase_CAN_SecurityUnlock
#define TestCase_CAN_SecurityUnlock

#include "test_case_diag_api.h"

s32 test_case_1022401()
{
    try {
        strcpy(TestCaseName, "TC_1022401_Service 0x27 Valid physical extendedDiagnosticSession supported service");
        strcpy(TestScopeName, "Security Unlock");
        strcpy(testPurpose_CH, "测试目的：");

        TestPrecondition();
            int level = 0;
            int levelx = 0;
            TestReport_HTML_StepDescription("Step: Enter Extended Session", Info);
            SID10_SessionControl(0x03);
            CheckPosResponse_SID10(0x03);

            TestReport_HTML_StepDescription("Step: Check Current Session", Info);
            CheckCurrentSession(0x03);
            //qq，检查是否支持0x27，返回level和levelx
            s8 flag = check_if_support_0x27();
            if (flag != -1)
            {
                for (int i = 0; i < SupportServices_Security[flag].subfuncsize; i++)
                {
                    u8 level_levelx[2] = { 0 };
                    memcpy(level_levelx, return_level_levelx(i, flag, 3), 2);
                    level = level_levelx[0];
                    levelx = level_levelx[1];
                    if (level == 0 || levelx == 0 || level == levelx)
                        continue;
                    if (gCEMFlg == 0) {
                        if (level == 3 || levelx == 3)
                            continue;
                    }
                    log("levelqq:0x%x,levelxqq:0x%x", level_levelx[0], level_levelx[1]);

                    //1
                    TestReport_HTML_StepDescription("Step: Request seed", Info);
                    SID27_SecurityAccess_RequestSeed(level);
                    CheckPosResponse_SID27_RequestSeed(level);
                    if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
                    {
                        TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
                    }

                    GenSAKeyExOpt(level);
                    KeyArray[2]++;
                    TestReport_HTML_StepDescription("Step: Send incorrect key", Info);
                    SID27_SecurityAccess_SendKey(level);
                    CheckNegResponse(0x35);

                    //2
                    TestReport_HTML_StepDescription("Step: Request seed", Info);
                    SID27_SecurityAccess_RequestSeed(level);
                    CheckPosResponse_SID27_RequestSeed(level);
                    if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
                    {
                        TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
                    }

                    GenSAKeyExOpt(level);
                    KeyArray[2]++;
                    TestReport_HTML_StepDescription("Step: Send incorrect key", Info);
                    SID27_SecurityAccess_SendKey(level);
                    CheckNegResponse(0x36);
                    if (level == 0x01) {
                        Diag_DelayTimer = 60000;
                    }
                    else {
                        Diag_DelayTimer = 10000;
                    }
                    TestInfoPrint("Step: Wait Diag_DelayTimer - 1s  %dms", Diag_DelayTimer - 1000);
                    //TestReport_HTML_StepDescription("Step: Wait 9s", Info);
                    app.wait(Diag_DelayTimer - 1000, "");

                    TestReport_HTML_StepDescription("Step: Request seed", Info);
                    SID27_SecurityAccess_RequestSeed(level);
                    CheckNegResponse(0x37);

                    TestReport_HTML_StepDescription("Step: Wait 2s", Info);
                    app.wait(2000, "");

                    TestReport_HTML_StepDescription("Step: Request seed", Info);
                    SID27_SecurityAccess_RequestSeed(level);
                    CheckPosResponse_SID27_RequestSeed(level);
                    if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
                    {
                        TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
                    }

                    GenSAKeyExOpt(level);
                    TestReport_HTML_StepDescription("Step: Send valid key", Info);
                    SID27_SecurityAccess_SendKey(level);
                    CheckPosResponse_SID27_SendKey(level);

                    TestReport_HTML_StepDescription("Step: Enter Extended Session", Info);
                    SID10_SessionControl(0x03);
                    CheckPosResponse_SID10(0x03);

                    TestReport_HTML_StepDescription("Step: Check Current Session", Info);
                    CheckCurrentSession(0x03);

                    TestReport_HTML_StepDescription("Step: Request seed", Info);
                    SID27_SecurityAccess_RequestSeed(level);
                    CheckPosResponse_SID27_RequestSeed(level);
                    if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
                    {
                        TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
                    }

                    GenSAKeyExOpt(level);
                    TestReport_HTML_StepDescription("Step: Send valid key", Info);
                    SID27_SecurityAccess_SendKey(level);
                    CheckPosResponse_SID27_SendKey(level);

                    TestReport_HTML_StepDescription("Step: Change to default session", Info);
                    SID10_SessionControl(DefaultSession);
                    CheckPosResponse_SID10(DefaultSession);

                    TestReport_HTML_StepDescription("Step: Check Current Session", Info);
                    CheckCurrentSession(DefaultSession);

                    TestReport_HTML_StepDescription("Step: Enter Extended Session", Info);//15
                    SID10_SessionControl(0x03);
                    CheckPosResponse_SID10(0x03);

                    TestReport_HTML_StepDescription("Step: Check Current Session", Info);
                    CheckCurrentSession(0x03);

                    TestReport_HTML_StepDescription("Step: Request seed", Info);
                    SID27_SecurityAccess_RequestSeed(level);
                    CheckPosResponse_SID27_RequestSeed(level);
                    if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
                        TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);

                    GenSAKeyExOpt(level);
                    TestReport_HTML_StepDescription("Step: Send valid key", Info);//18
                    SID27_SecurityAccess_SendKey(level);
                    CheckPosResponse_SID27_SendKey(level);

                    //TestReport_HTML_StepDescription("Step: Enter default Session", Info);
                    //SID10_SessionControl(0x01);
                    //CheckPosResponse_SID10(0x01);

                    //TestReport_HTML_StepDescription("Step: Check Current Session", Info);
                    //CheckCurrentSession(0x01);

                    //TestReport_HTML_StepDescription("Step: Enter Extended Session", Info);
                    //SID10_SessionControl(0x03);
                    //CheckPosResponse_SID10(0x03);

                    //TestReport_HTML_StepDescription("Step: Check Current Session", Info);
                    //CheckCurrentSession(0x03);//22

                    ////23
                    //TestReport_HTML_StepDescription("Step: Request seed", Info);
                    //SID27_SecurityAccess_RequestSeed(level);
                    //CheckPosResponse_SID27_RequestSeed(level);
                    //if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
                    //{
                    //    TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
                    //}

                    //GenSAKeyExOpt(level);
                    ////24
                    //TestReport_HTML_StepDescription("Step: Send valid key", Info);
                    //SID27_SecurityAccess_SendKey(level);
                    //CheckPosResponse_SID27_SendKey(level);
                    //25
                    TestReport_HTML_StepDescription("Step: Stop TesterPresent and wait 4.8s", Info);//25
                    SendTesterPresent(TP_OFF);
                    app.wait(4800, "");
                    //26
                    TestReport_HTML_StepDescription("Step: Request seed", Info);
                    SID27_SecurityAccess_RequestSeed(level);
                    CheckPosResponse_SID27_RequestSeed(level);
                    if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
                    {
                        TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Pass);
                    }
                    else
                    {
                        TestReport_HTML_StepDescription("Seed received not equal to 0x000000!", Fail);
                    }

                    TestReport_HTML_StepDescription("Step: Check Current Session", Info);
                    CheckCurrentSession(0x03);

                    TestReport_HTML_StepDescription("Step: Stop TesterPresent and wait 6s", Info);
                    app.wait(6000, "");

                    TestReport_HTML_StepDescription("Step: Check Current Session", Info);
                    CheckCurrentSession(DefaultSession);

                    SendTesterPresent(TP_ON);

                    TestReport_HTML_StepDescription("Step: Enter Extended Session， SPRMIB = True", Info);
                    SID10_SessionControl(0x03 + 0x80);
                    CheckNonResponse();

                    TestReport_HTML_StepDescription("Step: Check Current Session", Info);
                    CheckCurrentSession(0x03);

                    TestReport_HTML_StepDescription("Step: Request seed", Info);
                    SID27_SecurityAccess_RequestSeed(level);
                    CheckPosResponse_SID27_RequestSeed(level);
                    if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
                    {
                        TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
                    }
                    u8 SeedTemp1[3]; u8 SeedTemp2[3];
                    memcpy(SeedTemp1, SeedArray, 3);

                    TestReport_HTML_StepDescription("Step: Request seed", Info);
                    SID27_SecurityAccess_RequestSeed(level);
                    CheckPosResponse_SID27_RequestSeed(level);
                    if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
                    {
                        TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
                    }
                    memcpy(SeedTemp2, SeedArray, 3);
                    log("%x,%x", SeedTemp1[0], SeedTemp2[0]);
                    if (0 == memcmp(SeedTemp1, SeedTemp2, 3))
                        TestReport_HTML_StepDescription("Seed is not random!", Fail);
                    else
                        TestReport_HTML_StepDescription("Seed is random!", Pass);
                }
            }
            TestReport_HTML_StepDescription("Step: Check Current Session", Info);
            CheckCurrentSession(0x03);

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
s32 test_case_1022403()
{
    try {
        u8 NRC[2] = { 0x12,0x7E };
        strcpy(TestCaseName, "TC_1022403_Service 0x27 Valid physical notsubfunction extendedDiagnosticSession supported service");
        strcpy(TestScopeName, "Security Unlock");
        strcpy(testPurpose_CH, "测试目的：");

        TestPrecondition();

        int level = 0;

        TestReport_HTML_StepDescription("Step: Enter Extended Session", Info);
        SID10_SessionControl(0x03);
        CheckPosResponse_SID10(0x03);

        TestReport_HTML_StepDescription("Step: Check Current Session", Info);
        CheckCurrentSession(0x03);

        SID27_SecurityAccess_RequestSeed(0x01);
        u8 nrc[2] = { 0x12,0x7E };
        CheckNegResponse(nrc);

        TestReport_HTML_StepDescription("Step: Check Current Session", Info);
        CheckCurrentSession(0x03);


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
s32 test_case_1022495()
{
    try {
        strcpy(TestCaseName, "TC_1022495_SecurityAccess (0x27) - Physical Programming Session supported service");
        strcpy(TestScopeName, "Security Unlock");
        strcpy(testPurpose_CH, "测试目的：");

        TestPrecondition();

        int level = 0;

        TestReport_HTML_StepDescription("Step: Enter Extended Session", Info);
        SID10_SessionControl(0x02);
        CheckPosResponse_SID10(0x02);
        TestWait(ECUResetTime);

        TestReport_HTML_StepDescription("Step: Check Current Session", Info);
        CheckCurrentSession(0x02);

        int nummmm = -1;
        for (int ii = 0; ii < sizeof(SupportServices_Security); ii++)
        {
            if (SupportServices_Security[ii].service == 0x27)
            {
                nummmm = ii;
                break;
            }
        }
        if (nummmm == -1)
        {
            TestReport_HTML_StepDescription("不支持0x27服务", Warning);
            return 0;
        }
        level = 0x01;
        //3
        TestReport_HTML_StepDescription("Step: Request seed", Info);
        SID27_SecurityAccess_RequestSeed(level);
        CheckPosResponse_SID27_RequestSeed(level);
        if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
        {
            TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
        }
        GenSAKeyExOpt(level);
        KeyArray[15]++;

        TestReport_HTML_StepDescription("Step: Send invalid key", Info);
        SID27_SecurityAccess_SendKey(level);//5
        CheckNegResponse(0x35);
        //6
        TestReport_HTML_StepDescription("Step: Request seed", Info);
        SID27_SecurityAccess_RequestSeed(level);
        CheckPosResponse_SID27_RequestSeed(level);
        if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
        {
            TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
        }
        GenSAKeyExOpt(level);
        KeyArray[15]++;

        //8
        TestReport_HTML_StepDescription("Step: Send invalid key", Info);
        SID27_SecurityAccess_SendKey(level);
        CheckNegResponse(0x36);
        if (level == 0x01) {
            Diag_DelayTimer = 60000;
        }
        else {
            Diag_DelayTimer = 10000;
        }
        TestReport_HTML_StepDescription("Step: Wait for DelayTimer -1s", Info);
        TestWait(Diag_DelayTimer - 1000);
        //10
        TestReport_HTML_StepDescription("Step: Request seed", Info);
        SID27_SecurityAccess_RequestSeed(level);
        CheckNegResponse(0x37);
        TestReport_HTML_StepDescription("Step: Wait for 2s", Info);
        TestWait(2000);
        //12
        TestReport_HTML_StepDescription("Step: Request seed", Info);
        SID27_SecurityAccess_RequestSeed(level);
        CheckPosResponse_SID27_RequestSeed(level);
        GenSAKeyExOpt(level);

        TestReport_HTML_StepDescription("Step: Send valid key", Info);
        SID27_SecurityAccess_SendKey(level);
        CheckPosResponse_SID27_SendKey(level);
        //15
        SID10_SessionControl(0x02);
        CheckPosResponse_SID10(0x02);
        TestWait(Session02_Time);

        CheckCurrentSession(0x02);

        //17
        TestReport_HTML_StepDescription("Step: Request seed", Info);
        SID27_SecurityAccess_RequestSeed(level);
        CheckPosResponse_SID27_RequestSeed(level);
        if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
        {
            TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
        }

        GenSAKeyExOpt(level);
        //KeyArray[15]++;
        //18
        TestReport_HTML_StepDescription("Step: Send valid key", Info);
        SID27_SecurityAccess_SendKey(level);
        CheckPosResponse_SID27_SendKey(level);

        //19
        SID10_SessionControl(0x01);
        CheckPosResponse_SID10(0x01);
        TestWait(2000);
        CheckCurrentSession(0x01);
        //21
        SID10_SessionControl(0x02);
        CheckPosResponse_SID10(0x02);
        TestWait(Session02_Time);

        CheckCurrentSession(0x02);
        //23
        TestReport_HTML_StepDescription("Step: Request seed", Info);
        SID27_SecurityAccess_RequestSeed(level);
        CheckPosResponse_SID27_RequestSeed(level);
        if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
        {
            TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
        }

        GenSAKeyExOpt(level);
        // KeyArray[15]++;
         //24
        TestReport_HTML_StepDescription("Step: Send valid key", Info);
        SID27_SecurityAccess_SendKey(level);
        CheckPosResponse_SID27_SendKey(level);
        //25
        TestReport_HTML_StepDescription("Step: Stop TesterPresent and wait 4.8s", Info);
        SendTesterPresent(TP_OFF);
        app.wait(4800, "");
        //26
        TestReport_HTML_StepDescription("Step: Request seed", Info);
        SID27_SecurityAccess_RequestSeed(level);
        CheckPosResponse_SID27_RequestSeed(level);
        if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
        {
            TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Pass);
        }
        else
        {
            TestReport_HTML_StepDescription("Seed received not equal to 0x000000!", Fail);
        }
        //27
        TestReport_HTML_StepDescription("Step: Stop TesterPresent and wait 4s", Info);
        app.wait(4000, "");
        //28
        TestReport_HTML_StepDescription("Step: Request seed", Info);
        SID27_SecurityAccess_RequestSeed(level);
        CheckPosResponse_SID27_RequestSeed(level);
        if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
        {
            TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Pass);
        }
        else
        {
            TestReport_HTML_StepDescription("Seed received not equal to 0x000000!", Fail);
        }
        //29
        TestReport_HTML_StepDescription("Step: Check Current Session", Info);
        //TestWait(6000);   //mc
        CheckCurrentSession(0x02);
        //30
        TestWait(Diag_UDS_S3Server + ECUResetTime + 500);
        SendTesterPresent(TP_ON);
        //31
        //TestWait(6000);   
        CheckCurrentSession(0x01);
        //32
        TestReport_HTML_StepDescription("Step: Enter Extended Session, SPRMIB = True", Info);
        SID10_SessionControl(0x82);
        CheckNonResponse();
        TestWait(Session02_Time);
        //33
        TestReport_HTML_StepDescription("Step: Check Current Session", Info);
        CheckCurrentSession(0x02);
        //34
        TestReport_HTML_StepDescription("Step: Request seed", Info);
        SID27_SecurityAccess_RequestSeed(level);
        CheckPosResponse_SID27_RequestSeed(level);
        if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
        {
            TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
        }
        u8 SeedTemp1[3]; u8 SeedTemp2[3];
        memcpy(SeedTemp1, SeedArray, 3);
        //35
        TestReport_HTML_StepDescription("Step: Request seed", Info);
        SID27_SecurityAccess_RequestSeed(level);
        CheckPosResponse_SID27_RequestSeed(level);
        if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
        {
            TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
        }
        memcpy(SeedTemp2, SeedArray, 3);//对比3个就，一样返回0
        log("SeedTemp1[2]:0x%x", SeedTemp1[2]);
        log("SeedTemp2[2]:0x%x", SeedTemp2[2]);
        //67 01 后面开始是[0]
        if (0 == memcmp(SeedTemp1, SeedTemp2, 3))
        {
            TestReport_HTML_StepDescription("Seed is not random!", Fail);
        }
        //36
        for (int ii = 0x03; ii <= 0xFF; ii = ii + 2)
        {
            log("level:%x", level);
            log("ii:%x", ii);

            if (ii == 0x81)
                ii += 2;
            level = ii;

            SID27_SecurityAccess_RequestSeed(level);
            u8 nrc[2] = { 0x12,0x7e };
            CheckNegResponse(nrc);

        }

        //for (int ii = 0; ii < SupportServices_Security[nummmm].subfuncsize; ii++)
        //{
        //    level = SupportServices_Security[nummmm].subfunc[ii].Subfunc;
        //    if (SupportServices_Security[nummmm].subfunc[ii].Subfunc == 0x01)continue;
        //    if (SupportServices_Security[nummmm].subfunc[ii].Subfunc == 0x81)
        //        level += 2;

        //    SID27_SecurityAccess_RequestSeed(level);

        //    u8 nrc[2] = { 0x12,0x7e };
        //    CheckNegResponse(nrc);
        //}

        //40
        TestReport_HTML_StepDescription("Step: Check Current Session", Info);
        TestWait(Session02_Time);
        CheckCurrentSession(0x02);

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
s32 test_case_1022762()
{
    try {
        u8 N = 2;
        strcpy(TestCaseName, "TC_1022762_Seed and Key Validity Details Test valid physical extendedDiagnosticSession");
        strcpy(TestScopeName, "Security Unlock");
        strcpy(testPurpose_CH, "测试目的：");

        TestPrecondition();

        SID10_SessionControl(0x03);
        CheckPosResponse_SID10(0x03);

        CheckCurrentSession(0x03, Physical);

        int nummmm = -1;
        for (int ii = 0; ii < sizeof(SupportServices_Security); ii++)
        {
            if (SupportServices_Security[ii].service == 0x27)
            {
                nummmm = ii;//=5
                break;
            }
            //else
            //    printf("不支持27服务");
        }
        if (nummmm == -1)
        {
            printf("不支持27服务");
            return -1;

        }

        int level = 0, levelx = 0;
        int flag = 0;

        for (int j = 0; j < SupportServices_Security[nummmm].subfuncsize; j++)
        {
            flag = 0;
            int flag_step3to12 = 0;
            int flag_step36to44 = 0;

            level = SupportServices_Security[nummmm].subfunc[j].Subfunc;
            if (SupportServices_Security[nummmm].subfunc[j].Sup_Ext != 1)continue;
            log("flag%d", flag);//一直是0

            for (int m = 0; m < SupportServices_Security[nummmm].subfuncsize; m++)
            {
                if (m > j && SupportServices_Security[nummmm].subfunc[m].Sup_Ext != 0)
                {
                    flag = m;
                    break;
                }
            }

            if (flag != 0)
                levelx = SupportServices_Security[nummmm].subfunc[flag].Subfunc;//set LX = next security level
            else
            {
                //printf("TestCase Not Appliable!");
                //TestPostCondition_TP_CANFD();
                //return 0;
                flag_step3to12 = 1;
                flag_step36to44 = 1;
            }

            if (flag_step3to12 != 1)
            {
                if (gCEMFlg == 0) {
                    if (level == 3 || levelx == 3)
                        continue;
                }
                //5
                SID10_SessionControl(0x03);
                CheckPosResponse_SID10(0x03);
                TestWait(300);
                CheckCurrentSession(0x03);
                //mc
                TestReport_HTML_StepDescription("Step: Request seed", Info);
                SID27_SecurityAccess_RequestSeed(level);
                CheckPosResponse_SID27_RequestSeed(level);
                if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
                {
                    TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
                }

                GenSAKeyExOpt(level);
                //7
                TestReport_HTML_StepDescription("Step: Send valid key，LX+1", Info);
                SID27_SecurityAccess_SendKey(levelx);
                CheckNegResponse(0x24);
                //8
                TestReport_HTML_StepDescription("Step: Send valid key，L2", Info);
                SID27_SecurityAccess_SendKey(level);
                CheckNegResponse(0x24);

                //9
                TestReport_HTML_StepDescription("Step: Request seed", Info);
                SID27_SecurityAccess_RequestSeed(level);
                CheckPosResponse_SID27_RequestSeed(level);
                if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
                {
                    TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
                }

                GenSAKeyExOpt(level);

                //10
                TestReport_HTML_StepDescription("Step: Request seed", Info);
                SID27_SecurityAccess_RequestSeed(levelx);
                CheckPosResponse_SID27_RequestSeed(levelx);
                if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
                {
                    TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
                }

                GenSAKeyExOpt(levelx);

                //11
                TestReport_HTML_StepDescription("Step: Send valid key，L2", Info);
                SID27_SecurityAccess_SendKey(level);
                CheckNegResponse(0x24);
            }
            //12
            int flag19 = 0;
            char pppchar[100];
            TestReport_HTML_StepDescription("Step: Request seed", Info);
            SID27_SecurityAccess_RequestSeed(level);
            if (level == 25) flag19 = 1;
            CheckPosResponse_SID27_RequestSeed(level);
            //waringflag1
            /*sprintf(pppchar, "this is level 1 = %d", level);
            TestReport_HTML_StepDescription(pppchar);
            TestReport_HTML_StepDescription("this is 1", Warning);*/
            if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
            {
                if (flag19)  TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Pass);
                else TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
            }

            GenSAKeyExOpt(level);

            //13
            //TestWait(9000);        //mc
            TestReport_HTML_StepDescription("Step: Send valid key，L2", Info);
            SID27_SecurityAccess_SendKey(level);
            if (flag19) CheckNegResponse(0x24);
            else CheckPosResponse_SID27_SendKey(level);

            //waringflag2
            /*sprintf(pppchar, "this is level 2 = %d", level);
            TestReport_HTML_StepDescription(pppchar);
            TestReport_HTML_StepDescription("this is 2", Warning);*/

            //14
            SendTesterPresent(TP_OFF);
            TestWait(6000);
            SendTesterPresent(TP_ON);

            CheckCurrentSession(0x01);

            SID10_SessionControl(0x83);
            CheckNonResponse();

            CheckCurrentSession(0x03);

            //18
            TestReport_HTML_StepDescription("Step: Request seed", Info);
            SID27_SecurityAccess_RequestSeed(level);
            CheckPosResponse_SID27_RequestSeed(level);
            if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
            {
                TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
            }
            GenSAKeyExOpt(level);

            SendTesterPresent(TP_OFF);
            TestWait(6000);
            SendTesterPresent(TP_ON);

            CheckCurrentSession(0x01);

            SID10_SessionControl(0x83);
            CheckNonResponse();

            CheckCurrentSession(0x03);

            //23
            //TestWait(9000); 
            TestReport_HTML_StepDescription("Step: Send valid key", Info);
            SID27_SecurityAccess_SendKey(level);
            CheckNegResponse(0x24);


            for (u8 B = 1; B < (N + 1); B++)//执行2次
            {
                //27
                TestReport_HTML_StepDescription("Step: Request seed", Info);
                SID27_SecurityAccess_RequestSeed(level);
                CheckPosResponse_SID27_RequestSeed(level);
                if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
                {
                    TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
                }

                GenSAKeyExOpt(level);

                //28
                KeyArray[15]++;
                TestReport_HTML_StepDescription("Step: Send invalid key", Info);
                SID27_SecurityAccess_SendKey(level);
                if (B == 2) CheckNegResponse(0x36);
                else CheckNegResponse(0x35);
            }

            //30
            //TestWait(15000);  //mc
            if (level == 0x01) {
                Diag_DelayTimer = 60000;
            }
            else {
                Diag_DelayTimer = 10000;
            }
            TestInfoPrint("Step: Wait Diag_DelayTimer %dms", Diag_DelayTimer);
            TestWait(Diag_DelayTimer);
            TestReport_HTML_StepDescription("Step: Request seed", Info);
            SID27_SecurityAccess_RequestSeed(level);
            CheckPosResponse_SID27_RequestSeed(level);
            if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
            {
                TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
            }

            GenSAKeyExOpt(level);

            //31
            TestReport_HTML_StepDescription("Step: Send valid key", Info);
            SID27_SecurityAccess_SendKey(level);
            CheckPosResponse_SID27_SendKey(level);

            //CheckCurrentSession(0x01);
            CheckCurrentSession(0x03); // ------

            SID10_SessionControl(0x83);
            CheckNonResponse();

            CheckCurrentSession(0x03);

            //36
            //TestWait(9000);
            if (level == 0x01) {
                Diag_DelayTimer = 60000;
            }
            else {
                Diag_DelayTimer = 10000;
            }
            TestInfoPrint("Step: Wait Diag_DelayTimer - 1s  %dms", Diag_DelayTimer - 1000);
            TestWait(Diag_DelayTimer - 1000);
            TestReport_HTML_StepDescription("Step: Request seed", Info);
            SID27_SecurityAccess_RequestSeed(level);
            CheckPosResponse_SID27_RequestSeed(level);
            if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
            {
                TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
            }

            GenSAKeyExOpt(level);

            if (flag_step36to44 != 1)
            {
                TestInfoPrint("Step: Wait Diag_DelayTimer - 1s  %dms", Diag_DelayTimer - 1000);
                TestWait(Diag_DelayTimer - 1000);
                //TestWait(9000); //mc
                //38
                TestReport_HTML_StepDescription("Step: Request seed", Info);
                SID27_SecurityAccess_RequestSeed(level);
                CheckPosResponse_SID27_RequestSeed(level);
                if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
                {
                    TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
                }

                GenSAKeyExOpt(level);

                //39
                TestReport_HTML_StepDescription("Step: Send valid key", Info);
                SID27_SecurityAccess_SendKey(level);
                CheckPosResponse_SID27_SendKey(level);

                //40
                TestReport_HTML_StepDescription("Step: Request seed", Info);
                SID27_SecurityAccess_RequestSeed(levelx);
                CheckPosResponse_SID27_RequestSeed(levelx);
                if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
                {
                    TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
                }

                GenSAKeyExOpt(levelx);

                //41
                TestReport_HTML_StepDescription("Step: Send valid key", Info);
                SID27_SecurityAccess_SendKey(levelx);
                CheckPosResponse_SID27_SendKey(levelx);

                //42
                TestReport_HTML_StepDescription("Step: Request seed", Info);
                SID27_SecurityAccess_RequestSeed(level);
                CheckPosResponse_SID27_RequestSeed(level);
                if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
                {
                    TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
                }

                GenSAKeyExOpt(level);

                //43
                CheckCurrentSession(0x03);

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
s32 test_case_1022504() //28296->28390_02
{
    try {
        strcpy(TestCaseName, "TC_1022504_Seed and Key Validity Details Test - Physical programmingSession");
        strcpy(TestScopeName, "Security Unlock");
        strcpy(testPurpose_CH, "测试目的：");

        TestPrecondition();
        //1
        SID10_SessionControl(0x02);
        CheckPosResponse_SID10(0x02);
        TestWait(2000);

        CheckCurrentSession(0x02);

        int level = 0x01;

        TestReport_HTML_StepDescription("Step: Request seed", Info);
        SID27_SecurityAccess_RequestSeed(level);
        CheckPosResponse_SID27_RequestSeed(level);
        if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
        {
            TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
        }
        GenSAKeyExOpt(level);
        //4
        TestReport_HTML_StepDescription("Step: Send valid key", Info);
        SID27_SecurityAccess_SendKey(level);
        CheckPosResponse_SID27_SendKey(level);

        //5
        level = 0x03;
        TestReport_HTML_StepDescription("Step: Request seed", Info);
        SID27_SecurityAccess_RequestSeed(level);
        u8 nrc[2] = { 0x7E,0x12 };
        CheckNegResponse(nrc);
        //6
        level = 0x1;
        TestReport_HTML_StepDescription("Step: Request seed", Info);
        SID27_SecurityAccess_RequestSeed(level);
        CheckPosResponse_SID27_RequestSeed(level);
        if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
        {
            TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Pass);
        }
        //7
        SendTesterPresent(TP_OFF);
        TestWait(Diag_UDS_S3Server + ECUResetTime + 500);
        SendTesterPresent(TP_ON);

        //8
        TestWait(2000);
        CheckCurrentSession(0x01);
        SID10_SessionControl(0x82);
        CheckNonResponse();

        //TestReport_HTML_StepDescription("Step: Send valid key", Info);
        //SID27_SecurityAccess_SendKey(level);
        //CheckPosResponse_SID27_SendKey(level);

        //SID10_SessionControl(0x01);
        //CheckPosResponse_SID10(0x01);
        //TestWait(ECUResetTime);

        //SID10_SessionControl(0x02);
        //CheckPosResponse_SID10(0x02);
        //TestWait(ECUResetTime);

        //TestReport_HTML_StepDescription("Step: Request seed", Info);
        //SID27_SecurityAccess_RequestSeed(level);
        //CheckPosResponse_SID27_RequestSeed(level);
        //if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
        //{
        //    TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
        //}

        //GenSAKeyExOpt(level);

        //TestReport_HTML_StepDescription("Step: Send valid key", Info);
        //SID27_SecurityAccess_SendKey(level);
        //CheckPosResponse_SID27_SendKey(level);

        //u8 nrc[2] = { 0x7e,0x12 };
        //SID27_SecurityAccess_RequestSeed(0x03);
        //CheckNegResponse(nrc);

        //SID27_SecurityAccess_RequestSeed(level);
        //CheckPosResponse_SID27_RequestSeed(level);
        //if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
        //{
        //    TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Pass);
        //}

        //SendTesterPresent(TP_OFF);
        //TestWait(Diag_UDS_S3Server + ECUResetTime + 500);

        //CheckCurrentSession(0x01);

        //SendTesterPresent(TP_ON);

        //SID10_SessionControl(0x82);
        //CheckNonResponse();
        //TestWait(ECUResetTime);

        //10
        TestWait(2000);
        CheckCurrentSession(0x02);
        //11
        level = 1;
        SID27_SecurityAccess_RequestSeed(level);
        CheckPosResponse_SID27_RequestSeed(level);
        if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
        {
            TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
        }
        GenSAKeyExOpt(level);

        //12
        SendTesterPresent(TP_OFF);
        TestWait(Diag_UDS_S3Server + ECUResetTime + 500);
        SendTesterPresent(TP_ON);
        //13

        CheckCurrentSession(0x01);
        SID10_SessionControl(0x82);
        CheckNonResponse();
        TestWait(2000);
        CheckCurrentSession(0x02);
        //16
        SID27_SecurityAccess_SendKey(level);
        CheckNegResponse(0x24);

        SecurityUnlock(level);

        SID10_SessionControl(0x82);
        CheckNonResponse();

        TestReport_HTML_StepDescription("Step: Request seed", Info);
        SID27_SecurityAccess_RequestSeed(level);
        CheckPosResponse_SID27_RequestSeed(level);
        if (SeedArray[0] == 0x00 && SeedArray[1] == 0x00 && SeedArray[2] == 0x00)
        {
            TestReport_HTML_StepDescription("Seed received equals to 0x000000!", Fail);
        }

        GenSAKeyExOpt(level);
        KeyArray[15]++;

        TestReport_HTML_StepDescription("Step: Send incorrect key", Info);
        SID27_SecurityAccess_SendKey(level);
        CheckNegResponse(0x35);

        SecurityUnlock(level);
        TestWait(2000);
        CheckCurrentSession(0x02);

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

#endif // !TestCase_CAN_SecurityUnlock