# VT/CAPL 工程范式（Vector vTESTstudio + CANoe）

源自 `D:/VTProject/` 下 8 个明文可读工程。VT 工程用 CAPL 语言（Vector 专有，类 C），与 beastech 的 TSMaster C/C++ 工程是两套技术路线，但**协议事实与库分层方法互通**。本文件记 VT 的工程范式与 CAPL 库结构，供 beastech 工程化借鉴。

> 加密工程（5 个）核心逻辑不可读，见 [vt-encrypted-summary.md](vt-encrypted-summary.md)。明文库只提炼通用知识，不抄项目数据（DID/NRC 值/报文 ID）。

## CAPL 基础

- `.can` = 测试用例文件（含 `testcase` 关键字）
- `.cin` = 头库/实现文件（被 `.can` 通过 `#include` 引入）
- `.canencr` / `.cinencr` = 加密交付副本（与明文成对，忽略）
- `.cbf` = CANoe 编译二进制
- `.tse` / `.sttse` = vTESTstudio 测试模块序列（可视化编排）
- `.vsysvar` = 系统变量定义
- 文件多为 GBK 编码（`/*@!Encoding:936*/`）

### CAPL 用例结构
```capl
/*@!Encoding:936*/
includes {
  #include "..\02_Library\TestContext.cin"
}
variables {
  dword ECUAddress;
}

testcase Service_0x10_Valid_Physical_All_Sessions_Supported_Service() {
  // 测试步骤
  SID10_DiagnosticSessionControl(0x01, Physical, SPRMIB_FALSE);
  CheckPosResponse(0x10, ...);
}

void MainTest() {
  Service_0x10_Valid_Physical_All_Sessions_Supported_Service();
}
```

## VT 工程标准化目录

```
<工程>/
├── 01_Input/            # 输入：Arxml Parser.exe / Basetech配置工具.exe / 数据库
├── 02_Library/          # ★ 库（分层复用）
│   ├── 00_StandardLibrary/
│   │   ├── Common/           # Encryption/LogAndReport/ReportParsing
│   │   ├── DeviceControl/    # 硬件抽象（BOB/DRB/VH6501/示波器/电源）
│   │   ├── Diagnostic/       # 诊断库（分 01_TP~08_DB 子层）
│   │   └── SignalSimulation/ # 信号仿真
│   ├── 01_SimNode/
│   ├── 02_DLL/               # GenerateKeyExOpt/X509Api dll
│   └── 03_EXE/
├── 03_Script/           # 测试用例 TestCase.can + Parameters.cin
├── 04_Node/
├── 05_PanelAndSysvar/   # 面板与系统变量 .vsysvar
├── 06_LogAndReport/
└── 07_UserManual/
```

**借鉴点**：beastech 工程文件平铺，建议新工程采用编号目录；`Diagnostic` 库按 `01_TP/02_UDS/03_FileHandle/04_DLL/05_Tse/06_SimNode/07_Sysvar/08_DB` 分层，比 beastech 的单体 `test_case_diag_api.h`(474KB) 更清晰。

## Diagnostic 库分层（02_Library/00_StandardLibrary/Diagnostic/）

| 子目录 | 内容 |
|--------|------|
| 01_TP | 传输层：COM_TP.cin / CAN_TP.can / IP_TP.can / LIN_TP.can（按总线分） |
| 02_UDS | UDS_Service_1.5.cin(208KB) + UDS_Functions_1.0.cin + 0x29Function.cin |
| 03_FileHandle | FileHandle.cin（文件传输/SWDL） |
| 04_DLL | 密钥/证书 dll |
| 05_Tse | 测试序列编辑器 |
| 06_SimNode | Simulate_CAN/CANFD/TCAM.can（节点仿真） |
| 07_Sysvar | 系统变量 |
| 08_DB | 数据库 |
| DiagConfig.cin | 诊断配置（枚举/常量/状态） |
| DiagReqDB.cin | 诊断请求库 |

## UDS 服务封装（UDS_Service_1.5.cin）

命名 `SID<XX>_<Action>`，参数规范含 `enum AddressMode` / `enum e_SPRMIB`（响应抑制位）。

