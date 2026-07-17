#include "TSMaster.h"
#include "MPLibrary.h"
#include "Configuration.h"
#include "TSMasterBaseInclude.h"
#include <memory>
#include <string>
#include <vector>
//以下为自定义外部函数
extern void Pretest();
extern void TestFinish();
extern void SDDB_Parser();
extern s32 Pretestcase();

//diag server
extern s32 test_case_1350868();
extern s32 test_case_1350870();
extern s32 test_case_1022421();
extern s32 test_case_1350878();
extern s32 test_case_1350898();
extern s32 test_case_1088261();
extern s32 test_case_1350943();
extern s32 test_case_1350947();
extern s32 test_case_1350949();
extern s32 test_case_1351011();
extern s32 test_case_1351013();
extern s32 test_case_1351015();
extern s32 test_case_1022401();
extern s32 test_case_1022403();
extern s32 test_case_1022495();
extern s32 test_case_1022762();
extern s32 test_case_1022504();
extern s32 test_case_1350890();
extern s32 test_case_1350891();
extern s32 test_case_1350892();
extern s32 test_case_1350893();
//TO
extern s32 test_case_1999999();
extern s32 TC_Check_Sofeware_Version();

//NVM
extern s32 TC_LV_Write_NVM_Data_NVM_001();
extern s32 TC_HV_Write_NVM_Data_NVM_002();
extern s32 TC_Check_NVM_Data_Loss_Sleep_Wake_NVM_003();
extern s32 TC_Check_NVM_Write_Power_Reset_NVM_004();
extern s32 TC_Check_NVM_Access_Power_Reset_NVM_005();
extern s32 TC_Check_NVM_Data_Loss_While_Read_NVM_006();
extern s32 TC_Check_NVM_Write_High_CPU_Load_NVM_007();
extern s32 TC_Check_NVM_Write_Success_Hard_Reset_NVM_011();
extern s32 TC_Check_NVM_Access_Hard_Reset_NVM_012();
extern s32 TC_Check_NVM_Write_Version_Upgrade_NVM_015();
extern s32 TC_Check_NVM_Access_Version_Upgrade_NVM_016();

//SWDL
extern s32 test_case_1021823();
extern s32 test_case_1021932();
extern s32 test_case_boot_selfUpdate();
extern s32 test_case_1021825();
extern s32 test_case_1021841();
extern s32 test_case_1365394();


//E2E
extern s32 TC1023183_CRC_by_sender_CANCANFD();
extern s32 TC1023184_Counter_by_sender_CANCANFD();
extern s32 TC1023185_Counter_by_sender_CANCANFD_short_recovery();
extern s32 TC1023186_CRC_by_Receiver_on_CANCANFD();
extern s32 TC1023187_Counter_by_Receiver_on_CANCANFD();
extern s32 TC1023188_E2E_Confirmed_DTC_By_CRC_on_CANCANFD();
extern s32 TC1023189_E2E_Confirmed_DTC_By_Counter_on_CANCANFD();
extern s32 TC1023190_E2E_DTC_TRC_EIPowerLevel_on_CANCANFD();
extern s32 TC1023191_E2E_DTC_TRC_Voltage_on_CANCANFD();
extern s32 TC1023192_E2E_DTC_TRC_Car_Mode_on_CANCANFD();
extern s32 TC1023193_E2E_DTC_TRC_Car_Mode_Transition_on_CANCANFD();
extern s32 TC1023194_E2E_DTC_TRC_Usage_Mode_on_CANCANFD();
extern s32 TC1023195_E2E_DTC_TRC_Usage_Mode_Transition_on_CANCANFD();

//NM
extern s32 TC_1023218_CAN_NM_PNC_in_Mask_wakeup_and_down();
extern s32 TC_Check_CAN_Message();
extern s32 TC_Check_LIN_Schedule();
extern s32 TC_Sleep_And_WakeUp();
extern s32 TC_1023210_CAN_NM_Frame_ID_out_of_Range_not_wakeup();
extern s32 TC_1023219_CAN_NM_PNC_in_Mask_keep_awake();
extern s32 TC_1023220_CAN_NM_PNC_out_of_Mask_not_wakeup();
extern s32 TC_1023221_CAN_NM_PNC_out_of_Mask_not_keep_awake();
extern s32 TC_1023212_CAN_Network_Management_PDU_network_wakeup();
//extern s32 TC_1322254_WakeUpTimeTestFromMasterNode_WakeUpSignal();

