# CC 通信控制

基于 UDS 0x28 CommunicationControl，控制 ECU 报文的收发使能。

## 服务（0x28）

```
请求: 28 <controlType> <communicationType>
```

### controlType

| 值 | 含义 |
|----|------|
| 0x00 | enableRxAndTx（恢复收发） |
| 0x01 | enableRxAndDisableTx |
| 0x02 | disableRxAndEnableTx |
| 0x03 | disableRxAndTx（全抑制） |

### communicationType

字节位掩码：
- bit0: 默认通信（应用报文）
- bit1: 网络管理报文
- bit2: 应用报文（同 bit0，部分实现区分）

> 具体掩码定义按项目，查目标工程。

## 封装

- `SID28_CommunicationControl(controlType)` — 封装层
- 底层 `tsdiag_communication_control(moduleIndex, controlType)` / `tsdiag_can_communication_control(...)`

## 测试关注点

- 抑制应用报文后，对应 CAN 报文是否停止发送
- NM 报文是否受影响（通常 NM 不被 0x28 抑制，除非 communicationType 含 NM 位）
- 使能/禁用 Rx/Tx 的组合正确性
- 功能寻址（全网 ECU）vs 物理寻址（单 ECU）
- 抑制后恢复时序
- SPRMIB（肯定响应抑制位）行为

## 测试方法

1. 正常收 ECU 周期报文（trace 基线）
2. 发 0x28 抑制
3. 用 TSMaster trace / FIFO 确认目标报文停止
4. 发 0x28 恢复
5. 确认报文恢复

现场验证用 TSMaster MCP `tsmaster_trace_read`（按时间窗/ID 过滤）和 `tsmaster_bus_simulation` `fifo.read`/`statistics.read`。

## CS.json 清单

CC 用例登记在 `CS.json`，结构与 NM.json 相同（`MpLink`/`StartEvent`/`EndEvent`/`TestGroups.MM[]`）。`MpLink` 指向 CC 的 MiniProgram。

> CC 工程源码在 `GEEA4.0_CC_E2E_NM/GEEA_CAN_CC/`。本文件只记协议事实，具体 controlType/communicationType 取值与用例集查目标工程。
