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
│   ├── cc-comm-control.md        # CC 0x28 通信控制
│   ├── e2e-profile.md            # E2E profile/counter/CRC
│   ├── lin-communication.md      # LIN 协议 + LDF 解析 + LIN 测试动作库
│   └── test-case-pattern.md      # 用例范式 + pass/fail 判定
├── templates/
│   └── miniprogram-skeleton/     # 可复制的完整可编译 SDK 工程（1.9M）
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
1. 复制 `templates/miniprogram-skeleton/` 到目标目录。
2. 改工程名（同步 `.sln`/`.vcxproj`/`mp.def` 的 `LIBRARY` 行/`.cpp` 文件名）。
3. 在 `TestCase_CAN_*.h` 或新头文件里写用例，范式见 `references/test-case-pattern.md` + `assets/sample-test-case.h`。
4. 用例登记到对应 JSON（CS.json/NM.json），`TestFuncName` 与 C 函数名一致。
5. x86 编译 → dll 拖进 TSMaster。

### 写用例
查 `references/api-reference.md` 找封装函数（`SIDxx_*` + `CheckPosResponse_SIDxx`/`CheckNegResponse`/`CheckNonResponse`），套 `references/test-case-pattern.md` 的标准结构。协议点查对应 reference（uds/swdl/nm/cc/e2e）。

### 查协议事实
UDS 服务/NRC/会话/寻址 → `references/uds-services.md`。SWDL 流程 → `swdl-flow.md`。NM 状态机/PNC → `nm-state-machine.md`。**NM 工程实战范式**（config.ini/多通道/动作库）→ `nm-test-framework.md`。CC → `cc-comm-control.md`。E2E → `e2e-profile.md`。**LIN**（协议/LDF/动作库）→ `lin-communication.md`。

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
| 新用例写法 | `assets/sample-test-case.h` |
| 新功能域 | 新建 `references/<域>.md` + 在本文件登记 |

## 关联

- 执行角色 agent：`~/.claude/agents/beastech-dev-engineer.md`（方法论 + 工作原则，引用本工具包）
- 联调 skill：`tsmaster-geely-test`（vECU co-simulation）
