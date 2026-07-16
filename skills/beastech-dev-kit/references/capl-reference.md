# CAPL (CANoe Access Programming Language) 参考

Vector CANoe/CANalyzer 的 CAPL 脚本语言知识库。用于**编写 CAPL 测试脚本**（不是迁移到 TSMaster，而是直接在 CANoe 里写 CAPL）。

## 本地资源

```
references/
├── capl-help/
│   ├── CANeds.chm              # CANoe DE Family 编译帮助（含完整 CAPL 函数参考，4.4MB）
│   └── StandaloneManager.chm   # Standalone Manager 帮助（1.3MB）
└── capl-to-c-mapping.md        # 本文件

templates/capl-to-c/
├── 函数(CAPL函数映射).xlsx       # CAPL 函数快速索引（1669 函数 + 签名 + 中英文描述）
├── SysAndCan.h / .cpp           # CAPL 兼容层样板（保留作为 TSMaster 桥接参考）
└── MiniProgram_Extern_Scaffold.cpp
```

> **CHM 文件**在 Windows 上可直接双击打开，是 CAPL 最权威的函数参考手册。用 Ctrl+F 搜索函数名即可。

---

## 一、CAPL 函数快速索引

`函数(CAPL函数映射).xlsx` 包含如下分表：

| 分表 | 行数 | 内容 |
|------|------|------|
| Sheet1 | 1669 | 主索引：ADAS / MAP / CAPLProfiler / CRC / 字符串转换 / 各类 CAPL 函数 |
| CAN | 61 | `canEnableStatistics`, `canGetBusLoad`, `canFdSetConfiguration`, `canFlushTxQueue`... |
| CAN Disturbance | 14 | `canDisturbanceFrameTrigger`, `canDisturbanceSequence`, `canDisturbanceTriggerEnable`... |
| Lin | 154 | `linActivateResps`, `linChangeSchedTable`, `linCheckRespError`, `linBusIsAwake`... |
| Flexray | 48 | `frEnableStatistics`, `frGetBusLoadStatic`, `frGetFrameCRC`, `frSetMode`, `frSetPOCState`... |
| 系统变量 | 11 | `lookupSysvarFloat`, `lookupSysvarInt`, `sysvarIF::IsOfType`... |

每行包含：**函数名 → 完整签名（含多态重载）→ 中文描述 → 英文描述**

---

## 二、CAPL 函数分类概览（来自 CANoe 官方文档）

CAPL 函数按协议/功能域分 50+ 个类：

### 总线通信

| 类 | 常用函数举例 |
|----|-------------|
| **CAN** | `canEnableStatistics`, `canGetBusLoad`, `canFdSetConfiguration`, `canFlushTxQueue`, `canResetStatistics` |
| **CAN Disturbance** | `canDisturbanceFrameTrigger::SetMessage`, `canDisturbanceTriggerEnable`, `canDisturbanceTriggerNow` |
| **LIN** | `linActivateResps`, `linChangeSchedTable`, `linCheckRespError`, `linBusIsAwake`, `linConfigureResponse` |
| **FlexRay** | `frEnableStatistics`, `frGetBusLoadStatic/Dynamic`, `frSetMode`, `frSetPOCState`, `frSetKeySlot`, `frUpdatePDU` |
| **Ethernet** | 含 TCP/IP API、TLS API、DoIP 相关 |
| **MOST** | 光纤总线相关 |
| **A429** | ARINC 429 航空总线 |
| **J1939** | 商用车 CAN 高层协议 |
| **J1587** | 商用车诊断 |
| **ISO11783** | 农用总线 (ISOBUS) |
| **KLine** | K-Line 诊断 |

### 诊断

| 类 | 常用函数举例 |
|----|-------------|
| **Diagnostics** | `DiagRequest`, `DiagGetResponse`, `DiagSetParameter`, `DiagGetParameter` |

### 测试 (Test Feature Set)

| 类 | 常用函数举例 |
|----|-------------|
| **TSL (Test Service Library)** | `TestCaseReport`, `TestStepPass/Fail`, `TestWaitForMessage`, 各种 Check 函数 |
| **TSL Stimulus** | Ramp/Value/Toggle/CsvFile stimulus |
| **TSL Check** | Control/Observation/Signal checks |

