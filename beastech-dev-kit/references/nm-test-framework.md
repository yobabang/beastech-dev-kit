# NM 测试工程框架（NM_TEST 实战范式）

源自 `GEEA4.0_CC_E2E_NM/NM_TEST/`。这是比 DIAG_BENCH 单用例模式更成熟的**数据驱动、多通道遍历** NM 测试框架。写 NM 用例、解析 NM 工程时查本文件。

## 工程结构（与 DIAG_BENCH 的差异）

```
NM_TEST/
├── NM_TEST.cpp              # 主入口：on_canfd_rx/tx/pretx 回调 + step() 周期函数(5ms)
├── NM_TESTExtern.cpp        # 导出函数
├── Database.h/.cpp          # ★ 696KB 真实数据库实例（非空壳，含报文/信号/PNC 映射）
├── mp.def                   # LIBRARY NM_TEST + 4 导出
└── common/
    ├── test_include.h       # 总入口，聚合下面 5 个
    ├── test_list.h          # ★ 264KB：所有 TC 函数实现（TC_1023208...）
    ├── test_function.h      # ★ 227KB：NM 测试动作库（send_nm_msg/check_com/KL30...）
    ├── test_parameter.h     # ★ 参数与全局状态（NetworkCluster/BUS_configuration/config 读取）
    ├── test_other_function.h# 辅助函数
    ├── test_devices.h       # 设备控制（电源/继电器）
    ├── config.ini           # ★ 项目数据：每网段 PNC/TX/RX/NM node ID/唤醒能力
    ├── NM_TEST.py           # ★ Python runner：ctypes 绑定每个 TC 函数
    ├── parseArxml.py        # 从 ARXML 解析数据库生成配置
    ├── NM_TEST.mp/.pdb/.lib # 编译产物 + MiniProgram 包
    └── rtlUIDiagnostics.bpl # 运行时依赖
```

**关键差异**：DIAG_BENCH 用 `TestCase_CAN_*.h` + `test_case_diag_api.h`（SID 封装）；NM_TEST 用 `common/test_*.h` 五件套 + `config.ini` 数据驱动。两套范式不互斥——NM 用例不走 UDS，走总线仿真 + 报文收发 + 状态观测。

## 数据驱动架构

### config.ini（项目数据，与代码分离）

```ini
[AllChannel]
ecu_name=ZCUD
all_test_channel=ZCU_CANFD2,ZCU_CANFD1,ChassisCANFD2,ZCUD_CAN1,PropulsionCAN,ChassisCAN1
ecu_type=1                    ; 0=non_pnc_gateway 1=pnc_gateway 2=passive_node
ecu_address=0x1601
can_channel_count=7
if_kl30_power=0
wakeup_coding_network=0x02BD

[<网段名>]                     ; 每个网段一节
pncgatewaytype=2              ; 0=no_pnc 1=passive 2=active
pnccode=25,29,36              ; 该网段相关的 PNC bit
tx_msg_no_map=26,81,87,...    ; 不映射 PNC 的 TX 报文 ID 列表
tx_msg_pnc25=101,270,...      ; PNC25 控制的 TX 报文 ID 列表
rx_msg_no_map=64
txnmpduid=1286                ; 该网段 NM PDU 的 TX 报文 ID
nm_node_id=6                  ; NM Source Node ID（NM PDU byte0）
support_network_wakeup=1
support_hardware_wakeup=0
if_sleep_power_on=1           ; 上电后是否处于休眠
if_can_be_sleep_with_power=1  ; 带电能否睡眠
if_support_vfc=1
if_support_i_pdu_control=1
```

`test_parameter.h` 在启动时读 config.ini 填充 `bus_configuration[]` 数组与全局参数。**改项目只改 ini，不改代码**。

### NetworkCluster map（网段名→通道）

```cpp
std::map<std::string, int> NetworkCluster = {
    {"ZCU_CANFD2",CH2}, {"ZCU_CANFD1",CH3}, {"ChassisCANFD2",CH4},
    {"ZCUD_CAN1",CH5},  {"PropulsionCAN",CH6}, {"ChassisCAN1",CH7},
    {"ZCUD_CANFD3",CH8},{"ZCUD_CANFD4",CH9}, {"BodyExposedCAN",CH10}
};
```

### BUS_configuration 结构体（每通道能力）

