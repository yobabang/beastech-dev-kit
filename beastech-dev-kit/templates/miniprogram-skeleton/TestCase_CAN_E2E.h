
#ifndef TestCase_CAN_E2E
#define TestCase_CAN_E2E

#include "test_case_diag_api.h"
#include "test_function.h"

s32 TC1023183_CRC_by_sender_CANCANFD()
{
    try
    {
        //s32 CH = 1;
        //低电压情况下，将NVM数据写入
        strcpy(TestCaseName, "TC1023183_CRC_by_sender_CANCANFD");
        strcpy(TestScopeName, "E2E");
        strcpy(testPurpose_CH, "E2E");

        //TestStep("测试开始");
        PreE2Econdition();
        u8 channelCount = 0;
        if (gCEMFlg == 1) {
            channelCount = 8;
        }
        else {
            channelCount = 4;
        }
        for (s32 k = 0; k < channelCount; k++) {

            TestReport_HTML_StepDescription("", Info);
            sprintf(requestInfo, "当前通道网段名：%s", DUT_def_E2E_SG_TX_2D_Array[k].FName);
            TestReport_HTML_StepDescription(requestInfo, Info);

            int TestNo = 0;
            for (TestNo = 0; TestNo < DUT_def_E2E_SG_TX_2D_Array[k].DUT_def_E2E_SG_TX.size(); TestNo++)//DUT_def_E2E_SG_TX.size()
            {
                stepnumber = 0;
                TestReport_HTML_StepDescription("", Info);
                sprintf(requestInfo, "TestNo=%d,,---------------遍历所有E2E_TXMsg，当前测试报文ID为0x%0X---------------", TestNo + 1, DUT_def_E2E_SG_TX_2D_Array[k].DUT_def_E2E_SG_TX[TestNo].MsgID);
                TestReport_HTML_StepDescription(requestInfo, Info);

                TestMsgID = DUT_def_E2E_SG_TX_2D_Array[k].DUT_def_E2E_SG_TX[TestNo].MsgID;
                printf("TestMsgID=%0X", TestMsgID);
                //TestCH = DUT_def_E2E_SG_TX_2D_Array[CH].DUT_def_E2E_SG_TX[TestNo].channel;
                TestCH = k + 1;
                printf("CH=%d,TestCH=%d", k, TestCH);

                ClearAllVector();
                InitAllFlag();

                TestStep("KL30上电");
                KL30(on);
                app.wait(500, "");

                TestStep("唤醒DUT，触发某个E2E_TxMsg");
                NMWakeUp(1);
                TiggerDUTSendE2E_TXMsg();

                TestStep("检测第一帧E2E_TXMsg的CRC");
                WaitFlag(E2E_RX_Flag, CheckTime);
                CheckFirstCRC(DUT_def_E2E_SG_TX_2D_Array[k].DUT_def_E2E_SG_TX);

                TestStep("检测接下来20帧E2E_TXMsg的CRC");
                MessageWait(21, WaitTime);
                CheckCRCNumber(1, 21, DUT_def_E2E_SG_TX_2D_Array[k].DUT_def_E2E_SG_TX);

                TestStep("满足控制器休眠条件");
                NMWakeUp(0);
                //NMWakeUp(0);
                com.del_cyclic_messages();
                app.wait(5000, "");

                TestStep("KL30下电");
                KL30(off);
                app.wait(500, "");
            }


        }

        TestStep("测试结束");
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


s32 TC1023184_Counter_by_sender_CANCANFD()
{
    try
    {
        strcpy(TestCaseName, "TC1023184_Counter_by_sender_CANCANFD");
        strcpy(TestScopeName, "E2E");
        strcpy(testPurpose_CH, "E2E");

        PreE2Econdition();
        u8 channelCount = 0;
        if (gCEMFlg == 1) {
            channelCount = 8;
        }
        else {
            channelCount = 4;
        }
        for (s32 k = 0; k < channelCount; k++) {

            TestReport_HTML_StepDescription("", Info);
            sprintf(requestInfo, "当前通道网段名：%s", DUT_def_E2E_SG_TX_2D_Array[k].FName);
            TestReport_HTML_StepDescription(requestInfo, Info);
            int TestNo = 0;//用于测试步骤中的遍历
            for (TestNo = 0; TestNo < DUT_def_E2E_SG_TX_2D_Array[k].DUT_def_E2E_SG_TX.size(); TestNo++)
            {
                stepnumber = 0;
                TestReport_HTML_StepDescription("", Info);
                sprintf(requestInfo, "TestNo=%d,,---------------遍历所有E2E_TXMsg，当前测试报文ID为0x%0X---------------", TestNo + 1, DUT_def_E2E_SG_TX_2D_Array[k].DUT_def_E2E_SG_TX[TestNo].MsgID);
                TestReport_HTML_StepDescription(requestInfo, Info);
                //TestInfoPrint("---------------遍历所有E2E_TXMsg，当前测试报文ID为0x%0X---------------", DUT_def_E2E_SG_TX[TestNo].MsgID);

                TestMsgID = DUT_def_E2E_SG_TX_2D_Array[k].DUT_def_E2E_SG_TX[TestNo].MsgID;
                //TestCH = DUT_def_E2E_SG_TX_2D_Array[k].DUT_def_E2E_SG_TX[TestNo].channel;
                TestCH = k + 1;
                printf("CH=%d,TestCH=%d", k, TestCH);
                ClearAllVector();
                InitAllFlag();

                TestStep("KL30上电");
                KL30(on);
                app.wait(500, "");

                TestStep("唤醒DUT，触发当前测试E2E_TXMsg");

                NMWakeUp(1);
                TiggerDUTSendE2E_TXMsg();

                TestStep("检测接下来20帧E2E_TXMsg的Counter");
                MessageWait(20, WaitTime);
                CheckCounterNumber(0, 20, DUT_def_E2E_SG_TX_2D_Array[k].DUT_def_E2E_SG_TX);

                TestStep("满足控制器休眠条件");
                NMWakeUp(0);
                com.del_cyclic_messages();
                app.wait(5000, "");

                TestStep("KL30下电");
                KL30(off);
                app.wait(500, "");
            }


        }



        TestStep("测试结束");
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


//短路问题，设备如何使用
s32 TC1023185_Counter_by_sender_CANCANFD_short_recovery()
{
    try
    {


        strcpy(TestCaseName, "TC1023185_Counter_by_sender_CANCANFD_short_recovery");
        strcpy(TestScopeName, "E2E");
        strcpy(testPurpose_CH, "E2E");

        //TestStep("测试开始");
        PreE2Econdition();

        int TestNo = 0;//用于测试步骤中的遍历
        int WaitFrameNumber = 0;
        u8 channelCount = 0;
        TestStep("KL30上电");
        KL30(on);
        app.wait(500, "");
        if (gCEMFlg == 1) {
            channelCount = 8;
        }
        else {
            channelCount = 4;
        }
        for (s32 k = 0; k < channelCount; k++) {

            TestReport_HTML_StepDescription("", Info);
            sprintf(requestInfo, "当前通道网段名：%s", DUT_def_E2E_SG_TX_2D_Array[k].FName);
            TestReport_HTML_StepDescription(requestInfo, Info);
            for (TestNo = 0; TestNo < DUT_def_E2E_SG_TX_2D_Array[k].DUT_def_E2E_SG_TX.size(); TestNo++)
            {
                stepnumber = 0;
                TestReport_HTML_StepDescription("", Info);
                sprintf(requestInfo, "TestNo=%d,,---------------遍历所有E2E_TXMsg，当前测试报文ID为0x%0X---------------", TestNo + 1, DUT_def_E2E_SG_TX_2D_Array[k].DUT_def_E2E_SG_TX[TestNo].MsgID);
                TestReport_HTML_StepDescription(requestInfo, Info);

                TestMsgID = DUT_def_E2E_SG_TX_2D_Array[k].DUT_def_E2E_SG_TX[TestNo].MsgID;
                printf("TestMsgID=%0X", TestMsgID);
                //TestCH = DUT_def_E2E_SG_TX_2D_Array[CH].DUT_def_E2E_SG_TX[TestNo].channel;
                TestCH = k + 1;
                printf("CH=%d,TestCH=%d", k, TestCH);

                ClearAllVector();
                InitAllFlag();

                TestStep("唤醒DUT，触发当前测试E2E_TXMsg");
                NMWakeUp(1);
                SIMNMMessage();
                TiggerDUTSendE2E_TXMsg();

                TestStep("维持通信15s");
                app.wait(15000, "");

                //短路函数
                TestStep("短路CANH和CANL，持续1.5s");
                Short_CANH_To_CANL();
                app.wait(1500, "");

                //移除短路函数
                TestStep("移除短路，等待DUT恢复通信");
                ShortRecovery();
                //app.wait(60000, "");
                WaitDUTCommunicationRecovery();
                //TestReport_HTML_StepDescription("短路故障移除后60s,等待UT通信", Fail);

                TestStep("检测接下来20帧E2E_TXMsg的Counter");
                WaitFrameNumber = MessageWaitFromNow(20, WaitTime);
                CheckCounterNumber(WaitFrameNumber - 21, WaitFrameNumber - 1, DUT_def_E2E_SG_TX_2D_Array[k].DUT_def_E2E_SG_TX);

                TestStep("满足控制器休眠条件");
                NMWakeUp(0);
                com.del_cyclic_messages();
                app.wait(5000, "");
            }

        }

        TestStep("测试结束");
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

//DUT收的
s32 TC1023186_CRC_by_Receiver_on_CANCANFD()
{
    try
    {
        int TestNo, TestSGNo = 0;//用于测试步骤中的遍历
        int resDTC = 0;
        u32 E2EDTC = 0;

        strcpy(TestCaseName, "TC1023186_CRC_by_Receiver_on_CANCANFD");
        strcpy(TestScopeName, "E2E");
        strcpy(testPurpose_CH, "E2E");

        //TestStep("测试开始");
        PreE2Econdition();

        for (s32 k = 0; k < 6; k++) {

            TestReport_HTML_StepDescription("", Info);
            sprintf(requestInfo, "当前通道网段名：%s", DUT_def_E2E_SG_RX_2D_Array[k].FName);
            TestReport_HTML_StepDescription(requestInfo, Info);

            for (TestNo = 0; TestNo < DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX.size(); TestNo++)
            {
                TestMsgID = DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].MsgID;
                //TestCH = DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].channel;
                TestCH = k + 1;
                printf("CH=%d,TestCH=%d", k, TestCH);
                //DTC是针对信号组的DTC，后续更改
                for (TestSGNo = 0; TestSGNo < DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].SGInstance.size(); TestSGNo++)//DUT_def_E2E_SG_RX[TestNo].SGInstance.size()
                {
                    stepnumber = 0;

                    TestSGName = DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].SGInstance[TestSGNo].SGName;
                    E2EDTC = DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].SGInstance[TestSGNo].ChksDTC;

                    //如果DTC为0，表示没有此DTC，跳过
                    if (E2EDTC == 0)
                    {
                        continue;
                    }
                    resDTC = 1;

                    TestReport_HTML_StepDescription("", Info);
                    sprintf(requestInfo, "TestNo=%d,,---------------遍历所有E2E_TXMsg，当前测试报文ID为0x%0X，信号组为%s，当前测试DTC为CRCDTC 0x%X---------------", TestNo + 1, DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].MsgID, DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].SGInstance[TestSGNo].SGName.c_str(), E2EDTC);
                    TestReport_HTML_StepDescription(requestInfo, Info);
                    ClearAllVector();
                    InitAllFlag();

                    TestStep("唤醒DUT");
                    NMWakeUp(1);

                    TestStep("满足TRC");
                    FulfilTRC();

                    TestStep("按周期仿真CRC正确的E2E报文");
                    SIME2EMsgCycle(DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX);

                    app.wait(2000, "");
                    TestStep("使用14服务清除DTC");
                    SID_14_ClearDTC();

                    TestStep("等待较长时间（超过DTC故障成熟时间）");
                    app.wait(Time_DTCMaturity, "");

                    TestStep("使用19服务读取DTC");
                    SID_19_ReadDTCByMask(DefaultDTCMask);
                    storgeDTC();

                    TestStep("DUT不应记录此E2E DTC");
                    CheckDTCNotRecord(E2EDTC);

                    TestStep("使用错误的CRC更新此E2E报文");
                    sprintf(actualResponse, "made error message is 0x%X", TestMsgID);
                    TestReport_HTML_StepDescription(actualResponse, Info);
                    SIMErrorE2EMsgCycle(TestMsgID);

                    TestStep("等待较长时间（超过DTC故障成熟时间）");
                    app.wait(Time_DTCMaturity, "");

                    TestStep("使用19服务读取DTC");
                    SID_19_ReadDTCByMask(DefaultDTCMask);
                    storgeDTC();

                    TestStep("DUT应记录此E2E DTC");
                    CheckDTCRecord(E2EDTC);

                    TestStep("停止仿真E2E报文，满足控制器休眠条件");
                    NMWakeUp(0);
                    com.del_cyclic_messages();
                    app.wait(5000, "");

                    if (resDTC == 0)
                    {
                        TestStep("未获取到CRC DTC，跳过测试");
                    }
                }

            }


        }


        TestStep("测试结束");
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


s32 TC1023187_Counter_by_Receiver_on_CANCANFD()
{
    try
    {
        int TestNo = 0;//用于测试步骤中的遍历
        int TestSGNo = 0;
        int resDTC = 0;
        u32 E2EDTC = 0;

        strcpy(TestCaseName, "TC1023186_CRC_by_Receiver_on_CANCANFD");
        strcpy(TestScopeName, "E2E");
        strcpy(testPurpose_CH, "E2E");

        //TestStep("测试开始");
        PreE2Econdition();

        for (s32 k = 0; k < 6; k++) {

            TestReport_HTML_StepDescription("", Info);
            sprintf(requestInfo, "当前通道网段名：%s", DUT_def_E2E_SG_RX_2D_Array[k].FName);
            TestReport_HTML_StepDescription(requestInfo, Info);

            for (TestNo = 0; TestNo < DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX.size(); TestNo++)
            {
                TestMsgID = DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].MsgID;
                //TestCH = DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].channel;
                TestCH = k + 1;
                printf("CH=%d,TestCH=%d", k, TestCH);
                for (TestSGNo = 0; TestSGNo < DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].SGInstance.size(); TestSGNo++)
                {
                    TestSGName = DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].SGInstance[TestSGNo].SGName;
                    E2EDTC = DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].SGInstance[TestSGNo].CntrDTC;

                    stepnumber = 0;

                    //如果DTC为0，表示没有此DTC，跳过
                    if (E2EDTC == 0)
                    {
                        continue;
                    }
                    resDTC = 1;

                    sprintf(actualResponse, "TestSGNo=%d,,---------------遍历所有E2E_TXMsg，当前测试报文ID为0x%0X，信号组为%s，当前测试DTC为CounterDTC 0x%x---------------", TestSGNo + 1, DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].MsgID, DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].SGInstance[TestSGNo].SGName.c_str(), E2EDTC);
                    TestReport_HTML_StepDescription(actualResponse, Info);


                    ClearAllVector();
                    InitAllFlag();

                    TestStep("唤醒DUT");
                    NMWakeUp(1);

                    TestStep("满足TRC");
                    FulfilTRC();

                    TestStep("按周期仿真Counter正确的E2E报文");
                    SIME2EMsgCycle(DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX);

                    app.wait(2000, "");
                    TestStep("使用14服务清除DTC");
                    SID_14_ClearDTC();

                    TestStep("等待较长时间（超过DTC故障成熟时间）");
                    app.wait(Time_DTCMaturity, "");

                    TestStep("使用19服务读取DTC");
                    SID_19_ReadDTCByMask(DefaultDTCMask);
                    storgeDTC();

                    TestStep("DUT不应记录此E2E DTC");
                    CheckDTCNotRecord(E2EDTC);

                    TestStep("使用错误的CRC更新此E2E报文,保持Counter不变");
                    SIMConstCounterMsgCycle(TestMsgID);

                    TestStep("等待较长时间（超过DTC故障成熟时间）");
                    app.wait(Time_DTCMaturity, "");

                    TestStep("使用19服务读取DTC");
                    SID_19_ReadDTCByMask(DefaultDTCMask);
                    storgeDTC();

                    TestStep("DUT应记录此E2E DTC");
                    CheckDTCRecord(E2EDTC);

                    TestStep("停止仿真E2E报文，满足控制器休眠条件");
                    NMWakeUp(0);
                    com.del_cyclic_messages();
                    app.wait(5000, "");
                    if (resDTC == 0)
                    {
                        TestStep("未获取到Counter DTC，跳过测试");
                    }
                }
            }

        }



        TestStep("测试结束");
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


