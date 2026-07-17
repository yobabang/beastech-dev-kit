# MiniProgram 骨架模板（基于 SmokeTest）

本模板源自 `GEEA3.5SmokeTest/EEA4.0-SmokeTest-26R1-U1`，是 beastech 工程家族里**最成熟的工程模板**。清理了构建产物（.vs/output/x64/Debug/Release）和个人配置，保留完整可编译源码。

> 这是全栈模板：全局执行器（Pretest/TestFinish）、12 域用例头、四态结果、CAN/CANFD/LIN/Ethernet 全回调、自动 Python 绑定。比旧版 DIAG_BENCH 骨架更完整。旧版备份在 `templates/miniprogram-skeleton-diag-bench-legacy/`。

## 工程架构

详见 [smoketest-framework.md](../../references/smoketest-framework.md)。要点：

- **`GEEA_CAN_DIAG.cpp`** — 事件回调入口（CAN/CANFD/LIN/Ethernet RX/TX/PreTx、timer、start/stop）
- **`GEEA_CAN_DIAGExtern.cpp`** — extern 声明 + `retrieve_mp_abilities` 能力注册（回调/仪器/结构体校验）
- **`test_case_diag_api.h`** — 诊断 API + `Pretest()`/`TestFinish()`/`Pretestcase()` 全局执行器
- **12 个 `TestCase_CAN_*.h`** — 按功能域分用例（DiagnosticServer/NM/E2E/NVM/SWDL/SecurityUnlock/CS/Communication/CANFD/LIN/CheckVision/UpperLowerLink）
- **`test_function.h`** — 测试工具（信号操作、E2E、会话、DTC）
- **`TestReport.h`** — HTML 报告（四态：OK/NOK/Warning/NA）
- **`output/GEEA_CAN_DIAG.py`** — 自动生成的 Python ctypes 绑定（编译后产出）

## 用例范式（SmokeTest 版，注意返回值约定）

```cpp
s32 test_case_XXXX() {
    strcpy(TestCaseName, "TC_XXXX_Description");
    strcpy(TestScopeName, "Scope Group");
    strcpy(testPurpose_CH, "测试目的：...");
    TestPrecondition();              // 用例级前置
    // ... 测试步骤 ...
    CheckPosResponse_SID10(0x01);
    TestFinish();                    // 用例级后置
    return 0;                        // ⚠️ 0=成功（与 DIAG_BENCH 的 1=pass 相反）
}
```

## 全局执行器模式

```cpp
// 在主执行器里（非用例内）
Pretest();                  // 一次：初始化报告、配置总线
test_case_xxx();            // 用例序列
TestFinish();               // 一次：收尾报告
```

## 新建工程步骤

1. 复制本目录到目标位置。
2. 改工程名（同步 `.sln`/`.vcxproj` 文件名与内部工程名、`.cpp`/`Extern.cpp` 文件名）。
3. **保留 12 域头文件作范本**，按需修改；新域新建头文件并 `#include "test_case_diag_api.h"`。
4. 在 `GEEA_CAN_DIAGExtern.cpp` 注册新用例的 extern 声明。
5. 用例登记到 JSON 清单（CS.json/NM.json），`TestFuncName` 与 C 函数名一致。
6. VS2022 + v143 + C++17 + Win32 编译 → `output/*.mp`。
7. dll 拖进 TSMaster 或 Mini Program Library Manager。

## 构建（来自 CLAUDE.md）

```
VS2022 (v143), C++17, Windows
Debug Win32:  msbuild GEEA_CAN_DIAG.vcxproj /p:Configuration=Debug;Platform=Win32
Release x64:  msbuild GEEA_CAN_DIAG.vcxproj /p:Configuration=Release;Platform=x64
输出: ./output/*.mp (Win32) 或 *.mp64 (x64)
清理: msbuild /t:Clean
调试: 输出目录指向 TSMaster bin/
```

## 保留的资产（不要删）

- `TSMaster.h` / `TSMaster_2024.h` — SDK 类型定义（两个版本，按 TSMaster 版本选）
- `MPLibrary.h` — C API 函数指针表
- `test_report_new.dll` — 报告运行时依赖（如在旧版备份里，从那里取）
- `PDXParser.h` / `SBLParser.h` / `GenerateKeyExOpt.h` — 解析器/密钥
- `cJSON.c/.h` — JSON 解析

## 与旧版 DIAG_BENCH 骨架的差异

| 维度 | SmokeTest 模板（本） | DIAG_BENCH 骨架（legacy） |
|------|---------------------|--------------------------|
| 执行器 | 全局 Pretest/TestFinish | 用例内 TestPrecondition/TestPostCondition |
| 结果态 | 四态（OK/NOK/Warning/NA） | 两态（pass/fail） |
| 用例返回 | 0=成功 | 1=pass/0=fail |
| 回调 | CAN/CANFD/LIN/Ethernet/TCP/UDP/Timer | CAN/CANFD |
| 域头文件 | 12 个 | 6 个 |
| 自描述 | CLAUDE.md | 无 |
| Python 绑定 | 自动 output/*.py | 无 |
| 配置 | INI 运行时读 | 硬编码 |

## 注意

- 用例返回值约定与 DIAG_BENCH **相反**：SmokeTest `0=成功`，DIAG_BENCH `1=pass`。写用例前确认目标工程约定。
- `Configuration.h` 由 TSMaster 常量管理器自动生成，首次需在 TSMaster 里配置常量后重新生成。
- `test_case_diag_api.h`(488KB) 含 Pretest/TestFinish 实现，不要重复定义。
