# LIN 通信

LIN（Local Interconnect Network）车载串行总线，用于车身低速通信（ISO 17987）。GEEA 工程中 LIN 测试基于 TSMaster LIN 仿真 + LDF 数据库。

> 本文件只记 LIN 通信功能知识。vECU 虚拟 ECU 控制逻辑不在本工具包范围内（vECU 本质是控制虚拟 ECU 进行测试，按需另行实现）。

## LIN 协议要点

| 项 | 值/说明 |
|----|---------|
| 总线速率 | 通常 19200 bps（LDF `speed` 字段） |
| 主从架构 | 1 个 Master + 多个 Slave，Master 发 Header，Slave 响应 |
| 帧类型 | Unconditional(无条件) / Event-Triggered(事件触发) / Sporadic(偶发) |
| 帧 ID | 0x00–0x3F（6 bit），PID = (ID | ~ID<<2) & 0xFF（带奇偶校验的保护 ID） |
| 数据长度 | 2/4/8 字节（按 ID 段决定） |
| 校验和 | Classic（数据字节和）或 Enhanced（数据+PID） |
| 调度表 | Schedule Table，定义帧发送时序（帧+延时序列） |
| 唤醒/睡眠 | 唤醒信号 → 通信；Sleep 命令 → 进睡眠 |

## LDF（LIN Description File）数据结构

源自 VS_LINCOM/common/LDFAnalysis.h —— LDF 解析器。换项目时从目标工程 LDF 重新解析。

### 节点
```c
typedef struct {
    char* name;        // 节点名
    int   is_master;   // 是否主节点
    float cycle_time;  // 周期
    float timeout;     // 超时
} Node;
```

### 帧
```c
typedef enum {
    FRAME_TYPE_UNCONDITIONAL = 0,
    FRAME_TYPE_EVENT_TRIGGERED = 1,
    FRAME_TYPE_SPORADIC = 2
} FrameType;

typedef struct {
    char*     name;
    int       id;       // 帧 ID (0x00-0x3F)
    int       dlc;      // 2/4/8
    char*     sender;   // 发布节点
    FrameType type;
} Frame;
```

### 信号
```c
typedef struct {
    char*  name;
    int    size;          // 信号长度(bit)
    int    start_bit;     // 起始位
    char** receivers;     // 订阅节点列表
    int    frame_id;      // 所属帧 ID
} Signal;
```

### 调度表（Schedule Table）
```c
typedef struct {
    char* frame_name;   // 帧名
    int   delay_ms;     // 延时(ms)
} ScheduleFrameItem;

typedef struct {
    char*              name;
    ScheduleFrameItem* frame_items;
    int                frame_item_count;
} ScheduleTable;
```

### ParseResult（解析结果总根）
```c
typedef struct {
    char*  protocol_version;
    char*  language_version;
    float  speed;
    Node*  nodes;           int node_count;
    Frame* frames;          int frame_count;
    Signal* signals;        int signal_count;
    ScheduleTable* schedule_tables; int schedule_table_count;
} ParseResult;
```

## LDF 解析 API（LDFAnalysis.h）

| 函数 | 作用 |
|------|------|
| `parse_ldf_file(path, &result)` | 解析 LDF 文件填 ParseResult |
| `add_node/add_frame/add_signal/add_schedule_table` | 解析时填充 |
| `find_frame_id_by_name(result, name)` | 按帧名查 ID |
| `extract_node_frame_info(result, node, ...)` | 提取节点发布的帧 |
| `extract_frame_signal_info(result, node, ...)` | 提取帧的信号 |
| `extract_schedule(result, &schedule_ID, &schedule_Delay)` | 提取调度表为 ID+延时向量 |
| `free_parse_result(&result)` | 释放 |

## LIN 测试动作库（test_function.h）

### 唤醒与睡眠
| 函数 | 作用 |
|------|------|
| `WakeUpDUT()` | 唤醒 DUT |
| `DUTSleep()` | DUT 进睡眠 |
| `WakeUp()` | LIN 总线唤醒 |
| `LinGoSleep()` | LIN 进睡眠 |
| `LINSendSleepCmd()` | 发送 Sleep 命令 |
| `LINModeSet(mode)` | 设置 LIN 模式 |