s32 TC1023188_E2E_Confirmed_DTC_By_CRC_on_CANCANFD()
{
    try
    {
        int TestNo = 0;//用于测试步骤中的遍历
        int TestSGNo = 0;
        u8 resMask = 0;
        int resDTC = 0;
        int confirmedDTCLimit = 0;
        u32 E2EDTC = 0;

        strcpy(TestCaseName, "TC1023188_E2E_Confirmed_DTC_By_CRC_on_CANCANFD");
        strcpy(TestScopeName, "E2E");
        strcpy(testPurpose_CH, "E2E");

        //TestStep("测试开始");
        PreE2Econdition();

        for (s32 k = 0; k < 6; k++) {

            TestReport_HTML_StepDescription("", Info);
            sprintf(requestInfo, "当前通道网段名：%s", DUT_def_E2E_SG_RX_2D_Array[k].FName);
            TestReport_HTML_StepDescription(requestInfo, Info);
            for (TestNo = 0; TestNo < DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX.size(); TestNo++)
            {
                TestMsgID = DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].MsgID;
                //TestCH = DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].channel;
                TestCH = k + 1;
                printf("CH=%d,TestCH=%d", k, TestCH);
                for (TestSGNo = 0; TestSGNo < DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].SGInstance.size(); TestSGNo++)
                {
                    TestSGName = DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].SGInstance[TestSGNo].SGName;
                    E2EDTC = DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].SGInstance[TestSGNo].ChksDTC;
                    confirmedDTCLimit = DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].SGInstance[TestSGNo].ChksConfirmedDTCLimit;

                    stepnumber = 0;

                    //如果DTC为0，表示没有此DTC，跳过
                    if (E2EDTC == 0)
                    {
                        continue;
                    }
                    resDTC = 1;

                    sprintf(actualResponse, "TestSGNo=%d,,---------------遍历所有E2E_TXMsg，当前测试报文ID为0x%0X，信号组为%s，当前测试DTC为CRCDTC 0x%x---------------", TestSGNo + 1, DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].MsgID, TestSGName.c_str(), E2EDTC);
                    TestReport_HTML_StepDescription(actualResponse, Info);

                    ClearAllVector();
                    InitAllFlag();

                    TestStep("唤醒DUT");
                    NMWakeUp(1);

                    TestStep("满足TRC");
                    FulfilTRC();

                    TestStep("按周期仿真CRC正确的E2E报文");
                    SIME2EMsgCycle(DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX);

                    TestStep("使用14服务清除DTC");
                    SID_14_ClearDTC();

                    TestStep("等待较长时间（超过DTC故障成熟时间）");
                    app.wait(Time_DTCMaturity, "");

                    TestStep("使用19服务掩码0x0F读取DTC");
                    SID_19_ReadDTCByMask(F_DTCMask);
                    storgeDTC();

                    TestStep("DUT不应记录此E2E DTC");
                    CheckDTCNotRecord(E2EDTC);

                    TestStep("使用错误的CRC更新此E2E报文");
                    SIMErrorE2EMsgCycle(TestMsgID);

                    TestStep("等待较长时间（超过DTC故障成熟时间）");
                    app.wait(Time_DTCMaturity, "");

                    TestStep("使用19服务掩码0x0F读取DTC");
                    SID_19_ReadDTCByMask(F_DTCMask);
                    storgeDTC();

                    TestStep("DUT应记录此E2E DTC，且状态位与0x1相关");
                    resMask = CheckDTCRecord(E2EDTC);
                    CompareDTCStatusIncluded(CurrentDTCMask, resMask);

                    TestStep("满足DUT记录Confirm DTC的条件");
                    FulfilConfirm(confirmedDTCLimit);

                    TestStep("使用19服务掩码0x0F读取DTC");
                    SID_19_ReadDTCByMask(F_DTCMask);
                    storgeDTC();

                    TestStep("DUT应记录此E2E DTC，且状态位即与0x8相关，也与0x1相关");
                    resMask = CheckDTCRecord(E2EDTC);
                    CompareDTCStatusIncluded(DefaultDTCMask, resMask);

                    TestStep("重新按周期仿真CRC正确的E2E报文");
                    SIMErrorE2EFlag = 0;

                    TestStep("等待较长时间（超过DTC故障成熟时间）");
                    app.wait(Time_DTCMaturity, "");

                    TestStep("使用19服务读取DTC");
                    SID_19_ReadDTCByMask(DefaultDTCMask);
                    storgeDTC();

                    TestStep("DUT应记录此E2E DTC，且状态位与0x1无关，与0x8相关");
                    resMask = CheckDTCRecord(E2EDTC);
                    CompareDTCStatusIncluded(ConfirmedDTCMask, resMask);
                    CompareDTCStatusNotIncluded(CurrentDTCMask, resMask);

                    TestStep("使用14服务清除DTC");
                    SID_14_ClearDTC();

                    TestStep("等待较长时间（超过DTC故障成熟时间）");
                    app.wait(Time_DTCMaturity, "");

                    TestStep("使用19服务掩码0x0F读取DTC");
                    SID_19_ReadDTCByMask(F_DTCMask);
                    storgeDTC();

                    TestStep("检测DTC，DUT不记录此E2E DTC");
                    CheckDTCNotRecord(E2EDTC);

                    TestStep("停止仿真E2E报文，满足控制器休眠条件");
                    NMWakeUp(0);
                    com.del_cyclic_messages();
                    app.wait(5000, "");

                    if (resDTC == 0)
                    {
                        TestStep("未获取到CRC DTC，跳过测试");
                    }
                }
            }

        }



        TestStep("测试结束");
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


