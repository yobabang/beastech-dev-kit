#include "TSMaster.h"
#include "MPLibrary.h"
#include "Configuration.h"
#include "TSMasterBaseInclude.h"
#include <memory>
#include <string>
#include <vector>
#include <cstdarg>

//以下为自定义外部函数
extern void Pretest();
extern void TestFinish();
extern void SDDB_Parser();
extern s32 TestCase_1022856_BusoffDTC();
extern s32 TestCase_1022857_FDC10TestofBusoffDTC();
extern s32 TestCase_1022858_UnconfirmedDTCLimitTestofBusoffDTC();
extern s32 TestCase_1022859_BusoffDTCaged();
extern s32 TestCase_1022860_BusoffDTCTestwhenElPowerLevelis1();
extern s32 TestCase_1022861_BusoffDTCTestwhenthevoltageofpowersupplyisoutofrange();
extern s32 TestCase_1022862_BusoffDTCInLongTermMemoryTest();
extern s32 TestCase_1022863_BusoffDTCTestwhenCarModeisnotNormal();
extern s32 TestCase_1022864_BusoffDTCTestafterCarModetransition();
extern s32 TestCase_1022865_BusoffDTCTestInDifferentUsageMode();
extern s32 TestCase_1325140_CanSMBorTimeTxEnsuredValueTest();
extern s32 TestCase_1315750_HighSupplyVoltageDTCTest();
extern s32 TestCase_1315751_ExtendDataOfHighSupplyVoltageDTCTest();
extern s32 TestCase_1315752_HighSupplyVoltageDTCTestWhenEIPowerLevelIs1();
extern s32 TestCase_1315753_HighSupplyVoltageDTCTestWhenCarModeIsNotNormal();
extern s32 TestCase_1315754_HighSupplyVoltageDTCTestWhenCarModeTransmission();
extern s32 TestCase_1315755_HighSupplyVoltageDTCTestWhenUsageModeTransmission();
extern s32 TestCase_1315756_HighSupplyVoltageDTCTestInLongTermMemoryTest();
extern s32 TestCase_1315757_LowSupplyVoltageDTCTest();
extern s32 TestCase_1315758_ExtendDataOfLowSupplyVoltageDTCTest();
extern s32 TestCase_1315759_LowSupplyVoltageDTCTestWhenEIPowerLevelIs1();
extern s32 TestCase_1315760_LowSupplyVoltageDTCTestWhenCarModeIsNotNormal();
extern s32 TestCase_1315761_LowSupplyVoltageDTCTestWhenCarModeTransmission();
extern s32 TestCase_1315763_LowSupplyVoltageDTCTestWhenSupplyVoltageComparedFailureIsDetected();
extern s32 TestCase_1315762_LowSupplyVoltageDTCTestWhenUsageModeTransmission();
extern s32 TestCase_1315764_LowSupplyVoltageDTCTestInLongTermMemoryTest();
extern s32 TestCase_1022899_SupplyVoltageComparedFailureDTCTest();
extern s32 TestCase_1022907_ExtendDataOfSupplyVoltageComparedFailureDTCTest();
extern s32 TestCase_1022906_SupplyVoltageComparedFailureDTCTestWhenUbatIsLowerThan12V();
extern s32 TestCase_1022905_SupplyVoltageComparedFailureDTCTestWhenElPowerLevelIs1();
extern s32 TestCase_1022904_SupplyVoltageComparedFailureDTCTestWhenCarModeIsNotNormal();
extern s32 TestCase_1022900_SupplyVoltageComparedFailureDTCTestWhenCarModeTransmissionl();
extern s32 TestCase_1022903_SupplyVoltageComparedFailureDTCTestWhenUsageModeIsNotDriving();
extern s32 TestCase_1022902_SupplyVoltageComparedFailureDTCTestWhenUsageModeTransmission();
extern s32 TestCase_1022901_SupplyVoltageComparedFailureDTCTestInLongTermMemoryTest();
extern s32 TestCase_1022875_CANcircuiterrorDTC();
extern s32 TestCase_1022876_FDC10testofCANcircuiterrorDTC();
extern s32 TestCase_1022877_UnconfirmedDTCLimittestofCANcircuiterrorDTC();
extern s32 TestCase_1022878_ConfirmedDTCandDTCAgedLimittestofCANcircuiterrorDTC();
extern s32 TestCase_1022879_CANcircuiterrorDTCwhenElPowerLevelis1();
extern s32 TestCase_1022880_CANcircuiterrorDTCwhenthevoltageofpowersupplyisoutofrange();
extern s32 TestCase_1022881_CANcircuiterrorDTCwhenCarModeisnotNormal();
extern s32 TestCase_1022882_CANcircuiterrorDTCafterCarModetransmission();
extern s32 TestCase_1022883_CommonCANelectricalfaultDTCwhenUsageModetransition();
extern s32 TestCase_1022889_PermanentmissingframeDTCwhenElPowerLevelis1();
extern s32 TestCase_1022890_PermanentmissingframeDTCwhenthevoltageofpowersupplyisoutofrange();
extern s32 TestCase_1022891_PermanentmissingframeDTCwhenUBatisoutofrange();
extern s32 TestCase_1022892_PermanentmissingframeDTCwhenCarModeisnotNormal();
extern s32 TestCase_1022893_PermanentmissingframeDTCafterCarModetransition();
extern s32 TestCase_1022894_PermanentmissingframeDTCwhenUsageModeisnotdriving();
extern s32 TestCase_1022895_PermanentmissingframeDTCwhenVehModMngGlbSafeisnotavailable();
extern s32 TestCase_1022896_PermanentmissingframeDTCwhenUsageModeTransmission();
extern s32 TestCase_1022897_PermanentmissingframeDTCwhenDUTnotreceivemissingframeTRC();
extern s32 TestCase_1022898_PermanentmissingframeDTCwhenthemissingframenodeisnotconfigured();
extern s32 TestCase_1022885_PermanentmissingframeDTCInLongTermMemoryTest();
extern s32 TestCase_1022884_PermanentmissingframeDTC();
extern s32 TestCase_1022886_FDC10testofPermanentmissingframeDTC();
extern s32 TestCase_1022887_UnconfirmedDTCLimittestofPermanentmissingframeDTC();
extern s32 TestCase_1022888_ConfirmedDTCandDTCAgedLimittestofPermanentmissingframeDTC();
extern s32 TestCase_1365370_BusoffDTCTestWhenDUTReceivesNoACKerrorframe();
extern s32 TestCase_1365371_BusoffDTCTestDuringDifferentOperationCycles_CANnode();
extern s32 TestCase_1022908_OCCxValueTest();
extern s32 TestCase_1325139_DTCStatusBitTest();
extern s32 TestCase_1346148_DTCGlobalSnapshotDataAndTimeStampTest();
extern s32 TestCase_1365372_DTCTestWhenECUResetAfterDTCCleared();
extern s32 TestCase_1022852_DetectionOfMissingLINResponse();
extern s32 TestCase_1365362_UnconfirmedDTCLimitTestOfMissingLINResponseDTC();
extern s32 TestCase_1365364_ConfirmedDTCLimitAndAgedDTCLimittestofmissingLINResponseDTC();



// Variables definition
TTSApp app;
TTSCOM com;
TTSTest test;

// Templates Instantiation
template <>
void output<TCAN>(TCAN* canMsg) {
    com.transmit_can_async(canMsg);
}

template <>
void output<TLIN>(TLIN* linMsg) {
    com.transmit_lin_async(linMsg);
}

template <>
void output<TFlexRay>(TFlexRay* frMsg) {
    com.transmit_flexray_async(frMsg);
}

template <>
void output<TEthernetHeader>(TEthernetHeader* ethMsg) {
    com.transmit_ethernet_async(ethMsg);
}

