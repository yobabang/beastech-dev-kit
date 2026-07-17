# Beastech Dev Engineer — 能力与边界

> 本文件说明 `beastech-dev-engineer` agent 能做什么、不能做什么、以及各场景下的落地程度。供使用者判断"这个任务交给它能不能成"。

## 一句话定位

面向吉利 GEEA EEA4.0 平台的车载测试开发工程师,工作在 TSMaster MiniProgram SDK 之上。**能从测试需求出发,走 spec→plan→build→test→review 闭环,产出可编译运行的测试用例代码,并通过 TSMaster MCP 驱动执行验证。**

## 能力总览

| 能力域 | 程度 | 说明 |
|--------|------|------|
| TSMaster 用例开发 | ✅ 完整闭环 | 需求→代码→编译→运行→验证,有 MCP 执行通道 |
| VT/CAPL 用例开发 | ⚠️ 能写不能跑 | 能生成 CAPL 代码,编译运行验证需人工在 CANoe |
| 协议知识 | ✅ 随身 | UDS/SWDL/NM/CC/E2E/LIN/TP/DoIP 协议事实脱离源工程可用 |
| 工程解析与新建 | ✅ 随身 | SmokeTest 主模板 + 16 份 references + VT 范本,换电脑可用 |
| 多 agent 审查 | ✅ | 协议正确性 / 测试质量 / 安全 / 代码质量 四角审查 |
| 具体项目数据 | ⚠️ 现场读 | DID/NRC/pincode/报文ID 不预记忆,从目标工程或 MCP 读 |

## 能做到的事(详细)

### 1. TSMaster 测试用例 — 完整闭环 ✅

从测试需求到验证通过的全链路:

**Specify(澄清)** — 反问澄清 TC_ID、功能域、协议点、期望响应三态(肯定/否定带NRC/无响应)、前后置条件。需求不清不硬写。

**Plan(分解)** — 复杂改动拆可验证小任务,多用例批量先列清单。

**Build(实现)** —
- 查 `references/` 确认协议事实(UDS 服务/NRC/状态机/流程)
- 查 `api-reference.md` 找封装函数(`SIDxx_*` + `CheckPosResponse_*`/`CheckNegResponse`/`CheckNonResponse`)
- 套 `test-case-pattern.md` / `smoketest-framework.md` 范式写 C 代码
- 复用已有封装,不另造诊断调用
- 用例登记到 JSON 清单(CS.json/NM.json)

**Test(验证)** — TSMaster MCP 驱动:
- `tsmaster_coding` program.create → block.set → code.compile 编译
- `tsmaster_measurement` start 连接应用
- `code.run` 跑用例
- `tsmaster_trace_read` / `tsmaster_read_project_file` 读 trace 和系统消息,确认实际响应符合期望

**Review(审查)** — 派 `beastech-test-reviewer`(协议正确性)+ `test-engineer`(测试质量)+ `security-auditor`(诊断安全)+ `code-reviewer`(代码质量)并行审查。

### 2. 协议知识 — 随身携带 ✅

16 份 references 覆盖全协议域,脱离任何源工程成立:

- **UDS**:服务表(SID10~SID3E)+ NRC 表 + 安全访问(0x27 seed/key)
- **SWDL**:SBL 下载流程(0x34/0x36/0x37)
- **NM**:报文格式 + 状态机 + PNC 部分网络 + config.ini 数据驱动框架
- **CC**:0x28 通信控制
- **E2E**:profile/counter/CRC
- **LIN**:协议 + LDF 解析 + 动作库
- **TP**:CAN/CANFD/LIN/FlexRay/DoIP 分总线
- **设备控制**:BOB/DRB/VH6501/电源/示波器 硬件抽象

### 3. 工程解析与新建 — 随身携带 ✅

- **新建工程**:从 `templates/miniprogram-skeleton/`(SmokeTest 主模板)复制,改工程名即可起步。含全局执行器/12域/四态结果/Python 绑定。
- **解析现有工程**:懂 GEEA 工程的文件组织(DIAG_BENCH / NM_TEST / SmokeTest 三套范式)、TSMaster SDK 骨架、VT/CAPL 工程结构。
- **VT 范本**:`templates/capl-resources/vt-examples/` 含 8 个 VT 工程明文 CAPL 源码(219 文件),换电脑随身可用。

### 4. CAPL 代码生成 — 能写 ✅(验证靠人工)

- 随身带 `vt-capl-library.md`(CAPL 语法/UDS 封装签名)+ 219 个真实 CAPL 范本
- 能生成 CAPL 用例代码(UDS 服务调用、TP、设备控制)
- **边界**:CAPL 编译/运行/结果验证需人工在 CANoe 完成,agent 无 CANoe 自动化接口

### 5. 质量保障 — 多 agent 审查 ✅

借鉴 agent-skills `/ship` 并行扇出模式:
- `beastech-test-reviewer` — 协议正确性六轴审查
- `test-engineer` — 测试覆盖度/判定逻辑
- `security-auditor` — 0x27 密钥/pincode 泄露
- `code-reviewer` — 五轴代码审查

## 边界(不能做或受限)

### 硬边界(平台限制,非知识缺失)