s32 TC1023189_E2E_Confirmed_DTC_By_Counter_on_CANCANFD()
{
    try
    {
        int TestNo = 0;//用于测试步骤中的遍历
        int TestSGNo = 0;
        u8 resMask = 0;
        int resDTC = 0;
        int confirmedDTCLimit = 0;
        u32 E2EDTC = 0;

        strcpy(TestCaseName, "TC1023189_E2E_Confirmed_DTC_By_Counter_on_CANCANFD");
        strcpy(TestScopeName, "E2E");
        strcpy(testPurpose_CH, "E2E");

        //TestStep("测试开始");
        PreE2Econdition();

        for (s32 k = 0; k < 6; k++) {

            TestReport_HTML_StepDescription("", Info);
            sprintf(requestInfo, "当前通道网段名：%s", DUT_def_E2E_SG_RX_2D_Array[k].FName);
            TestReport_HTML_StepDescription(requestInfo, Info);

            for (TestNo = 0; TestNo < DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX.size(); TestNo++)
            {
                TestMsgID = DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].MsgID;
                //TestCH = DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].channel;
                TestCH = k + 1;
                printf("CH=%d,TestCH=%d", k, TestCH);
                for (TestSGNo = 0; TestSGNo < DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].SGInstance.size(); TestSGNo++)
                {
                    TestSGName = DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].SGInstance[TestSGNo].SGName;
                    E2EDTC = DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].SGInstance[TestSGNo].CntrDTC;
                    confirmedDTCLimit = DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].SGInstance[TestSGNo].CntrConfirmedDTCLimit;

                    stepnumber = 0;

                    //如果DTC为0，表示没有此DTC，跳过
                    if (E2EDTC == 0)
                    {
                        continue;
                    }
                    resDTC = 1;

                    sprintf(actualResponse, "TestSGNo=%d,,---------------遍历所有E2E_TXMsg，当前测试报文ID为0x%0X，信号组为%s，当前测试DTC为CounterDTC 0x%x---------------", TestSGNo + 1, DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].MsgID, DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].SGInstance[TestSGNo].SGName.c_str(), E2EDTC);
                    TestReport_HTML_StepDescription(actualResponse, Info);
                    ClearAllVector();
                    InitAllFlag();

                    TestStep("唤醒DUT");
                    NMWakeUp(1);

                    TestStep("满足TRC");
                    FulfilTRC();

                    TestStep("按周期仿真Counter正确的E2E报文");
                    SIME2EMsgCycle(DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX);

                    TestStep("使用14服务清除DTC");
                    SID_14_ClearDTC();

                    TestStep("等待较长时间（超过DTC故障成熟时间）");
                    app.wait(Time_DTCMaturity, "");

                    TestStep("使用19服务掩码0x0F读取DTC");
                    SID_19_ReadDTCByMask(F_DTCMask);
                    storgeDTC();

                    TestStep("DUT不应记录此E2E DTC");
                    CheckDTCNotRecord(E2EDTC);

                    TestStep("使用错误的Counter更新此E2E报文");
                    SIMConstCounterMsgCycle(TestMsgID);

                    TestStep("等待较长时间（超过DTC故障成熟时间）");
                    app.wait(Time_DTCMaturity, "");

                    TestStep("使用19服务掩码0x0F读取DTC");
                    SID_19_ReadDTCByMask(F_DTCMask);
                    storgeDTC();

                    TestStep("DUT应记录此E2E DTC，且状态位与0x1相关");
                    resMask = CheckDTCRecord(E2EDTC);
                    CompareDTCStatusIncluded(CurrentDTCMask, resMask);

                    TestStep("满足DUT记录Confirm DTC的条件");
                    FulfilConfirm(confirmedDTCLimit);

                    TestStep("使用19服务掩码0x0F读取DTC");
                    SID_19_ReadDTCByMask(F_DTCMask);
                    storgeDTC();

                    TestStep("DUT应记录此E2E DTC，且状态位即与0x8相关，也与0x1相关");
                    resMask = CheckDTCRecord(E2EDTC);
                    CompareDTCStatusIncluded(DefaultDTCMask, resMask);

                    TestStep("重新按周期仿真Counter正确的E2E报文");
                    SIMConstCntrFlag = 0;

                    TestStep("等待较长时间（超过DTC故障成熟时间）");
                    app.wait(Time_DTCMaturity, "");

                    TestStep("使用19服务读取DTC");
                    SID_19_ReadDTCByMask(DefaultDTCMask);
                    storgeDTC();

                    TestStep("DUT应记录此E2E DTC，且状态位与0x1无关，与0x8相关");
                    resMask = CheckDTCRecord(E2EDTC);
                    CompareDTCStatusIncluded(ConfirmedDTCMask, resMask);
                    CompareDTCStatusNotIncluded(CurrentDTCMask, resMask);

                    TestStep("使用14服务清除DTC");
                    SID_14_ClearDTC();

                    TestStep("等待较长时间（超过DTC故障成熟时间）");
                    app.wait(Time_DTCMaturity, "");

                    TestStep("使用19服务掩码0x0F读取DTC");
                    SID_19_ReadDTCByMask(F_DTCMask);
                    storgeDTC();

                    TestStep("检测DTC，DUT不记录此E2E DTC");
                    CheckDTCNotRecord(E2EDTC);

                    TestStep("停止仿真E2E报文，满足控制器休眠条件");
                    NMWakeUp(0);
                    com.del_cyclic_messages();
                    app.wait(5000, "");
                }
            }

        }

        if (resDTC == 0)
        {
            TestStep("未获取到Counter DTC，跳过测试");
        }

        TestStep("测试结束");
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


