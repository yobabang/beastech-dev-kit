# MiniProgram SDK 骨架

TSMaster MiniProgram = 一个 x86 dll，拖进 TSMaster 作为可调用库。所有 GEEA 测试工程共享此骨架。

## 标准文件集

| 文件 | 作用 |
|------|------|
| `<工程>.cpp` | 主源码，含 `on_start_*`/`on_stop_*` 入口事件 |
| `<工程>Extern.cpp` | 导出函数实现（大块业务逻辑通常在这） |
| `<工程>.sln` / `.vcxproj` / `.vcxproj.user` | VS 工程（**x86** 配置） |
| `mp.def` | 导出表，固定 4 项：`initialize_miniprogram` / `finalize_miniprogram` / `retrieve_mp_abilities` / `write_api_document_auto` |
| `TSMaster.h` | TSMaster 类型定义（大文件，376KB） |
| `TSMasterBaseInclude.h` / `TSMasterBaseSource.cpp` | SDK 基础 |
| `TSMasterCustomAPIDoc.cpp` | 自定义 API 文档桩 |
| `MPLibrary.h` | ★ TSMaster C API 函数指针表（诊断/报告/总线/系统变量） |
| `Configuration.h` | 配置常量（配合 TSMaster 常量管理器"Auto Generate Const in C Code"自动生成） |
| `Database.h` / `.cpp` | CAN/LIN/FlexRay 数据库容器（壳，运行时由 TSMaster 注入） |
| `PDXParser.h` | PDX 诊断数据库解析（DID/RID/DTC 服务定义） |
| `SBLParser.h` | SBL 软件块解析（SWDL 用） |
| `GenerateKeyExOpt.h` | 安全访问密钥生成（0x27 seed→key） |
| `test_case_diag_api.h` | ★ UDS 服务 + SWDL + 安全访问封装层（写用例的主 API） |
| `test_common.h` | 测试公共定义、全局变量（TestCaseName 等） |
| `test_tp.h` | 传输层（TP / 多帧） |
| `test_devices.h` / `test_devices_3.0.h` | 设备控制（电源、继电器等） |
| `tsdisturb.h` | 总线扰动 / 故障注入（E2E 高负载测试用） |
| `TestReport.h` | HTML 报告生成 |
| `cJSON.c` / `.h` | JSON 解析（读 CS.json/NM.json 用例清单） |
| `test_report_new.dll` | 报告运行时依赖（保留，不可删） |
| `TestCase_CAN_*.h` | 按功能域分文件的测试用例集（业务，模板里留范本） |

## 入口与导出

```c
// <工程>.cpp
void on_start_NewOn_Start1(void) { try { /* 程序启动事件：初始化、加载库 */ } ... }
void on_stop_NewOn_Stop1(void)  { try { /* 程序停止事件：清理 */ } ... }
```

`mp.def` 固定导出 4 个 TSMaster 约定函数，dll 才能被 TSMaster 识别为 MiniProgram。

## 编译与部署（readme.txt 标准流程）

1. VS 打开 `.sln`，选 **x86** 配置编译 → 产出 dll。
2. 把 dll 拖进 TSMaster → 安装为 MiniProgram 库。
3. 在 MiniProgram 库管理器里运行，或被其他 MiniProgram 调用。
4. 调试时把输出目录指向 TSMaster 工程的 `bin/`。

## 新建工程（脱离源工程）

从 `templates/miniprogram-skeleton/` 整体复制 → 改工程名（需同步改：`.sln`/`.vcxproj` 文件名与内部工程名、`mp.def` 的 `LIBRARY` 行、`.cpp`/`Extern.cpp` 文件名）→ 在 `TestCase_CAN_*.h` 或新建头文件里写用例 → 编译。

> 模板里 `LIBRARY CCode2133` 是占位名，复制后改成自己的库标识。

## 功能域用例头文件约定

| 头文件 | 功能域 |
|--------|--------|
| TestCase_CAN_SWDL.h | 软件下载 |
| TestCase_CAN_DiagnosticServer.h | 诊断服务 (UDS) |
| TestCase_CAN_Communication.h / TestCase_CANFD_Communication.h | CAN / CANFD 通信 |
| TestCase_CAN_DTC.h | 故障码 |
| TestCase_CAN_Upper_Lower_Link.h | 上下行链路 |
| TestCase_LIN_Communication.h | LIN 通信 |
