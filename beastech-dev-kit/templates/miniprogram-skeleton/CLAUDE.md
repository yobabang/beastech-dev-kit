# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Build Commands

- **Build all (Debug Win32)**: Open `GEEA_CAN_DIAG.sln` in VS2022 and build, or use `msbuild GEEA_CAN_DIAG.vcxproj /p:Configuration=Debug;Platform=Win32`
- **Build (Release x64)**: `msbuild GEEA_CAN_DIAG.vcxproj /p:Configuration=Release;Platform=x64`
- **Output**: DLLs generated in `./output/` with extension `.mp` (Win32) or `.mp64` (x64)
- **Clean**: `msbuild GEEA_CAN_DIAG.vcxproj /t:Clean`

## Project Overview

TSMaster Mini Program (DLL plugin) for automotive ECU CAN/CANFD diagnostic smoke testing on Geely's GEEA 3.0/3.5 platform. Built with VS2022 (v143), C++17, targeting Windows.

The DLL is loaded by TSMaster software and exposes test case functions callable from TSMaster's Mini Program environment.

## Code Architecture

### Entry Points
- **`GEEA_CAN_DIAG.cpp`** — TSMaster event callbacks: CAN/CANFD/LIN/Ethernet RX/TX/PreTx, timer, start/stop events. Includes and instantiates all test case modules.
- **`GEEA_CAN_DIAGExtern.cpp`** — Test case extern declarations, `output<T>` template specializations, logger functions, and the main test executor (calls `Pretest()` → test cases → `TestFinish()`).

### Test Domains (each is a header file)
| File | Domain |
|------|--------|
| `TestCase_CAN_DiagnosticServer.h` | UDS diagnostic services (SID 0x10, 0x11, 0x22, 0x27, 0x28, 0x2E, 0x3E, etc.) |
| `TestCase_CAN_NM.h` | Network Management (PNC wake/sleep, frame ID checks, timing) |
| `TestCase_CAN_E2E.h` | End-to-End protection (CRC, counter, DTC verification) |
| `TestCase_CAN_NVM.h` | NVM read/write under various power/reset conditions |
| `TestCase_CAN_SWDL.h` | Software download over UDS |
| `TestCase_CAN_SecurityUnlock.h` | Security access (seed/key, incorrect key, signature check) |
| `TestCase_CAN_CS.h` | Checksum verification |
| `TestCase_CAN_Communication.h` | CAN/CANFD communication tests |
| `TestCase_CANFD_Communication.h` | CANFD-specific communication tests |
| `TestCase_LIN_Communication.h` | LIN bus communication tests |
| `TestCase_CAN_CheckVision.h` | Software version check |

### Shared Infrastructure
- **`test_function.h`** (~233K) — Test utilities: signal manipulation, E2E CRC/counter helpers, session/routine control, DTC operations
- **`test_case_diag_api.h`** (~450K) — Diagnostic API: SID-specific request/response wrappers, TP layer functions
- **`test_common.h`** — Common type definitions, diagnostic IDs, bus configuration, CAN/LIN message vectors
- **`test_parameter.h`** — NM timing parameters (cycle time, timeout, sleep/wake), DID definitions, bus topology
- **`test_devices.h`** / **`test_devices_3.0.h`** — Hardware device control (power supplies, board cards, multimeter)
- **`test_tp.h`** — Transport protocol helpers
- **`TestReport.h`** — HTML report generation system (init, step logging, pass/fail, finalize)
- **`FunctionDeclaration.h`** — Central function declaration header for all modules

### External SDK
- **`TSMaster.h`** (~410K) / **`TSMaster_2024.h`** — TSMaster SDK API (app, com, test objects, CAN/LIN/Ethernet types)
- **`MPLibrary.h`** — Runtime-loaded function pointer tables for diagnostic operations (`rtlUIDiagnostics`) and VISA instrument control (`rtlTSInstrument`)
- **`cJSON.c`/`cJSON.h`** — JSON parsing
- **`PDXParser.h`** / **`SBLParser.h`** — PDX/SBL file parsing for software flashing
- **`GenerateKeyExOpt.h`** — Security key generation algorithms

### Configuration
- **`Configuration.h`** — Auto-generated constant definitions from TSMaster Constant Manager
- **INI files** — Runtime configuration for bus topology, DUT parameters (path read at runtime via `GetParameter()`)

### Other Files
- **`Database.h`** / **`Database.cpp`** — Database loading stubs
- **`TSMasterBaseSource.cpp`** / **`TSMasterBaseInclude.h`** — Base include chain
- **`mp.def`** — DLL exports (`initialize_miniprogram`, `finalize_miniprogram`, `retrieve_mp_abilities`, `write_api_document_auto`)
- **`output/GEEA_CAN_DIAG.py`** — Auto-generated Python ctypes bindings for the exported test functions

## Test Case Pattern

Each test case is `s32` function returning 0 on success. Standard flow:
```cpp
s32 test_case_XXXX() {
    strcpy(TestCaseName, "TC_XXXX_Description");
    strcpy(TestScopeName, "Scope Group");
    strcpy(testPurpose_CH, "测试目的：...");
    TestPrecondition();       // Setup: init report, configure bus
    // ... test steps using SID helper functions ...
    CheckPosResponse_SID10(0x01);
    TestFinish();              // Teardown: finalize report
}
```

Test results are tracked via `Test_Result` enum (`TestOK`/`TestNOK`/`TestWarning`/`TestNA`) and reported to HTML via `TestReport_HTML_Step()` / `TestInfoPrintPass()` / `TestInfoPrintFail()`.

## Deployment

Compiled `.mp`/`.mp64` files are loaded by TSMaster via drag-and-drop or Mini Program Library Manager. For debugging, set output directory to TSMaster's bin path (see `readme.txt`).