| SID | 封装签名 |
|-----|----------|
| 0x10 | `SID10_DiagnosticSessionControl(subfunc, Mode, SPRMIB)` |
| 0x11 | `SID11_ECUReset(subfunc, Mode, SPRMIB)` |
| 0x14 | `SID14_ClearDTC(DTC[], Mode)` |
| 0x19 | `SID19_Sub01/02/04/06/0A/55(...)` 多子功能 |
| 0x22 | `SID22_ReadDataByIdentifier(PID[]/long, Mode)` |
| 0x27 | `SID27_SecurityAccess_RequestSeed(level, Mode)` / `SendKey(level, KeyArray[], Mode)` |
| 0x28 | `SID28_CommunicationControl(subfunc, CommType/NodeID[], Mode, SPRMIB)` |
| 0x29 | `SID29_Authentication_verifyCertificateUnidirectional/proofOfOwnership/authenticationConfiguration/deAuthenticate` |
| 0x2A | `SID2A_ReadDataByPeriodicIdentifier(TMode, PID[], Mode)` |
| 0x2C | `SID2C_DynamicallyDefinedDataIdentifier_Identifier(...)` |
| 0x2E | `SID2E_WriteDataByIdentifier(PID[], RawData[], Mode)` |
| 0x2F | `SID2F_IOControl(PID[], IOParameter/ControlOptionRecord, Mode)` |
| 0x31 | `SID31_RoutineControl(RoutineType, RoutineID[], OptionRecord[], Mode, SPRMIB)` / `_WithResponse` |
| 0x34 | `SID34_RequestDownload(dataFormat, addrLenFormat, addr[], size[], Mode)` |
| 0x35 | `SID35_RequestUpload(...)` |
| 0x36 | `SID36_TransferData(Data[], length, Counter, Mode)` |
| 0x37 | `SID37_RequestTransferExit(Mode)` |
| 0x3D | `SID3D_WriteMemoryByAdress_14(...)` |
| 0x85 | `SID85_ControlDTCSetting(subfunc, DTC[], Mode)` |

### 响应校验（UDS_Functions / UDS_Service）
- `CheckPosResponse(SID, ExpectLength, reqGroup, reqNum)`
- `CheckNegResponse(expectNRC, ...)` / `CheckNegResponse_WithoutResponse(...)`
- `CheckPosOrNegResponse(expectNRC, ...)`
- `CheckNonResponse(...)` / `CheckNonResponse_WithoutResponse(...)`
- `CheckNotNegResponse(exceptNRC, ...)`
- `CheckCurrentSession(expectSession, Mode, ...)`
- `GetReceivedData(Data[]/Data[], StartPos, length)` / `GetRxLength()` / `CheckRxLength(length)`
- `NRC_Info(nrc)` — NRC 信息打印

### 枚举（DiagConfig.cin）
- `enum AddressMode { Physical, Functional }`
- `enum DiagnosticState { Diag_APP=1, Diag_Boot=2 }`
- `enum DiagnosticSwitch { Diag_On=0, Diag_Off=1 }`
- `enum EthNodeType { EdgeNode=1, InternalNode=2 }` — EdgeNode=TCP server, InternalNode=TCP client
- `enum e_SPRMIB { SPRMIB_FALSE, SPRMIB_TRUE }`
- `enum e_CommunicationType` / `enum e_RoutineControlType` / `enum e_TransmissionMode` / `enum e_InputOutputControlParameter`

## 设备控制库（DeviceControl）

VT 的硬件抽象比 beastech 完善，覆盖：
- **BOB**（Break-Out-Box）/ **BOBs** — 信号通断控制
- **DRB** — 继电器箱
- **PSRB** — 可编程固态继电器
- **VH1150 / VH6501** — Vector 总线干扰仪（故障注入）
- **VT** — Vector VT 板卡
- **PCS_DSOX3034A** — 安捷伦示波器
- **PMM_FLUKE8846A** — 万用表
- **PPS_SPPS_N6700C** — 可编程电源
- **IPClient** — 以太网客户端
- **ENV** — 环境模拟

详见 [device-control.md](device-control.md)。

## 与 beastech 工程对照

| 维度 | VT/CAPL | beastech/TSMaster |
|------|---------|-------------------|
| 用例单元 | `testcase` | `s32 test_case_<ID>()` |
| 库文件 | `.cin`（分 TP/UDS/FileHandle） | `test_case_diag_api.h`（单体） |
| UDS 封装 | `SIDxx_(subfunc, Mode, SPRMIB)` | `SIDxx_<Action>(subfunc)` |
| 响应校验 | `CheckPosResponse/CheckNegResponse` | `CheckPosResponse_SIDxx/CheckNegResponse` |
| 编排 | `.tse` 可视化 + JSON | JSON 清单 + Python runner |
| 执行 | CAPL 解释（CANoe 虚拟机） | C/C++ 原生 dll |
| AI 集成 | 无 MCP | 有 TSMaster MCP |
| 硬件 | BOB/DRB/VH6501/示波器/电源 | 电源+继电器为主 |

**核心认知**：VT 的 UDS/TP 封装函数签名更规范（带 AddressMode/Sprmib 枚举），beastech 的封装参数更简略。两者协议事实完全一致，可在写 beastech 用例时参考 VT 的参数组织方式。
