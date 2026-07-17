# NM 网络管理

GEEA 使用 AUTOSAR CAN NM（ISO 14229-5 / AUTOSAR NM 规范）。

> 本文件记 NM 协议事实。NM 工程实战范式（config.ini 数据驱动、多通道遍历、动作库、Python runner）见 [nm-test-framework.md](nm-test-framework.md)。

## NM 报文格式

CAN NM 报文通常 8 字节：

| 字节 | 字段 |
|------|------|
| 0 | Source Node ID（节点标识） |
| 1 | Control Bit Vector（含 PNI、Active Wakeup Bit、Repeat Message Request） |
| 2-5 | Reserved / User Data |
| 6 | NM Timeout Counter（高字节） |
| 7 | NM Timeout Counter（低字节） |

> 具体字节布局按项目 NM 配置，查目标工程 NM.json 或数据库。报文 ID 范围（如 0x500-0x5FF）也属项目配置。

## Control Bit Vector 关键位

| 位 | 含义 |
|----|------|
| PNI（PN Information） | 部分网络标识，决定是否唤醒特定子网 |
| Active Wakeup Bit | 主动唤醒标志 |
| Repeat Message Request | 重复消息请求 |

## PNC（Partial Networking Cluster）— GEEA4.0 核心

PNC 是 NM PDU 中的 bit，控制一组应用报文的收发使能。GEEA4.0 NM 的关键特性。

- **PNC bit 位置**：分布在 NM PDU byte2–byte5（`pnc_begin_byte=2, pnc_end_byte=5`），共 32 位
- **常见 PNC**：PNC25 / PNC29 / PNC36 / PNC38（项目相关）
- **PNC 网关类型**：`no_pnc_gateway`(0) / `passive_pnc_gateway`(1) / `active_pnc_gateway`(2)
- **报文-PNC 映射**：每个 PNC 控制一组报文 ID（如 `tx_msg_pnc25=101,270,361`）
- **测试动作**：`send_nm_msg_pni_and_all_pnc_is1(bus,id)` 把所有 PNC 置 1 唤醒全部部分网络
- **PNC 掩码测试**：in mask → 唤醒；out of mask → 不唤醒（TC_1023218–1023222）

PNC 与报文的具体映射是项目数据，查 config.ini（每网段 `pnccode` / `tx_msg_pncXX`）。

## 状态机

```
Bus Sleep → (网络/硬件唤醒) → Repeat Message State
                                  ↓
                          Normal Operation State
                                  ↓
                            Ready Sleep State
                                  ↓
                             Bus Sleep
```

| 状态 | 行为 |
|------|------|
| Bus Sleep | 总线静默，可被唤醒 |
| Repeat Message State | 快速发 NM 报文，声明节点在线 |
| Normal Operation State | 正常周期发 NM 报文 |
| Ready Sleep State | 等待睡眠，不再发报文 |

## 测试用例覆盖（NM.json 清单）

NM.json 结构：
```json
{
  "MpLink": "NM_TEST.mp",
  "StartEvent": "TestBegin()",
  "EndEvent": "TestEnd()",
  "TestGroups": {
    "MM": [
      { "TestName": "TC_<ID>_<场景>", "TestFuncName": "TC_<ID>_<场景>", "TestArgs": "()", "IsChecked": 1, "Comment": "" }
    ]
  }
}
```

典型用例（TC_1023208 系列）：
- NM 报文 ID 在范围内 → 唤醒 / 保持唤醒
- NM 报文 ID 超范围 → 不唤醒 / 不保持
- NM PDU 网络 / 硬件唤醒
- PNI 识别
- Active Wakeup Bit 识别
- Repeat Message Request 识别（Ready Sleep / Normal Operation 状态下）

用例命名：`TC_<ID>_CAN_NM_<场景描述>`。注意 `TestFuncName` 与 C 函数名一致。

## 测试关注点

- 唤醒源识别（网络唤醒 vs 硬件唤醒）
- 报文 ID 范围边界
- 状态迁移时序（Repeat Message → Normal → Ready Sleep → Sleep）
- NM Timeout 行为
- PNI 子网选择正确性
- 主动唤醒 vs 被动唤醒

## CS.json vs NM.json

- `CS.json` — 通信控制（CC）相关用例清单，结构同 NM.json
- `NM.json` — 网络管理用例清单
- 两文件均通过 `MpLink` 关联 MiniProgram（.mp），`StartEvent`/`EndEvent` 指定起止函数

> NM 具体报文 ID、节点 ID、超时值属项目配置，查目标工程 NM.json 与数据库，现场用 TSMaster MCP `database_manage`/`trace_read` 验证。