s32 TC1023190_E2E_DTC_TRC_EIPowerLevel_on_CANCANFD()
{
    try
    {
        int TestNo = 0;//用于测试步骤中的遍历
        int TestSGNo = 0;
        int resDTC = 0;
        u32 E2EDTC = 0;

        strcpy(TestCaseName, "TC1023190_E2E_DTC_TRC_EIPowerLevel_on_CANCANFD");
        strcpy(TestScopeName, "E2E");
        strcpy(testPurpose_CH, "E2E");

        //TestStep("测试开始");
        PreE2Econdition();

        for (s32 k = 0; k < 6; k++) {

            TestReport_HTML_StepDescription("", Info);
            sprintf(requestInfo, "当前通道网段名：%s", DUT_def_E2E_SG_RX_2D_Array[k].FName);
            TestReport_HTML_StepDescription(requestInfo, Info);
            for (TestNo = 0; TestNo < DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX.size(); TestNo++)
            {
                TestMsgID = DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].MsgID;
                //TestCH = DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].channel;
                TestCH = k + 1;
                printf("CH=%d,TestCH=%d", k, TestCH);
                for (TestSGNo = 0; TestSGNo < DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].SGInstance.size(); TestSGNo++)
                {
                    TestSGName = DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].SGInstance[TestSGNo].SGName;
                    E2EDTC = DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].SGInstance[TestSGNo].ChksDTC;

                    stepnumber = 0;

                    //如果DTC为0，表示没有此DTC，跳过
                    if (E2EDTC == 0)
                    {
                        continue;
                    }
                    resDTC = 1;

                    sprintf(actualResponse, "TestSGNo=%d,,---------------遍历所有E2E_TXMsg，当前测试报文ID为0x%0X，信号组为%s，当前测试DTC为CRCDTC 0x%x---------------", TestSGNo + 1, DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].MsgID, TestSGName.c_str(), E2EDTC);
                    TestReport_HTML_StepDescription(actualResponse, Info);
                    ClearAllVector();
                    InitAllFlag();

                    TestStep("唤醒DUT");
                    NMWakeUp(1);

                    TestStep("满足TRC");
                    FulfilTRC();

                    TestStep("按周期仿真CRC正确的E2E报文");
                    SIME2EMsgCycle(DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX);

                    TestStep("使用14服务清除DTC");
                    SID_14_ClearDTC();

                    TestStep("等待较长时间（超过DTC故障成熟时间）");
                    app.wait(Time_DTCMaturity, "");

                    TestStep("使用19服务掩码0x0F读取DTC");
                    SID_19_ReadDTCByMask(F_DTCMask);
                    storgeDTC();

                    TestStep("DUT不应记录此E2E DTC");
                    CheckDTCNotRecord(E2EDTC);

                    TestStep("设置EIPowerLevel为1");
                    SetEIPowerLevel(1);

                    TestStep("使用错误的CRC更新此E2E报文");
                    SIMErrorE2EMsgCycle(TestMsgID);

                    TestStep("等待较长时间（超过DTC故障成熟时间）");
                    app.wait(Time_DTCMaturity, "");

                    TestStep("使用19服务掩码0x0F读取DTC");
                    SID_19_ReadDTCByMask(F_DTCMask);
                    storgeDTC();

                    TestStep("DUT不应记录此E2E DTC");
                    CheckDTCNotRecord(E2EDTC);

                    TestStep("设置EIPowerLevel为0");
                    SetEIPowerLevel(0);

                    TestStep("等待较长时间（超过DTC故障成熟时间）");
                    app.wait(Time_DTCMaturity, "");

                    TestStep("使用19服务掩码0x0F读取DTC");
                    SID_19_ReadDTCByMask(F_DTCMask);
                    storgeDTC();

                    TestStep("DUT应记录此E2E DTC");
                    CheckDTCRecord(E2EDTC);

                    TestStep("停止仿真E2E报文，满足控制器休眠条件");
                    NMWakeUp(0);
                    com.del_cyclic_messages();
                    app.wait(5000, "");
                }
            }

        }

        if (resDTC == 0)
        {
            TestStep("未获取到CRC DTC，跳过测试");
        }

        TestStep("测试结束");
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


