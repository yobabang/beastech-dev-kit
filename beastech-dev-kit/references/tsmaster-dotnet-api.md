# TSMaster .NET Interop API 速查

从 `Interop.TSMasterAPI.dll` (v2023.10.20.978, .NET assembly) 反射提取的公共 API 面。TSMaster 自身是原生 C++ 应用，此 Interop DLL 是 C# 侧调用 TSMaster 原生能力的桥接层。

> **重要**：写 MiniProgram 测试用例用 C API（`MPLibrary.h` + `test_case_diag_api.h`），**.NET Interop 用于外部工具/脚本集成**，如 Python `pythonnet`、C# 自动化、CI pipeline 等。

## API 面统计

| 类别 | 数量 |
|------|------|
| Enums | 44 |
| Structs | 49 |
| Delegate Classes (dotnet 回调) | 62 |
| 主 API 类 `TsMasterApi` | 1（全静态方法） |

---

## 一、核心 Enums

### 通道与总线

| Enum | 值 |
|------|-----|
| `APP_CHANNEL` | CHN1..CHN32 (0..31) |
| `TLIBApplicationChannelType` | 应用通道类型 |
| `TLIBBusToolDeviceType` | 总线工具设备类型 |

### CAN / CANFD / CANXL
- `TLIBCANFDControllerType` — CANFD 控制器类型
- `TLIBCANFDControllerMode` — CANFD 控制器模式
- `TLIBCANBusStatistics` — CAN 总线统计

### LIN
- `LIN_PROTOCOL` — LIN_PROTOCOL_13 / 20 / 21 / J2602
- `TLINProtocol` — LIN 协议版本
- `TLINNodeType` — LIN 节点类型

### 诊断/TP
- `ISO_TP_RESAULT` — 传输层结果码（N_OK=0, N_TP_TIMEOUT_AS=139, N_TP_TIMEOUT_AR=140, N_TP_TIMEOUT_BS=141, N_TP_TIMEOUT_CR=142, N_TP_WRONG_SN=143, N_TP_INVALID_FS=144, N_TP_UNEXP_PDU=145, N_TP_WFT_OVRN=146, N_TP_BUFFER_OVFLW=147, N_TP_NOT_IDLE=148, N_TP_ERROR_FROM_CAN_DRIVER=149；LIN 变体 202-212；ETH 变体 234-251）

### 系统变量/自动化
- `TLIBSystemVarType` — 系统变量类型
- `TLIBPanelControlType` — 面板控件类型（pctText/pctImage/pctButton/pctLED/pctGauge... 共 25 种）
- `TLIBPanelSignalType` — 面板信号类型
- `TLIBAutomationSignalType` — 自动化信号类型
- `TLIBAutomationModuleRunningState` — 自动化模块运行状态

### 数据/DB
- `TLIBMBDDataType` — 模型数据类型（dtInherit/double/single/half/int8/int16/int32/int64/uint8/uint16/uint32/uint64/bool/string/enum/bus/image）

### 回放
- `TLIBOnlineReplayStatus` — orsNotStarted/orsRunning/orsPaused/orsCompleted/orsTerminated
- `TLIBOnlineReplayTimingMode` — ortImmediately/ortAsLog/ortDelayed
- `TReplayPhase` — rppInit/rppReplaying/rppEnded

### 信号测试
- `TSignalCheckKind` — sckAlways/sckAppear/sckStatistics/sckRisingEdge/sckFallingEdge/sckMonotonyRising/sckMonotonyFalling/sckFollow/sckJump/sckNoChange
- `TSignalStatisticsKind` — sskMin/sskMax/sskAverage/sskStdDeviation
- `TSignalTesterFailReason` — tfrNoError/tfrCheckSignalNotExistsInDB/tfrMinBiggerThanMax/...

### AI Flow（图形编程）
- `TAiFlowObjectType` — AIT_DIAGRAM/AIT_JUNCTION/AIT_GROUP/AIT_STATE/AIT_*_FUNCTION/AIT_TRUTH_TABLE...
- `TAiFlowAlignMode` — ALIGN_LEFT/RIGHT/TOP/BOTTOM/CENTER_H/CENTER_V
- `TAiFlowDecomposition` — DECOMP_OR/AND/CLUSTER

### FlexRay
- `TFlexRayCompuMethod` — FlexRay 计算方法

### 其他
- `lwip_ip_addr_type` — IPADDR_TYPE_V4/V6/ANY
- `TUDPFragmentProcessStatus` — ufpsNotFragment/ufpsInvalid/ufpsProcessing/ufpsDone
- `TTriggerSignalType` / `TTimeRangeTestMode` / `TSupportedObjType` / `TSignalType` / `TSTIMSignalStatus` / `TMPTacValueType` / `TMPTacDebugEvent` / `TMBD_PriorityKind` / `TLinkedDataChnType`

---

## 二、核心 Structs

### CAN 报文
- `TLIBCAN` — 经典 CAN 帧（Chn, ID, DLC, Data, Timestamp...）
- `TLIBCANFD` — CAN FD 帧（Chn, ID, DLC, BRS, ESI, Data[64]...）
- `TLIBCANXL` — CAN XL 帧

### LIN 报文
- `TLIBLIN` — LIN 帧（IdxChn, ID, DLC, IsTx, DataArray[]）
- `TMPLINSignal` — LIN 信号属性

### FlexRay 报文
- `TLIBFlexRay` — FlexRay 帧
- `TLIBFlexRayClusterParameters` / `TLIBFlexRayControllerParameters` / `TLIBFlexrayConfigurationPara` / `TLIBFlexrayFrameTrigger`