### 系统与工具

| 类 | 常用函数举例 |
|----|-------------|
| **SystemVariables** | `lookupSysvar`, `lookupSysvarFloat/Int/String`, `sysvarIF` |
| **SignalAccess** | 信号读写 |
| **CommunicationObjects** | 通信对象模型 |
| **DistributedObjects** | 分布式对象 |
| **AssociativeFields** | 关联数组 |
| **CAPLProfiler** | `Start`, `Stop`, `GetCallCount`, `GetAverageCallTimeNS` |

### ADAS / 地图

| 类 | 常用函数举例 |
|----|-------------|
| **ADAS** | `ADASActivateFMU`, `ADASSetOutputData` |
| **MapWindowAPI** | `AddMapPolylinePoint`, `ClearMapPolyline`, `DrawMapObject` |
| **Car2x** | V2X 通信 |
| **Sensor** | 传感器数据注入 |

### 外部交互

| 类 | 常用函数举例 |
|----|-------------|
| **GPIB** | GPIB 仪器控制 |
| **RS232** | 串口 |
| **MATLAB** | MATLAB 接口 |
| **CarMaker** | CarMaker 联合仿真 |

### 安全

| 类 | 常用函数举例 |
|----|-------------|
| **Security** | 安全协议相关 |
| **TLS API** | TLS 加密通信 |

---

## 三、CAPL 事件过程 (Event Procedures)

CAPL 是事件驱动的。核心事件入口：

```c
// CAN 消息接收
on message CAN_MSG   { /* CAN_MSG 是 DBC 中定义的消息名 */ }
on message *         { /* 所有 CAN 消息 */ }
on message 0x123     { /* ID=0x123 */ }

// 定时器
on timer myTimer     { /* 定时器回调 */ }

// 按键
on key 'A'           { /* 按 A 键 */ }
on key 'B'           { /* 按 B 键 */ }

// 系统变量变化
on sysvar MyVar      { /* 系统变量变化 */ }

// 诊断
on diagResponse REQ  { /* 诊断响应 */ }

// 环境变量
on envvar MyEnvVar   { /* 环境变量变化 */ }

// FlexRay
on frFrame *         { /* 所有 FlexRay 帧 */ }

// LIN
on linMessage *      { /* 所有 LIN 消息 */ }

// 错误帧
on errorFrame        { /* CAN 错误帧 */ }
```

---

## 四、常用 CAPL 语法速查

```c
variables {
    // 全局变量声明区
    msTimer myTimer;           // 毫秒定时器
    timer mySecTimer;          // 秒定时器
    message 0x123 txMsg;       // CAN 发送消息
    int counter = 0;
    char buffer[100];
}

// 定时器设置
setTimer(myTimer, 100);        // 100ms 后触发
setTimerCyclic(myTimer, 50);   // 每 50ms 触发

// 发送消息
output(txMsg);                 // 发送到总线
txMsg.dlc = 8;
txMsg.byte(0) = 0xAA;
output(txMsg);

// 字符串
snprintf(buffer, elcount(buffer), "Value: %d", counter);
write("Log: %s", buffer);      // Write 窗口输出
writeDbgLevel(2, "Debug: %d", counter);  // 调试输出

// 条件 / 循环
if (counter > 10) { ... }
for (i = 0; i < 10; i++) { ... }
while (condition) { ... }
switch (val) { case 1: ... }
```

---

## 五、如何在 TSMaster 中应用 CAPL 知识

虽然 beastech-dev-kit 主要面向 TSMaster MiniProgram（C++），但 CAPL 知识仍然有用：

1. **协议理解**：CAPL 函数文档是理解车载总线协议行为的最佳参考
2. **测试逻辑设计**：CAPL 的 Test Feature Set 中的 Check/Stimulus 模式可以直接映射到 TSMaster 测试设计
3. **兼容层**：`SysAndCan.h/.cpp` 展示了如何在 TSMaster 中复用 CAPL 风格的 API
4. **CANoe 联合调试**：当测试需要同时在 CANoe 和 TSMaster 上运行时，CAPL 知识是必须的