s32 TC1023191_E2E_DTC_TRC_Voltage_on_CANCANFD()
{
    try
    {
        int TestNo = 0;//用于测试步骤中的遍历
        int TestSGNo = 0;
        int resDTC = 0;
        double Voltage[5] = { 8,10,12,15,17 };
        int v = 0;
        u32 E2EDTC = 0;

        strcpy(TestCaseName, "TC1023191_E2E_DTC_TRC_Voltage_on_CANCANFD");
        strcpy(TestScopeName, "E2E");
        strcpy(testPurpose_CH, "E2E");

        //TestStep("测试开始");
        PreE2Econdition();

        for (s32 k = 0; k < 6; k++) {

            TestReport_HTML_StepDescription("", Info);
            sprintf(requestInfo, "当前通道网段名：%s", DUT_def_E2E_SG_RX_2D_Array[k].FName);
            TestReport_HTML_StepDescription(requestInfo, Info);
            for (TestNo = 0; TestNo < DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX.size(); TestNo++)
            {
                TestMsgID = DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].MsgID;
                //TestCH = DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].channel;
                TestCH = k + 1;
                printf("CH=%d,TestCH=%d", k, TestCH);
                for (TestSGNo = 0; TestSGNo < DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].SGInstance.size(); TestSGNo++)
                {
                    TestSGName = DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].SGInstance[TestSGNo].SGName;
                    E2EDTC = DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].SGInstance[TestSGNo].ChksDTC;

                    stepnumber = 0;

                    //如果DTC为0，表示没有此DTC，跳过
                    if (E2EDTC == 0)
                    {
                        continue;
                    }
                    resDTC = 1;

                    for (v = 0; v < sizeof(Voltage) / sizeof(Voltage[0]); v++)
                    {
                        sprintf(actualResponse, "TestSGNo=%d,,---------------遍历所有E2E_TXMsg，当前测试报文ID为0x%0X，信号组为%s，当前测试DTC为CRCDTC 0x%x---------------", TestSGNo + 1, DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].MsgID, TestSGName.c_str(), E2EDTC);
                        TestReport_HTML_StepDescription(actualResponse, Info);
                        ClearAllVector();
                        InitAllFlag();

                        TestStep("唤醒DUT");
                        NMWakeUp(1);

                        TestStep("满足TRC");
                        FulfilTRC();

                        TestStep("按周期仿真CRC正确的E2E报文");
                        SIME2EMsgCycle(DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX);

                        TestStep("使用14服务清除DTC");
                        SID_14_ClearDTC();

                        TestStep("等待较长时间（超过DTC故障成熟时间）");
                        app.wait(Time_DTCMaturity, "");

                        TestStep("使用19服务掩码0x0F读取DTC");
                        SID_19_ReadDTCByMask(F_DTCMask);
                        storgeDTC();

                        TestStep("DUT不应记录此E2E DTC");
                        CheckDTCNotRecord(E2EDTC);
                        sprintf(actualResponse, "设置电压为%.2fV", Voltage[v]);
                        TestStep(actualResponse);
                        SetVoltage(Voltage[v]);

                        TestStep("使用错误的CRC更新此E2E报文");
                        SIMErrorE2EMsgCycle(TestMsgID);

                        TestStep("等待较长时间（超过DTC故障成熟时间）");
                        app.wait(Time_DTCMaturity, "");

                        TestStep("使用19服务掩码0x0F读取DTC");
                        SID_19_ReadDTCByMask(F_DTCMask);
                        storgeDTC();

                        if (Voltage[v] == 8 || Voltage[v] == 17)
                        {
                            TestStep("DUT不应记录此E2E DTC");
                            CheckDTCNotRecord(E2EDTC);
                        }
                        else if (Voltage[v] == 10 || Voltage[v] == 12 || Voltage[v] == 15)
                        {
                            TestStep("DUT应记录此E2E DTC");
                            CheckDTCRecord(E2EDTC);
                        }

                        TestStep("停止仿真E2E报文，满足控制器休眠条件");
                        NMWakeUp(0);
                        com.del_cyclic_messages();
                        app.wait(5000, "");
                    }
                }
            }

        }

        if (resDTC == 0)
        {
            TestStep("未获取到CRC DTC，跳过测试");
        }

        TestStep("测试结束");
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


