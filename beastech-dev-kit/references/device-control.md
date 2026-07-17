# 设备控制（DeviceControl 硬件抽象）

源自 VT 工程 `02_Library/00_StandardLibrary/DeviceControl/` 与 `Geely3.5_ComDTC/02_Device/`。beastech 的 `test_devices.h` 只覆盖电源+继电器,VT 的硬件抽象更完善。本文件记设备控制范式,供 beastech 工程化借鉴。

## 设备分类

| 设备类型 | 工具名 | 作用 |
|----------|--------|------|
| **BOB**（Break-Out-Box） | BOB / uBOB | 信号通断控制（CANH/CANLO/总线断） |
| **DRB**（继电器箱） | DRB | CAN/LIN/FlexRay 通断、接地切换、示波器/LIN 万用表接入 |
| **PSRB**（可编程固态继电器） | PSRB | 节点接地/偏移接地/CAN 应力 |
| **VH6501** | VH6501 | Vector 总线干扰仪（故障注入） |
| **VH1150** | VH1150 | Vector 总线干扰仪 |
| **VT** | VT | Vector VT 板卡 |
| **PCS_DSOX3034A** | PCS | 安捷伦示波器（电压/位时间测量） |
| **PMM_FLUKE8846A** | PMM | 万用表 |
| **PPS_SPPS_N6700C** | PPS | 可编程电源（电压/电流/功率设置） |
| **IPClient** | IPClient | 以太网客户端 |

## BOB 控制（uBOB.cin）

| 函数 | 作用 |
|------|------|
| `BOBReset()` | BOB 复位 |
| `BusBreak(enum T_mode sw)` | 总线断开/接通 |
| `CANHO(enum T_mode sw)` | CAN High 断开/接通 |
| `CANLO(enum T_mode sw)` | CAN Low 断开/接通 |

`enum T_mode { T_off, T_on }` 控制通断状态。

## DRB 控制（DRB.cin）

DRB 提供多路继电器,按总线类型连接/断开:
- `OSC_CAN_Connect()` / `OSC_CAN_DisConnect()` — 示波器接 CAN
- `OSC_LIN_Connect()` / `OSC_LIN_DisConnect()` — 示波器接 LIN
- `OSC_FR_Connect()` / `OSC_FR_DisConnect()` — 示波器接 FlexRay
- `REF_Node_GND()` — 节点参考地
- `REF_SYS_GND()` — 系统参考地
- `MM_LIN_DisConnect()` / `MM_LIN_Connect()` — 万用表接 LIN

## PSRB 控制（PSRB.cin）

- `PSRB_InitConfig()` — 初始化
- `Node_GND_Connect()` / `Node_GND_DisConnect()` — 节点接地
- `Offset_GND_Connect()` / `Offset_GND_DisConnect()` — 偏移接地
- `Stress_DRB_CANL_Connect()` / `Stress_DRB_CANL_DisConnect()` — CANL 应力注入
- `Stress_DRB_CANH_Connect()` / `Stress_DRB_CANH_DisConnect()` — CANH 应力注入

## PPS 可编程电源（PPS.cin）

- `PPS_InitConfig()` — 初始化
- `PPS_OutputStsSet(enum PPS_OutputSts Sts)` — 输出状态
- `PPS_Shift_OutputStsSet(enum PPS_OutputSts Sts)` — 换挡输出状态
- `PPS_Shift_VoltageSet(float Volt)` / `PPS_Shift_CurrentSet(float Curr)` / `PPS_Shift_PowerSet(float Volt, float Curr)` — 换挡电压/电流/功率
- `PPS_VoltageSet(float Volt)` / `PPS_CurrentSet(float Curr)` — 电压/电流设置

## PCS 示波器（PCS.cin）

- `PCS_InitConfig()` — 初始化
- `PCS_ReceiveDataStg(char Src[])` — 接收数据
- `PCS_MaxVoltMeasure()` / `PCS_MinVoltMeasure()` — 最大/最小电压测量
- `PCS_TestVoltage()` — 电压测试
- `PCS_TestVoltage_DUT()` — DUT 电压测试
- `PCS_TestBitTime(int times, int wakeupIndex)` — 位时间测试

## 设备配置文件组织

ComDTC 工程的结构（推荐 beastech 借鉴）:

```
02_Device/
├── Bench/                      # 台架配置（多设备组合）
│   ├── DeviceConfig_Bench.cin  # 台架总配置
│   └── 02_PreCmd/              # 预命令（初始化/通断）
│       ├── PreCmd.cin
│       ├── PreCmd_SysVar.cin
│       ├── BOB_PreCmd.cin
│       ├── DRB.cin / DRB_SysVar.cin
│       ├── ENV.cin
│       ├── PSRB.cin
│       ├── PPS.cin / PPS_SysVar.cin
│       ├── PCS.cin / PCS_SysVar.cin
│       ├── Agilennt_PCS_PreCmd_SysVar.cin
│       └── VH6501.cin
├── BOB/DeviceConfig_BOB.cin
├── BOB/uBOB.cin
├── VH1150/DeviceConfig.cin + VH1150.cin
├── VH6501.cin
└── VH6501_null.cin  (空实现占位,便于无设备时编译)
```

## 与 beastech 对照

beastech 的 `test_devices.h`(78KB) 含电源/继电器控制,但未分层。建议:

1. 按设备类型分文件（BOB.cin/DRB.cin/PSRB.cin/PPS.cin）
2. 加 `*_SysVar.cin` 存放设备相关系统变量
3. 用 `*_null.cin` 作空实现占位（CI/CD 无设备时可用）
4. 加 `DeviceConfig_*.cin` 做设备组合配置

## 测试用例中的设备控制

```capl
// 典型用例: 信号通断 + 测量
testcase CANLO_Open_Circuit()
{
  PPS_VoltageSet(12.0);          // 电源 12V
  BOBReset();
  CANHO(T_on);                    // CANH 接通
  CANLO(T_off);                   // CANL 断开
  PCS_TestVoltage_DUT();          // DUT 电压测量
  CANLO(T_on);                    // 恢复
}
```

## 故障注入（VH6501/VH1150）

Vector 总线干扰仪可注入:
- 显性/隐性位注入
- 错误帧
- 位篡改
- 总线断开/短路
- 延迟抖动

通过 Vector 专用驱动控制,具体 API 查 VH6501.cin（需 Vector 硬件）。beastech 的 `tsdisturb.h` 是类似定位,但 API 风格不同。
