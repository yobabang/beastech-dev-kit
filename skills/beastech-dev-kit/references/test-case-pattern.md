# 测试用例范式

测试用例是导出的 C 函数，命名 `test_case_<TC_ID>`，返回 `s32`：**1 = pass，0 = fail**。

## 标准结构

```c
s32 test_case_1021808()
{
    // —— 元信息 ——
    strcpy(TestCaseName,    "TC_1021808_DiagnosticSessionControl (0x10) - Physical Addressing");
    strcpy(TestScopeName,   "Software Download");
    strcpy(testPurpose_CH,  "测试目的：验证 0x10 会话切换在物理寻址下的行为");

    TestPrecondition();                  // 前置条件

    // —— 测试步骤 ——
    TestSeq_EnterProgrammingSession();
    TestReport_HTML_StepDescription("正式运行测试步骤");
    StepNumber = 0;

    SID10_SessionControl(0x01);          // 发请求
    CheckPosResponse_SID10(0x01);        // 校验肯定响应
    CheckCurrentSession(0x01);           // 校验状态
    TestWait(ECUResetTime);              // 等待

    u8 NRC[2] = { 0x12, 0x7E };
    SID10_SessionControl(0x83);
    CheckNegResponse(NRC);               // 校验否定响应

    SID10_SessionControl(0x81);
    CheckNonResponse();                  // 校验无响应

    TestPostCondition();                 // 后置条件

    if (resultFlag == 0) return 1;       // pass
    else return 0;                       // fail
}
```

## 关键全局变量（test_common.h / test_case_diag_api.h）

| 变量 | 用途 |
|------|------|
| `TestCaseName` | 用例名（"TC_<ID>_<描述>"） |
| `TestScopeName` | 功能域（如 "Software Download"） |
| `testPurpose_CH` | 测试目的（中文） |
| `StepNumber` | 步骤号，每步递增 |
| `resultFlag` | 失败标志，0=无失败(pass)，≠0=有失败(fail) |
| `ECUResetTime` | 复位等待时长（项目配置） |
| `TpRxData[]` / `TpRxLength` | 接收缓存 |

## pass/fail 判定逻辑

- `resultFlag` 初始为 0。
- 任何 `Check*Response_*` 不符预期 → 置 `resultFlag` ≠ 0（通常 +1 累加失败次数）。
- 函数末尾 `resultFlag == 0 ? return 1 : return 0`。
- **不要**在中间 return——让用例跑完所有步骤，累积全部失败点，便于一次看全问题。

## 编写新用例的步骤

1. **查 TC_ID 与需求**：从需求文档（`需求.txt`，注意 GBK）或 JIRA 取 TC_ID 和测试目的。
2. **归入功能域头文件**：SWDL→`TestCase_CAN_SWDL.h`，诊断→`TestCase_CAN_DiagnosticServer.h`，等。新建域则新建头文件并 `#include "test_case_diag_api.h"`。
3. **写函数**：套上面的标准结构，复用 `SIDxx_*` / `Check*Response_*` / `TestSeq_*` 封装，**不要另造诊断调用**。
4. **登记到清单**：在对应 JSON（CS.json/NM.json）的 `TestGroups.MM[]` 加一条：
   ```json
   {
     "TestName": "TC_<ID>_<描述>",
     "TestFuncName": "test_case_<ID>",
     "TestArgs": "()",
     "IsChecked": 1,
     "Comment": ""
   }
   ```
   注意 `TestFuncName` 必须与 C 函数名完全一致。
5. **编译部署**：x86 编译 → dll 拖进 TSMaster。

## 用例命名约定

- C 函数：`test_case_<TC_ID>`（如 `test_case_1021808`）
- TestCaseName：`TC_<TC_ID>_<简短英文描述>`（如 `TC_1021808_DiagnosticSessionControl (0x10) - Physical Addressing`）
- NM 用例：`TC_<ID>_CAN_NM_<场景>`（如 `TC_1023208_CAN_NM_Frame_ID_in_Range_wakeup_and_down`）

## 期望响应三态必须明确

每条诊断请求都要有明确期望，三选一：
- 肯定响应 → `CheckPosResponse_SID<XX>(期望参数)`
- 否定响应 → `CheckNegResponse(NRC数组)`
- 无响应 → `CheckNonResponse()`

不确定期望就别写——查需求文档或协议规范，不要猜。