s32 TC1023192_E2E_DTC_TRC_Car_Mode_on_CANCANFD()
{
    try
    {
        int TestNo = 0;//用于测试步骤中的遍历
        int TestSGNo = 0;
        int resDTC = 0;
        int i = 0;
        u32 E2EDTC = 0;

        strcpy(TestCaseName, "TC1023192_E2E_DTC_TRC_Car_Mode_on_CANCANFD");
        strcpy(TestScopeName, "E2E");
        strcpy(testPurpose_CH, "E2E");

        //TestStep("测试开始");
        PreE2Econdition();

        for (s32 k = 0; k < 6; k++) {

            TestReport_HTML_StepDescription("", Info);
            sprintf(requestInfo, "当前通道网段名：%s", DUT_def_E2E_SG_RX_2D_Array[k].FName);
            TestReport_HTML_StepDescription(requestInfo, Info);
            for (TestNo = 0; TestNo < DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX.size(); TestNo++)//DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX.size()
            {
                TestMsgID = DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].MsgID;
                //TestCH = DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].channel;
                TestCH = k + 1;
                printf("CH=%d,TestCH=%d", k, TestCH);
                for (TestSGNo = 0; TestSGNo < DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].SGInstance.size(); TestSGNo++)//DUT_def_E2E_SG_RX[TestNo].SGInstance.size()
                {
                    TestSGName = DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].SGInstance[TestSGNo].SGName;
                    E2EDTC = DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].SGInstance[TestSGNo].ChksDTC;

                    stepnumber = 0;

                    //如果DTC为0，表示没有此DTC，跳过
                    if (E2EDTC == 0)
                    {
                        continue;
                    }
                    resDTC = 1;

                    for (i = 0; i < CarMode.size(); i++)
                    {
                        sprintf(actualResponse, "TestSGNo=%d,,---------------遍历所有E2E_TXMsg，当前测试报文ID为0x%0X，信号组为%s，当前测试DTC为CRCDTC 0x%x---------------", TestSGNo + 1, DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].MsgID, TestSGName.c_str(), E2EDTC);
                        TestReport_HTML_StepDescription(actualResponse, Info);
                        ClearAllVector();
                        InitAllFlag();

                        TestStep("唤醒DUT");
                        NMWakeUp(1);

                        TestStep("满足TRC");
                        FulfilTRC();

                        TestStep("按周期仿真CRC正确的E2E报文");
                        SIME2EMsgCycle(DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX);

                        TestStep("使用14服务清除DTC");
                        SID_14_ClearDTC();

                        TestStep("等待较长时间（超过DTC故障成熟时间）");
                        app.wait(Time_DTCMaturity, "");

                        TestStep("使用19服务掩码0x0F读取DTC");
                        SID_19_ReadDTCByMask(F_DTCMask);
                        storgeDTC();

                        TestStep("DUT不应记录此E2E DTC");
                        CheckDTCNotRecord(E2EDTC);

                        sprintf(actualResponse, "设置Car Mode为%s", CarMode[i].c_str());
                        TestStep(actualResponse);
                        //SetCarMode(i);
                        SetCarMode(CarMode[i]);

                        TestStep("使用错误的CRC更新此E2E报文");
                        SIMErrorE2EMsgCycle(TestMsgID);

                        TestStep("等待较长时间（超过DTC故障成熟时间）");
                        app.wait(Time_DTCMaturity, "");

                        TestStep("使用19服务掩码0x0F读取DTC");
                        SID_19_ReadDTCByMask(F_DTCMask);
                        storgeDTC();

                        if (CarMode[i] == "Normal")
                        {
                            TestStep("DUT应记录此E2E DTC");
                            CheckDTCRecord(E2EDTC);
                        }
                        else
                        {
                            TestStep("DUT不应记录此E2E DTC");
                            CheckDTCNotRecord(E2EDTC);
                        }

                        TestStep("停止仿真E2E报文，满足控制器休眠条件");
                        NMWakeUp(0);
                        com.del_cyclic_messages();
                        app.wait(5000, "");
                    }
                }
            }
            if (resDTC == 0)
            {
                TestStep("未获取到CRC DTC，跳过测试");
            }
        }



        TestStep("测试结束");
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


