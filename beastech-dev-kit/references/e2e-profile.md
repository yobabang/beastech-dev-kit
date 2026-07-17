# E2E 端到端保护

E2E（End-to-End）保护用于安全关键报文的完整性校验，防数据损坏/篡改。基于 AUTOSAR E2E 规范。

## 核心字段

| 字段 | 作用 |
|------|------|
| **Counter** | 计数器，0–14 循环（4 bit），每发一帧 +1，接收方检测跳变/丢帧 |
| **Data ID** | 数据标识，参与 CRC 计算，标识逻辑信号 |
| **CRC** | 校验和，覆盖 Data ID + Counter + 数据 |
| **Alive Counter** | 同 Counter（不同规范叫法） |

## E2E Profile

常见 profile（具体用哪个查目标工程）：
- **P1**（profile 1）：Counter 4 bit + CRC 8 bit
- **P2**（profile 2）：Counter 4 bit + CRC 8/16 bit
- **P4**/**P5**/**P6**：更复杂

GEEA E2E_CAN 工程的 profile 类型查 `GEEA4.0_CC_E2E_NM/E2E_CAN/` 源码与 `需求.txt`。

## 测试方法（counter 高负载场景）

典型用例（TC_1392714 范式，来自 E2E 需求.txt）：

1. 上电，DUT 周期发 `E2E_TxMsg`，Update bit=1
2. 稳定后模拟高负载发送，故障注入率 100%（`tsdisturb.h`）
3. 停止模拟，高负载后 5s 内 Counter 在 0–14 之间循环 +1
4. 重复故障率 80%，验证 counter 仍保持 +1 递增
5. 校验 E2E 保护在重复/丢失场景下的行为

## 故障注入（tsdisturb.h）

`tsdisturb.h` 提供总线扰动能力：
- 重复帧注入（指定重复率 %）
- 丢帧注入
- Counter 篡改
- CRC 篡改

用于验证 E2E 接收方的错误检测与处理。

## 测试关注点

- Counter 连续性（0–14 循环，无跳变）
- Counter 翻转行为（14 → 0）
- 丢帧检测（Counter 跳变 > 阈值）
- 重复帧检测（Counter 不变）
- CRC 错误检测
- 高负载下 E2E 鲁棒性
- Update bit 行为

## 测量与判定

- 用 TSMaster trace 抓 E2E_TxMsg 序列
- 解析每帧 Counter/CRC（可用 MiniProgram 解码或 trace_read 后离线分析）
- 校验 Counter 序列符合 +1 循环
- 校验故障注入后接收方正确报错/容错

## 编码注意

E2E `需求.txt` 多为 **GBK 编码**，读取可能乱码。靠 TC_ID、SID、数值和上下文辨认。新写文件统一 UTF-8。

> E2E profile、Data ID、报文 ID 属项目数据，查目标工程 `E2E_CAN/` 与数据库。本文件只记协议事实与测试方法。