### 以太网
- `TLIBEthernetHeader` / `TLIBEthernetMAX` / `TLIBEth_CMD_config`

### 系统变量
- `TLIBSystemVar` — 系统变量值
- `TLIBSystemVarDef` — 系统变量定义
- `TLIBTSMapping` — 符号映射

### GPS
- `TLIBGPSData` — GPS 数据

### 硬件
- `TLIBHWInfo` — 硬件信息（设备信息）

### 网络（lwIP 封装）
- `Teth_addr_t` / `Tip4_addr_t` / `Tip6_addr_t` / `Tip_addr_t`
- `Ts_in_addr` / `Ts_in6_addr`
- `Tts_addrinfo` / `Tts_sockaddr` / `Tts_sockaddr_in` / `Tts_sockaddr_in6`
- `Tts_msghdr` / `Tts_iovec` / `Tts_cmsghdr`
- `Tts_fd_set` / `Tts_pollfd` / `Tts_timeval`
- `Tts_net_device` / `Tts_hostent`

### 数据库属性
- `TMPDBProperties` / `TMPDBECUProperties` / `TMPDBFrameProperties` / `TMPDBPDUProperties` / `TMPDBSignalProperties`
- `TMPCANSignal` / `TMPFlexRaySignal`

### 触发/记录
- `TLIBTrigger_def` — 触发器定义
- `Trealtime_comment_t` — 实时注释
- `TEMMC_RECORD_DATA` — eMMC 记录数据
- `TTSMetricIntegerSnapshot` — 时序度量快照

---

## 三、主 API 类：`TsMasterApi`

全静态方法。以下是按功能域分类的方法集（部分列表）：

### 环境/初始化
- `add_path_to_environment(path)`
- `append_user_constant(name, value, desc)`
- `append_user_constants_from_c_header(headerFile)`

### CCP/XCP 标定 (cal_*)
- `cal_add_ccp_ecu` / `cal_add_xcp_ecu` / `cal_remove_ecu`
- `cal_get_ecu_a2l_list` / `cal_get_measurement_list` / `cal_get_var_property`
- `cal_load_new_a2l` / `cal_switch_to_ccp`
- `cal_set_all_datas_by_value` / `cal_set_datas_by_offset`

### 诊断
- `tsdiag_can_create` / `tsdiag_can_delete` / `tsdiag_can_delete_all`
- `tsdiag_set_fdmode` / `tsdiag_set_is_valid`
- `tstp_can_request_and_get_response` / `tstp_can_request_and_get_response_functional`

### 发送/Callback（delegate classes）
其余大量方法是 callback delegate 封装，对应 C API 的事件注册模式：
- `TCANQueueEvent` / `TCANFDQueueEvent` / `TLINQueueEvent` / `TFlexRayQueueEvent` / `TEthernetQueueEvent` / `TGPSQueueEvent` / `TDatapackageProcessEvent`
- `TOnSignalEvent` / `TOnSysVarChange` / `TOnSystemVarPreReadEvent`
- `TSSocketReceiveEvent` / `TSSocketTransmitEvent` / `TSSocketListenEvent` / `TSSocketNotifyEvent`
- `TOnAutoSARE2ECanEvt` / `TOnAutoSARPDUQueueEvent` / `TOnAutoSARPDUPreTxEvent`
- `TReadBLFSystemVarCallback` / `TReadBLFRealtimeCommentCallback`
- `TOnUSBPlugEvent` / `TOnIoIPData` / `TOnIoIPConnection`

### 文档/写 API
- `TLIBWriteAPIDocument` / `TLIBWriteAPIParaFunc` / `TLIBWriteAPIDocumentFunc`
- `TLIBTSMasterLogger` — 日志接口

---

## 四、与 C API 的对应关系

| .NET Interop | MiniProgram C |
|---|---|
| `Ttsdiag_can_create` | `tsdiag_can_create` (MPLibrary.h) |
| `Ttsdiag_can_delete` | `tsdiag_can_delete` |
| `Ttstp_can_request_and_get_response` | `tstp_can_request_and_get_response` |
| `TLIBCAN` struct | `TLIBCAN` (TSMaster.h) |
| `TLIBCANFD` struct | `TLIBCANFD` (TSMaster.h) |
| `TLIBSystemVar` | system var read/write API |
| `TCANQueueEvent` | `app.register_event_can` |

.NET Interop 本质上是 C API 的托管封装，**函数签名几乎 1:1 对应**。如果 C API 文档不足，可借助此 Interop 的类型信息逆向出函数签名和枚举值。

---

## 五、GenKeyX86Wrapper.exe

`C:\Program Files (x86)\TOSUN\TSMaster316\bin\GenKeyX86Wrapper.exe`
- 类型：PE32 .NET assembly（x86 托管控制台程序）
- 大小：70KB
- 用途：安全访问（0x27 SecurityAccess）密钥生成器 — 接收 seed，返回 key
- 对应 C 侧：`GenerateKeyExOpt.h`（MiniProgram 骨架中的安全算法头文件）
- 注意：此 EXE 是 x86 架构，需 x86 .NET 运行时执行；在 x64 dotnet 环境中无法直接 `Assembly.LoadFrom()` 加载

> 写 MiniProgram 测试用例时，直接在 C 代码里调用 `GenerateKeyExOpt.h` 的 key 生成函数即可，不需要这个 EXE。它主要用于外部脚本/工具链的独立 key 计算场景。
