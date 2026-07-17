// 示例测试用例 — 从 GEEA3.0_SWDL/TestCase_CAN_SWDL.h 的 test_case_1021808 提炼
// 这是脱离源工程后写新用例的范本。真实用例查 templates/miniprogram-skeleton/ 的 TestCase_CAN_*.h。
#ifndef SAMPLE_TEST_CASE_H
#define SAMPLE_TEST_CASE_H
#include "test_case_diag_api.h"

////////////////////////////////////////////////////////////////////////////////////////
// TC_1021808: DiagnosticSessionControl (0x10) - Physical Addressing
// 测试目的：验证物理寻址下 0x10 会话切换的肯定/否定/无响应行为
////////////////////////////////////////////////////////////////////////////////////////
s32 test_case_1021808()
{
    // —— 1. 元信息 ——
    strcpy(TestCaseName,   "TC_1021808_DiagnosticSessionControl (0x10) - Physical Addressing");
    strcpy(TestScopeName,  "Software Download");
    strcpy(testPurpose_CH, "测试目的：验证 0x10 会话切换在物理寻址下的行为");

    // —— 2. 前置条件 ——
    TestPrecondition();
    TestSeq_EnterProgrammingSession();          // 进编程会话（含安全访问）
    TestReport_HTML_StepDescription("测试前提:下载SBL，并不激活");
    TestSeq_DownloadSBL(0);                      // 下载 SBL 不激活

    // —— 3. 测试步骤 ——
    TestReport_HTML_StepDescription("正式运行测试步骤");
    StepNumber = 0;

    // DefaultSession 切换
    CheckCurrentSession(0x02);                   // 当前应是编程会话
    SID10_SessionControl(0x01);                  // 请求 DefaultSession
    CheckPosResponse_SID10(0x01);                // 期望肯定响应
    CheckCurrentSession(0x01);                   // 校验已切到 Default
    TestWait(ECUResetTime);

    // 抑制响应位 (bit7=1) → 无响应
    SID10_SessionControl(0x82);
    CheckNonResponse();
    TestWait(ECUResetTime);

    // 切回 ProgrammingSession
    SID10_SessionControl(0x02);
    CheckPosResponse_SID10(0x02);
    CheckCurrentSession(0x02);

    TestSeq_DownloadSBL(0);

    // 不支持的子功能 → 否定响应 NRC 0x12 0x7E
    SID10_SessionControl(0x03);
    u8 NRC[2] = { 0x12, 0x7E };
    CheckNegResponse(NRC);
    SID10_SessionControl(0x83);
    CheckNegResponse(NRC);

    // 激活 SBL 后再测
    ActivateSBL();
    SID10_SessionControl(0x03);
    CheckNegResponse(0x12);
    SID10_SessionControl(0x83);
    CheckNegResponse(0x12);

    // 抑制响应位 + Default
    SID10_SessionControl(0x81);
    CheckNonResponse();
    TestWait(500);
    CheckCurrentSession(0x01);

    SID10_SessionControl(0x82);
    CheckNonResponse();

    // 最终切回 Programming
    SID10_SessionControl(0x02);
    CheckPosResponse_SID10(0x02);
    CheckCurrentSession(0x02);
    TestSeq_DownloadSBL();
    SID10_SessionControl(0x82);
    CheckNonResponse();
    CheckCurrentSession(0x02);

    // —— 4. 后置条件 + 判定 ——
    TestPostCondition();

    if (resultFlag == 0)
        return 1;       // pass
    else
        return 0;       // fail
}

#endif // SAMPLE_TEST_CASE_H
