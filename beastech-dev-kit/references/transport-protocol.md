# 传输层 TP（CAN/CANFD/LIN/FlexRay/DoIP）

源自 VT 工程 `02_Library/04_TP/`（ComDTC 工程最全）与 `Diagnostic/01_TP/`。TP 层按总线分库实现，beastech 只有单体 `test_tp.h`，此处补全分总线范式。

## TP 库分层（按总线）

```
04_TP/
├── COM_TP.cin          # 公共 TP 接口
├── canTP/CAN_TP_V1.cin      # Classic CAN TP (ISO 15765-2)
├── canfdTP/CANFD_TP_V1.cin  # CANFD TP
├── linTP/LIN_TP_V1.cin      # LIN TP (ISO 15765-2 over LIN)
├── FrTP/                    # FlexRay TP (COM_TP/Fr_TP/FRconfig/Schedule_Config)
├── doipTP/                  # DoIP 以太网诊断 (DoIP_TP/FindEdgeNodeIP)
└── ipTP/                    # IP 层 (IPAddress/IP_TP_V1)
```

**借鉴点**：beastech 的 `test_tp.h` 是单文件，建议按总线拆分；切换总线时只改 TP_BusSet，上层 UDS 不变。

## 公共 TP 接口（COM_TP.cin）

| 函数 | 作用 |
|------|------|
| `DiagParameterinit()` | 诊断参数初始化 |
| `DiagEnable()` | 使能诊断 |
| `TP_BusSet(enum Bus_Type)` | **切换总线**（CAN/CANFD/LIN/ETH） |
| `TP_BusContextSet(char BusName[])` | 按名称设总线上下文 |
| `TP_TesterPresentSet(enum TP_Switch)` | 3E 在线保持开关 |
| `TP_Send(byte Data[], enum AddressMode Mode)` | 发诊断请求（物理/功能寻址） |
| `TP_Send(byte Data[], long length, enum AddressMode Mode)` | 带长度重载 |
| `TP_Send_SmallBuffer(byte Data[], int DataLen, int TpLen, enum AddressMode Mode)` | 小缓冲发送 |
| `TP_GetReceivedData(byte InputData[])` | 取响应数据 |
| `LocalWakeUpCondition()` | 本地唤醒条件 |
| `LinSendHeader(byte frameId)` | 发 LIN 头 |
| `CAN_TP_Init()` / `CANFD_TP_Init()` / `LIN_TP_Init()` / `ETH_TP_Init()` | 各总线 TP 初始化 |
| `GeelyParameterParsing()` | 吉利参数解析 |

## DoIP（车载以太网诊断，ISO 13400）

### 配置（IP_TP.can）
```capl
ETH_TP_SetTransportParameters(
  dword ECULogicAddress, dword TesterLogicAddress, dword FunctionalLogicAddress,
  char TesterIPAddress[], char VehicleIPAddress[], char ETHNetMask[])
ETH_TP_SetTesterMAC(byte MAC[])
```

### 节点类型（DiagConfig.cin）
- `EdgeNode=1` — 作为 TCP 连接的 server（被动监听）
- `InternalNode=2` — 作为 TCP 连接的 client（主动连接）

### 流程
1. `Server_Listen()` / `SetSendTCPConnection()` — 建立 TCP
2. `Waitforcnnected(timeout)` — 等待车辆连接
3. `_DoIP_VehicleConnectedInd()` — 连接建立回调
4. `DoIP_TP_Send(byte Data[], enum AddressMode Mode)` — 发诊断（封装 TCP）
5. `OnTcpReceive(socket, result, address, port, buffer[], size)` — 接收回调
6. `_DoIP_MessageAcknowledgeInd(payloadType, code, testerAddress, ecuAddress)` — 确认
7. `DoIP_ErrorInd(error)` / `_DoIP_ConnectionClosedInd(reason)` — 错误/关闭
8. `ETH_TP_Release()` — 释放

### 地址转换
`addresstrans(dword b, char ipv4[])` — IP/逻辑地址转换。

### DoIP TP 封装（doipTP/DoIP_TP.cin）
- `DoIP_TP_init()` / `DoIP_NRC_Info(byte nrc)` / `DoIP_TP_Send(...)`
- `FindEdgeNodeIP_DTC.cin` — 发现边缘节点 IP

### 多片/多 TCP 处理（UserDefine.cin 范式）
- `gflag_multi_chip_handle` — ECU 是否需要多 TCP 处理
- `gflag_wait_for_multi_tcp_socket` — 等待多 socket 建立
- `gflag_address_mode_change` — 功能寻址转物理寻址
- `first_ip/first_logical_address` / `second_ip/second_logical_address` — 多地址
- `gtime_closetcp_for_linkdown` — link down 后关 TCP RST 延时

## CAN/CANFD TP（ISO 15765-2）

- 单帧（SF）/ 首帧（FF）/ 连续帧（CF）/ 流控制帧（FC）
- 寻址：`Physical` / `Functional`
- 寻址格式：11bit / 29bit / mixed
- CANFD 支持更大 DLC（12/16/20/24/32/48/64 字节）
- 超时：`P2C_APP/P2C_BOOT/P2CD`（客户端）/ `P6C/P6CD` / `S3C/S3S`

## LIN TP

- 基于 LIN 帧传输诊断（单帧/多帧）
- `LIN_TP_Init()` 初始化
- Master 发 Header，Slave 响应数据
- `LinSendHeader(frameId)` 主动发头

## FlexRay TP

- `Fr_TP.cin` + `FRconfig.cin` + `Schedule_Config.cin`
- 基于 FlexRay 时隙调度

## 与 beastech 对照

beastech 的 `test_tp.h` 单文件含 TP 收发，但未按总线拆分。DoIP 在 beastech 工程中较弱（基本聚焦 CAN/CANFD），如需以太网诊断可参考 VT 的 `doipTP/` + `ipTP/` 范式实现。

## 测试关注点

- 总线切换正确性（TP_BusSet 后寻址/收发正常）
- 多帧拆装（SF/FF/CF/FC 时序）
- 寻址模式（物理/功能）
- 超时处理（P2/P2*/S3）
- DoIP 连接建立/释放/错误
- 多 TCP/多片处理
- CANFD 大 DLC 传输
