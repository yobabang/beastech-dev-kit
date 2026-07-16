# CAPL → TSMaster C 转换参考

将 CANoe CAPL（Communication Access Programming Language）测试逻辑迁移到 TSMaster MiniProgram C++ 环境的映射参考。

> 核心资料：`templates/capl-to-c/函数(CAPL函数映射).xlsx` — 1669 行 CAPL 函数 → TSMaster 对照表（含 CAN/CAN Disturbance/LIN/FlexRay/SystemVar 分表）

## 一、转换工程样板

`templates/capl-to-c/` 目录下是一个 CAPL 转 TSMaster MiniProgram 的完整样板：

```
capl-to-c/
├── MiniProgram_Extern_Scaffold.cpp  # 运行时骨架（logging/output/MP注册）
├── SysAndCan.h                      # CAPL 函数 → TSMaster C 映射声明
├── SysAndCan.cpp                    # CAPL 函数 → TSMaster C 映射实现
└── 函数(CAPL函数映射).xlsx           # 完整 CAPL 函数映射表（1669行）
```

### 1.1 MiniProgram 运行时骨架 (`MiniProgram_Extern_Scaffold.cpp`)

TSMaster 自动生成的 CAPL 转换骨架，包含：

- **全局对象**：`TTSApp app; TTSCOM com; TTSTest test;`
- **输出模板**：`output<TCAN>()`, `output<TLIN>()`, `output<TFlexRay>()`, `output<TEthernetHeader>()`
- **日志系统**：`internal_log()` / `internal_test_log()` / `test_logCAN()` — 支持 C++ `vsnprintf_s`
- **MiniProgram 注册**：`initialize_miniprogram()` / `finalize_miniprogram()` / `retrieve_mp_abilities()` — 结构体大小校验 + step function 注册

### 1.2 转换后的主文件 (`CCode3149.cpp`)

```cpp
#include "SysAndCan.h"  // CAPL-to-C 映射头文件

// CAPL on shortcut 'A' → C on_shortcut
void on_shortcut_NewOn_Shortcut1(const s32 AShortcut) {
    try {
        canEnableStatistics(1);  // 纯 CAPL 风格调用
    } catch (...) { 
        log_nok("CRASH detected"); 
        app.terminate_application(); 
    }
}

// CAPL timer / step → C step function (周期 100ms)
void step(void) {
    try {
        // 周期性逻辑
    } catch (...) { ... }
}
```

---

## 二、CAPL → TSMaster C 函数映射速查

### CAN 函数映射（部分，完整列表见 Excel CAN 表 61 行）

| CAPL 函数 | TSMaster C 等价 |
|---|---|
| `canEnableStatistics(channel, flag)` | `com.enable_bus_statistics(flag)` |
| `canGetBusLoad(channel, selector)` | `com.get_bus_load(channel, ...)` |
| `canFlushTxQueue(channel)` | `com.flush_tx_queue(channel)` |
| `canActivateTXSelfAck(channel, activate)` | 对应 TSMaster CAN 配置 API |
| `canConfigureBusOff(channel, canId, flags)` | 对应 `com.set_bus_off(...)` |

### LIN 函数映射（部分，完整列表见 Excel LIN 表 154 行）

| CAPL 函数 | TSMaster C 等价 |
|---|---|
| `linActivateResps()` | `com.lin_activate_responses(...)` |
| `linChangeSchedTable(tableIndex)` | `com.lin_change_schedule_table(...)` |
| `linCheckRespError()` | `com.lin_check_response_error(...)` |
| `linBusIsAwake()` | `com.lin_is_awake(...)` |

### FlexRay 函数映射（部分，完整列表见 Excel FlexRay 表 48 行）

| CAPL 函数 | TSMaster C 等价 |
|---|---|
| `frEnableStatistics(channel, flag)` | `com.enable_fr_statistics(...)` |
| `frGetBusLoadStatic(channel, ...)` | `com.get_fr_static_busload(...)` |
| `frGetBusLoadDynamic(channel, ...)` | `com.get_fr_dynamic_busload(...)` |
| `frGetFrameCRC(frame)` | 对应 TSMaster FlexRay CRC API |
| `frEnableNodeTx(channel, node, enable)` | `com.fr_enable_node_tx(...)` |

