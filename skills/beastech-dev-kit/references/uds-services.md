# UDS 诊断服务

GEEA 工程使用的 UDS（ISO 14229）服务集与封装约定。协议事实层——脱离具体工程也成立。

## 会话与寻址

| 项 | 值 |
|----|-----|
| DefaultSession | 0x01 |
| ProgrammingSession | 0x02 |
| ExtendedSession | 0x03 |
| Physical（物理寻址） | 0 |
| Functional（功能寻址） | 1 |
| HardReset | 0x01 |
| SoftReset | 0x03 |

## 服务总表

| SID | 服务 | 用途 |
|-----|------|------|
| 0x10 | DiagnosticSessionControl | 切换会话 |
| 0x11 | ECUReset | 复位 ECU |
| 0x14 | ClearDiagnosticInformation | 清 DTC |
| 0x19 | ReadDTCInformation | 读 DTC / 快照 / 扩展数据 |
| 0x22 | ReadDataByIdentifier | 按 DID 读数据 |
| 0x23 | ReadMemoryByAddress | 按地址读内存 |
| 0x27 | SecurityAccess | 安全访问（seed/key） |
| 0x28 | CommunicationControl | 通信控制（抑制报文） |
| 0x29 | Authentication | 认证（新车规） |
| 0x2A | ReadDataByPeriodicIdentifier | 周期读 |
| 0x2C | DynamicallyDefineDataIdentifier | 动态定义 DID |
| 0x2E | WriteDataByIdentifier | 按 DID 写数据 |
| 0x2F | InputOutputControlByIdentifier | IO 控制 |
| 0x31 | RoutineControl | 例程控制（含擦除） |
| 0x34 | RequestDownload | 请求下载 |
| 0x35 | RequestUpload | 请求上传 |
| 0x36 | TransferData | 传输数据 |
| 0x37 | RequestTransferExit | 退出传输 |
| 0x38 | RequestFileTransfer | 文件传输 |
| 0x3D | WriteMemoryByAddress | 按地址写内存 |
| 0x3E | TesterPresent | 在线保持 |

封装函数命名：`SID<XX>_<Action>`，校验：`CheckPosResponse_SID<XX>` / `CheckNegResponse(NRC)` / `CheckNonResponse()`。完整列表见 [api-reference.md](api-reference.md)。

## 通用 NRC（否定响应码）

| NRC | 含义 |
|-----|------|
| 0x10 | generalReject |
| 0x11 | serviceNotSupported |
| 0x12 | subFunctionNotSupported |
| 0x13 | incorrectMessageLengthOrInvalidFormat |
| 0x14 | responseTooLong |
| 0x22 | conditionsNotCorrect |
| 0x24 | requestSequenceError |
| 0x31 | requestOutOfRange |
| 0x33 | securityAccessDenied |
| 0x35 | invalidKey |
| 0x36 | exceedNumberOfAttempts |
| 0x37 | requiredTimeDelayNotExpired |
| 0x70 | uploadDownloadNotAccepted |
| 0x71 | transferSuspended |
| 0x72 | generalProgrammingFailure |
| 0x73 | wrongBlockSequenceCounter |
| 0x7E | subFunctionNotSupportedInActiveSession |
| 0x7F | serviceNotSupportedInActiveSession |

校验否定响应：`CheckNegResponse(NRC)`，NRC 为数组可带主副码，如 `{0x12, 0x7E}`。

## 安全访问（0x27）流程

1. `SID27_SecurityAccess_RequestSeed(level)` — level 为奇数（0x01/0x03/.../0x17），返回 seed 存 `SeedArray`。
2. 用 `GenerateKeyExOpt.h` 算法由 seed 派生 key，或查预置 pincode 表（`pincode0x01`..`pincode0x17`）。
3. `SID27_SecurityAccess_SendKey(level+1, key, size)` — level+1 为偶数。

预置 pincode 表覆盖 level：0x01/0x03/0x05/0x07/0x09/0x0B/0x0D/0x0F/0x11/0x13/0x15/0x17。未配置的 level 填 `0xFF*16`。具体 pincode 值属项目密钥，查目标工程的 `test_case_diag_api.h`，不在本知识库固化。

## 超时参数（tsdiag_set_*）

- `p2_server_max` — 服务器响应超时
- `p2*_server_max` — 扩展响应超时
- `s3_server` — 服务器会话保持超时
- `s3_client` — 客户端 TesterPresent 周期

具体数值按项目，查目标工程 `Configuration.h` 或 `on_start` 初始化代码。

## DID / RID / DTC

- **DID**（Data Identifier，0x22/0x2E）：2 字节标识，值由项目定义，查 PDX 数据库（`PDXParser.h` 解析）或现场用 TSMaster MCP `database_manage` 读。
- **RID**（Routine Identifier，0x31）：2 字节，如擦除例程。
- **DTC**（0x14/0x19）：故障码，状态掩码字节（bit0=testFailed ... bit7=warningIndicatorRequested）。

> DID/RID/DTC 的具体值是项目数据，**不预记忆**——现场从目标工程的 PDX/.dbc 读，或用 MCP `database_manage` 查。
