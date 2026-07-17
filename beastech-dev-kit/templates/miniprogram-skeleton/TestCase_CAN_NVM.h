
#ifndef TestCase_CAN_NVM
#define TestCase_CAN_NVM

#include "test_case_diag_api.h"


// 低电压情况下，将NVM数据写入
s32 TC_LV_Write_NVM_Data_NVM_001()
{
    try {
        u8 DID[2] = {};
        DID[0] = 0x40;
        DID[1] = 0x58;
        s32 DIDsize = 3;
        u8 Data = 0x60;
        u8 DID2E[3] = { 0x40,0x58, Data };
        s32 DID2Esize = 3;
        double Vol = 0;
        s32 flag = 0;
        strcpy(actualResponse, "");

        //低电压情况下，将NVM数据写入
        strcpy(TestCaseName, "TC_NVM_001");
        strcpy(TestScopeName, "NVM");
        strcpy(testPurpose_CH, "测试目的：检查低压NVM写入情况");

        TestPrecondition(); 
        TestInfoPrint("测试次数：%d", NVMCount);
        FulfilTRC();
        SID10_SessionControl(0x03);
        CheckPosResponse_SID10(0x3);
        if (gCEMFlg == 1) {
            SecurityUnlock(0x03);
            CarModSwtich(0);
        }
        else {
            CarModSwtich(0);
        }
        app.wait(100, "");
        SecurityUnlock(0x05);
        for (s32 i = 0; i < NVMCount; i++) {
            stepnumber = 0;
            TestInfoPrint("当前测试第%d次", i+1);
            //1.
            
            TestStep("调整输入电压阈值<8V，等待10s");
            TestReport_HTML_StepDescription("Step: 输入电压7V", Info);
            Vol = SetVolatgeAndRead(7, 1);

            if (Vol < 8) {
                sprintf(actualResponse, "Check: 低电压状态，当前电压为%.3fV", Vol);
                TestReport_HTML_StepDescription(actualResponse, Pass);
            }
            else {
                sprintf(actualResponse, "Check: 非低电压状态，当前电压为%.3fV", Vol);
                TestReport_HTML_StepDescription(actualResponse, Fail);
            }

            TestWait(10000);
            //GetVolatge(500);
            //2.
            TestStep("发送22服务读诊断数据：22 40 58");
            SID22_ReadDataByIdentifier(DID);
            //CheckPosResponse_SID22(DID, ReadDIDList[i].DIDLength);
            if (flag == 0) {
                CheckNVM_SID22(DID, 0);
                flag = 1;
            }
            else {
                CheckNVM_SID22(DID, 1, Data);
            }

            CheckCurrentSession(0x03);
            //3.
            sprintf(actualResponse, ("Step: 发送2E服务写诊断数据：2E 40 58 " + dec2hex(Data)).c_str());
            TestStep(actualResponse);
            SID2E_WriteDataById(DID2E, DID2Esize);
            CheckPosResponse_SID2E(DID);

            //4.
            TestStep("Step: 调整输入电压阈值=12V，等待10s");
            TestReport_HTML_StepDescription("Step: 输入电压12V", Info);
            Vol = SetVolatgeAndRead(12, 1);

            if ((Vol >= 8) && (Vol <= 16)) {
                sprintf(actualResponse, "Check: 正常电压状态，当前电压为%.3fV", Vol);
                TestReport_HTML_StepDescription(actualResponse, Pass);
            }
            else {
                sprintf(actualResponse, "Check: 非正常电压状态，当前电压为%.3fV", Vol);
                TestReport_HTML_StepDescription(actualResponse, Fail);
            }
            TestWait(10000);
            GetVolatge(500);

            //5.
            TestStep("Step: 发送22服务读诊断数据：22 40 58");
            SID22_ReadDataByIdentifier(DID);
            //CheckPosResponse_SID22(DID, ReadDIDList[i].DIDLength);
            CheckNVM_SID22(DID, 1, Data);

            //6.
            CheckCurrentSession(0x03);
            Data = 0x30;
            DID2E[2] = Data;
            sprintf(actualResponse, ("Step: 发送2E服务写诊断数据：2E 40 58 " + dec2hex(Data)).c_str());
            TestStep(actualResponse);
            SID2E_WriteDataById(DID2E, DID2Esize);
            CheckPosResponse_SID2E(DID);
            //
            app.wait(100, "");
    
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

// 高电压情况下，将NVM数据写入
s32 TC_HV_Write_NVM_Data_NVM_002() {
    try {
        u8 DID[2] = {};
        DID[0] = 0x40;
        DID[1] = 0x58;
        s32 DIDsize = 3;
        u8 Data = 0x20;
        u8 DID2E[3] = { 0x40,0x58, Data };
        s32 DID2Esize = 3;
        double Vol = 0;
        s32 flag = 0;
        strcpy(actualResponse, "");

        //低电压情况下，将NVM数据写入
        strcpy(TestCaseName, "TC_NVM_002");
        strcpy(TestScopeName, "NVM");
        strcpy(testPurpose_CH, "测试目的：检查高压NVM写入情况");

        TestPrecondition();
        TestInfoPrint("测试次数：%d", NVMCount);
        FulfilTRC();
        SID10_SessionControl(0x03);
        CheckPosResponse_SID10(0x3);
        if (gCEMFlg == 1) {
            SecurityUnlock(0x03);
            CarModSwtich(0);
        }
        app.wait(100, "");
        SecurityUnlock(0x05);
        for (s32 i = 0; i < NVMCount; i++) {

            TestInfoPrint("当前测试第%d次", i+1);
            TestReport_HTML_StepDescription("", Info);
            //1.
            stepnumber = 0;
            TestStep("调整输入电压阈值>16V，等待10s");
            TestReport_HTML_StepDescription("Step: 输入电压17V", Info);
            Vol = SetVolatgeAndRead(17, 1);

            if (Vol > 16) {
                sprintf(actualResponse, "Check: 高电压状态，当前电压为%.3fV", Vol);
                TestReport_HTML_StepDescription(actualResponse, Pass);
            }
            else {
                sprintf(actualResponse, "Check: 非高电压状态，当前电压为%.3fV", Vol);
                TestReport_HTML_StepDescription(actualResponse, Fail);
            }

            TestWait(10000);
            GetVolatge(500);
            //2.
            TestStep("发送22服务读诊断数据：22 40 58");
            SID22_ReadDataByIdentifier(DID);
            //CheckPosResponse_SID22(DID, ReadDIDList[i].DIDLength);
            if (flag == 0) {
                CheckNVM_SID22(DID, 0);
                flag = 1;
            }
            else {
                CheckNVM_SID22(DID, 1, Data);
            }

            CheckCurrentSession(0x03);
            //3.
            sprintf(actualResponse, ("Step: 发送2E服务写诊断数据：2E 40 58 " + dec2hex(Data)).c_str());
            TestStep(actualResponse);
            SID2E_WriteDataById(DID2E, DID2Esize);
            CheckPosResponse_SID2E(DID);

            //4.
            TestStep("Step: 调整输入电压阈值=12V，等待10s");
            TestReport_HTML_StepDescription("Step: 输入电压12V", Info);
            Vol = SetVolatgeAndRead(12, 1);

            if ((Vol >= 8) && (Vol <= 16)) {
                sprintf(actualResponse, "Check: 正常电压状态，当前电压为%.3fV", Vol);
                TestReport_HTML_StepDescription(actualResponse, Pass);
            }
            else {
                sprintf(actualResponse, "Check: 非正常电压状态，当前电压为%.3fV", Vol);
                TestReport_HTML_StepDescription(actualResponse, Fail);
            }
            TestWait(10000);
            GetVolatge(500);

            //5.
            TestStep("Step: 发送22服务读诊断数据：22 40 58");
            SID22_ReadDataByIdentifier(DID);
            //CheckPosResponse_SID22(DID, ReadDIDList[i].DIDLength);
            CheckNVM_SID22(DID, 1, Data);

            CheckCurrentSession(0x03);
            //6.
            Data = 0x30;
            DID2E[2] = Data;
            sprintf(actualResponse, ("Step: 发送2E服务写诊断数据：2E 40 58 " + dec2hex(Data)).c_str());
            TestStep(actualResponse);
            SID2E_WriteDataById(DID2E, DID2Esize);
            CheckPosResponse_SID2E(DID);
            //
            TestWait(1000);
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

// NVM-003 正常写入情况下，制造睡眠唤醒，检查NVM数据是否丢失
s32 TC_Check_NVM_Data_Loss_Sleep_Wake_NVM_003() {
    try {
        u8 DID[2] = {};
        DID[0] = 0x40;
        DID[1] = 0x58;

        u8 Data = 0x10;
        u8 DID2E[3] = { 0x40,0x58, Data };
        s32 DID2Esize = 3;
        double Vol = 0;
        s32 flag = 0;
        strcpy(actualResponse, "");

        //低电压情况下，将NVM数据写入
        strcpy(TestCaseName, "TC_NVM_003");
        strcpy(TestScopeName, "NVM");
        strcpy(testPurpose_CH, "测试目的：检查低压NVM写入情况");

        TestPrecondition();
        TestInfoPrint("测试次数：%d", NVMCount);
        FulfilTRC();
        SID10_SessionControl(0x03);
        CheckPosResponse_SID10(0x3);
        if (gCEMFlg == 1) {
            SecurityUnlock(0x03);
        }
        CarModSwtich(0);
        app.wait(100, "");
        SecurityUnlock(0x05);

        for (s32 i = 0; i < NVMCount; i++) {
            stepnumber = 0;
            TestInfoPrint("当前测试第%d次", i + 1);
            //1.
            sprintf(actualResponse, ("Step: 发送2E服务写诊断数据：2E 40 58 " + dec2hex(Data)).c_str());
            TestStep(actualResponse);
            //TestReport_HTML_StepDescription(("发送2E 40 58 " + dec2hex(Data)).c_str(), Info);
            SID2E_WriteDataById(DID2E, DID2Esize);
            //TestReport_HTML_StepDescription("检查2E响应" , Info);
            CheckPosResponse_SID2E(DID);
            //2.
            TestStep("非driver状态下(如Abandon)，停止发送网络唤醒报文");
            TestReport_HTML_StepDescription("设置InActive模式", Info);
            if (gCEMFlg == 1) {
                SecurityUnlock(0x03);
            }
            CarModSwtich(0);
            UsgModSwtich(0x01);
            TestReport_HTML_StepDescription("停止发送网络唤醒报文", Info);
            NMWakeUp(0);
            SendTesterPresent(TP_OFF);
            SimulateTRC(false);

            //3.
            TestStep("等待10s后，确认板子进入休眠状态");
            TestWait(10000);
            TestWaitForCurInRange(10, 150000);

            //4.
            TestStep(("记录最后一次的2E服务写成功的数值" + dec2hex(Data)).c_str());

            //5.
            TestStep("发送唤醒报文:3F 40 FF FF FF FF FF FF");
            NMWakeUp(1);
            SendTesterPresent(TP_ON);
            SimulateTRC(true);

            //6.
            TestStep("等待10s后，确认板子进入唤醒正常工作状态");
            TestWait(10000);
            if (APP_Flag == 1) {
                TestReport_HTML_StepDescription("板子进入网络唤醒状态", Pass);
            }
            else {
                TestReport_HTML_StepDescription("板子没有进入网络唤醒状态", Fail);
            }

            SID10_SessionControl(0x03);
            CheckPosResponse_SID10(0x3);
            SecurityUnlock(0x05);

            //7.
            TestStep("Step: 发送22服务读诊断数据：22 40 58");
            SID22_ReadDataByIdentifier(DID);
            CheckNVM_SID22(DID, 1, Data);

            //8.
            Data = 0x20;
            DID2E[2] = Data;
            sprintf(actualResponse, ("Step: 发送2E服务写诊断数据：2E 40 58 " + dec2hex(Data)).c_str());
            TestStep(actualResponse);
            //TestReport_HTML_StepDescription(("发送2E 40 58 " + dec2hex(Data)).c_str(), Info);
            SID2E_WriteDataById(DID2E, DID2Esize);
            //TestReport_HTML_StepDescription("检查2E响应" , Info);
            CheckPosResponse_SID2E(DID);

            //9.
            TestStep("非driver状态下(如Abandon)，停止发送网络唤醒报文");
            TestReport_HTML_StepDescription("设置InActive模式", Info);
            if (gCEMFlg == 1) {
                SecurityUnlock(0x03);
            }
            UsgModSwtich(0x01);
            TestReport_HTML_StepDescription("停止发送网络唤醒报文", Info);
            NMWakeUp(0);
            SendTesterPresent(TP_OFF);
            SimulateTRC(false);

            //10.
            TestStep("确认ECU进入深度休眠状态");
            //TestWait(15 * 60000);
            TestWaitForCurInRange(10, 150000);

            //11.
            TestStep(("记录最后一次的2E服务写成功的数值" + dec2hex(Data)).c_str());

            //12
            TestStep("发送唤醒报文:3F 40 FF FF FF FF FF FF");
            NMWakeUp(1);
            SendTesterPresent(TP_ON);
            SimulateTRC(true);

            //13.
            TestStep("等待10s后，确认板子进入唤醒正常工作状态");
            TestWait(10000);
            if (APP_Flag == 1) {
                TestReport_HTML_StepDescription("板子进入网络唤醒状态", Pass);
            }
            else {
                TestReport_HTML_StepDescription("板子没有进入网络唤醒状态", Fail);
            }
            SID10_SessionControl(0x03);
            CheckPosResponse_SID10(0x3);
            SecurityUnlock(0x05);
            //14.
            TestStep("Step: 发送22服务读诊断数据：22 40 58");
            SID22_ReadDataByIdentifier(DID);
            CheckNVM_SID22(DID, 1, Data);
            TestWait(100);
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

// NVM-004 正常写入情况下，power reset后，检查NVM数据是否写入成功
s32 TC_Check_NVM_Write_Power_Reset_NVM_004() {
    try {
        u8 DID[2] = {};
        DID[0] = 0x40;
        DID[1] = 0x58;

        u8 Data = 0x30;
        u8 DID2E[3] = { 0x40,0x58, Data };
        s32 DID2Esize = 3;
        double Vol = 0;
        s32 flag = 0;
        strcpy(actualResponse, "");

        //低电压情况下，将NVM数据写入
        strcpy(TestCaseName, "TC_NVM_004");
        strcpy(TestScopeName, "NVM");
        strcpy(testPurpose_CH, "测试目的：");

        TestPrecondition();
        SimulateTRC(true);
        TestInfoPrint("测试次数：%d", NVMCount);
        SID10_SessionControl(0x03);
        CheckPosResponse_SID10(0x3);
        if (gCEMFlg == 1) {
            SecurityUnlock(0x03);
            CarModSwtich(0);
        }
        app.wait(100, "");
        for (s32 i = 0; i < NVMCount; i++) {
            stepnumber = 0;
            TestInfoPrint("当前测试第%d次", i + 1);
            DID2E[2] = Data;
            SID10_SessionControl(0x03);
            CheckPosResponse_SID10(0x3);
            SecurityUnlock(0x05);
            //1.
            sprintf(actualResponse, ("Step: 发送2E服务写诊断数据：2E 40 58 " + dec2hex(Data)).c_str());
            TestStep(actualResponse);
            //TestReport_HTML_StepDescription(("发送2E 40 58 " + dec2hex(Data)).c_str(), Info);
            SID2E_WriteDataById(DID2E, DID2Esize);
            //TestReport_HTML_StepDescription("检查2E响应" , Info);
            CheckPosResponse_SID2E(DID);

            //2.
            TestStep("断掉电源并重新接通板子输入电源12v");
            KL15(off);
            KL30(off);
            TestWait(100);
            KL30(on);
            KL15(on);
            APP_Flag = 0;
            NM_Flag = 0;
            TestReport_HTML_StepDescription("输入电压12V", Info);
            SetVoltage(12);
            //3.
            TestStep("等待10s后，确认板子上电工作正常");
            TestWait(10000);
            //TestWaitForCurInRange(10, 500000);
            if (APP_Flag == 1) {
                TestReport_HTML_StepDescription("板子Power reset成功", Pass);
            }
            else {
                TestReport_HTML_StepDescription("板子Power reset失败", Fail);
            }

            //4.
            TestStep(("记录最后一次的2E服务写成功的数值" + dec2hex(Data)).c_str());

            //5.

            TestStep("Step: 发送22服务读诊断数据：22 40 58");
            SID22_ReadDataByIdentifier(DID);
            CheckNVM_SID22(DID, 1, Data);
            Data++;
            TestWait(1000);
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

// NVM-005 写入NVM过程中，power reset后，检查是否NVM能够重新访问
s32 TC_Check_NVM_Access_Power_Reset_NVM_005() {
    try {
        u8 DID[2] = {};
        if (1 == gCEMFlg) {
            DID[0] = 0x41;
            DID[1] = 0xC3;
        }
        else {
            DID[0] = 0xD1;
            DID[1] = 0x31;
        }

        u8 DID2E[34] = { 0 };
        DID2E[0] = DID[0];
        DID2E[1] = DID[1];
        memset(DID2E + 2,0,32);
        DID2E[30] = 0x01;
        DID2E[31] = 0x01;
        DID2E[32] = 0x01;
        DID2E[33] = 0x01;
        u8 DID2E_2[34] = { 0 };
        DID2E_2[0] = DID[0];
        DID2E_2[1] = DID[1];
        memset(DID2E_2 + 2, 0, 32);
        DID2E_2[30] = 0x02;
        DID2E_2[31] = 0x02;
        DID2E_2[32] = 0x02;
        DID2E_2[33] = 0x02;
        s32 DID2Esize = 34;

        u8 DID2E_ZCUD[50] = { 0 };
        DID2E_ZCUD[0] = DID[0];
        DID2E_ZCUD[1] = DID[1];
        memset(DID2E_ZCUD + 2, 0, 48);
        DID2E_ZCUD[46] = 0x01;
        DID2E_ZCUD[47] = 0x01;
        DID2E_ZCUD[48] = 0x01;
        DID2E_ZCUD[49] = 0x01;
        u8 DID2E_ZCUD_2[50] = { 0 };
        DID2E_ZCUD_2[0] = DID[0];
        DID2E_ZCUD_2[1] = DID[1];
        memset(DID2E_ZCUD_2 + 2, 0, 48);
        DID2E_ZCUD_2[46] = 0x02;
        DID2E_ZCUD_2[47] = 0x02;
        DID2E_ZCUD_2[48] = 0x02;
        DID2E_ZCUD_2[49] = 0x02;
        s32 DID2E_ZCUDsize = 50;
        double Vol = 0;
        s32 flag = 0;
        strcpy(actualResponse, "");

        //低电压情况下，将NVM数据写入
        strcpy(TestCaseName, "TC_NVM_005");
        strcpy(TestScopeName, "NVM");
        strcpy(testPurpose_CH, "测试目的：");

        TestPrecondition();
        SimulateTRC(true);
        TestInfoPrint("测试次数：%d", NVMCount);
        SID10_SessionControl(0x03);
        CheckPosResponse_SID10(0x3);
        if (gCEMFlg == 1) {
            SecurityUnlock(0x03);
            CarModSwtich(0);
        }
        TestWait(100);

        for (s32 i = 0; i < NVMCount; i++) {
            stepnumber = 0;
            TestReport_HTML_StepDescription("", Info);

            SID10_SessionControl(0x03);
            CheckPosResponse_SID10(0x3);
            SecurityUnlock(0x11);
            u8 lastDataRecord[48] = { 0 };
            SID22_ReadDataByIdentifier(DID, 2);
            if (0 == CheckPosResponse_SID22(DID)) {
                if (1 == gCEMFlg) {
                    StoreArray(lastDataRecord, TpRxData, 48, 3, 0);
                }
                else {
                    StoreArray(lastDataRecord, TpRxData, 32, 3, 0);
                }
            }

            //1.
            if (1 == gCEMFlg) {
                sprintf(actualResponse, ("Step: 发送2E服务写诊断数据：2E 41 C3 "));
                TestStep(actualResponse);
                //TestReport_HTML_StepDescription(("发送2E 40 58 " + dec2hex(Data)).c_str(), Info);
                SID2E_WriteDataById(DID2E_ZCUD, DID2E_ZCUDsize);
            }
            else {
                sprintf(actualResponse, ("Step: 发送2E服务写诊断数据：2E D1 31 "));
                TestStep(actualResponse);
                //TestReport_HTML_StepDescription(("发送2E 40 58 " + dec2hex(Data)).c_str(), Info);
                SID2E_WriteDataById(DID2E, DID2Esize);
            }
            
            CheckPosResponse_SID2E(DID);
            if (1 == gCEMFlg) {
                sprintf(actualResponse, ("Step: 发送2E服务写诊断数据：2E 41 C3 "));
                TestStep(actualResponse);
                SID2E_WriteDataById_Request(DID2E_ZCUD_2, DID2E_ZCUDsize);
            }
            else {
                sprintf(actualResponse, ("Step: 发送2E服务写诊断数据：2E D1 31 "));
                TestStep(actualResponse);
                SID2E_WriteDataById_Request(DID2E_2, DID2Esize);
            }
            

            //2.
            KL15(off);
            KL30(off);
            TestStep("断掉电源并重新接通板子输入电源12v");

            TestWait(1000);
            KL30(on);
            KL15(on);
            APP_Flag = 0;
            NM_Flag = 0;
            TestReport_HTML_StepDescription("输入电压12V", Info);
            SetVoltage(12);
            //3.
            TestStep("等待10s后，确认板子上电工作正常");
            TestWait(10000);
            //TestWaitForCurInRange(10, 500000);
            if (APP_Flag == 1) {
                TestReport_HTML_StepDescription("板子Power reset成功", Pass);
            }
            else {
                TestReport_HTML_StepDescription("板子Power reset失败", Fail);
            }

            //4.
            TestStep(("读取最后一次的2E服务写成功的数值"));

            //5.
            if (1 == gCEMFlg) {
                sprintf(actualResponse, ("Step: 发送22服务读诊断数据：22 41 C3 "));
            }
            else {
                sprintf(actualResponse, ("Step: 发送22服务读诊断数据：22 D1 31 "));
            }
            TestStep(actualResponse);
            u8 checkCurrentData[48] = { 0 };
            u8 checkSize = 0;
            SID22_ReadDataByIdentifier(DID);
            if (0 == CheckPosResponse_SID22(DID)) {
                if (1 == gCEMFlg) {
                    StoreArray(checkCurrentData, TpRxData, 48, 3, 0);
                    checkSize = 48;
                }
                else {
                    StoreArray(checkCurrentData, TpRxData, 32, 3, 0);
                    checkSize = 32;
                }
            }
            u8 checkCount = 0;
            for (checkCount = 0; checkCount < checkSize; ++checkCount) {
                if (checkCurrentData[checkCount] != lastDataRecord[checkCount]) {
                    break;
                }
            }
            if (checkCount >= checkSize) {
                TestReport_HTML_StepDescription("读取数据与上次2E成功记录相同", Pass);
            }
            else {
                TestReport_HTML_StepDescription("读取数据与上次2E成功记录不同", Fail);
            }

            TestWait(1000);
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

// NVM-006 制造写入NVM过程中再读取NVM，检查NVM是否数据丢失
s32 TC_Check_NVM_Data_Loss_While_Read_NVM_006() {
    try {
        u8 DID[2] = {};
        DID[0] = 0x40;
        DID[1] = 0x58;
        s32 DIDsize = 3;
        u8 Data = 0x60;
        u8 DID2E[3] = { 0x40,0x58, Data };
        s32 DID2Esize = 3;
        double Vol = 0;
        s32 flag = 0;
        strcpy(actualResponse, "");

        //低电压情况下，将NVM数据写入
        strcpy(TestCaseName, "TC_NVM_006");
        strcpy(TestScopeName, "NVM");
        strcpy(testPurpose_CH, "测试目的：");

        TestPrecondition();
        TestInfoPrint("测试次数：%d", NVMCount);
        FulfilTRC();
        SID10_SessionControl(0x03);
        CheckPosResponse_SID10(0x3);
        if (gCEMFlg == 1) {
            SecurityUnlock(0x03);
            CarModSwtich(0);
        }
        TestWait(100);
        SecurityUnlock(0x05);
        TestReport_HTML_StepDescription("Step: 输入电压12V", Info);
        Vol = SetVolatgeAndRead(12, 1);

        sprintf(actualResponse, "Check: 当前电压为%.3fV", Vol);
        TestReport_HTML_StepDescription(actualResponse, Info);
        NVMREG();
        for (s32 i = 0; i < NVMCount; i++) {
            stepnumber = 0;
            TestInfoPrint("当前测试第%d次", i+1);
            CheckCurrentSession(0x03);

            memset(TpRxData, 0, sizeof(TpRxData));
            //3.
            DID2E[2] = 0;
            TCANFD WriteMessage = { Diag_ch,0,8,3,Diag_Request_ID,0,{0x04,0x2E,0x40,0x58,0xAA,0xAA,0xAA,0xAA} };
            TCANFD ReadMessage = { Diag_ch,0,8,3,Diag_Request_ID,0,{0x03,0x22,0x40,0x58,0xAA,0xAA,0xAA,0xAA} };
            for (int times = 0; times < 100; ++times) {
                stepnumber = 0;
                DID2E[2]++;
                sprintf(actualResponse, ("Step: 发送2E服务写诊断数据：2E 40 58 " + dec2hex(DID2E[2])).c_str());
                TestStep(actualResponse);
                //SID2E_WriteDataById(DID2E, DID2Esize);
                //CheckPosResponse_SID2E(DID);
                WriteMessage.FData[4] = DID2E[2];
                com.transmit_canfd_async(&WriteMessage);
                TestWait(18);
                TestStep("Step: 发送22服务读诊断数据：22 40 58");
                com.transmit_canfd_async(&ReadMessage);
                
                if (TpRxData[0] == 0x6E && TpRxData[1] == 0x40 && TpRxData[2] == 0x58) {
                    TestInfoPrintPass("写入期望数据成功2E 40 58 %02X", TpRxData[3]);
                }
                else if(TpRxData[0] == 0x62){
                    TestInfoPrintFail("读取到22服务");
                }
                memset(TpRxData, 0, sizeof(TpRxData));
                TestWait(80);
            }
            TestWait(1000);
            SID22_ReadDataByIdentifier(DID);
            CheckNVM_SID22(DID, 1, DID2E[2]);
        }
        UNNVMREG();
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

// NVM-007 制造高CPU LOAD，检查NVM数据是否写入成功
s32 TC_Check_NVM_Write_High_CPU_Load_NVM_007() {
    try {
        u8 DID[2] = {};
        DID[0] = 0x40;
        DID[1] = 0x58;

        u8 Data = 0x10;
        u8 DID2E[3] = { 0x40,0x58, Data };
        s32 DID2Esize = 3;
        double Vol = 0;
        s32 flag = 0;
        strcpy(actualResponse, "");

        //低电压情况下，将NVM数据写入
        strcpy(TestCaseName, "TC_NVM_007");
        strcpy(TestScopeName, "NVM");
        strcpy(testPurpose_CH, "测试目的：");

        TestPrecondition();
        TestInfoPrint("测试次数：%d", NVMCount);
        FulfilTRC();
        SID10_SessionControl(0x03);
        CheckPosResponse_SID10(0x3);
        TCANFD ReadIdentifierMessage = { Diag_ch,0,8,3,Diag_Request_ID,0,{0x03,0x22,0xF1,0x86,0xAA,0xAA,0xAA,0xAA} };
        //com.add_cyclic_message_canfd(&ReadIdentifierMessage,50);
        if (gCEMFlg == 1) {
            SecurityUnlock(0x03);
            CarModSwtich(0);
        }
        app.wait(100, "");
        SecurityUnlock(0x05);

        for (s32 i = 0; i < NVMCount; i++) {
            stepnumber = 0;
            TestInfoPrint("当前测试第%d次", i + 1);
            DID2E[2] = Data;
            //1.
            sprintf(actualResponse, ("Step: 发送2E服务写诊断数据：2E 40 58 " + dec2hex(Data)).c_str());
            TestStep(actualResponse);
            //TestReport_HTML_StepDescription(("发送2E 40 58 " + dec2hex(Data)).c_str(), Info);
            SID2E_WriteDataById(DID2E, DID2Esize);
            //TestReport_HTML_StepDescription("检查2E响应" , Info);
            CheckPosResponse_SID2E(DID);
            //7.
            TestStep("Step: 发送22服务读诊断数据：22 40 58");
            SID22_ReadDataByIdentifier(DID);
            CheckNVM_SID22(DID, 1, Data);
            Data++;

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
// NVM-011 正常写入情况下，hard reset后，检查NVM数据是否写入成功
s32 TC_Check_NVM_Write_Success_Hard_Reset_NVM_011() {
    try {
        u8 DID[2] = {};
        DID[0] = 0x40;
        DID[1] = 0x58;

        u8 Data = 0x30;
        u8 DID2E[3] = { 0x40,0x58, Data };
        s32 DID2Esize = 3;
        double Vol = 0;
        s32 flag = 0;
        strcpy(actualResponse, "");

        //低电压情况下，将NVM数据写入
        strcpy(TestCaseName, "TC_NVM_011");
        strcpy(TestScopeName, "NVM");
        strcpy(testPurpose_CH, "测试目的：");

        TestPrecondition();

        TestInfoPrint("测试次数：%d", NVMCount);
        SID10_SessionControl(0x03);
        CheckPosResponse_SID10(0x3);
        if (gCEMFlg == 1) {
            SecurityUnlock(0x03);
            CarModSwtich(0);
        }
        app.wait(100, "");

        for (s32 i = 0; i < NVMCount; i++) {
            stepnumber = 0;
            TestInfoPrint("当前测试第%d次", i + 1);

            SID10_SessionControl(0x03);
            CheckPosResponse_SID10(0x3);
            SecurityUnlock(0x05);
            DID2E[2] = Data;
            //1.
            sprintf(actualResponse, ("Step: 发送2E服务写诊断数据：2E 40 58 " + dec2hex(Data)).c_str());
            TestStep(actualResponse);
            //TestReport_HTML_StepDescription(("发送2E 40 58 " + dec2hex(Data)).c_str(), Info);
            SID2E_WriteDataById(DID2E, DID2Esize);
            //TestReport_HTML_StepDescription("检查2E响应" , Info);
            CheckPosResponse_SID2E(DID);

            //2.
            TestStep("Hard Reset");
            SID11_ECUReset(0x01);
            CheckPosResponse_SID11(0x01);
            APP_Flag = 0;
            NM_Flag = 0;
            //3.
            TestStep("等待10s后，确认板子上电工作正常");
            TestWait(10000);
            //TestWaitForCurInRange(10, 500000);
            if (APP_Flag == 1) {
                TestReport_HTML_StepDescription("板子 Reset 成功", Pass);
            }
            else {
                TestReport_HTML_StepDescription("板子 Reset 失败", Fail);
            }

            //4.
            TestStep(("记录最后一次的2E服务写成功的数值" + dec2hex(Data)).c_str());


            TestStep("Step: 发送22服务读诊断数据：22 40 58");
            SID22_ReadDataByIdentifier(DID);
            CheckNVM_SID22(DID, 1, Data);
            Data++;
            TestWait(100);
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

// NVM-012 写入NVM过程中，hard reset后，检查是否NVM能够访问
s32 TC_Check_NVM_Access_Hard_Reset_NVM_012() {
    try {
        u8 DID[2] = {};
        if (1 == gCEMFlg) {
            DID[0] = 0x41;
            DID[1] = 0xC3;
        }
        else {
            DID[0] = 0xD1;
            DID[1] = 0x31;
        }

        u8 DID2E[34] = { 0 };
        DID2E[0] = DID[0];
        DID2E[1] = DID[1];
        memset(DID2E + 2, 0, 32);
        DID2E[30] = 0x01;
        DID2E[31] = 0x01;
        DID2E[32] = 0x01;
        DID2E[33] = 0x01;
        u8 DID2E_2[34] = { 0 };
        DID2E_2[0] = DID[0];
        DID2E_2[1] = DID[1];
        memset(DID2E_2 + 2, 0, 32);
        DID2E_2[30] = 0x02;
        DID2E_2[31] = 0x02;
        DID2E_2[32] = 0x02;
        DID2E_2[33] = 0x02;
        s32 DID2Esize = 34;

        u8 DID2E_ZCUD[50] = { 0 };
        DID2E_ZCUD[0] = DID[0];
        DID2E_ZCUD[1] = DID[1];
        memset(DID2E_ZCUD + 2, 0, 48);
        DID2E_ZCUD[46] = 0x01;
        DID2E_ZCUD[47] = 0x01;
        DID2E_ZCUD[48] = 0x01;
        DID2E_ZCUD[49] = 0x01;
        u8 DID2E_ZCUD_2[50] = { 0 };
        DID2E_ZCUD_2[0] = DID[0];
        DID2E_ZCUD_2[1] = DID[1];
        memset(DID2E_ZCUD_2 + 2, 0, 48);
        DID2E_ZCUD_2[46] = 0x02;
        DID2E_ZCUD_2[47] = 0x02;
        DID2E_ZCUD_2[48] = 0x02;
        DID2E_ZCUD_2[49] = 0x02;
        s32 DID2E_ZCUDsize = 50;
        double Vol = 0;
        s32 flag = 0;
        strcpy(actualResponse, "");

        //低电压情况下，将NVM数据写入
        strcpy(TestCaseName, "TC_NVM_012");
        strcpy(TestScopeName, "NVM");
        strcpy(testPurpose_CH, "测试目的：");

        TestPrecondition();
        SimulateTRC(true);
        TestInfoPrint("测试次数：%d", NVMCount);
        SID10_SessionControl(0x03);
        CheckPosResponse_SID10(0x3);
        if (gCEMFlg == 1) {
            SecurityUnlock(0x03);
            CarModSwtich(0);
        }
        TestWait(100);

        for (s32 i = 0; i < NVMCount; i++) {
            stepnumber = 0;
            TestReport_HTML_StepDescription("", Info);

            SID10_SessionControl(0x03);
            CheckPosResponse_SID10(0x3);
            SecurityUnlock(0x11);
            u8 lastDataRecord[48] = { 0 };
            SID22_ReadDataByIdentifier(DID, 2);
            if (0 == CheckPosResponse_SID22(DID)) {
                if (1 == gCEMFlg) {
                    StoreArray(lastDataRecord, TpRxData, 48, 3, 0);
                }
                else {
                    StoreArray(lastDataRecord, TpRxData, 32, 3, 0);
                }
            }

            //1.
            if (1 == gCEMFlg) {
                sprintf(actualResponse, ("Step: 发送2E服务写诊断数据：2E 41 C3 "));
                TestStep(actualResponse);
                //TestReport_HTML_StepDescription(("发送2E 40 58 " + dec2hex(Data)).c_str(), Info);
                SID2E_WriteDataById(DID2E_ZCUD, DID2E_ZCUDsize);
            }
            else {
                sprintf(actualResponse, ("Step: 发送2E服务写诊断数据：2E D1 31 "));
                TestStep(actualResponse);
                //TestReport_HTML_StepDescription(("发送2E 40 58 " + dec2hex(Data)).c_str(), Info);
                SID2E_WriteDataById(DID2E, DID2Esize);
            }

            CheckPosResponse_SID2E(DID);
            if (1 == gCEMFlg) {
                sprintf(actualResponse, ("Step: 发送2E服务写诊断数据：2E 41 C3 "));
                TestStep(actualResponse);
                SID2E_WriteDataById_Request(DID2E_ZCUD_2, DID2E_ZCUDsize);
                
            }
            else {
                sprintf(actualResponse, ("Step: 发送2E服务写诊断数据：2E D1 31 "));
                TestStep(actualResponse);
                SID2E_WriteDataById_Request(DID2E_2, DID2Esize);
            }

            memset(TpRxData, 0, sizeof(TpRxData));
            TestWait(1000);
            //2.
            TestStep("Hard Reset");
            SID11_ECUReset(0x01);
            CheckPosResponse_SID11(0x01);
            APP_Flag = 0;
            NM_Flag = 0;
            //3.
            TestStep("等待10s后，确认板子上电工作正常");
            TestWait(10000);
            //TestWaitForCurInRange(10, 500000);
            if (APP_Flag == 1) {
                TestReport_HTML_StepDescription("板子 Reset 成功", Pass);
            }
            else {
                TestReport_HTML_StepDescription("板子 Reset 失败", Fail);
            }

            //4.
            TestStep(("读取最后一次的2E服务写成功的数值"));

            //5.
            if (1 == gCEMFlg) {
                sprintf(actualResponse, ("Step: 发送22服务读诊断数据：22 41 C3 "));
            }
            else {
                sprintf(actualResponse, ("Step: 发送22服务读诊断数据：22 D1 31 "));
            }
            TestStep(actualResponse);
            u8 checkCurrentData[48] = { 0 };
            u8 checkSize = 0;
            SID22_ReadDataByIdentifier(DID);
            if (0 == CheckPosResponse_SID22(DID)) {
                if (1 == gCEMFlg) {
                    StoreArray(checkCurrentData, TpRxData, 48, 3, 0);
                    checkSize = 48;
                }
                else {
                    StoreArray(checkCurrentData, TpRxData, 32, 3, 0);
                    checkSize = 32;
                }
            }
            u8 checkCount = 0;
            for (checkCount = 0; checkCount < checkSize; ++checkCount) {
                if (checkCurrentData[checkCount] != lastDataRecord[checkCount]) {
                    break;
                }
            }
            if (checkCount >= checkSize) {
                TestReport_HTML_StepDescription("读取数据与上次2E成功记录相同", Pass);
            }
            else {
                TestReport_HTML_StepDescription("读取数据与上次2E成功记录不同", Fail);
            }

            TestWait(1000);
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

// NVM-015 正常写入情况下，进行版本升级后，检查NVM数据是否写入成功
s32 TC_Check_NVM_Write_Version_Upgrade_NVM_015() {
    try {
        u8 DID[2] = {};
        DID[0] = 0x40;
        DID[1] = 0x58;

        u8 Data = 0x30;
        u8 DID2E[3] = { 0x40,0x58, Data };
        s32 DID2Esize = 3;
        double Vol = 0;
        s32 flag = 0;
        strcpy(actualResponse, "");

        //低电压情况下，将NVM数据写入
        strcpy(TestCaseName, "TC_NVM_015");
        strcpy(TestScopeName, "NVM");
        strcpy(testPurpose_CH, "测试目的：");

        TestPrecondition();
        SimulateTRC(true);
        TestInfoPrint("测试次数：%d", NVMCount);
        SID10_SessionControl(0x03);
        CheckPosResponse_SID10(0x3);
        if (gCEMFlg == 1) {
            SecurityUnlock(0x03);
            CarModSwtich(0);
        }
        app.wait(100, "");

        for (s32 i = 0; i < NVMCount; i++) {
            stepnumber = 0;
            TestInfoPrint("当前测试第%d次", i + 1);
            TestInfoPrint("刷写旧版本");
            FullFileDownload_VersionA();

            SID10_SessionControl(0x03);
            CheckPosResponse_SID10(0x3);
            SecurityUnlock(0x05);
            DID2E[2] = Data;
            //1.
            sprintf(actualResponse, ("Step: 发送2E服务写诊断数据：2E 40 58 " + dec2hex(Data)).c_str());
            TestStep(actualResponse);
            //TestReport_HTML_StepDescription(("发送2E 40 58 " + dec2hex(Data)).c_str(), Info);
            SID2E_WriteDataById(DID2E, DID2Esize);
            //TestReport_HTML_StepDescription("检查2E响应" , Info);
            CheckPosResponse_SID2E(DID);

            TestWait(1000);
            TestStep("Update Software Version");

            FullFileDownload();
            TestStep("重新上下电并等待10s后，确认板子工作正常");
            KL30(off);
            TestWait(1000);
            KL30(on);
            //3.
            TestWait(10000);
            //TestWaitForCurInRange(10, 500000);
            if (APP_Flag == 1) {
                TestReport_HTML_StepDescription("板子 Reset 成功", Pass);
            }
            else {
                TestReport_HTML_StepDescription("板子 Reset 失败", Fail);
            }

            //4.
            TestStep(("记录最后一次的2E服务写成功的数值" + dec2hex(Data)).c_str());

            //5.

            TestStep("Step: 发送22服务读诊断数据：22 40 58");
            SID22_ReadDataByIdentifier(DID);
            CheckNVM_SID22(DID, 1, Data);
            Data++;
            TestWait(100);
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

// NVM-016 写入NVM过程中，进行版本升级后，检查是否NVM能够访问
s32 TC_Check_NVM_Access_Version_Upgrade_NVM_016() {
    try {
        u8 DID[2] = {};
        if (1 == gCEMFlg) {
            DID[0] = 0x41;
            DID[1] = 0xC3;
        }
        else {
            DID[0] = 0xD1;
            DID[1] = 0x31;
        }

        u8 DID2E[34] = { 0 };
        DID2E[0] = DID[0];
        DID2E[1] = DID[1];
        memset(DID2E + 2, 0, 32);
        DID2E[30] = 0x01;
        DID2E[31] = 0x01;
        DID2E[32] = 0x01;
        DID2E[33] = 0x01;
        u8 DID2E_2[34] = { 0 };
        DID2E_2[0] = DID[0];
        DID2E_2[1] = DID[1];
        memset(DID2E_2 + 2, 0, 32);
        DID2E_2[30] = 0x02;
        DID2E_2[31] = 0x02;
        DID2E_2[32] = 0x02;
        DID2E_2[33] = 0x02;
        s32 DID2Esize = 34;

        u8 DID2E_ZCUD[50] = { 0 };
        DID2E_ZCUD[0] = DID[0];
        DID2E_ZCUD[1] = DID[1];
        memset(DID2E_ZCUD + 2, 0, 48);
        DID2E_ZCUD[46] = 0x01;
        DID2E_ZCUD[47] = 0x01;
        DID2E_ZCUD[48] = 0x01;
        DID2E_ZCUD[49] = 0x01;
        u8 DID2E_ZCUD_2[50] = { 0 };
        DID2E_ZCUD_2[0] = DID[0];
        DID2E_ZCUD_2[1] = DID[1];
        memset(DID2E_ZCUD_2 + 2, 0, 48);
        DID2E_ZCUD_2[46] = 0x02;
        DID2E_ZCUD_2[47] = 0x02;
        DID2E_ZCUD_2[48] = 0x02;
        DID2E_ZCUD_2[49] = 0x02;
        s32 DID2E_ZCUDsize = 50;
        double Vol = 0;
        s32 flag = 0;
        strcpy(actualResponse, "");

        //低电压情况下，将NVM数据写入
        strcpy(TestCaseName, "TC_NVM_016");
        strcpy(TestScopeName, "NVM");
        strcpy(testPurpose_CH, "测试目的：");

        TestPrecondition();
        TestInfoPrint("测试次数：%d", NVMCount);
        SimulateTRC(true);
        SID10_SessionControl(0x03);
        CheckPosResponse_SID10(0x3);
        if (gCEMFlg == 1) {
            SecurityUnlock(0x03);
            CarModSwtich(0);
        }
        TestWait(100);

        for (s32 i = 0; i < NVMCount; i++) {
            stepnumber = 0;
            TestInfoPrint("刷写旧版本");
            FullFileDownload_VersionA();

            SID10_SessionControl(0x03);
            CheckPosResponse_SID10(0x3);
            SecurityUnlock(0x11);
            u8 lastDataRecord[48] = { 0 };
            SID22_ReadDataByIdentifier(DID, 2);
            if (0 == CheckPosResponse_SID22(DID)) {
                if (1 == gCEMFlg) {
                    StoreArray(lastDataRecord, TpRxData, 48, 3, 0);
                }
                else {
                    StoreArray(lastDataRecord, TpRxData, 32, 3, 0);
                }
            }

            //1.
            if (1 == gCEMFlg) {
                sprintf(actualResponse, ("Step: 发送2E服务写诊断数据：2E 41 C3 "));
                TestStep(actualResponse);
                //TestReport_HTML_StepDescription(("发送2E 40 58 " + dec2hex(Data)).c_str(), Info);
                SID2E_WriteDataById(DID2E_ZCUD, DID2E_ZCUDsize);
            }
            else {
                sprintf(actualResponse, ("Step: 发送2E服务写诊断数据：2E D1 31 "));
                TestStep(actualResponse);
                //TestReport_HTML_StepDescription(("发送2E 40 58 " + dec2hex(Data)).c_str(), Info);
                SID2E_WriteDataById(DID2E, DID2Esize);
            }

            CheckPosResponse_SID2E(DID);
            if (1 == gCEMFlg) {
                sprintf(actualResponse, ("Step: 发送2E服务写诊断数据：2E 41 C3 "));
                TestStep(actualResponse);
                SID2E_WriteDataById_Request(DID2E_ZCUD_2, DID2E_ZCUDsize);
            }
            else {
                sprintf(actualResponse, ("Step: 发送2E服务写诊断数据：2E D1 31 "));
                TestStep(actualResponse);
                SID2E_WriteDataById_Request(DID2E_2, DID2Esize);
            }
            TestWait(1000);
            TestStep("Update Software Version");

            FullFileDownload();
            TestStep("重新上下电并等待10s后，确认板子工作正常");
            KL30(off);
            TestWait(1000);
            KL30(on);
            //3.
            TestWait(10000);
            //TestWaitForCurInRange(10, 500000);
            if (APP_Flag == 1) {
                TestReport_HTML_StepDescription("板子 Reset 成功", Pass);
            }
            else {
                TestReport_HTML_StepDescription("板子 Reset 失败", Fail);
            }

            //4.
            TestStep(("读取最后一次的2E服务写成功的数值"));

            //5.
            if (1 == gCEMFlg) {
                sprintf(actualResponse, ("Step: 发送22服务读诊断数据：22 41 C3 "));
            }
            else {
                sprintf(actualResponse, ("Step: 发送22服务读诊断数据：22 D1 31 "));
            }
            TestStep(actualResponse);
            u8 checkCurrentData[48] = { 0 };
            u8 checkSize = 0;
            SID22_ReadDataByIdentifier(DID);
            if (0 == CheckPosResponse_SID22(DID)) {
                if (1 == gCEMFlg) {
                    StoreArray(checkCurrentData, TpRxData, 48, 3, 0);
                    checkSize = 48;
                }
                else {
                    StoreArray(checkCurrentData, TpRxData, 32, 3, 0);
                    checkSize = 32;
                }
            }
            u8 checkCount = 0;
            for (checkCount = 0; checkCount < checkSize; ++checkCount) {
                if (checkCurrentData[checkCount] != lastDataRecord[checkCount]) {
                    break;
                }
            }
            if (checkCount >= checkSize) {
                TestReport_HTML_StepDescription("读取数据与上次2E成功记录相同", Pass);
            }
            else {
                TestReport_HTML_StepDescription("读取数据与上次2E成功记录不同", Fail);
            }

            TestWait(1000);
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
#endif // !TestCase_CAN_NVM