```cpp
struct BUS_configuration {
    bool if_support_network_wakeup;      // 支持 NM 唤醒
    u16  dut_send_nm_msg_id;
    u8  nm_node_id;                       // NM PDU byte0 = source node ID
    bool if_support_hardware_wakeup;      // 硬线唤醒
    bool if_sleep_state_with_power_supply_on;
    bool if_support_VFC;                  // 关联 VFC（前提：支持硬线唤醒）
    bool if_support_I_PDU_control;        // IPDU 控制
    bool if_recv_diag_msg;                // 是否诊断 CAN
    bool CanNmPnEnabled;                  // 该通道是否启用 PN
    bool if_keep_waking_hardware;
    enum PNC_gateway_type bus_pnc_gateway_type;  // no_pnc/passive/active
    u8   relevant_pnc[32];                // 相关 PNC bit（升序，如 PNC25/29/36）
    u8   wakeup_way_count;
    enum All_wakeup_way all_wakeup_way[8];        // network/hardware1/hardware2/diag/other
    u8   hardware_wakeup_count;
    enum All_wakeup_way hardware_wakeup_way[8];
    u8   rx_pnc[32];
    // ...
};
```

## 用例范式（与 DIAG_BENCH 不同）

NM 用例**不走 UDS**，走总线仿真 + 报文观测。返回 `current_test_result`。

```cpp
s32 TC_1023208_CAN_NM_Frame_ID_in_Range_wakeup_and_down()
{
    pre_test(__func__);                    // 前置（替代 TestPrecondition）
    u8 m;
    for (m = 1; m < bus_count; m++) {      // ★ 遍历所有网段
        ClearAllVector();
        // 仅对"支持网络唤醒且能带电睡眠"的网段测试
        if (bus_configuration[m].if_can_be_sleep_with_power == 1
         && bus_configuration[m].if_support_network_wakeup == 1) {
            InfoStep = 0;
            TestStep("++++当前通道为%s(CAN%d)++++", bus_configuration[m].node_name, m+1);

            u16 test_id[5] = {0x500,0x50F,0x51F,0x52F,0x53F};  // NM 报文 ID 测试矩阵
            for (i = 0; i < 5; i++) {
                TestStep("关闭网络仿真，上电，检查网段休眠");
                close_simulate_network_and_begin_power_on_and_check(m);

                TestStep("发送ID 0x%X 的NM报文，PNI置位，所有PNC置位，周期 CanNmMsgCycleTime", test_id[i]);
                send_nm_msg_pni_and_all_pnc_is1(m, test_id[i]);
                res[1] = check_com(m, 0, 3000);          // 3s 内检查开始通信

                TestStep("持续仿真 NM 报文 CanNmPnResetTime*3，检查持续通信");
                res[2] = check_com(m, 1, CanNmPnResetTime * 3);

                TestStep("停止仿真，10s 内应停止通信");
                stop_send_nm_msg();
                res[3] = check_com(m, 2, 10000);

                TestStep("等待 CanNmWaitBusSleepTime+2s，KL30 OFF");
                wait(CanNmWaitBusSleepTime + 2000);
                KL30(off);
            }
        } else {
            TestInfoPrintWarning("网段%s 不满足测试条件", bus_configuration[m].node_name);
        }
    }
    post_test();                           // 后置（替代 TestPostCondition）
    return current_test_result;
}
```

### 范式要素

| 要素 | DIAG_BENCH | NM_TEST |
|------|-----------|---------|
| 前置/后置 | `TestPrecondition()`/`TestPostCondition()` | `pre_test(__func__)`/`post_test()` |
| 步骤 | `TestReport_HTML_StepDescription` | `TestStep(fmt,...)` + `InfoStep` 计数 |
| 失败标志 | `resultFlag` | `current_test_result` |
| 遍历 | 单用例 | `for (m=1; m<bus_count; m++)` 多通道 |
| 数据驱动 | 硬编码 | `bus_configuration[m]` + config.ini |
| 协议 | UDS 诊断 | 总线仿真 + 报文观测 |

## NM 测试动作库（test_function.h）