extern s32 test_case_request_seed_NextCycle();
extern s32 test_case_request_seed_PowerReset();
extern s32 test_case_request_seed();
extern s32 test_case_request_seed_NextCycle_Program();
extern s32 test_case_request_seed_PowerReset_Program();
extern s32 test_case_Check_Wakeup_Time_NetworkWakeup();
extern s32 test_case_Check_Wakeup_Time_PowerReset();
extern s32 test_case_Seed_Incorrect_Key();
extern s32 test_case_Seed_D01C();
extern s32 test_case_Check_Signature();
extern s32 test_case_Check_Read_C00X();
extern s32 test_case_Check_Read_C00A();
extern s32 test_case_Check_Read_C00B();

extern s32 CS_Test_001();
extern s32 CS_Test_002();
extern s32 CS_Test_003();
extern s32 CS_Test_004();
extern s32 CS_Test_005();
extern s32 CS_Test_006();
extern s32 CS_Test_007();
extern s32 CS_Test_008();
extern s32 CS_Test_009();
extern s32 CS_Test_010();
extern s32 CS_Test_011();
extern s32 CS_Test_012();
extern s32 CS_Test_013();
extern s32 CS_Test_014();
extern s32 CS_Test_015();
extern s32 CS_Test_016();
extern s32 CS_Test_017();
extern s32 CS_Test_018();
extern s32 CS_Test_019();
extern s32 CS_Test_020();
extern s32 CS_Test_021();
extern s32 CS_Test_022();
extern s32 CS_Test_023();
extern s32 CS_Test_024();
extern s32 CS_Test_025();
extern s32 CS_Test_026();
extern s32 CS_Test_027();
extern s32 CS_Test_028();
extern s32 CS_Test_029();
extern s32 CS_Test_030();
extern s32 CS_Test_031();
extern s32 CS_Test_032();
extern s32 CS_Test_033();
extern s32 CS_Test_034();
extern s32 CS_Test_035();
extern s32 CS_Test_036();
extern s32 CS_Test_037();
extern s32 CS_Test_038();
extern s32 CS_Test_039();
extern s32 CS_Test_040();
extern s32 CS_Test_041();
extern s32 CS_Test_005_007_052_059();
extern s32 CS_Test_053_060();
extern s32 CS_Test_054_061();
extern s32 CS_Test_055_062();
extern s32 CS_Test_042();
extern s32 CS_Test_043();
extern s32 CS_Test_044();
extern s32 CS_Test_045();
extern s32 CS_Test_046();
extern s32 CS_Test_047();
extern s32 CS_Test_048();
extern s32 CS_Test_049();
extern s32 CS_Test_050();
extern s32 CS_Test_051();
extern s32 CS_Test_056();
extern s32 CS_Test_057();
extern s32 CS_Test_058();
extern s32 CS_Test_063();
extern s32 CS_Test_064();
extern s32 CS_Test_065();
extern s32 CS_Test_066();
extern s32 CS_Test_067();
extern s32 CS_Test_068();
extern s32 CS_Test_069();
extern s32 CS_Test_070();
extern s32 CS_Test_071();
extern s32 CS_Test_072();
extern s32 CS_Test_073();
extern s32 CS_Test_074();
extern s32 CS_Test_075();
extern s32 CS_Test_076();
extern s32 CS_Test_077();
extern s32 CS_Test_078();
extern s32 CS_Test_079();
extern s32 CS_Test_080();
extern s32 CS_Test_081();
extern s32 CS_Test_082();
extern s32 CS_Test_083();
extern s32 CS_Test_084();
extern s32 CS_Test_085();
extern s32 CS_Test_086();
extern s32 CS_Test_087();
extern s32 CS_Test_088();
extern s32 CS_Test_089();
extern s32 CS_Test_090();
extern s32 CS_Test_091();
extern s32 CS_Test_092();
extern s32 CS_Test_093();

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
Ttest_report test_report;
TrtlUIDiagnostics rtlUIDiagnostics; 
TrtlTSInstrument rtlTSInstrument;

