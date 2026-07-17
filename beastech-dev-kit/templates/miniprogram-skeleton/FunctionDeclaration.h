#ifndef FUNCTION_DECLARATION
#define FUNCTION_DECLARATION


//GENERAL_FUNCTION
//void TestInfoPrint(const char* descriptionsBuffer, ...);
//void TestInfoPrintPass(const char* descriptionsBuffer, ...);
//void TestInfoPrintFail(const char* descriptionsBuffer, ...);
//void TestInfoPrintWarning(const char* descriptionsBuffer, ...);
//void TestStep(const char* descriptionsBuffer, ...);
//void TestStepPass(const char* descriptionsBuffer, ...);
//void TestStepFail(const char* descriptionsBuffer, ...);
//void TestStepWarning(const char* descriptionsBuffer, ...);

std::string lowerChange(std::string str1);
void RegexSpace(std::string& str1, std::string str2);


//TEST_DEVICES
class rigol_multimeter;
class main_power;
class bipolar_power;
class boardcard;
//enum Power;
void SetVoltage(double V,double I);
//void KL30(enum Power status);
//void KL15(enum Power status);
//void BoardCardInit_Devices();

//void SetBoardCardInit_Devices();
void DevicesInit();
s32 DevicesCreat();

//TEST_FUNCTION
typedef struct _signalinfo Tsignalinfo, * Psignalinfo;
typedef struct _Single TSingle, * PSingle;
typedef struct _SingleGroup TSingleGroup, * PSingleGroup;
typedef struct _ARXMLE2ESG TARXMLE2ESG, * PARXMLE2ESG;
typedef struct _MSGID TMSGID, * PMSGID;
typedef struct DTCStorge;
typedef std::vector<u8>CRCdata;
class ParsingINI;
void __stdcall OnAllCANMessage(const pnative_int pobj, const PCANFD ACANFD);
void __stdcall OnPreCANMessage(const pnative_int pobj, const PCANFD ACANFD);
void SIMNMMessage();
void StopSIMNMMessage();
void WakeUpDUT();
void DUTSleep();
void InitAllFlag();
void ClearAllVector();
void SetBoardCardInit();
void REG();
void UNREG();
//void DiagConnectionCreate();
void GetInfoByDataBase(std::vector<Tsignalinfo>& signalInfo_Func, std::vector<TMSGID>& MSGID_Func, std::vector<TDBFrameProperties>& framebuffer);
void SetRxPeriod_DBC(std::vector<TARXMLE2ESG>& DUT_def_E2E_SG_RX_Func);
void GetSGFromArxml(std::vector<TSingleGroup>& SGGetFromArxml_Func, std::vector<TARXMLE2ESG>& DUT_def_E2E_SG_TX_Func);
void SignalRearrangement(std::vector<TARXMLE2ESG>& DUT_def_E2E_SG_Func);
void E2EInfoGet();
int GetParameter(std::string inipath);
void Pretest();
void PreE2Econdition();
void TestFinish();
//void CaseFinish();
template<typename Value>
//bool CheckValueWithStandard(Value TestValue, Value Min, Value Max, char name[]);
int WaitFlag(int& Flag, int waittime);
//void WaitMessage(s32 ID, int waittime = 2000);
//void WaitMessage_TX(s32 ID, int waittime = 2000);
void TiggerDUTSendE2E_TXMsg();
void ChangeMsgToCRCData(TCANFD UnderCheckMsg, std::vector<TARXMLE2ESG>& DUT_def_E2E_SG_TX);
u8 CRCCheck(CRCdata data);
void CheckFirstCRC(std::vector<TARXMLE2ESG>& DUT_def_E2E_SG_TX);
void MessageWait(int Number, int waittime);
void CheckCRCNumber(int StartNumber, int StopNumber, std::vector<TARXMLE2ESG>& DUT_def_E2E_SG_TX);
void CheckCounterNumber(int StartNumber, int StopNumber, std::vector<TARXMLE2ESG>& DUT_def_E2E_SG_TX);
//void SID_14_ClearDTC();
//void SID_19_ReadDTCByMask(u8 Mask);
//void storgeDTC();
//u8 CheckDTCNotRecord(u32 DTC);
//u8 CheckDTCRecord(u32 DTC);
void SetSignalValue(const PCANFD Msg, int i, int j, int k, u32 value , std::vector<TARXMLE2ESG>& DUT_def_E2E_SG_RX);
void SetSignalValue(TCANFD& Msg, int i, int j, int k, u8 value , std::vector<TARXMLE2ESG>& DUT_def_E2E_SG_RX);
void SIME2EMsgByID(u32 MsgID, int Channel, std::vector<TARXMLE2ESG>& DUT_def_E2E_SG_RX);
void SIME2EMsgCycle(std::vector<TARXMLE2ESG>& DUT_def_E2E_SG_RX);
void SIMErrorE2EMsgCycle(u32 MsgID);
void SIMConstCounterMsgCycle(u32 MsgID);
void SetEIPowerLevel(int Level);
void SetCarMode(int Mode);
void SetCarMode(std::string Mode);
void SetUsageMode(int Mode);
void SetUsageMode(std::string Mode);
void CompareDTCStatusIncluded(u8 StandardMask, u8 TestMask);
void CompareDTCStatusNotIncluded(u8 StandardMask, u8 TestMask);
void FulfilConfirm(int confirmedDTCLimit);
void FulfilTRC();
void FulfilTRC_OnPreTX(const PCANFD Msg, std::vector<TARXMLE2ESG>& DUT_def_E2E_SG_RX);
void CalculateCntrAndCRC_OnPreTX(const PCANFD Msg, std::vector<TARXMLE2ESG>& DUT_def_E2E_SG_RX);
void SetUpdateBit(const PCANFD Msg, std::vector<TARXMLE2ESG>& DUT_def_E2E_SG_RX);
void SetErrorCRC(const PCANFD Msg, int i, int j, int k, std::vector<TARXMLE2ESG>& DUT_def_E2E_SG_RX);
void SetConstCntr(const PCANFD Msg, int i, int j, int k, std::vector<TARXMLE2ESG>& DUT_def_E2E_SG_RX);
//void printpasscrc(const PCANFD Msg);
void TurnToErrorCRC(const PCANFD Msg, std::vector<TARXMLE2ESG>& DUT_def_E2E_SG_RX);
void TurnToConstCntr(const PCANFD Msg, std::vector<TARXMLE2ESG>& DUT_def_E2E_SG_RX);
void UBCheck(TCANFD Msg, std::vector<TARXMLE2ESG>& DUT_def_E2E_SG_TX);

//E2E
void E2EInfoGet();


//NM
void pre_test();
void post_test();
void check_com(u8 m, u8 type);
bool check_diff_no_info(s64 Tbig, s64 Tsmall, double expect_diff_min, double expect_diff_max);
bool check_diff(s64 Tbig, s64 Tsmall, double expect_diff_min, double expect_diff_max);
void set_InterestPNC_1(u8 PNC, u8* test_data);
void handle_ini(const char* ini_path);
void __stdcall	On_RX_CAN_Msg(const pnative_int pobj, const PCANFD ACANFD);
void __stdcall On_TX_CAN_Msg(const pnative_int pobj, const PCANFD ACANFD);

s32 FindTRCPath();
s32 SimulateTRC(bool);

void Read_NM_INI(const char* ini_path);

#endif