# VT/CAPL 范本工程索引

8 个 Vector vTESTstudio + CANoe 工程的明文 CAPL 源码范本（.can/.cin），换电脑后随身可用。源自 `d:/VTProject/`，已排除加密副本（.canencr/.cinencr）和构建产物。

> 这些是**参考范本**，不是可直接编译的工程（缺 .cbf/.tse/.vsysvar 等配置）。用于查 CAPL 写法、UDS/TP 封装、设备控制实现。要跑需在 CANoe 里重建工程。

## 8 个工程

| 目录 | 域 | 体量 | 核心范本 |
|------|-----|------|---------|
| `GEEA3.0_DiagnosticService/` | UDS+TP | 1.8MB/80文件 | ★ 最全。UDS_Service_1.5.cin(208KB)、TestCase.can(849KB)、01_TP/(CAN/IP/LIN/COM_TP)、SimSigGroup.cin、DeviceControl 全套 |
| `Geely3.5_ComDTC/` | DTC+设备 | 0.7MB/78文件 | ★ 0加密。UDS_Service_1.0.cin、02_Device/(BOB/DRB/VH6501/PPS/PCS/PSRB) 全套硬件控制 |
| `SignalRouting/` | 信号路由 | 4.4MB/5文件 | Test_Input.cin 路由测试输入报文清单（项目数据，参考组织方式） |
| `GEELY_CANCANFD_COM/` | 通信矩阵 | 0.6MB/4文件 | COMMatrix.cin(565KB) 通信矩阵 |
| `DiagRouting_Upperlink_LIN/` | LIN 路由 | 0.25MB/27文件 | UDS_Client.cin/UDS_Server.cin、Busoff.can、SIM_LIN.can、TaskScheduling.cin |
| `DiagRouting_Upperlink_X/` | DoIP 路由 | 54KB/18文件 | 按网段分节点目录（01_ZCUD_CAN1/SIM_CAN.can 等）、TestParameter.cin |
| `GEEA3.0_CC_Slave/` | CC+E2E | 59KB/6文件 | Parameter.cin、CCPInfo.cin、ParameterOfSigGroups.cin、UsageMode_Sim.can |
| `Geely_NM_Device/` | NM 设备 | 25KB/3文件 | Context.cin、ECUInfor.cin、VH1150.cin |

## 关键范本文件速查

### UDS 服务封装（写 CAPL UDS 用例必查）
- `GEEA3.0_DiagnosticService/.../02_UDS/UDS_Service_1.5.cin` — SID10~SID3D 全套封装，参数带 AddressMode/Sprmib 枚举
- `GEEA3.0_DiagnosticService/.../02_UDS/UDS_Functions_1.0.cin` — CheckPosResponse/CheckNegResponse/CheckNonResponse 响应校验
- `GEEA3.0_DiagnosticService/.../02_UDS/0x29Function.cin` — 0x29 认证
- `Geely3.5_ComDTC/.../05_UDS/UDS_Service_1.0.cin` — 另一版 UDS 封装（含 SID31_RoutineControl_EraseMemory）

### TP 传输层（分总线）
- `GEEA3.0_DiagnosticService/.../01_TP/COM_TP.cin` — 公共 TP 接口（TP_BusSet/TP_Send/TP_GetReceivedData）
- `GEEA3.0_DiagnosticService/.../01_TP/CAN_TP.can` / `IP_TP.can` / `LIN_TP.can` — 分总线 TP
- `Geely3.5_ComDTC/.../04_TP/` — 更全：canfdTP/canTP/doipTP/linTP/FrTP/ipTP

### 设备控制（硬件抽象）
- `Geely3.5_ComDTC/.../02_Device/Bench/02_PreCmd/` — BOB_PreCmd/DRB/ENV/PSRB/PPS/PCS/VH6501/VT 全套
- `GEEA3.0_DiagnosticService/.../DeviceControl/` — BOB/DRB/VH1150/VH6501/示波器/电源/IPClient

### 路由测试
- `DiagRouting_Upperlink_LIN/.../UDS_Client.cin` / `UDS_Server.cin` — Client/Server 范式
- `DiagRouting_Upperlink_X/.../04_Node/<网段>/SIM_CAN.can` — 按网段分节点
- `SignalRouting/.../Test_Input.cin` — 路由输入报文清单声明式

### 用例主体
- `GEEA3.0_DiagnosticService/.../03_Script/TestCase.can`(849KB) — 完整用例集
- `GEEA3.0_DiagnosticService/.../03_Script/Parameters.cin` / `UserDefine.cin` — 参数/用户配置

## 与 references 的关系

这些范本是 `references/vt-capl-library.md` / `transport-protocol.md` / `device-control.md` / `vt-routing-and-com-testing.md` 里描述的 API 和范式的**原始源码**。reference 记结论，范本给完整实现。写 CAPL 时两者配合用：
- 查 API 签名/用法 → reference
- 看完整实现/复制代码片段 → 本目录范本

## 使用注意

- 文件多为 GBK 编码（`/*@!Encoding:936*/`），中文注释可能乱码，靠上下文辨认
- 这是 Vector CAPL 语法，不是 TSMaster C/C++——写 TSMaster 用例时只参考协议逻辑，不直接复制语法
- 具体报文 ID/DID/NRC 值是项目数据，参考其组织方式，不照抄值
- 5 个加密工程（SWDL/E2E/网关等）的核心逻辑不在此，见 `references/vt-encrypted-summary.md`