### System Variable 函数映射

| CAPL 函数 | TSMaster C 等价 |
|---|---|
| `lookupSysvarFloat(path)` | `app.get_system_var_float(...)` |
| `lookupSysvarInt(path)` | `app.get_system_var_int(...)` |
| `lookupSysvarString(path)` | `app.get_system_var_string(...)` |

---

## 三、SysAndCan 模式：CAPL 兼容层

`SysAndCan.h/cpp` 核心理念：**在 C++ 中声明与 CAPL 同名/同签名的函数**，内部调用 TSMaster COM/APP API。

```cpp
// SysAndCan.h — CAPL 函数声明
void canEnableStatistics(long flag);     // CAPL: canEnableStatistics(channel, flag)
void frSetKeySlot(long channel, long channelMask, long keySlotIndex, 
                  long keySlotId, long keySlotUsage);
long frSetMode(int channel, int channelMask, u32 mode);
long frSetPOCState(long channel, long ccNumber, long pocState);
void frSetPduFilter(frPDU message, long mode);
void output(frSlot fexRaySlot);
// ... 更多 FlexRay CAPL 函数

// SysAndCan.cpp — 实现映射
void canEnableStatistics(long flag) {
    if (flag) com.enable_bus_statistics(true);
    else      com.enable_bus_statistics(false);
}
// FlexRay 函数为桩实现 (TODO: 映射到 TSMaster C API)
```

这种模式让 CAPL 代码可以**最小修改**迁移到 TSMaster——只需实现兼容层的函数体。

---

## 四、转换模式总结

### 4.1 CAPL → C 一一对应

| CAPL 概念 | TSMaster MiniProgram C++ |
|---|---|
| `on key 'A'` | `void on_shortcut_XXX(const s32 AShortcut)` |
| `on timer <name>` | `void step(void)` 中周期检查 |
| `output(msg)` | `com.transmit_can_async(msg)` / `output<TCAN>(msg)` |
| `write(message)` | `com.transmit_can_async(msg)` |
| `testWaitForMessage(msg, timeout)` | `com.wait_for_can_message(...)` |
| `testStepPass/pass` | `test.pass(...)` / `test.pass(...)` |
| `testStepFail/fail` | `test.fail(...)` / `test.fail(...)` |
| `snprintf/sprintf` | `sprintf_s` 或 `internal_log` |
| `writeDbgLog` | `app.debug_log(...)` |

### 4.2 不要直接翻译

- **定时器** → MiniProgram `step()` 函数（设周期 ms），不用 Sleep/循环
- **CAPL 事件驱动** → MiniProgram 用 `On_Start` 注册回调、`Step_Function` 做轮询
- **CAPL 的 `this`** → MiniProgram 是普通 C 函数，无 `this`，用全局结构体 `app/com/test`

### 4.3 三态校验保持一致

CAPL 的 `DiagRequest` / `DiagGetResponse` / NRC 校验 → TSMaster 用相同的封装模式：
```c
SID10_SessionControl(0x01);
CheckPosResponse_SID10(0x01);    // 肯定响应
// or CheckNegResponse(NRC);     // 否定响应
// or CheckNonResponse();        // 无响应
```

---

## 五、补充：FlexRay 专用函数模板

`SysAndCan.h` 中声明的 FlexRay 函数是**完整的 CAPL FlexRay API 子集**，可直接用作 TSMaster FlexRay 测试的 API 需求清单：

- `frSetKeySlot` — 配置 Key Slot
- `frSetMode` — 设置控制器模式
- `frSetPOCState` — POC 状态机控制
- `frSetPayloadLengthInByte` — 帧负载长度
- `frSetPduFilter` — PDU 过滤
- `frSetSendFrame` / `frSetSendGroup` — 发送帧/组
- `frSetSendPDU` — 发送 PDU
- `frSetTrigger` — 触发配置
- `frUpdatePDU` / `frUpdateStatFrame` — 更新帧状态
- `resetFlexRayCC` / `resetFlexRayCCEx` — 控制器复位
- `setSignalStartValues` — 信号初始值
- `output(frSlot)` — 发送 FlexRay Slot

这些函数的实现体在 `SysAndCan.cpp` 中是空桩——**映射到 TSMaster C API 的工作尚未完成**，按需逐个补充。
