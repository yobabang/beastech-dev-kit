
#ifndef TestCase_CAN_NM
#define TestCase_CAN_NM



#include "test_function.h"
#include "test_case_diag_api.h"

#include<vector>
#include <unordered_map>
#include < numeric >
#include <set>
#include <algorithm>

//#define	GET_BIT(x, bit)	((x & (1 << bit)) >> bit)	/* 获取x第bit位,第0位:GET_BIT(a, 0) */
//尽量不要连着CAN2和CAN1在一起
//extern void TestBegin();
//extern void TestEnd();
s32 IfListenLIN = 0;

s32 wait()
{
    app.wait(1000, "");
    return 0;
}

void check_nm_pdu_count_is_1(u8 m)
{
    if (DUT_NMRxMsg[m].size() == 1)
        TestInfoPrintPass("网段CAN%d发送了1个NM PDU，符合预期", m + 1);
    else
        TestInfoPrintFail("网段CAN%d发送了%d个NM PDU，预期1个", m + 1, DUT_NMRxMsg[m].size());
}

void check_nm_pdu_count_is_over_n(u8 m, u8 n)
{
    if (DUT_NMRxMsg[m].size() >= n)
        TestInfoPrintPass("网段CAN%d发送了%d个NM PDU，符合预期(>=%d)", m + 1, DUT_NMRxMsg[m].size(), n);
    else
        TestInfoPrintFail("网段CAN%d发送了%d个NM PDU，预期至少%d个", m + 1, DUT_NMRxMsg[m].size(), n);
}

void check_nm_dlc_is_8_and_type_is_can(u8 m)
{
    if (DUT_NMRxMsg[m].size() == 0) {
        TestInfoPrintFail("网段CAN%d没有NM报文，无法校验DLC和类型", m + 1);
        return;
    }
    if (DUT_NMRxMsg[m][0].FDLC == 8)
        TestInfoPrintPass("网段CAN%d NM报文DLC为8，类型为CAN报文", m + 1);
    else
        TestInfoPrintFail("网段CAN%d NM报文DLC为%d，预期8", m + 1, DUT_NMRxMsg[m][0].FDLC);
}

void check_source_node_id(u8 m)
{
    if (DUT_NMRxMsg[m].size() == 0) {
        TestInfoPrintFail("网段CAN%d没有NM报文，无法校验source node ID", m + 1);
        return;
    }
    u8 source_id = DUT_NMRxMsg[m][0].FData[0];
    if (source_id == (bus_configuration[m].dut_send_nm_msg_id & 0xFF))
        TestInfoPrintPass("网段CAN%d NM报文source node ID: 0x%X，符合预期", m + 1, source_id);
    else
        TestInfoPrintFail("网段CAN%d NM报文source node ID: 0x%X，预期0x%X", m + 1, source_id, bus_configuration[m].dut_send_nm_msg_id & 0xFF);
}

void check_nm_msg_cbv_pni_byte6_is_1_and_byte1257_is_0(u8 m)
{
    if (DUT_NMRxMsg[m].size() == 0) {
        TestInfoPrintFail("网段CAN%d没有NM报文，无法校验CBV", m + 1);
        return;
    }
    u8 cbv = DUT_NMRxMsg[m][0].FData[1];
    bool pni_ok = (cbv & 0x40) != 0;
    bool unused_ok = (cbv & 0xA2) == 0;
    if (pni_ok && unused_ok)
        TestInfoPrintPass("网段CAN%d NM报文CBV=0x%X，PNI(bit6)=1，未使用位(bit1,2,5,7)=0，符合预期", m + 1, cbv);
    else
    {
        char err[256] = {0};
        if (!pni_ok) strcat(err, "PNI(bit6)不为1; ");
        if (!unused_ok) strcat(err, "未使用位(bit1,2,5,7)不为0; ");
        TestInfoPrintFail("网段CAN%d NM报文CBV=0x%X，%s", m + 1, cbv, err);
    }
}

void check_dut_send_nm_pdu_AWB_is_n(u8 m, u8 n)
{
    if (DUT_NMRxMsg[m].size() == 0) {
        TestInfoPrintFail("网段CAN%d没有NM报文，无法校验AWB", m + 1);
        return;
    }
    u8 awb = (DUT_NMRxMsg[m][0].FData[1] >> 4) & 0x01;
    if (awb == n)
        TestInfoPrintPass("网段CAN%d NM报文AWB(bit4)=%d，符合预期", m + 1, awb);
    else
        TestInfoPrintFail("网段CAN%d NM报文AWB(bit4)=%d，预期%d", m + 1, awb, n);
}

void record_pnc_gateway_AWB(u8 m)
{
    if (DUT_NMRxMsg[m].size() == 0) {
        TestInfoPrintWarning("网段CAN%d没有NM报文，无法记录AWB", m + 1);
        return;
    }
    u8 awb = (DUT_NMRxMsg[m][0].FData[1] >> 4) & 0x01;
    TestInfoPrint("网段CAN%d PNC gateway NM报文AWB(bit4)=%d", m + 1, awb);
}

