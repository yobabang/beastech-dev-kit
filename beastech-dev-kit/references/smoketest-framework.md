# SmokeTest 冒烟测试工程范式

源自 `d:/claudeProject/GEEA4.0/GEEA3.5SmokeTest/` 三个变体。这是 beastech 工程家族里**最成熟的工程模板**——有完整 CLAUDE.md 自描述、系统化执行器、12 域用例、自动 Python 绑定。比 DIAG_BENCH(单用例)和 NM_TEST(数据驱动)更全面,是"全栈测试工程"的范本。

## 三变体

| 变体 | 用途 | 差异 |
|------|------|------|
| EEA4.0-SmokeTest-26R1-U1 | 26R1-U1 版本 ECU | 基线,含 GEEA_CAN_DIAG 目录 |
| EEA4.0-SmokeTest-ZCUP-25R3U3 | ZCUP 25R3U3 版本 | 多 PowerControl.h(电源控制),无 GEEA_CAN_DIAG 目录 |
| EEA4.0-SmokeTest-master | 主线 | 与 26R1-U1 基本一致 |

三变体 TestCase 头文件**完全一致**(12 域),差异只在配置文件和少量设备控制头。变体管理靠复制工程 + 改配置,非条件编译。

## 工程架构（CLAUDE.md 自述）

### 入口
- **`GEEA_CAN_DIAG.cpp`** — TSMaster 事件回调:CAN/CANFD/LIN/Ethernet 的 RX/TX/PreTx、定时器、start/stop。include 并实例化所有测试模块
- **`GEEA_CAN_DIAGExtern.cpp`** — 测试用例 extern 声明、`output<T>` 模板特化、logger 函数、**主测试执行器**(调 `Pretest()` → 测试用例 → `TestFinish()`)

### 事件回调全集（比 NM_TEST/DIAG_BENCH 更全）
```cpp
void on_canfd_rx_NewOn_CAN_FD_Rx1(const TCANFD* ACANFD);    // CANFD 收
void on_canfd_tx_NewOn_CAN_FD_Tx1(const TCANFD* ACANFD);    // CANFD 发完成
void on_canfd_pretx_NewOn_CAN_FD_PreTx1(const PCANFD ACANFD); // CANFD 预发送
void on_lin_rx_NewOn_LIN_Rx1(const TLIN* ALIN);             // LIN 收
void on_lin_tx_NewOn_LIN_Tx1(const TLIN* ALIN);             // LIN 发完成
void on_lin_pretx_NewOn_LIN_PreTx1(const PLIN ALIN);        // LIN 预发送
void on_start_NewOn_Start1(void);                            // 启动
void on_stop_NewOn_Stop1(void);                              // 停止
```

## 测试执行器模式（关键范式）

与 DIAG_BENCH 的"用例内含 TestPrecondition/TestPostCondition"不同,SmokeTest 用**全局执行器**:

```cpp
// test_case_diag_api.h
void Pretest();       // 全局前置:初始化报告、配置总线、加载参数
void TestFinish();    // 全局后置:收尾报告
s32  Pretestcase();   // 单用例前置

// Extern.cpp 主执行器
Pretest();                        // 一次
test_case_1350868();              // 用例1
test_case_1350870();              // 用例2
// ...
TestFinish();                     // 一次
```

**优势**:报告初始化/收尾只做一次,用例间共享上下文;用例本身更纯粹(只含测试步骤)。

## Test_Result 四态（比 pass/fail 更细）

```cpp
enum TestResult {
  TestOK = 0,      // 通过
  TestNOK = 1,     // 不通过
  TestWarning = 2, // 警告
  TestNA = 3,      // 不适用
  Pass, Fail, Info, Warning
};
```

报告函数:`TestReport_HTML_Step()` / `TestInfoPrintPass()` / `TestInfoPrintFail()` / `TestInfoPrintWarning()` / `TestInfoPrintNA()`。

## 12 个测试域（TestCase_CAN_*.h）

| 头文件 | 域 |
|--------|-----|
| TestCase_CAN_DiagnosticServer.h | UDS 诊断服务 (0x10/0x11/0x22/0x27/0x28/0x2E/0x3E) |
| TestCase_CAN_NM.h | 网络管理 (PNC 唤醒/睡眠、帧 ID、时序) |
| TestCase_CAN_E2E.h | 端到端保护 (CRC/counter/DTC 校验) |
| TestCase_CAN_NVM.h | NVM 读写(各种电源/复位条件) |
| TestCase_CAN_SWDL.h | 软件下载 |
| TestCase_CAN_SecurityUnlock.h | 安全访问 (seed/key/错误密钥/签名校验) |
| TestCase_CAN_CS.h | 校验和 |
| TestCase_CAN_Communication.h | CAN/CANFD 通信 |
| TestCase_CANFD_Communication.h | CANFD 专用通信 |
| TestCase_LIN_Communication.h | LIN 通信 |
| TestCase_CAN_CheckVision.h | 软件版本检查 |
| TestCase_CAN_Upper_Lower_Link.h | 上下行链路 |