s32 TC1023193_E2E_DTC_TRC_Car_Mode_Transition_on_CANCANFD()
{
    try
    {
        int TestNo = 0;//用于测试步骤中的遍历
        int TestSGNo = 0;
        int resDTC = 0;
        int i = 0;
        double MIN_Wait = 0;
        double MAX_Wait = 0;
        u32 E2EDTC = 0;

        strcpy(TestCaseName, "TC1023193_E2E_DTC_TRC_Car_Mode_Transition_on_CANCANFD");
        strcpy(TestScopeName, "E2E");
        strcpy(testPurpose_CH, "E2E");

        //TestStep("测试开始");
        PreE2Econdition();

        for (s32 k = 0; k < 6; k++) {

            TestReport_HTML_StepDescription("", Info);
            sprintf(requestInfo, "当前通道网段名：%s", DUT_def_E2E_SG_RX_2D_Array[k].FName);
            TestReport_HTML_StepDescription(requestInfo, Info);

            for (TestNo = 0; TestNo < DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX.size(); TestNo++)//DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX.size();
            {
                TestMsgID = DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].MsgID;
                //TestCH = DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].channel;
                TestCH = k + 1;
                printf("CH=%d,TestCH=%d", k, TestCH);
                for (TestSGNo = 0; TestSGNo < DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].SGInstance.size(); TestSGNo++)//DUT_def_E2E_SG_RX[TestNo].SGInstance.size()
                {
                    MIN_Wait = 3000 + DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].SGInstance[TestSGNo].ChksMaxValue / DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].SGInstance[TestSGNo].ChksStepUpValue * DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].SGInstance[TestSGNo].ChksTestPeriod * 0.8;
                    MAX_Wait = 7000 + DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].SGInstance[TestSGNo].ChksMaxValue / DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].SGInstance[TestSGNo].ChksStepUpValue * DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].SGInstance[TestSGNo].ChksTestPeriod * 1.2;
                    E2EDTC = DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].SGInstance[TestSGNo].ChksDTC;
                    TestSGName = DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].SGInstance[TestSGNo].SGName;

                    stepnumber = 0;

                    //如果DTC为0，表示没有此DTC，跳过
                    if (E2EDTC == 0)
                    {
                        continue;
                    }
                    resDTC = 1;

                    sprintf(actualResponse, "TestSGNo=%d,,---------------遍历所有E2E_TXMsg，当前测试报文ID为0x%0X，信号组为%s，当前测试DTC为CRCDTC 0x%x---------------", TestSGNo + 1, DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].MsgID, TestSGName.c_str(), E2EDTC);
                    TestReport_HTML_StepDescription(actualResponse, Info);
                    ClearAllVector();
                    InitAllFlag();

                    TestStep("唤醒DUT");
                    NMWakeUp(1);

                    TestStep("满足TRC");
                    FulfilTRC();

                    TestStep("按周期仿真CRC正确的E2E报文");
                    SIME2EMsgCycle(DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX);

                    TestStep("设置Car Mode为非Normal，设置为Factory");
                    SetCarMode("Factory");

                    TestStep("使用错误的CRC更新此E2E报文");
                    SIMErrorE2EMsgCycle(TestMsgID);

                    TestStep("使用14服务清除DTC");
                    SID_14_ClearDTC();

                    TestStep("等待较长时间（超过DTC故障成熟时间）");
                    app.wait(Time_DTCMaturity, "");

                    TestStep("使用19服务掩码0x0F读取DTC");
                    SID_19_ReadDTCByMask(F_DTCMask);
                    storgeDTC();

                    TestStep("DUT不应记录此E2E DTC");
                    CheckDTCNotRecord(E2EDTC);

                    TestStep("设置Car Mode为Normal");
                    SetCarMode("Normal");

                    sprintf(actualResponse, "Wait 3s+ MaxValue / StepUpValue * TestPeriod * 0.8 = %.2f", MIN_Wait);
                    TestStep(actualResponse);
                    app.wait(MIN_Wait, "");

                    TestStep("重新使用正确的值更新E2ERX报文");
                    SIMErrorE2EFlag = 0;

                    TestStep("使用19服务掩码0x0F读取DTC");
                    SID_19_ReadDTCByMask(F_DTCMask);
                    storgeDTC();

                    TestStep("DUT不应记录此E2E DTC");
                    CheckDTCNotRecord(E2EDTC);

                    TestStep("--------------------------重复，修改等待时间--------------------------");

                    TestStep("设置Car Mode为非Normal，设置为Factory");
                    SetCarMode("Factory");

                    TestStep("使用错误的CRC更新此E2E报文");
                    SIMErrorE2EMsgCycle(TestMsgID);

                    TestStep("使用14服务清除DTC");
                    SID_14_ClearDTC();

                    TestStep("等待较长时间（超过DTC故障成熟时间）");
                    app.wait(Time_DTCMaturity, "");

                    TestStep("使用19服务掩码0x0F读取DTC");
                    SID_19_ReadDTCByMask(F_DTCMask);
                    storgeDTC();

                    TestStep("DUT不应记录此E2E DTC");
                    CheckDTCNotRecord(E2EDTC);

                    TestStep("设置Car Mode为Normal");
                    SetCarMode("Normal");


                    sprintf(actualResponse, "Wait 7s+ MaxValue / StepUpValue * TestPeriod * 1.2 = %.2f", MAX_Wait);
                    TestStep(actualResponse);
                    app.wait(MAX_Wait, "");

                    TestStep("重新使用正确的值更新E2ERX报文");
                    SIMErrorE2EFlag = 0;

                    TestStep("使用19服务掩码0x0F读取DTC");
                    SID_19_ReadDTCByMask(F_DTCMask);
                    storgeDTC();

                    TestStep("DUT应记录此E2E DTC");
                    CheckDTCRecord(E2EDTC);

                    TestStep("停止仿真E2E报文，满足控制器休眠条件");
                    NMWakeUp(0);
                    com.del_cyclic_messages();
                    app.wait(5000, "");
                }
            }
            if (resDTC == 0)
            {
                TestStep("未获取到CRC DTC，跳过测试");
            }
        }



        TestStep("测试结束");
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


s32 TC1023194_E2E_DTC_TRC_Usage_Mode_on_CANCANFD()
{
    try
    {
        int TestNo = 0;//用于测试步骤中的遍历
        int TestSGNo = 0;
        int resDTC = 0;
        int i = 0;
        u32 E2EDTC = 0;

        strcpy(TestCaseName, "TC1023186_CRC_by_Receiver_on_CANCANFD");
        strcpy(TestScopeName, "E2E");
        strcpy(testPurpose_CH, "E2E");

        //TestStep("测试开始");
        PreE2Econdition();

        for (s32 k = 0; k < 6; k++) {

            TestReport_HTML_StepDescription("", Info);
            sprintf(requestInfo, "当前通道网段名：%s", DUT_def_E2E_SG_RX_2D_Array[k].FName);
            TestReport_HTML_StepDescription(requestInfo, Info);

            for (TestNo = 0; TestNo < DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX.size(); TestNo++)
            {
                TestMsgID = DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].MsgID;
                //TestCH = DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].channel;
                TestCH = k + 1;
                printf("CH=%d,TestCH=%d", k, TestCH);
                for (TestSGNo = 0; TestSGNo < DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].SGInstance.size(); TestSGNo++)
                {
                    TestSGName = DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].SGInstance[TestSGNo].SGName;
                    E2EDTC = DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].SGInstance[TestSGNo].ChksDTC;

                    stepnumber = 0;

                    //如果DTC为0，表示没有此DTC，跳过
                    if (E2EDTC == 0)
                    {
                        continue;
                    }
                    resDTC = 1;

                    for (i = 0; i < UsgMode.size(); i++)
                    {
                        sprintf(actualResponse, "TestSGNo=%d,,---------------遍历所有E2E_TXMsg，当前测试报文ID为0x%0X，信号组为%s，当前测试DTC为CRCDTC 0x%x---------------", TestSGNo + 1, DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].MsgID, TestSGName.c_str(), E2EDTC);
                        TestReport_HTML_StepDescription(actualResponse, Info);
                        ClearAllVector();
                        InitAllFlag();

                        TestStep("唤醒DUT");
                        NMWakeUp(1);
                       

                        TestStep("满足TRC");
                        FulfilTRC();

                        TestStep("按周期仿真CRC正确的E2E报文");
                        SIME2EMsgCycle(DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX);

                        TestStep("使用14服务清除DTC");
                        SID_14_ClearDTC();

                        TestStep("等待较长时间（超过DTC故障成熟时间）");
                        app.wait(Time_DTCMaturity, "");

                        TestStep("使用19服务掩码0x0F读取DTC");
                        SID_19_ReadDTCByMask(F_DTCMask);
                        storgeDTC();

                        TestStep("DUT不应记录此E2E DTC");
                        CheckDTCNotRecord(E2EDTC);


                        sprintf(actualResponse, "设置Usage Mode为%s", UsgMode[i].c_str());
                        TestStep(actualResponse);
                        SetUsageMode(UsgMode[i]);

                        TestStep("使用错误的CRC更新此E2E报文");
                        SIMErrorE2EMsgCycle(TestMsgID);

                        TestStep("等待较长时间（超过DTC故障成熟时间）");
                        app.wait(Time_DTCMaturity, "");

                        TestStep("使用19服务掩码0x0F读取DTC");
                        SID_19_ReadDTCByMask(F_DTCMask);
                        storgeDTC();

                        if (UsgMode[i] == "Active" || UsgMode[i] == "Driving")
                        {
                            TestStep("DUT应记录此E2E DTC");
                            CheckDTCRecord(E2EDTC);
                        }
                        else
                        {
                            TestStep("DUT不应记录此E2E DTC");
                            CheckDTCNotRecord(E2EDTC);
                        }

                        TestStep("停止仿真E2E报文，满足控制器休眠条件");
                        NMWakeUp(0);
                        com.del_cyclic_messages();
                        app.wait(5000, "");
                    }
                }
            }
            if (resDTC == 0)
            {
                TestStep("未获取到CRC DTC，跳过测试");
            }

        }



        TestStep("测试结束");
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