| 边界 | 原因 | 变通 |
|------|------|------|
| **CAPL 编译/运行验证** | 无 CANoe MCP,agent 不能驱动 CANoe | CAPL 代码写完,人工在 CANoe 编译运行看结果 |
| **CAPL 5040 函数具体参数** | 函数库未合并进随身库(在远端 github 仓库) | 按需查远端 `d:/claudeProject/beastech-dev-kit/`;常用函数在 VT 范本里有实例 |
| **vECU 控制逻辑** | 按设计不纳入(vECU 本质是控制虚拟 ECU,另议) | LIN 通信知识已保留 |

### 软边界(需条件满足)

| 边界 | 条件 | 不满足时 |
|------|------|---------|
| **TSMaster 编译运行验证** | 需装 TSMaster + VS2022 + 配置 MCP | 只能到代码层,不能跑 |
| **具体项目数据** | DID/NRC/pincode/报文ID 从目标工程或数据库读 | 需提供目标工程或数据库文件 |
| **私有/非标协议** | GEEA 某私有诊断服务不在知识库 | 需提供规范或参考工程,agent 据此延伸 |
| **5 个 VT 加密工程** | SWDL/E2E/网关等核心逻辑加密读不到 | 这些域从 beastech 自有 TSMaster 明文工程补 |
| **硬件在环** | 需实际 HIL 硬件(BOB/DRB/VH6501) | 设备控制代码能写,但跑不起来 |

### 不做的事(设计取舍)

- **不预记忆项目数据**:DID/NRC/pincode 是项目数据,现场读,不固化进知识库(防过时/防错)
- **不臆造协议**:不确定的协议点查 reference 或标注"需核实",绝不猜
- **agent 间不互相调用**:编排由主对话或 `/review` `/ship` 负责(遵循 agent-skills Composition 约定)
- **不直接修改用例做审查**:审查 agent 只给 finding 和建议,修改由 dev-engineer 执行

## 典型场景落地程度

### 场景 1:本地有完整环境(TSMaster + VS2022 + MCP)
**结果:✅ 完整闭环**
需求 → 代码 → 编译 → 运行 → 验证 → 审查,全程 agent 驱动。

### 场景 2:换电脑,装了 TSMaster + VS2022
**结果:✅ 完整闭环**
随身库(13M)含模板+协议+范本,换电脑照常工作。

### 场景 3:换电脑,没装 TSMaster/VS
**结果:⚠️ 只到代码**
能写用例代码,但不能编译运行验证。代码本身是正确范式,装好环境后可直接编译。

### 场景 4:开发 CAPL(VT)用例
**结果:⚠️ 能写不能跑**
能生成 CAPL 代码(有范本),但编译运行验证需人工在 CANoe。TSMaster 侧才有 MCP 闭环。

### 场景 5:协议点不在知识库
**结果:⚠️ 需输入**
agent 会说"这个协议点我不确定",需你提供规范或参考工程。agent 能基于输入快速延伸。

## 能力上限由什么决定

不是协议知识堆叠(已够全),而是三个结构性因素:

1. **方法论骨架** — spec→plan→build→test→review 闭环(来自 agent-skills),保证产出工程级可靠,而非"能跑就行"
2. **多 agent 审查** — 四角并行审查 catch 单点盲区,质量不靠单点判断
3. **质量护栏** — doubt-driven(对抗式自检)+ source-driven(溯源)+ observability(可观测),防臆造防假设

这三者 × 领域知识(16 references + 模板 + VT 范本)= agent 的真正上限。

## 资产清单(随身携带)

```
~/.claude/
├── agents/
│   ├── beastech-dev-engineer.md      # 执行角色(95行,方法论+护栏+MCP)
│   └── beastech-test-reviewer.md     # 审查角色(99行,协议六轴)
└── skills/beastech-dev-kit/          # 13M 知识库
    ├── SKILL.md                       # 入口+导航+喂养路由
    ├── CAPABILITIES.md                # 本文件
    ├── references/  (16 份)           # 协议事实+API+工程范式
    ├── templates/
    │   ├── miniprogram-skeleton/      # SmokeTest 主模板(可编译)
    │   ├── miniprogram-skeleton-diag-bench-legacy/  # 旧模板备用
    │   └── capl-resources/vt-examples/  # 8 工程明文 CAPL 范本(219文件)
    └── assets/sample-test-case.h      # 范本用例
```

## 前置条件清单

要发挥完整能力,目标机器需具备:

| 条件 | 用途 | 必需性 |
|------|------|--------|
| TSMaster 安装 | MiniProgram 运行时 + MCP 服务器 | TSMaster 闭环必需 |
| VS2022(v143 + C++17) | 编译 MiniProgram dll | TSMaster 编译必需 |
| TSMaster MCP 配置 | agent 驱动执行 | TSMaster 自动化必需 |
| CANoe + Vector 硬件 | CAPL 编译运行 | CAPL 验证必需(写代码不需要) |
| 目标工程/数据库 | 查 DID/NRC/报文等项目数据 | 具体用例必需 |
| HIL 硬件 | 设备控制类用例 | 硬件在环用例必需 |

## 版本

- 知识库:16 references + SmokeTest 模板 + VT 8 工程范本
- agent:beastech-dev-engineer + beastech-test-reviewer
- 方法论:agent-skills 生命周期闭环 + 质量护栏 + 多 agent 审查
- 执行底座:TSMaster MCP(catalog-first 契约)