### 通信校验
| 函数 | 作用 |
|------|------|
| `CheckLINMasterCom(TestCH)` | 校验主节点通信 |
| `CheckLIN_CS_Classic(TestCH, state)` | Classic 校验和校验 |
| `CheckLIN_CS_Enhanced(TestCH)` | Enhanced 校验和校验 |
| `CheckLINMsgDLC(TestCH)` | 报文 DLC 校验 |
| `CheckLINHeader(A, B)` | 报文头校验 |

### 调度表校验
| 函数 | 作用 |
|------|------|
| `CheckScheduleOrder(TestCH)` | 调度表顺序校验 |
| `CheckScheduleSlot(TestCH)` | 调度表时隙校验 |

### RBS（残余总线仿真）
| 函数 | 作用 |
|------|------|
| `StartAllLINRBS()` | 启动所有 LIN RBS |
| `CloseAllLINRBS()` | 关闭所有 LIN RBS |
| `LINCTBoardConfig(diagramindex)` | LIN CT 板配置 |
| `LDFAnalysis()` | 调用 LDF 解析 |

### 故障注入
| 函数 | 作用 |
|------|------|
| `SimulateLINResponse_MissingResp(list, status)` | 模拟从节点缺失响应 |
| `SimulateLINResponse_FaultResp(list, status)` | 模拟错误响应 |
| `SimulateLINResponse_FaultTransmitData(list, status)` | 模拟错误传输数据 |

`LINDTC` 类型承载故障注入的目标帧/节点列表。

## LIN DTC（故障码）

LIN 故障注入测试用 `LINDTC` 结构描述故障项，配合 `SimulateLINResponse_*` 函数。典型故障：
- 从节点无响应（Missing Response）
- 错误响应（Fault Response）
- 错误数据（Fault Transmit Data）
- 校验和错误
- 报文头错误

## 测试关注点

- 主从通信时序（调度表顺序/时隙）
- 唤醒/睡眠流程
- Classic vs Enhanced 校验和
- 报文 DLC 正确性
- 从节点故障检测（缺失响应/错误数据）
- 总线速率（19200 bps）
- RBS 仿真启停

## 工程组织（VS_LINCOM）

VS_LINCOM 工程结构与 NM_TEST 类似（`common/` 五件套）：

```
VS_LINCOM/
├── VS_LINCOM.cpp / VS_LINCOMExtern.cpp   # 入口 + 导出
├── common/
│   ├── test_include.h         # 总入口，宏选模块 #define Group_LINCOM_ON
│   ├── test_function.h        # 100KB：LIN 测试动作库
│   ├── test_parameter.h       # 参数（MAX_TEST_CHANL=5 LIN 通道）
│   ├── test_devices.h         # 设备控制
│   ├── general_function.h     # 通用函数
│   ├── LDFAnalysis.h          # ★ LDF 解析器
│   ├── GenerateKeyExOpt.h     # 密钥生成（诊断用，非 LIN）
│   └── FunctionDeclaration.h  # 函数声明
└── mp.def
```

`test_include.h` 用宏选择编译模块（`Group_LINCOM_ON` / `Group_CANCOM_ON` / `Group_CANNM_ON` / `Group_CANDIAG_ON` / `Group_CANSWDL_ON`）——同一工程可裁剪为不同测试域。

## 与 TSMaster MCP

LIN 数据库加载与查询用 `tsmaster_database_manage`（支持 .ldf）。LIN 报文收发、调度表控制通过 MiniProgram RBS（`StartAllLINRBS`）或 MCP `tsmaster_bus_simulation`。

## 编码注意

- LDFAnalysis.h 中含 `_strdup` 自定义实现与 GBK 注释（乱码），结构定义清晰可读。
- LIN 通道数 `MAX_TEST_CHANL=5`，按项目调整。
- LDF 解析结果在内存，换项目需重新 `parse_ldf_file`。