## 共享基础设施

| 文件 | 大小 | 作用 |
|------|------|------|
| `test_function.h` | ~233K | 测试工具:信号操作、E2E CRC/counter、会话/例程、DTC |
| `test_case_diag_api.h` | ~450K | 诊断 API:SID 请求/响应封装、TP 层、**含 Pretest/TestFinish/Pretestcase** |
| `test_common.h` | — | 类型定义、诊断 ID、总线配置、CAN/LIN 报文向量 |
| `test_parameter.h` | — | NM 时序参数、DID 定义、总线拓扑 |
| `test_devices.h`/`test_devices_3.0.h` | — | 硬件设备控制 |
| `test_tp.h` | — | 传输协议 |
| `TestReport.h` | — | HTML 报告系统 |
| `FunctionDeclaration.h` | — | 所有模块的中央函数声明 |

## 配置驱动

- **`Configuration.h`** — TSMaster 常量管理器自动生成
- **INI 文件** — 运行时读总线拓扑、DUT 参数(通过 `GetParameter()`),与代码分离
- **`output/GEEA_CAN_DIAG.py`** — 自动生成的 Python ctypes 绑定,外部可调每个 test_case

## 构建与部署（CLAUDE.md）

```
VS2022 (v143), C++17, Windows
Build: msbuild GEEA_CAN_DIAG.vcxproj /p:Configuration=Debug;Platform=Win32
输出: ./output/*.mp (Win32) 或 *.mp64 (x64)
清理: msbuild /t:Clean
部署: dll 拖进 TSMaster 或 Mini Program Library Manager
调试: 输出目录指向 TSMaster bin/
```

## 用例范式（SmokeTest 版）

```cpp
s32 test_case_1350868() {
    strcpy(TestCaseName, "TC_1350868_Description");
    strcpy(TestScopeName, "Diagnostic Server");
    strcpy(testPurpose_CH, "测试目的：...");
    TestPrecondition();              // 用例级前置
    // ... 测试步骤,用 SID 封装 ...
    CheckPosResponse_SID10(0x01);
    TestFinish();                    // 用例级后置
    return 0;                        // 0=成功
}
```

注意:SmokeTest 用例返回 `0=成功`(与 DIAG_BENCH 的 `1=pass` 相反!)——写用例前先确认目标工程的返回约定。

## 与其他工程对照

| 维度 | SmokeTest | DIAG_BENCH | NM_TEST |
|------|-----------|-----------|---------|
| 执行器 | 全局 Pretest/TestFinish | 用例内 TestPrecondition/TestPostCondition | pre_test/post_test |
| 用例返回 | 0=成功 | 1=pass/0=fail | current_test_result |
| 结果态 | 四态(OK/NOK/Warning/NA) | 两态(pass/fail) | current_test_result |
| 域覆盖 | 12 域(最全) | 6 域 | NM 专精 |
| 回调 | CAN/CANFD/LIN/Eth 全 | CAN/CANFD | CANFD |
| 配置 | INI + Configuration.h | 硬编码 | config.ini 数据驱动 |
| 自描述 | 有 CLAUDE.md | 无 | 无 |
| Python 绑定 | 自动生成 output/*.py | 无 | 手写 NM_TEST.py |

## 借鉴点（对 beastech 新工程）

1. **全局执行器**优于用例内前置后置——报告只初始化一次
2. **Test_Result 四态**比 pass/fail 表达力强(NA 用于不适用场景)
3. **CLAUDE.md 自描述**——每个工程都该有,说明构建/架构/范式
4. **自动 Python 绑定**——output/*.py 让 CI/CD 可调
5. **12 域头文件**是完整测试域划分的参考
6. **INI 配置驱动**——DUT 参数运行时读,不硬编码
7. **变体管理**——复制工程 + 改配置,而非条件编译

> SmokeTest 是 beastech 新建工程的首选模板。比 `templates/miniprogram-skeleton/`(基于 DIAG_BENCH)更完整。建议后续把 SmokeTest 作为模板基础。
