# TSMaster C API 速查

写测试用例时调用的 API 分三层，自底向上：底层 C API（`MPLibrary.h` 函数指针表）→ 诊断封装层（`test_case_diag_api.h`）→ 报告层（`TestReport.h`）。日常写用例只用上两层。

## 一、诊断封装层（test_case_diag_api.h）— 主用

命名规律：`SID<XX>_<Action>` 发请求，`CheckPosResponse_SID<XX>` / `CheckNegResponse` / `CheckNonResponse` 校验响应。

### 会话与寻址枚举
- `DiagSession`: `DefaultSession=0x01` / `ProgrammingSession=0x02` / `ExtendedSession=0x03`
- `AddressMode`: `Physical=0` / `Functional=1`
- `ECUReset`: `HardReset=0x01` / `SoftReset=0x03`
- `SPRMIB`: `SPRMIB_FALSE` / `SPRMIB_TRUE`（肯定响应抑制位）

### UDS 服务封装（完整列表）
| SID | 服务 | 封装函数 |
|-----|------|----------|
| 0x10 | DiagnosticSessionControl | `SID10_SessionControl(sub)` |
| 0x11 | ECUReset | `SID11_ECUReset(reset)` |
| 0x14 | ClearDiagnosticInformation | `SID14_ClearDTCInfo` / `SID14_ClearAllDTC` |
| 0x19 | ReadDTCInformation | `SID19_ReadDTC` / `SID19_ReportDTCByStatusMask` / `SID19_ReportDTCSnapshotRecordByDTCNumber` / `SID19_ReportDTCExtendedDataRecordByDTCNumber` / `SID19_CheckSnapshotIdentification` / `SID19_ReportSupportDTC` / `SID19_ReportDTCSnapshotIdentification` |
| 0x22 | ReadDataByIdentifier | `SID22_ReadDataByIdentifier(DID)` |
| 0x23 | ReadMemoryByAddress | `SID23_ReadDataByAddress` |
| 0x27 | SecurityAccess | `SID27_SecurityAccess_RequestSeed(level)` / `SID27_SecurityAccess_SendKey(level)` |
| 0x28 | CommunicationControl | `SID28_CommunicationControl(ctrlType)` |
| 0x29 | Authentication | `SID29_Authentication_SF` |
| 0x2A | ReadDataByPeriodicIdentifier | `SID2A_useIDReadData` |
| 0x2C | DynamicallyDefineDataIdentifier | `SID2C_DynamicallyDefDataId` |
| 0x2E | WriteDataByIdentifier | `SID2E_WriteDataById` |
| 0x2F | InputOutputControlByIdentifier | `SID2F_IO_Control_byid` |
| 0x31 | RoutineControl | `SID31_RoutineControl` / `SID31_RoutineControl_EraseMemory` |
| 0x34 | RequestDownload | `SID34_Request_Download` |
| 0x35 | RequestUpload | `SID35_Request_Upload` |
| 0x36 | TransferData | `SID36_Transfer_Data` |
| 0x37 | RequestTransferExit | `SID37_RequestTransferExit` |
| 0x38 | RequestFileTransfer | `SID38_Request_File_Transfer` |
| 0x3D | WriteMemoryByAddress | `SID3D_writeMemoryByAddress` |
| 0x3E | TesterPresent | `SID3E_TesterPresent` |

### 响应校验三态
- `CheckPosResponse_SID<XX>(期望参数)` — 肯定响应
- `CheckNegResponse(NRC[])` — 否定响应（NRC 数组，如 `{0x12,0x7E}`）
- `CheckNonResponse()` — 无响应
- `CheckCurrentSession(0x02)` — 校验当前会话

### SWDL 序列封装
- `TestSeq_EnterProgrammingSession()`
- `TestSeq_DownloadSBL(0)` — 下载 SBL（参数 0 = 不激活）
- `ActivateSBL()` — 激活 SBL
- `DownloadSBL()`

### 测试流程封装
- `TestPrecondition()` / `TestPostCondition()` — 前置/后置
- `TestWait(ms)` — 等待
- `TestReport_HTML_StepDescription("...")` — 步骤描述

### 安全访问密钥
预置 pincode 表（`pincode0x01`..`pincode0x17`，16 字节 each），覆盖奇数 level（0x01/0x03/0x05/.../0x17）。seed→key 经 `GenerateKeyExOpt.h` 算法生成。详见 [uds-services.md](uds-services.md)。

## 二、报告层（TestReport.h）

### 高层封装
- `TestReport_HTML_Init` / `TestReport_HTML_Final` — 报告起止
- `TestReport_HTML_Title` — 用例标题
- `TestReport_HTML_Step` / `TestReport_HTML_StepDescription` — 步骤

### 底层 test_report 结构（MPLibrary.h）
`test_init` → `test_set_report_name` → `test_testinfo(产品/HW/SW/DUT/工程师/厂商)` → `test_title(组/用例/图/目的)` → `test_step` / `test_pass` / `test_fail` / `test_warning` → `test_final`。另含 `test_set_watermark` / `test_set_html_name_and_logo` / `sddb_to_json`。

## 三、底层 C API（MPLibrary.h，通常不直接用）

按类别（函数指针 typedef，`tsdiag_*` / `tstp_*` 前缀）：
- **诊断模块管理**: `tsdiag_can_create/delete/delete_all`，`tsdiag_set_p2_timeout/p2_extended/s3_servertime/s3_clienttime`
- **TP 传输**: `tstp_send_functional/send_request/request_and_get_response[_functional]`，CAN 变体 `tstp_can_*`
- **UDS 服务底层**: `tsdiag_session_control/communication_control/routine_control/security_access_request_seed/send_key/request_download/request_upload/transfer_data/request_transfer_exit/write_data_by_identifier/read_data_by_identifier` + `tsdiag_can_*` 变体
- **总线收发**: 见 TSMaster.h（CAN/CANFD/LIN 发送、接收回调、FIFO）
- **系统变量**: 读写系统变量
- **数据库**: `sddb_to_json`

> 底层 API 签名查 `templates/miniprogram-skeleton/MPLibrary.h`，诊断封装实现查 `test_case_diag_api.h`。写用例优先用封装层，封装层不够时再下沉。