s32 TC1023195_E2E_DTC_TRC_Usage_Mode_Transition_on_CANCANFD()
{
    try
    {
        int TestNo = 0;//用于测试步骤中的遍历
        int TestSGNo = 0;
        int resDTC = 0;
        int i = 0;
        double MIN_Wait = 0;
        double MAX_Wait = 0;
        u32 E2EDTC = 0;

        strcpy(TestCaseName, "TC1023195_E2E_DTC_TRC_Usage_Mode_Transition_on_CANCANFD");
        strcpy(TestScopeName, "E2E");
        strcpy(testPurpose_CH, "E2E");

        //TestStep("测试开始");
        PreE2Econdition();

        for (s32 k = 0; k < 6; k++) {

            TestReport_HTML_StepDescription("", Info);
            sprintf(requestInfo, "当前通道网段名：%s", DUT_def_E2E_SG_RX_2D_Array[k].FName);
            TestReport_HTML_StepDescription(requestInfo, Info);
            for (TestNo = 0; TestNo < DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX.size(); TestNo++)
            {
                TestMsgID = DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].MsgID;
                //TestCH = DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].channel;
                TestCH = k + 1;
                printf("CH=%d,TestCH=%d", k, TestCH);
                for (TestSGNo = 0; TestSGNo < DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].SGInstance.size(); TestSGNo++)
                {
                    MIN_Wait = 3000 + DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].SGInstance[TestSGNo].ChksMaxValue / DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].SGInstance[TestSGNo].ChksStepUpValue * DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].SGInstance[TestSGNo].ChksTestPeriod * 0.8;
                    MAX_Wait = 7000 + DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].SGInstance[TestSGNo].ChksMaxValue / DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].SGInstance[TestSGNo].ChksStepUpValue * DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].SGInstance[TestSGNo].ChksTestPeriod * 1.2;
                    E2EDTC = DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].SGInstance[TestSGNo].ChksDTC;
                    TestSGName = DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].SGInstance[TestSGNo].SGName;

                    stepnumber = 0;

                    //如果DTC为0，表示没有此DTC，跳过
                    if (E2EDTC == 0)
                    {
                        continue;
                    }
                    resDTC = 1;

                    sprintf(actualResponse, "TestSGNo=%d,,---------------遍历所有E2E_TXMsg，当前测试报文ID为0x%0X，信号组为%s，当前测试DTC为CRCDTC 0x%x---------------", TestSGNo + 1, DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX[TestNo].MsgID, TestSGName.c_str(), E2EDTC);
                    TestReport_HTML_StepDescription(actualResponse, Info);
                    ClearAllVector();
                    InitAllFlag();

                    TestStep("唤醒DUT");
                    NMWakeUp(1);

                    TestStep("满足TRC");
                    FulfilTRC();

                    TestStep("按周期仿真CRC正确的E2E报文");
                    SIME2EMsgCycle(DUT_def_E2E_SG_RX_2D_Array[k].DUT_def_E2E_SG_RX);

                    TestStep("设置Usage Mode设置为Inactive");
                    SetUsageMode("Inactive");

                    TestStep("使用错误的CRC更新此E2E报文");
                    SIMErrorE2EMsgCycle(TestMsgID);

                    TestStep("使用14服务清除DTC");
                    SID_14_ClearDTC();

                    TestStep("等待较长时间（超过DTC故障成熟时间）");
                    app.wait(Time_DTCMaturity, "");

                    TestStep("使用19服务掩码0x0F读取DTC");
                    SID_19_ReadDTCByMask(F_DTCMask);
                    storgeDTC();

                    TestStep("DUT不应记录此E2E DTC");
                    CheckDTCNotRecord(E2EDTC);

                    TestStep("设置Usage Mode为Active");
                    SetUsageMode("Active");

                    sprintf(actualResponse, "Wait 3s+ MaxValue / StepUpValue * TestPeriod * 0.8 = %.2f", MIN_Wait);
                    TestStep(actualResponse);
                    app.wait(MIN_Wait, "");

                    TestStep("重新使用正确的值更新E2ERX报文");
                    SIMErrorE2EFlag = 0;

                    TestStep("使用19服务掩码0x0F读取DTC");
                    SID_19_ReadDTCByMask(F_DTCMask);
                    storgeDTC();

                    TestStep("DUT不应记录此E2E DTC");
                    CheckDTCNotRecord(E2EDTC);

                    TestStep("--------------------------重复，修改等待时间--------------------------");

                    TestStep("设置Usage Mode设置为Inactive");
                    SetUsageMode("Inactive");

                    TestStep("使用错误的CRC更新此E2E报文");
                    SIMErrorE2EMsgCycle(TestMsgID);

                    TestStep("使用14服务清除DTC");
                    SID_14_ClearDTC();

                    TestStep("等待较长时间（超过DTC故障成熟时间）");
                    app.wait(Time_DTCMaturity, "");

                    TestStep("使用19服务掩码0x0F读取DTC");
                    SID_19_ReadDTCByMask(F_DTCMask);
                    storgeDTC();

                    TestStep("DUT不应记录此E2E DTC");
                    CheckDTCNotRecord(E2EDTC);

                    TestStep("设置Usage Mode为Active");
                    SetUsageMode("Active");

                    sprintf(actualResponse, "Wait 7s+ MaxValue / StepUpValue * TestPeriod * 1.2 = %.2f", MAX_Wait);
                    TestStep(actualResponse);
                    app.wait(MAX_Wait, "");

                    TestStep("重新使用正确的值更新E2ERX报文");
                    SIMErrorE2EFlag = 0;

                    TestStep("使用19服务掩码0x0F读取DTC");
                    SID_19_ReadDTCByMask(F_DTCMask);
                    storgeDTC();

                    TestStep("DUT应记录此E2E DTC");
                    CheckDTCRecord(E2EDTC);

                    TestStep("停止仿真E2E报文，满足控制器休眠条件");
                    NMWakeUp(0);
                    com.del_cyclic_messages();
                    app.wait(5000, "");
                }
            }
            if (resDTC == 0)
            {
                TestStep("未获取到CRC DTC，跳过测试");
            }
        }



        TestStep("测试结束");
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
#endif // !TestCase_CAN_E2E