//最后修改者：WYB
//最后修改时间：2025-02-20
#ifndef TestCase_CAN_DTC
#define TestCase_CAN_DTC

#include "test_case_diag_api.h"

////////////////////////////////////////////////////////////////////////////////////////CAN_DTC/////////////////////////////////////////////////////////////////////////////////

s32 TestCase_1315750_HighSupplyVoltageDTCTest()
{
    strcpy(TestCaseName, "TestCase_1315750_HighSupplyVoltageDTCTest");
    strcpy(TestScopeName, "Supply Voltage DTC Test");
    strcpy(testPurpose_CH, "测试目的：");

    int index, occ2;
    float volt;
    double time;
    char StepDespriction[256];
    u8 fdtc[3] = { 0 };
    fdtc[0] = (highVoltDTC.dtc & 0xff0000) >> 16;
    fdtc[1] = (highVoltDTC.dtc & 0xff00) >> 8;
    fdtc[2] = (highVoltDTC.dtc & 0xff);

    try
    {
        stepnumber = 0;
        //Step1
        if (0 != TestPrecondition())
            return -1;
        //TestStep("上电并唤醒被测件");
        //if (0 != CheckCommunication(5000)) {
        //    TestPostCondition();
        //    return -1;
        //}
        printf("Test Start TestFlg is %d", highVoltDTC.tstRlst);
        FulfilTRC();
        TRCSwitch(0x0, 0xD, 0x0);

        //step2
        TestStep("等待7s");
        AppWait(7000, "wait for 7s");

        //step3
        time = ceil(127 / highVoltDTC.SUValue) * highVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 1.2 + 5s");
        TestReport_HTML_StepDescription(StepDespriction);
        snprintf(StepDespriction, sizeof(StepDespriction), "清除DTC,等待%fms", time);
        TestStep(StepDespriction);
        SID14_ClearAllDTC();
        CheckPosResponse_SID14();
        AppWait(time, "");

        //Step4
        TestStep("使用掩码0x09读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x09);
        RX_CheckResponse_DTC_SID19_02(0x09, highVoltDTC.dtc, 0);

        //Step5
        volt = highVoltDTC.volt - 0.35;
        snprintf(StepDespriction, sizeof(StepDespriction), "设置电压为 %5.2fV", volt);
        TestStep(StepDespriction);
        SetVoltageAndRead(volt, 0);

        //Step6
        time = ceil(127 / highVoltDTC.SUValue) * highVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 1.2 + 5s");
        TestReport_HTML_StepDescription(StepDespriction);
        snprintf(StepDespriction, sizeof(StepDespriction), "等待 %fms", time);
        TestStep(StepDespriction);
        AppWait(time, "");

        //Step7
        TestStep("设置电压为12V，等待5s后通过19 02 03读DTC");
        SetVoltageAndRead(12, 0);
        AppWait(5000, "");
        RX_SID19_ReportDTCByStatusMask(0x03);
        RX_CheckResponse_DTC_SID19_02(0x03, highVoltDTC.dtc, 0);

        //Step8
        volt = highVoltDTC.volt + 0.35;
        snprintf(StepDespriction, sizeof(StepDespriction), "设置电压为 %5.2fV", volt);
        TestStep(StepDespriction);
        SetVoltageAndRead(volt, 0);

        //Step9
        time = ceil(127 / highVoltDTC.SUValue) * highVoltDTC.testProd * 0.8;
        snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 0.8");
        TestReport_HTML_StepDescription(StepDespriction);
        snprintf(StepDespriction, sizeof(StepDespriction), "等待 %fms", time);
        TestStep(StepDespriction);
        AppWait(time, "");

        //Step10
        TestStep("设置电压为12V，等待5s后通过19 02 03读DTC");
        SetVoltageAndRead(12, 0);
        AppWait(5000, "");
        RX_SID19_ReportDTCByStatusMask(0x03);
        RX_CheckResponse_DTC_SID19_02(0x03, highVoltDTC.dtc, 0);

        for (int index = 1; index < highVoltDTC.CofrmLmt + 1; index++)
        {
            snprintf(StepDespriction, sizeof(StepDespriction), "---------------------%dth test----------------------", index);
            TestReport_HTML_StepDescription(StepDespriction);
            printf("TestFlg is %d", highVoltDTC.tstRlst);
            stepnumber = 10;

            //Step11
            volt = highVoltDTC.volt + 0.35;
            snprintf(StepDespriction, sizeof(StepDespriction), "设置电压为 %5.2fV", volt);
            TestStep(StepDespriction);
            SetVoltageAndRead(volt, 0);

            //Step12
            time = ceil(127 / highVoltDTC.SUValue) * highVoltDTC.testProd * 1.2;
            snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 1.2");
            TestReport_HTML_StepDescription(StepDespriction);
            snprintf(StepDespriction, sizeof(StepDespriction), "等待 %fms", time);
            TestStep(StepDespriction);
            AppWait(time, "");

            //Step13
            TestStep("设置电压为12V，等待5s后通过19 02 03读DTC");
            SetVoltageAndRead(12, 0);
            AppWait(5000, "");
            RX_SID19_ReportDTCByStatusMask(0x03);
            //RX_CheckResponse_DTC_SID19_02(0x03, highVoltDTC.dtc, 1);
            if (0 != RX_CheckResponse_DTC_SID19_02(0x03, highVoltDTC.dtc, 1)) {
                highVoltDTC.tstRlst = 0;
            }
            else {
                highVoltDTC.tstRlst = 1;
            }

            //Step14
            time = ceil(128 / highVoltDTC.SDValue) * highVoltDTC.testProd * 1.2;
            snprintf(StepDespriction, sizeof(StepDespriction), "等待(128 / StepDownValue) * TestPeriod * 1.2");
            TestReport_HTML_StepDescription(StepDespriction);
            snprintf(StepDespriction, sizeof(StepDespriction), "等待 %fms", time);
            TestStep(StepDespriction);
            AppWait(time, "");

            //Step15
            TestStep("使用掩码0x01读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x01);
            RX_CheckResponse_DTC_SID19_02(0x01, highVoltDTC.dtc, 0);

            //Step16
            TestStep("使用掩码0x08读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x08);

            if (index < (highVoltDTC.CofrmLmt))
            {
                RX_CheckResponse_DTC_SID19_02(0x08, highVoltDTC.dtc, 0);
            }
            else
            {
                //RX_CheckResponse_DTC_SID19_02(0x08, highVoltDTC.dtc, 1);
                if (0 != RX_CheckResponse_DTC_SID19_02(0x08, highVoltDTC.dtc, 1)) {
                    //highVoltDTC.tstRlst = 0;
                }
            }

            //Step17
            TestStep("让被测件进入下一个CofrmLmt操作周期");
            NM_NextOperation();
            AppWait(10000, "");
        }

        //Step19
        TestStep("等待10s,使用掩码0x08读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x08);
        RX_CheckResponse_DTC_SID19_02(0x08, highVoltDTC.dtc, 1);
        printf("Confrm TestFlg is %d", highVoltDTC.tstRlst);
        for (index = 0; index < highVoltDTC.agedLmt; index++)
        {
            if (highVoltDTC.tstRlst != 1) {
                TestReport_HTML_StepDescription("No DTC,Break Test!", Warning);
                break;
            }
            snprintf(StepDespriction, sizeof(StepDespriction), "---------------------%dth Aged test----------------------", index + 1);
            TestReport_HTML_StepDescription(StepDespriction);
            stepnumber = 20;
            //Step20
            time = ceil(128 / highVoltDTC.SDValue) * highVoltDTC.testProd * 1.2 + 5000;
            snprintf(StepDespriction, sizeof(StepDespriction), "等待(128 / StepDownValue) * TestPeriod * 1.2 + 5s");
            TestReport_HTML_StepDescription(StepDespriction);
            snprintf(StepDespriction, sizeof(StepDespriction), "等待 %fms", time);
            TestStep(StepDespriction);
            AppWait(time, "");
            occ2 = ReadAgingCounter(fdtc, 0x02);

            if (occ2 == index)
            {
                snprintf(StepDespriction, sizeof(StepDespriction), "OCC2 is %d", occ2);
                TestReport_HTML_StepDescription(StepDespriction, Pass);
            }
            else
            {
                if (index > highVoltDTC.agedLmt - 1)
                {
                    snprintf(StepDespriction, sizeof(StepDespriction), "OCC2 is %d, expected is %d or 0", occ2, highVoltDTC.agedLmt);
                    TestReport_HTML_StepDescription(StepDespriction, Info);
                }
                else
                {
                    snprintf(StepDespriction, sizeof(StepDespriction), "OCC2 is %d, expected is %d", occ2, index);
                    TestReport_HTML_StepDescription(StepDespriction, Fail);
                }
            }

            //Step21
            TestStep("等待10s,使用掩码0x08读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x08);

            if (index < (highVoltDTC.agedLmt - 1))
            {
                RX_CheckResponse_DTC_SID19_02(0x08, highVoltDTC.dtc, 1);
            }
            else
            {
                RX_CheckResponse_DTC_SID19_02(0x08, highVoltDTC.dtc, 0);
            }

            //Step22
            TestStep("让被测件进入下一个agedLmt操作周期");
            NM_NextOperation();
            AppWait(10000, "");
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

s32 TestCase_1315751_ExtendDataOfHighSupplyVoltageDTCTest()
{
    strcpy(TestCaseName, "TestCase_1315751_ExtendDataOfHighSupplyVoltageDTCTest");
    strcpy(TestScopeName, "Supply Voltage DTC Test");
    strcpy(testPurpose_CH, "测试目的：");

    float volt;
    double time;
    char StepDespriction[256];
    int index, fdc1, fdc2, snapshot, stepflag;
    u8 fdtc[3];
    fdtc[0] = (highVoltDTC.dtc & 0xff0000) >> 16;
    fdtc[1] = (highVoltDTC.dtc & 0xff00) >> 8;
    fdtc[2] = (highVoltDTC.dtc & 0xff);

    stepnumber = 0;

    if (highVoltDTC.dtc == 0)
    {
        strcpy(StepDespriction, "No high voltage DTC, this case is not applicable.");
        TestReport_HTML_StepDescription(StepDespriction, Info);
        return 0;
    }

    try
    {
        //Step1
        if (0 != TestPrecondition())
            return -1;
        //TestStep("上电并唤醒被测件");
        //if (0 != CheckCommunication(5000)) {
        //    TestPostCondition();
        //    return -1;
        //}

        FulfilTRC();
        TRCSwitch(0x0, 0xD, 0x0);

        //step2
        TestStep("等待7s");
        AppWait(7000, "wait for 7s");

        //step3
        time = ceil(127 / highVoltDTC.SUValue) * highVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 1.2 + 5s");
        TestReport_HTML_StepDescription(StepDespriction);
        snprintf(StepDespriction, sizeof(StepDespriction), "清除DTC,等待%fms", time);
        TestStep(StepDespriction);
        SID14_ClearAllDTC();
        CheckPosResponse_SID14();
        AppWait(time, "");

        //Step4
        TestStep("使用掩码0x09读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x09);
        RX_CheckResponse_DTC_SID19_02(0x09, highVoltDTC.dtc, 0);

        //Step5
        volt = highVoltDTC.volt + 0.35;
        snprintf(StepDespriction, sizeof(StepDespriction), "设置电压为 %5.2fV", volt);
        TestStep(StepDespriction);
        SetVoltageAndRead(volt, 0);

        //Step6
        snprintf(StepDespriction, sizeof(StepDespriction), "Send diagnostic request with period %dms to read FDC10 and snapshot", highVoltDTC.testProd);
        TestStep(StepDespriction);
        gUnCofrmLmt = highVoltDTC.UnCofrmLmt;
        CheckStepUpValue(fdtc, highVoltDTC.testProd, highVoltDTC.SUValue);

        //Step8
        AppWait(10000, "");
        TestStep("使用掩码0x01读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x01);
        RX_CheckResponse_DTC_SID19_02(0x01, highVoltDTC.dtc, 1);

        //Step9
        TestStep("设置电压为12V");
        SetVoltageAndRead(12, 0);

        TestWait(20);

        //Step10
        snprintf(StepDespriction, sizeof(StepDespriction), "Send diagnostic request with period %dms to read FDC10", highVoltDTC.testProd);
        TestStep(StepDespriction);
        CheckStepDownValue(fdtc, highVoltDTC.testProd, highVoltDTC.SDValue);

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

s32 TestCase_1315752_HighSupplyVoltageDTCTestWhenEIPowerLevelIs1()
{
    strcpy(TestCaseName, "TestCase_1315752_HighSupplyVoltageDTCTestWhenEIPowerLevelIs1");
    strcpy(TestScopeName, "Supply Voltage DTC Test");
    strcpy(testPurpose_CH, "测试目的：");

    int index;
    float volt;
    double time;
    char StepDespriction[256];
    stepnumber = 0;
    u8 fdtc[3];
    fdtc[0] = (highVoltDTC.dtc & 0xff0000) >> 16;
    fdtc[1] = (highVoltDTC.dtc & 0xff00) >> 8;
    fdtc[2] = (highVoltDTC.dtc & 0xff);

    if (highVoltDTC.dtc == 0)
    {
        PrintCaseNameAndPurpose();
        TestReport_TableInit();
        strcpy(StepDespriction, "No high voltage DTC, this case is not applicable.");
        TestReport_HTML_StepDescription(StepDespriction, Info);
        return 0;
    }
    if (highVoltDTC.tstRlst == 0)
    {
        PrintCaseNameAndPurpose();
        TestReport_TableInit();
        strcpy(StepDespriction, "DUT can not record high voltage DTC, break this case.");
        TestReport_HTML_StepDescription(StepDespriction, Fail);
        return 0;
    }

    try
    {
        //Step1
        if (0 != TestPrecondition())
            return -1;
        //TestStep("上电并唤醒被测件");
        //if (0 != CheckCommunication(5000)) {
        //    TestPostCondition();
        //    return -1;
        //}

        FulfilTRC();
        TRCSwitch(0x0, 0xD, 0x0);

        //step2
        TestStep("等待7s");
        AppWait(7000, "wait for 7s");

        //step3
        time = ceil(127 / highVoltDTC.SUValue) * highVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 1.2 + 5s");
        TestReport_HTML_StepDescription(StepDespriction);
        snprintf(StepDespriction, sizeof(StepDespriction), "清除DTC,等待%fms", time);
        TestStep(StepDespriction);
        SID14_ClearAllDTC();
        CheckPosResponse_SID14();
        AppWait(time, "");

        //Step4
        TestStep("使用掩码0x09读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x09);
        RX_CheckResponse_DTC_SID19_02(0x09, highVoltDTC.dtc, 0);

        //Step5
        TestStep("设置 ElPowerLevel 为 1");
        UsgModSwtich(0xD);
        EPLvlSwitch(0x1);

        //Step6
        AppWait(7000, "wait for 7s");
        TestStep("等待7s");
        //Step7
        volt = highVoltDTC.volt + 0.36;
        snprintf(StepDespriction, sizeof(StepDespriction), "设置电压为 %5.2fV", volt);
        TestStep(StepDespriction);
        SetVoltageAndRead(volt, 0);

        time = ceil(127 / highVoltDTC.SUValue) * highVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 1.2 + 5s");
        TestReport_HTML_StepDescription(StepDespriction);
        snprintf(StepDespriction, sizeof(StepDespriction), "等待 %fms", time);
        TestReport_HTML_StepDescription(StepDespriction);
        AppWait(time, "");

        TestReport_HTML_StepDescription("使用掩码0x09读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x09);
        RX_CheckResponse_DTC_SID19_02(0x09, highVoltDTC.dtc, 0);

        //Step8
        TestStep("设置电压为12V");
        SetVoltageAndRead(12, 0);

        //Step9
        time = ceil(127 / highVoltDTC.SUValue) * highVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 1.2 + 5s");
        TestReport_HTML_StepDescription(StepDespriction);
        snprintf(StepDespriction, sizeof(StepDespriction), "清除DTC,等待%fms", time);
        TestStep(StepDespriction);
        SID14_ClearAllDTC();
        CheckPosResponse_SID14();
        AppWait(time, "");

        //Step10
        TestStep("使用掩码0x09读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x09);
        RX_CheckResponse_DTC_SID19_02(0x09, highVoltDTC.dtc, 0);

        //Step11
        TestStep("设置 ElPowerLevel 为 0");
        UsgModSwtich(0xD);
        EPLvlSwitch(0x0);

        //Step12
        AppWait(7000, "wait for 7s");

        //Step13
        volt = highVoltDTC.volt + 0.35;
        snprintf(StepDespriction, sizeof(StepDespriction), "设置电压为 %5.2fV", volt);
        TestStep(StepDespriction);
        SetVoltageAndRead(volt, 0);

        time = ceil(127 / highVoltDTC.SUValue) * highVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 1.2 + 5s");
        TestReport_HTML_StepDescription(StepDespriction);
        snprintf(StepDespriction, sizeof(StepDespriction), "等待 %fms", time);
        TestReport_HTML_StepDescription(StepDespriction);
        AppWait(time, "");

        TestReport_HTML_StepDescription("使用掩码0x09读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x09);
        RX_CheckResponse_DTC_SID19_02(0x09, highVoltDTC.dtc, 1);

        TestWait(time);

        TestReport_HTML_StepDescription("使用掩码0x09读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x09);
        RX_CheckResponse_DTC_SID19_02(0x09, highVoltDTC.dtc, 1);

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

s32 TestCase_1315753_HighSupplyVoltageDTCTestWhenCarModeIsNotNormal()
{
    strcpy(TestCaseName, "TestCase_1315753_High Supply Voltage DTC Test When CarMode Is Not Normal");
    strcpy(TestScopeName, "Supply Voltage DTC Test");
    strcpy(testPurpose_CH, "测试目的：");

    float volt;
    double time;
    int index, subindex;
    char StepDespriction[256];
    u8 carmode[3];
    stepnumber = 0;

    carmode[0] = 0x2; //Factory
    carmode[1] = 0x1; //Transport
    carmode[2] = 0x3; //Carsh

    if (highVoltDTC.dtc == 0)
    {
        PrintCaseNameAndPurpose();
        TestReport_TableInit();
        strcpy(StepDespriction, "No high voltage DTC, this case is not applicable.");
        TestReport_HTML_StepDescription(StepDespriction, Info);
        return 0;
    }
    if (highVoltDTC.tstRlst == 0)
    {
        PrintCaseNameAndPurpose();
        TestReport_TableInit();
        strcpy(StepDespriction, "DUT can not record high voltage DTC, break this case.");
        TestReport_HTML_StepDescription(StepDespriction, Fail);
        return 0;
    }

    try
    {
        //Step1
        if (0 != TestPrecondition())
            return -1;
        //TestStep("上电并唤醒被测件");
        //if (0 != CheckCommunication(5000)) {
        //    TestPostCondition();
        //    return -1;
        //}

        FulfilTRC();
        TRCSwitch(0x0, 0xB, 0x0);

        //step2
        TestStep("等待7s");
        AppWait(7000, "wait for 7s");

        for (index = 0; index < 3; index++)
        {
            stepnumber = 2;
            //step3
            time = ceil(127 / highVoltDTC.SUValue) * highVoltDTC.testProd * 1.2 + 5000;
            snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 1.2 + 5s");
            TestReport_HTML_StepDescription(StepDespriction);
            snprintf(StepDespriction, sizeof(StepDespriction), "清除DTC,等待%fms", time);
            TestStep(StepDespriction);
            SID14_ClearAllDTC();
            CheckPosResponse_SID14();
            AppWait(time, "");

            //Step4
            TestStep("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, highVoltDTC.dtc, 0);

            //Step5
            snprintf(StepDespriction, sizeof(StepDespriction), "设置 car mode 为 0x%x.(Transport:0x1;Factory:0x2;Crash:0x3)", carmode[index]);
            TestStep(StepDespriction);
            CarModSwtich(carmode[index]);

            //Step6
            TestStep("等待7s");
            AppWait(7000, "wait for 7s");

            //Step7
            volt = highVoltDTC.volt + 0.35;
            snprintf(StepDespriction, sizeof(StepDespriction), "设置电压为 %5.2fV", volt);
            TestStep(StepDespriction);
            SetVoltageAndRead(volt, 0);

            time = ceil(127 / highVoltDTC.SUValue) * highVoltDTC.testProd * 1.2 + 5000;
            snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 1.2 + 5s");
            TestReport_HTML_StepDescription(StepDespriction);
            snprintf(StepDespriction, sizeof(StepDespriction), "等待 %fms", time);
            TestReport_HTML_StepDescription(StepDespriction);
            AppWait(time, "");

            TestReport_HTML_StepDescription("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, highVoltDTC.dtc, 0);

            //Step8
            TestStep("设置电压为12V");
            SetVoltageAndRead(volt, 0);

            //Step9
            if (index != 2)
            {
                TestStep("重复步骤3-8，切换Car mode 为Transport和Carsh");
            }
        }

        //Step10
        TestStep("设置Car mode 为 normal");
        CarModSwtich(0x0);

        //Step11
        TestStep("等待7s");
        AppWait(7000, "wait for 7s");

        //Step12
        volt = highVoltDTC.volt + 0.35;
        snprintf(StepDespriction, sizeof(StepDespriction), "设置电压为 %5.2fV", volt);
        TestStep(StepDespriction);
        SetVoltageAndRead(volt, 0);

        time = ceil(127 / highVoltDTC.SUValue) * highVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 1.2 + 5s");
        TestReport_HTML_StepDescription(StepDespriction);
        snprintf(StepDespriction, sizeof(StepDespriction), "等待 %fms", time);
        TestReport_HTML_StepDescription(StepDespriction);
        AppWait(time, "");

        TestReport_HTML_StepDescription("使用掩码0x09读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x09);
        RX_CheckResponse_DTC_SID19_02(0x09, highVoltDTC.dtc, 1);

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

s32 TestCase_1315754_HighSupplyVoltageDTCTestWhenCarModeTransmission()
{
    strcpy(TestCaseName, "TestCase_1315754_HighSupplyVoltageDTCTestWhenCarModeTransmission");
    strcpy(TestScopeName, "Supply Voltage DTC Test");
    strcpy(testPurpose_CH, "测试目的：");

    float volt;
    double time;
    int index, subindex;
    char StepDespriction[256];
    stepnumber = 0;

    if (highVoltDTC.dtc == 0)
    {
        PrintCaseNameAndPurpose();
        TestReport_TableInit();
        strcpy(StepDespriction, "No high voltage DTC, this case is not applicable.");
        TestReport_HTML_StepDescription(StepDespriction, Info);
        return 0;
    }
    if (highVoltDTC.tstRlst == 0)
    {
        PrintCaseNameAndPurpose();
        TestReport_TableInit();
        strcpy(StepDespriction, "DUT can not record high voltage DTC, break this case.");
        TestReport_HTML_StepDescription(StepDespriction, Fail);
        return 0;
    }

    try
    {
        //Step1
        if (0 != TestPrecondition())
            return -1;
        //TestStep("上电并唤醒被测件");
        //if (0 != CheckCommunication(5000)) {
        //    TestPostCondition();
        //    return -1;
        //}

        FulfilTRC();
        TRCSwitch(0x0, 0xB, 0x0);

        //Step2
        TestStep("等待7s");
        AppWait(7000, "wait for 7s");

        //Step3
        time = ceil(127 / highVoltDTC.SUValue) * highVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 1.2 + 5s");
        TestReport_HTML_StepDescription(StepDespriction);
        snprintf(StepDespriction, sizeof(StepDespriction), "清除DTC,等待%fms", time);
        TestStep(StepDespriction);
        SID14_ClearAllDTC();
        CheckPosResponse_SID14();
        AppWait(time, "");

        //Step4
        TestStep("使用掩码0x09读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x09);
        RX_CheckResponse_DTC_SID19_02(0x09, highVoltDTC.dtc, 0);

        //Step5
        TestStep("设置Car mode 为 Factory");
        CarModSwtich(0x2);

        //Step6
        TestStep("等待7s");
        AppWait(7000, "wait for 7s");

        //Step7
        TestStep("设置Car mode 为 Normal");
        CarModSwtich(0x0);

        //Step8
        TestStep("等待2s");
        AppWait(2000, "wait for 2s");

        //Step9
        volt = highVoltDTC.volt + 0.35;
        snprintf(StepDespriction, sizeof(StepDespriction), "设置电压为 %5.2fV", volt);
        TestStep(StepDespriction);
        SetVoltageAndRead(volt, 0);

        time = ceil(127 / highVoltDTC.SUValue) * highVoltDTC.testProd * 1.2 + 1000;
        snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 1.2 + 1s");
        TestReport_HTML_StepDescription(StepDespriction);
        snprintf(StepDespriction, sizeof(StepDespriction), "等待 %fms", time);
        TestReport_HTML_StepDescription(StepDespriction);
        AppWait(time, "");

        TestReport_HTML_StepDescription("使用掩码0x09读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x09);
        RX_CheckResponse_DTC_SID19_02(0x09, highVoltDTC.dtc, 0);

        //Step10
        time = ceil(127 / highVoltDTC.SUValue) * highVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 1.2 + 5s");
        TestReport_HTML_StepDescription(StepDespriction);
        snprintf(StepDespriction, sizeof(StepDespriction), "等待 %fms", time);
        TestStep(StepDespriction);
        AppWait(time, "");

        TestReport_HTML_StepDescription("使用掩码0x09读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x09);
        RX_CheckResponse_DTC_SID19_02(0x09, highVoltDTC.dtc, 1);

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

s32 TestCase_1315755_HighSupplyVoltageDTCTestWhenUsageModeTransmission()
{
    strcpy(TestCaseName, "TestCase_1315755_HighSupplyVoltageDTCTestWhenUsageModeTransmission");
    strcpy(TestScopeName, "Supply Voltage DTC Test");
    strcpy(testPurpose_CH, "测试目的：");

    float volt;
    double time;
    int index, subindex;
    char StepDespriction[256];
    stepnumber = 0;

    if (highVoltDTC.dtc == 0)
    {
        PrintCaseNameAndPurpose();
        TestReport_TableInit();
        strcpy(StepDespriction, "No high voltage DTC, this case is not applicable.");
        TestReport_HTML_StepDescription(StepDespriction, Info);
        return 0;
    }
    if (highVoltDTC.tstRlst == 0)
    {
        PrintCaseNameAndPurpose();
        TestReport_TableInit();
        strcpy(StepDespriction, "DUT can not record high voltage DTC, break this case.");
        TestReport_HTML_StepDescription(StepDespriction, Fail);
        return 0;
    }

    try
    {
        //Step1
        if (0 != TestPrecondition())
            return -1;
        //TestStep("上电并唤醒被测件");
        //if (0 != CheckCommunication(5000)) {
        //    TestPostCondition();
        //    return -1;
        //}

        FulfilTRC();
        TRCSwitch(0x0, 0xD, 0x0);

        //Step2
        TestStep("等待7s");
        AppWait(7000, "wait for 7s");

        //Step3
        time = ceil(127 / highVoltDTC.SUValue) * highVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 1.2 + 5s");
        TestReport_HTML_StepDescription(StepDespriction);
        snprintf(StepDespriction, sizeof(StepDespriction), "清除DTC,等待%fms", time);
        TestStep(StepDespriction);
        SID14_ClearAllDTC();
        CheckPosResponse_SID14();
        AppWait(time, "");

        //Step4
        TestStep("使用掩码0x09读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x09);
        RX_CheckResponse_DTC_SID19_02(0x09, highVoltDTC.dtc, 0);

        //Step5
        TestStep("设置Usg mode 为 Inactive");
        UsgModSwtich(0xB);

        //Step6
        TestStep("等待7s");
        AppWait(7000, "wait for 7s");

        //Step7
        TestStep("设置Usg mode 为 Driving");
        UsgModSwtich(0xD);

        //Step8
        TestStep("等待2s");
        AppWait(2000, "wait for 2s");

        //Step9
        volt = highVoltDTC.volt + 0.35;
        snprintf(StepDespriction, sizeof(StepDespriction), "设置电压为 %5.2fV", volt);
        TestStep(StepDespriction);
        SetVoltageAndRead(volt, 0);

        time = ceil(127 / highVoltDTC.SUValue) * highVoltDTC.testProd * 1.2 + 1000;
        snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 1.2 + 1s");
        TestReport_HTML_StepDescription(StepDespriction);
        snprintf(StepDespriction, sizeof(StepDespriction), "等待 %fms", time);
        TestReport_HTML_StepDescription(StepDespriction);
        AppWait(time, "");

        TestReport_HTML_StepDescription("使用掩码0x09读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x09);
        RX_CheckResponse_DTC_SID19_02(0x09, highVoltDTC.dtc, 0);

        //Step10
        time = ceil(127 / highVoltDTC.SUValue) * highVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 1.2 + 5s");
        TestReport_HTML_StepDescription(StepDespriction);
        snprintf(StepDespriction, sizeof(StepDespriction), "等待 %fms", time);
        TestStep(StepDespriction);
        AppWait(time, "");

        TestReport_HTML_StepDescription("使用掩码0x09读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x09);
        RX_CheckResponse_DTC_SID19_02(0x09, highVoltDTC.dtc, 1);

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

s32 TestCase_1315756_HighSupplyVoltageDTCTestInLongTermMemoryTest()
{
    strcpy(TestCaseName, "TestCase_1315756_HighSupplyVoltageDTCTestInLongTermMemoryTest");
    strcpy(TestScopeName, "Supply Voltage DTC Test");
    strcpy(testPurpose_CH, "测试目的：");

    float volt;
    double time;
    int index, snapshot;
    char StepDespriction[256];
    u8 fdtc[3];
    fdtc[0] = (highVoltDTC.dtc & 0xff0000) >> 16;
    fdtc[1] = (highVoltDTC.dtc & 0xff00) >> 8;
    fdtc[2] = (highVoltDTC.dtc & 0xff);

    stepnumber = 0;

    if (highVoltDTC.dtc == 0)
    {
        PrintCaseNameAndPurpose();
        TestReport_TableInit();
        strcpy(StepDespriction, "No high voltage DTC, this case is not applicable.");
        TestReport_HTML_StepDescription(StepDespriction, Info);
        return 0;
    }
    if (highVoltDTC.tstRlst == 0)
    {
        PrintCaseNameAndPurpose();
        TestReport_TableInit();
        strcpy(StepDespriction, "DUT can not record high voltage DTC, break this case.");
        TestReport_HTML_StepDescription(StepDespriction, Fail);
        return 0;
    }

    try
    {
        //Step1
        if (0 != TestPrecondition())
            return -1;
        //TestStep("上电并唤醒被测件");
        //if (0 != CheckCommunication(5000)) {
        //    TestPostCondition();
        //    return -1;
        //}

        FulfilTRC();
        TRCSwitch(0x0, 0xD, 0x0);

        //Step2
        TestStep("等待7s");
        AppWait(7000, "wait for 7s");

        //Step3
        time = ceil(127 / highVoltDTC.SUValue) * highVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 1.2 + 5s");
        TestReport_HTML_StepDescription(StepDespriction);
        snprintf(StepDespriction, sizeof(StepDespriction), "清除DTC,等待%fms", time);
        TestStep(StepDespriction);
        SID14_ClearAllDTC();
        CheckPosResponse_SID14();
        AppWait(time, "");

        //Step4
        TestStep("使用掩码0x09读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x09);
        RX_CheckResponse_DTC_SID19_02(0x09, highVoltDTC.dtc, 0);

        //Step5
        volt = highVoltDTC.volt + 0.35;
        snprintf(StepDespriction, sizeof(StepDespriction), "设置电压为 %5.2fV", volt);
        TestStep(StepDespriction);
        SetVoltageAndRead(volt, 0);

        time = ceil(127 / highVoltDTC.SUValue) * highVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 1.2 + 5s");
        TestReport_HTML_StepDescription(StepDespriction);
        snprintf(StepDespriction, sizeof(StepDespriction), "等待 %fms", time);
        TestReport_HTML_StepDescription(StepDespriction);
        AppWait(time, "");

        //Step6
        TestStep("使用掩码0x09读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x09);
        RX_CheckResponse_DTC_SID19_02(0x09, highVoltDTC.dtc, 1);

        //Step7
        TestStep("满足被测设备将DTC信息存储到长期存储器中的条件。例如，让被测设备进入深度睡眠状态");
        UsgModSwtich(0x1);
        EPLvlSwitch(0x0);
        AppWait(1000, "");
        SendTesterPresent(TP_OFF);
        NMWakeUp(0);
        com.del_cyclic_messages();
        KL15(off);
        AppWait(2000, "");
        TestWaitForCurInRange(gLowPwrCur, gSleepTime);

        //Step8
        TestStep("关闭电源，然后等待 10 秒钟");
        KL30(off);
        AppWait(10000, "wait for 10s");

        //Step9
        TestStep("设置电压为12V");
        SetVoltageAndRead(12, 0);

        //Step10
        TestStep("上电并唤醒被测件");
        KL30(on);
        KL15(on);
        AppWait(2000, "");
        NMWakeUp(1);
        SendTesterPresent(TP_ON);
        if (0 != CheckCommunication(5000)) {
            TestPostCondition();
            return -1;
        }
        AppWait(2000, "");
        FulfilTRC();
        AppWait(1000, "");
        TRCSwitch(0x0, 0xD, 0x0);

        //Step11
        time = ceil(128 / highVoltDTC.SDValue) * highVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 1.2 + 5s");
        TestReport_HTML_StepDescription(StepDespriction);
        snprintf(StepDespriction, sizeof(StepDespriction), "等待 %fms", time);
        TestReport_HTML_StepDescription(StepDespriction);
        AppWait(time, "");

        //Step12
        TestStep("使用掩码0x2F读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x2F);
        RX_CheckResponse_DTC_SID19_02(0x2F, highVoltDTC.dtc, 1);

        //Step13
        TestStep("读取测试诊断故障码（DTC）的快照记录");
        snapshot = ReadSnapShot(fdtc);
        if (snapshot > 0)
        {
            snprintf(StepDespriction, sizeof(StepDespriction), "Find snapshot of high voltage 0x%X", highVoltDTC.dtc);
            TestReport_HTML_StepDescription(StepDespriction, Pass);
        }
        else
        {
            snprintf(StepDespriction, sizeof(StepDespriction), "No snapshot of high voltage 0x%X", highVoltDTC.dtc);
            TestReport_HTML_StepDescription(StepDespriction, Fail);
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

s32 TestCase_1315757_LowSupplyVoltageDTCTest()
{
    strcpy(TestCaseName, "TestCase_1315757_Low Supply Voltage DTC Test");
    strcpy(TestScopeName, "Supply Voltage DTC Test");
    strcpy(testPurpose_CH, "测试目的：");

    int index, occ2;
    float volt;
    double time;
    char StepDespriction[256];
    u8 fdtc[3] = { 0 };
    fdtc[0] = (lowVoltDTC.dtc & 0xff0000) >> 16;
    fdtc[1] = (lowVoltDTC.dtc & 0xff00) >> 8;
    fdtc[2] = (lowVoltDTC.dtc & 0xff);

    try
    {
        stepnumber = 0;
        //Step1
        if (0 != TestPrecondition())
            return -1;
        //TestStep("上电并唤醒被测件");
        //if (0 != CheckCommunication(5000)) {
        //    TestPostCondition();
        //    return -1;
        //}

        FulfilTRC();
        TRCSwitch(0x0, 0xD, 0x0);

        //step2
        TestStep("等待7s");
        AppWait(7000, "wait for 7s");

        //step3
        time = ceil(127 / lowVoltDTC.SUValue) * lowVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 1.2 + 5s");
        TestReport_HTML_StepDescription(StepDespriction);
        snprintf(StepDespriction, sizeof(StepDespriction), "清除DTC,等待%fms", time);
        TestStep(StepDespriction);
        SID14_ClearAllDTC();
        CheckPosResponse_SID14();
        AppWait(time, "");

        //Step4
        TestStep("使用掩码0x09读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x09);
        RX_CheckResponse_DTC_SID19_02(0x09, lowVoltDTC.dtc, 0);

        //Step5
        volt = lowVoltDTC.volt + 0.35;
        snprintf(StepDespriction, sizeof(StepDespriction), "设置电压为 %5.2fV", volt);
        TestStep(StepDespriction);
        SetVoltageAndRead(volt, 0);

        //Step6
        time = ceil(127 / lowVoltDTC.SUValue) * lowVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 1.2 + 5s");
        TestReport_HTML_StepDescription(StepDespriction);
        snprintf(StepDespriction, sizeof(StepDespriction), "等待 %fms", time);
        TestStep(StepDespriction);
        AppWait(time, "");

        //Step7
        TestStep("设置电压为12V，等待5s后通过19 02 03读DTC");
        SetVoltageAndRead(12, 0);
        AppWait(5000, "");
        RX_SID19_ReportDTCByStatusMask(0x03);
        RX_CheckResponse_DTC_SID19_02(0x03, lowVoltDTC.dtc, 0);

        //Step8
        volt = lowVoltDTC.volt - 0.35;
        snprintf(StepDespriction, sizeof(StepDespriction), "设置电压为 %5.2fV", volt);
        TestStep(StepDespriction);
        SetVoltageAndRead(volt, 0);

        //Step9
        time = ceil(127 / lowVoltDTC.SUValue) * lowVoltDTC.testProd * 0.8;
        snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 0.8");
        TestReport_HTML_StepDescription(StepDespriction);
        snprintf(StepDespriction, sizeof(StepDespriction), "等待 %fms", time);
        TestStep(StepDespriction);
        AppWait(time, "");

        //Step10
        TestStep("设置电压为12V，等待5s后通过19 02 03读DTC");
        SetVoltageAndRead(12, 0);
        AppWait(5000, "");
        RX_SID19_ReportDTCByStatusMask(0x03);
        RX_CheckResponse_DTC_SID19_02(0x03, lowVoltDTC.dtc, 0);

        for (int index = 0; index < lowVoltDTC.CofrmLmt; index++)
        {
            snprintf(StepDespriction, sizeof(StepDespriction), "---------------------%dth test----------------------", index + 1);
            TestReport_HTML_StepDescription(StepDespriction);
            stepnumber = 11;

            //Step11
            volt = lowVoltDTC.volt - 0.35;
            snprintf(StepDespriction, sizeof(StepDespriction), "设置电压为 %5.2fV", volt);
            TestStep(StepDespriction);
            SetVoltageAndRead(volt, 0);

            //Step12
            time = ceil(127 / lowVoltDTC.SUValue) * lowVoltDTC.testProd * 1.2;
            snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 1.2");
            TestReport_HTML_StepDescription(StepDespriction);
            snprintf(StepDespriction, sizeof(StepDespriction), "等待 %fms", time);
            TestStep(StepDespriction);
            AppWait(time, "");

            //Step13
            TestStep("设置电压为12V，等待5s后通过19 02 03读DTC");
            SetVoltageAndRead(12, 0);
            AppWait(5000, "");
            RX_SID19_ReportDTCByStatusMask(0x03);
            //RX_CheckResponse_DTC_SID19_02(0x03, lowVoltDTC.dtc, 1);
            if (0 != RX_CheckResponse_DTC_SID19_02(0x03, lowVoltDTC.dtc, 1)) {
                lowVoltDTC.tstRlst = 0;
            }

            //Step14
            time = ceil(128 / lowVoltDTC.SDValue) * lowVoltDTC.testProd * 1.2;
            snprintf(StepDespriction, sizeof(StepDespriction), "等待(128 / StepDownValue) * TestPeriod * 1.2");
            TestReport_HTML_StepDescription(StepDespriction);
            snprintf(StepDespriction, sizeof(StepDespriction), "等待 %fms", time);
            TestStep(StepDespriction);
            AppWait(time, "");

            //Step15
            TestStep("使用掩码0x01读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x01);
            RX_CheckResponse_DTC_SID19_02(0x01, lowVoltDTC.dtc, 0);

            //Step16
            TestStep("使用掩码0x08读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x08);

            if (index < (lowVoltDTC.CofrmLmt - 1))
            {
                RX_CheckResponse_DTC_SID19_02(0x08, lowVoltDTC.dtc, 0);
            }
            else
            {
                //RX_CheckResponse_DTC_SID19_02(0x08, lowVoltDTC.dtc, 1);
                if (0 != RX_CheckResponse_DTC_SID19_02(0x08, lowVoltDTC.dtc, 1)) {
                    //lowVoltDTC.tstRlst = 0;
                }
            }

            //Step17
            TestStep("让被测件进入下一个CofrmLmt操作周期");
            NM_NextOperation();
            AppWait(10000, "");
        }

        //Step19
        TestStep("等待10s,使用掩码0x08读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x08);
        RX_CheckResponse_DTC_SID19_02(0x08, lowVoltDTC.dtc, 1);

        for (index = 0; index < lowVoltDTC.agedLmt; index++)
        {
            if (lowVoltDTC.tstRlst != 1) {
                TestReport_HTML_StepDescription("No DTC,Break Test!", Warning);
                break;
            }
            snprintf(StepDespriction, sizeof(StepDespriction), "---------------------%dth Aged test----------------------", index + 1);
            TestReport_HTML_StepDescription(StepDespriction);
            stepnumber = 20;
            //Step20
            time = ceil(128 / lowVoltDTC.SDValue) * lowVoltDTC.testProd * 1.2 + 5000;
            snprintf(StepDespriction, sizeof(StepDespriction), "等待(128 / StepDownValue) * TestPeriod * 1.2 + 5s");
            TestReport_HTML_StepDescription(StepDespriction);
            snprintf(StepDespriction, sizeof(StepDespriction), "等待 %fms", time);
            TestStep(StepDespriction);
            AppWait(time, "");
            occ2 = ReadAgingCounter(fdtc, 0x02);

            if (occ2 == index)
            {
                snprintf(StepDespriction, sizeof(StepDespriction), "OCC2 is %d", occ2);
                TestReport_HTML_StepDescription(StepDespriction, Pass);
            }
            else
            {
                if (index > lowVoltDTC.agedLmt - 1)
                {
                    snprintf(StepDespriction, sizeof(StepDespriction), "OCC2 is %d, expected is %d or 0", occ2, lowVoltDTC.agedLmt);
                    TestReport_HTML_StepDescription(StepDespriction, Info);
                }
                else
                {
                    snprintf(StepDespriction, sizeof(StepDespriction), "OCC2 is %d, expected is %d", occ2, index);
                    TestReport_HTML_StepDescription(StepDespriction, Fail);
                }
            }

            //Step21
            TestStep("等待10s,使用掩码0x08读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x08);

            if (index < (lowVoltDTC.agedLmt - 1))
            {
                RX_CheckResponse_DTC_SID19_02(0x08, lowVoltDTC.dtc, 1);
            }
            else
            {
                RX_CheckResponse_DTC_SID19_02(0x08, lowVoltDTC.dtc, 0);
            }

            //Step22
            TestStep("让被测件进入下一个agedLmt操作周期");
            NM_NextOperation();
            AppWait(10000, "");
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

s32 TestCase_1315758_ExtendDataOfLowSupplyVoltageDTCTest()
{
    strcpy(TestCaseName, "TestCase_1315758_ExtendData Of LowSupply Voltage DTC Test");
    strcpy(TestScopeName, "Supply Voltage DTC Test");
    strcpy(testPurpose_CH, "测试目的：");

    float volt;
    double time;
    char StepDespriction[256];
    int index, fdc1, fdc2, snapshot, stepflag;
    u8 fdtc[3];
    fdtc[0] = (lowVoltDTC.dtc & 0xff0000) >> 16;
    fdtc[1] = (lowVoltDTC.dtc & 0xff00) >> 8;
    fdtc[2] = (lowVoltDTC.dtc & 0xff);

    stepnumber = 0;

    if (lowVoltDTC.dtc == 0)
    {
        strcpy(StepDespriction, "No low voltage DTC, this case is not applicable.");
        TestReport_HTML_StepDescription(StepDespriction, Info);
        return 0;
    }

    try
    {
        //Step1
        if (0 != TestPrecondition())
            return -1;
        //TestStep("上电并唤醒被测件");
        //if (0 != CheckCommunication(5000)) {
        //    TestPostCondition();
        //    return -1;
        //}

        FulfilTRC();
        TRCSwitch(0x0, 0xD, 0x0);

        //step2
        TestStep("等待7s");
        AppWait(7000, "wait for 7s");

        //step3
        time = ceil(127 / lowVoltDTC.SUValue) * lowVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 1.2 + 5s");
        TestReport_HTML_StepDescription(StepDespriction);
        snprintf(StepDespriction, sizeof(StepDespriction), "清除DTC,等待%fms", time);
        TestStep(StepDespriction);
        SID14_ClearAllDTC();
        CheckPosResponse_SID14();
        AppWait(time, "");

        //Step4
        TestStep("使用掩码0x09读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x09);
        RX_CheckResponse_DTC_SID19_02(0x09, lowVoltDTC.dtc, 0);

        //Step5
        volt = lowVoltDTC.volt - 0.35;
        snprintf(StepDespriction, sizeof(StepDespriction), "设置电压为 %5.2fV", volt);
        TestStep(StepDespriction);
        SetVoltageAndRead(volt, 0);

        //Step6
        snprintf(StepDespriction, sizeof(StepDespriction), "Send diagnostic request with period %dms to read FDC10 and snapshot", lowVoltDTC.testProd);
        TestStep(StepDespriction);
        gUnCofrmLmt = lowVoltDTC.UnCofrmLmt;
        CheckStepUpValue(fdtc, lowVoltDTC.testProd, lowVoltDTC.SUValue);

        //Step8
        AppWait(10000, "");
        TestStep("使用掩码0x01读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x01);
        RX_CheckResponse_DTC_SID19_02(0x01, lowVoltDTC.dtc, 1);

        //Step9
        TestStep("设置电压为12V");
        SetVoltageAndRead(12, 0);

        AppWait(20, "");

        //Step10
        snprintf(StepDespriction, sizeof(StepDespriction), "Send diagnostic request with period %dms to read FDC10", lowVoltDTC.testProd);
        TestStep(StepDespriction);
        CheckStepDownValue(fdtc, lowVoltDTC.testProd, lowVoltDTC.SDValue);

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

s32 TestCase_1315759_LowSupplyVoltageDTCTestWhenEIPowerLevelIs1()
{
    strcpy(TestCaseName, "TestCase_1315759_Low Supply Voltage DTC Test When EIPowerLevelIs1");
    strcpy(TestScopeName, "Supply Voltage DTC Test");
    strcpy(testPurpose_CH, "测试目的：");

    int index;
    float volt;
    double time;
    char StepDespriction[256];
    stepnumber = 0;
    u8 fdtc[3];
    fdtc[0] = (lowVoltDTC.dtc & 0xff0000) >> 16;
    fdtc[1] = (lowVoltDTC.dtc & 0xff00) >> 8;
    fdtc[2] = (lowVoltDTC.dtc & 0xff);

    if (lowVoltDTC.dtc == 0)
    {
        PrintCaseNameAndPurpose();
        TestReport_TableInit();
        strcpy(StepDespriction, "No low voltage DTC, this case is not applicable.");
        TestReport_HTML_StepDescription(StepDespriction, Info);
        return 0;
    }
    if (lowVoltDTC.tstRlst == 0)
    {
        PrintCaseNameAndPurpose();
        TestReport_TableInit();
        strcpy(StepDespriction, "DUT can not record low voltage DTC, break this case.");
        TestReport_HTML_StepDescription(StepDespriction, Fail);
        return 0;
    }

    try
    {
        //Step1
        if (0 != TestPrecondition())
            return -1;
        //TestStep("上电并唤醒被测件");
        //if (0 != CheckCommunication(5000)) {
        //    TestPostCondition();
        //    return -1;
        //}

        snprintf(StepDespriction, sizeof(StepDespriction), "low voltage DTC 0x%X", lowVoltDTC.dtc);
        TestReport_HTML_StepDescription(StepDespriction);

        FulfilTRC();
        TRCSwitch(0x0, 0xD, 0x0);

        //step2
        TestStep("等待7s");
        AppWait(7000, "wait for 7s");

        //step3
        time = ceil(127 / lowVoltDTC.SUValue) * lowVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 1.2 + 5s");
        TestReport_HTML_StepDescription(StepDespriction);
        snprintf(StepDespriction, sizeof(StepDespriction), "清除DTC,等待%fms", time);
        TestStep(StepDespriction);
        SID14_ClearAllDTC();
        CheckPosResponse_SID14();
        AppWait(time, "");

        //Step4
        TestStep("使用掩码0x09读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x09);
        RX_CheckResponse_DTC_SID19_02(0x09, lowVoltDTC.dtc, 0);

        //Step5
        TestStep("设置 ElPowerLevel 为 1");
        UsgModSwtich(0xD);
        EPLvlSwitch(0x1);

        //Step6
        AppWait(7000, "wait for 7s");

        //Step7
        volt = lowVoltDTC.volt - 0.35;
        snprintf(StepDespriction, sizeof(StepDespriction), "设置电压为 %5.2fV", volt);
        TestStep(StepDespriction);
        SetVoltageAndRead(volt, 0);

        time = ceil(127 / lowVoltDTC.SUValue) * lowVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 1.2 + 5s");
        TestReport_HTML_StepDescription(StepDespriction);
        snprintf(StepDespriction, sizeof(StepDespriction), "等待 %fms", time);
        TestReport_HTML_StepDescription(StepDespriction);
        AppWait(time, "");

        TestReport_HTML_StepDescription("使用掩码0x09读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x09);
        RX_CheckResponse_DTC_SID19_02(0x09, lowVoltDTC.dtc, 0);

        //Step8
        TestStep("设置电压为12V");
        SetVoltageAndRead(12, 0);

        //Step9
        time = ceil(127 / lowVoltDTC.SUValue) * lowVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 1.2 + 5s");
        TestReport_HTML_StepDescription(StepDespriction);
        snprintf(StepDespriction, sizeof(StepDespriction), "清除DTC,等待%fms", time);
        TestStep(StepDespriction);
        SID14_ClearAllDTC();
        CheckPosResponse_SID14();
        AppWait(time, "");

        //Step10
        TestStep("使用掩码0x09读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x09);
        RX_CheckResponse_DTC_SID19_02(0x09, lowVoltDTC.dtc, 0);

        //Step11
        TestStep("设置 ElPowerLevel 为 0");
        UsgModSwtich(0xD);
        EPLvlSwitch(0x0);

        //Step12
        AppWait(7000, "wait for 7s");

        //Step13
        volt = lowVoltDTC.volt - 0.35;
        snprintf(StepDespriction, sizeof(StepDespriction), "设置电压为 %5.2fV", volt);
        TestStep(StepDespriction);
        SetVoltageAndRead(volt, 0);

        time = ceil(127 / lowVoltDTC.SUValue) * lowVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 1.2 + 5s");
        TestReport_HTML_StepDescription(StepDespriction);
        snprintf(StepDespriction, sizeof(StepDespriction), "等待 %fms", time);
        TestReport_HTML_StepDescription(StepDespriction);
        AppWait(time, "");

        TestReport_HTML_StepDescription("使用掩码0x09读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x01);
        RX_CheckResponse_DTC_SID19_02(0x01, lowVoltDTC.dtc, 1);

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

s32 TestCase_1315760_LowSupplyVoltageDTCTestWhenCarModeIsNotNormal()
{
    strcpy(TestCaseName, "TestCase_1315760_Low Supply Voltage DTC Test When CarMode Is Not Normal");
    strcpy(TestScopeName, "Supply Voltage DTC Test");
    strcpy(testPurpose_CH, "测试目的：");

    float volt;
    double time;
    int index, subindex;
    char StepDespriction[256];
    u8 carmode[3];
    stepnumber = 0;

    carmode[0] = 0x2; //Factory
    carmode[1] = 0x1; //Transport
    carmode[2] = 0x3; //Carsh

    if (lowVoltDTC.dtc == 0)
    {
        PrintCaseNameAndPurpose();
        TestReport_TableInit();
        strcpy(StepDespriction, "No low voltage DTC, this case is not applicable.");
        TestReport_HTML_StepDescription(StepDespriction, Info);
        return 0;
    }
    if (lowVoltDTC.tstRlst == 0)
    {
        PrintCaseNameAndPurpose();
        TestReport_TableInit();
        strcpy(StepDespriction, "DUT can not record high voltage DTC, break this case.");
        TestReport_HTML_StepDescription(StepDespriction, Fail);
        return 0;
    }

    try
    {
        //Step1
        if (0 != TestPrecondition())
            return -1;
        //TestStep("上电并唤醒被测件");
        //if (0 != CheckCommunication(5000)) {
        //    TestPostCondition();
        //    return -1;
        //}

        FulfilTRC();
        TRCSwitch(0x0, 0xB, 0x0);

        //step2
        TestStep("等待7s");
        AppWait(7000, "wait for 7s");

        for (index = 0; index < 3; index++)
        {
            stepnumber = 3;
            //step3
            time = ceil(127 / lowVoltDTC.SUValue) * lowVoltDTC.testProd * 1.2 + 5000;
            snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 1.2 + 5s");
            TestReport_HTML_StepDescription(StepDespriction);
            snprintf(StepDespriction, sizeof(StepDespriction), "清除DTC,等待%fms", time);
            TestStep(StepDespriction);
            SID14_ClearAllDTC();
            CheckPosResponse_SID14();
            AppWait(time, "");

            //Step4
            TestStep("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, lowVoltDTC.dtc, 0);

            //Step5
            snprintf(StepDespriction, sizeof(StepDespriction), "设置 car mode 为 0x%x.(Transport:0x1;Factory:0x2;Crash:0x3)", carmode[index]);
            TestStep(StepDespriction);
            CarModSwtich(carmode[index]);

            //Step6
            TestStep("等待7s");
            AppWait(7000, "wait for 7s");

            //Step7
            volt = lowVoltDTC.volt - 0.35;
            snprintf(StepDespriction, sizeof(StepDespriction), "设置电压为 %5.2fV", volt);
            TestStep(StepDespriction);
            SetVoltageAndRead(volt, 0);

            time = ceil(127 / lowVoltDTC.SUValue) * lowVoltDTC.testProd * 1.2 + 5000;
            snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 1.2 + 5s");
            TestReport_HTML_StepDescription(StepDespriction);
            snprintf(StepDespriction, sizeof(StepDespriction), "等待 %fms", time);
            TestReport_HTML_StepDescription(StepDespriction);
            AppWait(time, "");

            TestReport_HTML_StepDescription("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, lowVoltDTC.dtc, 0);

            //Step8
            TestStep("设置电压为12V");
            SetVoltageAndRead(12, 0);

            //Step9
            if (index != 2)
            {
                TestStep("重复步骤3-8，切换Car mode 为Transport和Carsh");
            }
        }

        //Step10
        TestStep("设置Car mode 为 normal");
        CarModSwtich(0x0);

        //Step11
        TestStep("等待7s");
        AppWait(7000, "wait for 7s");

        //Step12
        volt = lowVoltDTC.volt - 0.35;
        snprintf(StepDespriction, sizeof(StepDespriction), "设置电压为 %5.2fV", volt);
        TestStep(StepDespriction);
        SetVoltageAndRead(volt, 0);

        time = ceil(127 / lowVoltDTC.SUValue) * lowVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 1.2 + 5s");
        TestReport_HTML_StepDescription(StepDespriction);
        snprintf(StepDespriction, sizeof(StepDespriction), "等待 %fms", time);
        TestReport_HTML_StepDescription(StepDespriction);
        AppWait(time, "");

        TestReport_HTML_StepDescription("使用掩码0x09读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x09);
        RX_CheckResponse_DTC_SID19_02(0x09, lowVoltDTC.dtc, 1);

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

s32 TestCase_1315761_LowSupplyVoltageDTCTestWhenCarModeTransmission()
{
    strcpy(TestCaseName, "TestCase_1315761_Low Supply Voltage DTC Test When CarMode Transmission");
    strcpy(TestScopeName, "Supply Voltage DTC Test");
    strcpy(testPurpose_CH, "测试目的：");

    float volt;
    double time;
    int index, subindex;
    char StepDespriction[256];
    stepnumber = 0;

    if (lowVoltDTC.dtc == 0)
    {
        PrintCaseNameAndPurpose();
        TestReport_TableInit();
        strcpy(StepDespriction, "No low voltage DTC, this case is not applicable.");
        TestReport_HTML_StepDescription(StepDespriction, Info);
        return 0;
    }
    if (lowVoltDTC.tstRlst == 0)
    {
        PrintCaseNameAndPurpose();
        TestReport_TableInit();
        strcpy(StepDespriction, "DUT can not record high voltage DTC, break this case.");
        TestReport_HTML_StepDescription(StepDespriction, Fail);
        return 0;
    }

    try
    {
        //Step1
        if (0 != TestPrecondition())
            return -1;
        //TestStep("上电并唤醒被测件");
        //if (0 != CheckCommunication(5000)) {
        //    TestPostCondition();
        //    return -1;
        //}

        FulfilTRC();
        TRCSwitch(0x0, 0xB, 0x0);

        //Step2
        TestStep("等待7s");
        AppWait(7000, "wait for 7s");

        //Step3
        time = ceil(127 / lowVoltDTC.SUValue) * lowVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 1.2 + 5s");
        TestReport_HTML_StepDescription(StepDespriction);
        snprintf(StepDespriction, sizeof(StepDespriction), "清除DTC,等待%fms", time);
        TestStep(StepDespriction);
        SID14_ClearAllDTC();
        CheckPosResponse_SID14();
        AppWait(time, "");

        //Step4
        TestStep("使用掩码0x09读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x09);
        RX_CheckResponse_DTC_SID19_02(0x09, lowVoltDTC.dtc, 0);

        //Step5
        TestStep("设置Car mode 为 Factory");
        CarModSwtich(0x2);

        //Step6
        TestStep("等待7s");
        AppWait(7000, "wait for 7s");

        //Step7
        TestStep("设置Car mode 为 Normal");
        CarModSwtich(0x0);

        //Step8
        TestStep("等待2s");
        AppWait(2000, "wait for 2s");

        //Step9
        volt = lowVoltDTC.volt - 0.35;
        snprintf(StepDespriction, sizeof(StepDespriction), "设置电压为 %5.2fV", volt);
        TestStep(StepDespriction);
        SetVoltageAndRead(volt, 0);

        time = ceil(127 / lowVoltDTC.SUValue) * lowVoltDTC.testProd * 1.2 + 1000;
        snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 1.2 + 1s");
        TestReport_HTML_StepDescription(StepDespriction);
        snprintf(StepDespriction, sizeof(StepDespriction), "等待 %fms", time);
        TestReport_HTML_StepDescription(StepDespriction);
        AppWait(time, "");

        TestReport_HTML_StepDescription("使用掩码0x09读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x09);
        RX_CheckResponse_DTC_SID19_02(0x09, lowVoltDTC.dtc, 0);

        //Step10
        time = ceil(127 / lowVoltDTC.SUValue) * lowVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "等待 %fms", time);
        TestStep(StepDespriction);
        AppWait(time, "");

        TestReport_HTML_StepDescription("使用掩码0x09读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x09);
        RX_CheckResponse_DTC_SID19_02(0x09, lowVoltDTC.dtc, 1);

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

s32 TestCase_1315762_LowSupplyVoltageDTCTestWhenUsageModeTransmission()
{
    strcpy(TestCaseName, "TestCase_1315762_LowSupplyVoltageDTCTestWhenUsageModeTransmission");
    strcpy(TestScopeName, "Supply Voltage DTC Test");
    strcpy(testPurpose_CH, "测试目的：");

    float volt;
    double time;
    int index, subindex;
    char StepDespriction[256];
    stepnumber = 0;

    if (lowVoltDTC.dtc == 0)
    {
        PrintCaseNameAndPurpose();
        TestReport_TableInit();
        strcpy(StepDespriction, "No low voltage DTC, this case is not applicable.");
        TestReport_HTML_StepDescription(StepDespriction, Info);
        return 0;
    }
    if (lowVoltDTC.tstRlst == 0)
    {
        PrintCaseNameAndPurpose();
        TestReport_TableInit();
        strcpy(StepDespriction, "DUT can not record high voltage DTC, break this case.");
        TestReport_HTML_StepDescription(StepDespriction, Fail);
        return 0;
    }

    try
    {
        //Step1
        if (0 != TestPrecondition())
            return -1;
        //TestStep("上电并唤醒被测件");
        //if (0 != CheckCommunication(5000)) {
        //    TestPostCondition();
        //    return -1;
        //}

        FulfilTRC();
        TRCSwitch(0x0, 0xD, 0x0);

        //Step2
        TestStep("等待7s");
        AppWait(7000, "wait for 7s");

        //Step3
        time = ceil(127 / lowVoltDTC.SUValue) * lowVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 1.2 + 5s");
        TestReport_HTML_StepDescription(StepDespriction);
        snprintf(StepDespriction, sizeof(StepDespriction), "清除DTC,等待%fms", time);
        TestStep(StepDespriction);
        SID14_ClearAllDTC();
        CheckPosResponse_SID14();
        AppWait(time, "");

        //Step4
        TestStep("使用掩码0x09读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x09);
        RX_CheckResponse_DTC_SID19_02(0x09, lowVoltDTC.dtc, 0);

        //Step5
        TestStep("设置Usg mode 为 Inactive");
        UsgModSwtich(0xB);

        //Step6
        TestStep("等待7s");
        AppWait(7000, "wait for 7s");

        //Step7
        TestStep("设置Usg mode 为 Driving");
        UsgModSwtich(0xD);

        //Step8
        TestStep("等待2s");
        AppWait(2000, "wait for 2s");

        //Step9
        volt = lowVoltDTC.volt - 0.35;
        snprintf(StepDespriction, sizeof(StepDespriction), "设置电压为 %5.2fV", volt);
        TestStep(StepDespriction);
        SetVoltageAndRead(volt, 0);

        time = ceil(127 / lowVoltDTC.SUValue) * lowVoltDTC.testProd * 1.2 + 1000;
        snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 1.2 + 1s");
        TestReport_HTML_StepDescription(StepDespriction);
        snprintf(StepDespriction, sizeof(StepDespriction), "等待 %fms", time);
        TestReport_HTML_StepDescription(StepDespriction);
        AppWait(time, "");

        TestReport_HTML_StepDescription("使用掩码0x09读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x09);
        RX_CheckResponse_DTC_SID19_02(0x09, lowVoltDTC.dtc, 0);

        //Step10
        time = ceil(127 / lowVoltDTC.SUValue) * lowVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 1.2 + 5s");
        TestReport_HTML_StepDescription(StepDespriction);
        snprintf(StepDespriction, sizeof(StepDespriction), "等待 %fms", time);
        TestStep(StepDespriction);
        AppWait(time, "");

        TestReport_HTML_StepDescription("使用掩码0x09读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x09);
        RX_CheckResponse_DTC_SID19_02(0x09, lowVoltDTC.dtc, 1);

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

s32 TestCase_1315763_LowSupplyVoltageDTCTestWhenSupplyVoltageComparedFailureIsDetected()
{
    strcpy(TestCaseName, "TestCase_1315763_LowSupplyVoltageDTCTestWhenSupplyVoltageComparedFailureIsDetected");
    strcpy(TestScopeName, "Supply Voltage DTC Test");
    strcpy(testPurpose_CH, "测试目的：");

    float volt;
    double time;
    int index, subindex;
    char StepDespriction[256];
    stepnumber = 0;

    if (lowVoltDTC.dtc == 0)
    {
        PrintCaseNameAndPurpose();
        TestReport_TableInit();
        strcpy(StepDespriction, "No low voltage DTC, this case is not applicable.");
        TestReport_HTML_StepDescription(StepDespriction, Info);
        return 0;
    }
    if (lowVoltDTC.tstRlst == 0)
    {
        PrintCaseNameAndPurpose();
        TestReport_TableInit();
        strcpy(StepDespriction, "DUT can not record high voltage DTC, break this case.");
        TestReport_HTML_StepDescription(StepDespriction, Fail);
        return 0;
    }

    try
    {
        //Step1
        if (0 != TestPrecondition())
            return -1;
        //TestStep("上电并唤醒被测件");
        //if (0 != CheckCommunication(5000)) {
        //    TestPostCondition();
        //    return -1;
        //}

        FulfilTRC();
        TRCSwitch(0x0, 0xD, 0x0);

        //Step2
        TestStep("等待7s");
        AppWait(7000, "wait for 7s");

        //Step3
        time = ceil(127 / lowVoltDTC.SUValue) * lowVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 1.2 + 5s");
        TestReport_HTML_StepDescription(StepDespriction);
        snprintf(StepDespriction, sizeof(StepDespriction), "清除DTC,等待%fms", time);
        TestStep(StepDespriction);
        SID14_ClearAllDTC();
        CheckPosResponse_SID14();
        AppWait(time, "");

        //Step4
        TestStep("使用掩码0x09读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x09);
        RX_CheckResponse_DTC_SID19_02(0x09, lowVoltDTC.dtc, 0);

        //Step5
        volt = lowVoltDTC.volt - 0.35;
        snprintf(StepDespriction, sizeof(StepDespriction), "设置电压为 %5.2fV  设置Ubat %.2lf", volt, max(lowVoltDTC.volt + 3, 12));
        TestStep(StepDespriction);
        SetVoltageAndRead(volt, 0);
        gVehBatVotg = max(lowVoltDTC.volt + 3, 12) * 10;

        //Step6
        time = ceil(127 / lowVoltDTC.SUValue) * lowVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 1.2 + 5s wait %fms", time);
        TestStep(StepDespriction);
        AppWait(time, "");

        //Step7
        TestStep("使用掩码0x09读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x09);
        RX_CheckResponse_DTC_SID19_02(0x09, lowVoltDTC.dtc, 0);

        //Step8
        snprintf(StepDespriction, sizeof(StepDespriction), "设置电压为12V  设置Ubat 12V");
        TestStep(StepDespriction);
        SetVoltageAndRead(12, 0);
        gVehBatVotg = gUbat12;

        //Step9
        time = ceil(127 / lowVoltDTC.SUValue) * lowVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 1.2 + 5s");
        TestReport_HTML_StepDescription(StepDespriction);
        snprintf(StepDespriction, sizeof(StepDespriction), "清除DTC,等待%fms", time);
        TestStep(StepDespriction);
        SID14_ClearAllDTC();
        CheckPosResponse_SID14();
        AppWait(time, "");

        //Step10
        TestReport_HTML_StepDescription("使用掩码0x09读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x09);
        RX_CheckResponse_DTC_SID19_02(0x09, lowVoltDTC.dtc, 0);

        //Step10
        volt = lowVoltDTC.volt - 0.35;
        TestReport_HTML_StepDescription("设置电压LowVolt-0.35V 设置Ubat 11.9V");
        SetVoltageAndRead(volt, 0);
        gVehBatVotg = 119;
        time = ceil(127 / lowVoltDTC.SUValue) * lowVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 1.2 + 5s");
        TestReport_HTML_StepDescription(StepDespriction);
        snprintf(StepDespriction, sizeof(StepDespriction), "等待 %fms", time);
        TestStep(StepDespriction);
        AppWait(time, "");

        TestReport_HTML_StepDescription("使用掩码0x09读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x09);
        RX_CheckResponse_DTC_SID19_02(0x09, lowVoltDTC.dtc, 1);

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

s32 TestCase_1315764_LowSupplyVoltageDTCTestInLongTermMemoryTest()
{
    strcpy(TestCaseName, "TestCase_1315764_Low Supply Voltage DTC Test In LongTerm Memory Test");
    strcpy(TestScopeName, "Supply Voltage DTC Test");
    strcpy(testPurpose_CH, "测试目的：");

    float volt;
    double time;
    int index, snapshot;
    char StepDespriction[256];
    u8 fdtc[3];
    fdtc[0] = (lowVoltDTC.dtc & 0xff0000) >> 16;
    fdtc[1] = (lowVoltDTC.dtc & 0xff00) >> 8;
    fdtc[2] = (lowVoltDTC.dtc & 0xff);

    stepnumber = 0;

    if (lowVoltDTC.dtc == 0)
    {
        PrintCaseNameAndPurpose();
        TestReport_TableInit();
        strcpy(StepDespriction, "No low voltage DTC, this case is not applicable.");
        TestReport_HTML_StepDescription(StepDespriction, Info);
        return 0;
    }
    if (lowVoltDTC.tstRlst == 0)
    {
        PrintCaseNameAndPurpose();
        TestReport_TableInit();
        strcpy(StepDespriction, "DUT can not record low voltage DTC, break this case.");
        TestReport_HTML_StepDescription(StepDespriction, Fail);
        return 0;
    }

    try
    {
        //Step1
        if (0 != TestPrecondition())
            return -1;
        //TestStep("上电并唤醒被测件");
        //if (0 != CheckCommunication(5000)) {
        //    TestPostCondition();
        //    return -1;
        //}

        FulfilTRC();
        TRCSwitch(0x0, 0xD, 0x0);

        //Step2
        TestStep("等待7s");
        AppWait(7000, "wait for 7s");

        //Step3
        time = ceil(127 / lowVoltDTC.SUValue) * lowVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 1.2 + 5s");
        TestReport_HTML_StepDescription(StepDespriction);
        snprintf(StepDespriction, sizeof(StepDespriction), "清除DTC,等待%fms", time);
        TestStep(StepDespriction);
        SID14_ClearAllDTC();
        CheckPosResponse_SID14();
        AppWait(time, "");

        //Step4
        TestStep("使用掩码0x09读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x09);
        RX_CheckResponse_DTC_SID19_02(0x09, lowVoltDTC.dtc, 0);

        //Step5
        volt = lowVoltDTC.volt - 0.35;
        snprintf(StepDespriction, sizeof(StepDespriction), "设置电压为 %5.2fV", volt);
        TestStep(StepDespriction);
        SetVoltageAndRead(volt, 0);

        time = ceil(127 / lowVoltDTC.SUValue) * lowVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 1.2 + 5s");
        TestReport_HTML_StepDescription(StepDespriction);
        snprintf(StepDespriction, sizeof(StepDespriction), "等待 %fms", time);
        TestReport_HTML_StepDescription(StepDespriction);
        AppWait(time, "");

        //Step6
        TestStep("使用掩码0x09读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x09);
        RX_CheckResponse_DTC_SID19_02(0x09, lowVoltDTC.dtc, 1);

        //Step7
        TestStep("满足被测设备将DTC信息存储到长期存储器中的条件。例如，让被测设备进入深度睡眠状态");
        UsgModSwtich(0x1);
        EPLvlSwitch(0x0);
        AppWait(1000, "");
        SendTesterPresent(TP_OFF);
        NMWakeUp(0);
        com.del_cyclic_messages();
        KL15(off);
        AppWait(2000, "");
        TestWaitForCurInRange(gLowPwrCur, gSleepTime);

        //Step8
        TestStep("关闭电源，然后等待 10 秒钟");
        KL30(off);
        AppWait(10000, "wait for 10s");

        //Step9
        TestStep("设置电压为12V");
        SetVoltageAndRead(12, 0);


        //Step10
        TestStep("上电并唤醒被测件");
        KL30(on);
        KL15(on);
        AppWait(2000, "");

        if (0 != CheckCommunication(5000)) {
            TestPostCondition();
            return -1;
        }

        NMWakeUp(1);
        SendTesterPresent(TP_ON);
        AppWait(2000, "");
        FulfilTRC();
        AppWait(1000, "");
        TRCSwitch(0x0, 0xD, 0x0);

        //Step11
        time = ceil(128 / lowVoltDTC.SDValue) * lowVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "等待(128 / StepDownValue) * TestPeriod * 1.2 + 5s");
        TestReport_HTML_StepDescription(StepDespriction);
        snprintf(StepDespriction, sizeof(StepDespriction), "等待 %fms", time);
        TestReport_HTML_StepDescription(StepDespriction);
        AppWait(time, "");

        //Step12
        TestStep("使用掩码0x2F读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x2F);
        RX_CheckResponse_DTC_SID19_02(0x2F, lowVoltDTC.dtc, 1);

        //Step13
        TestStep("读取测试诊断故障码（DTC）的快照记录");
        snapshot = ReadSnapShot(fdtc);
        if (snapshot > 0)
        {
            snprintf(StepDespriction, sizeof(StepDespriction), "Find snapshot of high voltage 0x%X", lowVoltDTC.dtc);
            TestReport_HTML_StepDescription(StepDespriction, Pass);
        }
        else
        {
            snprintf(StepDespriction, sizeof(StepDespriction), "No snapshot of high voltage 0x%X", lowVoltDTC.dtc);
            TestReport_HTML_StepDescription(StepDespriction, Fail);
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

s32 TestCase_1022899_SupplyVoltageComparedFailureDTCTest()
{
    strcpy(TestCaseName, "TestCase_1022899_Supply Voltage Compared Failure DTC Test");
    strcpy(TestScopeName, "Supply Voltage DTC Test");
    strcpy(testPurpose_CH, "测试目的：");

    int index, occ2;
    float volt;
    double time;
    char StepDespriction[256];
    u8 fdtc[3] = { 0 };
    fdtc[0] = (batVoltDTC.dtc & 0xff0000) >> 16;
    fdtc[1] = (batVoltDTC.dtc & 0xff00) >> 8;
    fdtc[2] = (batVoltDTC.dtc & 0xff);

    try
    {
        stepnumber = 0;
        //if (gCEMFlg == 1) {
        //    PrintCaseNameAndPurpose();
        //    TestReport_TableInit();
        //    strcpy(StepDespriction, "This is VMM, this case is not applicable.");
        //    TestReport_HTML_StepDescription(StepDespriction, Info);
        //    return 0;
        //}

        if (batVoltDTC.dtc == 0)
        {
            PrintCaseNameAndPurpose();
            TestReport_TableInit();
            strcpy(StepDespriction, "No Compared voltage DTC, this case is not applicable.");
            TestReport_HTML_StepDescription(StepDespriction, Info);
            return 0;
        }

        //Step1
        if (0 != TestPrecondition())
            return -1;
        //TestStep("上电并唤醒被测件");
        //if (0 != CheckCommunication(5000)) {
        //    TestPostCondition();
        //    return -1;
        //}

        FulfilTRC();
        TRCSwitch(0x0, 0xD, 0x0);
        if (gCEMFlg != 1) {
            //SimVehBat(gUbat12);
            gVehBatVotg = gUbat12;
        }

        //step2
        TestStep("等待7s");
        AppWait(7000, "wait for 7s");

        //step3
        time = ceil(127 / batVoltDTC.SUValue) * batVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "清除DTC,等待%fms", time);
        TestStep(StepDespriction);
        SID14_ClearAllDTC();
        CheckPosResponse_SID14();
        AppWait(time, "");

        //Step4
        TestStep("使用掩码0x09读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x09);
        RX_CheckResponse_DTC_SID19_02(0x09, batVoltDTC.dtc, 0);

        //Step5
        volt = (12 - batVoltDTC.volt) + 0.35;
        snprintf(StepDespriction, sizeof(StepDespriction), "设置电压为 %5.2fV", volt);
        TestStep(StepDespriction);
        SetVoltageAndRead(volt, 0);

        //Step6
        time = ceil(127 / batVoltDTC.SUValue) * batVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "等待 %fms后通过19 02 01读DTC", time);
        AppWait(time, "");
        RX_SID19_ReportDTCByStatusMask(0x01);
        RX_CheckResponse_DTC_SID19_02(0x01, batVoltDTC.dtc, 0);

        for (int index = 0; index < batVoltDTC.CofrmLmt; index++)
        {
            snprintf(StepDespriction, sizeof(StepDespriction), "---------------------%dth test----------------------", index + 1);
            TestReport_HTML_StepDescription(StepDespriction);
            stepnumber = 7;

            //Step7
            volt = (12 - batVoltDTC.volt) - 0.35;
            snprintf(StepDespriction, sizeof(StepDespriction), "设置电压为 %5.2fV", volt);
            TestStep(StepDespriction);
            SetVoltageAndRead(volt, 0);

            //Step8
            time = ceil(127 / batVoltDTC.SUValue) * batVoltDTC.testProd * 1.2;
            snprintf(StepDespriction, sizeof(StepDespriction), "等待 %fms后通过19 02 01读DTC", time);
            AppWait(time, "");
            RX_SID19_ReportDTCByStatusMask(0x01);
            RX_CheckResponse_DTC_SID19_02(0x01, batVoltDTC.dtc, 1);

            //Step9
            TestStep("设置电压为12V");
            SetVoltageAndRead(12, 0);

            //Step10
            time = ceil(128 / batVoltDTC.SDValue) * batVoltDTC.testProd * 1.2;
            snprintf(StepDespriction, sizeof(StepDespriction), "等待(128 / StepDownValue) * TestPeriod * 1.2");
            TestReport_HTML_StepDescription(StepDespriction);
            snprintf(StepDespriction, sizeof(StepDespriction), "等待 %fms", time);
            TestStep(StepDespriction);
            AppWait(time, "");

            //Step11
            TestStep("使用掩码0x01读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x01);
            RX_CheckResponse_DTC_SID19_02(0x01, batVoltDTC.dtc, 0);

            //Step12
            TestStep("使用掩码0x08读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x08);

            if (index < (batVoltDTC.CofrmLmt - 1))
            {
                RX_CheckResponse_DTC_SID19_02(0x08, batVoltDTC.dtc, 0);
            }
            else
            {
                RX_CheckResponse_DTC_SID19_02(0x08, batVoltDTC.dtc, 1);
            }

            //Step13
            TestStep("让被测件进入下一个CofrmLmt操作周期");
            NM_NextOperation();
            AppWait(10000, "");
        }

        //Step15
        TestStep("等待10s,使用掩码0x08读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x08);
        RX_CheckResponse_DTC_SID19_02(0x08, batVoltDTC.dtc, 1);

        for (index = 0; index < batVoltDTC.agedLmt; index++)
        {
            snprintf(StepDespriction, sizeof(StepDespriction), "---------------------%dth Aged test----------------------", index + 1);
            TestReport_HTML_StepDescription(StepDespriction);
            stepnumber = 16;
            //Step16
            time = ceil(128 / batVoltDTC.SDValue) * batVoltDTC.testProd * 1.2 + 5000;
            snprintf(StepDespriction, sizeof(StepDespriction), "等待(128 / StepDownValue) * TestPeriod * 1.2 + 5s");
            TestReport_HTML_StepDescription(StepDespriction);
            snprintf(StepDespriction, sizeof(StepDespriction), "等待 %fms", time);
            TestStep(StepDespriction);
            AppWait(time, "");
            occ2 = ReadAgingCounter(fdtc, 0x02);

            if (occ2 == index)
            {
                snprintf(StepDespriction, sizeof(StepDespriction), "OCC2 is %d", occ2);
                TestReport_HTML_StepDescription(StepDespriction, Pass);
            }
            else
            {
                if (index > batVoltDTC.agedLmt - 1)
                {
                    snprintf(StepDespriction, sizeof(StepDespriction), "OCC2 is %d, expected is %d or 0", occ2, batVoltDTC.agedLmt);
                    TestReport_HTML_StepDescription(StepDespriction, Info);
                }
                else
                {
                    snprintf(StepDespriction, sizeof(StepDespriction), "OCC2 is %d, expected is %d", occ2, index);
                    TestReport_HTML_StepDescription(StepDespriction, Fail);
                }
            }

            //Step17
            TestStep("使用掩码0x08读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x08);

            if (index < (batVoltDTC.agedLmt - 1))
            {
                RX_CheckResponse_DTC_SID19_02(0x08, batVoltDTC.dtc, 1);
            }
            else
            {
                RX_CheckResponse_DTC_SID19_02(0x08, batVoltDTC.dtc, 0);
            }

            //Step28
            TestStep("让被测件进入下一个agedLmt操作周期");
            NM_NextOperation();
            AppWait(10000, "");
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

s32 TestCase_1022900_SupplyVoltageComparedFailureDTCTestWhenCarModeTransmissionl()
{
    strcpy(TestCaseName, "TestCase_1022900_Supply Voltage Compared Failure DTC Test When CarMode Transmissionl");
    strcpy(TestScopeName, "Supply Voltage DTC Test");
    strcpy(testPurpose_CH, "测试目的：");

    float volt;
    double time;
    int index, subindex;
    char StepDespriction[256];
    stepnumber = 0;
    //if (gCEMFlg == 1) {
    //    PrintCaseNameAndPurpose();
    //    TestReport_TableInit();
    //    strcpy(StepDespriction, "This is VMM, this case is not applicable.");
    //    TestReport_HTML_StepDescription(StepDespriction, Info);
    //    return 0;
    //}
    if (batVoltDTC.dtc == 0)
    {
        PrintCaseNameAndPurpose();
        TestReport_TableInit();
        strcpy(StepDespriction, "No Compared voltage DTC, this case is not applicable.");
        TestReport_HTML_StepDescription(StepDespriction, Info);
        return 0;
    }
    if (batVoltDTC.tstRlst == 0)
    {
        PrintCaseNameAndPurpose();
        TestReport_TableInit();
        strcpy(StepDespriction, "DUT can not record Compared voltage DTC, break this case.");
        TestReport_HTML_StepDescription(StepDespriction, Fail);
        return 0;
    }

    try
    {
        //Step1
        if (0 != TestPrecondition())
            return -1;
        //TestStep("上电并唤醒被测件");
        //if (0 != CheckCommunication(5000)) {
        //    TestPostCondition();
        //    return -1;
        //}

        FulfilTRC();
        TRCSwitch(0x0, 0xB, 0x0);

        //Step2
        TestStep("等待7s");
        AppWait(7000, "wait for 7s");

        //Step3
        time = ceil(127 / batVoltDTC.SUValue) * batVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "清除DTC,等待%fms", time);
        TestStep(StepDespriction);
        SID14_ClearAllDTC();
        CheckPosResponse_SID14();
        AppWait(time, "");

        //Step4
        TestStep("使用掩码0x09读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x09);
        RX_CheckResponse_DTC_SID19_02(0x09, batVoltDTC.dtc, 0);

        //Step5
        TestStep("设置Car mode 为 Crash");
        CarModSwtich(0x3);

        //Step6
        TestStep("等待7s");
        AppWait(7000, "wait for 7s");

        //Step7
        TestStep("设置Car mode 为 Normal");
        CarModSwtich(0x0);
        UsgModSwtich(0xD);

        //Step8
        TestStep("等待2s");
        AppWait(2000, "wait for 2s");

        //Step9
        volt = (12 - batVoltDTC.volt) - 0.35;
        snprintf(StepDespriction, sizeof(StepDespriction), "设置电压为 %5.2fV", volt);
        TestStep(StepDespriction);
        SetVoltageAndRead(volt, 0);

        time = ceil(127 / batVoltDTC.SUValue) * batVoltDTC.testProd * 1.2 + 1000;
        snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 1.2 + 1s");
        TestReport_HTML_StepDescription(StepDespriction);
        snprintf(StepDespriction, sizeof(StepDespriction), "等待 %fms", time);
        TestReport_HTML_StepDescription(StepDespriction);
        AppWait(time, "");

        TestReport_HTML_StepDescription("使用掩码0x09读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x09);
        RX_CheckResponse_DTC_SID19_02(0x09, batVoltDTC.dtc, 0);

        //Step10
        time = ceil(127 / batVoltDTC.SUValue) * batVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 1.2 + 5s");
        TestReport_HTML_StepDescription(StepDespriction);
        snprintf(StepDespriction, sizeof(StepDespriction), "等待 %fms", time);
        TestStep(StepDespriction);
        AppWait(time, "");

        TestReport_HTML_StepDescription("使用掩码0x09读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x09);
        RX_CheckResponse_DTC_SID19_02(0x09, batVoltDTC.dtc, 1);

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

s32 TestCase_1022901_SupplyVoltageComparedFailureDTCTestInLongTermMemoryTest()
{
    strcpy(TestCaseName, "TestCase_1022901_Supply Voltage Compared Failure DTC Test In Long Term Memory Test");
    strcpy(TestScopeName, "Supply Voltage DTC Test");
    strcpy(testPurpose_CH, "测试目的：");

    float volt;
    double time;
    int index, snapshot;
    char StepDespriction[256];
    u8 fdtc[3];
    fdtc[0] = (batVoltDTC.dtc & 0xff0000) >> 16;
    fdtc[1] = (batVoltDTC.dtc & 0xff00) >> 8;
    fdtc[2] = (batVoltDTC.dtc & 0xff);

    stepnumber = 0;
    //if (gCEMFlg == 1) {
    //    PrintCaseNameAndPurpose();
    //    TestReport_TableInit();
    //    strcpy(StepDespriction, "This is VMM, this case is not applicable.");
    //    TestReport_HTML_StepDescription(StepDespriction, Info);
    //    return 0;
    //}
    if (batVoltDTC.dtc == 0)
    {
        PrintCaseNameAndPurpose();
        TestReport_TableInit();
        strcpy(StepDespriction, "No Compared voltage DTC, this case is not applicable.");
        TestReport_HTML_StepDescription(StepDespriction, Info);
        return 0;
    }
    if (batVoltDTC.tstRlst == 0)
    {
        PrintCaseNameAndPurpose();
        TestReport_TableInit();
        strcpy(StepDespriction, "DUT can not record Compared voltage DTC, break this case.");
        TestReport_HTML_StepDescription(StepDespriction, Fail);
        return 0;
    }

    try
    {
        //Step1
        if (0 != TestPrecondition())
            return -1;
        //TestStep("上电并唤醒被测件");
        //if (0 != CheckCommunication(5000)) {
        //    TestPostCondition();
        //    return -1;
        //}

        FulfilTRC();
        TRCSwitch(0x0, 0xD, 0x0);

        //Step2
        TestStep("等待7s");
        AppWait(7000, "wait for 7s");

        //Step3
        time = ceil(127 / batVoltDTC.SUValue) * batVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "清除DTC,等待%fms", time);
        TestStep(StepDespriction);
        SID14_ClearAllDTC();
        CheckPosResponse_SID14();
        AppWait(time, "");

        //Step4
        TestStep("使用掩码0x09读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x09);
        RX_CheckResponse_DTC_SID19_02(0x09, batVoltDTC.dtc, 0);

        //Step5
        volt = (12 - batVoltDTC.volt) - 0.35;
        snprintf(StepDespriction, sizeof(StepDespriction), "设置电压为 %5.2fV", volt);
        TestStep(StepDespriction);
        SetVoltageAndRead(volt, 0);

        time = ceil(127 / batVoltDTC.SUValue) * batVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 1.2 + 5s");
        TestReport_HTML_StepDescription(StepDespriction);
        snprintf(StepDespriction, sizeof(StepDespriction), "等待 %fms", time);
        TestReport_HTML_StepDescription(StepDespriction);
        AppWait(time, "");

        //Step6
        TestStep("使用掩码0x09读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x09);
        RX_CheckResponse_DTC_SID19_02(0x09, batVoltDTC.dtc, 1);

        //Step7
        TestStep("满足被测设备将DTC信息存储到长期存储器中的条件。例如，让被测设备进入深度睡眠状态");
        UsgModSwtich(0x1);
        EPLvlSwitch(0x0);
        AppWait(1000, "");
        SendTesterPresent(TP_OFF);
        NMWakeUp(0);
        com.del_cyclic_messages();
        KL15(off);
        AppWait(2000, "");
        TestWaitForCurInRange(gLowPwrCur, gSleepTime);

        //Step8
        TestStep("关闭电源，然后等待 10 秒钟");
        KL30(off);
        AppWait(10000, "wait for 10s");

        //Step9
        TestStep("设置电压为12V");
        SetVoltageAndRead(12, 0);

        //Step10
        TestStep("上电并唤醒被测件");
        KL30(on);
        KL15(on);
        NMWakeUp(1);
        if (0 != CheckCommunication(5000)) {
            TestPostCondition();
            return -1;
        }
        SendTesterPresent(TP_ON);
        FulfilTRC();
        AppWait(1000, "");
        TRCSwitch(0x0, 0xD, 0x0);

        //Step11
        time = ceil(128 / batVoltDTC.SDValue) * batVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "等待(128 / StepDownValue) * TestPeriod * 1.2 + 5s");
        TestReport_HTML_StepDescription(StepDespriction);
        snprintf(StepDespriction, sizeof(StepDespriction), "等待 %fms", time);
        TestReport_HTML_StepDescription(StepDespriction);
        AppWait(time, "");

        //Step12
        TestStep("使用掩码0x2F读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x2F);
        RX_CheckResponse_DTC_SID19_02(0x2F, batVoltDTC.dtc, 1);

        //Step13
        TestStep("读取测试诊断故障码（DTC）的快照记录");
        snapshot = ReadSnapShot(fdtc);
        if (snapshot > 0)
        {
            snprintf(StepDespriction, sizeof(StepDespriction), "Find snapshot of Compared voltage 0x%X", batVoltDTC.dtc);
            TestReport_HTML_StepDescription(StepDespriction, Pass);
        }
        else
        {
            snprintf(StepDespriction, sizeof(StepDespriction), "No snapshot of Compared voltage 0x%X", batVoltDTC.dtc);
            TestReport_HTML_StepDescription(StepDespriction, Fail);
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

s32 TestCase_1022902_SupplyVoltageComparedFailureDTCTestWhenUsageModeTransmission()
{
    strcpy(TestCaseName, "TestCase_1022902_Supply Voltage Compared Failure DTC Test When UsageMode Transmission");
    strcpy(TestScopeName, "Supply Voltage DTC Test");
    strcpy(testPurpose_CH, "测试目的：");

    float volt;
    double time;
    int index, subindex;
    char StepDespriction[256];
    stepnumber = 0;
    //if (gCEMFlg == 1) {
    //    PrintCaseNameAndPurpose();
    //    TestReport_TableInit();
    //    strcpy(StepDespriction, "This is VMM, this case is not applicable.");
    //    TestReport_HTML_StepDescription(StepDespriction, Info);
    //    return 0;
    //}
    if (batVoltDTC.dtc == 0)
    {
        PrintCaseNameAndPurpose();
        TestReport_TableInit();
        strcpy(StepDespriction, "No Compared voltage DTC, this case is not applicable.");
        TestReport_HTML_StepDescription(StepDespriction, Info);
        return 0;
    }
    if (batVoltDTC.tstRlst == 0)
    {
        PrintCaseNameAndPurpose();
        TestReport_TableInit();
        strcpy(StepDespriction, "DUT can not record high voltage DTC, break this case.");
        TestReport_HTML_StepDescription(StepDespriction, Fail);
        return 0;
    }

    try
    {
        //Step1
        if (0 != TestPrecondition())
            return -1;
        //TestStep("上电并唤醒被测件");
        //if (0 != CheckCommunication(5000)) {
        //    TestPostCondition();
        //    return -1;
        //}

        FulfilTRC();
        TRCSwitch(0x0, 0xD, 0x0);

        //Step2
        TestStep("等待7s");
        AppWait(7000, "wait for 7s");

        //Step3
        time = ceil(127 / batVoltDTC.SUValue) * batVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "清除DTC,等待%fms", time);
        TestStep(StepDespriction);
        SID14_ClearAllDTC();
        CheckPosResponse_SID14();
        AppWait(time, "");

        //Step4
        TestStep("使用掩码0x09读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x09);
        RX_CheckResponse_DTC_SID19_02(0x09, batVoltDTC.dtc, 0);

        //Step5
        TestStep("设置Usg mode 为 Inactive");
        UsgModSwtich(0xB);

        //Step6
        TestStep("等待7s");
        AppWait(7000, "wait for 7s");

        //Step7
        TestStep("设置Usg mode 为 Driving");
        UsgModSwtich(0xD);

        //Step8
        TestStep("等待2s");
        AppWait(2000, "wait for 2s");

        //Step9
        volt = (12 - batVoltDTC.volt) - 0.35;
        snprintf(StepDespriction, sizeof(StepDespriction), "设置电压为 %5.2fV", volt);
        TestStep(StepDespriction);
        SetVoltageAndRead(volt, 0);

        time = ceil(127 / batVoltDTC.SUValue) * batVoltDTC.testProd * 1.2 + 1000;
        snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 1.2 + 1s");
        TestReport_HTML_StepDescription(StepDespriction);
        snprintf(StepDespriction, sizeof(StepDespriction), "等待 %fms", time);
        TestReport_HTML_StepDescription(StepDespriction);
        AppWait(time, "");

        TestReport_HTML_StepDescription("使用掩码0x09读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x09);
        RX_CheckResponse_DTC_SID19_02(0x09, batVoltDTC.dtc, 0);

        //Step10
        time = ceil(127 / batVoltDTC.SUValue) * batVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 1.2 + 5s");
        TestReport_HTML_StepDescription(StepDespriction);
        snprintf(StepDespriction, sizeof(StepDespriction), "等待 %fms", time);
        TestStep(StepDespriction);
        AppWait(time, "");

        TestReport_HTML_StepDescription("使用掩码0x09读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x09);
        RX_CheckResponse_DTC_SID19_02(0x09, batVoltDTC.dtc, 1);

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

s32 TestCase_1022903_SupplyVoltageComparedFailureDTCTestWhenUsageModeIsNotDriving()
{
    strcpy(TestCaseName, "TestCase_1022903_Supply Voltage Compared Failure DTC Test When UsageMode Is Not Driving");
    strcpy(TestScopeName, "Supply Voltage DTC Test");
    strcpy(testPurpose_CH, "测试目的：");

    float volt;
    double time;
    int index, subindex;
    char StepDespriction[256];
    u8 usgmode[4];
    stepnumber = 0;

    usgmode[0] = 0x0; //Abandoned
    usgmode[1] = 0x1; //Inactive
    usgmode[2] = 0x2; //Convenience
    usgmode[3] = 0xB; //Active
    //if (gCEMFlg == 1) {
    //    PrintCaseNameAndPurpose();
    //    TestReport_TableInit();
    //    strcpy(StepDespriction, "This is VMM, this case is not applicable.");
    //    TestReport_HTML_StepDescription(StepDespriction, Info);
    //    return 0;
    //}
    if (batVoltDTC.dtc == 0)
    {
        PrintCaseNameAndPurpose();
        TestReport_TableInit();
        strcpy(StepDespriction, "No Compared voltage DTC, this case is not applicable.");
        TestReport_HTML_StepDescription(StepDespriction, Info);
        return 0;
    }
    if (batVoltDTC.tstRlst == 0)
    {
        PrintCaseNameAndPurpose();
        TestReport_TableInit();
        strcpy(StepDespriction, "DUT can not record high voltage DTC, break this case.");
        TestReport_HTML_StepDescription(StepDespriction, Fail);
        return 0;
    }

    try
    {
        //Step1
        if (0 != TestPrecondition())
            return -1;
        //TestStep("上电并唤醒被测件");
        //if (0 != CheckCommunication(5000)) {
        //    TestPostCondition();
        //    return -1;
        //}

        FulfilTRC();
        TRCSwitch(0x0, 0xB, 0x0);

        //step2
        TestStep("等待7s");
        AppWait(7000, "wait for 7s");

        for (index = 0; index < 4; index++)
        {
            stepnumber = 3;
            //step3
            time = ceil(127 / batVoltDTC.SUValue) * batVoltDTC.testProd * 1.2 + 5000;
            snprintf(StepDespriction, sizeof(StepDespriction), "清除DTC,等待%fms", time);
            TestStep(StepDespriction);
            SID14_ClearAllDTC();
            CheckPosResponse_SID14();
            AppWait(time, "");

            //Step4
            TestStep("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, batVoltDTC.dtc, 0);

            //Step5
            snprintf(StepDespriction, sizeof(StepDespriction), "设置 usg mode 为 0x%x.(Abandoned:0x0;Inactive:0x1;Convenience:0x2,Active:0xB)", usgmode[index]);
            TestStep(StepDespriction);
            UsgModSwtich(usgmode[index]);

            //Step6
            TestStep("等待7s");
            AppWait(7000, "wait for 7s");

            //Step7
            volt = (12 - batVoltDTC.volt) - 0.35;
            snprintf(StepDespriction, sizeof(StepDespriction), "设置电压为 %5.2fV", volt);
            TestStep(StepDespriction);
            SetVoltageAndRead(volt, 0);

            time = ceil(127 / batVoltDTC.SUValue) * batVoltDTC.testProd * 1.2 + 5000;
            snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 1.2 + 5s");
            TestReport_HTML_StepDescription(StepDespriction);
            snprintf(StepDespriction, sizeof(StepDespriction), "等待 %fms", time);
            TestReport_HTML_StepDescription(StepDespriction);
            AppWait(time, "");

            TestReport_HTML_StepDescription("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, batVoltDTC.dtc, 0);

            //Step8
            TestStep("设置电压为12V");
            SetVoltageAndRead(12, 0);

            //Step9
            if (index != 3)
            {
                TestStep("重复步骤3-8，切换Usage mode 为Abandoned:0x0;Inactive:0x1;Convenience:0x2,Active:0xB");
            }
        }

        //Step10
        TestStep("设置Usage mode 为 Driving");
        UsgModSwtich(0xD);

        //Step11
        TestStep("等待7s");
        AppWait(7000, "wait for 7s");

        //Step12
        time = ceil(127 / batVoltDTC.SUValue) * batVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "等待 %fms,使用掩码0x09读取DTC", time);
        TestStep(StepDespriction);
        AppWait(time, "");

        TestReport_HTML_StepDescription("使用掩码0x09读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x09);
        RX_CheckResponse_DTC_SID19_02(0x09, batVoltDTC.dtc, 1);

        //Step13
        TestStep("设置Usage mode 为 Convenience");
        UsgModSwtich(0x2);
        AppWait(7000, "wait for 7s");

        //Step14
        volt = (12 - batVoltDTC.volt) - 0.35;
        snprintf(StepDespriction, sizeof(StepDespriction), "设置电压为 %5.2fV", volt);
        TestStep(StepDespriction);
        SetVoltageAndRead(volt, 0);

        time = ceil(127 / batVoltDTC.SUValue) * batVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 1.2 + 5s");
        TestReport_HTML_StepDescription(StepDespriction);
        snprintf(StepDespriction, sizeof(StepDespriction), "等待 %fms", time);
        TestReport_HTML_StepDescription(StepDespriction);
        AppWait(time, "");

        TestReport_HTML_StepDescription("使用掩码0x09读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x09);
        RX_CheckResponse_DTC_SID19_02(0x09, batVoltDTC.dtc, 1);


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

s32 TestCase_1022904_SupplyVoltageComparedFailureDTCTestWhenCarModeIsNotNormal()
{
    strcpy(TestCaseName, "TestCase_1022904_Supply Voltage Compared Failure DTC Test When CarMode Is Not Normal");
    strcpy(TestScopeName, "Supply Voltage DTC Test");
    strcpy(testPurpose_CH, "测试目的：");

    float volt;
    double time;
    int index, subindex;
    char StepDespriction[256];
    u8 carmode[3];
    stepnumber = 0;

    carmode[0] = 0x2; //Factory
    carmode[1] = 0x1; //Transport
    carmode[2] = 0x3; //Carsh
    //if (gCEMFlg == 1) {
    //    PrintCaseNameAndPurpose();
    //    TestReport_TableInit();
    //    strcpy(StepDespriction, "This is VMM, this case is not applicable.");
    //    TestReport_HTML_StepDescription(StepDespriction, Info);
    //    return 0;
    //}
    if (batVoltDTC.dtc == 0)
    {
        PrintCaseNameAndPurpose();
        TestReport_TableInit();
        strcpy(StepDespriction, "No Compared voltage DTC, this case is not applicable.");
        TestReport_HTML_StepDescription(StepDespriction, Info);
        return 0;
    }
    if (batVoltDTC.tstRlst == 0)
    {
        PrintCaseNameAndPurpose();
        TestReport_TableInit();
        strcpy(StepDespriction, "DUT can not record high voltage DTC, break this case.");
        TestReport_HTML_StepDescription(StepDespriction, Fail);
        return 0;
    }

    try
    {
        //Step1
        if (0 != TestPrecondition())
            return -1;
        //TestStep("上电并唤醒被测件");
        //if (0 != CheckCommunication(5000)) {
        //    TestPostCondition();
        //    return -1;
        //}

        FulfilTRC();
        TRCSwitch(0x0, 0xB, 0x0);

        //step2
        TestStep("等待7s");
        AppWait(7000, "wait for 7s");

        for (index = 0; index < 3; index++)
        {
            stepnumber = 3;
            //step3
            time = ceil(127 / batVoltDTC.SUValue) * batVoltDTC.testProd * 1.2 + 5000;
            snprintf(StepDespriction, sizeof(StepDespriction), "清除DTC,等待%fms", time);
            TestStep(StepDespriction);
            SID14_ClearAllDTC();
            CheckPosResponse_SID14();
            AppWait(time, "");

            //Step4
            TestStep("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, batVoltDTC.dtc, 0);

            //Step5
            snprintf(StepDespriction, sizeof(StepDespriction), "设置 car mode 为 0x%x.(Transport:0x1;Factory:0x2;Crash:0x3)", carmode[index]);
            TestStep(StepDespriction);
            CarModSwtich(carmode[index]);
            UsgModSwtich(0xB);
            AppWait(100, "");
            CarModSwtich(carmode[index]);
            AppWait(100, "");
            UsgModSwtich(0xD);

            //Step6
            TestStep("等待7s");
            AppWait(7000, "wait for 7s");

            //Step7
            volt = (12 - batVoltDTC.volt) - 0.35;
            snprintf(StepDespriction, sizeof(StepDespriction), "设置电压为 %5.2fV", volt);
            TestStep(StepDespriction);
            SetVoltageAndRead(volt, 0);

            time = ceil(127 / batVoltDTC.SUValue) * batVoltDTC.testProd * 1.2 + 5000;
            snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 1.2 + 5s");
            TestReport_HTML_StepDescription(StepDespriction);
            snprintf(StepDespriction, sizeof(StepDespriction), "等待 %fms", time);
            TestReport_HTML_StepDescription(StepDespriction);
            AppWait(time, "");

            TestReport_HTML_StepDescription("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, batVoltDTC.dtc, 0);

            //Step8
            TestStep("设置电压为12V");
            SetVoltageAndRead(12, 0);

            //Step9
            if (index != 2)
            {
                TestStep("重复步骤3-8，切换Car mode 为Transport和Carsh");
            }
        }

        //Step10
        TestStep("设置Car mode 为 normal");
        UsgModSwtich(0xB);
        CarModSwtich(0x0);

        //Step11
        TestStep("等待7s");
        AppWait(7000, "wait for 7s");

        //Step12
        volt = (12 - batVoltDTC.volt) - 0.35;
        snprintf(StepDespriction, sizeof(StepDespriction), "设置电压为 %5.2fV", volt);
        TestStep(StepDespriction);
        SetVoltageAndRead(volt, 0);

        time = ceil(127 / batVoltDTC.SUValue) * batVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 1.2 + 5s");
        TestReport_HTML_StepDescription(StepDespriction);
        snprintf(StepDespriction, sizeof(StepDespriction), "等待 %fms", time);
        TestReport_HTML_StepDescription(StepDespriction);
        AppWait(time, "");

        TestReport_HTML_StepDescription("使用掩码0x09读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x09);
        RX_CheckResponse_DTC_SID19_02(0x09, batVoltDTC.dtc, 1);

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

s32 TestCase_1022906_SupplyVoltageComparedFailureDTCTestWhenUbatIsLowerThan12V()
{
    strcpy(TestCaseName, "TestCase_1022906_Supply Voltage Compared Failure DTC Test When Ubat Is Lower Than 12V");
    strcpy(TestScopeName, "Supply Voltage DTC Test");
    strcpy(testPurpose_CH, "测试目的：");

    float volt;
    u8 fdtc[3];
    int index;
    double time;
    char StepDespriction[256];
    //if (gCEMFlg == 1) {
    //    PrintCaseNameAndPurpose();
    //    TestReport_TableInit();
    //    strcpy(StepDespriction, "This is VMM, this case is not applicable.");
    //    TestReport_HTML_StepDescription(StepDespriction, Info);
    //    return 0;
    //}
    if (batVoltDTC.dtc == 0)
    {
        PrintCaseNameAndPurpose();
        TestReport_TableInit();
        strcpy(StepDespriction, "No high voltage DTC, this case is not applicable.");
        TestReport_HTML_StepDescription(StepDespriction, Info);
        return 0;
    }
    if (batVoltDTC.tstRlst == 0)
    {
        PrintCaseNameAndPurpose();
        TestReport_TableInit();
        strcpy(StepDespriction, "DUT can not record high voltage DTC, break this case.");
        TestReport_HTML_StepDescription(StepDespriction, Fail);
        return 0;
    }

    try
    {
        //Step1
        if (0 != TestPrecondition())
            return -1;
        //TestStep("上电并唤醒被测件");
        //if (0 != CheckCommunication(5000)) {
        //    TestPostCondition();
        //    return -1;
        //}

        snprintf(StepDespriction, sizeof(StepDespriction), "high voltage DTC 0x%X", batVoltDTC.dtc);
        TestReport_HTML_StepDescription(StepDespriction);

        FulfilTRC();
        TRCSwitch(0x0, 0xD, 0x0);

        //step2
        TestStep("等待7s");
        AppWait(7000, "wait for 7s");

        //step3
        time = ceil(127 / batVoltDTC.SUValue) * batVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "清除DTC,等待%fms", time);
        TestStep(StepDespriction);
        SID14_ClearAllDTC();
        CheckPosResponse_SID14();
        AppWait(time, "");

        //Step4
        TestStep("使用掩码0x09读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x09);
        RX_CheckResponse_DTC_SID19_02(0x09, batVoltDTC.dtc, 0);

        //Step5
        SimVehBat(11.9);
        volt = 8.9 - 0.35;
        snprintf(StepDespriction, sizeof(StepDespriction), "设置Ubat为11.9V,设置电压为 %5.2fV", volt);
        TestStep(StepDespriction);
        SetVoltageAndRead(volt, 0);

        //Step6
        time = ceil(127 / batVoltDTC.SUValue) * batVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 1.2 + 5s");
        TestReport_HTML_StepDescription(StepDespriction);
        snprintf(StepDespriction, sizeof(StepDespriction), "等待 %fms", time);
        TestStep(StepDespriction);
        AppWait(time, "");

        TestReport_HTML_StepDescription("使用掩码0x01读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x01);
        RX_CheckResponse_DTC_SID19_02(0x01, batVoltDTC.dtc, 0);

        //Step7
        SimVehBat(12);
        volt = 12;
        snprintf(StepDespriction, sizeof(StepDespriction), "设置Ubat为12V,设置电压为 %5.2fV", volt);
        TestStep(StepDespriction);
        SetVoltageAndRead(volt, 0);

        //Step8
        time = ceil(127 / batVoltDTC.SUValue) * batVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "清除DTC,等待%fms", time);
        TestStep(StepDespriction);
        SID14_ClearAllDTC();
        CheckPosResponse_SID14();
        AppWait(time, "");

        //Step9
        TestStep("使用掩码0x09读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x09);
        RX_CheckResponse_DTC_SID19_02(0x09, batVoltDTC.dtc, 0);

        //Stap10
        SimVehBat(12);
        volt = 9 - 0.35;
        snprintf(StepDespriction, sizeof(StepDespriction), "设置Ubat为12V,设置电压为 %5.2fV,等7s", volt);
        TestStep(StepDespriction);
        SetVoltageAndRead(volt, 0);
        AppWait(7000, "wait for 7s");

        //Step11
        time = ceil(127 / batVoltDTC.SUValue) * batVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 1.2 + 5s");
        TestReport_HTML_StepDescription(StepDespriction);
        snprintf(StepDespriction, sizeof(StepDespriction), "等待 %fms", time);
        TestStep(StepDespriction);
        AppWait(time, "");

        TestReport_HTML_StepDescription("使用掩码0x01读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x01);
        RX_CheckResponse_DTC_SID19_02(0x01, batVoltDTC.dtc, 1);

        //Step12
        SimVehBat(12);
        volt = 12;
        snprintf(StepDespriction, sizeof(StepDespriction), "设置Ubat为12V,设置电压为 %5.2fV", volt);
        TestStep(StepDespriction);
        SetVoltageAndRead(volt, 0);

        //Step13
        time = ceil(127 / batVoltDTC.SUValue) * batVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "清除DTC,等待%fms", time);
        TestStep(StepDespriction);
        SID14_ClearAllDTC();
        CheckPosResponse_SID14();
        AppWait(time, "");

        //Step14
        TestStep("使用掩码0x09读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x09);
        RX_CheckResponse_DTC_SID19_02(0x09, batVoltDTC.dtc, 0);

        //Step15
        SimVehBat(12.1);
        volt = 9.1 - 0.35;
        snprintf(StepDespriction, sizeof(StepDespriction), "设置Ubat为11.9V,设置电压为 %5.2fV,等7s", volt);
        TestStep(StepDespriction);
        SetVoltageAndRead(volt, 0);
        AppWait(7000, "wait for 7s");

        //Step16
        time = ceil(127 / batVoltDTC.SUValue) * batVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 1.2 + 5s");
        TestReport_HTML_StepDescription(StepDespriction);
        snprintf(StepDespriction, sizeof(StepDespriction), "等待 %fms", time);
        TestStep(StepDespriction);
        AppWait(time, "");

        TestReport_HTML_StepDescription("使用掩码0x01读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x01);
        RX_CheckResponse_DTC_SID19_02(0x01, batVoltDTC.dtc, 1);

        //Step17
        SimVehBat(12);
        volt = 12;
        snprintf(StepDespriction, sizeof(StepDespriction), "设置Ubat为12V,设置电压为 %5.2fV", volt);
        TestStep(StepDespriction);
        SetVoltageAndRead(volt, 0);

        //Step18
        time = ceil(127 / batVoltDTC.SUValue) * batVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "清除DTC,等待%fms", time);
        TestStep(StepDespriction);
        SID14_ClearAllDTC();
        CheckPosResponse_SID14();
        AppWait(time, "");

        //Step19
        TestStep("使用掩码0x09读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x09);
        RX_CheckResponse_DTC_SID19_02(0x09, batVoltDTC.dtc, 0);

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

s32 TestCase_1022905_SupplyVoltageComparedFailureDTCTestWhenElPowerLevelIs1()
{
    strcpy(TestCaseName, "TestCase_1022905_Supply Voltage Compared Failure DTC Test When ElPowerLevel Is 1");
    strcpy(TestScopeName, "Supply Voltage DTC Test");
    strcpy(testPurpose_CH, "测试目的：");

    int index;
    float volt;
    double time;
    char StepDespriction[256];
    stepnumber = 0;
    u8 fdtc[3];
    fdtc[0] = (batVoltDTC.dtc & 0xff0000) >> 16;
    fdtc[1] = (batVoltDTC.dtc & 0xff00) >> 8;
    fdtc[2] = (batVoltDTC.dtc & 0xff);
    //if (gCEMFlg == 1) {
    //    PrintCaseNameAndPurpose();
    //    TestReport_TableInit();
    //    strcpy(StepDespriction, "This is VMM, this case is not applicable.");
    //    TestReport_HTML_StepDescription(StepDespriction, Info);
    //    return 0;
    //}
    if (batVoltDTC.dtc == 0)
    {
        PrintCaseNameAndPurpose();
        TestReport_TableInit();
        strcpy(StepDespriction, "No Compared voltage DTC, this case is not applicable.");
        TestReport_HTML_StepDescription(StepDespriction, Info);
        return 0;
    }
    if (batVoltDTC.tstRlst == 0)
    {
        PrintCaseNameAndPurpose();
        TestReport_TableInit();
        strcpy(StepDespriction, "DUT can not record high voltage DTC, break this case.");
        TestReport_HTML_StepDescription(StepDespriction, Fail);
        return 0;
    }

    try
    {
        //Step1
        if (0 != TestPrecondition())
            return -1;
        //TestStep("上电并唤醒被测件");
        //if (0 != CheckCommunication(5000)) {
        //    TestPostCondition();
        //    return -1;
        //}

        snprintf(StepDespriction, sizeof(StepDespriction), "Compared voltage DTC 0x%X", batVoltDTC.dtc);
        TestReport_HTML_StepDescription(StepDespriction);

        FulfilTRC();
        TRCSwitch(0x0, 0xD, 0x0);

        //step2
        TestStep("等待7s");
        AppWait(7000, "wait for 7s");

        //step3
        time = ceil(127 / batVoltDTC.SUValue) * batVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "清除DTC,等待%fms", time);
        TestStep(StepDespriction);
        SID14_ClearAllDTC();
        CheckPosResponse_SID14();
        AppWait(time, "");

        //Step4
        TestStep("使用掩码0x09读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x09);
        RX_CheckResponse_DTC_SID19_02(0x09, batVoltDTC.dtc, 0);

        //Step5
        TestStep("设置 ElPowerLevel 为 1");
        UsgModSwtich(0xD);
        EPLvlSwitch(0x1);

        //Step6
        AppWait(7000, "wait for 7s");

        //Step7
        volt = (12 - batVoltDTC.volt) - 0.35;
        snprintf(StepDespriction, sizeof(StepDespriction), "设置电压为 %5.2fV", volt);
        TestStep(StepDespriction);
        SetVoltageAndRead(volt, 0);

        time = ceil(127 / batVoltDTC.SUValue) * batVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 1.2 + 5s");
        TestReport_HTML_StepDescription(StepDespriction);
        snprintf(StepDespriction, sizeof(StepDespriction), "等待 %fms", time);
        TestReport_HTML_StepDescription(StepDespriction);
        AppWait(time, "");

        TestReport_HTML_StepDescription("使用掩码0x09读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x09);
        RX_CheckResponse_DTC_SID19_02(0x09, batVoltDTC.dtc, 0);

        //Step8
        TestStep("设置电压为12V");
        SetVoltageAndRead(12, 0);

        //Step9
        time = ceil(127 / batVoltDTC.SUValue) * batVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "清除DTC,等待%fms", time);
        TestStep(StepDespriction);
        SID14_ClearAllDTC();
        CheckPosResponse_SID14();
        AppWait(time, "");

        //Step10
        TestStep("使用掩码0x09读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x09);
        RX_CheckResponse_DTC_SID19_02(0x09, batVoltDTC.dtc, 0);

        //Step11
        TestStep("设置 ElPowerLevel 为 0");
        UsgModSwtich(0xD);
        EPLvlSwitch(0x0);

        //Step12
        AppWait(7000, "wait for 7s");

        //Step13
        volt = (12 - batVoltDTC.volt) - 0.35;
        snprintf(StepDespriction, sizeof(StepDespriction), "设置电压为 %5.2fV", volt);
        TestStep(StepDespriction);
        SetVoltageAndRead(volt, 0);

        time = ceil(127 / batVoltDTC.SUValue) * batVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 1.2 + 5s");
        TestReport_HTML_StepDescription(StepDespriction);
        snprintf(StepDespriction, sizeof(StepDespriction), "等待 %fms", time);
        TestReport_HTML_StepDescription(StepDespriction);
        AppWait(time, "");

        TestReport_HTML_StepDescription("使用掩码0x09读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x01);
        RX_CheckResponse_DTC_SID19_02(0x01, batVoltDTC.dtc, 1);

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

s32 TestCase_1022907_ExtendDataOfSupplyVoltageComparedFailureDTCTest()
{
    strcpy(TestCaseName, "TestCase_1022907_Extend Data Of Supply Voltage Compared Failure DTC Test");
    strcpy(TestScopeName, "Network fault related DTC Test - Supply Voltage DTC Test");
    strcpy(testPurpose_CH, "测试目的：");

    float volt;
    double time;
    char StepDespriction[256];
    int index, fdc1, fdc2, snapshot, stepflag;
    u8 fdtc[3];
    fdtc[0] = (batVoltDTC.dtc & 0xff0000) >> 16;
    fdtc[1] = (batVoltDTC.dtc & 0xff00) >> 8;
    fdtc[2] = (batVoltDTC.dtc & 0xff);

    stepnumber = 0;
    //if (gCEMFlg == 1) {
    //    PrintCaseNameAndPurpose();
    //    TestReport_TableInit();
    //    strcpy(StepDespriction, "This is VMM, this case is not applicable.");
    //    TestReport_HTML_StepDescription(StepDespriction, Info);
    //    return 0;
    //}
    if (batVoltDTC.dtc == 0)
    {
        strcpy(StepDespriction, "No Compared voltage DTC, this case is not applicable.");
        TestReport_HTML_StepDescription(StepDespriction, Info);
        return 0;
    }

    try
    {
        //Step1
        if (0 != TestPrecondition())
            return -1;
        //TestStep("上电并唤醒被测件");
        //if (0 != CheckCommunication(5000)) {
        //    TestPostCondition();
        //    return -1;
        //}

        FulfilTRC();
        TRCSwitch(0x0, 0xD, 0x0);

        //step2
        TestStep("等待7s");
        AppWait(7000, "wait for 7s");

        //step3
        time = ceil(127 / batVoltDTC.SUValue) * highVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "清除DTC,等待%fms", time);
        TestStep(StepDespriction);
        SID14_ClearAllDTC();
        CheckPosResponse_SID14();
        AppWait(time, "");

        //Step4
        TestStep("使用掩码0x09读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x09);
        RX_CheckResponse_DTC_SID19_02(0x09, batVoltDTC.dtc, 0);

        //Step5
        volt = 8.5;
        snprintf(StepDespriction, sizeof(StepDespriction), "设置电压为 %5.2fV", volt);
        TestStep(StepDespriction);
        SetVoltageAndRead(volt, 0);

        //Step6
        snprintf(StepDespriction, sizeof(StepDespriction), "Send diagnostic request with period %dms to read FDC10 and snapshot", highVoltDTC.testProd);
        TestStep(StepDespriction);
        gUnCofrmLmt = highVoltDTC.UnCofrmLmt;
        CheckStepUpValue(fdtc, batVoltDTC.testProd, batVoltDTC.SUValue);

        //Step7
        AppWait(10000, "");
        TestStep("使用掩码0x01读取DTC");
        RX_SID19_ReportDTCByStatusMask(0x01);
        RX_CheckResponse_DTC_SID19_02(0x01, batVoltDTC.dtc, 1);

        //Step8
        TestStep("设置电压为12V");
        SetVoltageAndRead(12, 0);

        AppWait(20, "");

        //Step9
        snprintf(StepDespriction, sizeof(StepDespriction), "Send diagnostic request with period %dms to read FDC10", batVoltDTC.testProd);
        TestStep(StepDespriction);
        CheckStepDownValue(fdtc, batVoltDTC.testProd, batVoltDTC.SDValue);

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

s32 TestCase_1022875_CANcircuiterrorDTC()
{
    strcpy(TestCaseName, "TestCase_1022875_CAN circuit error DTC");
    strcpy(TestScopeName, "Network fault related DTC Test on CAN");
    strcpy(testPurpose_CH, "测试目的：");

    float volt;
    u8 fdtc[3];
    double time;
    int index, j = 0;
    char StepDespriction[256];
    stepnumber = 0;

    try
    {
        //Step1
        if (0 != TestPrecondition())
            return -1;
        //TestStep("上电并唤醒被测件");
        //if (0 != CheckCommunication(5000)) {
        //    TestPostCondition();
        //    return -1;
        //}

        FulfilTRC();
        TRCSwitch(0x0, 0xD, 0x0);

        for (index = 0; index < BusNum; index++)
        {
            //Step2
            TestStep("等待7s");
            AppWait(7000, "wait for 7s");

            //Step3
            time = 5000;
            snprintf(StepDespriction, sizeof(StepDespriction), "清除DTC,等待%fms", time);
            TestStep(StepDespriction);
            SID14_ClearAllDTC();
            CheckPosResponse_SID14();
            AppWait(time, "");

            //Step4
            TestStep("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].ElctrFltDTC, 0);

            //Step5
            switch (j)
            {
            case 0:
                strcpy(StepDespriction, "Open the CANH of test CAN");
                TestStep(StepDespriction);
                boardcard::instance()->CAN_H_to_NCAN_H(OFF);
                break;
            case 1:
                strcpy(StepDespriction, "Open the CANL of test CAN");
                TestStep(StepDespriction);
                boardcard::instance()->CAN_L_to_NCAN_L(OFF);
                break;
            case 2:
                strcpy(StepDespriction, "Open the CANH and CANL of test CAN");
                TestStep(StepDespriction);
                boardcard::instance()->CAN_H_to_NCAN_H(OFF);
                boardcard::instance()->CAN_L_to_NCAN_L(OFF);
                break;
            }
            //Step6
            time = ceil(127 / busFltDTCTemp[index].SUValue) * busFltDTCTemp[index].testProd * 0.8;
            snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 0.8");
            TestReport_HTML_StepDescription(StepDespriction);
            snprintf(StepDespriction, sizeof(StepDespriction), "等待 %fms", time);
            TestReport_HTML_StepDescription(StepDespriction);
            AppWait(time, "");


            //Step7
            TestStep("使用掩码0x01读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x01);
            RX_CheckResponse_DTC_SID19_02(0x01, busFltDTCTemp[index].ElctrFltDTC, 0);

            //Step8
            time = ceil(127 / busFltDTCTemp[index].SUValue) * busFltDTCTemp[index].testProd * 0.4;
            snprintf(StepDespriction, sizeof(StepDespriction), "等待(127 / StepUpValue) * TestPeriod * 0.4");
            TestReport_HTML_StepDescription(StepDespriction);
            snprintf(StepDespriction, sizeof(StepDespriction), "等待 %fms", time);
            TestReport_HTML_StepDescription(StepDespriction);
            AppWait(time, "");

            //Step9
            TestStep("使用掩码0x01读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x01);
            RX_CheckResponse_DTC_SID19_02(0x01, busFltDTCTemp[index].ElctrFltDTC, 1);

            //Step10
            TestStep("移除故障，等待DUT恢复通信");
            boardcard::instance()->CAN_H_to_NCAN_H(ON);
            boardcard::instance()->CAN_L_to_NCAN_L(ON);

            //Step11
            TestStep("等待w5s");
            AppWait(5000, "wait for 5s");

            //Step12
            TestStep("使用掩码0x01读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x01);
            RX_CheckResponse_DTC_SID19_02(0x01, busFltDTCTemp[index].ElctrFltDTC, 0);
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

s32 TestCase_1022876_FDC10testofCANcircuiterrorDTC()
{
    strcpy(TestCaseName, "TestCase_1022876_FDC10 test of CAN circuit error DTC");
    strcpy(TestScopeName, "Network fault related DTC Test on CAN");
    strcpy(testPurpose_CH, "测试目的：");

    float volt;
    int index, j;
    u8 fdtc[3];
    char StepDespriction[256];
    stepnumber = 0;
    index = 0;

    fdtc[0] = (busFltDTCTemp[index].ElctrFltDTC & 0xff0000) >> 16;
    fdtc[1] = (busFltDTCTemp[index].ElctrFltDTC & 0xff00) >> 8;
    fdtc[2] = (busFltDTCTemp[index].ElctrFltDTC & 0xff);

    try
    {
        //Step1
        if (0 != TestPrecondition())
            return -1;
        //TestStep("上电并唤醒被测件");
        //if (0 != CheckCommunication(5000)) {
        //    TestPostCondition();
        //    return -1;
        //}

        FulfilTRC();
        TRCSwitch(0x0, 0xD, 0x0);

        for (index = 0; index < BusNum; index++)
        {
            if (busFltDTCTemp[index].ElctrFltDTC == 0x0)continue;
            if (strncmp(busFltDTCTemp[index].cluster, TpBusName_Temp, 32) == 0)continue;

            snprintf(StepDespriction, sizeof(StepDespriction), "-------------------------Test bus %s CAN circuit error DTC:0x%X---------------------------", busFltDTCTemp[index].cluster, busFltDTCTemp[index].ElctrFltDTC);
            TestReport_HTML_StepDescription(StepDespriction);

            //Step2
            TestStep("等待7s");
            AppWait(7000, "wait for 7s");

            //Step3
            snprintf(StepDespriction, sizeof(StepDespriction), "清除DTC,等待10000ms", time);
            TestStep(StepDespriction);
            SID14_ClearAllDTC();
            CheckPosResponse_SID14();
            AppWait(10000, "");

            //Step4
            TestStep("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].ElctrFltDTC, 0);

            //Step5
            strcpy(StepDespriction, "Open the CANH of test CAN");
            TestStep(StepDespriction);
            boardcard::instance()->CAN_H_to_NCAN_H(OFF);

            //Step6
            snprintf(StepDespriction, sizeof(StepDespriction), "Send diagnostic request with period %dms to read FDC10", busFltDTCTemp[index].testProd);
            TestReport_HTML_Step(StepNumber, StepDespriction, Info);
            CheckStepUpValue(fdtc, busFltDTCTemp[index].testProd, busFltDTCTemp[index].SUValue);

            //Step7
            TestStep("移除故障，等待DUT恢复通信");
            boardcard::instance()->CAN_H_to_NCAN_H(ON);

            //Step8
            snprintf(StepDespriction, sizeof(StepDespriction), "Send diagnostic request with period %dms to read FDC10", busFltDTCTemp[index].testProd);
            TestReport_HTML_Step(StepNumber, StepDespriction, Info);
            CheckStepDownValue(fdtc, busFltDTCTemp[index].testProd, busFltDTCTemp[index].SDvalue);
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

s32 TestCase_1022877_UnconfirmedDTCLimittestofCANcircuiterrorDTC()
{
    strcpy(TestCaseName, "TestCase_1022876_FDC10 test of CAN circuit error DTC");
    strcpy(TestScopeName, "Network fault related DTC Test on CAN");
    strcpy(testPurpose_CH, "测试目的：");

    float volt;
    int index, j;
    u8 fdtc[3];
    char StepDespriction[256];
    int snapShot, FDC10;
    index = 0;

    stepnumber = 0;

    fdtc[0] = (busFltDTCTemp[index].ElctrFltDTC & 0xff0000) >> 16;
    fdtc[1] = (busFltDTCTemp[index].ElctrFltDTC & 0xff00) >> 8;
    fdtc[2] = (busFltDTCTemp[index].ElctrFltDTC & 0xff);

    try
    {
        //Step1
        if (0 != TestPrecondition())
            return -1;
        //TestStep("上电并唤醒被测件");
        //if (0 != CheckCommunication(5000)) {
        //    TestPostCondition();
        //    return -1;
        //}

        FulfilTRC();
        TRCSwitch(0x0, 0xD, 0x0);

        for (index = 0; index < BusNum; index++)
        {
            if (busFltDTCTemp[index].ElctrFltDTC == 0x0)continue;
            if (strncmp(busFltDTCTemp[index].cluster, TpBusName_Temp, 32) == 0)continue;

            snprintf(StepDespriction, sizeof(StepDespriction), "-------------------------Test bus %s CAN circuit error DTC:0x%X---------------------------", busFltDTCTemp[index].cluster, busFltDTCTemp[index].ElctrFltDTC);
            TestReport_HTML_StepDescription(StepDespriction);

            //Step2
            TestStep("等待7s");
            AppWait(7000, "wait for 7s");

            //Step3
            snprintf(StepDespriction, sizeof(StepDespriction), "清除DTC,等待5000ms", time);
            TestStep(StepDespriction);
            SID14_ClearAllDTC();
            CheckPosResponse_SID14();
            AppWait(5000, "");

            //Step4
            TestStep("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].ElctrFltDTC, 0);

            //Step5
            snapShot = ReadSnapShot(fdtc);
            if (snapShot == 0)
            {
                strcpy(StepDespriction, "No snapshot record in the response.");
                TestReport_HTML_Step(stepnumber, StepDespriction, Pass);
            }
            else
            {
                strcpy(StepDespriction, "Find snapshot record in the response.");
                TestReport_HTML_Step(stepnumber, StepDespriction, Fail);
            }

            //Step6
            strcpy(StepDespriction, "Open the CANH of test CAN");
            TestStep(StepDespriction);
            boardcard::instance()->CAN_H_to_NCAN_H(ON);

            //Step7
            strcpy(StepDespriction, "Send diagnostic request with 100ms to read FDC10 and UnconfirmedDTC, until FDC10 >= UnconfirmedDTCLimit");
            TestStep(StepDespriction);

            for (j = 0; j < 100; j++)
            {
                AppWait(100, "");
                FDC10 = ConvertUnsgnToSgn(ReadFDC10(fdtc));
                snapShot = ReadSnapShot(fdtc);
                if (FDC10 >= busFltDTCTemp[index].UnCofrmLmt)
                {
                    if (snapShot == 0)
                    {
                        strcpy(StepDespriction, "No snapshot record in the response when FDC10 is greater than UncomfirmedLimit.");
                        TestReport_HTML_StepDescription(StepDespriction, Fail);
                    }
                    else
                    {
                        strcpy(StepDespriction, "Find snapshot record in the response when FDC10 is greater than UncomfirmedLimit.");
                        TestReport_HTML_StepDescription(StepDespriction, Pass);
                    }
                    break;
                }
                else
                {
                    if (snapShot == 0)
                    {
                        strcpy(StepDespriction, "No snapshot record in the response when FDC10 is smaller than UncomfirmedLimit.");
                        TestReport_HTML_StepDescription(StepDespriction, Pass);
                    }
                    else
                    {
                        strcpy(StepDespriction, "Find snapshot record in the response when FDC10 is smaller than UncomfirmedLimit.");
                        TestReport_HTML_StepDescription(StepDespriction, Fail);
                    }
                }
            }
            //Step8
            TestStep("移除故障，等待DUT恢复通信");
            boardcard::instance()->CAN_H_to_NCAN_H(ON);
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

s32 TestCase_1022878_ConfirmedDTCandDTCAgedLimittestofCANcircuiterrorDTC()
{
    strcpy(TestCaseName, "TestCase_1022878_Confirmed DTC and DTC Aged Limittest of CAN circuit error DTC");
    strcpy(TestScopeName, "Network fault related DTC Test on CAN");
    strcpy(testPurpose_CH, "测试目的：");

    float volt;
    u8 fdtc[3];
    double time;
    int index, j;
    int occ6, occ2, ageDTC;
    char StepDespriction[256];
    stepnumber = 0;

    try
    {
        //Step1
        if (0 != TestPrecondition())
            return -1;
        //TestStep("上电并唤醒被测件");
        //if (0 != CheckCommunication(5000)) {
        //    TestPostCondition();
        //    return -1;
        //}

        FulfilTRC();
        TRCSwitch(0x0, 0xD, 0x0);

        for (index = 0; index < BusNum; index++)
        {
            if (busFltDTCTemp[index].ElctrFltDTC == 0x0)continue;
            if (strncmp(busFltDTCTemp[index].cluster, TpBusName_Temp, 32) == 0)continue;

            snprintf(StepDespriction, sizeof(StepDespriction), "-------------------------Test bus %s CAN circuit error DTC:0x%X---------------------------", busFltDTCTemp[index].cluster, busFltDTCTemp[index].ElctrFltDTC);
            TestReport_HTML_StepDescription(StepDespriction);

            //Step2
            TestStep("等待7s");
            AppWait(7000, "wait for 7s");

            //Step3
            time = 5000;
            snprintf(StepDespriction, sizeof(StepDespriction), "清除DTC,等待%fms", time);
            TestStep(StepDespriction);
            SID14_ClearAllDTC();
            CheckPosResponse_SID14();
            AppWait(time, "");

            //Step4
            TestStep("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].ElctrFltDTC, 0);


            //Step5
            strcpy(StepDespriction, "Open the CANH of test CAN");
            TestStep(StepDespriction);
            boardcard::instance()->CAN_H_to_NCAN_H(OFF);

            //Step6
            TestStep("等待5s,移除故障");
            AppWait(5000, "");
            boardcard::instance()->CAN_H_to_NCAN_H(ON);

            //Step7
            TestStep("Read OCC6 of test DTC");
            occ6 = ReadOCC6(fdtc);
            if (occ6 == 1)
            {
                TestReport_HTML_StepDescription("OCC6 值为 1", Pass);
            }
            else {
                TestReport_HTML_StepDescription("OCC6 值不为 1", Fail);
            }

            //Step8
            TestStep("使用掩码0x08读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x08);
            if (j == busFltDTCTemp[index].CofrmLmt)
            {
                RX_CheckResponse_DTC_SID19_02(0x08, busFltDTCTemp[index].ElctrFltDTC, 1);
            }
            else
            {
                RX_CheckResponse_DTC_SID19_02(0x08, busFltDTCTemp[index].ElctrFltDTC, 0);
            }

            for (j = 0; j < busFltDTCTemp[index].CofrmLmt - 1; j++)
            {
                stepnumber = 9;
                //Step9
                TestStep("使DUT进入下一个operation周期，等待7s");
                AppWait(7000, "");

                //Step10
                strcpy(StepDespriction, "Open the CANH of test CAN");
                TestStep(StepDespriction);
                boardcard::instance()->CAN_H_to_NCAN_H(OFF);

                //Step11
                TestStep("等待5s,移除故障");
                AppWait(5000, "");
                boardcard::instance()->CAN_H_to_NCAN_H(ON);

                //Step12
                TestStep("Read OCC6 of test DTC");
                if (gAsrVersion < 4.1)
                {
                    occ6 = ReadOCC6(fdtc);
                    if (occ6 == (j + 2))
                    {
                        snprintf(StepDespriction, sizeof(StepDespriction), "The OCC6 is %d", occ6);
                        TestReport_HTML_StepDescription(StepDespriction, Pass);
                    }
                    else
                    {
                        snprintf(StepDespriction, sizeof(StepDespriction), "The OCC6 is %d, expected is %d", occ6, j + 2);
                        TestReport_HTML_StepDescription(StepDespriction, Fail);
                    }
                }

                //Step14
                TestStep("等待7s,使用掩码0x08读取DTC");
                AppWait(7000, "");
                RX_SID19_ReportDTCByStatusMask(0x08);
                if (j == busFltDTCTemp[index].CofrmLmt - 2)
                {
                    RX_CheckResponse_DTC_SID19_02(0x08, busFltDTCTemp[index].ElctrFltDTC, 1);
                }
                else
                {
                    RX_CheckResponse_DTC_SID19_02(0x08, busFltDTCTemp[index].ElctrFltDTC, 0);
                }
            }

            AppWait(5000, "");
            for (j = 0; j < 300; j++)
            {
                //Step15
                TestStep("使DUT进入下一个operation周期，等待10s");
                AppWait(10000, "");

                //Step16
                TestStep("读OCC2或者0CC5");
                occ2 = ReadAgingCounter(fdtc, gOcc2Or5);
                if (occ2 == j)
                {
                    snprintf(StepDespriction, sizeof(StepDespriction), "OCC%d is %d", gOcc2Or5, occ2);
                    TestReport_HTML_StepDescription(StepDespriction, Pass);
                }
                else
                {
                    if (j > 254)
                    {
                        snprintf(StepDespriction, sizeof(StepDespriction), "OCC%d is %d, expected is %d or 0", gOcc2Or5, occ2, 255);
                        TestReport_HTML_StepDescription(StepDespriction, Info);
                    }
                    else
                    {
                        snprintf(StepDespriction, sizeof(StepDespriction), "OCC%d is %d, expected is %d", gOcc2Or5, occ2, j);
                        TestReport_HTML_StepDescription(StepDespriction, Fail);
                    }
                }

                //Step17
                TestStep("使用掩码0x09读取DTC");
                RX_SID19_ReportDTCByStatusMask(0x09);
                if (occ2 < 255)
                {
                    RX_CheckResponse_DTC_SID19_02(0x08, busFltDTCTemp[index].ElctrFltDTC, 1);
                }
                else
                {
                    RX_CheckResponse_DTC_SID19_02(0x08, busFltDTCTemp[index].ElctrFltDTC, 0);
                }

                //Step18
                TestStep("读AgedDTC");
                if (gAsrVersion < 4.1)
                {
                    ageDTC = ReadAgedDTC(fdtc);
                    if (ageDTC == 1)
                    {
                        if (occ2 < 255)
                        {
                            strcpy(StepDespriction, "ageDTC is 1");
                            TestReport_HTML_StepDescription(StepDespriction, Fail);
                        }
                        else
                        {
                            strcpy(StepDespriction, "ageDTC is 1, expected is 0");
                            TestReport_HTML_StepDescription(StepDespriction, Pass);
                        }
                    }
                    else
                    {
                        if (occ2 < 255)
                        {
                            strcpy(StepDespriction, "ageDTC is 0, expected is 0.");
                            TestReport_HTML_StepDescription(StepDespriction, Pass);
                        }
                        else
                        {
                            strcpy(StepDespriction, "ageDTC is 0, expected is 1");
                            TestReport_HTML_StepDescription(StepDespriction, Fail);
                        }
                    }
                }
                if (occ2 >= 255)
                {
                    break;
                }
                if ((occ2 == 0) && (j == 10))
                {
                    snprintf(StepDespriction, sizeof(StepDespriction), "OCC%d keeps to 0, break the test.", gOcc2Or5);
                    TestReport_HTML_StepDescription(StepDespriction, Fail);
                    break;
                }
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

s32 TestCase_1022879_CANcircuiterrorDTCwhenElPowerLevelis1()
{
    strcpy(TestCaseName, "TestCase_1022879_CAN circuit error DTC when ElPowerLevel is 1");
    strcpy(TestScopeName, "Network fault related DTC Test on CAN");
    strcpy(testPurpose_CH, "测试目的：");

    float volt;
    int index;
    double time;
    char StepDespriction[256];
    stepnumber = 0;

    try
    {
        //Step1
        if (0 != TestPrecondition())
            return -1;
        //TestStep("上电并唤醒被测件");
        //if (0 != CheckCommunication(5000)) {
        //    TestPostCondition();
        //    return -1;
        //}

        FulfilTRC();
        TRCSwitch(0x0, 0xD, 0x0);

        for (index = 0; index < BusNum; index++)
        {
            if (busFltDTCTemp[index].ElctrFltDTC == 0x0)continue;
            if (strncmp(busFltDTCTemp[index].cluster, TpBusName_Temp, 32) == 0)continue;

            snprintf(StepDespriction, sizeof(StepDespriction), "-------------------------Test bus %s CAN circuit error DTC:0x%X---------------------------", busFltDTCTemp[index].cluster, busFltDTCTemp[index].ElctrFltDTC);
            TestReport_HTML_StepDescription(StepDespriction);

            //Step2
            TestStep("等待7s");
            AppWait(7000, "wait for 7s");

            //Step3
            time = 5000;
            snprintf(StepDespriction, sizeof(StepDespriction), "清除DTC,等待%fms", time);
            TestStep(StepDespriction);
            SID14_ClearAllDTC();
            CheckPosResponse_SID14();
            AppWait(time, "");

            //Step4
            TestStep("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].ElctrFltDTC, 0);

            //Step5
            TestStep("设置 ElPowerLevel 为 1,等待7s");
            UsgModSwtich(0xD);
            EPLvlSwitch(0x1);
            AppWait(7000, "");

            //Step6
            strcpy(StepDespriction, "Open the CANH of test CAN");
            TestStep(StepDespriction);
            boardcard::instance()->CAN_H_to_NCAN_H(OFF);

            //Step7
            TestStep("等待5s");
            AppWait(5000, "");

            //Step8
            TestStep("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].ElctrFltDTC, 0);

            //Step9
            TestStep("移除故障");
            boardcard::instance()->CAN_H_to_NCAN_H(ON);
            AppWait(1000, "");

            //Step10
            TestStep("设置 ElPowerLevel 为 0 ，等待7s");
            UsgModSwtich(0xD);
            EPLvlSwitch(0x0);
            AppWait(7000, "");

            //Step11
            strcpy(StepDespriction, "Open the CANH of test CAN");
            TestStep(StepDespriction);
            boardcard::instance()->CAN_H_to_NCAN_H(OFF);

            //Step12
            TestStep("等待5s");
            AppWait(5000, "");

            //Step13
            TestStep("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x01);
            RX_CheckResponse_DTC_SID19_02(0x01, busFltDTCTemp[index].ElctrFltDTC, 0);

            //Step14
            TestStep("移除故障");
            boardcard::instance()->CAN_H_to_NCAN_H(ON);
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

s32 TestCase_1022880_CANcircuiterrorDTCwhenthevoltageofpowersupplyisoutofrange()
{
    strcpy(TestCaseName, "TestCase_1022880_CAN circuit error DTC when the voltage of power supply is out of range");
    strcpy(TestScopeName, "Network fault related DTC Test on CAN");
    strcpy(testPurpose_CH, "测试目的：");

    float volt;
    int index;
    double time;
    char StepDespriction[256];
    stepnumber = 0;

    try
    {
        //Step1
        if (0 != TestPrecondition())
            return -1;
        //TestStep("上电并唤醒被测件");
        //if (0 != CheckCommunication(5000)) {
        //    TestPostCondition();
        //    return -1;
        //}

        FulfilTRC();
        TRCSwitch(0x0, 0xD, 0x0);

        for (index = 0; index < BusNum; index++)
        {
            if (busFltDTCTemp[index].ElctrFltDTC == 0x0)continue;
            if (strncmp(busFltDTCTemp[index].cluster, TpBusName_Temp, 32) == 0)continue;

            snprintf(StepDespriction, sizeof(StepDespriction), "-------------------------Test bus %s CAN circuit error DTC:0x%X---------------------------", busFltDTCTemp[index].cluster, busFltDTCTemp[index].ElctrFltDTC);
            TestReport_HTML_StepDescription(StepDespriction);

            //Step2
            TestStep("等待7s");
            AppWait(7000, "wait for 7s");

            //Step3
            time = 5000;
            snprintf(StepDespriction, sizeof(StepDespriction), "清除DTC,等待%fms", time);
            TestStep(StepDespriction);
            SID14_ClearAllDTC();
            CheckPosResponse_SID14();
            AppWait(time, "");

            //Step4
            TestStep("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].ElctrFltDTC, 0);

            //Step5
            volt = gVoltLow;
            snprintf(StepDespriction, sizeof(StepDespriction), "设置电压为 %5.2fV", volt);
            TestStep(StepDespriction);
            SetVoltageAndRead(volt);

            //Step6
            strcpy(StepDespriction, "Open the CANH of test CAN");
            TestStep(StepDespriction);
            boardcard::instance()->CAN_H_to_NCAN_H(OFF);

            //Step7
            TestStep("等待5s");
            AppWait(5000, "");

            //Step8
            TestStep("使用掩码0x01读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x01);
            RX_CheckResponse_DTC_SID19_02(0x01, busFltDTCTemp[index].ElctrFltDTC, 0);

            //Step9
            TestStep("移除故障");
            boardcard::instance()->CAN_H_to_NCAN_H(ON);
            AppWait(1000, "");

            //Step10
            volt = gVoltHigh;
            snprintf(StepDespriction, sizeof(StepDespriction), "设置电压为 %5.2fV", volt);
            TestStep(StepDespriction);
            SetVoltageAndRead(volt);

            //Step11
            strcpy(StepDespriction, "Open the CANH of test CAN");
            TestStep(StepDespriction);
            boardcard::instance()->CAN_H_to_NCAN_H(OFF);

            //Step12
            TestStep("等待5s");
            AppWait(5000, "");

            //Step13
            TestStep("使用掩码0x01读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x01);
            RX_CheckResponse_DTC_SID19_02(0x01, busFltDTCTemp[index].ElctrFltDTC, 0);

            //Step14
            TestStep("移除故障");
            boardcard::instance()->CAN_H_to_NCAN_H(ON);
            AppWait(1000, "");

            //Step15
            volt = 12;
            snprintf(StepDespriction, sizeof(StepDespriction), "设置电压为 %5.2fV,等待7s", volt);
            TestStep(StepDespriction);
            SetVoltageAndRead(volt);
            AppWait(7000, "");

            //Step16
            strcpy(StepDespriction, "Open the CANH of test CAN");
            TestStep(StepDespriction);
            boardcard::instance()->CAN_H_to_NCAN_H(OFF);

            //Step17
            TestStep("等待5s");
            AppWait(5000, "");

            //Step18
            TestStep("使用掩码0x01读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x01);
            RX_CheckResponse_DTC_SID19_02(0x01, busFltDTCTemp[index].ElctrFltDTC, 1);

            //Step19
            TestStep("移除故障");
            boardcard::instance()->CAN_H_to_NCAN_H(ON);
            AppWait(1000, "");
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

s32 TestCase_1022881_CANcircuiterrorDTCwhenCarModeisnotNormal()
{
    strcpy(TestCaseName, "TestCase_1022881_CAN circuit error DTC when CarMode is not Normal");
    strcpy(TestScopeName, "Network fault related DTC Test on CAN");
    strcpy(testPurpose_CH, "测试目的：");

    float volt;
    int index, j;
    double time;
    u8 carmode[3];
    char StepDespriction[256];
    stepnumber = 0;

    carmode[0] = gCMFctyValue;
    carmode[1] = gCMTsptValue;
    carmode[2] = gCMCrshValue;

    try
    {
        //Step1
        if (0 != TestPrecondition())
            return -1;
        //TestStep("上电并唤醒被测件");
        //if (0 != CheckCommunication(5000)) {
        //    TestPostCondition();
        //    return -1;
        //}

        FulfilTRC();
        TRCSwitch(0x0, 0xD, 0x0);

        for (index = 0; index < BusNum; index++)
        {
            if (busFltDTCTemp[index].ElctrFltDTC == 0x0)continue;
            if (strncmp(busFltDTCTemp[index].cluster, TpBusName_Temp, 32) == 0)continue;

            snprintf(StepDespriction, sizeof(StepDespriction), "-------------------------Test bus %s CAN circuit error DTC:0x%X---------------------------", busFltDTCTemp[index].cluster, busFltDTCTemp[index].ElctrFltDTC);
            TestReport_HTML_StepDescription(StepDespriction);

            //Step2
            TestStep("等待7s");
            AppWait(7000, "wait for 7s");

            for (index = 0; index < 3; index++)
            {
                stepnumber = 3;
                //Step3
                time = 5000;
                snprintf(StepDespriction, sizeof(StepDespriction), "清除DTC,等待%fms", time);
                TestStep(StepDespriction);
                SID14_ClearAllDTC();
                CheckPosResponse_SID14();
                AppWait(time, "");

                //Step4
                TestStep("使用掩码0x09读取DTC");
                RX_SID19_ReportDTCByStatusMask(0x09);
                RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].ElctrFltDTC, 0);

                //Step5
                snprintf(StepDespriction, sizeof(StepDespriction), "设置 car mode 为 0x%x.(Transport:0x1;Factory:0x2;Crash:0x3)", carmode[index]);
                TestStep(StepDespriction);
                CarModSwtich(carmode[index]);

                //Step6
                TestStep("等待7s");
                AppWait(7000, "wait for 7s");

                //Step7
                strcpy(StepDespriction, "Open the CANH of test CAN");
                TestStep(StepDespriction);
                boardcard::instance()->CAN_H_to_NCAN_H(OFF);

                //Step8
                TestStep("等待5s");
                AppWait(7000, "wait for 5s");

                //Step9
                TestStep("使用掩码0x09读取DTC");
                RX_SID19_ReportDTCByStatusMask(0x09);
                RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].ElctrFltDTC, 0);

                //Step10
                TestStep("移除故障");
                boardcard::instance()->CAN_H_to_NCAN_H(ON);
                AppWait(1000, "");

                //Step11
                if (index != 2)
                {
                    TestStep("重复步骤3-8，切换Car mode 为Transport和Carsh");
                }
            }

            //Step12
            TestStep("设置Car mode 为 normal");
            CarModSwtich(0x0);

            //Step13
            TestStep("等待7s");
            AppWait(7000, "wait for 7s");

            //Step14
            strcpy(StepDespriction, "Open the CANH of test CAN");
            TestStep(StepDespriction);
            boardcard::instance()->CAN_H_to_NCAN_H(OFF);

            //Step15
            TestStep("等待5s");
            AppWait(5000, "wait for 5s");

            //Step16
            TestStep("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].ElctrFltDTC, 1);

            TestStep("移除故障");
            boardcard::instance()->CAN_H_to_NCAN_H(ON);
            AppWait(1000, "");
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

s32 TestCase_1022882_CANcircuiterrorDTCafterCarModetransmission()
{
    strcpy(TestCaseName, "TestCase_1022882_CAN circuit error DTC after CarMode transmission");
    strcpy(TestScopeName, "Network fault related DTC Test on CAN");
    strcpy(testPurpose_CH, "测试目的：");

    float volt;
    int index, j;
    int distrbCount;
    char distrbCountString[10];
    char StepDespriction[256];
    stepnumber = 0;

    try
    {
        //Step1
        if (0 != TestPrecondition())
            return -1;
        //TestStep("上电并唤醒被测件");
        //if (0 != CheckCommunication(5000)) {
        //    TestPostCondition();
        //    return -1;
        //}

        FulfilTRC();
        TRCSwitch(0x0, 0xD, 0x0);

        for (index = 0; index < BusNum; index++)
        {
            if (busFltDTCTemp[index].ElctrFltDTC == 0x0)continue;
            if (strncmp(busFltDTCTemp[index].cluster, TpBusName_Temp, 32) == 0)continue;

            snprintf(StepDespriction, sizeof(StepDespriction), "-------------------------Test bus %s CAN circuit error DTC:0x%X---------------------------", busFltDTCTemp[index].cluster, busFltDTCTemp[index].ElctrFltDTC);
            TestReport_HTML_StepDescription(StepDespriction);

            //Step2
            TestStep("等待7s");
            AppWait(7000, "wait for 7s");

            //Step3
            snprintf(StepDespriction, sizeof(StepDespriction), "清除DTC,等待5000ms", time);
            TestStep(StepDespriction);
            SID14_ClearAllDTC();
            CheckPosResponse_SID14();
            AppWait(5000, "");

            //Step4
            TestStep("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].ElctrFltDTC, 0);

            //Step5
            TestStep("设置Car mode 为 Factory");
            CarModSwtich(0x2);

            //Step6
            TestStep("等待7s");
            AppWait(7000, "wait for 7s");

            //Step7
            TestStep("设置Car mode 为 Normal");
            CarModSwtich(0x0);

            //Step8
            strcpy(StepDespriction, "Open the CANH of test CAN");
            TestStep(StepDespriction);
            boardcard::instance()->CAN_H_to_NCAN_H(OFF);

            //Step9
            TestStep("等待5s");
            AppWait(5000, "wait for 5s");

            //Step10
            TestStep("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].ElctrFltDTC, 0);

            //Step11
            TestStep("等待5s");
            AppWait(10000, "wait for 10s");

            //Step12
            TestStep("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].ElctrFltDTC, 1);

            //Step13
            TestStep("移除故障");
            boardcard::instance()->CAN_H_to_NCAN_H(ON);
            AppWait(1000, "");
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

s32 TestCase_1022883_CommonCANelectricalfaultDTCwhenUsageModetransition()
{
    strcpy(TestCaseName, "TestCase_1022883_Common CAN electrical fault DTC when Usage Mode transition");
    strcpy(TestScopeName, "Network fault related DTC Test on CAN");
    strcpy(testPurpose_CH, "测试目的：");

    int index, j;
    double time;
    char StepDespriction[256];
    stepnumber = 0;

    try
    {
        //Step1
        if (0 != TestPrecondition())
            return -1;
        //TestStep("上电并唤醒被测件");
        //if (0 != CheckCommunication(5000)) {
        //    TestPostCondition();
        //    return -1;
        //}

        FulfilTRC();
        TRCSwitch(0x0, 0xD, 0x0);

        for (index = 0; index < BusNum; index++)
        {
            if (busFltDTCTemp[index].ElctrFltDTC == 0x0)continue;
            if (strncmp(busFltDTCTemp[index].cluster, TpBusName_Temp, 32) == 0)continue;

            snprintf(StepDespriction, sizeof(StepDespriction), "-------------------------Test bus %s CAN circuit error DTC:0x%X---------------------------", busFltDTCTemp[index].cluster, busFltDTCTemp[index].ElctrFltDTC);
            TestReport_HTML_StepDescription(StepDespriction);

            //Step2
            TestStep("等待7s");
            AppWait(7000, "wait for 7s");

            //Step3
            time = 5000;
            snprintf(StepDespriction, sizeof(StepDespriction), "清除DTC,等待%fms", time);
            TestStep(StepDespriction);
            SID14_ClearAllDTC();
            CheckPosResponse_SID14();
            AppWait(time, "");

            //Step4
            TestStep("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].ElctrFltDTC, 0);

            //Step5
            TestStep("设置Usg mode 为 Inactive");
            UsgModSwtich(0xB);

            //Step6
            TestStep("等待2s");
            AppWait(2000, "wait for 2s");

            //Step7
            TestStep("设置Usg mode 为 Driving");
            UsgModSwtich(0xD);

            //Step8
            strcpy(StepDespriction, "Open the CANH of test CAN");
            TestStep(StepDespriction);
            boardcard::instance()->CAN_H_to_NCAN_H(OFF);

            //Step9
            TestStep("等待4s");
            AppWait(4000, "wait for 4s");

            //Step10
            TestStep("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].ElctrFltDTC, 0);

            //Step11
            TestStep("等5s");
            AppWait(5000, "wait for 5s");

            //Step12
            TestStep("使用掩码0x01读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x01);
            RX_CheckResponse_DTC_SID19_02(0x01, busFltDTCTemp[index].ElctrFltDTC, 1);

            //Step13
            TestStep("移除故障");
            boardcard::instance()->CAN_H_to_NCAN_H(ON);
            AppWait(1000, "");
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

s32 TestCase_1022856_BusoffDTC()
{
    strcpy(TestCaseName, "TestCase_1022856_Bus off DTC");
    strcpy(TestScopeName, "Busoff DTC");
    strcpy(testPurpose_CH, "测试目的：");

    int index;
    double time;
    char distrbCount[10];
    char StepDespriction[256];
    s32 THhandle = -1;
    s64 firstTime = 0;
    stepnumber = 0;

    try
    {
        //Step1
        if (0 != TestPrecondition())
            return -1;
        //TestStep("上电并唤醒被测件");
        //if (0 != CheckCommunication(5000)) {
        //    TestPostCondition();
        //    return -1;
        //}

        FulfilTRC();
        TRCSwitch(0x0, 0xD, 0x0);

        THhandle = DisturbConnect();

        for (index = 0; index < BusNum; index++)
        {
            snprintf(StepDespriction, sizeof(StepDespriction), "-------------------------Test bus %s busoff:0x%X---------------------------", busFltDTCTemp[index].cluster, busFltDTCTemp[index].busOffDTC);
            TestReport_HTML_StepDescription(StepDespriction, Info);

            //Step2
            TestStep("等待7s");
            AppWait(7000, "wait for 7s");

            //Step3
            time = 5000;
            snprintf(StepDespriction, sizeof(StepDespriction), "清除DTC,等待%fms", time);
            TestStep(StepDespriction);
            SID14_ClearAllDTC();
            CheckPosResponse_SID14();
            AppWait(time, "");

            //Step4
            TestStep("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].busOffDTC, 0);

            //Step5
            snprintf(StepDespriction, sizeof(StepDespriction), "干扰所有被测件发出的报文,确保被测件连续%d次Busoff,然后等待DUT恢复通信", CanSMBorCounterL2Err);
            TestStep(StepDespriction);
            //InterfereCanMessage(32,31);
            //GEEA3.0
             //DisturbFrame(THhandle, index, 32, 31, TimeOut_7011_Disturb);
             //GEEA3.5
            DisturbFrame(THhandle, index, 32, CanSMBorCounterL2Err, TimeOut_7011_Disturb);
            if (0 != CheckCommunication(DUTrecoveryTime, NetworkCluster[busFltDTCTemp[index].cluster])) {
                break;
            }

            //Step6
            TestStep("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].busOffDTC, 0);

            //Step7
            snprintf(StepDespriction, sizeof(StepDespriction), "干扰所有被测件发出的报文,确保被测件连续%d次Busoff,然后等待DUT恢复通信", CanSMBorCounterL2Err);
            TestStep(StepDespriction);
            AppWait(5000, "");
            //InterfereCanMessage(32, 31);
             //GEEA3.0
            //DisturbFrame(THhandle, index, 32, 31, TimeOut_7011_Disturb);
            //GEEA3.5
            DisturbFrame(THhandle, index, 32, CanSMBorCounterL2Err, TimeOut_7011_Disturb);
            if (0 != CheckCommunication(DUTrecoveryTime, NetworkCluster[busFltDTCTemp[index].cluster])) {
                break;
            }

            //Step8
            TestStep("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].busOffDTC, 0);

            //Step9
            time = 5000;
            snprintf(StepDespriction, sizeof(StepDespriction), "清除DTC,等待%fms", time);
            TestStep(StepDespriction);
            SID14_ClearAllDTC();
            CheckPosResponse_SID14();
            AppWait(time, "");

            //Step10
            TestStep("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].busOffDTC, 0);

            //Step11
            snprintf(StepDespriction, sizeof(StepDespriction), "干扰所有被测件发出的报文,确保被测件连续%d次Busoff,然后等待DUT恢复通信", CanSMBorCounterL2Err + 1);
            TestStep(StepDespriction);
            //InterfereCanMessage(32, 32);
            //GEEA3.0
             //DisturbFrame(THhandle, index, 32, 32, TimeOut_7011_Disturb);
             //GEEA3.5
            DisturbFrame(THhandle, index, 32, CanSMBorCounterL2Err + 1, TimeOut_7011_Disturb);
            if (0 != CheckCommunication(DUTrecoveryTime, NetworkCluster[busFltDTCTemp[index].cluster])) {
                break;
            }

            //Step12
            TestStep("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            if (0 != RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].busOffDTC, 1)) {
                busFltDTCTemp[index].TestRslt = 0;
            }

            //Step13
            TestStep("等待5s");
            AppWait(5000, "wait for 5s");

            //Step14
            TestStep("使用掩码0x01读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x01);
            RX_CheckResponse_DTC_SID19_02(0x01, busFltDTCTemp[index].busOffDTC, 0);

            //Step15
            time = 5000;
            snprintf(StepDespriction, sizeof(StepDespriction), "清除DTC,等待%fms", time);
            TestStep(StepDespriction);
            SID14_ClearAllDTC();
            CheckPosResponse_SID14();
            AppWait(time, "");

            //Step16
            TestStep("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].busOffDTC, 0);

            //Step17
            snprintf(StepDespriction, sizeof(StepDespriction), "干扰所有被测件发出的报文,确保被测件连续%d次Busoff,然后等待DUT恢复通信", CanSMBorCounterL2Err + 1);
            TestStep(StepDespriction);
            //InterfereCanMessage(32, 32);
             //GEEA3.0
             //DisturbFrame(THhandle, index, 32, 32, TimeOut_7011_Disturb);
             //GEEA3.5
            DisturbFrame(THhandle, index, 32, CanSMBorCounterL2Err + 1, TimeOut_7011_Disturb);
            if (0 != CheckCommunication(DUTrecoveryTime, NetworkCluster[busFltDTCTemp[index].cluster])) {
                break;
            }

            //Step18
            TestStep("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            //RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].busOffDTC, 1);
            if (0 != RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].busOffDTC, 1)) {
                busFltDTCTemp[index].TestRslt = 0;
            }
        }
        DisturbDisconnect();
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

s32 TestCase_1022857_FDC10TestofBusoffDTC()
{
    strcpy(TestCaseName, "TestCase_1022857_FDC10TestofBusoffDTC");
    strcpy(TestScopeName, "Busoff DTC");
    strcpy(testPurpose_CH, "测试目的：");

    double time;
    int index = 0;
    u8 fdtc[3];
    int FDC10_1;
    int FDC10_2;
    int FDC12, FDC11;
    char distrbCount[10];
    char StepDespriction[256];
    s32 THhandle;
    s64 firstTime;
    stepnumber = 0;

    try
    {
        //Step1
        if (0 != TestPrecondition())
            return -1;
        //TestStep("上电并唤醒被测件");
        //if (0 != CheckCommunication(5000)) {
        //    TestPostCondition();
        //    return -1;
        //}

        FulfilTRC();
        TRCSwitch(0x0, 0xD, 0x0);

        THhandle = DisturbConnect();
        for (index = 0; index < BusNum; index++)
        {
            snprintf(StepDespriction, sizeof(StepDespriction), "-------------------------Test bus %s busoff:0x%X---------------------------", busFltDTCTemp[index].cluster, busFltDTCTemp[index].busOffDTC);
            TestReport_HTML_StepDescription(StepDespriction, Info);

            if (busFltDTCTemp[index].TestRslt != 1) {
                snprintf(StepDespriction, sizeof(StepDespriction), "DUT can not record %s BusOff DTC, break this case.", busFltDTCTemp[index].cluster);
                TestReport_HTML_StepDescription(StepDespriction, Warning);
                continue;
            }

            fdtc[0] = (busFltDTCTemp[index].busOffDTC & 0xff0000) >> 16;
            fdtc[1] = (busFltDTCTemp[index].busOffDTC & 0xff00) >> 8;
            fdtc[2] = (busFltDTCTemp[index].busOffDTC & 0xff);

            //Step2
            TestStep("等待7s");
            AppWait(7000, "wait for 7s");

            //Step3
            time = 5000;
            snprintf(StepDespriction, sizeof(StepDespriction), "清除DTC,等待%fms", time);
            TestStep(StepDespriction);
            SID14_ClearAllDTC();
            CheckPosResponse_SID14();
            AppWait(time, "");

            //Step4
            TestStep("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].busOffDTC, 0);

            //Step5
            TestStep("干扰所有被测件发出的报文,确保被测件连续10次Busoff,然后等待DUT恢复通信");
            //InterfereCanMessage(32, 10);
            DisturbFrame(THhandle, index, 32, 10, TimeOut_7011_Disturb);
            if (0 != CheckCommunication(DUTrecoveryTime, NetworkCluster[busFltDTCTemp[index].cluster])) {
                break;
            }

            //Step6
            TestStep("读Busoff DTC的FDC10,FDC11,FDC12,然后等待5s");
            FDC10_1 = ReadFDC10(fdtc);
            if (FDC10_1 == 0x28)
            {
                snprintf(issuereason, sizeof(issuereason), "FDC10 is 0x%X, expected is 0x28", FDC10_1);
                TestReport_HTML_StepDescription(issuereason, Pass);
            }
            else if (FDC10_1 == 0x80)
            {
                snprintf(issuereason, sizeof(issuereason), "FDC10 is 0x80. If the DUT decreases FDC10 to -128 immediately after recovering communication, it is normal.");
                TestReport_HTML_StepDescription(issuereason, Info);
            }
            else
            {
                snprintf(issuereason, sizeof(issuereason), "FDC10 is 0x%X, expected is 0x28", FDC10_1);
                TestReport_HTML_StepDescription(issuereason, Fail);
            }

            FDC11 = ReadFDC11(fdtc);
            if (FDC11 == 0x28)
            {
                snprintf(issuereason, sizeof(issuereason), "FDC11 is 0x%X, expected is 0x28", FDC11);
                TestReport_HTML_StepDescription(issuereason, Pass);
            }
            else
            {
                if (busFltDTCTemp[index].bsfUcfm > 0x28)
                {
                    if (FDC11 == 0)
                    {
                        snprintf(issuereason, sizeof(issuereason), "FDC11 is 0. Note: it is normal if UnconfirmedDTCLimit is greater than 40");
                        TestReport_HTML_StepDescription(issuereason, Pass);
                    }
                    else
                    {
                        snprintf(issuereason, sizeof(issuereason), "FDC11 is 0x%X, expected is 0x28. Please check whether FDC11 is supported", FDC11);
                        TestReport_HTML_StepDescription(issuereason, Info);
                    }
                }
                else
                {
                    snprintf(issuereason, sizeof(issuereason), "FDC11 is 0x%X, expected is 0x28. Please check whether FDC11 is supported", FDC11);
                    TestReport_HTML_StepDescription(issuereason, Info);
                }
            }

            FDC12 = ReadFDC12(fdtc);
            if (FDC12 == 0x28)
            {
                snprintf(issuereason, sizeof(issuereason), "FDC12 is 0x%X, expected is 0x28", FDC12);
                TestReport_HTML_StepDescription(issuereason, Pass);
            }
            else
            {
                if (busFltDTCTemp[index].bsfUcfm > 0x28)
                {
                    if (FDC12 == 0)
                    {
                        snprintf(issuereason, sizeof(issuereason), "FDC12 is 0. Note: it is normal if UnconfirmedDTCLimit is greater than 40");
                        TestReport_HTML_StepDescription(issuereason, Pass);
                    }
                    else
                    {
                        snprintf(issuereason, sizeof(issuereason), "FDC12 is 0x%X, expected is 0x28", FDC12);
                        TestReport_HTML_StepDescription(issuereason, Info);
                    }
                }
                else
                {
                    snprintf(issuereason, sizeof(issuereason), "FDC12 is 0x%X, expected is 0x28", FDC12);
                    TestReport_HTML_StepDescription(issuereason, Fail);
                }
            }

            TestReport_HTML_StepDescription("等待5s");
            AppWait(5000, "wait for 5s");

            //Step7
            TestStep("干扰所有被测件发出的报文,确保被测件连续11次Busoff,然后等待DUT恢复通信");
            //InterfereCanMessage(32, 11);
            DisturbFrame(THhandle, index, 32, 11, TimeOut_7011_Disturb);
            if (0 != CheckCommunication(DUTrecoveryTime, NetworkCluster[busFltDTCTemp[index].cluster])) {
                break;
            }

            //Step8
            TestStep("读Busoff DTC的FDC10,FDC11,FDC12");
            FDC10_2 = ReadFDC10(fdtc);
            if (FDC10_2 == 0x2C)
            {
                snprintf(issuereason, sizeof(issuereason), "FDC10 is 0x % X, expected is 0x2C", FDC10_2);
                TestReport_HTML_StepDescription(issuereason, Pass);
            }
            else if (FDC10_2 == 0x80)
            {
                snprintf(issuereason, sizeof(issuereason), "FDC10 is 0x80. If the DUT decreases FDC10 to -128 immediately after recovering communication, it is normal.");
                TestReport_HTML_StepDescription(issuereason, Info);
            }
            else
            {
                snprintf(issuereason, sizeof(issuereason), "FDC10 is 0x%X, expected is 0x2C", FDC10_2);
                TestReport_HTML_StepDescription(issuereason, Fail);
            }
            FDC11 = ReadFDC11(fdtc);
            if (FDC11 == 0x2C)
            {
                snprintf(issuereason, sizeof(issuereason), "FDC11 is 0x%X, expected is 0x2C", FDC11);
                TestReport_HTML_StepDescription(issuereason, Pass);
            }
            else
            {
                if (busFltDTCTemp[index].bsfUcfm > 0x28)
                {
                    if (FDC11 == 0)
                    {
                        snprintf(issuereason, sizeof(issuereason), "FDC11 is 0. Note: it is normal if UnconfirmedDTCLimit is greater than 40");
                        TestReport_HTML_StepDescription(issuereason, Pass);
                    }
                    else
                    {
                        snprintf(issuereason, sizeof(issuereason), "FDC11 is 0x%X, expected is 0x2C", FDC11);
                        TestReport_HTML_StepDescription(issuereason, Info);
                    }
                }
                else
                {
                    snprintf(issuereason, sizeof(issuereason), "FDC11 is 0x%X, expected is 0x2C", FDC11);
                    TestReport_HTML_StepDescription(issuereason, Fail);
                }

            }
            FDC12 = ReadFDC12(fdtc);
            if (FDC12 == 0x2C)
            {
                snprintf(issuereason, sizeof(issuereason), "FDC12 is 0x%X, expected is 0x2C", FDC12);
                TestReport_HTML_StepDescription(issuereason, Pass);
            }
            else
            {
                if (busFltDTCTemp[index].bsfUcfm > 0x2C)
                {
                    if (FDC12 == 0)
                    {
                        snprintf(issuereason, sizeof(issuereason), "FDC12 is 0. Note: it is normal if UnconfirmedDTCLimit is greater than 40");
                        TestReport_HTML_StepDescription(issuereason, Pass);
                    }
                    else
                    {
                        snprintf(issuereason, sizeof(issuereason), "FDC12 is 0x%X, expected is 0x2C", FDC12);
                        TestReport_HTML_StepDescription(issuereason, Info);
                    }
                }
                else
                {
                    snprintf(issuereason, sizeof(issuereason), "FDC12 is 0x%X, expected is 0x2C", FDC12);
                    TestReport_HTML_StepDescription(issuereason, Fail);
                }
            }

            //Step9
            TestStep("等待5s 读Busoff DTC的FDC10,FDC11,FDC12");
            AppWait(5000, "");
            FDC10_1 = ReadFDC10(fdtc);
            if (FDC10_1 == 0x80)
            {
                snprintf(issuereason, sizeof(issuereason), "FDC10 is 0x%X, expected is 0x2C", 0x80);
                TestReport_HTML_StepDescription(issuereason, Pass);
            }
            else
            {
                snprintf(issuereason, sizeof(issuereason), "FDC10 is 0x%x, not 0x80", FDC10_1);
                TestReport_HTML_StepDescription(issuereason, Fail);
            }
            FDC11 = ReadFDC11(fdtc);
            if (FDC11 == 0x2C)
            {
                snprintf(issuereason, sizeof(issuereason), "FDC11 is 0x%X, expected is 0x2C", FDC11);
                TestReport_HTML_StepDescription(issuereason, Pass);
            }
            else
            {
                if (busFltDTCTemp[index].bsfUcfm > 0x28)
                {
                    if (FDC11 == 0)
                    {
                        snprintf(issuereason, sizeof(issuereason), "FDC11 is 0. Note: it is normal if UnconfirmedDTCLimit is greater than 40");
                        TestReport_HTML_StepDescription(issuereason, Pass);
                    }
                    else
                    {
                        snprintf(issuereason, sizeof(issuereason), "FDC11 is 0x%X, expected is 0x2C", FDC11);
                        TestReport_HTML_StepDescription(issuereason, Info);
                    }
                }
                else
                {
                    snprintf(issuereason, sizeof(issuereason), "FDC11 is 0x%X, expected is 0x2C", FDC11);
                    TestReport_HTML_StepDescription(issuereason, Fail);
                }

            }
            FDC12 = ReadFDC12(fdtc);
            if (FDC12 == 0x2C)
            {
                snprintf(issuereason, sizeof(issuereason), "FDC12 is 0x%X, expected is 0x2C", FDC12);
                TestReport_HTML_StepDescription(issuereason, Pass);
            }
            else
            {
                if (busFltDTCTemp[index].bsfUcfm > 0x2C)
                {
                    if (FDC12 == 0)
                    {
                        snprintf(issuereason, sizeof(issuereason), "FDC12 is 0. Note: it is normal if UnconfirmedDTCLimit is greater than 40");
                        TestReport_HTML_StepDescription(issuereason, Pass);
                    }
                    else
                    {
                        snprintf(issuereason, sizeof(issuereason), "FDC12 is 0x%X, expected is 0x2C", FDC12);
                        TestReport_HTML_StepDescription(issuereason, Info);
                    }
                }
                else
                {
                    snprintf(issuereason, sizeof(issuereason), "FDC12 is 0x%X, expected is 0x2C", FDC12);
                    TestReport_HTML_StepDescription(issuereason, Fail);
                }
            }

            //Step10
            TestStep("干扰所有被测件发出的报文,确保被测件连续32次Busoff,然后等待DUT恢复通信");
            //InterfereCanMessage(32, 32);
            DisturbFrame(THhandle, index, 32, 32, TimeOut_7011_Disturb);
            if (0 != CheckCommunication(DUTrecoveryTime, NetworkCluster[busFltDTCTemp[index].cluster])) {
                break;
            }

            //Step11
            TestStep("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].busOffDTC, 1);

            //Step12
            TestStep("读Busoff DTC的FDC10,FDC11,FDC12");
            FDC10_2 = ReadFDC10(fdtc);
            if (FDC10_2 == 0x7f)
            {
                snprintf(issuereason, sizeof(issuereason), "FDC10 is 0x%x", FDC10_2);
                TestReport_HTML_StepDescription(issuereason, Pass);
            }
            else if (FDC10_2 == 0x80)
            {
                snprintf(issuereason, sizeof(issuereason), "FDC10 is 0x80. If the DUT decreases FDC10 to -128 immediately after recovering communication, it is normal.");
                TestReport_HTML_StepDescription(issuereason, Info);
            }
            else
            {
                snprintf(issuereason, sizeof(issuereason), "FDC10 is 0x%x, not 0x7f", FDC10_2);
                TestReport_HTML_StepDescription(issuereason, Fail);
            }
            FDC11 = ReadFDC11(fdtc);
            if (FDC11 == 0x7F)
            {
                snprintf(issuereason, sizeof(issuereason), "FDC11 is 0x%X, expected is 0x7F", FDC11);
                TestReport_HTML_StepDescription(issuereason, Pass);
            }
            else
            {
                snprintf(issuereason, sizeof(issuereason), "FDC11 is 0x%X, expected is 0x7F", FDC11);
                TestReport_HTML_StepDescription(issuereason, Fail);
            }
            FDC12 = ReadFDC12(fdtc);
            if (FDC12 == 0x7F)
            {
                snprintf(issuereason, sizeof(issuereason), "FDC12 is 0x%X, expected is 0x7F", FDC12);
                TestReport_HTML_StepDescription(issuereason, Pass);
            }
            else
            {
                snprintf(issuereason, sizeof(issuereason), "FDC12 is 0x%X, expected is 0x7F", FDC12);
                TestReport_HTML_StepDescription(issuereason, Fail);
            }
        }
        DisturbDisconnect();
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

s32 TestCase_1022858_UnconfirmedDTCLimitTestofBusoffDTC()
{
    strcpy(TestCaseName, "TestCase_1022858_UnconfirmedDTCLimit Test of Busoff DTC");
    strcpy(TestScopeName, "Busoff DTC");
    strcpy(testPurpose_CH, "测试目的：");

    double time;
    int index;
    u8 fdtc[3];
    int unCfmBit;
    int distrbCount;
    char distrbCountString[10];
    char StepDespriction[256];
    s32 THhandle;
    s64 firstTime;
    stepnumber = 0;

    try
    {
        //Step1
        if (0 != TestPrecondition())
            return -1;
        //TestStep("上电并唤醒被测件");
        //if (0 != CheckCommunication(5000)) {
        //    TestPostCondition();
        //    return -1;
        //}

        FulfilTRC();
        TRCSwitch(0x0, 0xD, 0x0);
        THhandle = DisturbConnect();
        for (index = 0; index < BusNum; index++)
        {
            snprintf(StepDespriction, sizeof(StepDespriction), "-------------------------Test bus %s busoff:0x%X---------------------------", busFltDTCTemp[index].cluster, busFltDTCTemp[index].busOffDTC);
            TestReport_HTML_StepDescription(StepDespriction, Info);

            if (busFltDTCTemp[index].TestRslt != 1) {
                snprintf(StepDespriction, sizeof(StepDespriction), "DUT can not record %s BusOff DTC, break this case.", busFltDTCTemp[index].cluster);
                TestReport_HTML_StepDescription(StepDespriction, Warning);
                continue;
            }

            fdtc[0] = (busFltDTCTemp[index].busOffDTC & 0xff0000) >> 16;
            fdtc[1] = (busFltDTCTemp[index].busOffDTC & 0xff00) >> 8;
            fdtc[2] = (busFltDTCTemp[index].busOffDTC & 0xff);

            //Step2
            TestStep("等待7s");
            AppWait(7000, "wait for 7s");

            //Step3
            time = 5000;
            snprintf(StepDespriction, sizeof(StepDespriction), "清除DTC,等待%fms", time);
            TestStep(StepDespriction);
            SID14_ClearAllDTC();
            CheckPosResponse_SID14();
            AppWait(time, "");

            //Step4
            TestStep("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].busOffDTC, 0);

            //Step5
            TestStep("通过快照记录编号(SnapshotRecordNo.)读取DTC的快照记录");
            unCfmBit = ReadSnapShot(fdtc);
            if (unCfmBit == 0)
            {
                strcpy(issuereason, "No snapshot record in the response.");
                TestReport_HTML_StepDescription(issuereason, Pass);
            }
            else
            {
                strcpy(issuereason, "Find snapshot record in the response.");
                TestReport_HTML_StepDescription(issuereason, Fail);
            }

            //Step6
            if (gAsrVersion < 4.1)
            {
                TestStep("读 UnconfirmedDTC, UnconfirmedDTCSinceLastClear 和 UnconfirmedDTCThisOperationCycle of test DTC");
                if (ReadUncofirmedDTC(fdtc) == 0)
                {
                    snprintf(StepDespriction, sizeof(StepDespriction), "UnconfirmedDTC is 0");
                    TestReport_HTML_StepDescription(StepDespriction, Pass);
                }
                else
                {
                    snprintf(StepDespriction, sizeof(StepDespriction), "UnconfirmedDTC is 1");
                    TestReport_HTML_StepDescription(StepDespriction, Fail);
                }
                if (ReadUncnfrmDTCSncLstClr(fdtc) == 0)
                {
                    snprintf(StepDespriction, sizeof(StepDespriction), "UnconfirmedDTCSinceLastClear is 0");
                    TestReport_HTML_StepDescription(StepDespriction, Pass);
                }
                else
                {
                    snprintf(StepDespriction, sizeof(StepDespriction), "UnconfirmedDTCSinceLastClear is 1");
                    TestReport_HTML_StepDescription(StepDespriction, Fail);
                }
                if (ReadUncnfrmDTCThisOpCyc(fdtc) == 0)
                {
                    snprintf(StepDespriction, sizeof(StepDespriction), "UnconfirmedDTCThisOperationCycle is 0");
                    TestReport_HTML_StepDescription(StepDespriction, Pass);
                }
                else
                {
                    snprintf(StepDespriction, sizeof(StepDespriction), "UnconfirmedDTCThisOperationCycle is 1");
                    TestReport_HTML_StepDescription(StepDespriction, Fail);
                }
            }

            //Step7
            TestStep("干扰所有被测件发出的报文,确保被测件连续1次Busoff,然后等待DUT恢复通信");
            //InterfereCanMessage(32, 1);
            DisturbFrame(THhandle, index, 32, 1, TimeOut_7011_Disturb);
            if (0 != CheckCommunication(DUTrecoveryTime, NetworkCluster[busFltDTCTemp[index].cluster])) {
                break;
            }

            //Step8
            if (gAsrVersion < 4.1)
            {
                TestStep("读 UnconfirmedDTC, UnconfirmedDTCSinceLastClear 和 UnconfirmedDTCThisOperationCycle of test DTC");
                if (ReadUncofirmedDTC(fdtc) == 1)
                {
                    snprintf(StepDespriction, sizeof(StepDespriction), "UnconfirmedDTC is 1");
                    TestReport_HTML_StepDescription(StepDespriction, Pass);
                }
                else
                {
                    snprintf(StepDespriction, sizeof(StepDespriction), "UnconfirmedDTC is 0");
                    TestReport_HTML_StepDescription(StepDespriction, Fail);
                }
                if (ReadUncnfrmDTCSncLstClr(fdtc) == 1)
                {
                    snprintf(StepDespriction, sizeof(StepDespriction), "UnconfirmedDTCSinceLastClear is 1");
                    TestReport_HTML_StepDescription(StepDespriction, Pass);
                }
                else
                {
                    snprintf(StepDespriction, sizeof(StepDespriction), "UnconfirmedDTCSinceLastClear is 0");
                    TestReport_HTML_StepDescription(StepDespriction, Fail);
                }
                if (ReadUncnfrmDTCThisOpCyc(fdtc) == 1)
                {
                    snprintf(StepDespriction, sizeof(StepDespriction), "UnconfirmedDTCThisOperationCycle is 1");
                    TestReport_HTML_StepDescription(StepDespriction, Pass);
                }
                else
                {
                    snprintf(StepDespriction, sizeof(StepDespriction), "UnconfirmedDTCThisOperationCycle is 0");
                    TestReport_HTML_StepDescription(StepDespriction, Fail);
                }
            }

            //Step9
            TestStep("通过快照记录编号(SnapshotRecordNo.)读取DTC的快照记录");
            unCfmBit = ReadSnapShot(fdtc);
            if (unCfmBit == 1)
            {
                strcpy(issuereason, "Find snapshot record in the response.");
                TestReport_HTML_StepDescription(issuereason, Pass);
            }
            else
            {
                strcpy(issuereason, "No snapshot record in the response.");
                TestReport_HTML_StepDescription(issuereason, Fail);
            }

            //Step10
            if (gAsrVersion < 4.1)
            {
                TestStep("等待5s,读 UnconfirmedDTC 和 UnconfirmedDTCThisOperationCycle of test DTC");
                AppWait(5000, "");
                if (ReadUncofirmedDTC(fdtc) == 0)
                {
                    snprintf(StepDespriction, sizeof(StepDespriction), "UnconfirmedDTC is 0");
                    TestReport_HTML_StepDescription(StepDespriction, Pass);
                }
                else
                {
                    snprintf(StepDespriction, sizeof(StepDespriction), "UnconfirmedDTC is 1");
                    TestReport_HTML_StepDescription(StepDespriction, Fail);
                }
                if (ReadUncnfrmDTCThisOpCyc(fdtc) == 1)
                {
                    snprintf(StepDespriction, sizeof(StepDespriction), "UnconfirmedDTCThisOperationCycle is 1");
                    TestReport_HTML_StepDescription(StepDespriction, Pass);
                }
                else
                {
                    snprintf(StepDespriction, sizeof(StepDespriction), "UnconfirmedDTCThisOperationCycle is 0");
                    TestReport_HTML_StepDescription(StepDespriction, Fail);
                }
            }

            //Step11
            TestStep("让被测件进入下一个operation cycle");
            NM_NextOperation();

            //Step12
            if (gAsrVersion < 4.1)
            {
                TestStep("等待10s,读 UnconfirmedDTCSinceLastClear 和 UnconfirmedDTCThisOperationCycle of test DTC");
                AppWait(10000, "");
                if (ReadUncnfrmDTCSncLstClr(fdtc) == 1)
                {
                    snprintf(StepDespriction, sizeof(StepDespriction), "UnconfirmedDTCSinceLastClear is 1");
                    TestReport_HTML_StepDescription(StepDespriction, Pass);
                }
                else
                {
                    snprintf(StepDespriction, sizeof(StepDespriction), "UnconfirmedDTCSinceLastClear is 0");
                    TestReport_HTML_StepDescription(StepDespriction, Fail);
                }
                if (ReadUncnfrmDTCThisOpCyc(fdtc) == 0)
                {
                    snprintf(StepDespriction, sizeof(StepDespriction), "UnconfirmedDTCThisOperationCycle is 0");
                    TestReport_HTML_StepDescription(StepDespriction, Pass);
                }
                else
                {
                    snprintf(StepDespriction, sizeof(StepDespriction), "UnconfirmedDTCThisOperationCycle is 1");
                    TestReport_HTML_StepDescription(StepDespriction, Fail);
                }
            }

            //Step13
            TestStep("干扰所有被测件发出的报文,确保被测件连续32次Busoff,然后等待DUT恢复通信");
            //InterfereCanMessage(32, 32);
            DisturbFrame(THhandle, index, 32, 32, TimeOut_7011_Disturb);
            if (0 != CheckCommunication(DUTrecoveryTime, NetworkCluster[busFltDTCTemp[index].cluster])) {
                break;
            }
            //Step14
            TestStep("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].busOffDTC, 1);
        }
        DisturbDisconnect();
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

s32 TestCase_1022859_BusoffDTCaged()
{
    strcpy(TestCaseName, "TestCase_1022859_Busoff DTC aged");
    strcpy(TestScopeName, "Busoff DTC");
    strcpy(testPurpose_CH, "测试目的：");

    int index, j;
    double time;
    u8 fdtc[3];
    int unCfmBit, occ2;
    int distrbCount, ageDTC;
    char distrbCountString[10];
    char StepDespriction[256];
    s32 THhandle;
    s64 firstTime;
    stepnumber = 0;

    try
    {
        //Step1
        if (0 != TestPrecondition())
            return -1;
        //TestStep("上电并唤醒被测件");
        //if (0 != CheckCommunication(5000)) {
        //    TestPostCondition();
        //    return -1;
        //}

        FulfilTRC();
        TRCSwitch(0x0, 0xD, 0x0);
        THhandle = DisturbConnect();
        for (index = 0; index < BusNum; index++)
        {
            snprintf(StepDespriction, sizeof(StepDespriction), "-------------------------Test bus %s busoff:0x%X---------------------------", busFltDTCTemp[index].cluster, busFltDTCTemp[index].busOffDTC);
            TestReport_HTML_StepDescription(StepDespriction, Info);

            if (busFltDTCTemp[index].TestRslt != 1) {
                snprintf(StepDespriction, sizeof(StepDespriction), "DUT can not record %s BusOff DTC, break this case.", busFltDTCTemp[index].cluster);
                TestReport_HTML_StepDescription(StepDespriction, Warning);
                continue;
            }

            fdtc[0] = (busFltDTCTemp[index].busOffDTC & 0xff0000) >> 16;
            fdtc[1] = (busFltDTCTemp[index].busOffDTC & 0xff00) >> 8;
            fdtc[2] = (busFltDTCTemp[index].busOffDTC & 0xff);
            //Step2
            TestStep("等待7s");
            AppWait(7000, "wait for 7s");

            //Step3
            time = 5000;
            snprintf(StepDespriction, sizeof(StepDespriction), "清除DTC,等待%fms", time);
            TestStep(StepDespriction);
            SID14_ClearAllDTC();
            CheckPosResponse_SID14();
            AppWait(time, "");

            //Step4
            TestStep("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].busOffDTC, 0);

            //Step5
            TestStep("干扰所有被测件发出的报文,确保被测件连续32次Busoff,然后等待DUT恢复通信");
            //InterfereCanMessage(32, 32);

            DisturbFrame(THhandle, index, 32, 32, TimeOut_7011_Disturb);
            if (0 != CheckCommunication(DUTrecoveryTime, NetworkCluster[busFltDTCTemp[index].cluster])) {
                break;
            }
            //Step6
            TestStep("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].busOffDTC, 1);

            //Step7
            if (gAsrVersion < 4.1)
            {
                TestStep("Read testFailedSincelLastClearOrAged of the busoff DTC");
                if (ReadTestFailSncLstClrOrAged(fdtc) == 0)
                {
                    strcpy(issuereason, "testFailedSincelLastClearOrAged is 0");
                    TestReport_HTML_StepDescription(issuereason, Fail);
                }
                else
                {
                    strcpy(issuereason, "testFailedSincelLastClearOrAged is 1");
                    TestReport_HTML_StepDescription(issuereason, Pass);
                }
            }

            //Step8
            TestStep("等待5s");
            AppWait(5000, "wait for 5s");

            //Step11
            for (j = 0; j < 260; j++)
            {
                stepnumber = 8;
                snprintf(StepDespriction, sizeof(StepDespriction), "-------------------- - The % dth repeat of step9 to step10--------------------", j + 1);
                TestReport_HTML_StepDescription(StepDespriction, Info);

                //Step9
                TestStep("让被测件进入下一个operation cycle");
                NM_NextOperation();

                //Step10
                TestStep("等待10s,使用掩码0x09读取DTC");
                AppWait(10000, "wait for 10s");
                RX_SID19_ReportDTCByStatusMask(0x09);
                RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].busOffDTC, 1);

                strcpy(issuereason, "Read OCC2 or OCC5 of test DTC");
                TestReport_HTML_StepDescription(issuereason, Info);
                occ2 = ReadAgingCounter(fdtc, gOcc2Or5);
                if (occ2 == j)
                {
                    snprintf(StepDespriction, sizeof(StepDespriction), "OCC%d is %d", gOcc2Or5, occ2);
                    TestReport_HTML_StepDescription(StepDespriction, Pass);
                }
                else
                {
                    snprintf(StepDespriction, sizeof(StepDespriction), "OCC%d is %d, expected is %d", gOcc2Or5, occ2, j);
                    TestReport_HTML_StepDescription(StepDespriction, Fail);
                }

                if (occ2 >= 254)
                {
                    break;
                }
                if ((occ2 == 0) && (j == 10))
                {
                    break;
                }
            }

            //Step12
            TestStep("让被测件进入下一个operation cycle");
            NM_NextOperation();

            //Step13
            TestStep("等待10s,使用掩码0x09读取DTC");
            AppWait(10000, "wait for 10s");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].busOffDTC, 0);

            //Step14
            if (gAsrVersion < 4.1)
            {
                TestStep("读 AgedDTC");
                ageDTC = ReadAgedDTC(fdtc);
                if (ageDTC == 1 || ageDTC == -1)
                {
                    strcpy(issuereason, "The AgedDTC is 1 or not applicable");
                    TestReport_HTML_StepDescription(issuereason, Pass);
                }
                else
                {
                    strcpy(issuereason, "The AgedDTC is 0, expected is 1.");
                    TestReport_HTML_StepDescription(issuereason, Fail);
                }

                //Step15
                TestStep("Read testFailedSincelLastClearOrAged of the busoff DTC");
                if (ReadTestFailSncLstClrOrAged(fdtc) == 0)
                {
                    strcpy(issuereason, "testFailedSincelLastClearOrAged is 0");
                    TestReport_HTML_StepDescription(issuereason, Pass);
                }
                else
                {
                    strcpy(issuereason, "testFailedSincelLastClearOrAged is 1");
                    TestReport_HTML_StepDescription(issuereason, Fail);
                }
            }

            //Step16
            TestStep("干扰所有被测件发出的报文,确保被测件连续32次Busoff,然后等待DUT恢复通信");
            //InterfereCanMessage(32, 32);
            DisturbFrame(THhandle, index, 32, 32, TimeOut_7011_Disturb);
            if (0 != CheckCommunication(DUTrecoveryTime, NetworkCluster[busFltDTCTemp[index].cluster])) {
                break;
            }

            //Step17
            TestStep("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].busOffDTC, 1);

            //Step18
            if (gAsrVersion < 4.1)
            {
                TestStep("读 AgedDTC");
                ageDTC = ReadAgedDTC(fdtc);
                if (ageDTC == 1)
                {
                    strcpy(issuereason, "The AgedDTC is 1, expected is 0.");
                    TestReport_HTML_StepDescription(issuereason, Fail);
                }
                else
                {
                    strcpy(issuereason, "The AgedDTC is 0 or not applicable");
                    TestReport_HTML_StepDescription(issuereason, Pass);
                }
            }
        }
        DisturbDisconnect();
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

s32 TestCase_1022860_BusoffDTCTestwhenElPowerLevelis1()
{
    strcpy(TestCaseName, "TestCase_1022860_Busoff DTC Test when ElPowerLevel is 1");
    strcpy(TestScopeName, "Busoff DTC");
    strcpy(testPurpose_CH, "测试目的：");

    int index;
    double time;
    int distrbCount;
    char distrbCountString[10];
    char StepDespriction[256];
    s32 THhandle = -1;
    s64 firstTime = 0;
    stepnumber = 0;

    try
    {
        //Step1
        if (0 != TestPrecondition())
            return -1;
        //TestStep("上电并唤醒被测件");
        //if (0 != CheckCommunication(5000)) {
        //    TestPostCondition();
        //    return -1;
        //}

        FulfilTRC();
        TRCSwitch(0x0, 0xD, 0x0);
        THhandle = DisturbConnect();
        for (index = 0; index < BusNum; index++)
        {
            snprintf(StepDespriction, sizeof(StepDespriction), "-------------------------Test bus %s busoff:0x%X---------------------------", busFltDTCTemp[index].cluster, busFltDTCTemp[index].busOffDTC);
            TestReport_HTML_StepDescription(StepDespriction, Info);

            if (busFltDTCTemp[index].TestRslt != 1) {
                snprintf(StepDespriction, sizeof(StepDespriction), "DUT can not record %s BusOff DTC, break this case.", busFltDTCTemp[index].cluster);
                TestReport_HTML_StepDescription(StepDespriction, Warning);
                continue;
            }

            //Step2
            TestStep("等待7s");
            AppWait(7000, "wait for 7s");

            //Step3
            time = 5000;
            snprintf(StepDespriction, sizeof(StepDespriction), "清除DTC,等待%fms", time);
            TestStep(StepDespriction);
            SID14_ClearAllDTC();
            CheckPosResponse_SID14();
            AppWait(time, "");

            //Step4
            TestStep("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].busOffDTC, 0);

            //Step5
            TestStep("设置 ElPowerLevel 为 1");
            UsgModSwtich(0xD);
            EPLvlSwitch(0x1);

            //Step6
            AppWait(7000, "wait for 7s");

            //Step7
            TestStep("干扰所有被测件发出的报文,确保被测件连续32次Busoff,然后等待DUT恢复通信");
            //InterfereCanMessage(32, 32);

            DisturbFrame(THhandle, index, 32, 32, TimeOut_7011_Disturb);
            if (0 != CheckCommunication(DUTrecoveryTime, NetworkCluster[busFltDTCTemp[index].cluster])) {
                break;
            }
            //Step8
            TestStep("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].busOffDTC, 0);

            //Step9
            TestStep("设置 ElPowerLevel 为 0");
            UsgModSwtich(0xD);
            EPLvlSwitch(0x0);

            //Step10
            TestStep("等待7s");
            AppWait(7000, "wait for 7s");

            //Step11
            TestStep("干扰所有被测件发出的报文,确保被测件连续32次Busoff,然后等待DUT恢复通信");
            //InterfereCanMessage(32, 32);
            DisturbFrame(THhandle, index, 32, 32, TimeOut_7011_Disturb);
            if (0 != CheckCommunication(DUTrecoveryTime, NetworkCluster[busFltDTCTemp[index].cluster])) {
                break;
            }
            //Step12
            TestStep("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].busOffDTC, 1);
        }
        DisturbDisconnect();
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

s32 TestCase_1022861_BusoffDTCTestwhenthevoltageofpowersupplyisoutofrange()
{
    strcpy(TestCaseName, "TestCase_1022861_Busoff DTC Test when the voltage of power supply is out of range");
    strcpy(TestScopeName, "Busoff DTC");
    strcpy(testPurpose_CH, "测试目的：");

    int index;
    float volt;
    double time;
    int distrbCount;
    char distrbCountString[10];
    char StepDespriction[256];
    s32 THhandle;
    s64 firstTime;
    stepnumber = 0;

    try
    {
        //Step1
        if (0 != TestPrecondition())
            return -1;
        //TestStep("上电并唤醒被测件");
        //if (0 != CheckCommunication(5000)) {
        //    TestPostCondition();
        //    return -1;
        //}

        FulfilTRC();
        TRCSwitch(0x0, 0xD, 0x0);
        THhandle = DisturbConnect();
        for (index = 0; index < BusNum; index++)
        {
            snprintf(StepDespriction, sizeof(StepDespriction), "-------------------------Test bus %s busoff:0x%X---------------------------", busFltDTCTemp[index].cluster, busFltDTCTemp[index].busOffDTC);
            TestReport_HTML_StepDescription(StepDespriction, Info);

            if (busFltDTCTemp[index].TestRslt != 1) {
                snprintf(StepDespriction, sizeof(StepDespriction), "DUT can not record %s BusOff DTC, break this case.", busFltDTCTemp[index].cluster);
                TestReport_HTML_StepDescription(StepDespriction, Warning);
                continue;
            }

            //Step2
            TestStep("等待7s");
            AppWait(7000, "wait for 7s");

            //Step3
            time = 5000;
            snprintf(StepDespriction, sizeof(StepDespriction), "清除DTC,等待%fms", time);
            TestStep(StepDespriction);
            SID14_ClearAllDTC();
            CheckPosResponse_SID14();
            AppWait(time, "");

            //Step4
            TestStep("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].busOffDTC, 0);

            //Step5
            volt = gVoltLow;
            snprintf(StepDespriction, sizeof(StepDespriction), "设置电压为 %5.2fV", volt);
            TestStep(StepDespriction);
            SetVoltageAndRead(volt);

            //Step6
            TestStep("等待7s");
            AppWait(7000, "wait for 7s");

            //Step7
            TestStep("干扰所有被测件发出的报文,确保被测件连续32次Busoff,然后等待DUT恢复通信");
            //InterfereCanMessage(32, 32);
            DisturbFrame(THhandle, index, 32, 32, TimeOut_7011_Disturb);
            if (0 != CheckCommunication(DUTrecoveryTime, NetworkCluster[busFltDTCTemp[index].cluster])) {
                break;
            }

            //Step8
            TestStep("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].busOffDTC, 0);

            //Step9
            volt = gVoltHigh;
            snprintf(StepDespriction, sizeof(StepDespriction), "设置电压为 %5.2fV", volt);
            TestStep(StepDespriction);
            SetVoltageAndRead(volt);

            //Step10
            TestStep("等待7s");
            AppWait(7000, "wait for 7s");

            //Step11
            TestStep("干扰所有被测件发出的报文,确保被测件连续32次Busoff,然后等待DUT恢复通信");
            //InterfereCanMessage(32, 32);
            DisturbFrame(THhandle, index, 32, 32, TimeOut_7011_Disturb);
            if (0 != CheckCommunication(DUTrecoveryTime, NetworkCluster[busFltDTCTemp[index].cluster])) {
                break;
            }


            //Step12
            TestStep("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].busOffDTC, 0);

            //Step13
            volt = gVoltNorm;
            snprintf(StepDespriction, sizeof(StepDespriction), "设置电压为 %5.2fV", volt);
            TestStep(StepDespriction);
            SetVoltageAndRead(volt);

            //Step14
            TestStep("等待7s");
            AppWait(7000, "wait for 7s");

            //Step15
            TestStep("干扰所有被测件发出的报文,确保被测件连续32次Busoff,然后等待DUT恢复通信");
            //InterfereCanMessage(32, 32);
            DisturbFrame(THhandle, index, 32, 32, TimeOut_7011_Disturb);
            if (0 != CheckCommunication(DUTrecoveryTime, NetworkCluster[busFltDTCTemp[index].cluster])) {
                break;
            }

            //Step16
            TestStep("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].busOffDTC, 1);
        }
        DisturbDisconnect();
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

s32 TestCase_1022862_BusoffDTCInLongTermMemoryTest()
{
    strcpy(TestCaseName, "TestCase_1022862_Busoff DTC In Long Term Memory Test");
    strcpy(TestScopeName, "Busoff DTC");
    strcpy(testPurpose_CH, "测试目的：");

    int index, j;
    float volt;
    double time;
    int distrbCount;
    char distrbCountString[10];
    char StepDespriction[256];
    s32 THhandle;
    s64 firstTime;
    stepnumber = 0;

    try
    {
        //Step1
        if (0 != TestPrecondition())
            return -1;
        //TestStep("上电并唤醒被测件");
        //if (0 != CheckCommunication(5000)) {
        //    TestPostCondition();
        //    return -1;
        //}

        FulfilTRC();
        TRCSwitch(0x0, 0xD, 0x0);
        THhandle = DisturbConnect();
        for (index = 0; index < BusNum; index++)
        {
            snprintf(StepDespriction, sizeof(StepDespriction), "-------------------------Test bus %s busoff:0x%X---------------------------", busFltDTCTemp[index].cluster, busFltDTCTemp[index].busOffDTC);
            TestReport_HTML_StepDescription(StepDespriction, Info);

            if (busFltDTCTemp[index].TestRslt != 1) {
                snprintf(StepDespriction, sizeof(StepDespriction), "DUT can not record %s BusOff DTC, break this case.", busFltDTCTemp[index].cluster);
                TestReport_HTML_StepDescription(StepDespriction, Warning);
                continue;
            }

            //Step2
            TestStep("等待7s");
            AppWait(7000, "wait for 7s");

            //Step3
            time = 5000;
            snprintf(StepDespriction, sizeof(StepDespriction), "清除DTC,等待%fms", time);
            TestStep(StepDespriction);
            SID14_ClearAllDTC();
            CheckPosResponse_SID14();
            AppWait(time, "");

            //Step4
            TestStep("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].busOffDTC, 0);

            //Step5
            TestStep("干扰所有被测件发出的报文,确保被测件连续32次Busoff,然后等待DUT恢复通信");
            //InterfereCanMessage(32, 32);
            DisturbFrame(THhandle, index, 32, 32, TimeOut_7011_Disturb);
            if (0 != CheckCommunication(DUTrecoveryTime, NetworkCluster[busFltDTCTemp[index].cluster])) {
                break;
            }

            //Step6
            TestStep("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].busOffDTC, 1);

            //Step7
            TestStep("满足被测设备将DTC信息存储到长期存储器中的条件。例如，让被测设备进入深度睡眠状态");
            UsgModSwtich(0x1);
            EPLvlSwitch(0x0);
            AppWait(1000, "");
            SendTesterPresent(TP_OFF);
            NMWakeUp(0);
            com.del_cyclic_messages();
            KL15(off);
            AppWait(2000, "");
            TestWaitForCurInRange(gLowPwrCur, gSleepTime);

            //Step8
            TestStep("关闭电源，然后等待 10 秒钟");
            KL30(off);
            AppWait(10000, "wait for 10s");

            //Step9
            TestStep("上电并唤醒被测件");
            KL30(on);
            KL15(on);
            NMWakeUp(1);
            if (0 != CheckCommunication(5000)) {
                break;
            }
            SendTesterPresent(TP_ON);
            FulfilTRC();
            AppWait(1000, "");
            TRCSwitch(0x0, 0xD, 0x0);

            //Step10
            TestStep("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].busOffDTC, 1);

            //Step11
            UsgModSwtich(0xB);
            EPLvlSwitch(0x0);
            TestStep("干扰所有被测件发出的报文,确保被测件连续32次Busoff,然后等待DUT恢复通信");
            //InterfereCanMessage(32, 32);
            DisturbFrame(THhandle, index, 32, 32, TimeOut_7011_Disturb);
            if (0 != CheckCommunication(DUTrecoveryTime, NetworkCluster[busFltDTCTemp[index].cluster])) {
                break;
            }

            //Step12
            TestStep("发送诊断请求重启ECU");
            SID11_ECUReset(0x01);
            CheckPosResponse_SID11(0x01);
            AppWait(1000, "");
            FulfilTRC();
            AppWait(1000, "");

            //Step13
            TestStep("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].busOffDTC, 1);

            //Step14
            TestStep("干扰所有被测件发出的报文,确保被测件连续32次Busoff,然后等待DUT恢复通信");
            //InterfereCanMessage(32, 32);
            DisturbFrame(THhandle, index, 32, 32, TimeOut_7011_Disturb);
            if (0 != CheckCommunication(DUTrecoveryTime, NetworkCluster[busFltDTCTemp[index].cluster])) {
                break;
            }

            //Step15
            volt = 3;
            TestStep("设置电压为3V,等待5s,设置电压为12V");
            SetVoltageAndRead(volt);
            AppWait(5000, "wait for 5s");
            volt = 12;
            SetVoltageAndRead(volt);

            //Step16
            TestStep("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].busOffDTC, 1);

            //Step17
            TestStep("干扰所有被测件发出的报文,确保被测件连续32次Busoff,然后等待DUT恢复通信");
            //InterfereCanMessage(32, 32);
            DisturbFrame(THhandle, index, 32, 32, TimeOut_7011_Disturb);
            if (0 != CheckCommunication(DUTrecoveryTime, NetworkCluster[busFltDTCTemp[index].cluster])) {
                break;
            }

            //Step18
            TestStep("重新上下电");
            KL30(off);
            AppWait(2000, "wait for 2s");
            KL30(on);
            NMWakeUp(1);
            SendTesterPresent(TP_ON);
            FulfilTRC();
            AppWait(1000, "");
            TRCSwitch(0x0, 0xD, 0x0);

            //Step19
            TestStep("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].busOffDTC, 1);
        }
        DisturbDisconnect();
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

s32 TestCase_1022863_BusoffDTCTestwhenCarModeisnotNormal()
{
    strcpy(TestCaseName, "TestCase_1022863_Busoff DTC Test when Car Mode is not Normal");
    strcpy(TestScopeName, "Busoff DTC");
    strcpy(testPurpose_CH, "测试目的：");

    int index, j;
    float volt;
    double time;
    u8 carmode[3];
    int distrbCount;
    char distrbCountString[10];
    char StepDespriction[256];
    s32 THhandle;
    s64 firstTime;
    stepnumber = 0;

    carmode[0] = gCMFctyValue;
    carmode[1] = gCMTsptValue;
    carmode[2] = gCMCrshValue;

    try
    {
        //Step1
        if (0 != TestPrecondition())
            return -1;
        //TestStep("上电并唤醒被测件");
        //if (0 != CheckCommunication(5000)) {
        //    TestPostCondition();
        //    return -1;
        //}
        FulfilTRC();
        TRCSwitch(0x0, 0xB, 0x0);
        THhandle = DisturbConnect();
        for (index = 0; index < BusNum; index++)
        {
            snprintf(StepDespriction, sizeof(StepDespriction), "-------------------------Test bus %s busoff:0x%X---------------------------", busFltDTCTemp[index].cluster, busFltDTCTemp[index].busOffDTC);
            TestReport_HTML_StepDescription(StepDespriction, Info);

            if (busFltDTCTemp[index].TestRslt != 1) {
                snprintf(StepDespriction, sizeof(StepDespriction), "DUT can not record %s BusOff DTC, break this case.", busFltDTCTemp[index].cluster);
                TestReport_HTML_StepDescription(StepDespriction, Warning);
                continue;
            }

            //Step2
            TestStep("等待7s");
            AppWait(7000, "wait for 7s");

            for (j = 0; j < 3; j++)
            {
                stepnumber = 3;

                //Step3
                time = 5000;
                snprintf(StepDespriction, sizeof(StepDespriction), "清除DTC,等待%fms", time);
                TestStep(StepDespriction);
                SID14_ClearAllDTC();
                CheckPosResponse_SID14();
                AppWait(time, "");

                //Step4
                TestStep("使用掩码0x09读取DTC");
                RX_SID19_ReportDTCByStatusMask(0x09);
                RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].busOffDTC, 0);

                //Step5
                snprintf(StepDespriction, sizeof(StepDespriction), "设置 car mode 为 0x%x.(Transport:0x1;Factory:0x2;Crash:0x3)", carmode[j]);
                TestStep(StepDespriction);
                CarModSwtich(carmode[j]);

                //Step6
                TestStep("等待7s");
                AppWait(7000, "wait for 7s");

                //Step7
                TestStep("干扰所有被测件发出的报文,确保被测件连续32次Busoff,然后等待DUT恢复通信");
                //InterfereCanMessage(32, 32);
                DisturbFrame(THhandle, index, 32, 32, TimeOut_7011_Disturb);
                if (0 != CheckCommunication(DUTrecoveryTime, NetworkCluster[busFltDTCTemp[index].cluster])) {
                    break;
                }

                //Step8
                TestStep("使用掩码0x09读取DTC");
                RX_SID19_ReportDTCByStatusMask(0x09);
                RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].busOffDTC, 0);
            }

            //Step10
            TestStep("设置Car mode 为 normal");
            CarModSwtich(0x0);

            //Step11
            TestStep("等待7s");
            AppWait(7000, "wait for 7s");

            //Step12
            TestStep("干扰所有被测件发出的报文,确保被测件连续32次Busoff,然后等待DUT恢复通信");
            //InterfereCanMessage(32, 32);
            DisturbFrame(THhandle, index, 32, 32, TimeOut_7011_Disturb);
            if (0 != CheckCommunication(DUTrecoveryTime, NetworkCluster[busFltDTCTemp[index].cluster])) {
                break;
            }

            //Step13
            TestStep("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].busOffDTC, 1);
        }
        DisturbDisconnect();
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

s32 TestCase_1022864_BusoffDTCTestafterCarModetransition()
{
    strcpy(TestCaseName, "TestCase_1022864_Busoff DTC Test after Car Mode transition");
    strcpy(TestScopeName, "Busoff DTC");
    strcpy(testPurpose_CH, "测试目的：");

    int index, j;
    float volt;
    double time;
    u8 carmode[3];
    u8 dtc[3] = { 0xff,0xff,0xff };
    int distrbCount;
    char distrbCountString[10];
    char StepDespriction[256];
    s32 THhandle;
    s64 firstTime;
    stepnumber = 0;

    try
    {
        //Step1
        if (0 != TestPrecondition())
            return -1;
        //TestStep("上电并唤醒被测件");
        //if (0 != CheckCommunication(5000)) {
        //    TestPostCondition();
        //    return -1;
        //}
        FulfilTRC();
        TRCSwitch(0x0, 0xB, 0x0);
        THhandle = DisturbConnect();
        for (index = 0; index < BusNum; index++)
        {
            snprintf(StepDespriction, sizeof(StepDespriction), "-------------------------Test bus %s busoff:0x%X---------------------------", busFltDTCTemp[index].cluster, busFltDTCTemp[index].busOffDTC);
            TestReport_HTML_StepDescription(StepDespriction, Info);

            if (busFltDTCTemp[index].TestRslt != 1) {
                snprintf(StepDespriction, sizeof(StepDespriction), "DUT can not record %s BusOff DTC, break this case.", busFltDTCTemp[index].cluster);
                TestReport_HTML_StepDescription(StepDespriction, Warning);
                continue;
            }

            //Step2
            TestStep("等待7s");
            AppWait(7000, "wait for 7s");

            //Step3
            time = 5000;
            snprintf(StepDespriction, sizeof(StepDespriction), "清除DTC,等待%fms", time);
            TestStep(StepDespriction);
            SID14_ClearAllDTC();
            CheckPosResponse_SID14();
            AppWait(time, "");

            //Step4
            TestStep("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].busOffDTC, 0);

            //Step5
            TestStep("设置Car mode 为 Factory");
            CarModSwtich(0x2);

            //Step6
            TestStep("等待7s");
            AppWait(7000, "wait for 7s");

            //Step7
            TestStep("设置Car mode 为 Normal");
            CarModSwtich(0x0);

            //Step8
            TestStep("等待1s");
            AppWait(1000, "wait for 1s");

            //Step9
            TestStep("干扰所有被测件发出的报文,确保被测件连续32次Busoff,然后等待DUT恢复通信");
            //InterfereCanMessage(32, 32);
            DisturbFrame(THhandle, index, 32, 32, TimeOut_7011_Disturb);
            if (0 != CheckCommunication(DUTrecoveryTime, NetworkCluster[busFltDTCTemp[index].cluster])) {
                break;
            }

            //Step10
            TestStep("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].busOffDTC, 1);
        }
        DisturbDisconnect();
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

s32 TestCase_1022865_BusoffDTCTestInDifferentUsageMode()
{
    strcpy(TestCaseName, "TestCase_1022865_Busoff DTC Test In Different UsageMode");
    strcpy(TestScopeName, "Busoff DTC");
    strcpy(testPurpose_CH, "测试目的：");

    int index, j;
    double time;
    u8 usagMode[5];
    int distrbCount;
    u8 dtc[3] = { 0xff,0xff,0xff };
    char distrbCountString[10];
    char StepDespriction[256];
    s32 THhandle;
    s64 firstTime;
    stepnumber = 0;

    usagMode[0] = gUMAbanValue;
    usagMode[1] = gUMInActValue;
    usagMode[2] = gUMConValue;
    usagMode[3] = gUMActValue;
    usagMode[4] = gUMDrivValue;

    try
    {
        //Step1
        if (0 != TestPrecondition())
            return -1;
        //TestStep("上电并唤醒被测件");
        //if (0 != CheckCommunication(5000)) {
        //    TestPostCondition();
        //    return -1;
        //}
        FulfilTRC();
        TRCSwitch(0x0, 0xB, 0x0);
        THhandle = DisturbConnect();
        for (index = 0; index < BusNum; index++)
        {
            snprintf(StepDespriction, sizeof(StepDespriction), "-------------------------Test bus %s busoff:0x%X---------------------------", busFltDTCTemp[index].cluster, busFltDTCTemp[index].busOffDTC);
            TestReport_HTML_StepDescription(StepDespriction, Info);

            if (busFltDTCTemp[index].TestRslt != 1) {
                snprintf(StepDespriction, sizeof(StepDespriction), "DUT can not record %s BusOff DTC, break this case.", busFltDTCTemp[index].cluster);
                TestReport_HTML_StepDescription(StepDespriction, Warning);
                continue;
            }

            //Step2
            for (j = 0; j < 4; j++)
            {
                stepnumber = 1;
                if ((gCEMFlg == 1) && (j == 0))
                {
                    continue;
                }
                snprintf(StepDespriction, sizeof(StepDespriction), "Set the Car mode to normal and Usage mode to 0x%X, and wait for 7s.", usagMode[j]);
                TestStep(StepDespriction);
                TRCSwitch(gCMNrmlValue, usagMode[j], 0x0);
                AppWait(7000, "");

                //Step3
                time = 5000;
                snprintf(StepDespriction, sizeof(StepDespriction), "清除DTC,等待%fms", time);
                TestStep(StepDespriction);
                SID14_ClearAllDTC();
                CheckPosResponse_SID14();
                AppWait(time, "");

                //Step4
                TestStep("使用掩码0x09读取DTC");
                RX_SID19_ReportDTCByStatusMask(0x09);
                RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].busOffDTC, 0);

                //Step5
                TestStep("干扰所有被测件发出的报文,确保被测件连续32次Busoff,然后等待DUT恢复通信");
                //InterfereCanMessage(32, 32);
                DisturbFrame(THhandle, index, 32, 32, TimeOut_7011_Disturb);
                if (0 != CheckCommunication(DUTrecoveryTime, NetworkCluster[busFltDTCTemp[index].cluster])) {
                    break;
                }

                //Step6
                TestStep("使用掩码0x09读取DTC");
                RX_SID19_ReportDTCByStatusMask(0x09);
                if (usagMode[j] >= busFltDTCTemp[index].UsgMod)
                {
                    RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].busOffDTC, 1);
                }
                else
                {
                    RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].busOffDTC, 0);
                }
            }

            //Step8
            time = 5000;
            snprintf(StepDespriction, sizeof(StepDespriction), "清除DTC,等待%fms", time);
            TestStep(StepDespriction);
            SID14_ClearAllDTC();
            CheckPosResponse_SID14();
            AppWait(time, "");

            //Step9
            TestStep("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].busOffDTC, 0);

            //Step10
            TestStep("切换Usage mode 为 Driving");
            TRCSwitch(0, 0xD, 0);

            //Step11
            TestStep("等待1s");
            AppWait(1000, "wait fof 1s");

            //Step12
            TestStep("干扰所有被测件发出的报文,确保被测件连续32次Busoff,然后等待DUT恢复通信");
            //InterfereCanMessage(32, 32);
            DisturbFrame(THhandle, index, 32, 32, TimeOut_7011_Disturb);
            if (0 != CheckCommunication(DUTrecoveryTime, NetworkCluster[busFltDTCTemp[index].cluster])) {
                break;
            }

            //Step13
            TestStep("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].busOffDTC, 1);

            //Step14
            time = 5000;
            snprintf(StepDespriction, sizeof(StepDespriction), "清除DTC,等待%fms", time);
            TestStep(StepDespriction);
            SID14_ClearAllDTC();
            CheckPosResponse_SID14();
            AppWait(time, "");

            //Step15
            TestStep("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].busOffDTC, 0);

            //Step16
            TestStep("切换Usage mode 为 Inactive");
            UsgModSwtich(0x1);
            EPLvlSwitch(0x0);

            //Step17
            TestStep("等待7s");
            AppWait(7000, "wait fof 7s");

            //Step18
            TestStep("干扰所有被测件发出的报文,确保被测件连续32次Busoff,然后等待DUT恢复通信");
            //InterfereCanMessage(32, 32);
            DisturbFrame(THhandle, index, 32, 32, TimeOut_7011_Disturb);

            if (0 != CheckCommunication(DUTrecoveryTime, NetworkCluster[busFltDTCTemp[index].cluster])) {
                break;
            }

            //Step19
            TestStep("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            if (gUMInActValue >= busFltDTCTemp[index].UsgMod)
            {
                RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].busOffDTC, 1);
            }
            else
            {
                RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].busOffDTC, 0);
            }
        }
        DisturbDisconnect();
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

s32 TestCase_1325140_CanSMBorTimeTxEnsuredValueTest()
{
    strcpy(TestCaseName, "TestCase_1325140_CanSMBorTimeTxEnsuredValueTest");
    strcpy(TestScopeName, "Busoff DTC");
    strcpy(testPurpose_CH, "测试目的：");

    u8 fdtc[3];
    int fdc10;
    int index;
    double time;
    int distrbCount;
    u8 dtc[3] = { 0xff,0xff,0xff };
    char distrbCountString[10];
    char StepDespriction[256];
    s32 THhandle;
    stepnumber = 0;

    try
    {
        //Step1
        if (0 != TestPrecondition())
            return -1;
        //TestStep("上电并唤醒被测件");
        //if (0 != CheckCommunication(5000)) {
        //    TestPostCondition();
        //    return -1;
        //}
        FulfilTRC();
        TRCSwitch(0x0, 0xD, 0x0);
        THhandle = DisturbConnect();
        for (index = 0; index < BusNum; index++)
        {
            snprintf(StepDespriction, sizeof(StepDespriction), "-------------------------Test bus %s busoff:0x%X---------------------------", busFltDTCTemp[index].cluster, busFltDTCTemp[index].busOffDTC);
            TestReport_HTML_StepDescription(StepDespriction, Info);

            if (busFltDTCTemp[index].TestRslt != 1) {
                snprintf(StepDespriction, sizeof(StepDespriction), "DUT can not record %s BusOff DTC, break this case.", busFltDTCTemp[index].cluster);
                TestReport_HTML_StepDescription(StepDespriction, Warning);
                continue;
            }

            fdtc[0] = (busFltDTCTemp[index].busOffDTC & 0xff0000) >> 16;
            fdtc[1] = (busFltDTCTemp[index].busOffDTC & 0xff00) >> 8;
            fdtc[2] = (busFltDTCTemp[index].busOffDTC & 0xff);

            //Step2
            TestStep("等待7s");
            AppWait(7000, "wait for 7s");

            //Step3
            time = 5000;
            snprintf(StepDespriction, sizeof(StepDespriction), "清除DTC,等待%fms", time);
            TestStep(StepDespriction);
            SID14_ClearAllDTC();
            CheckPosResponse_SID14();
            AppWait(time, "");

            //Step4
            TestStep("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].busOffDTC, 0);

            //Step5
            TestStep("干扰所有被测件发出的报文,确保被测件连续31次Busoff,然后等待DUT恢复通信");

            DisturbFrame(THhandle, index, 32, 31, TimeOut_7011_Disturb);
            if (0 != CheckCommunication(DUTrecoveryTime, NetworkCluster[busFltDTCTemp[index].cluster])) {
                break;
            }
            ////连接设备
            //s32 handle = 0;
            //s32 count = 0;
            //count = tsdisturb.tsdisturb_scan();
            //log("硬件数量 = %d", count);
            //log("handle初始化值 = %d", handle);
            //s32 result1 = tsdisturb.tsdisturb_connect_can(0, &handle, 500, 80, 2000, 80);
            //log("connect函数返回值 = %d", result1);

            ////配置帧触发
            //can_disturbance_trigger trigger = {};
            //canfd_filter filter = {};
            //Disturb_Frame_config_t start_pos = {};
            //memset(&filter, 0, sizeof(canfd_filter));

            ////配置干扰起始位
            //start_pos.Frame_Field = CRC_FIELD_FLAG;//CRC场
            //start_pos.Pos = 15;//第几位
            //start_pos.Stuff_Pos = 0;
            //tsdisturb.tsdisturb_set_trigger_frame(&trigger, &filter, &start_pos);

            ////配置位序列
            //can_disturbance_sequence sequence = {};
            //sequence_config_t config = {};
            //config.bit_num = 20;//序列中有多少 BIT
            //config.Frame_format[0] = 0b101010;//表示序列的高低电平
            //config.div = 1;//表示 1bit 长度
            //tsdisturb.tsdisturb_set_sequence(&sequence, &config);

            ////配置大小干扰周期、间隔
            //disturbance_repetitions repetitions = {};//干扰次数与周期
            //memset(&repetitions, 0, sizeof(repetitions));
            //repetitions.Cycles = 31;//周期
            //repetitions.Cycles_hold = 15;//周期间隔
            //repetitions.Repetitions = 32;//重复次数
            //repetitions.Repetitions_hold = 0;//重复间隔

            ////目的是干扰ECU所发的报文，所以先停止报文仿真
            //com.del_cyclic_messages();

            //if (BusTypeNow == CAN)
            //{
            //    com.transmit_can_async(&TesterPresent);
            //}
            //else if (BusTypeNow == CANFD)
            //{
            //    com.transmit_canfd_async(&TesterPresentFD);
            //}

            //s32 result2 = tsdisturb.tsdisturb_can_disturb_start(handle, &trigger, &sequence, &repetitions, NULL);
            //log("startt函数返回值 = %d", result2);
            //AppWait(2500, ""); //函数执行有延时

            //u32 turetimes = 0;
            //tsdisturb.tsdisturb_query(handle, &turetimes);
            //log("干扰帧数 = %d", turetimes);

            //if (BusTypeNow == CAN)
            //{
            //    com.transmit_can_async(&TesterPresent);
            //}
            //else if (BusTypeNow == CANFD)
            //{
            //    com.transmit_canfd_async(&TesterPresentFD);
            //}

            //Step6
            TestStep("等待1.1s");
            AppWait(1100, "wait for 1.1s");

            //Step7
            TestStep("干扰所有被测件发出的报文,确保被测件连续1次Busoff,然后等待DUT恢复通信");
            DisturbFrame(THhandle, index, 32, 1, TimeOut_7011_Disturb);
            if (0 != CheckCommunication(DUTrecoveryTime, NetworkCluster[busFltDTCTemp[index].cluster])) {
                break;
            }
            ////配置大小干扰周期、间隔
            ////disturbance_repetitions repetitions = {};//干扰次数与周期
            //memset(&repetitions, 0, sizeof(repetitions));
            //repetitions.Cycles = 1;//周期
            //repetitions.Cycles_hold = 15;//周期间隔
            //repetitions.Repetitions = 32;//重复次数
            //repetitions.Repetitions_hold = 0;//重复间隔


            //result2 = tsdisturb.tsdisturb_can_disturb_start(handle, &trigger, &sequence, &repetitions, NULL);
            //log("startt函数返回值 = %d", result2);
            //AppWait(200, ""); //函数执行有延时

            //turetimes = 0;
            //tsdisturb.tsdisturb_query(handle, &turetimes);
            //log("干扰帧数 = %d", turetimes);

            //tsdisturb.tsdisturb_stop(handle);

            //SendTesterPresent(TP_ON);

            //if (gCEMFlg == 1)
            //{
            //    NMWakeUp(1);
            //    AppWait(200, "");
            //    SimVehMtnSpdCANFD();
            //    SimEngStsCAN();
            //    SimIDcDcActLoSideCAN();
            //}
            //else
            //{
            //    NMWakeUp(1);
            //    SimVehMtnGlbSafeCANFD();
            //    SimVehBat(gUbat12);
            //}

            //Step8
            TestStep("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].busOffDTC, 0);

            //Step9
            fdc10 = ReadFDC10(fdtc);
            if (fdc10 == 0x4)
            {
                snprintf(StepDespriction, sizeof(StepDespriction), "The FDC10 is 0x%X", fdc10);
                TestReport_HTML_StepDescription(StepDespriction, Pass);
            }
            else
            {
                snprintf(StepDespriction, sizeof(StepDespriction), "The FDC10 is 0x%X, expected is 0x4.", fdc10);
                TestReport_HTML_StepDescription(StepDespriction, Fail);
            }

            //Step10
            TestStep("等待5s 干扰所有被测件发出的报文,确保被测件连续31次Busoff,然后等待DUT恢复通信");
            AppWait(5000, "");

            DisturbFrame(THhandle, index, 32, 31, TimeOut_7011_Disturb, 0);
            if (0 != CheckCommunication(DUTrecoveryTime, NetworkCluster[busFltDTCTemp[index].cluster])) {
                break;
            }

            ////配置大小干扰周期、间隔
            ////disturbance_repetitions repetitions = {};//干扰次数与周期
            //memset(&repetitions, 0, sizeof(repetitions));
            //repetitions.Cycles = 31;//周期
            //repetitions.Cycles_hold = 15;//周期间隔
            //repetitions.Repetitions = 32;//重复次数
            //repetitions.Repetitions_hold = 0;//重复间隔

            ////目的是干扰ECU所发的报文，所以先停止报文仿真
            //com.del_cyclic_messages();

            //if (BusTypeNow == CAN)
            //{
            //    com.transmit_can_async(&TesterPresent);
            //}
            //else if (BusTypeNow == CANFD)
            //{
            //    com.transmit_canfd_async(&TesterPresentFD);
            //}

            //result2 = tsdisturb.tsdisturb_can_disturb_start(handle, &trigger, &sequence, &repetitions, NULL);
            //log("startt函数返回值 = %d", result2);
            //AppWait(1300, ""); //函数执行有延时
            //turetimes = 0;
            //tsdisturb.tsdisturb_query(handle, &turetimes);
            //log("干扰帧数 = %d", turetimes);

            //Step11
            TestStep("等待0.9s");
            AppWait(900, "wait for 0.9s");


            //Step12
            TestStep("干扰所有被测件发出的报文,确保被测件连续1次Busoff,然后等待DUT恢复通信");
            DisturbFrame(THhandle, index, 32, 1, TimeOut_7011_Disturb);
            if (0 != CheckCommunication(slowRecoveryTime, NetworkCluster[busFltDTCTemp[index].cluster])) {
                break;
            }
            ////配置大小干扰周期、间隔
            ////disturbance_repetitions repetitions = {};//干扰次数与周期
            //memset(&repetitions, 0, sizeof(repetitions));
            //repetitions.Cycles = 1;//周期
            //repetitions.Cycles_hold = 15;//周期间隔
            //repetitions.Repetitions = 32;//重复次数
            //repetitions.Repetitions_hold = 0;//重复间隔

            ////Step12
            //TestStep("干扰所有被测件发出的报文,确保被测件连续1次Busoff,然后等待DUT恢复通信");
            //log("Cycles is %d", repetitions.Cycles);
            //result2 = tsdisturb.tsdisturb_can_disturb_start(handle, &trigger, &sequence, &repetitions, NULL);
            //log("startt函数返回值 = %d", result2);
            //AppWait(100, ""); //函数执行有延时  //该延时设为1500ms时，DTC不能报出，修改为100ms
            //turetimes = 0;
            //tsdisturb.tsdisturb_query(handle, &turetimes);
            //log("干扰帧数 = %d", turetimes);

            //if (BusTypeNow == CAN)
            //{
            //    com.transmit_can_async(&TesterPresent);
            //}
            //else if (BusTypeNow == CANFD)
            //{
            //    com.transmit_canfd_async(&TesterPresentFD);
            //}

            //tsdisturb.tsdisturb_stop(handle);

            //SendTesterPresent(TP_ON);

            //if (gCEMFlg == 1)
            //{
            //    NMWakeUp(1);
            //    AppWait(200, "");
            //    SimVehMtnSpdCANFD();
            //    SimEngStsCAN();
            //    SimIDcDcActLoSideCAN();
            //}
            //else
            //{
            //    NMWakeUp(1);
            //    SimVehMtnGlbSafeCANFD();
            //    SimVehBat(gUbat12);
            //}

            ////断开干扰仪连接
            //tsdisturb.tsdisturb_disconnect_all();

            //Step13
            TestStep("使用掩码0x09读取DTC");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].busOffDTC, 1);

            //Step14
            fdc10 = ReadFDC10(fdtc);
            if (fdc10 == 0x7F)
            {
                snprintf(StepDespriction, sizeof(StepDespriction), "The FDC10 is 0x%X", fdc10);
                TestReport_HTML_StepDescription(StepDespriction, Pass);
            }
            else
            {
                snprintf(StepDespriction, sizeof(StepDespriction), "The FDC10 is 0x%X, expected is 0x7F.", fdc10);
                TestReport_HTML_StepDescription(StepDespriction, Fail);
            }
        }
        DisturbDisconnect();
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

s32 TestCase_1022889_PermanentmissingframeDTCwhenElPowerLevelis1()         //新的
{
    int index, j;
    double time;
    //u8 dtc[3] = { 0xff,0xff,0xff };
    char StepDespriction[256];
    carConfigType = MissFrame;
    strcpy(TestCaseName, "TestCase_1022889_PermanentmissingframeDTCwhenElPowerLevelis1");
    strcpy(TestScopeName, "Permanent Missing Frame DTC");
    strcpy(testPurpose_CH, "测试目的：检查当ElPowerLevel的TRC不满足时，是否记录通信丢失DTC");

    strcpy(StepDespriction, "Set the power supply on and wake up DUT.");
    TestReport_HTML_StepDescription(StepDespriction, Info);
    //NetWork从CCPConfigTemp内取第三个报文
    TCANFD CCNetWorkMsg = { NetworkCluster[CCPConfigTemp[2].cluster],1,15,1,CCPConfigTemp[2].slotID,0,{0x0,0x80,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00} };
    try
    {

        //Step1
        if (0 != TestPrecondition())
            return -1;

        //TestStep("上电并唤醒被测件");
        //if (0 != CheckCommunication(5000)) {
        //    TestPostCondition();
        //    return -1;
        //}
        for (index = 0; index < MISMSG_DTC_NUM; index++)
        {
            if (misMsgDTCList[index].tstRsltFlg == 0) {
                continue;
            }
            snprintf(StepDespriction, sizeof(StepDespriction), "-------------------------Test missing node %s:0x%X---------------------------", misMsgDTCList[index].node, misMsgDTCList[index].dtc);
            TestReport_HTML_StepDescription(StepDespriction, Info);
            for (int k = 0; k < 600; k++)
            {
                gCCPDataExt[k] = 0;
                gCCPData[k] = 0;

            }
            FulfilTRC();
            TRCSwitch(0x0, 0xD, 0x0);
            TestReport_HTML_StepDescription("Start Simulate CCP", Info);

            simulateCCP();

            snprintf(StepDespriction, sizeof(StepDespriction), "Start the simulation of a missing frame DTC related messages.");
            TestReport_HTML_StepDescription(StepDespriction, Info);
            //snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
            //    misMsgDTCList[index].misMsgList->msgID, misMsgDTCList[index].misMsgList->misMsgSignalList->sigName);
            snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
                g_MissFrame_path[index].FFrameIndentifier, misMsgDTCList[index].signalResult->signalName.c_str());
            TestReport_HTML_StepDescription(StepDespriction, Info);
            SimMissMsg(index);                               //仿真missMsg

            //Step2
            TestStep("wait for 7s");
            AppWait(7000, "");

            //Step3
            TestStep("Clear DTCs with diagnostic request, then wait for 127/stepupvalue*TestPeriod*1.2+5s.");
            time = ceil(127 / misMsgDTCList[index].SUValue) * misMsgDTCList[index].testProd * 1.2 + 5000;
            SID14_ClearAllDTC();
            CheckPosResponse_SID14();
            AppWait(time, "");

            //step4
            TestStep("Read DTC from the DUT with status mask 0x09.");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, misMsgDTCList[index].dtc, 0);

            //step5
            TestStep("Set the ElPowerLevel to 1,and wait for 7s");
            EPLvlSwitch(1);
            AppWait(7000, " ");

            //step6
            TestStep("Stop the simulation of a missing frame DTC related messages.");
            //snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
            //    misMsgDTCList[index].misMsgList->msgID, misMsgDTCList[index].misMsgList->misMsgSignalList->sigName);
            snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
                g_MissFrame_path[index].FFrameIndentifier, misMsgDTCList[index].signalResult->signalName.c_str());
            TestReport_HTML_StepDescription(StepDespriction, Info);
            CancelMissMsg(index);                                                        //取消报文仿真

            //step7
            TestStep("wait for 127/stepupvalue*TestPeriod*1.2+5s");
            time = ceil(127 / misMsgDTCList[index].SUValue) * misMsgDTCList[index].testProd * 1.2 + 5000;
            AppWait(time, "");

            //step8
            TestStep("Read DTC from the DUT with status mask 0x09.");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, misMsgDTCList[index].dtc, 0);

            //step9
            TestStep("Set the ElPowerLevel to 0,and wait for 7s");
            UsgModSwtich(0xD);
            EPLvlSwitch(0);
            //TRCSwitch(0,0xD,0);
            AppWait(7000, " ");

            //step10
            TestStep(" wait for 5s");
            AppWait(5000, "");

            //step11
            TestStep("Read DTC from the DUT with status mask 0x01.");
            RX_SID19_ReportDTCByStatusMask(0x01);
            RX_CheckResponse_DTC_SID19_02(0x01, misMsgDTCList[index].dtc, 1);
            CancelMissMsg(index);
            index_cc2 = 0;
            break;
        }
        if (index == MISMSG_DTC_NUM)
        {
            strcpy(StepDespriction, "All DTC is abnormal, break the test.");
            TestReport_HTML_StepDescription(StepDespriction, Fail);
        }
        index_cc2 = 0;
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

s32 TestCase_1022890_PermanentmissingframeDTCwhenthevoltageofpowersupplyisoutofrange()
{
    int index, j;
    double time;
    u8 dtc[3] = { 0xff,0xff,0xff };
    char StepDespriction[256];
    carConfigType = MissFrame;

    strcpy(TestCaseName, "TestCase_1022890_PermanentmissingframeDTCwhenthevoltageofpowersupplyisoutofrange");
    strcpy(TestScopeName, "Permanent Missing Frame DTC");
    strcpy(testPurpose_CH, "测试目的：检查当供电电压的TRC不满足时，是否记录通信丢失DTC");

    strcpy(StepDespriction, "Set the power supply on and wake up DUT.");
    TestReport_HTML_Step(StepNumber, StepDespriction, Info);

    TCANFD CCNetWorkMsg = { NetworkCluster[CCPConfigTemp[2].cluster],1,15,1,CCPConfigTemp[2].slotID,0,{0x0,0x80,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00} };
    try
    {
        //Step1

        if (0 != TestPrecondition())
            return -1;
        //TestStep("上电并唤醒被测件");
        //if (0 != CheckCommunication(5000)) {
        //    TestPostCondition();
        //    return -1;
        //}
        for (index = 0; index < MISMSG_DTC_NUM; index++)
        {
            if (misMsgDTCList[index].tstRsltFlg == 0) {
                continue;
            }
            snprintf(StepDespriction, sizeof(StepDespriction), "-------------------------Test missing node %s:0x%X---------------------------", misMsgDTCList[index].node, misMsgDTCList[index].dtc);
            TestReport_HTML_StepDescription(StepDespriction, Info);

            //if (strcmp(misMsgDTCList[index].cluster, "ZCUD_CAN1") != 0)
            //{
            //    log("下一个循环misMsgDTCList[index].cluster  %s", misMsgDTCList[index].cluster);
            //    TestStep("没有这个节点的通道");
            //    continue;
            //}
            for (int k = 0; k < 600; k++)
            {
                gCCPDataExt[k] = 0;
                gCCPData[k] = 0;

            }
            FulfilTRC();
            TRCSwitch(0x0, 0xD, 0x0);
            TestReport_HTML_StepDescription("Start Simulate CCP", Info);

            simulateCCP();

            snprintf(StepDespriction, sizeof(StepDespriction), "Start the simulation of a missing frame DTC related messages.");
            TestReport_HTML_StepDescription(StepDespriction, Info);
            //snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
            //    misMsgDTCList[index].misMsgList->msgID, misMsgDTCList[index].misMsgList->misMsgSignalList->sigName);
            snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
                g_MissFrame_path[index].FFrameIndentifier, misMsgDTCList[index].signalResult->signalName.c_str());
            TestReport_HTML_StepDescription(StepDespriction, Info);
            SimMissMsg(index);                           //仿真missMsg

            //Step2
            TestStep("wait for 7s");
            AppWait(7000, "");

            //Step3
            TestStep("Clear DTCs with diagnostic request, then wait for 127/stepupvalue*TestPeriod*1.2+5s.");
            time = ceil(127 / misMsgDTCList[index].SUValue) * misMsgDTCList[index].testProd * 1.2 + 5000;
            SID14_ClearAllDTC();
            CheckPosResponse_SID14();
            AppWait(time, "");

            //step4
            TestStep("Read DTC from the DUT with status mask 0x09.");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, misMsgDTCList[index].dtc, 0);

            //step5
            TestStep("Set the Power supply to 8V,and wait for 7s");
            main_power::instance()->Set_Voltage(8);
            AppWait(7000, " ");

            //step6
            TestStep("Stop the simulation of a missing frame DTC related messages.");
            //snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
            //    misMsgDTCList[index].misMsgList->msgID, misMsgDTCList[index].misMsgList->misMsgSignalList->sigName);
            snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
                g_MissFrame_path[index].FFrameIndentifier, misMsgDTCList[index].signalResult->signalName.c_str());
            TestReport_HTML_StepDescription(StepDespriction, Info);
            CancelMissMsg(index);                                                        //取消报文仿真

            //step7
            TestStep("wait for 127/stepupvalue*TestPeriod*1.2+5s");
            time = ceil(127 / misMsgDTCList[index].SUValue) * misMsgDTCList[index].testProd * 1.2 + 5000;
            AppWait(time, "");

            //step8
            TestStep("Read DTC from the DUT with status mask 0x09.");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, misMsgDTCList[index].dtc, 0);

            //step9
            TestStep("Set the Power supply to 12v,and wait for 7s");
            SetVoltageAndRead(12);
            AppWait(7000, " ");

            //step10
            TestStep(" wait for 127/stepupvalue*TestPeriod*1.2+5s,then read DTC with status mask 0x09");
            time = ceil(127 / misMsgDTCList[index].SUValue) * misMsgDTCList[index].testProd * 1.2 + 5000;
            AppWait(time, "");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, misMsgDTCList[index].dtc, 1);

            //step11
            TestStep("Resume the simulation of test DTC related messages");
            //snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
            //    misMsgDTCList[index].misMsgList->msgID, misMsgDTCList[index].misMsgList->misMsgSignalList->sigName);
            snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
                g_MissFrame_path[index].FFrameIndentifier, misMsgDTCList[index].signalResult->signalName.c_str());
            TestReport_HTML_StepDescription(StepDespriction, Info);
            SimMissMsg(index);

            //step12
            TestStep("Clear DTCs with diagnostic request, then wait for 5s.");
            SID14_ClearAllDTC();
            CheckPosResponse_SID14();
            AppWait(5000, "");

            //step13
            TestStep("Read DTC from the DUT with status mask 0x09.");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, misMsgDTCList[index].dtc, 0);

            //step14
            TestStep("Set the power supply to 18V,wait for 7s");
            main_power::instance()->Set_Voltage(18);
            AppWait(7000, " ");

            //step15
            TestStep("Stop the simulation of a missing frame DTC related messages, and wait for 127/stepupvalue*TestPeriod*1.2+5s.");
            //snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
            //    misMsgDTCList[index].misMsgList->msgID, misMsgDTCList[index].misMsgList->misMsgSignalList->sigName);
            snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
                g_MissFrame_path[index].FFrameIndentifier, misMsgDTCList[index].signalResult->signalName.c_str());
            TestReport_HTML_StepDescription(StepDespriction, Info);
            CancelMissMsg(index);
            time = ceil(127 / misMsgDTCList[index].SUValue) * misMsgDTCList[index].testProd * 1.2 + 5000;
            AppWait(time, "");

            //step16
            TestStep("Read DTC from the DUT with status mask 0x09.");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, misMsgDTCList[index].dtc, 0);

            //step17
            TestStep("Set the power supply to 12V,wait for 7s");
            SetVoltageAndRead(12);
            AppWait(7000, "");

            //step18
            TestStep("wait for 127/stepupvalue*TestPeriod*1.2+5s.then read DTC with status mask 0x09");
            time = ceil(127 / misMsgDTCList[index].SUValue) * misMsgDTCList[index].testProd * 1.2 + 5000;
            AppWait(time, "");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, misMsgDTCList[index].dtc, 1);
            CancelMissMsg(index);
            index_cc2 = 0;
            break;
        }
        if (index == MISMSG_DTC_NUM)
        {
            strcpy(StepDespriction, "All DTC is abnormal, break the test.");
            TestReport_HTML_Step(StepNumber, StepDespriction, Fail);
        }
        index_cc2 = 0;
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

s32 TestCase_1022891_PermanentmissingframeDTCwhenUBatisoutofrange()
{
    int index, j;
    double time;
    u8 dtc[3] = { 0xff,0xff,0xff };
    char StepDespriction[256];
    carConfigType = MissFrame;

    strcpy(TestCaseName, " TestCase_1022891_PermanentmissingframeDTCwhenUBatisoutofrange");
    strcpy(TestScopeName, "Permanent Missing Frame DTC");
    strcpy(testPurpose_CH, "测试目的：检查当UBat的TRC不满足时，是否记录通信丢失DTC");


    if (gCEMFlg == 1)
    {
        strcpy(StepDespriction, "This testcase is not applicable for VMM Master.");
        TestReport_HTML_Step(StepNumber, StepDespriction, Info);
        return 0;
    }

    if (0 != TestPrecondition())
        return -1;
    //TestStep("上电并唤醒被测件");
    //if (0 != CheckCommunication(5000)) {
    //    TestPostCondition();
    //    return -1;
    //}



    strcpy(StepDespriction, "Set the power supply on and wake up DUT.");
    TestStep(StepDespriction);

    TCANFD CCNetWorkMsg = { NetworkCluster[CCPConfigTemp[2].cluster],1,15,1,CCPConfigTemp[2].slotID,0,{0x0,0x80,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00} };
    try
    {
        //Step1

        for (index = 0; index < MISMSG_DTC_NUM; index++)
        {
            if (misMsgDTCList[index].tstRsltFlg == 0) {
                continue;
            }
            snprintf(StepDespriction, sizeof(StepDespriction), "-------------------------Test missing node %s:0x%X---------------------------", misMsgDTCList[index].node, misMsgDTCList[index].dtc);
            TestReport_HTML_StepDescription(StepDespriction, Info);

            for (int k = 0; k < 600; k++)
            {
                gCCPDataExt[k] = 0;
                gCCPData[k] = 0;

            }
            FulfilTRC();
            TRCSwitch(0x0, 0xD, 0x0);
            TestReport_HTML_StepDescription("Start Simulate CCP", Info);

            simulateCCP();

            TestReport_HTML_StepDescription("Simulate related messages of test DTC to the bus", Info);
            //snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
            //    misMsgDTCList[index].misMsgList->msgID, misMsgDTCList[index].misMsgList->misMsgSignalList->sigName);
            snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
                g_MissFrame_path[index].FFrameIndentifier, misMsgDTCList[index].signalResult->signalName.c_str());
            TestReport_HTML_StepDescription(StepDespriction, Info);
            SimMissMsg(index);                               //仿真missMsg

            //Step2
            TestStep("wait for 7s");
            AppWait(7000, "");

            //Step3
            TestStep("Clear DTCs with diagnostic request, then wait for 127/stepupvalue*TestPeriod*1.2+5s.");
            time = ceil(127 / misMsgDTCList[index].SUValue) * misMsgDTCList[index].testProd * 1.2 + 5000;
            SID14_ClearAllDTC();
            CheckPosResponse_SID14();
            AppWait(time, "");

            //step4
            TestStep("Read DTC from the DUT with status mask 0x09.");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, misMsgDTCList[index].dtc, 0);

            //step5
            TestStep("Set the Ubat to 8V.,and wait for 7s");
            SimVehBat(gUbat8);
            //gVehBatVotg = gUbat8;
            AppWait(7000, " ");

            //step6
            TestStep("Stop the simulation of a missing frame DTC related messages.");
            //snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
            //    misMsgDTCList[index].misMsgList->msgID, misMsgDTCList[index].misMsgList->misMsgSignalList->sigName);
            snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
                g_MissFrame_path[index].FFrameIndentifier, misMsgDTCList[index].signalResult->signalName.c_str());
            TestReport_HTML_StepDescription(StepDespriction, Info);
            CancelMissMsg(index);                                                        //取消报文仿真

            //step7
            TestStep("wait for 127/stepupvalue*TestPeriod*1.2+5s");
            time = ceil(127 / misMsgDTCList[index].SUValue) * misMsgDTCList[index].testProd * 1.2 + 5000;
            AppWait(time, "");

            //step8
            TestStep("Read DTC from the DUT with status mask 0x09.");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, misMsgDTCList[index].dtc, 0);

            //step9
            TestStep("Set the Ubat to 12V.,and wait for 7s");
            if (gCEMFlg != 1)
                SimVehBat(gUbat12);
            //gVehBatVotg = gUbat12;
            AppWait(7000, " ");

            //step10
            TestStep(" wait for 127/stepupvalue*TestPeriod*1.2+5s,then read DTC with status mask 0x09");
            time = ceil(127 / misMsgDTCList[index].SUValue) * misMsgDTCList[index].testProd * 1.2 + 5000;
            AppWait(time, "");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, misMsgDTCList[index].dtc, 1);

            //step11
            TestStep("Resume the simulation of test DTC related messages");
            //snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
            //    misMsgDTCList[index].misMsgList->msgID, misMsgDTCList[index].misMsgList->misMsgSignalList->sigName);
            snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
                g_MissFrame_path[index].FFrameIndentifier, misMsgDTCList[index].signalResult->signalName.c_str());
            TestReport_HTML_StepDescription(StepDespriction, Info);
            SimMissMsg(index);

            //step12
            TestStep("Clear DTCs with diagnostic request, then wait for 5s.");
            SID14_ClearAllDTC();
            CheckPosResponse_SID14();
            AppWait(5000, "");

            //step13
            TestStep("Read DTC from the DUT with status mask 0x09.");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, misMsgDTCList[index].dtc, 0);

            //step14
            TestStep("Set the Ubat to 16V.,and wait for 7s");
            //gVehBatVotg = gUbat16;
            SimVehBat(gUbat16);
            AppWait(7000, " ");

            //step15
            TestStep("Stop the simulation of a missing frame DTC related messages, and wait for 127/stepupvalue*TestPeriod*1.2+5s.");
            //snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
            //    misMsgDTCList[index].misMsgList->msgID, misMsgDTCList[index].misMsgList->misMsgSignalList->sigName);
            snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
                g_MissFrame_path[index].FFrameIndentifier, misMsgDTCList[index].signalResult->signalName.c_str());
            TestReport_HTML_StepDescription(StepDespriction, Info);
            CancelMissMsg(index);
            time = ceil(127 / misMsgDTCList[index].SUValue) * misMsgDTCList[index].testProd * 1.2 + 5000;
            AppWait(time, "");

            //step16
            TestStep("Read DTC from the DUT with status mask 0x09.");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, misMsgDTCList[index].dtc, 0);

            //step17
            TestStep("Set the Ubat to 12V.,and wait for 7s");
            //gVehBatVotg = gUbat12;
            if (gCEMFlg != 1)
                SimVehBat(gUbat12);
            AppWait(7000, " ");

            //step18
            TestStep("wait for 127/stepupvalue*TestPeriod*1.2+5s.then read DTC with status mask 0x09");
            time = ceil(127 / misMsgDTCList[index].SUValue) * misMsgDTCList[index].testProd * 1.2 + 5000;
            AppWait(time, "");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, misMsgDTCList[index].dtc, 1);
            break;

        }
        if (index == MISMSG_DTC_NUM)
        {
            strcpy(StepDespriction, "All DTC is abnormal, break the test.");
            TestReport_HTML_Step(StepNumber, StepDespriction, Fail);
        }
        CancelMissMsg(index);
        index_cc2 = 0;
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

s32 TestCase_1022892_PermanentmissingframeDTCwhenCarModeisnotNormal()
{
    int index, j;
    double time;
    u8 dtc[3] = { 0xff,0xff,0xff };
    char StepDespriction[256];
    u8 TempCarMode = 0;
    carConfigType = MissFrame;

    strcpy(TestCaseName, "TestCase_1022892_PermanentmissingframeDTCwhenCarModeisnotNormal");
    strcpy(TestScopeName, "Permanent Missing Frame DTC");
    strcpy(testPurpose_CH, "测试目的：检查当Carmode的TRC不满足时，是否记录通信丢失DTC");

    //TestPrecondition();
    //strcpy(StepDespriction, "Set the power supply on and wake up DUT.");
    //TestStep(StepDespriction);

    TCANFD CCNetWorkMsg = { NetworkCluster[CCPConfigTemp[2].cluster],1,15,1,CCPConfigTemp[2].slotID,0,{0x0,0x80,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00} };
    try
    {
        //Step1
        if (0 != TestPrecondition())
            return -1;
        //TestStep("上电并唤醒被测件");
        //if (0 != CheckCommunication(5000)) {
        //    TestPostCondition();
        //    return -1;
        //}
        for (index = 0; index < MISMSG_DTC_NUM; index++)
        {
            if (misMsgDTCList[index].tstRsltFlg == 0) {
                continue;
            }
            snprintf(StepDespriction, sizeof(StepDespriction), "-------------------------Test missing node %s:0x%X---------------------------", misMsgDTCList[index].node, misMsgDTCList[index].dtc);
            TestReport_HTML_StepDescription(StepDespriction, Info);

            /*if (strcmp(misMsgDTCList[index].cluster, "ZCUD_CAN1") != 0)
            {
                log("下一个循环misMsgDTCList[index].cluster  %s", misMsgDTCList[index].cluster);
                TestStep("没有这个节点的通道");
                continue;
            }*/
            for (int k = 0; k < 600; k++)
            {
                gCCPDataExt[k] = 0;
                gCCPData[k] = 0;

            }
            FulfilTRC();
            TRCSwitch(0x0, 0xB, 0x0);
            TestReport_HTML_StepDescription("Start Simulate CCP", Info);

            simulateCCP();

            TestReport_HTML_StepDescription("Simulate related messages of test DTC to the bus", Info);
            //snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
            //    misMsgDTCList[index].misMsgList->msgID, misMsgDTCList[index].misMsgList->misMsgSignalList->sigName);
            snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
                g_MissFrame_path[index].FFrameIndentifier, misMsgDTCList[index].signalResult->signalName.c_str());
            TestReport_HTML_StepDescription(StepDespriction, Info);
            SimMissMsg(index);                           //仿真missMsg

            //Step2
            TestStep("wait for 7s");
            AppWait(7000, "");


            for (int k = 0; k < 3; k++)
            {
                //Step3
                TestStep("Clear DTCs with diagnostic request, then wait for 127/stepupvalue*TestPeriod*1.2+5s.");
                time = ceil(127 / misMsgDTCList[index].SUValue) * misMsgDTCList[index].testProd * 1.2 + 5000;
                SID14_ClearAllDTC();
                CheckPosResponse_SID14();
                AppWait(time, "");

                //step4
                TestStep("Read DTC from the DUT with status mask 0x09.");
                RX_SID19_ReportDTCByStatusMask(0x09);
                RX_CheckResponse_DTC_SID19_02(0x09, misMsgDTCList[index].dtc, 0);

                if (gCEMFlg) {
                    TempCarMode = 3;
                }
                else {
                    switch (k)
                    {
                    case 0:
                        TempCarMode = 2;
                        break;
                    case 1:
                        TempCarMode = 1;
                        break;
                    case 2:
                        TempCarMode = 3;
                        break;
                    default:
                        break;
                    }
                }
                snprintf(StepDespriction, sizeof(StepDespriction), "Set the car mode to 0x%X.(Transport:0x1;Factory:0x2;Crash:0x3),and wait for 7s", TempCarMode);
                //step5
                TestStep(StepDespriction);         //此处要根据被测件来，已经写死  //已修改根据DUT打印报告
                if (gCEMFlg)
                {
                    log("k  %d", k);
                    UsgModSwtich(0xB);
                    CarModSwtich(3);
                    UsgModSwtich(0xD);
                }
                else
                {
                    switch (k)
                    {
                    case 0:
                        UsgModSwtich(0xB);
                        CarModSwtich(2);
                        UsgModSwtich(0xD);
                    case 1:
                        UsgModSwtich(0xB);
                        CarModSwtich(1);
                        UsgModSwtich(0xD);
                    case 2:
                        UsgModSwtich(0xB);
                        CarModSwtich(3);
                        UsgModSwtich(0xD);
                    default:
                        break;
                    }

                }

                AppWait(7000, " ");

                //step6
                TestStep("Stop the simulation of a missing frame DTC related messages.");
                //snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
                //    misMsgDTCList[index].misMsgList->msgID, misMsgDTCList[index].misMsgList->misMsgSignalList->sigName);
                snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
                    g_MissFrame_path[index].FFrameIndentifier, misMsgDTCList[index].signalResult->signalName.c_str());
                TestReport_HTML_StepDescription(StepDespriction, Info);
                CancelMissMsg(index);                                                        //取消报文仿真

                //step7
                TestStep("wait for 127/stepupvalue*TestPeriod*1.2+5s");
                time = ceil(127 / misMsgDTCList[index].SUValue) * misMsgDTCList[index].testProd * 1.2 + 5000;
                AppWait(time, "");

                //step8
                TestStep("Read DTC from the DUT with status mask 0x09.");
                RX_SID19_ReportDTCByStatusMask(0x09);
                RX_CheckResponse_DTC_SID19_02(0x09, misMsgDTCList[index].dtc, 0);

                //step9
                TestStep("Resume the simulation of test DTC related messages");
                //snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
                //    misMsgDTCList[index].misMsgList->msgID, misMsgDTCList[index].misMsgList->misMsgSignalList->sigName);
                snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
                    g_MissFrame_path[index].FFrameIndentifier, misMsgDTCList[index].signalResult->signalName.c_str());
                TestReport_HTML_StepDescription(StepDespriction, Info);
                SimMissMsg(index);
                stepnumber = 2;
            }
            CancelMissMsg(index);
            index_cc2 = 0;
            break;

        }

        if (index == MISMSG_DTC_NUM)
        {
            strcpy(StepDespriction, "All DTC is abnormal, break the test.");
            TestReport_HTML_Step(StepNumber, StepDespriction, Fail);
        }
        index_cc2 = 0;
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

s32 TestCase_1022893_PermanentmissingframeDTCafterCarModetransition()
{
    int index, j;
    double time;
    u8 dtc[3] = { 0xff,0xff,0xff };
    char StepDespriction[256];
    bool isZCU = true;
    carConfigType = MissFrame;

    strcpy(TestCaseName, " TestCase_1022893_PermanentmissingframeDTCafterCarModetransition");
    strcpy(TestScopeName, "Permanent Missing Frame DTC");
    strcpy(testPurpose_CH, "测试目的：检查当Carmode切换的TRC不满足时，是否记录通信丢失DTC");

    if (gCEMFlg == 1)
    {
        strcpy(StepDespriction, "This testcase is not applicable for VMM Master.");
        TestReport_HTML_Step(StepNumber, StepDespriction, Info);
        return 0;
    }

    if (0 != TestPrecondition())
        return -1;
    //TestStep("上电并唤醒被测件");
    //if (0 != CheckCommunication(5000)) {
    //    TestPostCondition();
    //    return -1;
    //}


    //strcpy(StepDespriction, "Set the power supply on and wake up DUT.");
    //TestStep(StepDespriction);

    TCANFD CCNetWorkMsg = { NetworkCluster[CCPConfigTemp[2].cluster],1,15,1,CCPConfigTemp[2].slotID,0,{0x0,0x80,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00} };
    try
    {
        //Step1

        for (index = 0; index < MISMSG_DTC_NUM; index++)
        {
            if (misMsgDTCList[index].tstRsltFlg == 0) {
                continue;
            }
            snprintf(StepDespriction, sizeof(StepDespriction), "-------------------------Test missing node %s:0x%X---------------------------", misMsgDTCList[index].node, misMsgDTCList[index].dtc);
            TestReport_HTML_StepDescription(StepDespriction, Info);

            /*  if (strcmp(misMsgDTCList[index].cluster, "ZCUD_CAN1") != 0)
                {
                    log("下一个循环misMsgDTCList[index].cluster  %s", misMsgDTCList[index].cluster);
                    TestStep("没有这个节点的通道");
                    continue;
                }*/
            for (int k = 0; k < 600; k++)
            {
                gCCPDataExt[k] = 0;
                gCCPData[k] = 0;

            }
            FulfilTRC();
            TRCSwitch(0x0, 0xD, 0x0);
            TestReport_HTML_StepDescription("Start Simulate CCP", Info);

            simulateCCP();

            TestReport_HTML_StepDescription("Simulate related messages of test DTC to the bus", Info);
            //snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
            //    misMsgDTCList[index].misMsgList->msgID, misMsgDTCList[index].misMsgList->misMsgSignalList->sigName);
            snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
                g_MissFrame_path[index].FFrameIndentifier, misMsgDTCList[index].signalResult->signalName.c_str());
            TestReport_HTML_StepDescription(StepDespriction, Info);
            SimMissMsg(index);                           //仿真missMsg

            //Step2
            TestStep("wait for 7s");
            AppWait(7000, "");

            //Step3
            TestStep("Clear DTCs with diagnostic request, then wait for 127/stepupvalue*TestPeriod*1.2+5s.");
            time = ceil(127 / misMsgDTCList[index].SUValue) * misMsgDTCList[index].testProd * 1.2 + 5000;
            SID14_ClearAllDTC();
            CheckPosResponse_SID14();
            AppWait(time, "");

            //step4
            TestStep("Read DTC from the DUT with status mask 0x09.");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, misMsgDTCList[index].dtc, 0);

            //step5
            TestStep("Set the car mode to 0x3.(Transport:0x1;Factory:0x2;Crash:0x3)");
            UsgModSwtich(0xb);
            CarModSwtich(3);

            //step6
            TestStep(" wait for 7s");
            AppWait(7000, " ");

            //step7
            TestStep("Set the car mode to normal : 0x0,Set the usage mode to Driving:0xD.");
            CarModSwtich(0);
            UsgModSwtich(0xD);

            //step8
            TestStep("Stop the simulation of a missing frame DTC related messages.");
            //snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
            //    misMsgDTCList[index].misMsgList->msgID, misMsgDTCList[index].misMsgList->misMsgSignalList->sigName);
            snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
                g_MissFrame_path[index].FFrameIndentifier, misMsgDTCList[index].signalResult->signalName.c_str());
            TestReport_HTML_StepDescription(StepDespriction, Info);
            CancelMissMsg(index);

            //step9
            TestStep("Clear DTCs with diagnostic request, then wait for 127/stepupvalue*TestPeriod+2s.");
            time = ceil(127 / misMsgDTCList[index].SUValue) * misMsgDTCList[index].testProd + 2000;
            AppWait(time, "");

            //step10
            TestStep("Read DTC from the DUT with status mask 0x09.");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, misMsgDTCList[index].dtc, 0);

            //step11
            TestStep(" wait for 7s");
            AppWait(7000, " ");

            //step12
            TestStep("Read DTC from the DUT with status mask 0x01.");
            RX_SID19_ReportDTCByStatusMask(0x01);
            RX_CheckResponse_DTC_SID19_02(0x01, misMsgDTCList[index].dtc, 1);
            CancelMissMsg(index);
            index_cc2 = 0;
            break;
        }
        index_cc2 = 0;
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

s32 TestCase_1022894_PermanentmissingframeDTCwhenUsageModeisnotdriving()
{
    int index, j;
    double time;
    u8 dtc[3] = { 0xff,0xff,0xff };
    char StepDespriction[256];
    u8 TempUsgMod[4] = { 0,0x01,0x02,0x0B };
    carConfigType = MissFrame;

    strcpy(TestCaseName, " TestCase_1022894_PermanentmissingframeDTCwhenUsageModeisnotdriving");
    strcpy(TestScopeName, "Permanent Missing Frame DTC");
    strcpy(testPurpose_CH, "测试目的：检查当Usagemode的TRC不满足时，是否记录通信丢失DTC");

    if (0 != TestPrecondition())
        return -1;
    //TestStep("上电并唤醒被测件");
    //if (0 != CheckCommunication(5000)) {
    //    TestPostCondition();
    //    return -1;
    //}
    strcpy(StepDespriction, "Set the power supply on and wake up DUT.");
    TestReport_HTML_StepDescription(StepDespriction, Info);

    //TestPrecondition_TP_CAN();
    TCANFD CCNetWorkMsg = { NetworkCluster[CCPConfigTemp[2].cluster],1,15,1,CCPConfigTemp[2].slotID,0,{0x0,0x80,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00} };
    try
    {
        //Step1

        for (index = 0; index < MISMSG_DTC_NUM; index++)
        {
            if (misMsgDTCList[index].tstRsltFlg == 0) {
                continue;
            }
            snprintf(StepDespriction, sizeof(StepDespriction), "-------------------------Test missing node %s:0x%X---------------------------", misMsgDTCList[index].node, misMsgDTCList[index].dtc);
            TestReport_HTML_StepDescription(StepDespriction, Info);

            //if (strcmp(misMsgDTCList[index].cluster, "ZCUD_CAN1") != 0)
            //{
            //    log("下一个循环misMsgDTCList[index].cluster  %s", misMsgDTCList[index].cluster);
            //    TestStep("没有这个节点的通道");
            //    continue;
            //}
            for (int k = 0; k < 600; k++)
            {
                gCCPDataExt[k] = 0;
                gCCPData[k] = 0;

            }
            FulfilTRC();
            TRCSwitch(0x0, 0xD, 0x0);
            TestReport_HTML_StepDescription("Start Simulate CCP", Info);

            simulateCCP();

            TestReport_HTML_StepDescription("Simulate related messages of test DTC to the bus", Info);
            //snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
            //    misMsgDTCList[index].misMsgList->msgID, misMsgDTCList[index].misMsgList->misMsgSignalList->sigName);
            snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
                g_MissFrame_path[index].FFrameIndentifier, misMsgDTCList[index].signalResult->signalName.c_str());
            TestReport_HTML_StepDescription(StepDespriction, Info);
            SimMissMsg(index);                           //仿真missMsg

            for (int i = 1; i < 4; i++)
            {
                snprintf(StepDespriction, sizeof(StepDespriction), "Set the usage mode to 0x%X and wait for 7s.(Abandoned:0x0;Inactive:0x1;Convenience:0x2;Active:0xb)", TempUsgMod[i]);
                //Step2
                TestStep(StepDespriction);
                //TestReport_HTML_Step(StepNumber, StepDespriction, Info);
                switch (i)
                {
                case 0:
                    if (gCEMFlg)
                    {
                        UsgModSwtich(0x1);
                    }
                    else
                    {
                        UsgModSwtich(0x0);
                    }
                    break;
                case 1:
                    if (gCEMFlg)
                    {
                        UsgModSwtich(TempUsgMod[i]);
                    }
                    else
                    {
                        UsgModSwtich(0x1);
                    }
                    break;
                case 2:
                    if (gCEMFlg)
                    {
                        UsgModSwtich(TempUsgMod[i]);
                    }
                    else
                    {
                        UsgModSwtich(0x2);
                    }
                    break;
                case 3:
                    if (gCEMFlg)
                    {
                        UsgModSwtich(TempUsgMod[i]);
                    }
                    else
                    {
                        UsgModSwtich(0xb);
                    }
                    break;
                }


                AppWait(7000, "");

                //Step3
                TestStep("Clear DTCs with diagnostic request, then wait for 127/stepupvalue*TestPeriod*1.2+5s.");
                time = ceil(127 / misMsgDTCList[index].SUValue) * misMsgDTCList[index].testProd * 1.2 + 5000;
                SID14_ClearAllDTC();
                CheckPosResponse_SID14();
                AppWait(time, "");

                //step4
                TestStep("Read DTC from the DUT with status mask 0x09.");
                RX_SID19_ReportDTCByStatusMask(0x09);
                RX_CheckResponse_DTC_SID19_02(0x09, misMsgDTCList[index].dtc, 0);

                //step5
                TestStep("Stop the simulation of a missing frame DTC related messages.");
                //snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
                //    misMsgDTCList[index].misMsgList->msgID, misMsgDTCList[index].misMsgList->misMsgSignalList->sigName);
                snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
                    g_MissFrame_path[index].FFrameIndentifier, misMsgDTCList[index].signalResult->signalName.c_str());
                TestReport_HTML_StepDescription(StepDespriction, Info);
                CancelMissMsg(index);                                                        //取消报文仿真

                //step6
                TestStep("wait for 127/stepupvalue*TestPeriod*1.2+5s.");
                time = ceil(127 / misMsgDTCList[index].SUValue) * misMsgDTCList[index].testProd * 1.2 + 5000;
                AppWait(time, "");

                //step7
                TestStep("Read DTC from the DUT with status mask 0x09.");
                RX_SID19_ReportDTCByStatusMask(0x09);
                RX_CheckResponse_DTC_SID19_02(0x09, misMsgDTCList[index].dtc, 0);

                //step8
                TestStep("Resume the simulation of test DTC related messages");
                //snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
                //    misMsgDTCList[index].misMsgList->msgID, misMsgDTCList[index].misMsgList->misMsgSignalList->sigName);
                snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
                    g_MissFrame_path[index].FFrameIndentifier, misMsgDTCList[index].signalResult->signalName.c_str());
                TestReport_HTML_StepDescription(StepDespriction, Info);
                SimMissMsg(index);
                if (i < 4)
                    stepnumber = 1;
            }


            //step9
            TestStep("Repeat step2-8 to test other usage mode except Driving");


            //step10
            TestStep(" Set Usage mode to Driving, and wait for 7s.");
            UsgModSwtich(0xD);
            AppWait(7000, "");

            //step11
            TestStep("Clear DTCs with diagnostic request, then wait for 127/stepupvalue*TestPeriod*1.2+5s.");
            time = ceil(127 / misMsgDTCList[index].SUValue) * misMsgDTCList[index].testProd * 1.2 + 5000;
            SID14_ClearAllDTC();
            CheckPosResponse_SID14();
            AppWait(time, "");

            //step12
            TestStep("Read DTC from the DUT with status mask 0x09.");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, misMsgDTCList[index].dtc, 0);

            //step13
            TestStep("Set Usage mode to Convenience, and wait for 7s.");
            UsgModSwtich(0x2);
            AppWait(7000, "");

            //step14
            TestStep("Stop the simulation of a missing frame DTC related messages.");
            //snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
            //    misMsgDTCList[index].misMsgList->msgID, misMsgDTCList[index].misMsgList->misMsgSignalList->sigName);
            snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
                g_MissFrame_path[index].FFrameIndentifier, misMsgDTCList[index].signalResult->signalName.c_str());
            TestReport_HTML_StepDescription(StepDespriction, Info);
            CancelMissMsg(index);                                                        //取消报文仿真


            //step15
            TestStep("wait for 127/stepupvalue*TestPeriod*1.2+5s.");
            time = ceil(127 / misMsgDTCList[index].SUValue) * misMsgDTCList[index].testProd * 1.2 + 5000;
            AppWait(time, "");

            //step16
            TestStep("Read DTC from the DUT with status mask 0x09.");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, misMsgDTCList[index].dtc, 0);
            CancelMissMsg(index);
            index_cc2 = 0;
            break;
        }
        if (index == MISMSG_DTC_NUM)
        {
            strcpy(StepDespriction, "All DTC is abnormal, break the test.");
            TestReport_HTML_Step(StepNumber, StepDespriction, Fail);
        }
        index_cc2 = 0;
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

s32 TestCase_1022895_PermanentmissingframeDTCwhenVehModMngGlbSafeisnotavailable()                  //未完成 
{
    int index, j;
    double time;
    u8 dtc[3] = { 0xff,0xff,0xff };
    char StepDespriction[256];
    carConfigType = MissFrame;

    strcpy(TestCaseName, " TestCase_1022895_PermanentmissingframeDTCwhenVehModMngGlbSafeisnotavailable");
    strcpy(TestScopeName, "Permanent Missing Frame DTC");
    strcpy(testPurpose_CH, "测试目的：检查当VehModMngGlbSafe的TRC不满足时，是否记录通信丢失DTC");

    if (gCEMFlg == 1)
    {
        strcpy(StepDespriction, "This testcase is not applicable for VMM Master.");
        TestReport_HTML_Step(StepNumber, StepDespriction, Info);
        return 0;
    }

    if (0 != TestPrecondition())
        return -1;
    //TestStep("上电并唤醒被测件");
    //if (0 != CheckCommunication(5000)) {
    //    TestPostCondition();
    //    return -1;
    //}


    strcpy(StepDespriction, "Set the power supply on and wake up DUT.");
    TestReport_HTML_StepDescription(StepDespriction, Info);

    TCANFD CCNetWorkMsg = { NetworkCluster[CCPConfigTemp[2].cluster],1,15,1,CCPConfigTemp[2].slotID,0,{0x0,0x80,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00} };
    try
    {
        //Step1
        for (index = 0; index < MISMSG_DTC_NUM; index++)
        {
            if (misMsgDTCList[index].tstRsltFlg == 0) {
                continue;
            }
            snprintf(StepDespriction, sizeof(StepDespriction), "-------------------------Test missing node %s:0x%X---------------------------", misMsgDTCList[index].node, misMsgDTCList[index].dtc);
            TestReport_HTML_StepDescription(StepDespriction, Info);

            //if (strcmp(misMsgDTCList[index].cluster, "ZCUD_CAN1") != 0)
            //{
            //    log("下一个循环misMsgDTCList[index].cluster  %s", misMsgDTCList[index].cluster);
            //    TestStep("没有这个节点的通道");
            //    continue;
            //}
            for (int k = 0; k < 600; k++)
            {
                gCCPDataExt[k] = 0;
                gCCPData[k] = 0;

            }
            FulfilTRC();
            TRCSwitch(0x0, 0xD, 0x0);
            TestReport_HTML_StepDescription("Start Simulate CCP", Info);

            simulateCCP();


            TestReport_HTML_StepDescription("Simulate related messages of test DTC to the bus", Info);
            //snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
            //    misMsgDTCList[index].misMsgList->msgID, misMsgDTCList[index].misMsgList->misMsgSignalList->sigName);
            snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
                g_MissFrame_path[index].FFrameIndentifier, misMsgDTCList[index].signalResult->signalName.c_str());
            TestReport_HTML_StepDescription(StepDespriction, Info);
            SimMissMsg(index);                           //仿真missMsg

            //Step2
            TestStep("wait for 7s");
            AppWait(7000, "");

            //Step3
            TestStep("Clear DTCs with diagnostic request, then wait for 127/stepupvalue*TestPeriod*1.2+5s.");
            time = ceil(127 / misMsgDTCList[index].SUValue) * misMsgDTCList[index].testProd * 1.2 + 5000;
            SID14_ClearAllDTC();
            CheckPosResponse_SID14();
            AppWait(time, "");

            //step4
            TestStep("Read DTC from the DUT with status mask 0x09.");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, misMsgDTCList[index].dtc, 0);

            //step5
            TestStep("Keep the update bit of signalgroup VehModMngGlbSafe to 0, then wait for 7s.");
            gVehManUB = 0;
            AppWait(7000, " ");

            //step6
            TestStep("Stop the simulation of a missing frame DTC related messages.");
            //snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
            //    misMsgDTCList[index].misMsgList->msgID, misMsgDTCList[index].misMsgList->misMsgSignalList->sigName);
            snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
                g_MissFrame_path[index].FFrameIndentifier, misMsgDTCList[index].signalResult->signalName.c_str());
            TestReport_HTML_StepDescription(StepDespriction, Info);
            CancelMissMsg(index);                                                        //取消报文仿真

            //step7
            TestStep("wait for 127/stepupvalue*TestPeriod*1.2+5s");
            time = ceil(127 / misMsgDTCList[index].SUValue) * misMsgDTCList[index].testProd * 1.2 + 5000;
            AppWait(time, "");

            //step8
            TestStep("Read DTC from the DUT with status mask 0x01.");
            RX_SID19_ReportDTCByStatusMask(0x01);
            RX_CheckResponse_DTC_SID19_02(0x01, misMsgDTCList[index].dtc, 0);

            //step9
            TestStep("set the update bit of signalgroup VehModMngGlbSafe to 1");
            gVehManUB = 1;

            //step10
            TestStep(" wait for 127/stepupvalue*TestPeriod*1.2+5s");
            time = ceil(127 / misMsgDTCList[index].SUValue) * misMsgDTCList[index].testProd * 1.2 + 5000;
            AppWait(time, "");

            //step11
            TestStep("Read DTC from the DUT with status mask 0x01.");
            RX_SID19_ReportDTCByStatusMask(0x01);
            RX_CheckResponse_DTC_SID19_02(0x01, misMsgDTCList[index].dtc, 1);
            CancelMissMsg(index);
            index_cc2 = 0;
            break;
        }
        if (index == MISMSG_DTC_NUM)
        {
            strcpy(StepDespriction, "All DTC is abnormal, break the test.");
            TestReport_HTML_Step(StepNumber, StepDespriction, Fail);
        }
        index_cc2 = 0;
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

s32 TestCase_1022896_PermanentmissingframeDTCwhenUsageModeTransmission()
{
    int index, j;
    double time;
    u8 dtc[3] = { 0xff,0xff,0xff };
    char StepDespriction[256];
    carConfigType = MissFrame;

    strcpy(TestCaseName, " TestCase_1022896_PermanentmissingframeDTCwhenUsageModeTransmission");
    strcpy(TestScopeName, "Permanent Missing Frame DTC");
    strcpy(testPurpose_CH, "测试目的：检查当Usagemode切换的TRC不满足时，是否记录通信丢失DTC");

    if (0 != TestPrecondition())
        return -1;
    //TestStep("上电并唤醒被测件");
    //if (0 != CheckCommunication(5000)) {
    //    TestPostCondition();
    //    return -1;
    //}

    TCANFD CCNetWorkMsg = { NetworkCluster[CCPConfigTemp[2].cluster],1,15,1,CCPConfigTemp[2].slotID,0,{0x0,0x80,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00} };
    try
    {
        //Step1

        for (index = 0; index < MISMSG_DTC_NUM; index++)
        {
            if (misMsgDTCList[index].tstRsltFlg == 0) {
                continue;
            }
            snprintf(StepDespriction, sizeof(StepDespriction), "-------------------------Test missing node %s:0x%X---------------------------", misMsgDTCList[index].node, misMsgDTCList[index].dtc);
            TestReport_HTML_StepDescription(StepDespriction, Info);

            //if (strcmp(misMsgDTCList[index].cluster, "ZCUD_CAN1") != 0)
            //{
            //    log("下一个循环misMsgDTCList[index].cluster  %s", misMsgDTCList[index].cluster);
            //    TestStep("没有这个节点的通道");
            //    continue;
            //}
            for (int k = 0; k < 600; k++)
            {
                gCCPDataExt[k] = 0;
                gCCPData[k] = 0;

            }
            FulfilTRC();
            TRCSwitch(0x0, 0xD, 0x0);
            TestReport_HTML_StepDescription("Start Simulate CCP", Info);

            simulateCCP();

            TestReport_HTML_StepDescription("Simulate related messages of test DTC to the bus", Info);
            //snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
            //    misMsgDTCList[index].misMsgList->msgID, misMsgDTCList[index].misMsgList->misMsgSignalList->sigName);
            snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
                g_MissFrame_path[index].FFrameIndentifier, misMsgDTCList[index].signalResult->signalName.c_str());
            TestReport_HTML_StepDescription(StepDespriction, Info);
            SimMissMsg(index);                           //仿真missMsg

            //Step2
            TestStep("wait for 7s");
            AppWait(7000, "");

            //Step3
            TestStep("Clear DTCs with diagnostic request, then wait for 5s.");
            SID14_ClearAllDTC();
            CheckPosResponse_SID14();
            AppWait(5000, "");

            //step4
            TestStep("Read DTC from the DUT with status mask 0x09.");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, misMsgDTCList[index].dtc, 0);

            //step5
            TestStep(" Set Usage mode to Active.");
            UsgModSwtich(0xb);

            //step6
            TestStep("Wait for 7s, and then set the usage mode to Driving.");
            AppWait(7000, "");
            UsgModSwtich(0xD);

            //step7
            TestStep("Stop the simulation of a missing frame DTC related messages");
            //snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
            //    misMsgDTCList[index].misMsgList->msgID, misMsgDTCList[index].misMsgList->misMsgSignalList->sigName);
            snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
                g_MissFrame_path[index].FFrameIndentifier, misMsgDTCList[index].signalResult->signalName.c_str());
            TestReport_HTML_StepDescription(StepDespriction, Info);
            CancelMissMsg(index);

            //step8
            TestStep("wait for 5s");
            AppWait(5000, "");

            //step9
            TestStep("Read DTC from the DUT with status mask 0x01.");
            RX_SID19_ReportDTCByStatusMask(0x01);
            RX_CheckResponse_DTC_SID19_02(0x01, misMsgDTCList[index].dtc, 0);

            //step10
            TestStep("wait for 5s");
            AppWait(5000, "");

            //step11
            TestStep("Read DTC from the DUT with status mask 0x01.");
            RX_SID19_ReportDTCByStatusMask(0x01);
            RX_CheckResponse_DTC_SID19_02(0x01, misMsgDTCList[index].dtc, 1);
            index_cc2 = 0;
            break;
        }
        if (index == MISMSG_DTC_NUM)
        {
            strcpy(StepDespriction, "All DTC is abnormal, break the test.");
            TestReport_HTML_Step(StepNumber, StepDespriction, Fail);
        }
        index_cc2 = 0;
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

s32 TestCase_1022897_PermanentmissingframeDTCwhenDUTnotreceivemissingframeTRC()    //未完成
{
    int index, j;
    double time;
    u8 dtc[3] = { 0xff,0xff,0xff };
    char StepDespriction[256];
    carConfigType = MissFrame;

    strcpy(TestCaseName, "TestCase_1022897_PermanentmissingframeDTCwhenDUTnotreceivemissingframeTRC");
    strcpy(TestScopeName, "Permanent Missing Frame DTC");
    strcpy(testPurpose_CH, "测试目的：检查当TRC不满足时，是否记录通信丢失DTC");

    if (gCEMFlg == 1)
    {
        strcpy(StepDespriction, "This testcase is not applicable for VMM Master.");
        TestReport_HTML_Step(StepNumber, StepDespriction, Info);
        return 0;
    }
    if (0 != TestPrecondition())
        return -1;
    //TestStep("上电并唤醒被测件");
    //if (0 != CheckCommunication(5000)) {
    //    TestPostCondition();
    //    return -1;
    //}



    TCANFD CCNetWorkMsg = { NetworkCluster[CCPConfigTemp[2].cluster],1,15,1,CCPConfigTemp[2].slotID,0,{0x0,0x80,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00} };
    try
    {
        //Step1

        for (index = 0; index < MISMSG_DTC_NUM; index++)
        {
            snprintf(StepDespriction, sizeof(StepDespriction), "-------------------------Test missing node %s:0x%X---------------------------", misMsgDTCList[index].node, misMsgDTCList[index].dtc);
            TestReport_HTML_Step(StepNumber, StepDespriction, Info);

            //if (strcmp(misMsgDTCList[index].cluster, "ZCUD_CAN1") != 0)
            //{
            //    log("下一个循环misMsgDTCList[index].cluster  %s", misMsgDTCList[index].cluster);
            //    TestStep("没有这个节点的通道");
            //    continue;
            //}
            for (int k = 0; k < 600; k++)
            {
                gCCPDataExt[k] = 0;
                gCCPData[k] = 0;

            }
            FulfilTRC();
            TRCSwitch(0x0, 0xD, 0x0);
            TestReport_HTML_StepDescription("Start Simulate CCP", Info);

            simulateCCP();

            TestReport_HTML_StepDescription("Simulate related messages of test DTC to the bus", Info);
            //snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
            //    misMsgDTCList[index].misMsgList->msgID, misMsgDTCList[index].misMsgList->misMsgSignalList->sigName);
            snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
                g_MissFrame_path[index].FFrameIndentifier, misMsgDTCList[index].signalResult->signalName.c_str());
            TestReport_HTML_StepDescription(StepDespriction, Info);
            SimMissMsg(index);                           //仿真missMsg

            //Step2
            TestStep("stop simulating the signalgroup VehModMngGlbSafe .wait for 7s");
            AppWait(7000, "");

            //Step3
            TestStep("Clear DTCs with diagnostic request, then wait for 127/stepupvalue*TestPeriod*1.2+5s.");
            time = ceil(127 / misMsgDTCList[index].SUValue) * misMsgDTCList[index].testProd * 1.2 + 5000;
            SID14_ClearAllDTC();
            CheckPosResponse_SID14();
            AppWait(time, "");

            //step4
            TestStep("Read DTC from the DUT with status mask 0x09.");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, misMsgDTCList[index].dtc, 0);

            //step5
            TestStep("Stop the simulation of a missing frame DTC related messages.");
            //snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
            //    misMsgDTCList[index].misMsgList->msgID, misMsgDTCList[index].misMsgList->misMsgSignalList->sigName);
            snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
                g_MissFrame_path[index].FFrameIndentifier, misMsgDTCList[index].signalResult->signalName.c_str());
            TestReport_HTML_StepDescription(StepDespriction, Info);
            CancelMissMsg(index);

            //step6
            TestStep("wait for 127/stepupvalue*TestPeriod*1.2+5s");
            time = ceil(127 / misMsgDTCList[index].SUValue) * misMsgDTCList[index].testProd * 1.2 + 5000;
            AppWait(time, "");

            //step7
            TestStep("Read DTC from the DUT with status mask 0x01.");
            RX_SID19_ReportDTCByStatusMask(0x01);
            RX_CheckResponse_DTC_SID19_02(0x01, misMsgDTCList[index].dtc, 0);

            //step8
            TestStep("Resume the simulation of signalgroup VehModMngGlbSafe,and wait for 7s");

            AppWait(7000, " ");

            //step10
            TestStep(" wait for 127/stepupvalue*TestPeriod*1.2+5s,then read DTC with status mask 0x01");
            time = ceil(127 / misMsgDTCList[index].SUValue) * misMsgDTCList[index].testProd * 1.2 + 5000;
            AppWait(time, "");
            RX_SID19_ReportDTCByStatusMask(0x01);
            RX_CheckResponse_DTC_SID19_02(0x01, misMsgDTCList[index].dtc, 1);
            CancelMissMsg(index);
            index_cc2 = 0;

        }
        index_cc2 = 0;
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

s32 TestCase_1022898_PermanentmissingframeDTCwhenthemissingframenodeisnotconfigured()
{
    int index, j;
    double time;
    u8 dtc[3] = { 0xff,0xff,0xff };
    char StepDespriction[256];
    carConfigType = MissFrame;

    strcpy(TestCaseName, " TestCase_1022898_PermanentmissingframeDTCwhenthemissingframenodeisnotconfigured");
    strcpy(TestScopeName, "Permanent Missing Frame DTC");
    strcpy(testPurpose_CH, "测试目的：检查当TRC不满足时，是否记录通信丢失DTC");

    if (0 != TestPrecondition())
        return -1;
    //TestStep("上电并唤醒被测件");
    //if (0 != CheckCommunication(5000)) {
    //    TestPostCondition();
    //    return -1;
    //}

    try
    {
        for (index = 0; index < MISMSG_DTC_NUM; index++)
        {
            stepnumber = 0;
            //Step1
            if (misMsgDTCList[index].tstRsltFlg == 0)
            {
                continue;
            }
            if (misMsgDTCList[index].CCPPsn20 == 0)
            {
                snprintf(StepDespriction, sizeof(StepDespriction), "There is no CCP value of DTC 0x%X. Skip this DTC.", misMsgDTCList[index].dtc);
                TestReport_HTML_StepDescription(StepDespriction);
                continue;
            }
            snprintf(StepDespriction, sizeof(StepDespriction), "-------------------------Test missing node %s:0x%X---------------------------", misMsgDTCList[index].node, misMsgDTCList[index].dtc);
            TestReport_HTML_Step(StepNumber, StepDespriction, Info);
            for (int k = 0; k < 600; k++)
            {
                gCCPDataExt[k] = 0;
                gCCPData[k] = 0;
            }
            FulfilTRC();
            TRCSwitch(0x0, 0xD, 0x0);

            u8 temp = 6 % 4;
            
            TestReport_HTML_StepDescription("Simulate ListOfNode or CCP value, which does not configure the test missing frame node", Info);
            
            simulateCCP(false);

            index_cc2 = 0;

            //Step2
            TestStep("Simulate related messages of test DTC to the bus,and wait for 7s");
            //snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
            //    misMsgDTCList[index].misMsgList->msgID, misMsgDTCList[index].misMsgList->misMsgSignalList->sigName);
            snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
                g_MissFrame_path[index].FFrameIndentifier, misMsgDTCList[index].signalResult->signalName.c_str());
            TestReport_HTML_StepDescription(StepDespriction, Info);
            SimMissMsg(index);                           //仿真missMsg
            AppWait(7000, "");

            //Step3
            TestStep("Clear DTCs with diagnostic request, then wait for 127/stepupvalue*TestPeriod*1.2+5s.");
            time = ceil(127 / misMsgDTCList[index].SUValue) * misMsgDTCList[index].testProd * 1.2 + 5000;
            SID14_ClearAllDTC();
            CheckPosResponse_SID14();
            AppWait(time, "");

            //step4
            TestStep("Read DTC from the DUT with status mask 0x09.");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, misMsgDTCList[index].dtc, 0);

            //step5
            TestStep("Stop the simulation of a missing frame DTC related messages.");
            //snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
            //    misMsgDTCList[index].misMsgList->msgID, misMsgDTCList[index].misMsgList->misMsgSignalList->sigName);
            snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
                g_MissFrame_path[index].FFrameIndentifier, misMsgDTCList[index].signalResult->signalName.c_str());
            TestReport_HTML_StepDescription(StepDespriction, Info);
            CancelMissMsg(index);

            //step6
            TestStep("wait for 127/stepupvalue*TestPeriod*1.2+5s.");
            time = ceil(127 / misMsgDTCList[index].SUValue) * misMsgDTCList[index].testProd * 1.2 + 5000;
            AppWait(time, "");

            //step7
            TestStep("Read DTC from the DUT with status mask 0x01.");
            RX_SID19_ReportDTCByStatusMask(0x01);
            RX_CheckResponse_DTC_SID19_02(0x01, misMsgDTCList[index].dtc, 0);

            com.del_cyclic_messages();
            KL30(off);
            AppWait(2000, " ");

            //step8
            TestStep("Reset the power supply. Simulate CCP values which configure the test missing message node, and wait for 7s.");          //有点问题
            SetVoltageAndRead(12);


            KL30(on);
            TestWait(500);
            KL15(on);
            SendTesterPresent(TP_ON);
            NMWakeUp(1);
            AppWait(1000, "");
            FulfilTRC();
            TRCSwitch(0x0, 0xD, 0x0);

            TestReport_HTML_StepDescription("Start Simulate CCP", Info);
            simulateCCP(true);
            //SimMissMsg(index);
            AppWait(7000, "");

            //step9
            TestStep("wait for 127/stepupvalue*TestPeriod*1.2+5s");
            time = ceil(127 / misMsgDTCList[index].SUValue) * misMsgDTCList[index].testProd * 1.2 + 5000;
            AppWait(time, "");

            //step10
            TestStep("Read DTC from the DUT with status mask 0x01.");
            RX_SID19_ReportDTCByStatusMask(0x01);
            RX_CheckResponse_DTC_SID19_02(0x01, misMsgDTCList[index].dtc, 1);
            CancelMissMsg(index);
            index_cc2 = 0;

        }
        index_cc2 = 0;
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

s32 TestCase_1022885_PermanentmissingframeDTCInLongTermMemoryTest()
{
    int index, j;
    double time;
    int snapShot;
    u8 fdtc[3];
    u8 dtc[3] = { 0xff,0xff,0xff };
    char StepDespriction[256];
    carConfigType = MissFrame;

    strcpy(TestCaseName, " TestCase_1022885_PermanentmissingframeDTCInLongTermMemoryTest");
    strcpy(TestScopeName, "Permanent Missing Frame DTC");
    strcpy(testPurpose_CH, "测试目的：检查当TRC不满足时，是否记录通信丢失DTC");
    if (0 != TestPrecondition())
        return -1;
    //TestStep("上电并唤醒被测件");
    //if (0 != CheckCommunication(5000)) {
    //    TestPostCondition();
    //    return -1;
    //}
    try
    {
        //Step1

        for (index = 0; index < MISMSG_DTC_NUM; index++)
        {
            if (misMsgDTCList[index].tstRsltFlg == 0) {
                continue;
            }
            snprintf(StepDespriction, sizeof(StepDespriction), "-------------------------Test missing node %s:0x%X---------------------------", misMsgDTCList[index].node, misMsgDTCList[index].dtc);
            TestReport_HTML_StepDescription(StepDespriction, Info);
            for (int k = 0; k < 600; k++)
            {
                gCCPDataExt[k] = 0;
                gCCPData[k] = 0;

            }
            app.log_text("start TRC", lvlInfo);
            FulfilTRC();
            TRCSwitch(0x0, 0xD, 0x0);

            TestReport_HTML_StepDescription("Start Simulate CCP", Info);
            simulateCCP();

            TestReport_HTML_StepDescription("Simulate related messages of test DTC to the bus", Info);
            //snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
            //    misMsgDTCList[index].misMsgList->msgID, misMsgDTCList[index].misMsgList->misMsgSignalList->sigName);
            snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
                g_MissFrame_path[index].FFrameIndentifier, misMsgDTCList[index].signalResult->signalName.c_str());
            TestReport_HTML_StepDescription(StepDespriction, Info);
            SimMissMsg(index);                           //仿真missMsg

            //Step2
            TestStep("wait for 7s");
            AppWait(7000, "");

            //Step3
            TestStep("Clear DTCs with diagnostic request, then wait for 127/stepupvalue*TestPeriod*1.2+5s.");
            time = ceil(127 / misMsgDTCList[index].SUValue) * misMsgDTCList[index].testProd * 1.2 + 5000;
            SID14_ClearAllDTC();
            CheckPosResponse_SID14();
            AppWait(time, "");

            //step4
            TestStep("Read DTC from the DUT with status mask 0x09.");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, misMsgDTCList[index].dtc, 0);

            //step5
            TestStep("Stop the simulation of a missing frame DTC related messages.wait for 127/stepupvalue*TestPeriod*1.2+5s.");
            //snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
            //    misMsgDTCList[index].misMsgList->msgID, misMsgDTCList[index].misMsgList->misMsgSignalList->sigName);
            snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
                g_MissFrame_path[index].FFrameIndentifier, misMsgDTCList[index].signalResult->signalName.c_str());
            TestReport_HTML_StepDescription(StepDespriction, Info);
            CancelMissMsg(index);
            time = ceil(127 / misMsgDTCList[index].SUValue) * misMsgDTCList[index].testProd * 1.2 + 5000;
            AppWait(time, "");

            //step6
            TestStep("Read DTC from the DUT with status mask 0x09.");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, misMsgDTCList[index].dtc, 1);                                                      //取消报文仿真

            //step7
            TestStep("Meet the conditions for DUT to store DTC information in the long_term memory. Let DUT go to deep_sleep");
            //UsgModSwtich(0x1);
            //EPLvlSwitch(0x0);
            //AppWait(1000, "");
            //SendTesterPresent(TP_OFF);
            //NMWakeUp(0);
            //com.del_cyclic_messages();
            //KL15(off);
            //AppWait(2000, "");
            //TestWaitForCurInRange(gLowPwrCur, gSleepTime);
            NM_NextOperation();

            //step8
            TestStep("Set the power supply off, and wait for 10s.");
            KL30(off);
            AppWait(10000, "");

            //step9
            TestStep("Set the power supply on and wake up DUT.");
            SetVoltageAndRead(12);
            KL30(on);
            KL15(on);
            NMWakeUp(1);
            SendTesterPresent(TP_ON);
            FulfilTRC();
            AppWait(1000, "");
            TRCSwitch(0x0, 0xD, 0x0);

            //step10
            TestStep("Simulate related messages of test DTC to the bus. Wait for 10s");                                  //仿真对否
            //snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
            //    misMsgDTCList[index].misMsgList->msgID, misMsgDTCList[index].misMsgList->misMsgSignalList->sigName);
            snprintf(StepDespriction, sizeof(StepDespriction), "Message 0x%X, Signal %s",
                g_MissFrame_path[index].FFrameIndentifier, misMsgDTCList[index].signalResult->signalName.c_str());
            TestReport_HTML_StepDescription(StepDespriction, Info);
            SimMissMsg(index);
            AppWait(10000, "");

            //step11
            TestStep("Read DTC from the DUT with mask 0x2F");
            RX_SID19_ReportDTCByStatusMask(0x2F);
            RX_CheckResponse_DTC_SID19_02(0x2F, misMsgDTCList[index].dtc, 1);

            //step12
            if (gAsrVersion < 4.1)
            {
                strcpy(StepDespriction, "Read snapshot record of test DTC by DTCSnapshotRecordNumber 0x20");
                TestReport_HTML_Step(StepNumber, StepDespriction, Info);
                fdtc[0] = misMsgDTCList[index].dtc >> 16;
                fdtc[1] = misMsgDTCList[index].dtc >> 8;
                fdtc[2] = misMsgDTCList[index].dtc;
                snapShot = ReadSnapShotBy20(fdtc);
                if (snapShot == 1)
                {
                    strcpy(StepDespriction, "Find snapshot in the response.");
                    TestReport_HTML_Step(StepNumber, StepDespriction, Pass);
                }
                else
                {
                    strcpy(StepDespriction, "No snapshot in the response.");
                    TestReport_HTML_Step(StepNumber, StepDespriction, Fail);
                }
            }
            CancelMissMsg(index);
            index_cc2 = 0;
            break;
        }
        if (index == MISMSG_DTC_NUM)
        {
            strcpy(StepDespriction, "All DTC is abnormal, break the test.");
            TestReport_HTML_Step(StepNumber, StepDespriction, Fail);
        }
        index_cc2 = 0;
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

s32 TestCase_1022884_PermanentmissingframeDTC()
{
    int index = 0, j = 0;
    double time;
    u8 dtc[3] = { 0xff,0xff,0xff };
    char StepDespriction[256];

    strcpy(TestCaseName, "TestCase_1022884_PermanentmissingframeDTC");
    strcpy(TestScopeName, "Permanent Missing Frame DTC");
    strcpy(testPurpose_CH, "测试目的：检查当UsageMode切换的TRC不满足时,是否记录CAN circuit error DTC");
    if (0 != TestPrecondition())
        return -1;
    //TestStep("上电并唤醒被测件");
    //if (0 != CheckCommunication(5000)) {
    //    TestPostCondition();
    //    return -1;
    //}
    try
    {
        carConfigType = MissFrame;
        //Step1
        for (index = 0; index < MISMSG_DTC_NUM; index++)
        {
            index_cc2 = 0;
            index_cc1 = 0;
            snprintf(StepDespriction, sizeof(StepDespriction), "-------------------------Test missing node %s:0x%X---------------------------", misMsgDTCList[index].node, misMsgDTCList[index].dtc);
            TestReport_HTML_StepDescription(StepDespriction, Info);

            FulfilTRC();
            TRCSwitch(0x0, 0xD, 0x0);

            TestReport_HTML_StepDescription("Start Simulate CCP", Info);

            simulateCCP();
            TestReport_HTML_StepDescription("star the simulation of test DTC related messages");
            //snprintf(StepDespriction, sizeof(StepDespriction), "%s, Message 0x%X, Signal %s",misMsgDTCList[index].node, 
            //    misMsgDTCList[index].misMsgList->msgID, misMsgDTCList[index].misMsgList->misMsgSignalList->sigName);
            snprintf(StepDespriction, sizeof(StepDespriction), "%s, Message 0x%X, Signal %s", misMsgDTCList[index].node,
                g_MissFrame_path[index].FFrameIndentifier, misMsgDTCList[index].signalResult->signalName.c_str());
            TestReport_HTML_StepDescription(StepDespriction, Info);
            SimMissMsg(index);                           //仿真missMsg，可改仿真的通道

            //Step2
            TestStep("wait for 7s");
            AppWait(7000, "");

            //Step3
            TestStep("Clear DTCs with diagnostic request, then wait for 127/stepupvalue*TestPeriod*1.2+5s.");
            time = ceil(127 / misMsgDTCList[index].SUValue) * misMsgDTCList[index].testProd * 1.2 + 5000;
            SID14_ClearAllDTC();
            CheckPosResponse_SID14();
            AppWait(time, "");

            //step4
            TestStep("Read DTC from the DUT with status mask 0x09.");
            RX_SID19_ReportDTCByStatusMask(0x09);
            if (0 != RX_CheckResponse_DTC_SID19_02(0x09, misMsgDTCList[index].dtc, 0)) {
                misMsgDTCList[index].tstRsltFlg = 0;
            }
            //RX_CheckResponse_DTC_SID19_02(0x01, misMsgDTCList[index].dtc, 0);

            //step5
            TestStep("Stop the simulation of a missing frame DTC related messages.");
            //snprintf(StepDespriction, sizeof(StepDespriction), "%s, Message 0x%X, Signal %s", misMsgDTCList[index].node,
            //    misMsgDTCList[index].misMsgList->msgID, misMsgDTCList[index].misMsgList->misMsgSignalList->sigName);
            snprintf(StepDespriction, sizeof(StepDespriction), "%s, Message 0x%X, Signal %s", misMsgDTCList[index].node,
                g_MissFrame_path[index].FFrameIndentifier, misMsgDTCList[index].signalResult->signalName.c_str());
            TestReport_HTML_StepDescription(StepDespriction, Info);
            CancelMissMsg(index);

            //step6
            TestStep("wait for 127/stepupvalue*TestPeriod*0.8.");
            time = ceil(127 / misMsgDTCList[index].SUValue) * misMsgDTCList[index].testProd * 0.8;
            AppWait(time, "");

            //step7
            TestStep("Read DTC from the DUT with status mask 0x01.");
            RX_SID19_ReportDTCByStatusMask(0x01);
            if (0 != RX_CheckResponse_DTC_SID19_02(0x01, misMsgDTCList[index].dtc, 0)) {
                misMsgDTCList[index].tstRsltFlg = 0;
            }
            //RX_CheckResponse_DTC_SID19_02(0x01, misMsgDTCList[index].dtc, 0);

            //step8
            TestStep("wait for 127/stepupvalue*TestPeriod*0.4.");
            time = ceil(127 / misMsgDTCList[index].SUValue) * misMsgDTCList[index].testProd * 0.4;
            AppWait(time, "");

            //step9
            TestStep("Read DTC from the DUT with status mask 0x01.");
            RX_SID19_ReportDTCByStatusMask(0x01);
            if (0 != RX_CheckResponse_DTC_SID19_02(0x01, misMsgDTCList[index].dtc, 1)) {
                misMsgDTCList[index].tstRsltFlg = 0;
            }
            //RX_CheckResponse_DTC_SID19_02(0x01, misMsgDTCList[index].dtc, 1);

            //step10
            TestStep("Resume the simulation of test DTC related messages");
            SimMissMsg(index);

            //step11
            TestStep("wait for 128/stepupvalue*TestPeriod*1.2.");
            time = ceil(128 / misMsgDTCList[index].SUValue) * misMsgDTCList[index].testProd * 1.2;
            AppWait(time, "");

            //step12
            TestStep("Read DTC from the DUT with status mask 0x01.");
            RX_SID19_ReportDTCByStatusMask(0x01);
            if (0 != RX_CheckResponse_DTC_SID19_02(0x01, misMsgDTCList[index].dtc, 0)) {
                misMsgDTCList[index].tstRsltFlg = 0;
            }
            //RX_CheckResponse_DTC_SID19_02(0x01, misMsgDTCList[index].dtc, 0);
            CancelMissMsg(index);
            index_cc2 = 0;
            stepnumber = 1;
        }

        index_cc2 = 0;
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

s32 TestCase_1022886_FDC10testofPermanentmissingframeDTC()
{
    int index, j;
    double time;
    u8 fdtc[3];
    char StepDespriction[256];

    strcpy(TestCaseName, " TestCase_1022886_FDC10testofPermanentmissingframeDTC");
    strcpy(TestScopeName, "Permanent Missing Frame DTC");
    strcpy(testPurpose_CH, "测试目的：检查通信丢失DTC的FDC10");
    if (0 != TestPrecondition())
        return -1;
    //TestStep("上电并唤醒被测件");
    //if (0 != CheckCommunication(5000)) {
    //    TestPostCondition();
    //    return -1;
    //}
    TCANFD CCNetWorkMsg = { NetworkCluster[CCPConfigTemp[2].cluster],1,15,1,CCPConfigTemp[2].slotID,0,{0x0,0x80,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00} };
    try
    {
        //Step1

        for (index = 0; index < 1; index++)
        {
            snprintf(StepDespriction, sizeof(StepDespriction), "-------------------------Test missing node %s:0x%X---------------------------", misMsgDTCList[index].node, misMsgDTCList[index].dtc);
            TestReport_HTML_StepDescription(StepDespriction, Info);
            for (int k = 0; k < 600; k++)
            {
                gCCPDataExt[k] = 0;
                gCCPData[k] = 0;


            }
            FulfilTRC();
            TRCSwitch(0x0, 0xD, 0x0);
            TestReport_HTML_StepDescription("Start Simulate CCP", Info);
            simulateCCP();

            fdtc[0] = (misMsgDTCList[index].dtc & 0xff0000) >> 16;
            fdtc[1] = (misMsgDTCList[index].dtc & 0xff00) >> 8;
            fdtc[2] = (misMsgDTCList[index].dtc & 0xff);
            TestReport_HTML_StepDescription("Simulate related messages");
            //snprintf(StepDespriction, sizeof(StepDespriction), "%s, Message 0x%X, Signal %s", misMsgDTCList[index].node,
            //    misMsgDTCList[index].misMsgList->msgID, misMsgDTCList[index].misMsgList->misMsgSignalList->sigName);
            snprintf(StepDespriction, sizeof(StepDespriction), "%s, Message 0x%X, Signal %s", misMsgDTCList[index].node,
                g_MissFrame_path[index].FFrameIndentifier, misMsgDTCList[index].signalResult->signalName.c_str());
            TestReport_HTML_StepDescription(StepDespriction, Info);
            SimMissMsg(index);                           //仿真missMsg

            //Step2
            TestStep("wait for 7s");
            AppWait(7000, "");

            //Step3
            TestStep("Clear DTCs with diagnostic request, then wait for 127/stepupvalue*TestPeriod*1.2+5s.");
            time = ceil(127 / misMsgDTCList[index].SUValue) * misMsgDTCList[index].testProd * 1.2 + 5000;
            SID14_ClearAllDTC();
            CheckPosResponse_SID14();
            AppWait(time, "");

            //step4
            TestStep("Read DTC from the DUT with status mask 0x09.");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, misMsgDTCList[index].dtc, 0);

            //step5
            TestReport_HTML_StepDescription("Stop the communication simulation of related messages.");
            //snprintf(StepDespriction, sizeof(StepDespriction), "%s, Message 0x%X, Signal %s", misMsgDTCList[index].node,
            //    misMsgDTCList[index].misMsgList->msgID, misMsgDTCList[index].misMsgList->misMsgSignalList->sigName);
            snprintf(StepDespriction, sizeof(StepDespriction), "%s, Message 0x%X, Signal %s", misMsgDTCList[index].node,
                g_MissFrame_path[index].FFrameIndentifier, misMsgDTCList[index].signalResult->signalName.c_str());
            TestReport_HTML_StepDescription(StepDespriction, Info);
            CancelMissMsg(index);

            //step6
            TestStep("Send diagnostic request with TestPeriod to read FDC10,until FDC10 is 0x7F");
            gUnCofrmLmt = misMsgDTCList[index].UnCofrmLmt;
            CheckStepUpValue(fdtc, misMsgDTCList[index].testProd, misMsgDTCList[index].SUValue, 0);

            //step7
            TestStep("Resume the simulation of test DTC related messages");
            //snprintf(StepDespriction, sizeof(StepDespriction), "%s, Message 0x%X, Signal %s", misMsgDTCList[index].node,
            //    misMsgDTCList[index].misMsgList->msgID, misMsgDTCList[index].misMsgList->misMsgSignalList->sigName);
            snprintf(StepDespriction, sizeof(StepDespriction), "%s, Message 0x%X, Signal %s", misMsgDTCList[index].node,
                g_MissFrame_path[index].FFrameIndentifier, misMsgDTCList[index].signalResult->signalName.c_str());
            TestReport_HTML_StepDescription(StepDespriction, Info);
            SimMissMsg(index);

            //step8
            TestStep("Send diagnostic request with TestPeriod to read FDC10");
            CheckStepDownValue(fdtc, misMsgDTCList[index].testProd, misMsgDTCList[index].SUValue);
            CancelMissMsg(index);
            index_cc2 = 0;
        }

        index_cc2 = 0;
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

s32 TestCase_1022887_UnconfirmedDTCLimittestofPermanentmissingframeDTC()
{
    int index, j;
    double time;
    u8 fdtc[3];
    int FDC10;
    int snapShot;
    char StepDespriction[256];
    int unCfmBit;
    int unCfmSncLstClr;
    int unCfmThisOpCyc;

    strcpy(TestCaseName, "TestCase_1022887_UnconfirmedDTCLimittestofPermanentmissingframeDTC");
    strcpy(TestScopeName, "Permanent Missing Frame DTC");
    strcpy(testPurpose_CH, "测试目的：检查通信丢失DTC的UnconfirmedDTCLimit");

    if (0 != TestPrecondition())
        return -1;
    //TestStep("上电并唤醒被测件");
    //if (0 != CheckCommunication(5000)) {
    //    TestPostCondition();
    //    return -1;
    //}
    try
    {
        //Step1
        for (index = 0; index < 1; index++)
        {
            snprintf(StepDespriction, sizeof(StepDespriction), "-------------------------Test missing node %s:0x%X---------------------------", misMsgDTCList[index].node, misMsgDTCList[index].dtc);
            TestReport_HTML_StepDescription(StepDespriction, Info);

            for (int k = 0; k < 600; k++)
            {
                gCCPDataExt[k] = 0;
                gCCPData[k] = 0;
                gCCPDataExt2[k] = 0;
            }

            FulfilTRC();
            TRCSwitch(0x0, 0xD, 0x0);
            TestReport_HTML_StepDescription("Start Simulate CCP", Info);
            simulateCCP();

            fdtc[0] = (misMsgDTCList[index].dtc & 0xff0000) >> 16;
            fdtc[1] = (misMsgDTCList[index].dtc & 0xff00) >> 8;
            fdtc[2] = (misMsgDTCList[index].dtc & 0xff);

            TestReport_HTML_StepDescription("Simulate related messages");
            //snprintf(StepDespriction, sizeof(StepDespriction), "%s, Message 0x%X, Signal %s", misMsgDTCList[index].node,
            //    misMsgDTCList[index].misMsgList->msgID, misMsgDTCList[index].misMsgList->misMsgSignalList->sigName);
            snprintf(StepDespriction, sizeof(StepDespriction), "%s, Message 0x%X, Signal %s", misMsgDTCList[index].node,
                g_MissFrame_path[index].FFrameIndentifier, misMsgDTCList[index].signalResult->signalName.c_str());
            TestReport_HTML_StepDescription(StepDespriction, Info);
            SimMissMsg(index);                           //仿真missMsg

            //Step2
            TestStep("wait for 7s");
            AppWait(7000, "");

            //Step3
            TestStep("Clear DTCs with diagnostic request, then wait for 127/stepupvalue*TestPeriod*1.2+5s.");
            time = ceil(127 / misMsgDTCList[index].SUValue) * misMsgDTCList[index].testProd * 1.2 + 5000;
            SID14_ClearAllDTC();
            CheckPosResponse_SID14();
            AppWait(time, "");

            //step4
            TestStep("Read DTC from the DUT with status mask 0x09.");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, misMsgDTCList[index].dtc, 0);

            //step5
            TestStep("Read snapshot record of test DTC by SnapshotRecordNo");
            unCfmBit = ReadSnapShot(fdtc);
            if (unCfmBit == 0)
            {
                strcpy(issuereason, "No snapshot record in the response.");
                TestReport_HTML_StepDescription("No snapshot record in the response.", Pass);
            }
            else
            {
                strcpy(issuereason, "Find snapshot record in the response.");
                TestReport_HTML_StepDescription("Find snapshot record in the response.", Fail);
            }
            ;
            //Step6
            if (gAsrVersion < 4.1)
            {
                TestStep("read UnconfirmedDTC, UnconfirmedDTCSinceLastClear and UnconfirmedDTCThisOperationCycle of test DTC");
                if (ReadUncofirmedDTC(fdtc) == 0)
                {
                    snprintf(StepDespriction, sizeof(StepDespriction), "UnconfirmedDTC is 0");
                    TestReport_HTML_StepDescription(StepDespriction, Pass);
                }
                else
                {
                    snprintf(StepDespriction, sizeof(StepDespriction), "UnconfirmedDTC is 1");
                    TestReport_HTML_StepDescription(StepDespriction, Fail);
                }
                if (ReadUncnfrmDTCSncLstClr(fdtc) == 0)
                {
                    snprintf(StepDespriction, sizeof(StepDespriction), "UnconfirmedDTCSinceLastClear is 0");
                    TestReport_HTML_StepDescription(StepDespriction, Pass);
                }
                else
                {
                    snprintf(StepDespriction, sizeof(StepDespriction), "UnconfirmedDTCSinceLastClear is 1");
                    TestReport_HTML_StepDescription(StepDespriction, Fail);
                }
                if (ReadUncnfrmDTCThisOpCyc(fdtc) == 0)
                {
                    snprintf(StepDespriction, sizeof(StepDespriction), "UnconfirmedDTCThisOperationCycle is 0");
                    TestReport_HTML_StepDescription(StepDespriction, Pass);
                }
                else
                {
                    snprintf(StepDespriction, sizeof(StepDespriction), "UnconfirmedDTCThisOperationCycle is 1");
                    TestReport_HTML_StepDescription(StepDespriction, Fail);
                }
            }

            //step7
            TestStep("Stop the simulation of a missing frame DTC related messages.");
            //snprintf(StepDespriction, sizeof(StepDespriction), "%s, Message 0x%X, Signal %s", misMsgDTCList[index].node,
            //    misMsgDTCList[index].misMsgList->msgID, misMsgDTCList[index].misMsgList->misMsgSignalList->sigName);
            snprintf(StepDespriction, sizeof(StepDespriction), "%s, Message 0x%X, Signal %s", misMsgDTCList[index].node,
                g_MissFrame_path[index].FFrameIndentifier, misMsgDTCList[index].signalResult->signalName.c_str());
            TestReport_HTML_StepDescription(StepDespriction, Info);
            CancelMissMsg(index);

            //step8
            TestStep("Send diagnostic request with testProd to read FDC10 and Snapshot data, until FDC10 >= UnconfirmedDTCLimit");
            for (j = 0; j < 50; j++)
            {
                AppWait(misMsgDTCList[index].testProd, "");
                snprintf(StepDespriction, sizeof(StepDespriction), "---------------------------The %dth Cycle test----------------------------", j + 1);
                TestReport_HTML_StepDescription(StepDespriction, Info);
                FDC10 = ConvertUnsgnToSgn(ReadFDC10(fdtc));
                snapShot = ReadSnapShot(fdtc);
                if (gAsrVersion < 4.1)
                {
                    unCfmBit = ReadUncofirmedDTC(fdtc);
                    unCfmSncLstClr = ReadUncnfrmDTCSncLstClr(fdtc);
                    unCfmThisOpCyc = ReadUncnfrmDTCThisOpCyc(fdtc);
                }


                if (FDC10 >= misMsgDTCList[index].UnCofrmLmt)
                {
                    if (snapShot == 0)
                    {
                        strcpy(StepDespriction, "No snapshot record in the response.");
                        TestReport_HTML_StepDescription(StepDespriction, Fail);
                    }
                    else
                    {
                        strcpy(StepDespriction, "Find snapshot record in the response.");
                        TestReport_HTML_StepDescription(StepDespriction, Pass);
                    }
                    if (gAsrVersion < 4.1)
                    {
                        if (unCfmBit == 0x1)
                        {
                            snprintf(StepDespriction, sizeof(StepDespriction), "UnconfirmedDTC is set to 1 when FDC10 is %d", FDC10);
                            TestReport_HTML_StepDescription(StepDespriction, Pass);
                        }
                        else
                        {
                            snprintf(StepDespriction, sizeof(StepDespriction), "UnconfirmedDTC is not set to 1 when FDC10 is %d", FDC10);
                            TestReport_HTML_StepDescription(StepDespriction, Fail);
                        }

                        if (unCfmSncLstClr == 0x1)
                        {
                            snprintf(StepDespriction, sizeof(StepDespriction), "UnconfirmedDTCSinceLastClear is set to 1 when FDC10 is %d", FDC10);
                            TestReport_HTML_StepDescription(StepDespriction, Pass);
                        }
                        else
                        {
                            snprintf(StepDespriction, sizeof(StepDespriction), "UnconfirmedDTCSinceLastClear is not set to 1 when FDC10 is %d", FDC10);
                            TestReport_HTML_StepDescription(StepDespriction, Fail);
                        }
                        if (unCfmThisOpCyc == 0x1)
                        {
                            snprintf(StepDespriction, sizeof(StepDespriction), "UnconfirmedDTCThisOperationCycle is set to 1 when FDC10 is %d", FDC10);
                            TestReport_HTML_StepDescription(StepDespriction, Pass);
                        }
                        else
                        {
                            snprintf(StepDespriction, sizeof(StepDespriction), "UnconfirmedDTCThisOperationCycle is not set to 1 when FDC10 is %d", FDC10);
                            TestReport_HTML_StepDescription(StepDespriction, Fail);
                        }
                    }
                    break;
                }
                else
                {
                    if (snapShot == 0)
                    {
                        strcpy(StepDespriction, "No snapshot record in the response.");
                        TestReport_HTML_StepDescription(StepDespriction, Pass);
                    }
                    else
                    {
                        strcpy(StepDespriction, "Find snapshot record in the response.");
                        TestReport_HTML_StepDescription(StepDespriction, Fail);
                    }
                    if (gAsrVersion < 4.1)
                    {
                        if (unCfmBit == 0x1)
                        {
                            snprintf(StepDespriction, sizeof(StepDespriction), "UnconfirmedDTC is set to 1 when FDC10 < UnconfirmedDTCLimit");
                            TestReport_HTML_StepDescription(StepDespriction, Fail);
                        }
                        else
                        {
                            snprintf(StepDespriction, sizeof(StepDespriction), "UnconfirmedDTC is not set to 1 when FDC10 < UnconfirmedDTCLimit");
                            TestReport_HTML_StepDescription(StepDespriction, Pass);
                        }
                        if (unCfmSncLstClr == 0x1)
                        {
                            snprintf(StepDespriction, sizeof(StepDespriction), "UnconfirmedDTCSinceLastClear is set to 1 when FDC10 is %d", FDC10);
                            TestReport_HTML_StepDescription(StepDespriction, Fail);
                        }
                        else
                        {
                            snprintf(StepDespriction, sizeof(StepDespriction), "UnconfirmedDTCSinceLastClear is not set to 1 when FDC10 is %d", FDC10);
                            TestReport_HTML_StepDescription(StepDespriction, Pass);
                        }
                        if (unCfmThisOpCyc == 0x1)
                        {
                            snprintf(StepDespriction, sizeof(StepDespriction), "UnconfirmedDTCThisOperationCycle is set to 1 when FDC10 is %d", FDC10);
                            TestReport_HTML_StepDescription(StepDespriction, Fail);
                        }
                        else
                        {
                            snprintf(StepDespriction, sizeof(StepDespriction), "UnconfirmedDTCThisOperationCycle is not set to 1 when FDC10 is %d", FDC10);
                            TestReport_HTML_StepDescription(StepDespriction, Pass);
                        }
                    }
                }
            }
            //step9
            TestStep(" simulation of test DTC related messages");
            //snprintf(StepDespriction, sizeof(StepDespriction), "%s, Message 0x%X, Signal %s", misMsgDTCList[index].node,
            //    misMsgDTCList[index].misMsgList->msgID, misMsgDTCList[index].misMsgList->misMsgSignalList->sigName);
            snprintf(StepDespriction, sizeof(StepDespriction), "%s, Message 0x%X, Signal %s", misMsgDTCList[index].node,
                g_MissFrame_path[index].FFrameIndentifier, misMsgDTCList[index].signalResult->signalName.c_str());
            TestReport_HTML_StepDescription(StepDespriction, Info);
            SimMissMsg(index);

            //step10
            TestStep("wait for 5s,read UnconfirmedDTC, UnconfirmedDTCSinceLastClear and UnconfirmedDTCThisOperationCycle of test DTC");
            snprintf(StepDespriction, sizeof(StepDespriction), "Expect UnconfirmedDTCSinceLastClear is 0,UnconfirmedDTCThisOperationCycle is 1");
            TestReport_HTML_StepDescription(StepDespriction, Info);
            AppWait(5000, "");
            if (ReadUncofirmedDTC(fdtc) == 0)
            {
                snprintf(StepDespriction, sizeof(StepDespriction), "UnconfirmedDTC is 0");
                TestReport_HTML_StepDescription(StepDespriction, Pass);
            }
            else
            {
                snprintf(StepDespriction, sizeof(StepDespriction), "UnconfirmedDTC is 1");
                TestReport_HTML_StepDescription(StepDespriction, Fail);
            }
            if (ReadUncnfrmDTCThisOpCyc(fdtc) == 0)
            {
                snprintf(StepDespriction, sizeof(StepDespriction), "UnconfirmedDTCThisOperationCycle is 0");
                TestReport_HTML_StepDescription(StepDespriction, Fail);
            }
            else
            {
                snprintf(StepDespriction, sizeof(StepDespriction), "UnconfirmedDTCThisOperationCycle is 1");
                TestReport_HTML_StepDescription(StepDespriction, Pass);
            }

            //step11
            TestStep("Let the DUT go to the next operation cycle.simulation of test DTC related messages");
            NM_NextOperation();
            SimMissMsg(index);


            //step12
            TestStep("Wait for 10s, then read UnconfirmedDTCSinceLastClear and UnconfirmedDTCThisOperationCycle of test DTC");
            snprintf(StepDespriction, sizeof(StepDespriction), "Expect UnconfirmedDTCSinceLastClear is 1,UnconfirmedDTCThisOperationCycle is 0");
            TestReport_HTML_StepDescription(StepDespriction, Info);
            if (gAsrVersion < 4.1)
            {
                if (ReadUncnfrmDTCSncLstClr(fdtc) == 0)
                {
                    snprintf(StepDespriction, sizeof(StepDespriction), "UnconfirmedDTCSinceLastClear is 0");
                    TestReport_HTML_StepDescription(StepDespriction, Fail);
                }
                else
                {
                    snprintf(StepDespriction, sizeof(StepDespriction), "UnconfirmedDTCSinceLastClear is 1");
                    TestReport_HTML_StepDescription(StepDespriction, Pass);
                }
                if (ReadUncnfrmDTCThisOpCyc(fdtc) == 0)
                {
                    snprintf(StepDespriction, sizeof(StepDespriction), "UnconfirmedDTCThisOperationCycle is 0");
                    TestReport_HTML_StepDescription(StepDespriction, Pass);
                }
                else
                {
                    snprintf(StepDespriction, sizeof(StepDespriction), "UnconfirmedDTCThisOperationCycle is 1");
                    TestReport_HTML_StepDescription(StepDespriction, Fail);
                }
            }
            TestReport_HTML_StepDescription("Stop the simulation of a missing frame DTC related messages.");
            //snprintf(StepDespriction, sizeof(StepDespriction), "%s, Message 0x%X, Signal %s", misMsgDTCList[index].node,
            //    misMsgDTCList[index].misMsgList->msgID, misMsgDTCList[index].misMsgList->misMsgSignalList->sigName);
            snprintf(StepDespriction, sizeof(StepDespriction), "%s, Message 0x%X, Signal %s", misMsgDTCList[index].node,
                g_MissFrame_path[index].FFrameIndentifier, misMsgDTCList[index].signalResult->signalName.c_str());
            TestReport_HTML_StepDescription(StepDespriction, Info);
            CancelMissMsg(index);
            index_cc2 = 0;

        }
        index_cc2 = 0;
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

s32 TestCase_1022888_ConfirmedDTCandDTCAgedLimittestofPermanentmissingframeDTC()                      //有问题
{
    int index, j = 0;
    double time;
    u8 dtc[3] = { 0xff,0xff,0xff };
    char StepDespriction[256];
    int occ6, occ2;
    u8 fdtc[3];

    strcpy(TestCaseName, " TestCase_1022888_ConfirmedDTCandDTCAgedLimittestofPermanentmissingframeDTC");
    strcpy(TestScopeName, "Permanent Missing Frame DTC");
    strcpy(testPurpose_CH, "测试目的：检查通信丢失DTC的ConfirmedDTCLimit和DTCAgedLimit");

    if (0 != TestPrecondition())
        return -1;
    //TestStep("上电并唤醒被测件");
    //if (0 != CheckCommunication(5000)) {
    //    TestPostCondition();
    //    return -1;
    //}
    try
    {
        FulfilTRC();
        for (index = 0; index < MISMSG_DTC_NUM; index++)
        {
            if (misMsgDTCList[index].tstRsltFlg == 0) {
                continue;
            }
            snprintf(StepDespriction, sizeof(StepDespriction), "-------------------------Test missing node %s:0x%X---------------------------", misMsgDTCList[index].node, misMsgDTCList[index].dtc);
            TestReport_HTML_StepDescription(StepDespriction, Info);
            for (int k = 0; k < 600; k++)
            {
                gCCPDataExt[k] = 0;
                gCCPData[k] = 0;

            }
            TRCSwitch(0x0, 0xD, 0x0);

            TestReport_HTML_StepDescription("Start Simulate CCP", Info);

            simulateCCP();

            fdtc[0] = (misMsgDTCList[index].dtc & 0xff0000) >> 16;
            fdtc[1] = (misMsgDTCList[index].dtc & 0xff00) >> 8;
            fdtc[2] = (misMsgDTCList[index].dtc & 0xff);

            TestReport_HTML_StepDescription("Simulate related messages");
            //snprintf(StepDespriction, sizeof(StepDespriction), "%s, Message 0x%X, Signal %s", misMsgDTCList[index].node,
            //    misMsgDTCList[index].misMsgList->msgID, misMsgDTCList[index].misMsgList->misMsgSignalList->sigName);
            snprintf(StepDespriction, sizeof(StepDespriction), "%s, Message 0x%X, Signal %s", misMsgDTCList[index].node,
                g_MissFrame_path[index].FFrameIndentifier, misMsgDTCList[index].signalResult->signalName.c_str());
            TestReport_HTML_StepDescription(StepDespriction, Info);
            SimMissMsg(index);                           //仿真missMsg

            //Step2
            TestStep("wait for 7s");
            AppWait(7000, "");

            //Step3
            TestStep("Clear DTCs with diagnostic request, then wait for 127/stepupvalue*TestPeriod*1.2+5s.");
            time = ceil(127 / misMsgDTCList[index].SUValue) * misMsgDTCList[index].testProd * 1.2 + 5000;
            SID14_ClearAllDTC();
            CheckPosResponse_SID14();
            AppWait(time, "");

            //step4
            TestStep("Read DTC from the DUT with status mask 0x09.");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, misMsgDTCList[index].dtc, 0);

            //step5
            TestStep("Stop the simulation of a missing frame DTC related messages.");
            //snprintf(StepDespriction, sizeof(StepDespriction), "%s, Message 0x%X, Signal %s", misMsgDTCList[index].node,
            //    misMsgDTCList[index].misMsgList->msgID, misMsgDTCList[index].misMsgList->misMsgSignalList->sigName);
            snprintf(StepDespriction, sizeof(StepDespriction), "%s, Message 0x%X, Signal %s", misMsgDTCList[index].node,
                g_MissFrame_path[index].FFrameIndentifier, misMsgDTCList[index].signalResult->signalName.c_str());
            TestReport_HTML_StepDescription(StepDespriction, Info);
            CancelMissMsg(index);

            //step6
            TestStep("wait for 127/stepupvalue*TestPeriod*1.2+5s.Resume the simulation of test DTC related messages");
            //snprintf(StepDespriction, sizeof(StepDespriction), "%s, Message 0x%X, Signal %s", misMsgDTCList[index].node,
            //    misMsgDTCList[index].misMsgList->msgID, misMsgDTCList[index].misMsgList->misMsgSignalList->sigName);
            snprintf(StepDespriction, sizeof(StepDespriction), "%s, Message 0x%X, Signal %s", misMsgDTCList[index].node,
                g_MissFrame_path[index].FFrameIndentifier, misMsgDTCList[index].signalResult->signalName.c_str());
            TestReport_HTML_StepDescription(StepDespriction, Info);
            time = ceil(127 / misMsgDTCList[index].SUValue) * misMsgDTCList[index].testProd * 1.2 + 5000;
            AppWait(time, "");
            SimMissMsg(index);

            //step7
            TestStep("Read OCC6 of test DTC");
            occ6 = ReadOCC6(fdtc);
            if (occ6 == 1)
            {
                snprintf(StepDespriction, sizeof(StepDespriction), "OCC6 is %d, expected is 1", occ6);
                TestReport_HTML_StepDescription(StepDespriction, Pass);
                if (occ6 < misMsgDTCList[index].CofrmLmt)
                {
                    snprintf(StepDespriction, sizeof(StepDespriction), "OCC6 is %d when DTC < confirmed", occ6);
                    TestReport_HTML_StepDescription(StepDespriction, Pass);
                }
            }
            else
            {
                snprintf(StepDespriction, sizeof(StepDespriction), "OCC6 is %d or not applicable", occ6);
                TestReport_HTML_StepDescription(StepDespriction, Fail);
                if (occ6 < misMsgDTCList[index].CofrmLmt)
                {
                    snprintf(StepDespriction, sizeof(StepDespriction), "OCC6 is %d when DTC < confirmed", occ6);
                    TestReport_HTML_StepDescription(StepDespriction, Pass);
                }
            }

            //step8
            TestStep("Read DTC from the DUT with status mask 0x08.");
            RX_SID19_ReportDTCByStatusMask(0x08);
            RX_CheckResponse_DTC_SID19_02(0x08, misMsgDTCList[index].dtc, 0);

            int occ6_1 = 0;
            for (j = 0; j < misMsgDTCList[index].CofrmLmt - 1; j++)
            {
                //step9
                TestStep("Let the DUT go to the next operation cycle.then wait for 10s.");
                NM_NextOperation();
                AppWait(time, "");

                //step10
                TestStep("stop Simulate related messages of test missing frame DTC.");
                //snprintf(StepDespriction, sizeof(StepDespriction), "%s, Message 0x%X, Signal %s", misMsgDTCList[index].node,
                //    misMsgDTCList[index].misMsgList->msgID, misMsgDTCList[index].misMsgList->misMsgSignalList->sigName);
                snprintf(StepDespriction, sizeof(StepDespriction), "%s, Message 0x%X, Signal %s", misMsgDTCList[index].node,
                    g_MissFrame_path[index].FFrameIndentifier, misMsgDTCList[index].signalResult->signalName.c_str());
                TestReport_HTML_StepDescription(StepDespriction, Info);
                CancelMissMsg(index);

                //step11
                TestStep("wait for 127/stepupvalue*TestPeriod*1.2+5s.Resume the simulation of test DTC related messages");
                time = ceil(127 / misMsgDTCList[index].SUValue) * misMsgDTCList[index].testProd * 1.2 + 5000;
                AppWait(time, "");
                SimMissMsg(index);

                //step12
                TestStep("Read OCC6 of test DTC");
                occ6_1 = occ6;
                occ6 = ReadOCC6(fdtc);
                if (j == 0)
                {
                    if (occ6 == 2)
                    {
                        snprintf(StepDespriction, sizeof(StepDespriction), "OCC6 is %d, expected is %d", occ6, 2);
                        TestReport_HTML_StepDescription(StepDespriction, Pass);
                    }
                    else
                    {
                        snprintf(StepDespriction, sizeof(StepDespriction), "OCC6 is %d, expected is %d", occ6, 2);
                        TestReport_HTML_StepDescription(StepDespriction, Fail);
                    }
                }
                else
                {
                    if (occ6 == (occ6_1 + 1))
                    {
                        snprintf(StepDespriction, sizeof(StepDespriction), "OCC6 should add 1 is %d", occ6);
                        TestReport_HTML_StepDescription(StepDespriction, Pass);
                    }
                    else
                    {
                        snprintf(StepDespriction, sizeof(StepDespriction), "OCC6 not add 1 is %d", occ6);
                        TestReport_HTML_StepDescription(StepDespriction, Fail);
                    }
                }
                if (occ6 < misMsgDTCList[index].CofrmLmt)
                {
                    snprintf(StepDespriction, sizeof(StepDespriction), "OCC6 is %d when DTC < confirmed", occ6);
                    TestReport_HTML_StepDescription(StepDespriction, Pass);
                }
                else
                {
                    snprintf(StepDespriction, sizeof(StepDespriction), "OCC6 is %d when DTC >= confirmed", occ6);
                    TestReport_HTML_StepDescription(StepDespriction, Fail);
                }

                //step13
                TestStep("Read DTC from the DUT with status mask 0x08.");
                RX_SID19_ReportDTCByStatusMask(0x08);
                RX_CheckResponse_DTC_SID19_02(0x08, misMsgDTCList[index].dtc, 0);

                if (j == misMsgDTCList[index].CofrmLmt - 3)
                {
                    if (occ6 == misMsgDTCList[index].CofrmLmt)
                    {
                        snprintf(StepDespriction, sizeof(StepDespriction), "OCC6 is equal to DTCConfirmedLimit  %d", occ6);
                        TestReport_HTML_StepDescription(StepDespriction, Pass);
                    }
                    else
                    {
                        snprintf(StepDespriction, sizeof(StepDespriction), "OCC6 isnot equal to DTCConfirmedLimit  %d", occ6);
                        TestReport_HTML_StepDescription(StepDespriction, Fail);
                    }
                }

            }

            for (j = 0; j < misMsgDTCList[index].agdLmt + 10; j++)
            {
                //Step15
                TestStep("Let the DUT go to the next operation cycle.simulation of test DTC related messages");
                NM_NextOperation();
                SimMissMsg(index);

                //Step16
                TestStep("wait for 10s");
                AppWait(10000, "");

                //Step17
                TestStep("读OCC2或者0CC5");
                occ2 = ReadAgingCounter(fdtc, gOcc2Or5);
                if (occ2 == j)
                {
                    snprintf(StepDespriction, sizeof(StepDespriction), "OCC%d is %d", gOcc2Or5, occ2);
                    TestReport_HTML_StepDescription(StepDespriction, Pass);
                }
                else
                {
                    if (j > 254)
                    {
                        snprintf(StepDespriction, sizeof(StepDespriction), "OCC%d is %d, expected is %d or 0", gOcc2Or5, occ2, 255);
                        TestReport_HTML_StepDescription(StepDespriction, Info);
                    }
                    else
                    {
                        snprintf(StepDespriction, sizeof(StepDespriction), "OCC%d is %d, expected is %d", gOcc2Or5, occ2, j);
                        TestReport_HTML_StepDescription(StepDespriction, Fail);
                    }
                }

                //Step18
                TestStep("Read DTC from the DUT with status mask 0x08.");
                RX_SID19_ReportDTCByStatusMask(0x08);
                if (occ2 < 255)
                {
                    RX_CheckResponse_DTC_SID19_02(0x08, misMsgDTCList[index].dtc, 1);
                }
                else
                {
                    RX_CheckResponse_DTC_SID19_02(0x08, misMsgDTCList[index].dtc, 0);
                }

                if (occ2 >= 255)
                {
                    break;
                }
                if ((occ2 == 0) && (j == 10))
                {
                    snprintf(StepDespriction, sizeof(StepDespriction), "OCC%d keeps to 0, break the test.", gOcc2Or5);
                    TestReport_HTML_StepDescription(StepDespriction, Fail);
                    break;
                }
            }

            //step19
            if (gAsrVersion < 4.1)
            {
                TestStep("Read testFailedSincelLastClearOrAged of the test DTC");
                if (ReadTestFailSncLstClrOrAged(fdtc) == 0)
                {
                    strcpy(issuereason, "testFailedSincelLastClearOrAged is 0");
                    TestReport_HTML_StepDescription(issuereason, Fail);
                }
                else
                {
                    strcpy(issuereason, "testFailedSincelLastClearOrAged is 1");
                    TestReport_HTML_StepDescription(issuereason, Pass);
                }
            }

            //step20
            TestStep("Read AgedDTC of the test DTC");
            if (gAsrVersion < 4.1)
            {
                if (ReadAgedDTC(fdtc) == 0)
                {
                    strcpy(issuereason, "ageDTC is 0");
                    TestReport_HTML_StepDescription(issuereason, Pass);
                }
                else
                {
                    strcpy(issuereason, "ageDTC is 1");
                    TestReport_HTML_StepDescription(issuereason, Fail);
                }

            }

            //Step22
            TestStep("Let the DUT go to the next operation cycle.simulation of test DTC related messages");
            NM_NextOperation();
            SimMissMsg(index);


            //Step23
            TestStep("wait for 10s");
            AppWait(10000, "");

            //Step24
            TestStep("Read DTC from the DUT with status mask 0x08.");
            RX_SID19_ReportDTCByStatusMask(0x08);
            RX_CheckResponse_DTC_SID19_02(0x08, misMsgDTCList[index].dtc, 0);


            //step25
            if (gAsrVersion < 4.1)
            {
                TestStep("Read testFailedSincelLastClearOrAged of the test DTC");
                if (ReadTestFailSncLstClrOrAged(fdtc) == 0)
                {
                    strcpy(issuereason, "testFailedSincelLastClearOrAged is 0");
                    TestReport_HTML_StepDescription(issuereason, Pass);
                }
                else
                {
                    strcpy(issuereason, "testFailedSincelLastClearOrAged is 1");
                    TestReport_HTML_StepDescription(issuereason, Fail);
                }
            }

            //step26
            TestStep("Read AgedDTC of the test DTC");
            if (gAsrVersion < 4.1)
            {
                if (ReadAgedDTC(fdtc) == 0)
                {
                    strcpy(issuereason, "ageDTC is 0");
                    TestReport_HTML_StepDescription(issuereason, Fail);
                }
                else
                {
                    strcpy(issuereason, "ageDTC is 1");
                    TestReport_HTML_StepDescription(issuereason, Pass);
                }

            }
            index_cc2 = 0;
            CancelMissMsg(index);
            break;
        }

        if (index == MISMSG_DTC_NUM)
        {
            strcpy(StepDespriction, "All DTC is abnormal, break the test.");
            TestReport_HTML_Step(StepNumber, StepDespriction, Fail);
        }
        index_cc2 = 0;
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

s32 TestCase_1365370_BusoffDTCTestWhenDUTReceivesNoACKerrorframe() {
    int index, j = 0;
    double time;
    u8 dtc[3] = { 0xff,0xff,0xff };
    char StepDespriction[256];
    int occ6, occ2;
    u8 fdtc[3];

    strcpy(TestCaseName, "TestCase_1365370_BusoffDTCTestWhenDUTReceivesNoACKerrorframe");
    strcpy(TestScopeName, "Network fault related DTC Test - Scenario Test");
    strcpy(testPurpose_CH, "测试目的：This testcase is to check whether DUT detect and set bus off DTC when it receives NoACK error frame.");

    if (0 != TestPrecondition())
        return -1;
    try
    {
        FulfilTRC();
        for (index = 0; index < BusNum; index++)
        {
            snprintf(StepDespriction, sizeof(StepDespriction), "Current Test Bus is %s", busFltDTCTemp[index].cluster);
            TestReport_HTML_StepDescription(StepDespriction);
            if (busFltDTCTemp[index].TestRslt == 0) {
                TestReport_HTML_StepDescription("The Bus Busoff DTC Is Not Find", Warning);
                continue;
            }

            //step2
            stepnumber = 1;
            TestStep("Wait for 7s");
            TestWait(7000);

            //step3
            TestStep("Clear DTCs with diagnostic request, then wait for 5s.");
            SID14_ClearAllDTC();
            TestWait(5000);

            //step4
            TestStep("Read DTC from the DUT with status mask 0x09.");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].busOffDTC, 0);

            //step5
            TestStep("Meet DUT sleep condition and send APP messages with 10ms cycle to the bus");
            if (0 == CheckBusSleepCondition(busFltDTCTemp[index].cluster)) {
                SimulateAPPMessage(busFltDTCTemp[index].busType, busFltDTCTemp[index].cluster, 1, 10);
            }
            else {
                TestReport_HTML_StepDescription("The Bus Not Meet Sleep Condition;Break The Bus Test");
                continue;
            }

            //step6
            TestStep("Wait for DUT to go to low-power mode, then reset the network analyser");
            UsgModSwtich(0x1);
            EPLvlSwitch(0x0);
            AppWait(1000, "");
            SendTesterPresent(TP_OFF);
            NMWakeUp(0);
            com.del_cyclic_messages();
            KL15(off);
            AppWait(2000, "");
            TestWaitForCurInRange(gLowPwrCur, gSleepTime);

            //step7
            TestStep("Wake up DUT and meet the condition of TRC");
            NMWakeUp(1);
            FulfilTRC();
            u8 chn = 0;
            chn = NetworkCluster[busFltDTCTemp[index].cluster];
            CheckCommunication(5000, chn);

            //step8
            TestStep("Wait for 7s and read DTC from the DUT with status mask 0x09.");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].busOffDTC, 0);
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

s32 TestCase_1365371_BusoffDTCTestDuringDifferentOperationCycles_CANnode() {
    int index, j = 0;
    double time;
    u8 dtc[3] = { 0xff,0xff,0xff };
    char StepDespriction[256];
    int occ6, occ2;
    u8 fdtc[3];
    s32 THhandle;

    strcpy(TestCaseName, " TestCase_1365371_Busoff DTC Test during different operation cycles-CAN node");
    strcpy(TestScopeName, "Network fault related DTC Test - Scenario Test");
    strcpy(testPurpose_CH, "测试目的：This testcase is to check whether DUT can clear testfailed of busoff DTC in the next operation cycle.");

    if (0 != TestPrecondition())
        return -1;
    try
    {
        FulfilTRC();
        THhandle = DisturbConnect();

        //step2
        stepnumber = 1;
        TestStep("Wait for 7s");
        TestWait(7000);
        for (index = 0; index < BusNum; index++)
        {
            snprintf(StepDespriction, sizeof(StepDespriction), "Current Test Bus is %s", busFltDTCTemp[index].cluster);
            TestReport_HTML_StepDescription(StepDespriction);
            if (busFltDTCTemp[index].TestRslt == 0) {
                TestReport_HTML_StepDescription("The Bus Busoff DTC Is Not Find", Warning);
                continue;
            }

            //step3
            stepnumber = 2;
            TestStep("Clear DTCs with diagnostic request, then wait for 5s.");
            SID14_ClearAllDTC();
            TestWait(5000);

            //step4
            TestStep("Read DTC from the DUT with status mask 0x09.");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].busOffDTC, 0);

            //step5
            TestStep("Disturb all CAN frames transmitted by the DUT to ensure that DUT consecutive bus off(CanSMBorCounterL2Err + 1) times .");
            DisturbFrame(THhandle, index, 32, CanSMBorCounterL2Err + 1, TimeOut_7011_Disturb);
            //if (0 != CheckCommunication(DUTrecoveryTime, NetworkCluster[busFltDTCTemp[index].cluster])) {
            //    break;
            //}

            //step6
            TestStep("Release the network request and let DUT goes to sleep");
            UsgModSwtich(0x1);
            EPLvlSwitch(0x0);
            AppWait(1000, "");
            SendTesterPresent(TP_OFF);
            NMWakeUp(0);
            com.del_cyclic_messages();
            KL15(off);
            AppWait(2000, "");

            //step7
            TestStep("Wait for DUT to go to low - power mode");
            TestWaitForCurInRange(gLowPwrCur, gSleepTime);

            //step8
            TestStep("Wake up DUT, and meet the condition of TRC");
            NMWakeUp(1);
            FulfilTRC();
            u8 chn = 0;
            chn = NetworkCluster[busFltDTCTemp[index].cluster];
            CheckCommunication(5000, chn);

            //step9
            TestStep("Wait for 10s");
            TestWait(10000);

            //step10
            TestStep("Read DTC from the DUT with status mask 0x01.");
            RX_SID19_ReportDTCByStatusMask(0x01);
            RX_CheckResponse_DTC_SID19_02(0x01, busFltDTCTemp[index].busOffDTC, 0);

        }
        DisturbDisconnect();
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

s32 TestCase_1022908_OCCxValueTest() {
    int index, j = 0;
    double time;
    u8 dtc[3] = { 0xff,0xff,0xff };
    char StepDespriction[256];
    int occ1, occ2, occ3, occ4, occ5, occ6, occ7;
    u8 fdtc[3];
    s32 THhandle;

    strcpy(TestCaseName, " TestCase_1022908_OCCx Value Test");
    strcpy(TestScopeName, "Network fault related DTC Test - Scenario Test");
    strcpy(testPurpose_CH, "测试目的：This testcase is to check the OCCx Value.");

    if (0 != TestPrecondition())
        return -1;
    try
    {
        FulfilTRC();
        THhandle = DisturbConnect();

        //step2
        stepnumber = 1;
        TestStep("Wait for 7s");
        TestWait(7000);
        for (index = 0; index < BusNum; index++)
        {
            if (busFltDTCTemp[index].TestRslt == 0) {
                TestReport_HTML_StepDescription("The Bus Busoff DTC Is Not Find", Warning);
                continue;
            }

            //step3
            TestStep("Clear DTCs with diagnostic request, then wait for 5s.");
            SID14_ClearAllDTC();
            TestWait(5000);

            //step4
            TestStep("Read DTC from the DUT with status mask 0x09.");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].busOffDTC, 0);

            //step5
            TestStep("Disturb all CAN frames transmitted by the DUT to ensure that DUT consecutive bus off(CanSMBorCounterL2Err + 1) times;\
            And wait for DUT resume communication.");
            DisturbFrame(THhandle, index, 32, CanSMBorCounterL2Err + 1, TimeOut_7011_Disturb);
            //快恢复？慢恢复？
            if (0 != CheckCommunication(DUTrecoveryTime, NetworkCluster[busFltDTCTemp[index].cluster])) {
                break;
            }

            //step6
            TestStep("Wait for 5s; then read OCC1 OCC2 OCC3 OCC4 OCC6 OCC7 of test DTC.");
            TestWait(5000);
            dtc[0] = busFltDTCTemp[index].busOffDTC >> 16 & 0xFF;
            dtc[1] = busFltDTCTemp[index].busOffDTC >> 8 & 0xFF;
            dtc[2] = busFltDTCTemp[index].busOffDTC & 0xFF;

            ReadAllOCC(dtc, 0, 0, 0, 1, 0, 0);

            //step7
            TestStep("Let the DUT go to the next operation cycle");
            NM_NextOperation();

            //step8
            TestStep("Wait for 10s; then read OCC1 OCC2 OCC3 OCC4 OCC6 OCC7 of test DTC.");
            TestWait(10000);
            if (busFltDTCTemp[index].CofrmLmt > 1) {
                ReadAllOCC(dtc, 1, 0, 1, 1, 1, 1);
            }
            else if (busFltDTCTemp[index].CofrmLmt == 1) {
                ReadAllOCC(dtc, 1, 0, 1, 1, 0, 1);
            }

            //step9
            TestStep("Disturb DUT to make sure FDC10 of test\
                DTC is 0x7F at current operation cycle.\
                Note: if test DTC is busoff DTC, disturb all\
                CAN frames transmitted by the DUT to\
                ensure that DUT consecutive bus off\
                (CanSMBorCounterL2Err + 1) times and\
                wait for DUT resume communication;");
            DisturbFrame(THhandle, index, 32, CanSMBorCounterL2Err + 1, TimeOut_7011_Disturb);
            //快恢复？慢恢复？
            if (0 != CheckCommunication(DUTrecoveryTime, NetworkCluster[busFltDTCTemp[index].cluster])) {
                break;
            }

            //step10
            TestStep("Wait for 5s; then read OCC1 OCC2 OCC3 OCC4 OCC6 OCC7 of test DTC.");
            TestWait(5000);
            if (busFltDTCTemp[index].CofrmLmt > 2) {
                ReadAllOCC(dtc, 0, 0, 1, 2, 2, 1);
            }
            //if(OCC6 == CofrmLmt) OCC6 should be cleared;
            else if (busFltDTCTemp[index].CofrmLmt < 2) {
                ReadAllOCC(dtc, 0, 0, 1, 2, 0, 1);
            }

            //step11
            TestStep("Let the DUT go to the next operation cycle");
            NM_NextOperation();

            //step12
            TestStep("Wait for 10s; then read OCC1 OCC2 OCC3 OCC4 OCC7 of test DTC.");
            TestWait(10000);
            ReadAllOCC(dtc, 1, 0, 2, 2, -1, 2);

            //step13
            TestStep("Let the DUT go to the next operation cycle");
            NM_NextOperation();

            //step14
            TestStep("Wait for 10s; then read OCC1 OCC2 OCC3 OCC4 OCC7 of test DTC.");
            TestWait(10000);
            ReadAllOCC(dtc, 2, 1, 3, 2, -1, 3);

            //step15
            TestStep("Disturb DUT to make sure FDC10 of test\
                DTC is 0x7F at current operation cycle.\
                Note: if test DTC is busoff DTC, disturb all\
                CAN frames transmitted by the DUT to\
                ensure that DUT consecutive bus off\
                (CanSMBorCounterL2Err + 1) times and\
                wait for DUT resume communication;");
            DisturbFrame(THhandle, index, 32, CanSMBorCounterL2Err + 1, TimeOut_7011_Disturb);
            //快恢复？慢恢复？
            if (0 != CheckCommunication(DUTrecoveryTime, NetworkCluster[busFltDTCTemp[index].cluster])) {
                break;
            }

            //step16
            TestStep("Wait for 5s; then read OCC1 OCC2 OCC3 OCC4 OCC7 of test DTC.");
            TestWait(5000);
            ReadAllOCC(dtc, 0, 0, 3, 3, -1, 3);

            //step17
            TestStep("Disturb DUT to make sure FDC10 of test\
                DTC is 0x7F at current operation cycle.\
                Note: if test DTC is busoff DTC, disturb all\
                CAN frames transmitted by the DUT to\
                ensure that DUT consecutive bus off\
                (CanSMBorCounterL2Err + 1) times and\
                wait for DUT resume communication;");
            DisturbFrame(THhandle, index, 32, CanSMBorCounterL2Err + 1, TimeOut_7011_Disturb);
            //快恢复？慢恢复？
            if (0 != CheckCommunication(DUTrecoveryTime, NetworkCluster[busFltDTCTemp[index].cluster])) {
                break;
            }

            //step18
            TestStep("Wait for 5s; then read OCC3 OCC4 of test DTC.");
            TestWait(5000);
            ReadAllOCC(dtc, -1, -1, 3, 3, -1, -1);

            //step19
            TestStep("Let the DUT go to the next operation cycle");
            NM_NextOperation();

            //step20
            TestStep("Wait for 10s; then read OCC1 OCC2 OCC3 OCC4 OCC7 of test DTC.");
            TestWait(10000);
            ReadAllOCC(dtc, 1, 0, 4, 3, -1, 4);

            //step21
            TestStep("Disturb DUT to make sure FDC10 of test\
                DTC is 0x7F at current operation cycle.\
                Note: if test DTC is busoff DTC, disturb all\
                CAN frames transmitted by the DUT to\
                ensure that DUT consecutive bus off\
                (CanSMBorCounterL2Err + 1) times and\
                wait for DUT resume communication;");
            DisturbFrame(THhandle, index, 32, CanSMBorCounterL2Err + 1, TimeOut_7011_Disturb);
            //快恢复？慢恢复？
            if (0 != CheckCommunication(DUTrecoveryTime, NetworkCluster[busFltDTCTemp[index].cluster])) {
                break;
            }

            //step22
            TestStep("Wait for 5s; then read OCC1 OCC2 OCC3 OCC4 OCC7 of test DTC.");
            TestWait(5000);
            ReadAllOCC(dtc, 0, 0, 4, 4, -1, 4);

            //step23
            TestStep("Following step is applicable for emission\
                related busoff DTC or start up error DTC.");
            if (gOcc2Or5 == 5) {
                //step24
                TestStep("Read OCC5 of test DTC");
                occ5 = ReadOCC5(fdtc);
                snprintf(StepDespriction, sizeof(StepDespriction), "OCC5 is %d", occ5);
                if (occ5 == 0)
                {
                    TestReport_HTML_StepDescription(StepDespriction, Pass);
                }
                else
                {
                    TestReport_HTML_StepDescription(StepDespriction, Fail);
                }

                //step25
                TestStep("Meet the condition of MIL switch off. And then let DUT goes to next warm-up cycles.");
                NextWarmUpCyc();

                //step26
                TestStep("Wait for 5s, then read OCC5 of test DTC");
                TestWait(5000);
                occ5 = ReadOCC5(fdtc);
                snprintf(StepDespriction, sizeof(StepDespriction), "OCC5 is %d", occ5);
                if (occ5 == 1)
                {
                    TestReport_HTML_StepDescription(StepDespriction, Pass);
                }
                else
                {
                    TestReport_HTML_StepDescription(StepDespriction, Fail);
                }

                //step27
                TestStep("Let DUT goes to next warm-up cycles.");
                NextWarmUpCyc();

                //step28
                TestStep("Wait for 10s, then read OCC5 of test DTC");
                TestWait(10000);
                occ5 = ReadOCC5(fdtc);
                snprintf(StepDespriction, sizeof(StepDespriction), "OCC5 is %d", occ5);
                if (occ5 == 2)
                {
                    TestReport_HTML_StepDescription(StepDespriction, Pass);
                }
                else
                {
                    TestReport_HTML_StepDescription(StepDespriction, Fail);
                }

                //step29
                TestStep("Disturb DUT to make sure FDC10 of test\
                DTC is 0x7F at current operation cycle.\
                Note: if test DTC is busoff DTC, disturb all\
                CAN frames transmitted by the DUT to\
                ensure that DUT consecutive bus off\
                (CanSMBorCounterL2Err + 1) times and\
                wait for DUT resume communication;");
                DisturbFrame(THhandle, index, 32, CanSMBorCounterL2Err + 1, TimeOut_7011_Disturb);
                //快恢复？慢恢复？
                if (0 != CheckCommunication(DUTrecoveryTime, NetworkCluster[busFltDTCTemp[index].cluster])) {
                    break;
                }

                //step30
                TestStep("Wait for 5s, then read OCC5 of test DTC");
                TestWait(5000);
                occ5 = ReadOCC5(fdtc);
                snprintf(StepDespriction, sizeof(StepDespriction), "OCC5 is %d", occ5);
                if (occ5 == 0)
                {
                    TestReport_HTML_StepDescription(StepDespriction, Pass);
                }
                else
                {
                    TestReport_HTML_StepDescription(StepDespriction, Fail);
                }
            }
            else {
                TestReport_HTML_StepDescription("OCC2 is not applicable", Warning);
            }

            break;
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

s32 TestCase_1325139_DTCStatusBitTest() {
    int index, j = 0;
    double time;
    u8 dtc[3] = { 0xff,0xff,0xff };
    char StepDespriction[256];
    int occ6, occ2;
    u8 fdtc[3];

    strcpy(TestCaseName, " TestCase_1325139_DTC Status Bit Test");
    strcpy(TestScopeName, "Network fault related DTC Test - Scenario Test");
    strcpy(testPurpose_CH, "测试目的：This testcase is to check DTC status bits.");

    if (0 != TestPrecondition())
        return -1;
    try
    {
        FulfilTRC();
        for (index = 0; index < BusNum; index++)
        {
            snprintf(StepDespriction, sizeof(StepDespriction), "Current Test Bus is %s", busFltDTCTemp[index].cluster);
            TestReport_HTML_StepDescription(StepDespriction);
            if (busFltDTCTemp[index].TestRslt == 0) {
                TestReport_HTML_StepDescription("The Bus Busoff DTC Is Not Find", Warning);
                continue;
            }

            //step2
            stepnumber = 1;
            TestStep("Wait for 7s");
            TestWait(7000);

            //step3
            TestStep("Clear DTCs with diagnostic request, then wait for 5s.");
            SID14_ClearAllDTC();
            TestWait(5000);

            //step4
            TestStep("Read DTC from the DUT with status mask 0x09.");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].busOffDTC, 0);

            //step5
            TestStep("Meet DUT sleep condition and send APP messages with 10ms cycle to the bus");
            if (0 == CheckBusSleepCondition(busFltDTCTemp[index].cluster)) {
                SimulateAPPMessage(busFltDTCTemp[index].busType, busFltDTCTemp[index].cluster, 1, 10);
            }
            else {
                TestReport_HTML_StepDescription("The Bus Not Meet Sleep Condition;Break The Bus Test");
                continue;
            }

            //step6
            TestStep("Wait for DUT to go to low-power mode, then reset the network analyser");
            FulfilTRC();
            NM_NextOperation();

            //step7
            TestStep("Wake up DUT and meet the condition of TRC");
            NMWakeUp(1);
            FulfilTRC();
            u8 chn = 0;
            chn = NetworkCluster[busFltDTCTemp[index].cluster];
            CheckCommunication(5000, chn);

            //step8
            TestStep("Wait for 7s and read DTC from the DUT with status mask 0x09.");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, busFltDTCTemp[index].busOffDTC, 0);


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

s32 TestCase_1346148_DTCGlobalSnapshotDataAndTimeStampTest() {
    int index, j = 0;
    double time;
    u8 dtc[3] = { 0xff,0xff,0xff };
    char StepDespriction[256];
    u8 fdtc[3];
    u8 recordNum;
    double volt;

    strcpy(TestCaseName, " TestCase_1346148_DTC Global Snapshot Data and Time stamp Testt");
    strcpy(TestScopeName, "Network fault related DTC Test - Scenario Test");
    strcpy(testPurpose_CH, "测试目的：This testcase is to check DTC Global Snapshot Data and Time stamp Extended Data.");

    if (0 != TestPrecondition())
        return -1;
    try
    {
        FulfilTRC();
        if (highVoltDTC.dtc == 0)
        {
            TestReport_HTML_StepDescription("No high voltage DTC, this case is not applicable.", Warning);
            TestPostCondition();
            return 0;
        }

        fdtc[0] = (highVoltDTC.dtc & 0xff0000) >> 16;
        fdtc[1] = (highVoltDTC.dtc & 0xff00) >> 8;
        fdtc[2] = (highVoltDTC.dtc & 0xff);

        if (gCndtnOfClrDtc < 2)gVehSpdSimFlag = 0;
        TRCSwitch(0x0, 0xD, 0x0);
        if (gCEMFlg != 1) {
            SimVehBat(gUbat12);
        }

        //step2
        TestStep("Wait for 7s");
        TestWait(7000);

        //step3
        time = ceil(127 / highVoltDTC.SUValue) * highVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "Clear DTCs with diagnostic request, then wait for %lfms (127/StepUpValue*TestPeriod*1.2+5s)", time);
        TestStep(StepDespriction);
        SID14_ClearAllDTC();
        CheckPosResponse_SID14();
        TestWait(time);

        //step4
        TestStep("Read DTC from the DUT with status mask 0x0F.");
        RX_SID19_ReportDTCByStatusMask(0x0F);
        RX_CheckResponse_DTC_SID19_02(0x0F, highVoltDTC.dtc, 0);

        //step5
        u8 subFunction_03[2] = { 0x03 };
        TestStep("Send 19 03");
        RX_SID19_ReportDTCSnapshotIdentification();
        RX_SID19_CheckSnapshotIdentification(subFunction_03, highVoltDTC.dtc, gSnapshotRecordNo, 0);

        //step6
        snprintf(StepDespriction, sizeof(StepDespriction), "Read snapshot record of test DTC by SnapshotRecordNo %0xX", gSnapshotRecordNo);
        TestStep(StepDespriction);
        if (!ReadSnapShot(fdtc))
        {
            TestReport_HTML_StepDescription("No snapshot record in the response.", Pass);
        }
        else
        {
            TestReport_HTML_StepDescription("Find snapshot record in the response.", Fail);
        }

        //step7
        TestStep("Read time stamp #20, #21 of test DTC");
        for (u8 i = 0; i < 2; i++)
        {
            recordNum = 0x20 + i;
            if (ReadTimeStamp(fdtc, recordNum) == 0)
            {
                snprintf(StepDespriction, sizeof(StepDespriction), "No time stamp #%X record in the response.", recordNum);
                TestReport_HTML_StepDescription(StepDespriction, Pass);
            }
            else
            {
                snprintf(StepDespriction, sizeof(StepDespriction), "Find time stamp #%X record in the response.", recordNum);
                TestReport_HTML_StepDescription(StepDespriction, Pass);
            }
        }

        //step8
        volt = highVoltDTC.volt + 0.35;
        snprintf(StepDespriction, sizeof(StepDespriction), "Set voltage to %5.2fV HighVolt+0.35V", volt);
        TestStep(StepDespriction);
        SetVoltage(volt);
        snprintf(StepDespriction, sizeof(StepDespriction), "Get Current Voltage is %.3lf", GetVoltage());
        TestReport_HTML_StepDescription(StepDespriction);

        //step9
        time = ceil(127 / highVoltDTC.SUValue) * highVoltDTC.testProd * 1.2;
        snprintf(StepDespriction, sizeof(StepDespriction), "wait for %lfms (127/StepUpValue*TestPeriod*1.2)", time);
        TestStep(StepDespriction);
        TestWait(time);

        //step10
        TestStep("Set voltage to 12V and wait for 5s, then read DTC by 19 02 03");
        SetVoltage(12);
        snprintf(StepDespriction, sizeof(StepDespriction), "Get Current Voltage is %.3lf", GetVoltage());
        TestReport_HTML_StepDescription(StepDespriction);
        if (gCEMFlg != 1)
            SimVehBat(gUbat12);
        TestWait(5000);
        RX_SID19_ReportDTCByStatusMask(0x03);
        RX_CheckResponse_DTC_SID19_02(0x03, highVoltDTC.dtc, 1);

        //step11
        TestStep("Send 19 03");
        RX_SID19_ReportDTCSnapshotIdentification();
        RX_SID19_CheckSnapshotIdentification(subFunction_03, highVoltDTC.dtc, gSnapshotRecordNo, 1);

        //step12
        snprintf(StepDespriction, sizeof(StepDespriction), "Read Snapshot Record of test DTC by 0x%X", gSnapshotRecordNo);
        TestStep(StepDespriction);
        ReadSnapShot(fdtc);
        if (gCndtnOfClrDtc < 2)
            CheckSnapshotDataValue(fdtc, 0xFFFFFFFF, 0xFFFFFF, 0x30, 0xFFFF, gNotAvailableArray, gUagMode, gAllPNC, 0, gCarMode);
        else
            CheckSnapshotDataValue(fdtc, 0xFFFFFFFF, 0xFFFFFF, 0x30, gVehSpd, gNotAvailableArray, gUagMode, gAllPNC, 0, gCarMode);

        //step13
        TestStep("Read time stamp #20, #21 of test DTC");
        CheckTimeStamp(0x20, fdtc, gCarTiGlb_invalid);
        CheckTimeStamp(0x21, fdtc, gCarTiGlb_invalid);
        //CheckTimeStamp(0x22, fdtc, gNotAvailableArray);
        //CheckTimeStamp(0x23, fdtc, gNotAvailableArray);

          /*---------------------------------------------Invalid Value----------------------------------------------------------*/
        TestReport_HTML_StepDescription("------Invalid Value--------");

        //step14
        TestStep("Simulate all global snapshot data related messages to the bus and the value of related signals is invalid value.");
        Snapshot_SigValueChange(gCarTiGlb_invalid, gBkpOfDstTrvld_invalid, gVehSpd_invalid, gTiAndDateIndn_invalid, sizeof(gTiAndDateIndn_invalid) / sizeof(gTiAndDateIndn_invalid[0]));
        SimCarTiGlb();//DD00
        SimBkpOfDstTrvld();//DD01
        SimTiAndDateIndn();//DD07
        if (gCndtnOfClrDtc < 2)
        {
            SimVehMtnSpd_Creat();
            gVehSpdSimFlag = 1;
        }

        //step15
        time = ceil(127 / highVoltDTC.SUValue) * highVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "Clear DTCs with diagnostic request; \
            then wait for %dms (127/StepUpValue*TestPeriod*1.2+5s)", time + 5000);
        TestStep(StepDespriction);
        SID14_ClearAllDTC();
        CheckPosResponse_SID14();
        TestWait(time);

        //step16
        TestStep("Read DTC from the DUT with status mask 0x0F.");
        RX_SID19_ReportDTCByStatusMask(0x0F);
        RX_CheckResponse_DTC_SID19_02(0x0F, highVoltDTC.dtc, 0);

        //step17
        volt = highVoltDTC.volt + 0.35;
        snprintf(StepDespriction, sizeof(StepDespriction), "Set voltage to %5.2fV HighVolt+0.35V", volt);
        TestStep(StepDespriction);
        SetVoltage(volt);
        snprintf(StepDespriction, sizeof(StepDespriction), "Get Current Voltage is %.3lf", GetVoltage());
        TestReport_HTML_StepDescription(StepDespriction);

        //step18
        time = ceil(127 / highVoltDTC.SUValue) * highVoltDTC.testProd * 1.2;
        snprintf(StepDespriction, sizeof(StepDespriction), "wait for %lfms (127/StepUpValue*TestPeriod*1.2)", time);
        TestStep(StepDespriction);
        TestWait(time);

        //step19
        TestStep("Set voltage to 12V and wait for 5s, then read DTC by 19 02 03");
        SetVoltage(12);
        snprintf(StepDespriction, sizeof(StepDespriction), "Get Current Voltage is %.3lf", GetVoltage());
        TestReport_HTML_StepDescription(StepDespriction);
        if (gCEMFlg != 1)
            SimVehBat(gUbat12);
        TestWait(5000);
        RX_SID19_ReportDTCByStatusMask(0x03);
        RX_CheckResponse_DTC_SID19_02(0x03, highVoltDTC.dtc, 1);

        //step20
        TestStep("Send 19 03");
        RX_SID19_ReportDTCSnapshotIdentification();
        RX_SID19_CheckSnapshotIdentification(subFunction_03, highVoltDTC.dtc, gSnapshotRecordNo, 1);

        if (gCndtnOfClrDtc < 2)
            CheckSnapshotDataValue(fdtc, 0xFFFFFFFF, 0xFFFFFF, 0x30, 0xFFFF, gNotAvailableArray, gUagMode, gAllPNC, 0, gCarMode);
        else
            CheckSnapshotDataValue(fdtc, 0xFFFFFFFF, 0xFFFFFF, 0x30, gVehSpd, gNotAvailableArray, gUagMode, gAllPNC, 0, gCarMode);

        //step21
        TestStep("Read time stamp #20, #21 of test DTC");
        CheckTimeStamp(0x20, fdtc, gCarTiGlb_invalid);
        CheckTimeStamp(0x21, fdtc, gCarTiGlb_invalid);
        //CheckTimeStamp(0x22, fdtc, gNotAvailableArray);
        //CheckTimeStamp(0x23, fdtc, gNotAvailableArray);

        //step22
          /*---------------------------------------------Min Valid Value----------------------------------------------------------*/
        TestReport_HTML_StepDescription("-------Min Valid Value------");
        TestStep("Simulate all global snapshot data related messages to the bus and the value of related signals is min valid value.");
        Snapshot_SigValueChange(gCarTiGlb_min, gBkpOfDstTrvld_min, gVehSpd_min, gTiAndDateIndn_min, sizeof(gTiAndDateIndn_min) / sizeof(gTiAndDateIndn_min[0]));

        //step23
        time = ceil(127 / highVoltDTC.SUValue) * highVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "Clear DTCs with diagnostic request, then wait for %dms (127/StepUpValue*TestPeriod*1.2+5s)", time + 5000);
        TestStep(StepDespriction);
        SID14_ClearAllDTC();
        CheckPosResponse_SID14();
        TestWait(time);

        //step24
        TestStep("Read DTC from the DUT with status mask 0x0F.");
        RX_SID19_ReportDTCByStatusMask(0x0F);
        RX_CheckResponse_DTC_SID19_02(0x0F, highVoltDTC.dtc, 0);

        //step25
        volt = highVoltDTC.volt + 0.35;
        snprintf(StepDespriction, sizeof(StepDespriction), "Set voltage to %5.2fV HighVolt+0.35V", volt);
        TestStep(StepDespriction);
        SetVoltage(volt);
        snprintf(StepDespriction, sizeof(StepDespriction), "Get Current Voltage is %.3lf", GetVoltage());
        TestReport_HTML_StepDescription(StepDespriction);

        //step27
        time = ceil(127 / highVoltDTC.SUValue) * highVoltDTC.testProd * 1.2;
        snprintf(StepDespriction, sizeof(StepDespriction), "wait for %lfms (127/StepUpValue*TestPeriod*1.2)", time);
        stepnumber = 26;
        TestStep(StepDespriction);
        TestWait(time);

        //step28
        TestStep("Set voltage to 12V and wait for 5s, then read DTC by 19 02 03");
        SetVoltage(12);
        snprintf(StepDespriction, sizeof(StepDespriction), "Get Current Voltage is %.3lf", GetVoltage());
        TestReport_HTML_StepDescription(StepDespriction);
        if (gCEMFlg != 1)
            SimVehBat(gUbat12);
        TestWait(5000);
        RX_SID19_ReportDTCByStatusMask(0x03);
        RX_CheckResponse_DTC_SID19_02(0x03, highVoltDTC.dtc, 1);

        //step29
        snprintf(StepDespriction, sizeof(StepDespriction), "Read Snapshot Record of test DTC by 0x%X", gSnapshotRecordNo);
        TestStep(StepDespriction);
        ReadSnapShot(fdtc);
        CheckSnapshotDataValue(fdtc, gCarTiGlb, gBkpOfDstTrvld, 0x30, gVehSpd, gTiAndDateIndn, gUagMode, gAllPNC, 0, gCarMode);


        //step30
        TestStep("Read time stamp #20, #21 of test DTC");
        CheckTimeStamp(0x20, fdtc, gCarTiGlb);
        CheckTimeStamp(0x21, fdtc, gCarTiGlb);
        //CheckTimeStamp(0x22, fdtc, gTiAndDateIndn);
        //CheckTimeStamp(0x23, fdtc, gTiAndDateIndn);

        //step31
        /*---------------------------------------------Max Valid Value----------------------------------------------------------*/
        TestReport_HTML_StepDescription("------Max Valid Value-------");
        TestStep("Simulate all global snapshot data related messages to the bus and the value of related signals is max valid value.");
        Snapshot_SigValueChange(gCarTiGlb_max, gBkpOfDstTrvld_max, gVehSpd_max, gTiAndDateIndn_max, sizeof(gTiAndDateIndn_max) / sizeof(gTiAndDateIndn_max[0]));

        //step32
        time = ceil(127 / highVoltDTC.SUValue) * highVoltDTC.testProd * 1.2;
        snprintf(StepDespriction, sizeof(StepDespriction), "Clear DTCs with diagnostic request, then wait for %dms (127/StepUpValue*TestPeriod*1.2+5s)", time + 5000);
        TestStep(StepDespriction);
        SID14_ClearAllDTC();
        CheckPosResponse_SID14();
        TestWait(time);

        //step33
        TestStep("Read DTC from the DUT with status mask 0x0F.");
        RX_SID19_ReportDTCByStatusMask(0x0F);
        RX_CheckResponse_DTC_SID19_02(0x0F, highVoltDTC.dtc, 0);

        //step34
        volt = highVoltDTC.volt + 0.35;
        snprintf(StepDespriction, sizeof(StepDespriction), "Set voltage to %5.2fV HighVolt+0.35V", volt);
        TestStep(StepDespriction);
        SetVoltage(volt);
        snprintf(StepDespriction, sizeof(StepDespriction), "Get Current Voltage is %.3lf", GetVoltage());
        TestReport_HTML_StepDescription(StepDespriction);

        //step35
        time = ceil(127 / highVoltDTC.SUValue) * highVoltDTC.testProd * 1.2;
        snprintf(StepDespriction, sizeof(StepDespriction), "wait for %lfms (127/StepUpValue*TestPeriod*1.2)", time);
        TestStep(StepDespriction);
        TestWait(time);

        //step36
        TestStep("Set voltage to 12V and wait for 5s, then read DTC by 19 02 03");
        SetVoltage(12);
        snprintf(StepDespriction, sizeof(StepDespriction), "Get Current Voltage is %.3lf", GetVoltage());
        TestReport_HTML_StepDescription(StepDespriction);
        if (gCEMFlg != 1)
            SimVehBat(gUbat12);
        TestWait(5000);
        RX_SID19_ReportDTCByStatusMask(0x03);
        RX_CheckResponse_DTC_SID19_02(0x03, highVoltDTC.dtc, 1);

        //step37
        snprintf(StepDespriction, sizeof(StepDespriction), "Read Snapshot Record of test DTC by 0x%X", gSnapshotRecordNo);
        TestStep(StepDespriction);
        ReadSnapShot(fdtc);
        if (gCndtnOfClrDtc < 2)
            CheckSnapshotDataValue(fdtc, gCarTiGlb, gBkpOfDstTrvld, 0x30, gVehSpd, gTiAndDateIndn, gUagMode, gAllPNC, 0, gCarMode);
        else
            CheckSnapshotDataValue(fdtc, gCarTiGlb, gBkpOfDstTrvld, 0x30, gVehSpd_valid, gTiAndDateIndn, gUagMode, gAllPNC, 0, gCarMode);


        //step38
        TestStep("Read time stamp #20, #21 of test DTC");
        CheckTimeStamp(0x20, fdtc, gCarTiGlb);
        CheckTimeStamp(0x21, fdtc, gCarTiGlb);
        //CheckTimeStamp(0x22, fdtc, gTiAndDateIndn);
        //CheckTimeStamp(0x23, fdtc, gTiAndDateIndn);

        //step39
         /*----------------------------------------------Valid value between min and max------------------------------------------------------------*/
        TestReport_HTML_StepDescription("--------Valid Value-------");
        TestStep("Simulate all global snapshot data related messages to the bus and the value of related signals is between min and max valid value.");
        Snapshot_SigValueChange(gCarTiGlb_valid, gBkpOfDstTrvld_valid, gVehSpd_valid, gTiAndDateIndn_valid, sizeof(gTiAndDateIndn_valid) / sizeof(gTiAndDateIndn_valid[0]));


        //step40
        time = ceil(127 / highVoltDTC.SUValue) * highVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "wait for %lfms (127/StepUpValue*TestPeriod*1.2 + 5s)", time);
        TestStep(StepDespriction);
        SID14_ClearAllDTC();
        CheckPosResponse_SID14();
        TestWait(time);

        //step41
        TestStep("Read DTC from the DUT with status mask 0x0F.");
        RX_SID19_ReportDTCByStatusMask(0x0F);
        RX_CheckResponse_DTC_SID19_02(0x0F, highVoltDTC.dtc, 0);

        //step42
        TestStep("Send 19 03");
        RX_SID19_ReportDTCSnapshotIdentification();
        RX_SID19_CheckSnapshotIdentification(subFunction_03, highVoltDTC.dtc, gSnapshotRecordNo, 0);

        //step43
        snprintf(StepDespriction, sizeof(StepDespriction), "Read Snapshot Record of test DTC by 0x%X", gSnapshotRecordNo);
        TestStep(StepDespriction);
        if (!ReadSnapShot(fdtc))
        {
            TestReport_HTML_StepDescription("No snapshot record in the response.", Pass);
        }
        else
        {
            TestReport_HTML_StepDescription("Find snapshot record in the response.", Fail);
        }

        //step44
        TestStep("Read time stamp #20, #21 of test DTC");
        for (u8 i = 0; i < 2; i++)
        {
            recordNum = 0x20 + i;
            if (ReadTimeStamp(fdtc, recordNum) == 0)
            {
                snprintf(StepDespriction, sizeof(StepDespriction), "No time stamp #%X record in the response.", recordNum);
                TestReport_HTML_StepDescription(StepDespriction, Pass);
            }
            else
            {
                snprintf(StepDespriction, sizeof(StepDespriction), "Find time stamp #%X record in the response.", recordNum);
                TestReport_HTML_StepDescription(StepDespriction, Fail);
            }
        }

        //step45
        volt = highVoltDTC.volt + 0.35;
        snprintf(StepDespriction, sizeof(StepDespriction), "Set voltage to %5.2fV HighVolt+0.35V", volt);
        TestStep(StepDespriction);
        SetVoltage(volt);
        snprintf(StepDespriction, sizeof(StepDespriction), "Get Current Voltage is %.3lf", GetVoltage());
        TestReport_HTML_StepDescription(StepDespriction);

        //step46
        time = ceil(127 / highVoltDTC.SUValue) * highVoltDTC.testProd * 1.2;
        snprintf(StepDespriction, sizeof(StepDespriction), "wait for %lfms (127/StepUpValue*TestPeriod*1.2)", time);
        TestStep(StepDespriction);
        TestWait(time);

        //step47
        TestStep("Set voltage to 12V and wait for 5s, then read DTC by 19 02 03");
        SetVoltage(12);
        snprintf(StepDespriction, sizeof(StepDespriction), "Get Current Voltage is %.3lf", GetVoltage());
        TestReport_HTML_StepDescription(StepDespriction);
        if (gCEMFlg != 1)
            SimVehBat(gUbat12);
        TestWait(5000);
        RX_SID19_ReportDTCByStatusMask(0x03);
        RX_CheckResponse_DTC_SID19_02(0x03, highVoltDTC.dtc, 1);

        //step48
        TestStep("Send 19 03");
        RX_SID19_ReportDTCSnapshotIdentification();
        RX_SID19_CheckSnapshotIdentification(subFunction_03, highVoltDTC.dtc, gSnapshotRecordNo, 0);

        //step49
        snprintf(StepDespriction, sizeof(StepDespriction), "Read Snapshot Record of test DTC by 0x%X", gSnapshotRecordNo);
        TestStep(StepDespriction);
        ReadSnapShot(fdtc);
        CheckSnapshotDataValue(fdtc, gCarTiGlb, gBkpOfDstTrvld, 0x30, gVehSpd, gTiAndDateIndn, gUagMode, gAllPNC, 0, gCarMode);

        //step50
        TestStep("Read time stamp #20, #21 of test DTC");
        CheckTimeStamp(0x20, fdtc, gCarTiGlb);
        CheckTimeStamp(0x21, fdtc, gCarTiGlb);
        //CheckTimeStamp(0x22, fdtc, gTiAndDateIndn);
        //CheckTimeStamp(0x23, fdtc, gTiAndDateIndn);

        //step51
        /*---------------------------------------------Invalid Value----------------------------------------------------------*/
        TestReport_HTML_StepDescription("-------Invalid Value-------");
        TestStep("Simulate all global snapshot data related messages to the bus and the value of related signals is invalid value.");
        Snapshot_SigValueChange(gCarTiGlb_invalid, gBkpOfDstTrvld_invalid, gVehSpd_invalid, gTiAndDateIndn_invalid, sizeof(gTiAndDateIndn_invalid) / sizeof(gTiAndDateIndn_invalid[0]));

        //step52
        time = ceil(127 / highVoltDTC.SUValue) * highVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "Clear DTCs with diagnostic request, then wait for %dms (127/StepUpValue*TestPeriod*1.2+5s)", time);
        TestStep(StepDespriction);
        SID14_ClearAllDTC();
        CheckPosResponse_SID14();
        TestWait(time);

        //step53
        TestStep("Read DTC from the DUT with status mask 0x0F.");
        RX_SID19_ReportDTCByStatusMask(0x0F);
        RX_CheckResponse_DTC_SID19_02(0x0F, highVoltDTC.dtc, 0);

        //step54
        volt = highVoltDTC.volt + 0.35;
        snprintf(StepDespriction, sizeof(StepDespriction), "Set voltage to %5.2fV HighVolt+0.35V", volt);
        TestStep(StepDespriction);
        SetVoltage(volt);
        snprintf(StepDespriction, sizeof(StepDespriction), "Get Current Voltage is %.3lf", GetVoltage());
        TestReport_HTML_StepDescription(StepDespriction);

        //step55
        time = ceil(127 / highVoltDTC.SUValue) * highVoltDTC.testProd * 1.2;
        snprintf(StepDespriction, sizeof(StepDespriction), "wait for %lfms (127/StepUpValue*TestPeriod*1.2)", time);
        TestStep(StepDespriction);
        TestWait(time);

        //step56
        TestStep("Set voltage to 12V and wait for 5s, then read DTC by 19 02 03");
        SetVoltage(12);
        snprintf(StepDespriction, sizeof(StepDespriction), "Get Current Voltage is %.3lf", GetVoltage());
        TestReport_HTML_StepDescription(StepDespriction);
        if (gCEMFlg != 1)
            SimVehBat(gUbat12);
        TestWait(5000);
        RX_SID19_ReportDTCByStatusMask(0x03);
        RX_CheckResponse_DTC_SID19_02(0x03, highVoltDTC.dtc, 1);

        //step57
        snprintf(StepDespriction, sizeof(StepDespriction), "Read Snapshot Record of test DTC by 0x%X", gSnapshotRecordNo);
        TestStep(StepDespriction);
        ReadSnapShot(fdtc);
        CheckSnapshotDataValue(fdtc, gCarTiGlb_valid, gBkpOfDstTrvld_valid, 0x30, gVehSpd_valid, gTiAndDateIndn_valid, gUagMode, gAllPNC, 0, gCarMode);


        //step58
        TestStep("Read time stamp #20, #21 of test DTC");
        CheckTimeStamp(0x20, fdtc, gCarTiGlb_valid);
        CheckTimeStamp(0x21, fdtc, gCarTiGlb_valid);
        //CheckTimeStamp(0x22, fdtc, gTiAndDateIndn_valid);
        //CheckTimeStamp(0x23, fdtc, gTiAndDateIndn_valid);


        //step59
        /*-------------------------------------------------Test difference between time stamp #20/#22 and #21/#23---------------------------------------------------------------*/
        TestReport_HTML_StepDescription("--------time stamp test in different OCs--------");
        TestStep("Let DUT go to next operation cycle.");
        StopSimCarTiGlb();
        StopSimBkpOfDstTrvld();
        StopSimTiAndDateIndn();
        if (gCndtnOfClrDtc < 2)
        {
            com.del_cyclic_message_canfd(&Sim_VehSpd);
            gVehSpdSimFlag = 0;
        }
        NM_NextOperation();

        //step60
        TestStep("Simulate all global snapshot data related messages to the bus and the value of related signals is min valid value.");
        Snapshot_SigValueChange(gCarTiGlb_min, gBkpOfDstTrvld_min, gVehSpd_min, gTiAndDateIndn_min, sizeof(gTiAndDateIndn_min) / sizeof(gTiAndDateIndn_min[0]));
        SimCarTiGlb();//DD00
        SimBkpOfDstTrvld();//DD01
        SimTiAndDateIndn();//DD07
        if (gCndtnOfClrDtc < 2)
        {
            SimVehMtnSpd_Creat();
            gVehSpdSimFlag = 1;
        }

        //step61
        volt = highVoltDTC.volt + 0.35;
        snprintf(StepDespriction, sizeof(StepDespriction), "Wait 10s  Set voltage to %5.2fV HighVolt+0.35V", volt);
        TestStep(StepDespriction);
        TestWait(10000);
        SetVoltage(volt);
        snprintf(StepDespriction, sizeof(StepDespriction), "Get Current Voltage is %.3lf", GetVoltage());
        TestReport_HTML_StepDescription(StepDespriction);

        //step62
        time = ceil(127 / highVoltDTC.SUValue) * highVoltDTC.testProd * 1.2;
        snprintf(StepDespriction, sizeof(StepDespriction), "wait for %lfms (127/StepUpValue*TestPeriod*1.2)", time);
        TestStep(StepDespriction);
        TestWait(time);

        //step63
        TestStep("Set voltage to 12V and wait for 5s, then read DTC by 19 02 03");
        SetVoltage(12);
        snprintf(StepDespriction, sizeof(StepDespriction), "Get Current Voltage is %.3lf", GetVoltage());
        TestReport_HTML_StepDescription(StepDespriction);
        if (gCEMFlg != 1)
            SimVehBat(gUbat12);
        TestWait(5000);
        RX_SID19_ReportDTCByStatusMask(0x03);
        RX_CheckResponse_DTC_SID19_02(0x03, highVoltDTC.dtc, 1);

        //step64
        TestStep("Read time stamp #20, #21 of test DTC");
        CheckTimeStamp(0x20, fdtc, gCarTiGlb_valid);
        CheckTimeStamp(0x21, fdtc, gCarTiGlb_min);
        //CheckTimeStamp(0x22, fdtc, gTiAndDateIndn_valid);
        //CheckTimeStamp(0x23, fdtc, gTiAndDateIndn_min);

        //step65
        TestStep("Simulate all global snapshot data related messages to the bus and the value of related signals is max valid value.");
        Snapshot_SigValueChange(gCarTiGlb_max, gBkpOfDstTrvld_max, gVehSpd_max, gTiAndDateIndn_max, sizeof(gTiAndDateIndn_max) / sizeof(gTiAndDateIndn_max[0]));
        TestWait(5000);

        //step66
        volt = highVoltDTC.volt + 0.35;
        snprintf(StepDespriction, sizeof(StepDespriction), "Set voltage to %5.2fV HighVolt+0.35V", volt);
        TestStep(StepDespriction);
        SetVoltage(volt);
        snprintf(StepDespriction, sizeof(StepDespriction), "Get Current Voltage is %.3lf", GetVoltage());
        TestReport_HTML_StepDescription(StepDespriction);

        //step67
        time = ceil(127 / highVoltDTC.SUValue) * highVoltDTC.testProd * 1.2;
        snprintf(StepDespriction, sizeof(StepDespriction), "wait for %lfms (127/StepUpValue*TestPeriod*1.2)", time);
        TestStep(StepDespriction);
        TestWait(time);

        //step68
        TestStep("Set voltage to 12V and wait for 5s, then read DTC by 19 02 03");
        SetVoltage(12);
        snprintf(StepDespriction, sizeof(StepDespriction), "Get Current Voltage is %.3lf", GetVoltage());
        TestReport_HTML_StepDescription(StepDespriction);
        if (gCEMFlg != 1)
            SimVehBat(gUbat12);
        TestWait(5000);
        RX_SID19_ReportDTCByStatusMask(0x03);
        RX_CheckResponse_DTC_SID19_02(0x03, highVoltDTC.dtc, 1);

        //step69
        TestStep("Read time stamp #20, #21 of test DTC");
        CheckTimeStamp(0x20, fdtc, gCarTiGlb_valid);
        CheckTimeStamp(0x21, fdtc, gCarTiGlb_min);
        //CheckTimeStamp(0x22, fdtc, gTiAndDateIndn_valid);
        //CheckTimeStamp(0x23, fdtc, gTiAndDateIndn_min);

        /*-------------------------------------------------Test correct value not available in new OC---------------------------------------------------------------*/
        TestReport_HTML_StepDescription("---------correct value not available in different OCs---------");
        for (u8 i = 0; i < 2; i++)
        {
            if (i == 0)
            {
                //step70
                stepnumber = 69;
                TestStep("Let DUT go to next operation cycle.");
                StopSimCarTiGlb();
                StopSimBkpOfDstTrvld();
                StopSimTiAndDateIndn();
                if (gCndtnOfClrDtc < 2)
                {
                    com.del_cyclic_message_canfd(&Sim_VehSpd);
                    gVehSpdSimFlag = 0;
                }
                NM_NextOperation();
            }
            else
            {
                //step70
                stepnumber = 69;
                TestStep("Reset power supply.");
                NMWakeUp(0);
                SendTesterPresent(TP_OFF);
                KL30(off);
                TestWait(5000);
                KL30(on);
                FulfilTRC();
                TRCSwitch(0, 0x0D, 0x0);
            }
            TestWait(10000);

            //step71
            time = ceil(127 / highVoltDTC.SUValue) * highVoltDTC.testProd * 1.2 + 5000;
            snprintf(StepDespriction, sizeof(StepDespriction), "Clear DTCs with diagnostic request, then wait for %dms (127/StepUpValue*TestPeriod*1.2+5s)", time);
            TestStep(StepDespriction);
            SID14_ClearAllDTC();
            CheckPosResponse_SID14();
            TestWait(time);

            //step72
            volt = highVoltDTC.volt + 0.35;
            time = ceil(127 / highVoltDTC.SUValue) * highVoltDTC.testProd * 1.2;
            snprintf(StepDespriction, sizeof(StepDespriction), "Set voltage to %5.2fV HighVolt+0.35V then wait for %lfms (127/StepUpValue*TestPeriod*1.2)", volt, time);
            TestStep(StepDespriction);
            TestWait(10000);
            SetVoltage(volt);
            snprintf(StepDespriction, sizeof(StepDespriction), "Get Current Voltage is %.3lf", GetVoltage());
            TestReport_HTML_StepDescription(StepDespriction);
            TestWait(time);

            //step73
            TestStep("Set voltage to 12V and wait for 5s, then read DTC by 19 02 03");
            SetVoltage(12);
            snprintf(StepDespriction, sizeof(StepDespriction), "Get Current Voltage is %.3lf", GetVoltage());
            TestReport_HTML_StepDescription(StepDespriction);
            if (gCEMFlg != 1)
                SimVehBat(gUbat12);
            TestWait(5000);
            RX_SID19_ReportDTCByStatusMask(0x03);
            RX_CheckResponse_DTC_SID19_02(0x03, highVoltDTC.dtc, 1);

            //step74
            snprintf(StepDespriction, sizeof(StepDespriction), "Read Snapshot Record of test DTC by 0x%X", gSnapshotRecordNo);
            TestStep(StepDespriction);
            ReadSnapShot(fdtc);
            if (gCndtnOfClrDtc < 2)
                CheckSnapshotDataValue(fdtc, 0xFFFFFFFF, 0xFFFFFF, 0x30, 0xFFFF, gNotAvailableArray, gUagMode, gAllPNC, 0, gCarMode);
            else
                CheckSnapshotDataValue(fdtc, 0xFFFFFFFF, 0xFFFFFF, 0x30, gVehSpd, gNotAvailableArray, gUagMode, gAllPNC, 0, gCarMode);
        }

        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 TestCase_1365372_DTCTestWhenECUResetAfterDTCCleared() {
    int index, j = 0;
    double time, volt;
    u8 dtc[3] = { 0xff,0xff,0xff };
    char StepDespriction[256];
    int occ6, occ2;
    u8 fdtc[3];

    strcpy(TestCaseName, " TestCase_1365372_DTC Test When ECU Reset After DTC Cleared");
    strcpy(TestScopeName, "Network fault related DTC Test - Scenario Test");
    strcpy(testPurpose_CH, "测试目的：This testcase is to check DTC information can be cleared successfully when ECU reset after DTC is cleared by diagnostic service.");

    if (0 != TestPrecondition())
        return -1;
    try
    {
        FulfilTRC();
        TRCSwitch(0x0, 0xD, 0x0);

        //step2
        TestStep("Wait for 7s");
        TestWait(7000);

        //step3
        time = time = ceil(127 / highVoltDTC.SUValue) * highVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "Clear DTCs with diagnostic request, then wait for %lfms (127/StepUpValue*TestPeriod*1.2+5s)", time);
        TestStep(StepDespriction);
        SID14_ClearAllDTC();
        CheckPosResponse_SID14();
        TestWait(time);

        //step4
        TestStep("Read DTC from the DUT with status mask 0x09.");
        RX_SID19_ReportDTCByStatusMask(0x09);
        RX_CheckResponse_DTC_SID19_02(0x09, highVoltDTC.dtc, 0);


        //step5
        volt = highVoltDTC.volt + 0.35;
        snprintf(StepDespriction, sizeof(StepDespriction), "Set voltage to %5.2fV HighVolt+0.35V", volt);
        TestStep(StepDespriction);
        SetVoltage(volt);
        //snprintf(StepDespriction, sizeof(StepDespriction), "Get Current Voltage is %.3lf", GetVoltage());
        //TestReport_HTML_StepDescription(StepDespriction);

        //step6
        time = ceil(127 / highVoltDTC.SUValue) * highVoltDTC.testProd * 1.2;
        snprintf(StepDespriction, sizeof(StepDespriction), "wait for %lfms (127/StepUpValue*TestPeriod*1.2)", time);
        TestStep(StepDespriction);
        TestWait(time);

        //step7
        TestStep("Set voltage to 12V and wait for 5s, then read DTC by 19 02 03");
        SetVoltage(12);
        //snprintf(StepDespriction, sizeof(StepDespriction), "Get Current Voltage is %.3lf", GetVoltage());
        //TestReport_HTML_StepDescription(StepDespriction);
        if (gCEMFlg != 1)
            SimVehBat(gUbat12);
        TestWait(5000);
        RX_SID19_ReportDTCByStatusMask(0x03);
        RX_CheckResponse_DTC_SID19_02(0x03, highVoltDTC.dtc, 1);

        //step8
        time = time = ceil(127 / highVoltDTC.SUValue) * highVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "Clear DTCs with diagnostic request, then wait for %.0lfms (127/StepUpValue*TestPeriod*1.2+5s)", time);
        TestStep(StepDespriction);
        SID14_ClearAllDTC();
        CheckPosResponse_SID14();
        TestWait(time);

        //step9
        TestStep("Read DTC from the DUT with status mask 0x0F.");
        RX_SID19_ReportDTCByStatusMask(0x0F);
        RX_CheckResponse_DTC_SID19_02(0x0F, highVoltDTC.dtc, 0);

        //step10
        TestStep("Reset the power supply of DUT.");
        PowerReset();
        FulfilTRC();

        //step11
        TestStep("Read DTC from the DUT with status mask 0x0F.");
        RX_SID19_ReportDTCByStatusMask(0x0F);
        RX_CheckResponse_DTC_SID19_02(0x0F, highVoltDTC.dtc, 0);

        //step12
        volt = highVoltDTC.volt + 0.35;
        snprintf(StepDespriction, sizeof(StepDespriction), "Set voltage to %5.2fV HighVolt+0.35V", volt);
        TestStep(StepDespriction);
        SetVoltage(volt);
        //snprintf(StepDespriction, sizeof(StepDespriction), "Get Current Voltage is %.3lf", GetVoltage());
        //TestReport_HTML_StepDescription(StepDespriction);

        //step13
        time = ceil(127 / highVoltDTC.SUValue) * highVoltDTC.testProd * 1.2;
        snprintf(StepDespriction, sizeof(StepDespriction), "wait for %lfms (127/StepUpValue*TestPeriod*1.2)", time);
        TestStep(StepDespriction);
        TestWait(time);

        //step14
        TestStep("Set voltage to 12V and wait for 5s, then read DTC by 19 02 03");
        SetVoltage(12);
        //snprintf(StepDespriction, sizeof(StepDespriction), "Get Current Voltage is %.3lf", GetVoltage());
        //TestReport_HTML_StepDescription(StepDespriction);
        if (gCEMFlg != 1)
            SimVehBat(gUbat12);
        TestWait(5000);
        RX_SID19_ReportDTCByStatusMask(0x03);
        RX_CheckResponse_DTC_SID19_02(0x03, highVoltDTC.dtc, 1);

        //step15
        time = time = ceil(127 / highVoltDTC.SUValue) * highVoltDTC.testProd * 1.2 + 5000;
        snprintf(StepDespriction, sizeof(StepDespriction), "Clear DTCs with diagnostic request, then wait for %dms (127/StepUpValue*TestPeriod*1.2+5s)", time);
        TestStep(StepDespriction);
        SID14_ClearAllDTC();
        CheckPosResponse_SID14();
        TestWait(time);

        //step16
        TestStep("Read DTC from the DUT with status mask 0x0F.");
        RX_SID19_ReportDTCByStatusMask(0x0F);
        RX_CheckResponse_DTC_SID19_02(0x0F, highVoltDTC.dtc, 0);

        //step17
        TestStep("Send a diagnostic request to the DUT to perform a ECUReset(hardReset) once.");
        SID11_ECUReset(0x01);
        CheckPosResponse_SID11(0x01);
        TestWait(ECUResetTime);
        FulfilTRC();

        //step18
        TestStep("Read DTC from the DUT with status mask 0x0F.");
        RX_SID19_ReportDTCByStatusMask(0x0F);
        RX_CheckResponse_DTC_SID19_02(0x0F, highVoltDTC.dtc, 0);

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

s32 TestCase_1022850_DetectionOfFaultyLINResponse() {
    return 0;
}

s32 TestCase_1022852_DetectionOfMissingLINResponse()
{
    int index = 0, j = 0;
    double time;
    u8 dtc[3] = { 0xff,0xff,0xff };
    char StepDespriction[256];

    strcpy(TestCaseName, "TestCase_1022852_DetectionOfMissingLINResponse");
    strcpy(TestScopeName, "LIN Master - UDSonLIN");
    strcpy(testPurpose_CH, "测试目的：");
    if (0 != TestPrecondition())
        return -1;


    //TestStep("上电并唤醒被测件");
    //if (0 != CheckCommunication(5000)) {
    //    TestPostCondition();
    //    return -1;
    //}
    try
    {
        carConfigType = MissResponse;
        FulfilTRC();
        //Step1
        for (index = 0; index < MISRESP_DTC_NUM; index++)
        {
            stepnumber = 0;
            dtc[0] = (misResponseDTCList[index].dtc & 0xff0000) >> 16;
            dtc[1] = (misResponseDTCList[index].dtc & 0xff00) >> 8;
            dtc[2] = (misResponseDTCList[index].dtc & 0xff);
            index_cc2 = 0;
            index_cc1 = 0;
            UsgModSwtich(InActive);
            AppWait(5000,"");
            UsgModSwtich(Driving);
            simulateCCP(false);
            UsgModSwtich(InActive);
            snprintf(StepDespriction, sizeof(StepDespriction), "-------------------------Test missing node %s:0x%X---------------------------", misResponseDTCList[index].node, misResponseDTCList[index].dtc);
            TestReport_HTML_StepDescription(StepDespriction, Info);
            for (int k = 0; k < 600; k++)
            {
                gCCPDataExt[k] = 0;
                gCCPData[k] = 0;
            }
           
            TRCSwitch(0x0, 0xD, 0x0);
            TestReport_HTML_StepDescription("Start Simulate CCP", Info);
            simulateCCP();

            //step1
            TestReport_HTML_StepDescription("star the simulation of test DTC related messages");
            SimulateLINResponse(index, true);

            //step2
            stepnumber = 1;
            TestStep("Clear DTCs");
            SID14_ClearAllDTC();
            CheckPosResponse_SID14();

            //step3
            TestStep("Read DTC from the DUT with status mask 0x09");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, misResponseDTCList[index].dtc, 0);

            //step4
            TestStep("Stop simulating 5 TestPeriod sent from test slave node");
            SimulateLINResponse(index, false);

            //step5
            TestStep("Read FDC10 of the test DTC,FDC10 should be 5*StepUpValue");
            CheckStepUpValueByIndex(5,dtc, misResponseDTCList[index].testProd, misResponseDTCList[index].SUValue);

            //step6
            TestStep("Resume the simulation of test slave. Wait for 5s.");
            SimulateLINResponse(index, true);
            AppWait(5000, "");

            //step7
            TestStep("Stop simulating frames sent from test slave node for (127 / StepUpValue * TestPeriod * 0.8).");
            time = ceil(127 / misResponseDTCList[index].SUValue * misResponseDTCList[index].testProd * 0.8);
            SimulateLINResponse(index, false);
            AppWait(time, "");

            //step8
            TestStep("Read DTC from the DUT with status mask 0x03");
            RX_SID19_ReportDTCByStatusMask(0x03);
            RX_CheckResponse_DTC_SID19_02(0x03, misResponseDTCList[index].dtc, 0);

            //step9
            TestStep("Resume the simulation of test slave. Wait for 5s.");
            SimulateLINResponse(index, true);
            AppWait(5000, "");

            //step10
            TestStep("Stop simulating frames sent from test slave node for (127 / StepUpValue * TestPeriod * 1.2).");
            time = ceil(127 / misResponseDTCList[index].SUValue * misResponseDTCList[index].testProd * 1.2);
            SimulateLINResponse(index, false);
            AppWait(time, "");

            //step11
            TestStep("Read DTC from the DUT with status mask 0x03");
            RX_SID19_ReportDTCByStatusMask(0x03);
            RX_CheckResponse_DTC_SID19_02(0x03, misResponseDTCList[index].dtc, 1);

            //step12
            TestStep("Read FDC10 of the test DTC");
            if (0x7F == ReadFDC10(dtc)) {
                TestReport_HTML_StepDescription("FDC10 is 0x7F",Pass);
            }
            else {
                TestReport_HTML_StepDescription("FDC10 is not 0x7F", Fail);
            }

            //step13
            TestStep("Resume the simulation of test slave. Wait for 5 TestPeriod of simulation frame.");
            SimulateLINResponse(index, true);

            //step14
            TestStep("Read FDC10 of the test DTC,FDC10 should be -5* StepDownValue");
            CheckStepDownValueByIndex(5, dtc, misResponseDTCList[index].testProd, misResponseDTCList[index].SUValue);

            //step15
            TestStep("Wait for 5s");
            AppWait(5000, "");

            //step16
            TestStep("Read DTC from the DUT with status mask 0x01");
            RX_SID19_ReportDTCByStatusMask(0x01);
            RX_CheckResponse_DTC_SID19_02(0x01, misResponseDTCList[index].dtc, 0);

            //step17
            TestStep("Read FDC10 of the test DTC");
            if (0x80 == ReadFDC10(dtc)) {
                TestReport_HTML_StepDescription("FDC10 is 0x80", Pass);
            }
            else {
                TestReport_HTML_StepDescription("FDC10 is not 0x80", Fail);
            }
            UsgModSwtich(0x01);
            SimulateLINResponse(index, false);
            index_cc2 = 0;
            stepnumber = 1;
        }

        index_cc2 = 0;
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

s32 TestCase_1365362_UnconfirmedDTCLimitTestOfMissingLINResponseDTC()
{
    int index = 0, j = 0;
    double time;
    u8 fdtc[3] = { 0xff,0xff,0xff };
    char StepDespriction[256];

    int FDC10;
    int snapShot;
    int unCfmBit;
    int unCfmSncLstClr;
    int unCfmThisOpCyc;

    strcpy(TestCaseName, "TestCase_1365362_UnconfirmedDTCLimitTestOfMissingLINResponseDTC");
    strcpy(TestScopeName, "LIN Master - UDSonLIN");
    strcpy(testPurpose_CH, "测试目的：");
    if (0 != TestPrecondition())
        return -1;


    TestStep("上电并唤醒被测件");
    if (0 != CheckCommunication(5000)) {
        TestPostCondition();
        return -1;
    }
    try
    {
        carConfigType = MissResponse;
        FulfilTRC();
        //Step1
        for (index = 0; index < MISRESP_DTC_NUM; index++)
        {
            stepnumber = 0;
            index_cc2 = 0;
            index_cc1 = 0;
            UsgModSwtich(InActive);
            AppWait(5000, "");
            UsgModSwtich(Driving);
            simulateCCP(false);
            UsgModSwtich(InActive);
            snprintf(StepDespriction, sizeof(StepDespriction), "-------------------------Test missing node %s:0x%X---------------------------", misResponseDTCList[index].node, misResponseDTCList[index].dtc);
            TestReport_HTML_StepDescription(StepDespriction, Info);

            for (int k = 0; k < 600; k++)
            {
                gCCPDataExt[k] = 0;
                gCCPData[k] = 0;
                gCCPDataExt2[k] = 0;
            }

            FulfilTRC();
            TRCSwitch(0x0, 0xD, 0x0);
            TestReport_HTML_StepDescription("Start Simulate CCP", Info);
            simulateCCP();

            fdtc[0] = (misResponseDTCList[index].dtc & 0xff0000) >> 16;
            fdtc[1] = (misResponseDTCList[index].dtc & 0xff00) >> 8;
            fdtc[2] = (misResponseDTCList[index].dtc & 0xff);
            //step1
            stepnumber = 0;
            TestStep("simulated slave nodes respond to all headers sent by the master");
            SimulateLINResponse(index, true);

            //step2
            TestStep("Clear DTCs");
            SID14_ClearAllDTC();
            CheckPosResponse_SID14();

            //step3
            TestStep("Read DTC from the DUT with status mask 0x09");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, misResponseDTCList[index].dtc, 0);

            //step4
            TestStep("Stop simulating frames sent from test slave node.");
            SimulateLINResponse(index, false);

            //step5
            TestStep("Send diagnostic request with testProd to read FDC10 and Snapshot data, until FDC10 >= UnconfirmedDTCLimit");
            gUnCofrmLmt = misResponseDTCList[index].UnCofrmLmt;
            CheckStepUpValue(fdtc, misResponseDTCList[index].testProd, misResponseDTCList[index].SUValue);

            //Step6
            TestStep("Read DTC by 19 02 01");
            RX_SID19_ReportDTCByStatusMask(0x01);
            RX_CheckResponse_DTC_SID19_02(0x01, misResponseDTCList[index].dtc, 1);

            //Ste7
            TestStep("Resume the simulation of test slave");
            SimulateLINResponse(index,true);

            //Step8
            TestStep("Send diagnostic request with period TestPeriod to read FDC10");
            CheckStepDownValue(fdtc, misResponseDTCList[index].testProd, misResponseDTCList[index].SDValue);

        }
        index_cc2 = 0;
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

s32 TestCase_1365364_ConfirmedDTCLimitAndAgedDTCLimittestofmissingLINResponseDTC()
{
    int index, j = 0;
    double time;
    u8 dtc[3] = { 0xff,0xff,0xff };
    char StepDespriction[256];
    int occ6, occ2;
    u8 fdtc[3];

    strcpy(TestCaseName, " TestCase_1365364_ConfirmedDTCLimitAndAgedDTCLimittestofmissingLINResponseDTC");
    strcpy(TestScopeName, "LIN Master - UDSonLIN");
    strcpy(testPurpose_CH, "测试目的：");

    if (0 != TestPrecondition())
        return -1;

    try
    {
        FulfilTRC();
        for (index = 0; index < MISRESP_DTC_NUM; index++)
        {
            if (misResponseDTCList[index].tstRsltFlg == 0) {
                continue;
            }
            snprintf(StepDespriction, sizeof(StepDespriction), "-------------------------Test missing node %s:0x%X---------------------------", misMsgDTCList[index].node, misMsgDTCList[index].dtc);
            TestReport_HTML_StepDescription(StepDespriction, Info);
            for (int k = 0; k < 600; k++)
            {
                gCCPDataExt[k] = 0;
                gCCPData[k] = 0;

            }
            TRCSwitch(0x0, 0xD, 0x0);

            TestReport_HTML_StepDescription("Start Simulate CCP", Info);

            simulateCCP();

            fdtc[0] = (misResponseDTCList[index].dtc & 0xff0000) >> 16;
            fdtc[1] = (misResponseDTCList[index].dtc & 0xff00) >> 8;
            fdtc[2] = (misResponseDTCList[index].dtc & 0xff);

            //step1
            stepnumber = 0;
            TestStep("Simulate related messages");
            SimulateLINResponse(index, true);

            //step2
            TestStep("Clear DTCs");
            SID14_ClearAllDTC();
            CheckPosResponse_SID14();

            //step3
            TestStep("Read DTC from the DUT with status mask 0x09");
            RX_SID19_ReportDTCByStatusMask(0x09);
            RX_CheckResponse_DTC_SID19_02(0x09, misResponseDTCList[index].dtc, 0);

            for (j = 0; j < misResponseDTCList[index].CofrmLmt; j++) {
                //step4
                stepnumber = 3;
                TestStep("Stop simulating frames sent from test slave node for (127 / StepUpValue * TestPeriod * 1.2)");
                time = ceil(127 / misResponseDTCList[index].SUValue) * misResponseDTCList[index].testProd * 1.2;
                SimulateLINResponse(index, false);
                AppWait(time, "");

                //step5
                TestStep("Read DTC from the DUT with status mask 0x01.");
                RX_SID19_ReportDTCByStatusMask(0x01);
                RX_CheckResponse_DTC_SID19_02(0x01, misResponseDTCList[index].dtc, 1);

                //step6
                TestStep("Resume the simulation of test slave. Wait for 5s.");
                SimulateLINResponse(index, true);

                //step7
                TestStep("Read DTC from the DUT with status mask 0x08.");
                RX_SID19_ReportDTCByStatusMask(0x08);
                if (j < misResponseDTCList[index].CofrmLmt - 1) {
                    RX_CheckResponse_DTC_SID19_02(0x08, misResponseDTCList[index].dtc, 0);
                }
                else {
                    RX_CheckResponse_DTC_SID19_02(0x08, misResponseDTCList[index].dtc, 1);
                }

                //step8
                TestStep("Let DUT go to next operation cycle.");
                NM_NextOperation();
            }
            
            
            for (j = 0; j <= misResponseDTCList[index].agdLmt; j++) {
                SimulateLINResponse(index, true);
                //step10
                stepnumber = 9;
                TestStep("Wait for (127 / StepDownValue * TestPeriod * 1.2 + 5s), read OCC2 of test DTC");
                time = ceil(127 / misResponseDTCList[index].SDValue) * misResponseDTCList[index].testProd * 1.2 + 5000;
                AppWait(time, "");
                occ2 = ReadOCC2(fdtc);

                if (occ2 == j) {
                    snprintf(StepDespriction, sizeof(StepDespriction), "OCC2 is %d", occ2);
                    TestReport_HTML_StepDescription(StepDespriction, Pass);
                }
                else {
                    snprintf(StepDespriction, sizeof(StepDespriction), "OCC2 is %d   should be %d", occ2,j);
                    TestReport_HTML_StepDescription(StepDespriction, Fail);
                }

                if ((occ2 == 0) && (j == 10))
                {
                    snprintf(StepDespriction, sizeof(StepDespriction), "OCC%d keeps to 0, break the test.", gOcc2Or5);
                    TestReport_HTML_StepDescription(StepDespriction, Fail);
                    break;
                }

                //step11
                TestStep("Read DTC from the DUT with status mask 0x08.");
                RX_SID19_ReportDTCByStatusMask(0x08);
                RX_CheckResponse_DTC_SID19_02(0x08, misResponseDTCList[index].dtc, 1);

                //step12
                TestStep("Let DUT go to next operation cycle.");
                NM_NextOperation();
            }

        }

        if (index == MISRESP_DTC_NUM)
        {
            strcpy(StepDespriction, "All DTC is abnormal, break the test.");
            TestReport_HTML_Step(StepNumber, StepDespriction, Fail);
        }
        index_cc2 = 0;
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
#endif // !TestCase_CAN_DTC