| 函数 | 作用 |
|------|------|
| `send_nm_msg_pni_and_all_pnc_is1(bus, id)` | 发 NM 报文，PNI 置位、所有 PNC 置 1 |
| `stop_send_nm_msg()` | 停止 NM 仿真 |
| `check_com(bus, mode, timeout_ms)` | 检查通信状态，mode: 0=开始通信 1=持续通信 2=停止通信 |
| `close_simulate_network_and_begin_power_on_and_check(bus)` | 关网络仿真 + 上电 + 检查休眠 |
| `KL30(on/off)` | KL30 电源控制 |
| `ClearAllVector()` | 清空报文缓存向量 |
| `TestStep(fmt,...)` | 步骤记录 |
| `TestInfoPrintWarning(fmt,...)` | 警告 |
| `pre_test(__func__)` / `post_test()` | 用例前后置 |
| `output_openissue(bus, __func__)` | 登记缺陷 |

## PNC（部分网络）核心

GEEA4.0 NM 的关键特性。PNC 是 NM PDU 中的 bit，控制一组报文的收发使能。

- **PNC bit**：如 PNC25/29/36/38，分布在 NM PDU 的 byte2–byte5（`pnc_begin_byte=2, pnc_end_byte=5`）
- **PNC 网关类型**：`no_pnc_gateway` / `passive_pnc_gateway` / `active_pnc_gateway`
- **报文-PNC 映射**：config.ini 的 `tx_msg_pnc25=101,270,...` 表示这些报文受 PNC25 控制
- **PNI（Partial Network Information）**：NM PDU control bit vector 中的位，标识是否有 PNC 请求
- 测试时 `send_nm_msg_pni_and_all_pnc_is1` 把所有 PNC 置 1 唤醒全部部分网络

## NM 报文 ID 范围

NM 报文 ID 范围（如 0x500–0x53F）用于唤醒测试矩阵。`test_id[5]={0x500,0x50F,0x51F,0x52F,0x53F}` 覆盖范围内边界点。超范围 ID（如 0x540+）应不唤醒。

## Python runner（NM_TEST.py）

通过 ctypes 把每个 TC 函数绑定为可调用对象，外部 Python 可直接调 `TC_xxx()`。结构：

```python
from ctypes import *
# 类型定义 u8/pu8/s32/...
_TC_1023208_CAN_NM_Frame_ID_in_Range_wakeup_and_down = CFUNCTYPE(s32)
# ... 每个 TC 一个 CFUNCTYPE
```

用途：CI/CD 编排、批量执行、与外部测试管理系统集成。`parseArxml.py` 从 ARXML 数据库反生成 config.ini 的报文/PNC 映射。

## 主入口回调（NM_TEST.cpp）

```cpp
void on_canfd_rx_NewOn_CAN_FD_Rx1(const TCANFD* ACANFD)  { On_RX_CAN_Msg(ACANFD); }   // 收
void on_canfd_tx_NewOn_CAN_FD_Tx1(const TCANFD* ACANFD)  { On_TX_CAN_Msg(ACANFD); }   // 发完成
void on_canfd_pretx_NewOn_CAN_FD_PreTx1(const PCANFD ACANFD) { /* 预发送 */ }          // 预发送
void step(void) { /* 5ms 周期任务 */ }                                                    // 周期
```

所有回调包 `try/catch`，异常时 `log_nok("CRASH..."); app.terminate_application()`。

## 用例命名（NM.json 清单）

`TC_<ID>_CAN_NM_<场景>`，常见系列：
- 1023208–1023211：NM 报文 ID 范围内/外的唤醒与保持
- 1023212–1023213：NM PDU 网络/硬件唤醒
- 1023214–1023215：PNI / Active Wakeup Bit 识别
- 1023216–1023217：Repeat Message Request（Ready Sleep / Normal Operation）
- 1023218–1023222：PNC 掩码内/外的唤醒、保持、接收

## 测试关注点

- 唤醒源：网络唤醒 vs 硬线唤醒（hardware1/hardware2）
- NM 报文 ID 范围边界
- PNI / Active Wakeup Bit 识别
- Repeat Message Request 在不同状态下的行为
- PNC 掩码匹配（in mask 唤醒，out of mask 不唤醒）
- 通信起始/持续/停止时序（CanNmMsgCycleTime / CanNmPnResetTime / CanNmWaitBusSleepTime）
- 带电睡眠能力
- VFC（Vehicle Function Control）关联
- IPDU 控制

## 编码注意

- `Database.h` 是 696KB 实例（非空壳），含真实报文/信号/PNC 映射，由 `parseArxml.py` 从 ARXML 生成——换项目要重新生成。
- config.ini 是项目数据入口，改项目优先改 ini。
- NM 用例代码与项目数据强分离：代码读 `bus_configuration[]`，数据在 ini。