// Timers defintions
extern TMPTimerMS NewTimer1;
extern TMPTimerMS CCPTimer;

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
//extern void on_tcp_listen_NewOn_TCP_Listen1(const native_int ASocket, const native_int AClientSocket, const native_int AResult);
extern void on_tcp_connect_NewOn_TCP_Connect1(const native_int ASocket, const native_int AResult);
extern void on_tcp_close_NewOn_TCP_Close1(const native_int ASocket, const native_int AResult);
extern void on_tcp_receive_NewOn_TCP_Receive1(const native_int ASocket, const native_int AResult, const u32 AAddr, const u32 APort, const pu8 AData, const native_int ASize);
extern void on_tcp_transmit_NewOn_TCP_Transmit1(const native_int ASocket, const native_int AResult, const pu8 AData, const native_int ASize);
extern void on_udp_receive_from_NewOn_UDP_Receive_From1(const native_int ASocket, const native_int AResult, const u32 AAddr, const u32 APort, const pu8 AData, const native_int ASize);
extern void on_udp_transmit_to_NewOn_UDP_Transmit_To1(const native_int ASocket, const native_int AResult, const pu8 AData, const native_int ASize);
extern void on_var_change_On_TestSystem_CsvStatus(void);
extern void on_timer_NewOn_Timer1(void);
extern void on_timer_On_CCPTimer(void);
extern void on_start_NewOn_Start1(void);
extern void on_stop_NewOn_Stop1(void);
DLLEXPORT s32 __stdcall retrieve_mp_abilities(const void* AObj, const TRegTSMasterFunction AReg) {
  #define TSMASTER_VERSION "2024.6.7.1126"

  //以下为自定义外部函数
  if (!AReg(AObj, "on_custom_callback", "Pretest", "void", reinterpret_cast<const void*>(&Pretest), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TestFinish", "void", reinterpret_cast<const void*>(&TestFinish), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "SDDB_Parser", "void", reinterpret_cast<const void*>(&SDDB_Parser), "")) return -1;
  


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
  //if (!AReg(AObj, "on_tcp_listen_callback", "on_tcp_listen_NewOn_TCP_Listen1", "0,192.168.1.111:1234", reinterpret_cast<const void*>(&on_tcp_listen_NewOn_TCP_Listen1), "")) return -1;
  if (!AReg(AObj, "on_tcp_connect_callback", "on_tcp_connect_NewOn_TCP_Connect1", "0,192.168.1.111:1234", reinterpret_cast<const void*>(&on_tcp_connect_NewOn_TCP_Connect1), "")) return -1;
  if (!AReg(AObj, "on_tcp_close_callback", "on_tcp_close_NewOn_TCP_Close1", "0,192.168.1.111:1234", reinterpret_cast<const void*>(&on_tcp_close_NewOn_TCP_Close1), "")) return -1;
  if (!AReg(AObj, "on_tcp_receive_callback", "on_tcp_receive_NewOn_TCP_Receive1", "0,192.168.1.111:1234", reinterpret_cast<const void*>(&on_tcp_receive_NewOn_TCP_Receive1), "")) return -1;
  if (!AReg(AObj, "on_tcp_transmit_callback", "on_tcp_transmit_NewOn_TCP_Transmit1", "0,192.168.1.111:1234", reinterpret_cast<const void*>(&on_tcp_transmit_NewOn_TCP_Transmit1), "")) return -1;
  if (!AReg(AObj, "on_udp_receive_from_callback", "on_udp_receive_from_NewOn_UDP_Receive_From1", "0,192.168.1.111:1234", reinterpret_cast<const void*>(&on_udp_receive_from_NewOn_UDP_Receive_From1), "")) return -1;
  if (!AReg(AObj, "on_udp_transmit_to_callback", "on_udp_transmit_to_NewOn_UDP_Transmit_To1", "0,192.168.1.111:1234", reinterpret_cast<const void*>(&on_udp_transmit_to_NewOn_UDP_Transmit_To1), "")) return -1;
  if (!AReg(AObj, "on_var_change_callback", "on_var_change_On_TestSystem_CsvStatus", "TestSystem.CsvStatus", reinterpret_cast<const void*>(&on_var_change_On_TestSystem_CsvStatus), "")) return -1;
  if (!AReg(AObj, "on_timer_callback", "on_timer_On_CCPTimer", "CCPTimer", reinterpret_cast<const void*>(&on_timer_On_CCPTimer), "")) return -1;
  if (!AReg(AObj, "on_timer_callback", "on_timer_NewOn_Timer1", "NewTimer1", reinterpret_cast<const void*>(&on_timer_NewOn_Timer1), "")) return -1;
  if (!AReg(AObj, "on_start_callback", "on_start_NewOn_Start1", "", reinterpret_cast<const void*>(&on_start_NewOn_Start1), "")) return -1;
  if (!AReg(AObj, "on_stop_callback", "on_stop_NewOn_Stop1", "", reinterpret_cast<const void*>(&on_stop_NewOn_Stop1), "")) return -1;
  // MP library functions
  // MP Library Test_Report
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
  //if (!AReg(AObj, "reg_mp_library_func", "Test_Report.TestReport_PrintFileName", "const char* sddbfile, const char* jsonfile", &Test_Report.sddb_to_json, "")) return -1;
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
  if (!AReg(AObj, "reg_mp_library_func", "rtlUIDiagnostics.tsdiag_set_filled_byte", "s32 ADiagModuleIndex, u8 AFilledByte", &rtlUIDiagnostics.tsdiag_set_filled_byte, "")) return -1;

  ////diag server
  //if (!AReg(AObj, "on_custom_callback", "TC_LV_Write_NVM_Data_NVM_001", "void", reinterpret_cast<const void*>(&TC_LV_Write_NVM_Data_NVM_001), "")) return -1;
  //if (!AReg(AObj, "on_custom_callback", "TC_HV_Write_NVM_Data_NVM_002", "void", reinterpret_cast<const void*>(&TC_HV_Write_NVM_Data_NVM_002), "")) return -1;
  //if (!AReg(AObj, "on_custom_callback", "TC_Check_NVM_Data_Loss_Sleep_Wake_NVM_003", "void", reinterpret_cast<const void*>(&TC_Check_NVM_Data_Loss_Sleep_Wake_NVM_003), "")) return -1;
  //if (!AReg(AObj, "on_custom_callback", "TC_Check_NVM_Write_Power_Reset_NVM_004", "void", reinterpret_cast<const void*>(&TC_Check_NVM_Write_Power_Reset_NVM_004), "")) return -1;
  //if (!AReg(AObj, "on_custom_callback", "TC_Check_NVM_Access_Power_Reset_NVM_005", "void", reinterpret_cast<const void*>(&TC_Check_NVM_Access_Power_Reset_NVM_005), "")) return -1;
  //if (!AReg(AObj, "on_custom_callback", "TC_Check_NVM_Data_Loss_While_Read_NVM_006", "void", reinterpret_cast<const void*>(&TC_Check_NVM_Data_Loss_While_Read_NVM_006), "")) return -1;
  //if (!AReg(AObj, "on_custom_callback", "TC_Check_NVM_Write_High_CPU_Load_NVM_007", "void", reinterpret_cast<const void*>(&TC_Check_NVM_Write_High_CPU_Load_NVM_007), "")) return -1;
  //if (!AReg(AObj, "on_custom_callback", "TC_Check_NVM_Write_Success_Hard_Reset_NVM_011", "void", reinterpret_cast<const void*>(&TC_Check_NVM_Write_Success_Hard_Reset_NVM_011), "")) return -1;
  //if (!AReg(AObj, "on_custom_callback", "TC_Check_NVM_Access_Hard_Reset_NVM_012", "void", reinterpret_cast<const void*>(&TC_Check_NVM_Access_Hard_Reset_NVM_012), "")) return -1;
  //if (!AReg(AObj, "on_custom_callback", "TC_Check_NVM_Write_Version_Upgrade_NVM_015", "void", reinterpret_cast<const void*>(&TC_Check_NVM_Write_Version_Upgrade_NVM_015), "")) return -1;
  //if (!AReg(AObj, "on_custom_callback", "TC_Check_NVM_Access_Version_Upgrade_NVM_016", "void", reinterpret_cast<const void*>(&TC_Check_NVM_Access_Version_Upgrade_NVM_016), "")) return -1;



  if (!AReg(AObj, "on_custom_callback", "Pretestcase", "void", reinterpret_cast<const void*>(&Pretestcase), "")) return -1;
  //SWDL
  if (!AReg(AObj, "on_custom_callback", "test_case_1021823", "void", &test_case_1021823, "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "test_case_1021932", "void", reinterpret_cast<const void*>(&test_case_1021932), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "test_case_boot_selfUpdate", "void", reinterpret_cast<const void*>(&test_case_boot_selfUpdate), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "test_case_1021825", "void", reinterpret_cast<const void*>(&test_case_1021825), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "test_case_1021841", "void", reinterpret_cast<const void*>(&test_case_1021841), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "test_case_1365394", "void", reinterpret_cast<const void*>(&test_case_1365394), "")) return -1;

  //diag server
  if (!AReg(AObj, "on_custom_callback", "test_case_1350868", "void", reinterpret_cast<const void*>(&test_case_1350868), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "test_case_1350870", "void", reinterpret_cast<const void*>(&test_case_1350870), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "test_case_1022421", "void", reinterpret_cast<const void*>(&test_case_1022421), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "test_case_1350878", "void", reinterpret_cast<const void*>(&test_case_1350878), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "test_case_1350898", "void", reinterpret_cast<const void*>(&test_case_1350898), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "test_case_1088261", "void", reinterpret_cast<const void*>(&test_case_1088261), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "test_case_1350943", "void", reinterpret_cast<const void*>(&test_case_1350943), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "test_case_1350947", "void", reinterpret_cast<const void*>(&test_case_1350947), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "test_case_1350949", "void", reinterpret_cast<const void*>(&test_case_1350949), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "test_case_1351011", "void", reinterpret_cast<const void*>(&test_case_1351011), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "test_case_1351013", "void", reinterpret_cast<const void*>(&test_case_1351013), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "test_case_1351015", "void", reinterpret_cast<const void*>(&test_case_1351015), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "test_case_1022401", "void", reinterpret_cast<const void*>(&test_case_1022401), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "test_case_1022403", "void", reinterpret_cast<const void*>(&test_case_1022403), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "test_case_1022495", "void", reinterpret_cast<const void*>(&test_case_1022495), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "test_case_1022762", "void", reinterpret_cast<const void*>(&test_case_1022762), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "test_case_1022504", "void", reinterpret_cast<const void*>(&test_case_1022504), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "test_case_1350890", "void", reinterpret_cast<const void*>(&test_case_1350890), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "test_case_1350891", "void", reinterpret_cast<const void*>(&test_case_1350891), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "test_case_1350892", "void", reinterpret_cast<const void*>(&test_case_1350892), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "test_case_1350893", "void", reinterpret_cast<const void*>(&test_case_1350893), "")) return -1;

  


  //E2E
  if (!AReg(AObj, "on_custom_callback", "TC1023183_CRC_by_sender_CANCANFD", "", &TC1023183_CRC_by_sender_CANCANFD, "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TC1023184_Counter_by_sender_CANCANFD", "", &TC1023184_Counter_by_sender_CANCANFD, "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TC1023185_Counter_by_sender_CANCANFD_short_recovery", "", &TC1023185_Counter_by_sender_CANCANFD_short_recovery, "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TC1023186_CRC_by_Receiver_on_CANCANFD", "", &TC1023186_CRC_by_Receiver_on_CANCANFD, "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TC1023187_Counter_by_Receiver_on_CANCANFD", "", &TC1023187_Counter_by_Receiver_on_CANCANFD, "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TC1023188_E2E_Confirmed_DTC_By_CRC_on_CANCANFD", "", &TC1023188_E2E_Confirmed_DTC_By_CRC_on_CANCANFD, "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TC1023189_E2E_Confirmed_DTC_By_Counter_on_CANCANFD", "", &TC1023189_E2E_Confirmed_DTC_By_Counter_on_CANCANFD, "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TC1023190_E2E_DTC_TRC_EIPowerLevel_on_CANCANFD", "", &TC1023190_E2E_DTC_TRC_EIPowerLevel_on_CANCANFD, "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TC1023191_E2E_DTC_TRC_Voltage_on_CANCANFD", "", &TC1023191_E2E_DTC_TRC_Voltage_on_CANCANFD, "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TC1023192_E2E_DTC_TRC_Car_Mode_on_CANCANFD", "", &TC1023192_E2E_DTC_TRC_Car_Mode_on_CANCANFD, "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TC1023193_E2E_DTC_TRC_Car_Mode_Transition_on_CANCANFD", "", &TC1023193_E2E_DTC_TRC_Car_Mode_Transition_on_CANCANFD, "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TC1023194_E2E_DTC_TRC_Usage_Mode_on_CANCANFD", "", &TC1023194_E2E_DTC_TRC_Usage_Mode_on_CANCANFD, "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TC1023195_E2E_DTC_TRC_Usage_Mode_Transition_on_CANCANFD", "", &TC1023195_E2E_DTC_TRC_Usage_Mode_Transition_on_CANCANFD, "")) return -1;
  //NVM
  if (!AReg(AObj, "on_custom_callback", "TC_LV_Write_NVM_Data_NVM_001", "void", reinterpret_cast<const void*>(&TC_LV_Write_NVM_Data_NVM_001), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TC_HV_Write_NVM_Data_NVM_002", "void", reinterpret_cast<const void*>(&TC_HV_Write_NVM_Data_NVM_002), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TC_Check_NVM_Data_Loss_Sleep_Wake_NVM_003", "void", reinterpret_cast<const void*>(&TC_Check_NVM_Data_Loss_Sleep_Wake_NVM_003), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TC_Check_NVM_Write_Power_Reset_NVM_004", "void", reinterpret_cast<const void*>(&TC_Check_NVM_Write_Power_Reset_NVM_004), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TC_Check_NVM_Access_Power_Reset_NVM_005", "void", reinterpret_cast<const void*>(&TC_Check_NVM_Access_Power_Reset_NVM_005), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TC_Check_NVM_Data_Loss_While_Read_NVM_006", "void", reinterpret_cast<const void*>(&TC_Check_NVM_Data_Loss_While_Read_NVM_006), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TC_Check_NVM_Write_High_CPU_Load_NVM_007", "void", reinterpret_cast<const void*>(&TC_Check_NVM_Write_High_CPU_Load_NVM_007), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TC_Check_NVM_Write_Success_Hard_Reset_NVM_011", "void", reinterpret_cast<const void*>(&TC_Check_NVM_Write_Success_Hard_Reset_NVM_011), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TC_Check_NVM_Access_Hard_Reset_NVM_012", "void", reinterpret_cast<const void*>(&TC_Check_NVM_Access_Hard_Reset_NVM_012), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TC_Check_NVM_Write_Version_Upgrade_NVM_015", "void", reinterpret_cast<const void*>(&TC_Check_NVM_Write_Version_Upgrade_NVM_015), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TC_Check_NVM_Access_Version_Upgrade_NVM_016", "void", reinterpret_cast<const void*>(&TC_Check_NVM_Access_Version_Upgrade_NVM_016), "")) return -1;

  //NM
  if (!AReg(AObj, "on_custom_callback", "TC_1023218_CAN_NM_PNC_in_Mask_wakeup_and_down", "void", reinterpret_cast<const void*>(&TC_1023218_CAN_NM_PNC_in_Mask_wakeup_and_down), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TC_Sleep_And_WakeUp", "void", reinterpret_cast<const void*>(&TC_Sleep_And_WakeUp), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TC_Check_LIN_Schedule", "void", reinterpret_cast<const void*>(&TC_Check_LIN_Schedule), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TC_Check_CAN_Message", "void", reinterpret_cast<const void*>(&TC_Check_CAN_Message), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TC_1023210_CAN_NM_Frame_ID_out_of_Range_not_wakeup", "void", reinterpret_cast<const void*>(&TC_1023210_CAN_NM_Frame_ID_out_of_Range_not_wakeup), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TC_1023219_CAN_NM_PNC_in_Mask_keep_awake", "void", reinterpret_cast<const void*>(&TC_1023219_CAN_NM_PNC_in_Mask_keep_awake), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TC_1023220_CAN_NM_PNC_out_of_Mask_not_wakeup", "void", reinterpret_cast<const void*>(&TC_1023220_CAN_NM_PNC_out_of_Mask_not_wakeup), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TC_1023221_CAN_NM_PNC_out_of_Mask_not_keep_awake", "void", reinterpret_cast<const void*>(&TC_1023221_CAN_NM_PNC_out_of_Mask_not_keep_awake), "")) return -1;
  //if (!AReg(AObj, "on_custom_callback", "TC_1322254_WakeUpTimeTestFromMasterNode_WakeUpSignal", "void", reinterpret_cast<const void*>(&TC_1322254_WakeUpTimeTestFromMasterNode_WakeUpSignal), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TC_1023212_CAN_Network_Management_PDU_network_wakeup", "void", reinterpret_cast<const void*>(&TC_1023212_CAN_Network_Management_PDU_network_wakeup), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "TC_Check_Sofeware_Version", "void", reinterpret_cast<const void*>(&TC_Check_Sofeware_Version), "")) return -1;

  if (!AReg(AObj, "on_custom_callback", "test_case_request_seed_NextCycle", "void", reinterpret_cast<const void*>(&test_case_request_seed_NextCycle), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "test_case_request_seed_PowerReset", "void", reinterpret_cast<const void*>(&test_case_request_seed_PowerReset), "")) return -1;

  if (!AReg(AObj, "on_custom_callback", "test_case_request_seed", "void", reinterpret_cast<const void*>(&test_case_request_seed), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "test_case_request_seed_NextCycle_Program", "void", reinterpret_cast<const void*>(&test_case_request_seed_NextCycle_Program), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "test_case_request_seed_PowerReset_Program", "void", reinterpret_cast<const void*>(&test_case_request_seed_PowerReset_Program), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "test_case_Check_Wakeup_Time_NetworkWakeup", "void", reinterpret_cast<const void*>(&test_case_Check_Wakeup_Time_NetworkWakeup), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "test_case_Check_Wakeup_Time_PowerReset", "void", reinterpret_cast<const void*>(&test_case_Check_Wakeup_Time_PowerReset), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "test_case_Seed_Incorrect_Key", "void", reinterpret_cast<const void*>(&test_case_Seed_Incorrect_Key), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "test_case_Seed_D01C", "void", reinterpret_cast<const void*>(&test_case_Seed_D01C), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "test_case_Check_Signature", "void", reinterpret_cast<const void*>(&test_case_Check_Signature), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "test_case_Check_Read_C00X", "void", reinterpret_cast<const void*>(&test_case_Check_Read_C00X), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "test_case_Check_Read_C00A", "void", reinterpret_cast<const void*>(&test_case_Check_Read_C00A), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "test_case_Check_Read_C00B", "void", reinterpret_cast<const void*>(&test_case_Check_Read_C00B), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_001", "void", reinterpret_cast<const void*>(&CS_Test_001), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_002", "void", reinterpret_cast<const void*>(&CS_Test_002), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_003", "void", reinterpret_cast<const void*>(&CS_Test_003), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_004", "void", reinterpret_cast<const void*>(&CS_Test_004), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_005", "void", reinterpret_cast<const void*>(&CS_Test_005), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_006", "void", reinterpret_cast<const void*>(&CS_Test_006), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_007", "void", reinterpret_cast<const void*>(&CS_Test_007), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_008", "void", reinterpret_cast<const void*>(&CS_Test_008), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_009", "void", reinterpret_cast<const void*>(&CS_Test_009), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_010", "void", reinterpret_cast<const void*>(&CS_Test_010), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_011", "void", reinterpret_cast<const void*>(&CS_Test_011), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_012", "void", reinterpret_cast<const void*>(&CS_Test_012), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_013", "void", reinterpret_cast<const void*>(&CS_Test_013), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_014", "void", reinterpret_cast<const void*>(&CS_Test_014), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_015", "void", reinterpret_cast<const void*>(&CS_Test_015), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_016", "void", reinterpret_cast<const void*>(&CS_Test_016), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_017", "void", reinterpret_cast<const void*>(&CS_Test_017), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_018", "void", reinterpret_cast<const void*>(&CS_Test_018), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_019", "void", reinterpret_cast<const void*>(&CS_Test_019), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_020", "void", reinterpret_cast<const void*>(&CS_Test_020), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_021", "void", reinterpret_cast<const void*>(&CS_Test_021), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_022", "void", reinterpret_cast<const void*>(&CS_Test_022), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_023", "void", reinterpret_cast<const void*>(&CS_Test_023), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_024", "void", reinterpret_cast<const void*>(&CS_Test_024), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_025", "void", reinterpret_cast<const void*>(&CS_Test_025), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_026", "void", reinterpret_cast<const void*>(&CS_Test_026), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_027", "void", reinterpret_cast<const void*>(&CS_Test_027), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_028", "void", reinterpret_cast<const void*>(&CS_Test_028), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_029", "void", reinterpret_cast<const void*>(&CS_Test_029), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_030", "void", reinterpret_cast<const void*>(&CS_Test_030), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_031", "void", reinterpret_cast<const void*>(&CS_Test_031), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_032", "void", reinterpret_cast<const void*>(&CS_Test_032), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_033", "void", reinterpret_cast<const void*>(&CS_Test_033), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_034", "void", reinterpret_cast<const void*>(&CS_Test_034), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_035", "void", reinterpret_cast<const void*>(&CS_Test_035), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_036", "void", reinterpret_cast<const void*>(&CS_Test_036), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_037", "void", reinterpret_cast<const void*>(&CS_Test_037), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_038", "void", reinterpret_cast<const void*>(&CS_Test_038), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_039", "void", reinterpret_cast<const void*>(&CS_Test_039), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_040", "void", reinterpret_cast<const void*>(&CS_Test_040), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_041", "void", reinterpret_cast<const void*>(&CS_Test_041), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_005_007_052_059", "void", reinterpret_cast<const void*>(&CS_Test_005_007_052_059), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_053_060", "void", reinterpret_cast<const void*>(&CS_Test_053_060), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_054_061", "void", reinterpret_cast<const void*>(&CS_Test_054_061), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_055_062", "void", reinterpret_cast<const void*>(&CS_Test_055_062), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_042", "void", reinterpret_cast<const void*>(&CS_Test_042), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_043", "void", reinterpret_cast<const void*>(&CS_Test_043), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_044", "void", reinterpret_cast<const void*>(&CS_Test_044), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_045", "void", reinterpret_cast<const void*>(&CS_Test_045), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_046", "void", reinterpret_cast<const void*>(&CS_Test_046), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_047", "void", reinterpret_cast<const void*>(&CS_Test_047), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_048", "void", reinterpret_cast<const void*>(&CS_Test_048), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_049", "void", reinterpret_cast<const void*>(&CS_Test_049), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_050", "void", reinterpret_cast<const void*>(&CS_Test_050), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_051", "void", reinterpret_cast<const void*>(&CS_Test_051), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_056", "void", reinterpret_cast<const void*>(&CS_Test_056), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_057", "void", reinterpret_cast<const void*>(&CS_Test_057), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_058", "void", reinterpret_cast<const void*>(&CS_Test_058), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_063", "void", reinterpret_cast<const void*>(&CS_Test_063), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_064", "void", reinterpret_cast<const void*>(&CS_Test_064), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_065", "void", reinterpret_cast<const void*>(&CS_Test_065), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_066", "void", reinterpret_cast<const void*>(&CS_Test_066), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_067", "void", reinterpret_cast<const void*>(&CS_Test_067), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_068", "void", reinterpret_cast<const void*>(&CS_Test_068), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_069", "void", reinterpret_cast<const void*>(&CS_Test_069), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_070", "void", reinterpret_cast<const void*>(&CS_Test_070), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_071", "void", reinterpret_cast<const void*>(&CS_Test_071), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_072", "void", reinterpret_cast<const void*>(&CS_Test_072), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_073", "void", reinterpret_cast<const void*>(&CS_Test_073), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_074", "void", reinterpret_cast<const void*>(&CS_Test_074), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_075", "void", reinterpret_cast<const void*>(&CS_Test_075), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_076", "void", reinterpret_cast<const void*>(&CS_Test_076), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_077", "void", reinterpret_cast<const void*>(&CS_Test_077), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_078", "void", reinterpret_cast<const void*>(&CS_Test_078), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_079", "void", reinterpret_cast<const void*>(&CS_Test_079), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_080", "void", reinterpret_cast<const void*>(&CS_Test_080), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_081", "void", reinterpret_cast<const void*>(&CS_Test_081), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_082", "void", reinterpret_cast<const void*>(&CS_Test_082), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_083", "void", reinterpret_cast<const void*>(&CS_Test_083), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_084", "void", reinterpret_cast<const void*>(&CS_Test_084), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_085", "void", reinterpret_cast<const void*>(&CS_Test_085), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_086", "void", reinterpret_cast<const void*>(&CS_Test_086), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_087", "void", reinterpret_cast<const void*>(&CS_Test_087), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_088", "void", reinterpret_cast<const void*>(&CS_Test_088), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_089", "void", reinterpret_cast<const void*>(&CS_Test_089), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_090", "void", reinterpret_cast<const void*>(&CS_Test_090), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_091", "void", reinterpret_cast<const void*>(&CS_Test_091), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_092", "void", reinterpret_cast<const void*>(&CS_Test_092), "")) return -1;
  if (!AReg(AObj, "on_custom_callback", "CS_Test_093", "void", reinterpret_cast<const void*>(&CS_Test_093), "")) return -1;

  return 1;
}