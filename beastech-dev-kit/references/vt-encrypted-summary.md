# VT 加密工程边界（不可读）

`D:/VTProject/` 下 5 个工程核心逻辑加密（`.canencr`/`.cinencr` 无对应明文），**无法作为养料**。记录边界,避免重复探查。

## 不可读工程

| 工程 | 明文残留 | 缺失的域 |
|------|---------|---------|
| GEEA3.5_SWDL_V8 1.4 2.7 | 仅 vbf.cin(VBF 数据)+ TestFunctionLibrary.cin(8KB) | 25 个 SWDL 用例 can 全加密,SWDL 核心逻辑读不到 |
| GEEA3.0_DiagnosticCommunicationCANFD_Encry_V1.3.4 | 仅 4KB cin | CANFD 诊断通信用例+库全加密 |
| GWTP_CAN(FD)_Lowerlink_V1.1.7 - Encr | 仅 5KB cin | 网关 CAN(FD) 下行测试逻辑全加密 |
| GWTP_X_Upperlink_V1.2.8 - Encr | 仅 5KB cin | 网关以太网上行测试逻辑全加密 |
| Geely3.0_E2E Sender Project_V1.1-encr | 仅 1 个 cin(61KB) | 70 个 cinencr,E2E 发送端逻辑基本读不到 |

## 替代来源

这些域的养料从 **beastech 自有 TSMaster 明文工程**补:

| 缺失域 | beastech 替代工程 | 对应 reference |
|--------|------------------|---------------|
| SWDL | GEEA3.0_SWDL/GEEA_CAN_DIAG_BENCH | [swdl-flow.md](swdl-flow.md) |
| E2E | GEEA4.0_CC_E2E_NM/E2E_CAN | [e2e-profile.md](e2e-profile.md) |
| CANFD 通信 | GEEA_CAN_DIAG_BENCH TestCase_CANFD | [vt-capl-library.md](vt-capl-library.md)（UDS 通用） |
| 网关路由 | 无直接对应,参考 DiagRouting 明文工程 | [vt-routing-and-com-testing.md](vt-routing-and-com-testing.md) |

## 说明

- "Encry" 后缀工程大多是"明文 + 加密副本"双交付（见 [vt-capl-library.md](vt-capl-library.md)）,这 5 个是例外——核心逻辑也加密了
- 加密文件用 Vector CodeMeter 许可（`Basetech测试配置工具.exe` 带有效期）,需硬件加密狗解密
- 如需这些域的养料,需联系工程所有者获取明文版,或从 beastech TSMaster 工程侧补
