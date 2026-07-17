# 路由测试与通信矩阵（VT 范式）

源自 VT 4 个工程:SignalRouting / CANCANFD_COM / DiagRouting_LIN / DiagRouting_X。这些工程的养料偏"工程范式"（如何组织路由/通信测试）而非协议事实。

## 信号路由测试（SignalRouting_V2.1）

### 工程结构
```
SignalRouting_V2.1 - Encr/
├── 01_DB/                    # 数据库（按编号分 1/2/3/4）
│   └── <n>/Test_Input.cin    # ★ 路由测试输入（报文清单+信号）
├── 02_Library/
├── 03_TestModel&Report/
├── 04_Pannel/
├── 05_Environment/
├── 06_Project/
└── 07_Report&Log/
```

### Test_Input.cin 结构
路由测试输入是 CAPL `variables` 块,声明源/目标网段和报文清单:
```capl
variables {
  const int Event = 1002;
  char NodeName[20] = "ZCUD5";
  // 目标网段报文清单
  LinFrame ZCUD_LIN1.0x36 des_DdsZcud_Lin1Fr01 = {dlc=7};
  LinFrame CEM_LIN3.0x20 des_OhcCem_Lin3Fr01 = {dlc=7};
  // ...
  // 源网段报文清单（注释标注）
}
```

### 范式要点
- **源/目标网段分离**:Test_Input.cin 注释标注源网段与目标网段
- **按网段分目录**:01_DB/1~4 每个子目录一套路由场景的输入
- **报文清单声明式**:用 CAPL `LinFrame`/`message` 声明预期报文,DLC 内联
- **测试输入与逻辑分离**:Test_Input.cin 只存数据,测试逻辑在 03_TestModel

> 具体报文 ID/信号是项目数据,不固化。借鉴的是"输入数据声明式 + 按场景分目录"的组织方式。

## 通信矩阵测试（CANCANFD_COM_Device_V2.0.7）

### 工程结构
```
GEELY3.0&3.5_CANCANFD_COM_Device_V2.0.7_Encr/
├── 01_Input/
├── 02_Library/
│   └── 00_StandardLibrary/
├── 03_Script/
│   ├── code/                 # C# 代码（codefactory 生成）
│   └── xml/                  # 测试配置 XML
├── COMMatrix.cin (565KB)     # ★ 通信矩阵
├── Parameters.cin
└── SystemInformation.cin
```

### COMMatrix.cin
565KB 通信矩阵,声明所有 CAN/CANFD 报文的周期/DLC/信号映射。是项目数据,不固化。

### 范式要点
- **通信矩阵集中**:所有报文定义集中在一个 .cin,便于维护
- **codefactory 生成**:`03_Script/code/` 下 C# 代码由工具自动生成,手改 `.cin` 数据
- **XML 测试配置**:测试参数用 XML 管理

## 上行诊断路由（DiagRouting_Upperlink）

两个变体:LIN 版与以太网(X)版。测试 DUT 作为网关,将上行诊断请求路由到下行总线。

### DiagRouting_LIN 结构（明文 19 can + 8 cin）
```
03_Script/
├── TestParameter.cin / TestParameter-LIN.cin   # 测试参数
├── Context.cin                                   # 上下文
├── TaskScheduling.cin                            # 任务调度
├── UDS_Client.cin                                # ★ 诊断客户端
├── UDS_Server.cin                                # 诊断服务端（仿真 ECU）
├── Busoff.can                                    # Busoff 测试
├── SIM_LIN.can                                   # LIN 仿真
└── SwitchMode.can                                # 模式切换
04_Node/
└── UDS_Server.cin                                # 节点端服务端
```

### UDS_Client/Server 范式
- `UDS_Client.cin` — 测试端发诊断请求,`CheckECU_Response(double P2, struct ServiceVars testerReq, struct ServiceVars ecuResp)` 校验响应
- `UDS_Server.cin` — 仿真下行总线的 ECU,响应诊断请求
- `struct ServiceVars` 封装请求/响应变量

### DiagRouting_X 结构（以太网版）
```
01_Input/TestParameter.cin
04_Node/
├── 01_ZCUD_CAN1/SIM_CAN.can       # 每个网段一个仿真节点
├── 02_ChassisCAN1/SIM_CAN.can
├── 03_ChassisCANFD2/SIM_CAN.can
├── 04_ZCUD_CANFD3/SIM_CAN.can
├── 05_ZCUD_CANFD4/SIM_CAN.can
├── 06_BodyExposedCAN/SIM_CAN.can
├── Demo_CAN/SIM_CAN.can
├── Demo_CANFD/SIM_CAN.can
└── RoutingCAN_NoDiag/NoDiagCAN.can  # 无诊断路由 CAN
```

### 范式要点
- **按网段分节点目录**:`04_Node/<序号>_<网段名>/SIM_CAN.can`,每网段独立仿真节点
- **Client/Server 分离**:测试端 Client 发请求,仿真端 Server 响应
- **任务调度**:`TaskScheduling.cin` 编排多网段测试顺序
- **模式切换**:`SwitchMode.can` 处理测试模式切换

## CC 从站测试（GEEA3.0_CC_Slave_V1.0.8-25R2）

```
02_Library/
├── 02_SimEnv/
│   ├── Parameter.cin (24KB)    # 仿真参数
│   └── CCPInfo.cin (24KB)      # CC/E2E 信息
├── 00_MiscConfig/
│   └── PRJconfig.cin (8KB)     # 项目配置
01_CMX/
├── GenerateE2E_CC/
│   └── ParameterOfSigGroups.cin  # E2E/CC 信号组参数
└── UsageMode_VehSpd_Sim.can      # 车速仿真模式
    UsageMode_Sim.can             # 仿真模式
```

### 范式要点
- **CCPInfo.cin** 集中 CC（通信控制）与 E2E 测试信息
- **ParameterOfSigGroups.cin** 管理 E2E 信号组参数（信号名/大小/起始位/校验和）
- **UsageMode** 用 .can 控制仿真模式（如车速仿真）
- **CMX**（Configuration Manager eXtended）目录放生成配置

## NM 设备控制（NM_Device_V1.1.12）

```
03_TestModel&Report/
├── Context.cin (17KB)     # 测试上下文
└── ECUInfor.cin (3KB)     # ECU 信息
02_Library/
└── VH1150.cin (5KB)       # 总线干扰仪
```

NM 设备工程较薄,核心是 Context.cin（测试上下文管理）+ VH1150 故障注入。NM 协议事实已在 [nm-state-machine.md](nm-state-machine.md) 与 [nm-test-framework.md](nm-test-framework.md) 覆盖。

## 通用借鉴点（对 beastech）

1. **数据声明式**:测试输入（报文清单/通信矩阵）用声明式 `variables` 块集中管理,与逻辑分离（beastech 的 config.ini 已是此思路）
2. **按网段分节点目录**:多网段仿真时每网段独立节点文件
3. **Client/Server 分离**:路由测试中测试端与仿真端职责分离
4. **编号目录标准化**:01_Input~07_Report 的组织方式
5. **任务调度独立文件**:多场景测试编排单独管理
6. **UsageMode 模式控制**:用 .can 切换仿真模式

> 这些工程的具体报文 ID、信号名、参数值都是项目数据,不在本库固化。借鉴的是工程组织方法。
