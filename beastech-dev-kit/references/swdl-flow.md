# SWDL 软件下载流程

GEEA 软件下载（Software Download）基于 UDS 0x34/0x36/0x37 + SBL（Secondary Bootloader）机制。

## 总体序列

```
1. 进编程会话    TestSeq_EnterProgrammingSession()
   ├─ SID10 SessionControl → ProgrammingSession (0x02)
   ├─ SID27 SecurityAccess (seed/key，level 通常 0x01)
   └─ SID11 ECUReset（按需）
2. 下载 SBL      TestSeq_DownloadSBL(0)     // 参数 0 = 下载但不激活
   ├─ SID34 RequestDownload(SBL 地址, SBL 大小)
   ├─ SID36 TransferData(分块)            // SBLParser 解析 SBL 文件分块
   └─ SID37 RequestTransferExit
3. 激活 SBL      ActivateSBL()             // 跳转到 SBL 执行
   └─ SID11 ECUReset / 或 SID31 RoutineControl 跳转
4. 进 SBL 后再次安全访问（SBL 的 level 可能不同）
5. 下载应用      SID34 → SID36 → SID37     // 应用二进制
   ├─ 可选 SID31 RoutineControl_EraseMemory 先擦除
   └─ 地址/大小由 SBLParser 或配置给出
6. 校验          SID31 RoutineControl(校验例程, checksum)
7. 退出          SID37 / SID11 复位生效
```

## 关键封装

| 封装 | 作用 |
|------|------|
| `TestSeq_EnterProgrammingSession()` | 进编程会话（含安全访问） |
| `TestSeq_DownloadSBL(activate)` | 下载 SBL，0=不激活，1=激活 |
| `DownloadSBL()` | 下载 SBL（激活变体） |
| `ActivateSBL()` | 激活 SBL |
| `SID34_Request_Download(addr, size)` | 请求下载 |
| `SID36_Transfer_Data(src, size, reqCase)` | 传输数据块 |
| `SID37_RequestTransferExit()` | 退出传输 |
| `SID31_RoutineControl_EraseMemory()` | 擦除例程 |

## SBL 文件解析（SBLParser.h）

SBL = Secondary Bootloader 二进制，需解析出：
- **下载地址**（内存起始地址，传给 0x34）
- **数据块**（分块传给 0x36，块大小受 MaxLength 约束，常见 0x2000）
- **校验信息**

`SBLParser.h` 负责把 SBL 文件拆成可传输的块序列。应用文件同理。

## 地址与大小

- `MaxLength = 0x2000`（典型单块最大 8KB，见 test_case_diag_api.h `reqSWDL_ID`/`MaxLength`）
- 内存地址格式：0x34 的 addressAndLengthFormatIdentifier 指定地址/长度字段宽度
- 具体地址、SBL/应用文件路径属项目数据，查目标工程配置，不在本库固化

## 典型 NRC（SWDL 相关）

| NRC | 含义 |
|-----|------|
| 0x70 | uploadDownloadNotAccepted |
| 0x71 | transferSuspended |
| 0x72 | generalProgrammingFailure |
| 0x73 | wrongBlockSequenceCounter |
| 0x74 | requestIncorrectlyReceivedInvalidBlockSequence |

## 测试关注点

- 会话切换正确性（Default/Extended/Programming 互斥）
- 安全访问失败次数限制（0x36 exceedNumberOfAttempts）
- 下载中断/断点续传
- 校验失败处理
- SBL 激活失败保护（不激活旧 SBL 仍可回退）

参考范本：`templates/miniprogram-skeleton/` 的 `TestCase_CAN_SWDL.h`（test_case_1021808 起）。
