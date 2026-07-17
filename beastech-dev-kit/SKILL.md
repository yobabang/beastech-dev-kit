---
name: beastech-dev-kit
description: Beastech (GEEA EEA4.0 / TSMaster) 测试开发工具包 — 脱离源工程即可生产测试用例的随身知识库与 SDK 模板。包含可复制的 MiniProgram 骨架、TSMaster C API 速查、UDS/SWDL/NM/CC/E2E 协议事实、用例范式范本。当要在 TSMaster 上新建/扩展 GEEA 诊断/通信/刷写测试工程、写测试用例、查协议点时使用。
---

# Beastech 测试开发工具包 (GEEA EEA4.0 / TSMaster)

随身知识库 + 可复制 SDK 模板，让测试开发**脱离任何具体源工程**也能生产。

## 结构

```
beastech-dev-kit/
├── SKILL.md                      # 本文件（入口与导航）
├── references/                   # 协议事实 + API（脱离源工程的核心）
│   ├── sdk-skeleton.md           # MiniProgram 工程文件集 + 编译部署
│   ├── api-reference.md          # TSMaster C API 三层 + SID 封装总表
│   ├── uds-services.md           # UDS 服务表 + NRC + 安全访问
│   ├── swdl-flow.md              # SWDL/SBL 下载流程
│   ├── nm-state-machine.md       # NM 报文 + 状态机 + PNC + NM.json
│   ├── nm-test-framework.md      # NM_TEST 工程实战范式（config.ini 数据驱动/多通道/动作库/Python runner）
│   ├── smoketest-framework.md    # SmokeTest 冒烟测试工程范式（最成熟模板：全局执行器/12域/四态结果/Python绑定）
│   ├── cc-comm-control.md        # CC 0x28 通信控制
│   ├── e2e-profile.md            # E2E profile/counter/CRC
│   ├── lin-communication.md      # LIN 协议 + LDF 解析 + LIN 测试动作库
│   ├── vt-capl-library.md        # VT/CAPL 工程范式 + UDS 封装（Vector 对照版）
│   ├── transport-protocol.md     # TP 分总线（CAN/CANFD/LIN/FR/DoIP）
│   ├── device-control.md         # 硬件抽象（BOB/DRB/VH6501/PSRB/PPS/PCS）
│   ├── vt-routing-and-com-testing.md # 路由测试/通信矩阵/CC/NM 设备范式
│   ├── vt-encrypted-summary.md   # VT 5 个加密工程边界（不可读）
│   └── test-case-pattern.md      # 用例范式 + pass/fail 判定
├── templates/
│   ├── miniprogram-skeleton/     # ★ 主模板，基于 SmokeTest（2.8M，全局执行器/12域/四态/Python 绑定）
│   │   └── TEMPLATE-README.md    # 模板用法/架构/构建说明
│   ├── miniprogram-skeleton-diag-bench-legacy/  # 旧模板（基于 DIAG_BENCH，保留备用）
│   └── capl-resources/
│       └── vt-examples/          # ★ 8 个 VT 工程明文 CAPL 范本（7.7MB/221文件，换电脑随身可用）
│           └── README.md         # 范本索引（UDS/TP/设备控制/路由 速查）
└── assets/
    └── sample-test-case.h        # 范本用例（test_case_1021808 注释版）
```

## 何时用

- 新建一个 GEEA 测试工程（从模板复制）
- 写/改测试用例（查 references + 范本）
- 查 UDS 服务 / NRC / 协议事实
- 不依赖 d:/claudeProject/GEEA4.0/ 源工程时

## 怎么用

### 新建工程
1. 复制 `templates/miniprogram-skeleton/`（基于 SmokeTest，主模板）到目标目录。
2. 改工程名（同步 `.sln`/`.vcxproj`/`mp.def` 的 `LIBRARY` 行/`.cpp` 文件名）。
3. 保留 12 域 `TestCase_CAN_*.h` 作范本，按需修改；新域新建头文件。
4. 在 `GEEA_CAN_DIAGExtern.cpp` 注册新用例 extern 声明，用例登记到对应 JSON（CS.json/NM.json）。
5. VS2022 + C++17 + Win32 编译 → `output/*.mp` → 拖进 TSMaster。
6. **注意返回值约定**：SmokeTest 模板用例 `0=成功`（与 DIAG_BENCH legacy 的 `1=pass` 相反）。范式见 `references/test-case-pattern.md` + `references/smoketest-framework.md` + `assets/sample-test-case.h`。

### 写用例
查 `references/api-reference.md` 找封装函数（`SIDxx_*` + `CheckPosResponse_SIDxx`/`CheckNegResponse`/`CheckNonResponse`），套 `references/test-case-pattern.md` 的标准结构。协议点查对应 reference（uds/swdl/nm/cc/e2e）。

### 查协议事实
UDS 服务/NRC/会话/寻址 → `references/uds-services.md`。SWDL 流程 → `swdl-flow.md`。NM 状态机/PNC → `nm-state-machine.md`。**NM 工程实战范式**（config.ini/多通道/动作库）→ `nm-test-framework.md`。**SmokeTest 全栈工程范式**（全局执行器/12域/四态/Python绑定）→ `smoketest-framework.md`。CC → `cc-comm-control.md`。E2E → `e2e-profile.md`。**LIN**（协议/LDF/动作库）→ `lin-communication.md`。**TP 传输层**（CAN/CANFD/LIN/FR/DoIP）→ `transport-protocol.md`。**设备控制**（BOB/DRB/VH6501/电源/示波器）→ `device-control.md`。