s32 TC_1023218_CAN_NM_PNC_in_Mask_wakeup_and_down()
{
    try
    {

        strcpy(TestCaseName, "TC_1023218_CAN_NM_PNC_in_Mask_wakeup_and_down");
        strcpy(TestScopeName, "NM");
        strcpy(testPurpose_CH, "NM");

        //TestStep("测试开始");
		pre_test();

        print_all_bus_configurations();

		u8 m;
		for (m = 1; m < bus_count; m++)
		{
			if (bus_configuration[m].if_support_I_PDU_control == 1)
			//if (1)
			{
				std::vector <u16> return_dut_need_send_msg_id;
				std::vector <u16> return_dut_need_send_pnc_msg_id;
				stepnumber = 0;
				
				sprintf(actualResponse,"++++++++++++++++++++++++++++++++++++++++当前通道%s为CAN%d++++++++++++++++++++++++++++++++++++++++", NetworkCluster_CH[m].c_str(), m + 1);
                TestStep(actualResponse);
                //if (gCEMFlg == 1) {
                //    if (NetworkCluster_CH[m] == "ZCU_CANFD2" || NetworkCluster_CH[m] == "BodyExposedCAN") {
                //        snprintf(StepDespriction, sizeof(StepDespriction), "当前通道%s不适用", NetworkCluster_CH[m].c_str());
                //        TestReport_HTML_StepDescription(StepDespriction, Warning);
                //        continue;
                //    }
                //}
                
				u16 test_id = nm_dut_send_id_min;
				u8 test_data[8] = { test_id & 0XFF,0x40 };
				TestStep("————————————————————————————————————————————————————");
				TestStep("关闭网络仿真，开启供电电源，检查网段处于休眠，如果该网段不支持网络唤醒，则触发其通信并执行下一步");
				close_simulate_network_and_begin_power_on_and_check(m);
                if (bus_configuration[m].if_support_network_wakeup != 1) {
                    printf("trigger_wakeup");
                    trigger_wakeup(m);
                }

				for (int i = 0; i < pnc_max_count_in_one_bus; i++)
				{
					if (bus_configuration[m].relevant_pnc[i] != 0)
					{
                        ClearAllNMVector();
						set_InterestPNC_1(bus_configuration[m].relevant_pnc[i], test_data);
						sprintf(actualResponse, "开始仿真NM PDU：0x%X，PNI置位，将InterestPNC中某个PNC%d置位，NM data:0x%X，0x%X，0x%X，0x%X，0x%X，0x%X，0x%X，0x%X，仿真快发，即以20ms周期发送20次NM PDU，然后恢复1s周期，检查DUT开始通信", test_id, bus_configuration[m].relevant_pnc[i], test_data[0], test_data[1], test_data[2], test_data[3], test_data[4], test_data[5], test_data[6], test_data[7]);
						TestStep(actualResponse);
						send_nm_msg_fast2nor(m, test_id, test_data);
						check_com(m, 0, 3000);
						stop_send_nm_msg();

						sprintf(actualResponse,"等待2s，检查PN识别和映射关系：与仿真的PNC%d相关的MapFrames和NoMapFrames应该被发送到该网段；与仿真的PNC无关的MapFrames不应该发送到该网段", bus_configuration[m].relevant_pnc[i]);
						TestStep(actualResponse);
						app.wait(2000, "");
						return_dut_need_send_msg_id = check_dut_send_pnc_MapFrames_and_NoMapFrames(m, bus_configuration[m].relevant_pnc[i]);

                        //TestInfoPrint("等待10s，检查映射报文停止时间，从最后一个相关PNC开始计时（检查周期报文），对每个检测报文，停止时间为CanNmPnResetTime在[%d，%d]ms内", CanNmPnResetTimeMin, CanNmPnResetTimeMax);
                        TestInfoPrint("检查映射报文停止时间，从最后一个相关PNC开始计时（检查周期报文），对每个检测报文，停止时间为CanNmPnResetTime在[%d，%d]ms内", CanNmPnResetTimeMin, CanNmPnResetTimeMax);
                        return_dut_need_send_pnc_msg_id = return_dut_send_pnc_MapFrames(m, bus_configuration[m].relevant_pnc[i]);
						//app.wait(10000, "");
						check_map_msg_time(m, return_dut_need_send_pnc_msg_id);

						TestStep("等待网段停止发送NM PDU，检查到停止通信");
						check_com(m, 2);
					}
					else
						break;
				}
				//KL30(off);
                MainPower_OFF();
                AppWait(5000, "");
			}
			else
				TestInfoPrintWarning("网段CAN%d，不支持I-PDU控制，不满足测试条件", m + 1);
		}
		


        TestStep("测试结束");
		post_test();
        //TestPostCondition();

        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
	pre_test();

}

s32 TC_Sleep_And_WakeUp() {
    try {
        strcpy(TestCaseName, "TC_Sleep_And_WakeUp");
        strcpy(TestScopeName, "Sleep And WakeUp");
        strcpy(testPurpose_CH, "Sleep And WakeUp");

        TestPrecondition();

        TestReport_HTML_StepDescription("深度休眠测试");

        //FulfilTRC();

        //UsgModSwtich(0x1);
        //EPLvlSwitch(0x0);
        app.wait(1000, "");
        SendTesterPresent(TP_OFF);
        NMWakeUp(0);
        com.del_cyclic_messages();


        TestWaitForCurInRange(gLowPwrCur, gSleepTime);

        //Step8
        TestStep("关闭电源，然后等待 10 秒钟");
        //KL30(off);
        MainPower_OFF();
        app.wait(10000, "wait for 10s");

        //Step9
        TestStep("设置电压为12V");
        SetVolatgeAndRead(12);

        //Step10
        TestStep("上电并唤醒被测件");
        //KL30(on);
        //KL15(on);
        MainPower_ON();
        app.wait(2000, "");
        NMWakeUp(1);
        SendTesterPresent(TP_ON);

        TestReport_HTML_StepDescription("开始检查通信");
        CheckCommunication(5000, 1000);

        TestWait(1000);

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

s32 TC_1023210_CAN_NM_Frame_ID_out_of_Range_not_wakeup()
{
    try {
    strcpy(TestCaseName, "TC_1023210_CAN_NM_Frame_ID_out_of_Range_not_wakeup");
    strcpy(TestScopeName, "NM");
    strcpy(testPurpose_CH, "NM");
    pre_test();
    u8 m;
    for (m = 1; m < bus_count; m++)
    {
        ClearAllVector();
        if (bus_configuration[m].if_can_be_sleep_with_power == 1 && bus_configuration[m].if_support_network_wakeup == 1)
        {
            stepnumber = 0;

            sprintf(actualResponse, "++++++++++++++++++++++++++++++++++++++++当前通道%s为CAN%d++++++++++++++++++++++++++++++++++++++++", NetworkCluster_CH[m].c_str(), m + 1);
            TestStep(actualResponse);

            int i;
            u16 dut_recv_msg_id = bus_configuration[m].rx_msg[0];
            u16 test_id[3] = { 0x4FF,0x540,dut_recv_msg_id };
            for (i = 0; i < 3; i++)
            {
                u8 test_data[8] = { test_id[i] & 0XFF,0x40,0xff,0xff,0xff,0xff,0xff,0xff };

                TestStep("关闭网络仿真，开启供电电源，如果网段需要额外条件来进入睡眠，满足此条件，检查网段处于休眠");
                close_simulate_network_and_begin_power_on_and_check(m);


                sprintf(actualResponse, "发送ID为0x%X的报文，PNI置位，所有PNC置位，周期为CanNmMsgCycleTime：%.2fs，检查DUT不应该被唤醒:3s后DUT没有发送NM报文", test_id[i], nm_nor_period / 1000.0);
                TestStep(actualResponse);
                send_nm_msg(m, test_id[i], test_data);
                check_com(m, 3, 0);
                stop_send_nm_msg();

                MainPower_OFF();
                AppWait(5000, "");
            }
        }
        else
        {
            if (bus_configuration[m].if_can_be_sleep_with_power != 1)
                TestInfoPrintWarning("网段CAN%d，在供电状态下不能处于休眠，不满足测试条件", m + 1);
            else
                TestInfoPrintWarning("网段CAN%d，不支持NM唤醒，不满足测试条件", m + 1);
        }

    }
    post_test();
    return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 TC_1023212_CAN_Network_Management_PDU_network_wakeup()
{
    strcpy(TestCaseName, "TC_1023212_CAN_Network_Management_PDU_network_wakeup");
    strcpy(TestScopeName, "NM");
    strcpy(testPurpose_CH, "NM");
    pre_test();
    u8 m;
    for (m = 1; m < bus_count; m++)
    {
        ClearAllVector();
        if (bus_configuration[m].if_can_be_sleep_with_power == 1 && bus_configuration[m].if_support_network_wakeup == 1)
        {
            u16 test_id = 0x53F;
            u8 test_data[8] = { 0X3F,0x40,0xff,0xff,0xff,0xff,0xff,0xff };
            //KL30(on);
            sprintf(actualResponse, "++++++++++++++++++++++++++++++++++++++++当前通道为%s(CAN%d)++++++++++++++++++++++++++++++++++++++++", bus_configuration[m].node_name, m + 1);
            TestStep(actualResponse);

            TestStep("关闭网络仿真，开启供电电源，如果网段需要额外条件来进入睡眠，满足此条件，检查网段处于休眠");
            close_simulate_network_and_begin_power_on_and_check(m);

            sprintf(actualResponse, "发送ID为0x%X的NM报文，PNI置位，所有PNC置位，周期为CanNmMsgCycleTime：%.2fs，检查DUT开始通信", test_id, nm_nor_period / 1000.0);
            TestStep(actualResponse);
            send_nm_msg(m, test_id, test_data);
            check_com(m, 0, 3000);

            TestStep("记录大约15s，停发NM报文，检查DUT发送的NM报文数量。若网段不是PNC gateway，只有1个NM PDU；若是PNC gateway，至少有20个NM PDU");
            TestWait(15000);
            stop_send_nm_msg();
            if (DUT_NMRxMsg[m].size() == 0)
                TestInfoPrintFail("DUT没有发送NM报文，无法校验");
            else
            {
                if (bus_configuration[m].bus_pnc_gateway_type == non_pnc_gateway)
                    check_nm_pdu_count_is_1(m);
                else
                    check_nm_pdu_count_is_over_n(m, 20);

                TestStep("检查NM报文的DLC为8，类型为CAN报文");
                check_nm_dlc_is_8_and_type_is_can(m);

                TestStep("检查NM报文的source node ID");
                check_source_node_id(m);

                TestStep("检查NM报文的CBV字节(byte1)的PNI(bit6)置位1，未使用位(bit1，2，5，7)置位0");
                check_nm_msg_cbv_pni_byte6_is_1_and_byte1257_is_0(m);

                TestStep("检查非PNC网关的NM报文的CBV字节(byte1)的AWB(bit4)置位0，记录PNC网关的AWB位");
                if (ecu_gateway_type == non_pnc_gateway)
                    check_dut_send_nm_pdu_AWB_is_n(m, 0);
                else
                    record_pnc_gateway_AWB(m);

                TestStep("检查NM PDU中的PNC:如果该网段不是PNC gateway或者是Passive PNC gateway：所有的PNC应该为0；如果该网段是Active PNC gateway：与该网段的PNC掩码相关的PNC应该为1，其他PNC为0");
                u8 data[4] = { 0 };
                u8 test_data[4] = { DUT_NMRxMsg[m][0].FData[2] ,DUT_NMRxMsg[m][0].FData[3],DUT_NMRxMsg[m][0].FData[4],DUT_NMRxMsg[m][0].FData[5] };
                if (bus_configuration[m].bus_pnc_gateway_type != active_pnc_gateway)
                {
                    bool cmp_res = memcmp(test_data, data, 4);
                    if (cmp_res == 0)
                        TestInfoPrintPass("网段不是Active PNC gateway：NM PDU:0x%X，所有的PNC为0，报文时间戳%fs", DUT_NMRxMsg[m][0].FIdentifier, DUT_NMRxMsg[m][0].FTimeUs / 1000000.0);
                    else
                        TestInfoPrintFail("网段不是Active PNC gateway：NM PDU:0x%X，所有的PNC不全为0：byte2-byte5:0x%X，0x%X，0x%X，0x%X，报文时间戳%fs", DUT_NMRxMsg[m][0].FIdentifier, test_data[0], test_data[1], test_data[2], test_data[3], DUT_NMRxMsg[m][0].FTimeUs / 1000000.0);
                }
                else
                    check_active_all_pnc_gateway_bit(m);//arxml中是{0，32，0，0}=>pnc输入29，byte3为0x20
            }
            KL30(off);
        }
        else
        {
            if (bus_configuration[m].if_can_be_sleep_with_power != 1)
                TestInfoPrintWarning("网段%s(CAN%d)，在供电状态下不能处于休眠，不满足测试条件", bus_configuration[m].node_name, m + 1);
            else
                TestInfoPrintWarning("网段%s(CAN%d)，不支持NM唤醒，不满足测试条件", bus_configuration[m].node_name, m + 1);
        }
    }
    post_test(); return current_test_result;
}

s32 TC_1023219_CAN_NM_PNC_in_Mask_keep_awake() {
    try{
        strcpy(TestCaseName, "TC_1023219_CAN_NM_PNC_in_Mask_keep_awake");
        strcpy(TestScopeName, "NM");
        strcpy(testPurpose_CH, "NM");
        pre_test();
        print_all_bus_configurations();
        u8 m;
        for (m = 1; m < bus_count; m++)
        {
            if (bus_configuration[m].if_can_be_sleep_with_power == 1 && bus_configuration[m].if_support_I_PDU_control == 1)
            {
                stepnumber = 0;

                sprintf(actualResponse, "++++++++++++++++++++++++++++++++++++++++当前通道%s为CAN%d++++++++++++++++++++++++++++++++++++++++", NetworkCluster_CH[m].c_str(), m + 1);
                TestStep(actualResponse);

                u16 test_id = nm_dut_send_id_min;
                u8 test_data[8] = { test_id & 0XFF,0x40,0xff,0xff,0xff,0xff,0xff,0xff };

                TestStep("关闭网络仿真，开启供电电源，如果网段需要额外条件来进入睡眠，满足此条件，检查网段处于休眠");
                close_simulate_network_and_begin_power_on_and_check(m);

                sprintf(actualResponse, "开始仿真NM PDU：0x%X，PNI置位，所有PNC置位，1s周期，检查DUT开始通信", test_id);
                TestStep(actualResponse);
                send_nm_msg(m, test_id, test_data);
                AppWait(6000, "");
                check_com(m, 0, CanStopCommunicate);
                stop_send_nm_msg();
                TestWaitForCurInRange(gLowPwrCur, gSleepTime);

                for (int i = 0; i < pnc_max_count_in_one_bus; i++)
                {
                    if (bus_configuration[m].relevant_pnc[i] != 0)
                    {
                        int waitTime = 0;
                        ClearAllVector();
                        set_InterestPNC_1(bus_configuration[m].relevant_pnc[i], test_data);
                        sprintf(actualResponse, "开始仿真NM PDU：0x%X，PNI置位，将InterestPNC中PNC%d置位，NM data:0x%X，0x%X，0x%X，0x%X，0x%X，0x%X，0x%X，0x%X，以1s周期发送", test_id, bus_configuration[m].relevant_pnc[i], test_data[0], test_data[1], test_data[2], test_data[3], test_data[4], test_data[5], test_data[6], test_data[7]);
                        TestStep(actualResponse);
                        TestStep;
                        send_nm_msg(m, test_id, test_data);

                        ClearAllVector();
                        if (bus_configuration[m].bus_pnc_gateway_type == active_pnc_gateway) {
                            waitTime = 10;
                        }
                        else {
                            waitTime = 5;
                        }
                        TestInfoPrint("等待%ds", waitTime);
                        AppWait(waitTime * 1000, "");

                        sprintf(actualResponse, "检查PN识别和映射关系：与仿真的PNC%d相关的MapFrames和NoMapFrames应该被发送到该网段；与仿真的PNC无关的MapFrames不应该发送到该网段", bus_configuration[m].relevant_pnc[i]);
                        TestStep(actualResponse);

                        stop_send_nm_msg();
                        check_dut_send_pnc_MapFrames_and_NoMapFrames(m, bus_configuration[m].relevant_pnc[i]);
                        TestWaitForCurInRange(gLowPwrCur, gSleepTime);
                    }
                    else
                        break;
                }

                TestStep("关闭网络仿真，检查到停止通信");
                stop_send_nm_msg();
                check_com(m, 2);

                MainPower_OFF();
                AppWait(5000, "");
            }
            else
            {
                if (bus_configuration[m].if_can_be_sleep_with_power != 1)
                    TestInfoPrintWarning("网段CAN%d，在供电状态下不能处于休眠，不满足测试条件", m + 1);
                else
                    TestInfoPrintWarning("网段CAN%d，不支持I-PDU控制，不满足测试条件", m + 1);
            }

        }
        post_test();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 TC_1023220_CAN_NM_PNC_out_of_Mask_not_wakeup() {
    try{
        strcpy(TestCaseName, "TC_1023220_CAN_NM_PNC_out_of_Mask_not_wakeup");
        strcpy(TestScopeName, "NM");
        strcpy(testPurpose_CH, "NM");
        pre_test();
        u8 m;
        u16 test_id = nm_dut_send_id_min;
        for (m = 1; m < bus_count; m++)
        {
            u8 test_data_all_InterestPNC_is0[8] = { test_id & 0XFF,0x40,0xff,0xff,0xff,0xff,0xff,0xff };
            u8 test_data_all_PNC_is0[8] = { test_id & 0XFF,0x40 };
            if (bus_configuration[m].if_can_be_sleep_with_power == 1 && bus_configuration[m].if_support_I_PDU_control == 1)
            {
                stepnumber = 0;

                sprintf(actualResponse, "++++++++++++++++++++++++++++++++++++++++当前通道%s为CAN%d++++++++++++++++++++++++++++++++++++++++", NetworkCluster_CH[m].c_str(), m + 1);
                TestStep(actualResponse);


                TestStep("关闭网络仿真，开启供电电源，如果网段需要额外条件来进入睡眠，满足此条件，检查网段处于休眠");
                close_simulate_network_and_begin_power_on_and_check(m);

                set_all_InterestPNC_0(m, test_data_all_InterestPNC_is0);
                sprintf(actualResponse, "开始仿真NM PDU：0x%X，PNI置位，除InterestPNC外其他PNC均置位，NM data:0x%X，0x%X，0x%X，0x%X，0x%X，0x%X，0x%X，0x%X，1s周期，检查DUT应该保持休眠无通信", test_id, test_data_all_InterestPNC_is0[0], test_data_all_InterestPNC_is0[1], test_data_all_InterestPNC_is0[2], test_data_all_InterestPNC_is0[3], test_data_all_InterestPNC_is0[4], test_data_all_InterestPNC_is0[5], test_data_all_InterestPNC_is0[6], test_data_all_InterestPNC_is0[7]);
                TestStep(actualResponse);
                send_nm_msg(m, test_id, test_data_all_InterestPNC_is0);
                //AppWait(1000, "");
                check_com(m, 3, CanStopCommunicate);
                stop_send_nm_msg();

                //KL30(off);
                MainPower_OFF();
                AppWait(5000, "");


                TestStep("关闭网络仿真，开启供电电源，如果网段需要额外条件来进入睡眠，满足此条件，检查网段处于休眠");
                close_simulate_network_and_begin_power_on_and_check(m);


                sprintf(actualResponse, "开始仿真NM PDU：0x%X，PNI置位，PNC均为0，1s周期，检查DUT应该保持休眠无通信", test_id);
                TestStep(actualResponse);
                send_nm_msg(m, test_id, test_data_all_PNC_is0);
                //AppWait(1000, "");
                check_com(m, 3, CanStopCommunicate);
                stop_send_nm_msg();

                MainPower_OFF();
                AppWait(5000, "");
            }
            else
            {
                if (bus_configuration[m].if_can_be_sleep_with_power != 1)
                    TestInfoPrintWarning("网段CAN%d，在供电状态下不能处于休眠，不满足测试条件", m + 1);
                else
                    TestInfoPrintWarning("网段CAN%d，不支持I-PDU控制，不满足测试条件", m + 1);
            }
        }
        post_test();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 TC_1023221_CAN_NM_PNC_out_of_Mask_not_keep_awake() {
    try {
        strcpy(TestCaseName, "TC_1023221_CAN_NM_PNC_out_of_Mask_not_keep_awake");
        strcpy(TestScopeName, "NM");
        strcpy(testPurpose_CH, "NM");
        pre_test();
        u8 m;
        u16 test_id = nm_dut_send_id_min;

        for (m = 1; m < bus_count; m++)
        {
            u8 test_data_all_PNC_is0[8] = { test_id & 0XFF,0x40 };
            u8 test_data_all_PNC_is1[8] = { test_id & 0XFF,0x40,0xff,0xff,0xff,0xff,0xff,0xff };
            u8 test_data_all_InterestPNC_is0[8] = { test_id & 0XFF,0x40,0xff,0xff,0xff,0xff,0xff,0xff };
            if (bus_configuration[m].if_can_be_sleep_with_power == 1 && bus_configuration[m].if_support_I_PDU_control == 1)
            {
                stepnumber = 0;

                sprintf(actualResponse, "++++++++++++++++++++++++++++++++++++++++当前通道%s为CAN%d++++++++++++++++++++++++++++++++++++++++", NetworkCluster_CH[m].c_str(), m + 1);
                TestStep(actualResponse);

                TestStep("关闭网络仿真，开启供电电源，如果网段需要额外条件来进入睡眠，满足此条件，检查网段处于休眠");
                close_simulate_network_and_begin_power_on_and_check(m);

                sprintf(actualResponse, "开始仿真NM PDU：0x%X，PNI置位，PNC均为1，1s周期，检查DUT开始通信", test_id);
                TestStep(actualResponse);

                send_nm_msg(m, test_id, test_data_all_PNC_is1);
                check_com(m, 0);

                sprintf(actualResponse, "持续仿真NM报文，维持时间CanNmPnResetTime * 3:%.2fs，检查持续通信", CanNmPnResetTime * 3 / 1000.0);
                TestStep(actualResponse);

                check_com(m, 1, CanNmPnResetTime * 3);
                stop_send_nm_msg();

                set_all_InterestPNC_0(m, test_data_all_InterestPNC_is0);

                sprintf(actualResponse, "开始仿真NM PDU：0x%X，PNI置位，除InterestPNC外其他PNC均置位，NM data:0x%X，0x%X，0x%X，0x%X，0x%X，0x%X，0x%X，0x%X，1s周期，检查DUT应该停止通信", test_id, test_data_all_InterestPNC_is0[0], test_data_all_InterestPNC_is0[1], test_data_all_InterestPNC_is0[2], test_data_all_InterestPNC_is0[3], test_data_all_InterestPNC_is0[4], test_data_all_InterestPNC_is0[5], test_data_all_InterestPNC_is0[6], test_data_all_InterestPNC_is0[7]);
                TestStep(actualResponse);
                send_nm_msg(m, test_id, test_data_all_InterestPNC_is0);
                check_com(m, 2, CanStopCommunicate);
                stop_send_nm_msg();

                MainPower_OFF();
                AppWait(5000, "");

                TestStep("关闭网络仿真，开启供电电源，如果网段需要额外条件来进入睡眠，满足此条件，检查网段处于休眠");
                close_simulate_network_and_begin_power_on_and_check(m);

                sprintf(actualResponse, "开始仿真NM PDU：0x%X，PNI置位，PNC均为1，1s周期，检查DUT开始通信", test_id);
                TestStep(actualResponse);

                send_nm_msg(m, test_id, test_data_all_PNC_is1);
                check_com(m, 0);

                sprintf(actualResponse, "持续仿真NM报文，维持时间CanNmPnResetTime * 3:%.2fs，检查持续通信", CanNmPnResetTime * 3 / 1000.0);
                TestStep(actualResponse);

                check_com(m, 1, CanNmPnResetTime * 3);
                stop_send_nm_msg();

                sprintf(actualResponse, "开始仿真NM PDU：0x%X，PNI置位，PNC均为0，1s周期，检查DUT可以停止通信", test_id);
                TestStep(actualResponse);

                send_nm_msg(m, test_id, test_data_all_PNC_is0);
                check_com(m, 2);
                stop_send_nm_msg();

                MainPower_OFF();
                AppWait(5000, "");
            }
            else
            {
                if (bus_configuration[m].if_can_be_sleep_with_power != 1)
                    TestInfoPrintWarning("网段CAN%d，在供电状态下不能处于休眠，不满足测试条件", m + 1);
                else
                    TestInfoPrintWarning("网段CAN%d，不支持I-PDU控制，不满足测试条件", m + 1);
            }

        }
        post_test();
        return Test_Result;
    }

    catch (test_terminate&)
    {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}

s32 TC_Check_LIN_Schedule() {
    try {
        strcpy(TestCaseName, "TC_Check_LIN_Schedule");
        strcpy(TestScopeName, "NM");
        strcpy(testPurpose_CH, "测试目的：");

        TestPrecondition();
        ReportDescriptionFlag = 1;

        TestWait(5000);
        char TempNum[500];
        char PriRes[500];
        s32 LINChannelCount = -1;

        app.get_lin_channel_count(&LINChannelCount);
        if (1 > LINChannelCount) {
            TestReport_HTML_StepDescription("当前没有设置LIN通道", Warning);
            TestPostCondition();
            return 0;
        }
        FulfilTRC();

        UsgModSwtich(0xd);
        u8 DID_DD0A[2] = { 0xDD,0x0A };
        SID22_ReadDataByIdentifier(DID_DD0A);
        //CEM_LIN3、CEM_LIN4,CEM_LIN6,CEM_LIN7,ZCUD_LIN3,ZCUP_LIN1
        u8 DID_F13F[2] = { 0xF1,0x3F };
        //cc274=84
        SID22_ReadDataByIdentifier(DID_F13F);
        Diag_TX_Lin_V.clear();
        Diag_RX_Lin_V.clear();
        ZCUDChecker checker(Diag_Request_ID == 0x701 ? "ZCUD" : "ZCUP");
        checker.init();
        if (Diag_Request_ID == 0x701) {
            std::map<u8, std::string> custom_names = {
            {0, "CEM_LIN2"},
            {1, "CEM_LIN7"},
            {2, "CEM_LIN1"},
            {3, "CEM_LIN4"},
            {4, "ZCUD_LIN3"},
            {5, "ZCUD_LIN2"},
            {6, "CEM_LIN3"},
            {7, "CEM_LIN6"},
            {8, "ZCUD_LIN1"},
            };
            checker.set_channel_db_names(custom_names);
            
        }
        else {
            std::map<u8, std::string> custom_names = {
            {0, "CCM_LIN3"},
            {1, "CCM_LIN4"},
            {2, "CCM_LIN2"},
            {3, "ZCUP_LIN1"},
            {4, "CEM_LIN5"},
            {5, "CCM_LIN1"},
            {6, "CEM_LIN8"}
            };
            checker.set_channel_db_names(custom_names);
            //180报文
            com.can_rbs_activate_message_by_name(Diag_ch, true, "ZCU_CANFD2","ZCUP","ZCUDZCUCANFD2Fr15");
            com.can_rbs_set_message_cycle_by_name(Diag_ch, 40, "ZCU_CANFD2", "ZCUP", "ZCUDZCUCANFD2Fr15");
            com.can_rbs_set_signal_value_by_address("2/ZCU_CANFD2/ZCUP/ZCUDZCUCANFD2Fr15/OnRlyPwrDistbnCmd1WdPreBattSaveCmd",0);
            com.can_rbs_set_signal_value_by_address("2/ZCU_CANFD2/ZCUP/ZCUDZCUCANFD2Fr15/OnRlyPwrDistbnCmd1WdPreBattSaveCmd_UB", 1);
            //322报文
            com.can_rbs_activate_message_by_name(Diag_ch, true, "ZCU_CANFD2", "ZCUP", "DhuZCUCANFD2Fr21");
            com.can_rbs_set_message_cycle_by_name(Diag_ch, 150, "ZCU_CANFD2", "ZCUP", "DhuZCUCANFD2Fr21");
            com.can_rbs_set_signal_value_by_address("2/ZCU_CANFD2/ZCUP/DhuZCUCANFD2Fr21/RlyPwrDistbnCmd1WdBattSaveCmd", 1);
            com.can_rbs_set_signal_value_by_address("2/ZCU_CANFD2/ZCUP/DhuZCUCANFD2Fr21/RlyPwrDistbnCmd1WdBattSaveCmd_UB", 1);
            //2C0报文
            com.can_rbs_activate_message_by_name(Diag_ch, true, "ZCU_CANFD2", "ZCUP", "DhuZCUCANFD2Fr42");
            com.can_rbs_set_message_cycle_by_name(Diag_ch, 100, "ZCU_CANFD2", "ZCUP", "DhuZCUCANFD2Fr42");
            com.can_rbs_set_signal_value_by_address("2/ZCU_CANFD2/ZCUP/DhuZCUCANFD2Fr42/HmiFanLvlRow1", 14);
            com.can_rbs_set_signal_value_by_address("2/ZCU_CANFD2/ZCUP/DhuZCUCANFD2Fr42/HmiFanLvlRow1_UB", 1);  

            //CEM_LIN5
            UsgModSwtich(0xB);
            
        }
        //std::map<u8, std::string> custom_names = {
        //	{0, "主LIN总线"},
        //	{1, "副LIN总线"},
        //	{2, "车窗控制"},
        //	{3, "座椅控制"},
        //	{4, "空调控制"}
        //};
        //checker.set_channel_db_names(custom_names);
        IfListenLIN = 1;

        TestWait(5000);
        // 修改统计数据结构，按通道存储
        std::map<u8, std::map<MsgKey, int>> channel_zcud_msg_counts;    // 通道 -> (ID, DLC) -> 计数
        std::map<u8, std::map<MsgKey, int>> channel_non_zcud_msg_counts; // 通道 -> (ID, DLC) -> 计数

        // 统计RX报文
        for (int Temp_Size = 0; Temp_Size < 5; Temp_Size++)
        {
            TestWait(1000);
            sprintf(TempNum, "第%d次监测", Temp_Size + 1);
            TestReport_HTML_StepDescription(TempNum);

            for (size_t i = 0; i < Diag_RX_Lin_V.size(); ++i) {
                const TLIN& msg = Diag_RX_Lin_V[i];
                u8 channel = msg.FIdxChn;
                MsgKey key(msg.FIdentifier, msg.FDLC);
                if (checker.is_zcud_message(msg)) {
                    channel_zcud_msg_counts[channel][key]++;
                }
                else {
                    channel_non_zcud_msg_counts[channel][key]++;
                }
            }

            // 统计TX报文
            for (size_t i = 0; i < Diag_TX_Lin_V.size(); ++i) {
                const TLIN& msg = Diag_TX_Lin_V[i];
                u8 channel = msg.FIdxChn;
                MsgKey key(msg.FIdentifier, msg.FDLC);
                if (checker.is_zcud_message(msg)) {
                    channel_zcud_msg_counts[channel][key]++;
                }
                else {
                    channel_non_zcud_msg_counts[channel][key]++;
                }
            }

            // 按通道输出统计结果
            std::map<u8, std::string> channel_db_names = checker.get_channel_db_names();
            for (auto& channel_entry : channel_db_names) {
                u8 channel = channel_entry.first;
                std::string db_name = channel_entry.second;

                // 输出通道信息
                sprintf(PriRes, "通道 %d - 数据库: %s", channel, db_name.c_str());
                TestReport_HTML_StepDescription(PriRes, Info);

                // 输出该通道的ZCUD报文统计结果
                if (channel_zcud_msg_counts.find(channel) != channel_zcud_msg_counts.end()) {
                    for (auto& msg_entry : channel_zcud_msg_counts[channel]) {
                        u8 id = msg_entry.first.first;
                        u8 dlc = msg_entry.first.second;
                        int count = msg_entry.second;
                        sprintf(PriRes, "  Found %s message: ID=0x%02X, DLC=%d (Count: %d)",
                            checker.get_ecu_name(), id, dlc, count);
                        TestReport_HTML_StepDescription(PriRes, Pass);
                    }
                }

                // 输出该通道的非ZCUD报文统计结果
                if (channel_non_zcud_msg_counts.find(channel) != channel_non_zcud_msg_counts.end()) {
                    for (auto& msg_entry : channel_non_zcud_msg_counts[channel]) {
                        u8 id = msg_entry.first.first;
                        u8 dlc = msg_entry.first.second;
                        int count = msg_entry.second;
                        sprintf(PriRes, "  Not Found %s message: ID=0x%02X, DLC=%d (Count: %d)",
                            checker.get_ecu_name(), id, dlc, count);
                        TestReport_HTML_StepDescription(PriRes, Fail);
                    }
                }

                // 如果该通道没有任何报文，输出提示
                if (channel_zcud_msg_counts.find(channel) == channel_zcud_msg_counts.end() &&
                    channel_non_zcud_msg_counts.find(channel) == channel_non_zcud_msg_counts.end()) {
                    sprintf(PriRes, "  通道 %d 未检测到任何报文", channel);
                    TestReport_HTML_StepDescription(PriRes, Warning);
                }

                TestReport_HTML_StepDescription(""); // 空行分隔不同通道
            }
        }
        ReportDescriptionFlagLin = 0;
        IfListenLIN = 0;
        UsgModSwtich(1);
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

s32 TC_Check_CAN_Message() {
    try {
        strcpy(TestCaseName, "TC_Check_CAN_Message");
        strcpy(TestScopeName, "NM");
        strcpy(testPurpose_CH, "测试目的：验证所有通道节点TX报文ID及周期符合数据库定义");

        TestPrecondition();

        // 1. 获取网络拓扑
        CANNetworkTopology topo = GetCANNetworkTopology();

        // 2. 解析数据库期望值（与电压无关，只需一次）
        CANMessageExpectations exp = ParseDatabaseExpectations(topo);

        // 3. 电压循环：12V（正常）、8V（欠压）、18V（过压）
        double voltages[] = { 12.0, 18.0 };
        bool anyVoltageFailed = false;

        for (int vIdx = 0; vIdx < 2; vIdx++) {
            double volt = voltages[vIdx];
            if (volt == 8) volt = volt + 0.5;
            // 首次 12V 已在 TestPrecondition 中上电，无需重复设置
            if (vIdx > 0) {
                SetVoltage(volt, 3);
                AppWait(3000, "等待电压稳定");
            }

            char voltStr[64];
            if (volt == 8.5) volt = volt - 0.5;
            snprintf(voltStr, sizeof(voltStr), "========== 电压 %.1fV 测试 ==========", volt);
            TestReport_HTML_StepDescription(voltStr);

            // 3a. 收集总线报文
            CollectCANMessages(60000);

            // 3b. 逐通道检查
            bool voltageHasError = false;
            for (int ch : topo.channelsToTest) {
                const std::string& chName = topo.channelToName[ch];
                snprintf(StepDesc, sizeof(StepDesc), "------- [%.1fV] 当前测试通道CH%d：%s---------",
                         volt, ch + 1, chName.c_str());
                TestReport_HTML_StepDescription(StepDesc);

                // ID 一致性检查
                std::vector<u64> missingIds;
                bool idErr = CheckIDConsistency(ch, chName, exp,
                                                g_receivedIdsPerChannel[ch], missingIds);

                // 周期检查
                bool cycleErr = CheckCycleTimes(ch, chName, exp,
                                                g_receivedTimestampsPerChannel[ch]);

                if (!idErr && !cycleErr) {
                    log_ok("CH%d 所有检查通过", ch);
                    snprintf(StepDesc, sizeof(StepDesc), "[%.1fV] 通道 %s 所有报文周期和ID检查通过", volt, chName.c_str());
                    TestReport_HTML_StepDescription(StepDesc, Pass);
                } else {
                    voltageHasError = true;
                    log_nok("CH%d 部分检查失败", ch);
                    snprintf(StepDesc, sizeof(StepDesc), "[%.1fV] 通道 %s 部分检查失败", volt, chName.c_str());
                    TestReport_HTML_StepDescription(StepDesc, Fail);
                }
            }

            // 3c. 错误帧统计
            TestReport_HTML_StepDescription("-------错误帧统计---------");
            bool voltageHasErrorFrame = false;
            for (int ch : topo.channelsToTest) {
                auto errIt = g_errorFrameTimestampsPerChannel.find(ch);
                int errCount = (errIt != g_errorFrameTimestampsPerChannel.end()) ? (int)errIt->second.size() : 0;
                if (errCount == 0) {
                    snprintf(StepDesc, sizeof(StepDesc), "[%.1fV] 通道 %s: 未检测到错误帧", volt, topo.channelToName[ch].c_str());
                    TestReport_HTML_StepDescription(StepDesc, Pass);
                } else {
                    voltageHasErrorFrame = true;
                    double firstErr = errIt->second.front();
                    double lastErr = errIt->second.back();
                    snprintf(StepDesc, sizeof(StepDesc), "[%.1fV] 通道 %s: 检测到 %d 个错误帧 (首次 %.1fms, 末次 %.1fms)",
                             volt, topo.channelToName[ch].c_str(), errCount, firstErr, lastErr);
                    TestReport_HTML_StepDescription(StepDesc, Fail);
                    log_nok("[%.1fV][%s] 错误帧 %d 个", volt, topo.channelToName[ch].c_str(), errCount);
                }
            }
            if (!voltageHasErrorFrame) {
                snprintf(StepDesc, sizeof(StepDesc), "[%.1fV] 所有通道均未检测到错误帧", volt);
                TestReport_HTML_StepDescription(StepDesc, Pass);
            }

            if (voltageHasError || voltageHasErrorFrame) {
                anyVoltageFailed = true;
            }
        }

        // 恢复 12V
        SetVoltage(12.0, 3);

        if (anyVoltageFailed) {
            Test_Result = TestNOK;
        }

        TestPostCondition();
        return Test_Result;
    }
    catch (test_terminate&) {
        TestInfoPrintWarning("测试异常中断，退出测试");
        TestTerminate();
        return TestWarning;
    }
}



#endif // !TestCase_CAN_NM