// logger functions definition
#define TEMP_STR_LEN 1024
typedef char* va_list;
#ifndef __va_rounded_size
#define __va_rounded_size(TYPE) (((sizeof(TYPE)+sizeof(int)-1)/sizeof(int))*sizeof(int))
#endif
 
#ifndef va_start
#define va_start(AP, LASTARG)   (AP = ((char *)& (LASTARG) + __va_rounded_size(LASTARG)))
#endif
 
#ifndef va_arg
#define va_arg(AP, TYPE)        (AP += __va_rounded_size(TYPE), *((TYPE *)(AP - __va_rounded_size(TYPE))))
#endif
 
#ifndef va_end
#define va_end(AP)              (AP = (va_list)0 )
#endif

// Utility functions definition
void internal_log(const char* AFile, const char* AFunc, const s32 ALine, const TLogLevel ALevel, const char* fmt, ...)
{ 
  va_list ap;    
  va_start(ap, fmt);
  std::vector<char> buf(1024);
  int ret;  
#if __cplusplus == 201103L
  while((ret = vsnprintf_s(&buf[0], buf.size(), fmt, ap)) == -1){
#else
  // in VC++ version is 199711L, log("__cplusplus: %d\n", __cplusplus);
  while ((ret = vsnprintf_s(&buf[0], buf.size(), buf.size(), fmt, ap)) == -1) {
#endif
    buf.resize(buf.size() * 2);    
  }
  va_end(ap);
  if (ret == static_cast<int>(buf.size())){
    buf[buf.size() - 1] = '\0';    
  }         
  app.debug_log(AFile, AFunc, ALine, &buf[0], ALevel);
}
//void internal_log(const char* AFile, const char* AFunc, const s32 ALine, const TLogLevel ALevel, const char* fmt, ...)
//{
//    va_list ap;
//    va_start(ap, fmt);
//
//    std::vector<char> buf(1024);
//    int ret;
//
//    while (true) {
//        va_list ap_copy;
//        va_copy(ap_copy, ap);
//        ret = vsnprintf_s(&buf[0], buf.size(), buf.size() - 1, fmt, ap_copy);
//        va_end(ap_copy);
//
//        if (ret != -1) break;
//        buf.resize(buf.size() * 2);
//    }
//
//    va_end(ap);
//    buf[std::min(static_cast<size_t>(ret), buf.size() - 1)] = '\0';
//
//    app.debug_log(AFile, AFunc, ALine, &buf[0], ALevel);
//}



void internal_test_log(const char* AFile, const char* AFunc, const s32 ALine, const TLogLevel ALevel, const char* fmt, ...)
{ 
  va_list ap;    
  va_start(ap, fmt);
  std::vector<char> buf(1024);
  int ret;  
#if __cplusplus == 201103L
  while ((ret = vsnprintf_s(&buf[0], buf.size(), fmt, ap)) == -1) {
#else
  while ((ret = vsnprintf_s(&buf[0], buf.size(), buf.size(), fmt, ap)) == -1) {
#endif
    buf.resize(buf.size() * 2);    
  }
  va_end(ap);
  if (ret == static_cast<int>(buf.size())){
    buf[buf.size() - 1] = '\0';    
  }         
  test.debug_log_info(AFile, AFunc, ALine, &buf[0], ALevel);
}

void test_logCAN(const char* ADesc, PCAN ACAN, const TLogLevel ALevel)
{
    char s[TEMP_STR_LEN];
    // channel, id, dlc, [data]
    if (ACAN->is_tx){
        if (ACAN->is_data){
            sprintf(s, "%s %d %03X Tx d %d [%02X %02X %02X %02X %02X %02X %02X %02X]", ADesc, ACAN->FIdxChn, ACAN->FIdentifier, ACAN->FDLC, ACAN->FData[0], ACAN->FData[1], ACAN->FData[2], ACAN->FData[3], ACAN->FData[4], ACAN->FData[5], ACAN->FData[6], ACAN->FData[7]);
        } else {
            sprintf(s, "%s %d %03X Tx r %d", ADesc, ACAN->FIdxChn, ACAN->FIdentifier, ACAN->FDLC);
        }
    } else {
        if (ACAN->is_data){
            sprintf(s, "%s %d %03X Rx d %d [%02X %02X %02X %02X %02X %02X %02X %02X]", ADesc, ACAN->FIdxChn, ACAN->FIdentifier, ACAN->FDLC, ACAN->FData[0], ACAN->FData[1], ACAN->FData[2], ACAN->FData[3], ACAN->FData[4], ACAN->FData[5], ACAN->FData[6], ACAN->FData[7]);
        } else {
            sprintf(s, "%s %d %03X Rx r %d", ADesc, ACAN->FIdxChn, ACAN->FIdentifier, ACAN->FDLC);
        }
    }
    test.log_info(s, ALevel);
}

DLLEXPORT s32 __stdcall initialize_miniprogram(const PTSMasterConfiguration AConf)
{
    app = AConf->FTSApp;
    com = AConf->FTSCOM;
    test = AConf->FTSTest;
    return 0;
    
}

DLLEXPORT s32 __stdcall finalize_miniprogram(void)
{
    return 0;
    
}

// MP library functions definition
//TTest_Report Test_Report;
Ttest_report test_report;
TrtlUIDiagnostics rtlUIDiagnostics; 
TrtlTSInstrument rtlTSInstrument;
Ttsdisturb tsdisturb;
Ttsdisturb_dll tsdisturb_dll;

// Timers defintions
extern TMPTimerMS NewTimer1;
extern TMPTimerMS CCPTimer;
extern TMPTimerMS TestStepValueByIndex;


// Retrieve TSMP abilities
typedef s32 (__stdcall* TRegTSMasterFunction)(const void* AObj, const char* AFuncType, const char* AFuncName, const char* AData, const void* AFuncPointer, const char* ADescription);
extern void step(void);
extern void on_can_rx_NewOn_CAN_Rx1(const TCAN* ACAN);
extern void on_canfd_rx_NewOn_CAN_FD_Rx1(const TCANFD* ACANFD);
extern void on_can_tx_NewOn_CAN_Tx1(const TCAN* ACAN);
extern void on_canfd_tx_NewOn_CAN_FD_Tx1(const TCANFD* ACANFD);
extern void on_can_pretx_NewOn_CAN_PreTx1(const PCAN ACAN);
extern void on_canfd_pretx_NewOn_CAN_FD_PreTx1(const PCANFD ACANFD);
extern void on_lin_rx_NewOn_LIN_Rx1(const TLIN* ALIN);
extern void on_lin_tx_NewOn_LIN_Tx1(const TLIN* ALIN);
extern void on_lin_pretx_NewOn_LIN_PreTx1(const PLIN ALIN);
extern void on_ethernet_rx_NewOn_Ethernet_Rx1(const TEthernetHeader* AEthernetHeader);
extern void on_ethernet_tx_NewOn_Ethernet_Tx1(const TEthernetHeader* AEthernetHeader);
extern void on_ethernet_pretx_NewOn_Ethernet_PreTx1(const PEthernetHeader AEthernetHeader);
extern void on_tcp_listen_NewOn_TCP_Listen1(const native_int ASocket, const native_int AClientSocket, const native_int AResult);
extern void on_tcp_connect_NewOn_TCP_Connect1(const native_int ASocket, const native_int AResult);
extern void on_tcp_close_NewOn_TCP_Close1(const native_int ASocket, const native_int AResult);
extern void on_tcp_receive_NewOn_TCP_Receive1(const native_int ASocket, const native_int AResult, const u32 AAddr, const u32 APort, const pu8 AData, const native_int ASize);
extern void on_tcp_transmit_NewOn_TCP_Transmit1(const native_int ASocket, const native_int AResult, const pu8 AData, const native_int ASize);
extern void on_udp_receive_from_NewOn_UDP_Receive_From1(const native_int ASocket, const native_int AResult, const u32 AAddr, const u32 APort, const pu8 AData, const native_int ASize);
extern void on_udp_transmit_to_NewOn_UDP_Transmit_To1(const native_int ASocket, const native_int AResult, const pu8 AData, const native_int ASize);
extern void on_var_change_On_TestSystem_CsvStatus(void);
extern void on_timer_T_StepUpTest(void);
extern void on_timer_On_CCPTimer(void);
extern void on_timer_TestStepValue(void);
//extern void on_timer_NewOn_Timer1
extern void on_start_NewOn_Start1(void);
extern void on_stop_NewOn_Stop1(void);
DLLEXPORT s32 __stdcall retrieve_mp_abilities(const void* AObj, const TRegTSMasterFunction AReg) {
  #define TSMASTER_VERSION "2024.6.7.1126"

  //以下为自定义外部函数
  if (!AReg(AObj, "on_custom_callback", "Pretest", "void", reinterpret_cast<const void*>(&Pretest), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestFinish", "void", reinterpret_cast<const void*>(&TestFinish), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "SDDB_Parser", "void", reinterpret_cast<const void*>(&SDDB_Parser), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1315750_HighSupplyVoltageDTCTest", "void", reinterpret_cast<const void*>(&TestCase_1315750_HighSupplyVoltageDTCTest), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1315751_ExtendDataOfHighSupplyVoltageDTCTest", "void", reinterpret_cast<const void*>(&TestCase_1315751_ExtendDataOfHighSupplyVoltageDTCTest), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1315752_HighSupplyVoltageDTCTestWhenEIPowerLevelIs1", "void", reinterpret_cast<const void*>(&TestCase_1315752_HighSupplyVoltageDTCTestWhenEIPowerLevelIs1), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1315753_HighSupplyVoltageDTCTestWhenCarModeIsNotNormal", "void", reinterpret_cast<const void*>(&TestCase_1315753_HighSupplyVoltageDTCTestWhenCarModeIsNotNormal), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1315754_HighSupplyVoltageDTCTestWhenCarModeTransmission", "void", reinterpret_cast<const void*>(&TestCase_1315754_HighSupplyVoltageDTCTestWhenCarModeTransmission), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1315755_HighSupplyVoltageDTCTestWhenUsageModeTransmission", "void", reinterpret_cast<const void*>(&TestCase_1315755_HighSupplyVoltageDTCTestWhenUsageModeTransmission), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1315756_HighSupplyVoltageDTCTestInLongTermMemoryTest", "void", reinterpret_cast<const void*>(&TestCase_1315756_HighSupplyVoltageDTCTestInLongTermMemoryTest), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1315757_LowSupplyVoltageDTCTest", "void", reinterpret_cast<const void*>(&TestCase_1315757_LowSupplyVoltageDTCTest), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1315758_ExtendDataOfLowSupplyVoltageDTCTest", "void", reinterpret_cast<const void*>(&TestCase_1315758_ExtendDataOfLowSupplyVoltageDTCTest), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1315759_LowSupplyVoltageDTCTestWhenEIPowerLevelIs1", "void", reinterpret_cast<const void*>(&TestCase_1315759_LowSupplyVoltageDTCTestWhenEIPowerLevelIs1), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1315760_LowSupplyVoltageDTCTestWhenCarModeIsNotNormal", "void", reinterpret_cast<const void*>(&TestCase_1315760_LowSupplyVoltageDTCTestWhenCarModeIsNotNormal), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1315761_LowSupplyVoltageDTCTestWhenCarModeTransmission", "void", reinterpret_cast<const void*>(&TestCase_1315761_LowSupplyVoltageDTCTestWhenCarModeTransmission), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1315762_LowSupplyVoltageDTCTestWhenUsageModeTransmission", "void", reinterpret_cast<const void*>(&TestCase_1315762_LowSupplyVoltageDTCTestWhenUsageModeTransmission), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1315763_LowSupplyVoltageDTCTestWhenSupplyVoltageComparedFailureIsDetected", "void", reinterpret_cast<const void*>(&TestCase_1315763_LowSupplyVoltageDTCTestWhenSupplyVoltageComparedFailureIsDetected), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1315764_LowSupplyVoltageDTCTestInLongTermMemoryTest", "void", reinterpret_cast<const void*>(&TestCase_1315764_LowSupplyVoltageDTCTestInLongTermMemoryTest), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1022899_SupplyVoltageComparedFailureDTCTest", "void", reinterpret_cast<const void*>(&TestCase_1022899_SupplyVoltageComparedFailureDTCTest), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1022900_SupplyVoltageComparedFailureDTCTestWhenCarModeTransmissionl", "void", reinterpret_cast<const void*>(&TestCase_1022900_SupplyVoltageComparedFailureDTCTestWhenCarModeTransmissionl), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1022901_SupplyVoltageComparedFailureDTCTestInLongTermMemoryTest", "void", reinterpret_cast<const void*>(&TestCase_1022901_SupplyVoltageComparedFailureDTCTestInLongTermMemoryTest), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1022902_SupplyVoltageComparedFailureDTCTestWhenUsageModeTransmission", "void", reinterpret_cast<const void*>(&TestCase_1022902_SupplyVoltageComparedFailureDTCTestWhenUsageModeTransmission), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1022903_SupplyVoltageComparedFailureDTCTestWhenUsageModeIsNotDriving", "void", reinterpret_cast<const void*>(&TestCase_1022903_SupplyVoltageComparedFailureDTCTestWhenUsageModeIsNotDriving), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1022904_SupplyVoltageComparedFailureDTCTestWhenCarModeIsNotNormal", "void", reinterpret_cast<const void*>(&TestCase_1022904_SupplyVoltageComparedFailureDTCTestWhenCarModeIsNotNormal), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1022905_SupplyVoltageComparedFailureDTCTestWhenElPowerLevelIs1", "void", reinterpret_cast<const void*>(&TestCase_1022905_SupplyVoltageComparedFailureDTCTestWhenElPowerLevelIs1), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1022906_SupplyVoltageComparedFailureDTCTestWhenUbatIsLowerThan12V", "void", reinterpret_cast<const void*>(&TestCase_1022906_SupplyVoltageComparedFailureDTCTestWhenUbatIsLowerThan12V), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1022907_ExtendDataOfSupplyVoltageComparedFailureDTCTest", "void", reinterpret_cast<const void*>(&TestCase_1022907_ExtendDataOfSupplyVoltageComparedFailureDTCTest), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1022875_CANcircuiterrorDTC", "void", reinterpret_cast<const void*>(&TestCase_1022875_CANcircuiterrorDTC), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1022876_FDC10testofCANcircuiterrorDTC", "void", reinterpret_cast<const void*>(&TestCase_1022876_FDC10testofCANcircuiterrorDTC), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1022877_UnconfirmedDTCLimittestofCANcircuiterrorDTC", "void", reinterpret_cast<const void*>(&TestCase_1022877_UnconfirmedDTCLimittestofCANcircuiterrorDTC), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1022878_ConfirmedDTCandDTCAgedLimittestofCANcircuiterrorDTC", "void", reinterpret_cast<const void*>(&TestCase_1022878_ConfirmedDTCandDTCAgedLimittestofCANcircuiterrorDTC), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1022879_CANcircuiterrorDTCwhenElPowerLevelis1", "void", reinterpret_cast<const void*>(&TestCase_1022879_CANcircuiterrorDTCwhenElPowerLevelis1), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1022880_CANcircuiterrorDTCwhenthevoltageofpowersupplyisoutofrange", "void", reinterpret_cast<const void*>(&TestCase_1022880_CANcircuiterrorDTCwhenthevoltageofpowersupplyisoutofrange), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1022881_CANcircuiterrorDTCwhenCarModeisnotNormal", "void", reinterpret_cast<const void*>(&TestCase_1022881_CANcircuiterrorDTCwhenCarModeisnotNormal), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1022882_CANcircuiterrorDTCafterCarModetransmission", "void", reinterpret_cast<const void*>(&TestCase_1022882_CANcircuiterrorDTCafterCarModetransmission), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1022883_CommonCANelectricalfaultDTCwhenUsageModetransition", "void", reinterpret_cast<const void*>(&TestCase_1022883_CommonCANelectricalfaultDTCwhenUsageModetransition), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1022856_BusoffDTC", "void", reinterpret_cast<const void*>(&TestCase_1022856_BusoffDTC), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1022857_FDC10TestofBusoffDTC", "void", reinterpret_cast<const void*>(&TestCase_1022857_FDC10TestofBusoffDTC), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1022858_UnconfirmedDTCLimitTestofBusoffDTC", "void", reinterpret_cast<const void*>(&TestCase_1022858_UnconfirmedDTCLimitTestofBusoffDTC), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1022859_BusoffDTCaged", "void", reinterpret_cast<const void*>(&TestCase_1022859_BusoffDTCaged), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1022860_BusoffDTCTestwhenElPowerLevelis1", "void", reinterpret_cast<const void*>(&TestCase_1022860_BusoffDTCTestwhenElPowerLevelis1), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1022861_BusoffDTCTestwhenthevoltageofpowersupplyisoutofrange", "void", reinterpret_cast<const void*>(&TestCase_1022861_BusoffDTCTestwhenthevoltageofpowersupplyisoutofrange), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1022862_BusoffDTCInLongTermMemoryTest", "void", reinterpret_cast<const void*>(&TestCase_1022862_BusoffDTCInLongTermMemoryTest), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1022863_BusoffDTCTestwhenCarModeisnotNormal", "void", reinterpret_cast<const void*>(&TestCase_1022863_BusoffDTCTestwhenCarModeisnotNormal), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1022864_BusoffDTCTestafterCarModetransition", "void", reinterpret_cast<const void*>(&TestCase_1022864_BusoffDTCTestafterCarModetransition), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1022865_BusoffDTCTestInDifferentUsageMode", "void", reinterpret_cast<const void*>(&TestCase_1022865_BusoffDTCTestInDifferentUsageMode), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1325140_CanSMBorTimeTxEnsuredValueTest", "void", reinterpret_cast<const void*>(&TestCase_1325140_CanSMBorTimeTxEnsuredValueTest), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1022889_PermanentmissingframeDTCwhenElPowerLevelis1", "void", reinterpret_cast<const void*>(&TestCase_1022889_PermanentmissingframeDTCwhenElPowerLevelis1), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1022890_PermanentmissingframeDTCwhenthevoltageofpowersupplyisoutofrange", "void", reinterpret_cast<const void*>(&TestCase_1022890_PermanentmissingframeDTCwhenthevoltageofpowersupplyisoutofrange), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1022891_PermanentmissingframeDTCwhenUBatisoutofrange", "void", reinterpret_cast<const void*>(&TestCase_1022891_PermanentmissingframeDTCwhenUBatisoutofrange), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1022892_PermanentmissingframeDTCwhenCarModeisnotNormal", "void", reinterpret_cast<const void*>(&TestCase_1022892_PermanentmissingframeDTCwhenCarModeisnotNormal), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1022893_PermanentmissingframeDTCafterCarModetransition", "void", reinterpret_cast<const void*>(&TestCase_1022893_PermanentmissingframeDTCafterCarModetransition), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1022894_PermanentmissingframeDTCwhenUsageModeisnotdriving", "void", reinterpret_cast<const void*>(&TestCase_1022894_PermanentmissingframeDTCwhenUsageModeisnotdriving), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1022895_PermanentmissingframeDTCwhenVehModMngGlbSafeisnotavailable", "void", reinterpret_cast<const void*>(&TestCase_1022895_PermanentmissingframeDTCwhenVehModMngGlbSafeisnotavailable), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1022896_PermanentmissingframeDTCwhenUsageModeTransmission", "void", reinterpret_cast<const void*>(&TestCase_1022896_PermanentmissingframeDTCwhenUsageModeTransmission), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1022897_PermanentmissingframeDTCwhenDUTnotreceivemissingframeTRC", "void", reinterpret_cast<const void*>(&TestCase_1022897_PermanentmissingframeDTCwhenDUTnotreceivemissingframeTRC), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1022898_PermanentmissingframeDTCwhenthemissingframenodeisnotconfigured", "void", reinterpret_cast<const void*>(&TestCase_1022898_PermanentmissingframeDTCwhenthemissingframenodeisnotconfigured), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1022885_PermanentmissingframeDTCInLongTermMemoryTest", "void", reinterpret_cast<const void*>(&TestCase_1022885_PermanentmissingframeDTCInLongTermMemoryTest), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1022884_PermanentmissingframeDTC", "void", reinterpret_cast<const void*>(&TestCase_1022884_PermanentmissingframeDTC), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1022886_FDC10testofPermanentmissingframeDTC", "void", reinterpret_cast<const void*>(&TestCase_1022886_FDC10testofPermanentmissingframeDTC), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1022887_UnconfirmedDTCLimittestofPermanentmissingframeDTC", "void", reinterpret_cast<const void*>(&TestCase_1022887_UnconfirmedDTCLimittestofPermanentmissingframeDTC), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1022888_ConfirmedDTCandDTCAgedLimittestofPermanentmissingframeDTC", "void", reinterpret_cast<const void*>(&TestCase_1022888_ConfirmedDTCandDTCAgedLimittestofPermanentmissingframeDTC), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1365370_BusoffDTCTestWhenDUTReceivesNoACKerrorframe", "void", reinterpret_cast<const void*>(&TestCase_1365370_BusoffDTCTestWhenDUTReceivesNoACKerrorframe), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1365371_BusoffDTCTestDuringDifferentOperationCycles_CANnode", "void", reinterpret_cast<const void*>(&TestCase_1365371_BusoffDTCTestDuringDifferentOperationCycles_CANnode), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1022908_OCCxValueTest", "void", reinterpret_cast<const void*>(&TestCase_1022908_OCCxValueTest), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1325139_DTCStatusBitTest", "void", reinterpret_cast<const void*>(&TestCase_1325139_DTCStatusBitTest), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1346148_DTCGlobalSnapshotDataAndTimeStampTest", "void", reinterpret_cast<const void*>(&TestCase_1346148_DTCGlobalSnapshotDataAndTimeStampTest), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1365372_DTCTestWhenECUResetAfterDTCCleared", "void", reinterpret_cast<const void*>(&TestCase_1365372_DTCTestWhenECUResetAfterDTCCleared), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1022852_DetectionOfMissingLINResponse", "void", reinterpret_cast<const void*>(&TestCase_1022852_DetectionOfMissingLINResponse), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1365362_UnconfirmedDTCLimitTestOfMissingLINResponseDTC", "void", reinterpret_cast<const void*>(&TestCase_1365362_UnconfirmedDTCLimitTestOfMissingLINResponseDTC), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestCase_1365364_ConfirmedDTCLimitAndAgedDTCLimittestofmissingLINResponseDTC", "void", reinterpret_cast<const void*>(&TestCase_1365364_ConfirmedDTCLimitAndAgedDTCLimittestofmissingLINResponseDTC), "")) return -1;

  //// MP Library VISA
  //if (!AReg(AObj, "reg_mp_library_func", "VISA.visa_initializing", "pu32 defaultRM", &VISA.visa_initializing, "")) return -1;
  //if (!AReg(AObj, "reg_mp_library_func", "VISA.get_usb_device", "u32 defaultRM, pu32 list, pu32 nmatches, char* matches", &VISA.get_usb_device, "")) return -1;
  //if (!AReg(AObj, "reg_mp_library_func", "VISA.findnext_usb_device", "u32 list, char* matches", &VISA.findnext_usb_device, "")) return -1;
  //if (!AReg(AObj, "reg_mp_library_func", "VISA.open_usb_device", "u32 defaultRM,char* matches,pu32 handle", &VISA.open_usb_device, "")) return -1;
  //if (!AReg(AObj, "reg_mp_library_func", "VISA.visa_write", "u32 handle, char* buff", &VISA.visa_write, "")) return -1;
  //if (!AReg(AObj, "reg_mp_library_func", "VISA.visa_read", "u32 handle,  char** result,u32* len", &VISA.visa_read, "")) return -1;
  //if (!AReg(AObj, "reg_mp_library_func", "VISA.visa_close", "u32 handle", &VISA.visa_close, "")) return -1;
  //MP Library rtlTSInstrument
  if (!AReg(AObj, "reg_mp_library_func", "rtlTSInstrument.visa_query_device_list", "const char** ADeviceList", &rtlTSInstrument.visa_query_device_list, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "rtlTSInstrument.visa_create_device", "const s32* AID, const s32 AVISAType, const char* AConnectString", &rtlTSInstrument.visa_create_device, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "rtlTSInstrument.visa_send", "const s32 AID, const char* ASendCommand", &rtlTSInstrument.visa_send, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "rtlTSInstrument.visa_query", "const s32 AID, const char* ASendCommand, const char** AResult", &rtlTSInstrument.visa_query, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "rtlTSInstrument.visa_query_bin", "const s32 AID, const char* ASendCommand, const char* AResult, const int* ADataLength", &rtlTSInstrument.visa_query_bin, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "rtlTSInstrument.visa_finalizelib", "void", &rtlTSInstrument.visa_finalizelib, "")) return -1;


  if (!AReg(AObj, "check_mp_internal", "version", TSMASTER_VERSION, 0, "")) return -1;
  if (!AReg(AObj, "check_mp_internal", "struct_size", "struct_size_app", (void *)sizeof(TTSMasterConfiguration), "")) return -1;
  if (!AReg(AObj, "check_mp_internal", "struct_size", "struct_size_tcan", (void *)sizeof(TCAN), "")) return -1;
  if (!AReg(AObj, "check_mp_internal", "struct_size", "struct_size_tcanfd", (void *)sizeof(TCANFD), "")) return -1;
  if (!AReg(AObj, "check_mp_internal", "struct_size", "struct_size_tlin", (void *)sizeof(TLIN), "")) return -1;
  if (!AReg(AObj, "check_mp_internal", "struct_size", "struct_size_tflexray", (void *)sizeof(TFlexRay), "")) return -1;
  if (!AReg(AObj, "check_mp_internal", "struct_size", "struct_size_tethernetheader", (void *)sizeof(TEthernetHeader), "")) return -1;
  if (!AReg(AObj, "check_mp_internal", "struct_size", "struct_size_TMPVarInt", (void *)sizeof(TMPVarInt), "")) return -1;
  if (!AReg(AObj, "check_mp_internal", "struct_size", "struct_size_TMPVarDouble", (void *)sizeof(TMPVarDouble), "")) return -1;
  if (!AReg(AObj, "check_mp_internal", "struct_size", "struct_size_TMPVarString", (void *)sizeof(TMPVarString), "")) return -1;
  if (!AReg(AObj, "check_mp_internal", "struct_size", "struct_size_TMPVarCAN", (void *)sizeof(TMPVarCAN), "")) return -1;
  if (!AReg(AObj, "check_mp_internal", "struct_size", "struct_size_TMPVarCANFD", (void *)sizeof(TMPVarCANFD), "")) return -1;
  if (!AReg(AObj, "check_mp_internal", "struct_size", "struct_size_TMPVarLIN", (void *)sizeof(TMPVarLIN), "")) return -1;
  if (!AReg(AObj, "check_mp_internal", "struct_size", "struct_size_TLIBTSMapping", (void *)sizeof(TLIBTSMapping), "")) return -1;
  if (!AReg(AObj, "check_mp_internal", "struct_size", "struct_size_TLIBSystemVarDef", (void *)sizeof(TLIBSystemVarDef), "")) return -1;
  if (!AReg(AObj, "check_mp_internal", "auto_start", "0", 0, "")) return -1;
  if (!AReg(AObj, "check_mp_internal", "addr_conf", "app", &app, "")) return -1;
  if (!AReg(AObj, "step_function", "step", "5", reinterpret_cast<const void*>(&step), "")) return -1;
  if (!AReg(AObj, "timer", "NewTimer1", "100", reinterpret_cast<const void*>(&NewTimer1), "")) return -1;
  if (!AReg(AObj, "timer", "CCPTimer", "100", reinterpret_cast<const void*>(&CCPTimer), "")) return -1;
  if (!AReg(AObj, "timer", "TestStepValueByIndex", "100", reinterpret_cast<const void*>(&TestStepValueByIndex), "")) return -1;

  //if (!AReg(AObj, "timer", "T_StepUpTest", "100", reinterpret_cast<const void*>(&T_StepUpTest), "")) return -1;
  if (!AReg(AObj, "on_can_rx_callback", "on_can_rx_NewOn_CAN_Rx1", "-1,-1,0", reinterpret_cast<const void*>(&on_can_rx_NewOn_CAN_Rx1), "")) return -1;
  if (!AReg(AObj, "on_can_rx_callback", "on_canfd_rx_NewOn_CAN_FD_Rx1", "-1,-1,-1", reinterpret_cast<const void*>(&on_canfd_rx_NewOn_CAN_FD_Rx1), "")) return -1;
  if (!AReg(AObj, "on_can_tx_callback", "on_can_tx_NewOn_CAN_Tx1", "-1,-1,0", reinterpret_cast<const void*>(&on_can_tx_NewOn_CAN_Tx1), "")) return -1;
  if (!AReg(AObj, "on_can_tx_callback", "on_canfd_tx_NewOn_CAN_FD_Tx1", "-1,-1,-1", reinterpret_cast<const void*>(&on_canfd_tx_NewOn_CAN_FD_Tx1), "")) return -1;
  if (!AReg(AObj, "on_can_pretx_callback", "on_can_pretx_NewOn_CAN_PreTx1", "-1,-1,0", reinterpret_cast<const void*>(&on_can_pretx_NewOn_CAN_PreTx1), "")) return -1;
  if (!AReg(AObj, "on_can_pretx_callback", "on_canfd_pretx_NewOn_CAN_FD_PreTx1", "-1,-1,-1", reinterpret_cast<const void*>(&on_canfd_pretx_NewOn_CAN_FD_PreTx1), "")) return -1;
  if (!AReg(AObj, "on_lin_rx_callback", "on_lin_rx_NewOn_LIN_Rx1", "-1", reinterpret_cast<const void*>(&on_lin_rx_NewOn_LIN_Rx1), "")) return -1;
  if (!AReg(AObj, "on_lin_tx_callback", "on_lin_tx_NewOn_LIN_Tx1", "-1", reinterpret_cast<const void*>(&on_lin_tx_NewOn_LIN_Tx1), "")) return -1;
  if (!AReg(AObj, "on_lin_pretx_callback", "on_lin_pretx_NewOn_LIN_PreTx1", "-1", reinterpret_cast<const void*>(&on_lin_pretx_NewOn_LIN_PreTx1), "")) return -1;
  if (!AReg(AObj, "on_ethernet_rx_callback", "on_ethernet_rx_NewOn_Ethernet_Rx1", "-1", reinterpret_cast<const void*>(&on_ethernet_rx_NewOn_Ethernet_Rx1), "")) return -1;
  if (!AReg(AObj, "on_ethernet_tx_callback", "on_ethernet_tx_NewOn_Ethernet_Tx1", "-1", reinterpret_cast<const void*>(&on_ethernet_tx_NewOn_Ethernet_Tx1), "")) return -1;
  if (!AReg(AObj, "on_ethernet_pretx_callback", "on_ethernet_pretx_NewOn_Ethernet_PreTx1", "-1", reinterpret_cast<const void*>(&on_ethernet_pretx_NewOn_Ethernet_PreTx1), "")) return -1;
  if (!AReg(AObj, "on_tcp_listen_callback", "on_tcp_listen_NewOn_TCP_Listen1", "0,192.168.1.111:1234", reinterpret_cast<const void*>(&on_tcp_listen_NewOn_TCP_Listen1), "")) return -1;
  if (!AReg(AObj, "on_tcp_connect_callback", "on_tcp_connect_NewOn_TCP_Connect1", "0,192.168.1.111:1234", reinterpret_cast<const void*>(&on_tcp_connect_NewOn_TCP_Connect1), "")) return -1;
  if (!AReg(AObj, "on_tcp_close_callback", "on_tcp_close_NewOn_TCP_Close1", "0,192.168.1.111:1234", reinterpret_cast<const void*>(&on_tcp_close_NewOn_TCP_Close1), "")) return -1;
  if (!AReg(AObj, "on_tcp_receive_callback", "on_tcp_receive_NewOn_TCP_Receive1", "0,192.168.1.111:1234", reinterpret_cast<const void*>(&on_tcp_receive_NewOn_TCP_Receive1), "")) return -1;
  if (!AReg(AObj, "on_tcp_transmit_callback", "on_tcp_transmit_NewOn_TCP_Transmit1", "0,192.168.1.111:1234", reinterpret_cast<const void*>(&on_tcp_transmit_NewOn_TCP_Transmit1), "")) return -1;
  if (!AReg(AObj, "on_udp_receive_from_callback", "on_udp_receive_from_NewOn_UDP_Receive_From1", "0,192.168.1.111:1234", reinterpret_cast<const void*>(&on_udp_receive_from_NewOn_UDP_Receive_From1), "")) return -1;
  if (!AReg(AObj, "on_udp_transmit_to_callback", "on_udp_transmit_to_NewOn_UDP_Transmit_To1", "0,192.168.1.111:1234", reinterpret_cast<const void*>(&on_udp_transmit_to_NewOn_UDP_Transmit_To1), "")) return -1;
  if (!AReg(AObj, "on_var_change_callback", "on_var_change_On_TestSystem_CsvStatus", "TestSystem.CsvStatus", reinterpret_cast<const void*>(&on_var_change_On_TestSystem_CsvStatus), "")) return -1;
  if (!AReg(AObj, "on_timer_callback", "on_timer_T_StepUpTest", "NewTimer1", reinterpret_cast<const void*>(&on_timer_T_StepUpTest), "")) return -1;
  if (!AReg(AObj, "on_timer_callback", "on_timer_On_CCPTimer", "CCPTimer", reinterpret_cast<const void*>(&on_timer_On_CCPTimer), "")) return -1;
  if (!AReg(AObj, "on_timer_callback", "on_timer_TestStepValue", "TestStepValueByIndex", reinterpret_cast<const void*>(&on_timer_TestStepValue), "")) return -1;
  //if (!AReg(AObj, "on_timer_callback", "on_timer_NewOn_Timer1", "NewTimer1", reinterpret_cast<const void*>(&on_timer_NewOn_Timer1), "")) return -1;

  if (!AReg(AObj, "on_start_callback", "on_start_NewOn_Start1", "", reinterpret_cast<const void*>(&on_start_NewOn_Start1), "")) return -1;
  if (!AReg(AObj, "on_stop_callback", "on_stop_NewOn_Stop1", "", reinterpret_cast<const void*>(&on_stop_NewOn_Stop1), "")) return -1;
  // MP library functions
  // MP Library Test_Report
  //if (!AReg(AObj, "reg_mp_library_func", "Test_Report.test_report_init", "const pnative_int Handle", &Test_Report.test_report_init, "")) return -1;
  //if (!AReg(AObj, "reg_mp_library_func", "Test_Report.test_report_add_head", "const native_int Handle,const char* info,const s32 ALevel", &Test_Report.test_report_add_head, "")) return -1;
  //if (!AReg(AObj, "reg_mp_library_func", "Test_Report.test_report_finalize", "const native_int Handle", &Test_Report.test_report_finalize, "")) return -1;
  //if (!AReg(AObj, "reg_mp_library_func", "Test_Report.test_report_add_info", "const native_int Handle,const char* AMsg", &Test_Report.test_report_add_info, "")) return -1;
  //if (!AReg(AObj, "reg_mp_library_func", "Test_Report.test_report_start_add_table", "const native_int Handle,const char* AMsg,const char* ATableName,const s32 cols", &Test_Report.test_report_start_add_table, "")) return -1;
  //if (!AReg(AObj, "reg_mp_library_func", "Test_Report.test_report_add_table_info", "const native_int Handle,const char* AMsg", &Test_Report.test_report_add_table_info, "")) return -1;
  //if (!AReg(AObj, "reg_mp_library_func", "Test_Report.test_report_end_add_table", "const native_int Handle", &Test_Report.test_report_end_add_table, "")) return -1;
  //if (!AReg(AObj, "reg_mp_library_func", "Test_Report.test_report_add_image", "const native_int FileHandle, const char* AImagePath, const char* AImageName,const float ASize", &Test_Report.test_report_add_image, "")) return -1;
  //if (!AReg(AObj, "reg_mp_library_func", "Test_Report.test_report_test_result", "PTSTest test, const native_int FileHandle, const char* testcaseName, bool IsPass", &Test_Report.test_report_test_result, "")) return -1;
  //if (!AReg(AObj, "reg_mp_library_func", "Test_Report.test_report_hyperlink", "const native_int Handle, const char* AName, const char* AAddr", &Test_Report.test_report_hyperlink, "")) return -1;
  //if (!AReg(AObj, "reg_mp_library_func", "Test_Report.test_report_cover", "const native_int FileHandle, const char* AWordName, const char* ASNumber, const char* TestECUName, const char* AComName", &Test_Report.test_report_cover, "")) return -1;
  //if (!AReg(AObj, "reg_mp_library_func", "Test_Report.test_export_report", "", &Test_Report.test_export_report, "")) return -1;
  //if (!AReg(AObj, "reg_mp_library_func", "Test_Report.test_init", "const pnative_int Handle,const char* ReportName", &Test_Report.test_init, "")) return -1;
  //if (!AReg(AObj, "reg_mp_library_func", "Test_Report.test_title", "const native_int Handle,const char* testgroup, const char* testcase, const char* image, const char* testpurpose", &Test_Report.test_title, "")) return -1;
  //if (!AReg(AObj, "reg_mp_library_func", "Test_Report.test_step", "const native_int Handle,const char* step,const char* AMsg", &Test_Report.test_step, "")) return -1;
  //if (!AReg(AObj, "reg_mp_library_func", "Test_Report.test_pass", "const native_int Handle,const char* step,const char* AMsg", &Test_Report.test_pass, "")) return -1;
  //if (!AReg(AObj, "reg_mp_library_func", "Test_Report.test_fail", "const native_int Handle,const char* step,const char* AMsg", &Test_Report.test_fail, "")) return -1;
  //if (!AReg(AObj, "reg_mp_library_func", "Test_Report.test_warning", "const native_int Handle,const char* step,const char* AMsg", &Test_Report.test_warning, "0")) return -1;
  //if (!AReg(AObj, "reg_mp_library_func", "Test_Report.test_final", "const native_int Handle", &Test_Report.test_final, "")) return -1;
  //if (!AReg(AObj, "reg_mp_library_func", "Test_Report.test_openissue", "const native_int Handle,const char* openissueinfo", &Test_Report.test_openissue, "")) return -1;
  //if (!AReg(AObj, "reg_mp_library_func", "Test_Report.sddb_to_json", "const char* sddbfile, const char* jsonfile", &Test_Report.sddb_to_json, "")) return -1;
  //
  
  if (!AReg(AObj, "reg_mp_library_func", "test_report.test_init", "const pnative_int Handle", &test_report.test_init, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "test_report.test_set_report_name", "const native_int Handle, const char* reportName", &test_report.test_set_report_name, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "test_report.test_title", "const native_int Handle,const char* testgroup, const char* testcase, const char* image, const char* testpurpose", &test_report.test_title, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "test_report.test_step", "const native_int Handle,const char* step,const char* AMsg", &test_report.test_step, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "test_report.test_pass", "const native_int Handle,const char* step,const char* AMsg", &test_report.test_pass, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "test_report.test_fail", "const native_int Handle,const char* step,const char* AMsg", &test_report.test_fail, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "test_report.test_warning", "const native_int Handle,const char* step,const char* AMsg", &test_report.test_warning, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "test_report.test_final", "const native_int Handle", &test_report.test_final, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "test_report.test_openissue", "const native_int Handle,const char* openissueinfo", &test_report.test_openissue, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "test_report.test_testinfo", "const native_int Handle, const char* ProductName, const char* HWVersion, const char* SWVersion, const char* DUTVersion, const char* TestEngineer, const char* ManufacturerName", &test_report.test_testinfo, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "test_report.test_set_watermark", "const native_int Handle,const char* AWaterMark", &test_report.test_set_watermark, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "test_report.test_set_html_name_and_logo", "const native_int Handle,const char* AHtmlName,const char* ALogoName", &test_report.test_set_html_name_and_logo, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "test_report.sddb_to_json", "const char* sddbfile, const char* jsonfile", &test_report.sddb_to_json, "")) return -1;
 
  // MP Library rtlUIDiagnostics
  if (!AReg(AObj, "reg_mp_library_func", "rtlUIDiagnostics.tsdiag_can_create", "ps32 pDiagModuleIndex, u32 AChnIndex, u8 ASupportFDCAN, u8 AMaxDLC,u32 ARequestID, bool ARequestIDIsStd, u32 AResponseID, bool AResponseIDIsStd, u32 AFunctionID, bool AFunctionIDIsStd", &rtlUIDiagnostics.tsdiag_can_create, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "rtlUIDiagnostics.tsdiag_can_delete", "s32 ADiagModuleIndex", &rtlUIDiagnostics.tsdiag_can_delete, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "rtlUIDiagnostics.tsdiag_can_delete_all", "", &rtlUIDiagnostics.tsdiag_can_delete_all, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "rtlUIDiagnostics.tsdiag_set_p2_timeout", "s32 ADiagModuleIndex, s32 ATimeMs", &rtlUIDiagnostics.tsdiag_set_p2_timeout, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "rtlUIDiagnostics.tsdiag_set_p2_extended", "s32 ADiagModuleIndex, s32 ATimeMs", &rtlUIDiagnostics.tsdiag_set_p2_extended, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "rtlUIDiagnostics.tsdiag_set_s3_servertime", "s32 ADiagModuleIndex, s32 ATimeMs", &rtlUIDiagnostics.tsdiag_set_s3_servertime, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "rtlUIDiagnostics.tsdiag_set_s3_clienttime", "s32 ADiagModuleIndex, s32 ATimeMs", &rtlUIDiagnostics.tsdiag_set_s3_clienttime, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "rtlUIDiagnostics.tstp_send_functional", "s32 ADiagModuleIndex, pu8 AReqDataArray, s32 AReqDataSize", &rtlUIDiagnostics.tstp_send_functional, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "rtlUIDiagnostics.tstp_send_request", "s32 ADiagModuleIndex, pu8 AReqDataArray, s32 AReqDataSize", &rtlUIDiagnostics.tstp_send_request, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "rtlUIDiagnostics.tstp_request_and_get_response", "s32 ADiagModuleIndex, pu8 AReqDataArray, s32 AReqDataSize, pu8 AResponseDataArray, ps32 AResponseDataSize", &rtlUIDiagnostics.tstp_request_and_get_response, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "rtlUIDiagnostics.tstp_request_and_get_response_functional", "s32 ADiagModuleIndex, pu8 AReqDataArray, s32 AReqDataSize, pu8 AResponseDataArray, ps32 AResponseDataSize", &rtlUIDiagnostics.tstp_request_and_get_response_functional, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "rtlUIDiagnostics.tstp_can_send_functional", "s32 ADiagModuleIndex, pu8 AReqDataArray, s32 AReqDataSize", &rtlUIDiagnostics.tstp_can_send_functional, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "rtlUIDiagnostics.tstp_can_send_request", "s32 ADiagModuleIndex, pu8 AReqDataArray, s32 AReqDataSize", &rtlUIDiagnostics.tstp_can_send_request, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "rtlUIDiagnostics.tstp_can_request_and_get_response", "s32 ADiagModuleIndex, pu8 AReqDataArray, s32 AReqDataSize, pu8 AResponseDataArray, ps32 AResponseDataSize", &rtlUIDiagnostics.tstp_can_request_and_get_response, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "rtlUIDiagnostics.tstp_can_request_and_get_response_functional", "s32 ADiagModuleIndex, pu8 AReqDataArray, s32 AReqDataSize, pu8 AResponseDataArray, ps32 AResponseDataSize", &rtlUIDiagnostics.tstp_can_request_and_get_response_functional, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "rtlUIDiagnostics.tsdiag_session_control", "s32 ADiagModuleIndex, u8 ASubSession", &rtlUIDiagnostics.tsdiag_session_control, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "rtlUIDiagnostics.tsdiag_communication_control", "s32 ADiagModuleIndex, u8 AControlType", &rtlUIDiagnostics.tsdiag_communication_control, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "rtlUIDiagnostics.tsdiag_routine_control", "s32 ADiagModuleIndex, u8 ARoutineControlType, u16 ARoutintID", &rtlUIDiagnostics.tsdiag_routine_control, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "rtlUIDiagnostics.tsdiag_security_access_request_seed", "s32 ADiagModuleIndex, s32 ALevel, pu8 ARecSeed, ps32 ARecSeedSize", &rtlUIDiagnostics.tsdiag_security_access_request_seed, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "rtlUIDiagnostics.tsdiag_security_access_send_key", "s32 ADiagModuleIndex, s32 ALevel, pu8 AKeyValue, s32 AKeySize", &rtlUIDiagnostics.tsdiag_security_access_send_key, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "rtlUIDiagnostics.tsdiag_request_download", "s32 ADiagModuleIndex, u32 AMemAddr, u32 AMemSize", &rtlUIDiagnostics.tsdiag_request_download, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "rtlUIDiagnostics.tsdiag_request_upload", "s32 ADiagModuleIndex, u32 AMemAddr, u32 AMemSize", &rtlUIDiagnostics.tsdiag_request_upload, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "rtlUIDiagnostics.tsdiag_transfer_data", "s32 ADiagModuleIndex, pu8 ASourceData, s32 ADataSize, s32 AReqCase", &rtlUIDiagnostics.tsdiag_transfer_data, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "rtlUIDiagnostics.tsdiag_request_transfer_exit", "s32 ADiagModuleIndex", &rtlUIDiagnostics.tsdiag_request_transfer_exit, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "rtlUIDiagnostics.tsdiag_write_data_by_identifier", "s32 ADiagModuleIndex, u16 ADataIdentifier, pu8 AWriteData, s32 AWriteDataSize", &rtlUIDiagnostics.tsdiag_write_data_by_identifier, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "rtlUIDiagnostics.tsdiag_read_data_by_identifier", "s32 ADiagModuleIndex, u16 ADataIdentifier, pu8 AReturnArray, ps32 AReturnArraySize", &rtlUIDiagnostics.tsdiag_read_data_by_identifier, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "rtlUIDiagnostics.tsdiag_can_session_control", "s32 ADiagModuleIndex, u8 ASubSession", &rtlUIDiagnostics.tsdiag_can_session_control, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "rtlUIDiagnostics.tsdiag_can_communication_control", "s32 ADiagModuleIndex, u8 AControlType", &rtlUIDiagnostics.tsdiag_can_communication_control, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "rtlUIDiagnostics.tsdiag_can_routine_control", "s32 ADiagModuleIndex, u8 ARoutineControlType, u16 ARoutintID", &rtlUIDiagnostics.tsdiag_can_routine_control, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "rtlUIDiagnostics.tsdiag_can_security_access_request_seed", "s32 ADiagModuleIndex, s32 ALevel, pu8 ARecSeed, ps32 ARecSeedSize", &rtlUIDiagnostics.tsdiag_can_security_access_request_seed, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "rtlUIDiagnostics.tsdiag_can_security_access_send_key", "s32 ADiagModuleIndex, s32 ALevel, pu8 AKeyValue, s32 AKeySize", &rtlUIDiagnostics.tsdiag_can_security_access_send_key, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "rtlUIDiagnostics.tsdiag_can_request_download", "s32 ADiagModuleIndex, u32 AMemAddr, u32 AMemSize", &rtlUIDiagnostics.tsdiag_can_request_download, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "rtlUIDiagnostics.tsdiag_can_request_upload", "s32 ADiagModuleIndex, u32 AMemAddr, u32 AMemSize", &rtlUIDiagnostics.tsdiag_can_request_upload, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "rtlUIDiagnostics.tsdiag_can_transfer_data", "s32 ADiagModuleIndex, pu8 ASourceData, s32 ADataSize, s32 AReqCase", &rtlUIDiagnostics.tsdiag_can_transfer_data, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "rtlUIDiagnostics.tsdiag_can_request_transfer_exit", "s32 ADiagModuleIndex", &rtlUIDiagnostics.tsdiag_can_request_transfer_exit, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "rtlUIDiagnostics.tsdiag_can_write_data_by_identifier", "s32 ADiagModuleIndex, u16 ADataIdentifier, pu8 AWriteData, s32 AWriteDataSize", &rtlUIDiagnostics.tsdiag_can_write_data_by_identifier, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "rtlUIDiagnostics.tsdiag_can_read_data_by_identifier", "s32 ADiagModuleIndex, u16 ADataIdentifier, pu8 AReturnArray, ps32 AReturnArraySize", &rtlUIDiagnostics.tsdiag_can_read_data_by_identifier, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "rtlUIDiagnostics.tsdiag_set_is_valid", "s32 ADiagModuleIndex, bool AIsValid", &rtlUIDiagnostics.tsdiag_set_is_valid, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "rtlUIDiagnostics.tsdiag_set_fdmode", "s32 ADiagModuleIndex, bool AFDMode, bool ASupportBRS, s32 AMaxDLC", &rtlUIDiagnostics.tsdiag_set_fdmode, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "rtlUIDiagnostics.tsdiag_lin_create", "ps32 pDiagModuleIndex, u32 AChnIndex, u8 ANad", &rtlUIDiagnostics.tsdiag_lin_create, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "rtlUIDiagnostics.tstp_lin_set_run_with_normal_schedule_table", "s32 ADiagModuleIndex, bool ADiagRunWithNormalScheduleTable", &rtlUIDiagnostics.tstp_lin_set_run_with_normal_schedule_table, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "rtlUIDiagnostics.tstp_lin_tp_para_default", "s32 ADiagModuleIndex, u16 AReqIntervalMs, u16 AResIntervalMs, u16 AResRetryTime", &rtlUIDiagnostics.tstp_lin_tp_para_default, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "rtlUIDiagnostics.tstp_lin_tp_para_special", "s32 ADiagModuleIndex, u16 AReqIntervalMs, u16 AResIntervalMs, u16 AResRetryTime", &rtlUIDiagnostics.tstp_lin_tp_para_special, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "rtlUIDiagnostics.tsdiag_doip_create", "ps32 pDiagModuleIndex, s32 AToolType, u32 AChnIndex, char* ATesterIP, u16 ATesterPort, char* ADUTIP, u16 ADUTPort, u32 ARequestID, u32 AResponseID, u32 AFunctionID", &rtlUIDiagnostics.tsdiag_doip_create, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "rtlUIDiagnostics.tsdiag_doip_connect", "s32 ADiagModuleIndex", &rtlUIDiagnostics.tsdiag_doip_connect, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "rtlUIDiagnostics.tsdiag_doip_routing_activation", "s32 ADiagModuleIndex, u8 AActivateType", &rtlUIDiagnostics.tsdiag_doip_routing_activation, "")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "rtlUIDiagnostics.tsdiag_doip_disconnect", "s32 ADiagModuleIndex", &rtlUIDiagnostics.tsdiag_doip_disconnect, "")) return -1;
  // MP Library tsdisturb
  if (!AReg(AObj, "reg_mp_library_func", "tsdisturb.tsdisturb_scan", "", &tsdisturb.tsdisturb_scan, "0")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "tsdisturb.tsdisturb_get_device_serial", "int index, u8 serial[8]", &tsdisturb.tsdisturb_get_device_serial, "0")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "tsdisturb.tsdisturb_connect_can", "int index, s32* handle, int arb_bitrate, int arb_sp, int data_bitrate, int data_sp", &tsdisturb.tsdisturb_connect_can, "0")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "tsdisturb.tsdisturb_disconnect", "s32 handle", &tsdisturb.tsdisturb_disconnect, "0")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "tsdisturb.tsdisturb_disconnect_all", "", &tsdisturb.tsdisturb_disconnect_all, "0")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "tsdisturb.tsdisturb_stop", "s32 handle", &tsdisturb.tsdisturb_stop, "0")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "tsdisturb.tsdisturb_query", "s32 handle, u32* times", &tsdisturb.tsdisturb_query, "0")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "tsdisturb.tsdisturb_can_sample_point_test", "s32 handle, u32 can_id, s32 test_times, s32 timeout, int guest_arbit_sample_pos, int guest_data_sample_pos, int delay_bus_d_to_rx, int delay_bus_r_to_rx, float* arb_sp, float* data_sp", &tsdisturb.tsdisturb_can_sample_point_test, "0")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "tsdisturb.tsdisturb_can_disturb_start", "s32 handle, const void* trigger, void* sequence, void* repetitions, void* options", &tsdisturb.tsdisturb_can_disturb_start, "0")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "tsdisturb.tsdisturb_set_trigger_frame", "void* trigger, void* filter, void* start_pos", &tsdisturb.tsdisturb_set_trigger_frame, "0")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "tsdisturb.tsdisturb_set_trigger_io", "void* trigger, void* io, void* start_pos", &tsdisturb.tsdisturb_set_trigger_io, "0")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "tsdisturb.tsdisturb_set_trigger_error_frame", "void* trigger, void* start_pos", &tsdisturb.tsdisturb_set_trigger_error_frame, "0")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "tsdisturb.tsdisturb_set_sequence", "void* sequence, void* config", &tsdisturb.tsdisturb_set_sequence, "0")) return -1;
  if (!AReg(AObj, "reg_mp_library_func", "tsdisturb.tsdisturb_set_sequence_can_frame", "void* sequence, TCANFD* frame", &tsdisturb.tsdisturb_set_sequence_can_frame, "0")) return -1;
  return 1;
}