### 查 VT/Vector 范式（对照借鉴）
VT/CAPL 工程结构 + UDS 封装对照 → `references/vt-capl-library.md`。路由测试/通信矩阵/CC/NM 设备范式 → `vt-routing-and-com-testing.md`。VT 加密工程边界 → `vt-encrypted-summary.md`。

### 写/查 CAPL 代码（CANoe 脚本）
**CAPL 范本源码随身带** → `templates/capl-resources/vt-examples/`（8 个 VT 工程明文 .can/.cin，含 UDS_Service_1.5.cin / TestCase.can / TP 分总线 / 设备控制全套，索引见该目录 README.md）。`vt-capl-library.md` 记 CAPL 语法/事件过程/UDS 封装签名。**具体 CAPL 函数**（5040 函数参数/返回值/示例）不在随身库——按需查远端 `d:/claudeProject/beastech-dev-kit/skills/beastech-dev-kit/`（`references/capl-reference.md` + `capl-help/` 的 5040 HTML zip + CHM + `templates/capl-resources/` 的 1669 函数 xlsx）。

> **能力边界**：agent 能**生成 CAPL 代码**（随身有语法+真实范本，换电脑可用；具体函数查远端），但 CAPL 的**编译/运行/结果验证需人工在 CANoe 完成**——agent 没有 CANoe 自动化接口。TSMaster 侧才有 MCP 执行闭环。

### 具体工程数据（不预记忆）
DID/RID/DTC 值、NM 报文 ID、SBL 地址、pincode —— 这些是**项目数据**，从目标工程的 `test_case_diag_api.h`/`PDXParser.h`/`Configuration.h`/数据库查，或现场用 TSMaster MCP `database_manage`/`trace_read` 读。本知识库只固化**协议事实**（与具体工程无关的部分）。

## 执行（TSMaster MCP）

通过 TSMaster MCP 驱动实际执行，catalog-first 契约（调 System API 前用 `tsmaster_api_catalog` 核实）：

1. `tsmaster_instance_list` → `tsmaster_target_select` 选目标
2. `tsmaster_measurement` `status/start/wait/stop`（总线/仿真操作前确认 running）
3. `tsmaster_database_manage` 加载/查询 .dbc/.arxml/.ldf/.pdx
4. `tsmaster_coding` MiniProgram 编程（program.create → block.set → code.generate/compile/run）
5. `tsmaster_systemvar` 读写系统变量
6. `tsmaster_bus_simulation` 通道/FIFO/统计
7. `tsmaster_trace_read` 读总线 trace
8. `tsmaster_runtime_evidence` 读日志/截图

联调 vECU（Python runner + dSPACE Veos，ZCUD/ZCUP）见 `tsmaster-geely-test` skill。

## 喂养新知识（可持续扩展）

新知识按**类别**追加到对应 references 文件，**不改 SKILL.md / agent**：

| 新学到 | 追加到 |
|--------|--------|
| 新 DID/NRC 表 | `references/uds-services.md` |
| 新 NM 行为/报文格式 | `references/nm-state-machine.md` |
| 新 NM 工程动作/框架模式 | `references/nm-test-framework.md` |
| 新 UDS 封装函数 | `references/api-reference.md` |
| 新 SWDL 步骤 | `references/swdl-flow.md` |
| 新 LIN 行为/动作 | `references/lin-communication.md` |
| 新 TP/DoIP 行为 | `references/transport-protocol.md` |
| 新设备控制 | `references/device-control.md` |
| 新 VT 工程范式 | `references/vt-capl-library.md` 或 `vt-routing-and-com-testing.md` |
| 新用例写法 | `assets/sample-test-case.h` |
| 新功能域 | 新建 `references/<域>.md` + 在本文件登记 |

## 关联

- 执行角色 agent：`~/.claude/agents/beastech-dev-engineer.md`（方法论 + 工作原则，引用本工具包）
- 审查角色 agent：`~/.claude/agents/beastech-test-reviewer.md`（协议正确性审查，六轴框架）
- 联调 skill：`tsmaster-geely-test`（vECU co-simulation）

## 工程方法论与质量护栏（agent-skills 集成）

beastech-dev-engineer 遵循 **spec → plan → build → test → review** 生命周期闭环（来自 agent-skills 的 spec-driven / planning / incremental / test-driven 体系），每个阶段有车载测试域适配。详见 agent 文件"工作方法论"段。

**内置质量护栏**（全程生效）：
- `doubt-driven-development` — 对非平凡决策做对抗式自检，防把假设当事实
- `source-driven-development` — 协议/API 决策必须有出处，不凭记忆
- `observability-and-instrumentation` — 用例写可解析证据日志，失败可复盘
- **不臆造协议** — 以上三条在 beastech 域的具体化：DID/NRC/pincode 必须溯源

**多 agent 审查**（Review 阶段，借鉴 `/ship` 并行扇出）：
| 角度 | agent |
|------|-------|
| 协议正确性 | `beastech-test-reviewer` |
| 测试质量 | `test-engineer`（agent-skills） |
| 诊断安全 | `security-auditor`（agent-skills） |
| 代码质量 | `code-reviewer`（agent-skills） |

编排由主对话或 `/review` `/ship` 负责，agent 之间不互相调用。
