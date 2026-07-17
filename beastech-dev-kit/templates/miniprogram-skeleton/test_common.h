#ifndef TEST_COMMON
#define TEST_COMMON

#include <map>
#include <TSMaster.h>
#include <string>
#include <atomic>  // 加入此行
#include<stdarg.h>
#include<fstream>
#include<regex>
#include<sstream>

//enum TestResult { TestOK = 0, TestNOK = 1, TestWarning = 2, TestNA = 3, Pass, Fail, Info, Warning };
//TestResult Test_Result;

//VISA
#define VT_AGILENT_VISA 1
#define VT_NORMAL_VISA 0

//定义上下电参数
typedef enum CtrlStatus {
    OFF = 0,
    ON = 1,
    //off = 0,
    //on = 1
}CtrlStatus;

enum Power { on, off };
//定义终端电阻
typedef enum CANTerminalR { Res0, Res60, Res120 }CANTerminalR;
typedef enum CANRCLoad { Rx0C0, Rx0Cmin, Rx0Cmax, Rx180Cmin, Rx180Cmax }CANRCLoad;

// 定义数据库类型
enum EnumDBType {
    DB_TYPE_DBC,
    DB_TYPE_ARXML
};

// 全局变量，根据实际加载的文件动态设置
EnumDBType g_CurrentDBType = DB_TYPE_ARXML;

//诊断公共信息
//诊断ID 
u8 Diag_ch = CH2;
//std::string TpBusName_Temp = "ZCU_CANFD2";
int  gTpBusFlag = 2;//CAN:1 CANFD:2 LIN:3 FlexRay:4 Ethernet:5
char TpBusType_Temp[20] = "CANFD";
char TpBusName_Temp[32] = "ZCU_CANFD2";
char TpNMBusName_Temp[32] = "ZCU_CANFD1";
char DUTName[50] = "";
int gCEMFlg = 1; //0:No,1:Yes DUT不是ZCUDM则配置No
u8 Diag_IFFD = 1;
u16 Diag_FD_MAX_LEN = 15;
s32 Diag_Request_ID = 0x701;
u8 Diag_Req_IF_STR = 1;
s32 Diag_Response_ID = 0x601;
u8 Diag_Res_IF_STR = 1;
s32 Diag_Function_ID = 0x7FF;
u8 Diag_Fun_IF_STR = 1;
u32 Diag_ECUInitTime = 2000;//ECU上电或复位后所需的初始化时间
u16 Diag_SepTimeStep = 20;//诊断请求时间间隔
u32 Diag_DelayTimer = 60000;//超过最大解锁次数后的超时时间
s64 currentMessageTime = 0;
volatile bool checkFirstMessageFlag = false;
s32 NM_WakeUp_ID = 0x53F;

//Check NM Wakeup
bool FirstNMMessageFlg = true;
bool CheckLINMessageFlg = true;
s64 FirstNMMessageTime = 0;
s64 FirstLINMessageTime[10] = { 0 };
s64 WakeUpTime = 130; //ms

int NVMCount = 3;

int SecurityUnlockCycle = 10;
int SecurityUnlockTimes = 10000;

s32 gLowPwrCur = 10;//ma
s32 gSleepTime = 40000;//ms

//

//LIN诊断
u8 NAD = 0x47;
 
// 
// 
//诊断网络层参数
u16 Diag_TP_N_As = 1000;
u16 Diag_TP_N_Ar = 1000;
u16 Diag_TP_N_Bs = 1000;
u16 Diag_TP_N_Br = 20;
u16 Diag_TP_N_Cs = 20;
u16 Diag_TP_N_Cr = 1000;
u16 Diag_TP_FC_BS = 00;
u16 Diag_TP_FC_STmin = 20;
//
//诊断会话层参数
u16 Diag_UDS_P2Server = 50;
u16 Diag_UDS_P2Server_Delay = 500;
u16 Diag_UDS_P2Server_Extend = 50;
u16 Diag_UDS_P2Server_Delay_Extend = 500;
u16 Diag_UDS_P2Server_Program = 25;
u16 Diag_UDS_P2Server_Delay_Program = 500;
u16 Diag_UDS_P2Client = 50;
u16 Diag_UDS_P2Clent_Delay = 200;
u16 Diag_UDS_S3Server = 5000;
u16 Diag_UDS_S3Client = 2000;
//

//
u32 errorFrameCount = 0;

int counterVehMn = 0;
int counterVehSpd = 0;
int cntrEngSts = 0;
int cntrIDcDcActLoSide = 0;
int counterVehMnGlSa = 0;

const int C00A_Size = 9;
const int C00A_Count = 10;
const int C00B_Size = 8;
const int C00B_Count = 20;
const int C00C_Size = 13;
const int C00C_Count = 10;
const int C00E_Size = 7;
const int C00E_Count = 20;
const int C00F_Size = 9;
const int C00F_Count = 20;
const int C012_Size = 15;
const int C012_Count = 10;


/*------------variables of CCP--------------*/
char CarConfigBus_Temp[30] = "ZCU_CANFD1";
std::vector<std::string> CarConfigSignalGroup = { {"VehCfgPrm"},{"VehCfgPrmExt"} ,{"VehCfgPrmExt2"} ,{"ListOfNodAv"} };
int ActualCarConfigCount = 0;;
u8 gCCPData[600];
u8 gCCPDataExt[600];
u8 gCCPDataExt2[600];
u32 gCCPDataListOfNodAv[8];

int gBolockId = 0;
int gBolockExtId = 0;
int gBolockExt2Id = 0;
bool g_CCPIsCanFD = false;

bool CarConfigFlg = true;
bool carConfigEndFlg = false;
enum CarConfigType { MissFrame, MissResponse, FaultResponse, SlaveFault };
CarConfigType carConfigType;

struct _CarConfigSignalGroupData {
    std::string SignalName;
    int StartBit;
    int Length;
};

struct _CarConfigData {
    std::string DBName;
    std::string ECUName;
    std::string FrameName;
    std::string UBSignal;
    _CarConfigSignalGroupData carConfigSignalGroupData[8];
    u16 FFrameIdentifier;
    u16 UBbit;
};

_CarConfigData carConfigData[4];

struct recordCarConfigData
{
    int CCPPsn;
    s32 CCPValue;
};
const int CarConfig_NUM = 1760;
struct recordCarConfigData recordCarConfigDataList[CarConfig_NUM];


int index_cc1 = 0, index_cc2 = 0;
bool lock_cc1 = true, lock_cc2 = true;

//服务列表

//DLC对应数据长度
std::map<unsigned int, unsigned int>CANFD_DLC
{
	{1,1},
	{2,2},
	{3,3},
	{4,4},
	{5,5},
	{6,6},
	{7,7},
	{8,8},
	{9,12},
	{10,16},
	{11,20},
	{12,24},
	{13,32},
	{14,48},
	{15,64}
};

std::map<unsigned int, unsigned int> DataLengthToDLC = {
    {1,1},
    {2,2},
    {3,3},
    {4,4},
    {5,5},
    {6,6},
    {7,7},
    {8,8},
    {12,9},
    {16,10},
    {20,11},
    {24,12},
    {32,13},
    {48,14},
    {64,15}
};

s32 GetDLCLength(u8 DLC)
{
	auto it = CANFD_DLC.find(DLC);

	unsigned int dataLength = it->second;

	return dataLength;
}

//诊断响应NRC映射表
std::map<unsigned int, std::string>NRC
{
	{ 0x10,"一般性拒绝。" },
	{ 0x11,"服务不支持。" },
	{ 0x12,"子功能不支持。" },
	{ 0x13,"报文长度错误或无效格式" },
	{ 0x14,"响应字节数太长" },
	{ 0x21,"服务器繁忙，请稍后再次请求" },
	{ 0x22,"条件不满足" },
	{ 0x24,"请求序列错误" },
	{ 0x25,"子网节点无响应" },
	{ 0x26,"DTC出现了错误的记录，阻止执行所请求的操作" },
	{ 0x31,"请求超出范围" },
	{ 0x33,"安全访问被拒绝" },
	{ 0x35,"无效的安全解锁密钥" },
	{ 0x36,"超过最大解锁次数" },
	{ 0x37,"所需的延迟时间未满足" },
	{ 0x70,"上传下载受限" },
	{ 0x71,"数据传输中断" },
	{ 0x72,"一般编程性错误" },
	{ 0x73,"块序列号错误" },
	{ 0x78,"请求正常接收，响应正在应答中" },
	{ 0x7E,"子功能当前会话下不支持" },
	{ 0x7F,"服务当前会话下不支持" },
	{ 0x81,"编程管理地址过高" },
	{ 0x82,"编程管理地址过低" },
	{ 0x83,"发动机运转" },
	{ 0x84,"发动机未运转" },
	{ 0x85,"发动机运行时间过短" },
	{ 0x86,"温度过高" },
	{ 0x87,"温度过低" },
	{ 0x88,"车速过高" },
	{ 0x89,"车速过低" },
	{ 0x8A,"油门/踏板太高" },
	{ 0x8B,"油门/踏板太低" },
	{ 0x8C,"非空挡" },
	{ 0x8D,"不在指定挡位" },
	{ 0x8F,"踏板开关未关闭" },
	{ 0x90,"车辆处于非P档" },
	{ 0x91,"液力变矩器离合器锁定" },
	{ 0x92,"电压过高" },
	{ 0x93,"电压过低" },
};

/*------------variables of test flag--------------*/
int gTRCfdFlag = 1;
int gCCPfdFlag = 1;
int gVehSpdfdFlag = 1;
int gDTCIndex;
int gWarnFlag = 0;
long gVmmFrameSetUB[65535];
long gUbatFrameSetUB[65535];

/*-----------------DD00-----------------*/
int gCarTiGlbSimFlag = 0;
u32 gCarTiGlb = 0x12345678;
u32 gCarTiGlb_min = 0x00000000;
u32 gCarTiGlb_valid = 0x12345678;
u32 gCarTiGlb_max = 0xFFFFFFFE;
u32 gCarTiGlb_invalid = 0xFFFFFFFF;
/*--------------------------------------*/

/*-----------------DD01-----------------*/
int gBkpOfDstTrvldSimFlag = 0;
u32 gBkpOfDstTrvld = 0x012345;
u32 gBkpOfDstTrvld_min = 0x000000;
u32 gBkpOfDstTrvld_valid = 0x012345;
u32 gBkpOfDstTrvld_max = 0x0F4240;
u32 gBkpOfDstTrvld_invalid = 0x100000;
/*--------------------------------------*/

/*-----------------DD06-----------------*/
int gVehSpdSimFlag = 1;
//u16 gVehSpd = 0x00AA;//0.6647m/s≈2.4km/h<3km/h
u16 gVehSpd = 0x00;
u16 gVehSpd_min = 0x0000;//0m/s
u16 gVehSpd_valid = 0x00AA;//0.6647m/s≈2.4km/h<3km/h
u16 gVehSpd_max = 0x7CE2;//125.0027m/s
u16 gVehSpd_invalid = 0x7EEE;
/*--------------------------------------*/

u8 msgData[64];
u8 Assignment_data[64];
u8 UsgMod_data[64];

u8 gEPLvl, gUagMode, gCarMode;

int gVehManUB = 1;
int gVehBatQf = 3;
int gVehBatVotg = 0x82;
const u8 gUbat8 = 0x50;
const u8 gUbat12 = 0x78;
const u8 gUbat16 = 0xA0;
const u8 gUbat12_Phy = 12;
double gCarTiGlb_Sim = 0;

int StopFlag = 0;
//定义测试相关仿真报文及仿真定时器
TCAN Sim_UsgMode_CAN, Sim_VehBat_CAN, Sim_VehCCP_CAN, Sim_VehCCPExt_CAN, Sim_VehSpd_CAN, Sim_EngSts_CAN, Sim_IDcDcActLoSide_CAN;
TCANFD Sim_CarTiGlb, Sim_BkpOfDstTrvld, Sim_TiAndDateIndn, Sim_VehSpd, Sim_LstOfNod, Sim_UsgMode_CANFD, Sim_VehBat_CANFD, Sim_VehCCP_CANFD, Sim_VehCCPExt_CANFD, Sim_VehSpd_CANFD, Sim_EngSts_CANFD, Sim_IDcDcActLoSide_CANFD;
/*UsgMode : 仿真包含UsgMode信号的报文
  VehBat : 仿真包含车辆蓄电池电压信号的报文
  VehCCP : 仿真包含车辆配置参数信号的报文
  VehSpd : 仿真包含车辆车速信号的报文
  LstOfNod :
  EngSts : 仿真包含车辆引擎信号的报文
  IDcDcActLoSide :
  CarTiGlb :
  BkpOfDstTrvld :
  TiAndDateIndn :
*/


struct voltDTC
{
    int dtc;
    float volt;
    int CofrmLmt;
    int agedLmt;

    int UnCofrmLmt;
    int testProd;
    int SUValue;
    int SDValue;

    int usgMod;
    int tstRlst;
};
struct voltDTC highVoltDTC =
{
    //0xEE0368,16,3,255,6,100,3,6,0,1
      0xEE0368,16,3,5,6,100,3,6,0,1
};
struct voltDTC lowVoltDTC =
{
    //0xEE0468,9,3,255,6,100,3,6,0,1
    0xEE0468,9,3,5,6,100,3,6,0,1
};
struct voltDTC batVoltDTC =
{
    //0xF00362,3,10,255,6,100,3,6,0xD,1
      0xF00362,3,10,5,6,100,3,6,0xD,1
};

struct busFaultDTC
{
    char cluster[100];
    char busType[100];
    u8 highID;
    s32 busOffDTC;
    int Volt;//0: no voltage req; 1: 8V; 2:8V+16V
    int bsfUcfm;
    int bsfCfm;
    int TestRslt;//0: NOK; 1:OK
    s32 ElctrFltDTC;
    int testProd;
    int SUValue;
    int SDvalue;
    int UnCofrmLmt;
    int CofrmLmt;
    int UsgMod;
};
const int BusNum = 1;
struct busFaultDTC busFltDTCTemp[BusNum] = {
    //{"PropulsionCANFD","CAN",0x700,0xDC1C88,2,4,1,1,0,100,3,2,5,3,0}
      //{"ZCU_CANFD2","CANFD",0x700,0xDC7888,2,4,1,1,0,100,13,128,13,127,0}//ZCU 3.5
      { "ZCU_CANFD1","CANFD",0x700,0xDC7788,2,4,1,1,0,100,13,128,13,127,0 }//ZCU 3.5
      //{ "ChassisCANFD2","CANFD",0x700,0xDC1B88,2,4,1,1,0,100,13,128,13,127,0 }//ZCU 3.5
      //{ "ZCUD_CAN1","CAN",0x700,0xDC4088,2,4,1,1,0,100,13,128,13,127,0 }//ZCU 3.5
      //{ "PropulsionCANFD","CAN",0x700,0xDC1C88,2,4,1,1,0,100,13,128,13,127,0 }//ZCU 3.5
      //{ "ChassisCANFD1","CAN",0x700,0xDC1A88,2,4,1,1,0,100,13,128,13,127,0 }//ZCU 3.5
      //{ "BodyExposedCAN","CAN",0x700,0xDC0188,2,4,1,1,0,100,13,128,13,127,0 }//ZCU 3.5
};

struct signalStruct
{
    char cluster[100];
    int msgId;
    int startBit;
    int updateBit;
    u32 value;
    char sgName[100];
    int msgdatalen;
};

TCAN gPtActvnReqMsg, gDrvgCycMsg, gMonrCycCmpl;
TCANFD gPtActvnReqMsg_FD, gDrvgCycMsg_FD, gMonrCycCmpl_FD;

struct signalStruct DrvgAndMonrCyc[3];
struct signalStruct PtActvnReq =
{
  "ChassisCANFD2",326,177,178,0,"isPtActvnReq1WdPtActvnReq",64
};

/*------------variables of CCP--------------*/


struct e2eSignal
{
    char sigName[100];
    int sigLength;
    int startBit;
    int sigValue;
};

struct VehModMng
{
    char cluster[50];
    char FrameName[32];
    s64 FrameCycle;
    u8 dlc;
    char sigGroupName[64];
    s32 slotID;
    s64 baseCycle;
    s64 RepetitionCycle;
    s32 dataID;
    int sigGroup_UB;
    u8 sigNum;
    int pduStartBit;
    struct e2eSignal e2eSignalList[100];
};

struct VehModMng VehModMngtGlbSafe[2] =
{
    //{
    //"PropulsionCANFD","VddmPropFr18",10,8,"igVehModMngtGlbSafe1",86,0,0,116,32,10,0,
    //{
    //  {"isVehModMngtGlbSafe1Chks",8,7,0x0},
    //  {"isVehModMngtGlbSafe1Cntr",4,15,0x0},
    //  {"isVehModMngtGlbSafe1CarModSts1",3,39,0x0},
    //  {"isVehModMngtGlbSafe1CarModSubtypWdCarModSubtyp",3,36,0x0},
    //  {"isVehModMngtGlbSafe1EgyLvlElecMai",4,27,0x0},
    //  {"isVehModMngtGlbSafe1EgyLvlElecSubtyp",4,31,0x0},
    //  {"isVehModMngtGlbSafe1FltEgyCnsWdSts",1,33,0x0},
    //  {"isVehModMngtGlbSafe1PwrLvlElecMai",4,19,0x0},
    //  {"isVehModMngtGlbSafe1PwrLvlElecSubtyp",4,23,0x0},
    //  {"isVehModMngtGlbSafe1UsgModSts",4,11,0x0}
    //}
    //},
    //{
    //  "PropulsionCANFD","VddmPropFr19",10,8,"igVehBattU",645,0,0,0,2,2,0,
    //  {
    //    {"isVehBattUSysU",8,15,0x0},
    //    {"isVehBattUSysUQf",2,1,0x0}
    //  }
    //}
     {
      "ZCU_CANFD1","ZcudZCUCANFD1Fr10",10,64,"igVehModMngtGlbSafe1_7",83,0,0,116,376,10,0,
      {
        {"isVehModMngtGlbSafe1Chks_7",8,351,0x0},
        {"isVehModMngtGlbSafe1Cntr_7",4,359,0x0},
        {"isVehModMngtGlbSafe1CarModSts1_7",3,383,0x0},
        {"isVehModMngtGlbSafe1CarModSubtypWdCarModSubtyp_7",3,380,0x0},
        {"isVehModMngtGlbSafe1EgyLvlElecMai_7",4,355,0x0},
        {"isVehModMngtGlbSafe1EgyLvlElecSubtyp_7",4,367,0x0},
        {"isVehModMngtGlbSafe1FltEgyCnsWdSts_7",1,377,0x0},
        {"isVehModMngtGlbSafe1PwrLvlElecMai_7",4,363,0x0},
        {"isVehModMngtGlbSafe1PwrLvlElecSubtyp_7",4,375,0x0},
        {"isVehModMngtGlbSafe1UsgModSts_7",4,371,0x0}
      }

      },
      {
        "ZCU_CANFD1","ZcudZCUCANFD1Fr09",10,64,"igVehBattU_5",931,0,0,0,59,2,0,
        {
          {"isVehBattUSysU_5",8,15,0x0},
          {"isVehBattUSysUQf_5",2,1,0x0}
        }
      }
};

struct VehModMng VehGlbSafe =
{
    "ZCU_CANFD1","ZcudZCUCANFD1Fr10",10,64,"igVehModMngtGlbSafe1",83,0,0,116,376,10,0,
    {
        {"isVehModMngtGlbSafe1Chks",8,351,0x0},
        {"isVehModMngtGlbSafe1Cntr",4,359,0x0},
        {"isVehModMngtGlbSafe1CarModSts1",3,383,0x0},
        {"isVehModMngtGlbSafe1CarModSubtypWdCarModSubtyp",3,380,0x0},
        {"isVehModMngtGlbSafe1EgyLvlElecMai",4,355,0x0},
        {"isVehModMngtGlbSafe1EgyLvlElecSubtyp",4,367,0x0},
        {"isVehModMngtGlbSafe1FltEgyCnsWdSts",1,377,0x0},
        {"isVehModMngtGlbSafe1PwrLvlElecMai",4,363,0x0},
        {"isVehModMngtGlbSafe1PwrLvlElecSubtyp",4,375,0x0},
        {"isVehModMngtGlbSafe1UsgModSts",4,371,0x0}
    }
};

struct VehModMng VehBattU =
{
    "ZCU_CANFD1","ZcudZCUCANFD1Fr09",400,64,"igVehBattU",931,0,0,0,59,10,0,
    {
      {"isVehBattUSysU",8,15,0x0},
      {"isVehBattUSysUQf",2,1,0x0}
    }
};

struct VehModMng VehMtnSt =
{
  "ZCU_CANFD1","BcmZCUCANFD1Fr16",0,64,"igVehMtnSt_0",130,0,0,54,72,3,0,
  {
    {"isVehMtnStChks_0",8,71,0x0},
    {"isVehMtnStCntr_0",4,79,0x0},
    {"isVehMtnStVehMtnSt_0",3,75,0x0}
  }
};

struct VehModMng VehSpdLgt =
{
  "ZCU_CANFD1","BcmZCUCANFD1Fr16",0,64,"igVehSpdLgt_0",98,0,0,55,18,4,0,
  {
    {"isVehSpdLgtChks_0",8,47,0x0},
    {"isVehSpdLgtCntr_0",4,55,0x0},
    {"isVehSpdLgtA_0",15,31,0x0},
    {"isVehSpdLgtQf_0",2,51,0x0}
  }
};

struct VehModMng EngSts[2] =
{
   {
      "PropulsionCANFD","EcmPropFr24",0,8,"EcmPropSignalIPdu24",75,0,0,137,31,3,0,
      {
        {"isEngSt1WdStsChks_0",8,39,0x0},
        {"isEngSt1WdStsEngSt1WdSts_0",4,43,0x1},
        {"isEngSt1WdStsCntr_0",4,47,0x0}
      }
   },

   {
      "PropulsionCANFD","CddIgmPropFr02",0,8,"CddIgmPropSignalIPdu02",329,0,0,20,20,3,0,
      {
        {"isIDcDcActLoSideCntr_0",4,19,0x0},
        {"isIDcDcActLoSideChks_0",8,31,0x0},
        {"isIDcDcActLoSideIDcDcActLoSide_0",12,39,0x0}
      }
   }
};

//CC报文
//CC报文根据不同DUT，提取不同信息，CCPConfigTemp固定为2，由DUT决定CC报文为PropulsionCANFD还是ZCU_CANFD1
struct VehModMng CCPConfigTemp[2] =
{
    //{
    //  "PropulsionCANFD","VddmPropFr06",0,8,"igVehCfgPrm",565,0,0,0,0,8,0,
    //  {
    //    {"isVehCfgPrmCCPBytePosn6",8,47,0x0},
    //    {"isVehCfgPrmBlkIDBytePosn1",8,7,0x0},
    //    {"isVehCfgPrmCCPBytePosn4",8,31,0x0},
    //    {"isVehCfgPrmCCPBytePosn3",8,23,0x0},
    //    {"isVehCfgPrmCCPBytePosn2",8,15,0x0},
    //    {"isVehCfgPrmCCPBytePosn8",8,63,0x0},
    //    {"isVehCfgPrmCCPBytePosn7",8,55,0x0},
    //    {"isVehCfgPrmCCPBytePosn5",8,39,0x0}
    //  }
    //},
    //{
    //  "PropulsionCANFD","VddmPropFr26",0,8,"igVehCfgPrmExt",566,0,0,0,0,8,0,
    //  {
    //    {"isVehCfgPrmExtCCPBytePosn5",8,39,0x0},
    //    {"isVehCfgPrmExtCCPBytePosn6",8,47,0x0},
    //    {"isVehCfgPrmExtCCPBytePosn2",8,15,0x0},
    //    {"isVehCfgPrmExtCCPBytePosn3",8,23,0x0},
    //    {"isVehCfgPrmExtCCPBytePosn8",8,63,0x0},
    //    {"isVehCfgPrmExtCCPBytePosn4",8,31,0x0},
    //    {"isVehCfgPrmExtCCPBytePosn7",8,55,0x0},
    //    {"isVehCfgPrmExtBlkIDBytePosn1",8,7,0x0}
    //  }
    //}
      {
        "ZCU_CANFD1","DhuZCUCANFD1Fr04",0,64,"igVehCfgPrm_0",373,0,0,0,244,8,0,
        {
          {"isVehCfgPrmCCPBytePosn6_0",8,295,0x0},
          {"isVehCfgPrmBlkIDBytePosn1_0",8,255,0x0},
          {"isVehCfgPrmCCPBytePosn4_0",8,279,0x0},
          {"isVehCfgPrmCCPBytePosn3_0",8,271,0x0},
          {"isVehCfgPrmCCPBytePosn2_0",8,263,0x0},
          {"isVehCfgPrmCCPBytePosn8_0",8,311,0x0},
          {"isVehCfgPrmCCPBytePosn7_0",8,303,0x0},
          {"isVehCfgPrmCCPBytePosn5_0",8,287,0x0}
        }
      },
      {
        "ZCU_CANFD1","DhuZCUCANFD1Fr12",0,64,"igVehCfgPrmExt_0",501,0,0,0,71,8,0,
        {
          {"isVehCfgPrmExtCCPBytePosn5_0",8,39,0x0},
          {"isVehCfgPrmExtCCPBytePosn6_0",8,47,0x0},
          {"isVehCfgPrmExtCCPBytePosn2_0",8,15,0x0},
          {"isVehCfgPrmExtCCPBytePosn3_0",8,23,0x0},
          {"isVehCfgPrmExtCCPBytePosn8_0",8,63,0x0},
          {"isVehCfgPrmExtCCPBytePosn4_0",8,31,0x0},
          {"isVehCfgPrmExtCCPBytePosn7_0",8,55,0x0},
          {"isVehCfgPrmExtBlkIDBytePosn1_0",8,7,0x0}
        }
      }
};

struct signalStruct SlaveSignal =
{
  "PropulsionCANFD",534,22,6,0,"OnBdChrgrSt",8
};
struct signalStruct CarTiGlb =
{
  "PropulsionCANFD",354,7,39,0,"CarTiGlb",8
};
struct signalStruct BkpOfDstTrvld =
{
  "PropulsionCANFD",645,20,21,0,"BkpOfDstTrvld",8
};
struct VehModMng TiAndDateIndn =
{
  "PropulsionCANFD","VcuPropFr05",0,8,"igTiAndDateIndcn",337,0,0,0,7,7,0,
  {
    {"isTiAndDateIndcnDay",5,28,0x0},
    {"isTiAndDateIndcnYr1",7,46,0x0},
    {"isTiAndDateIndcnMins1",6,13,0x0},
    {"isTiAndDateIndcnMth1",4,35,0x0},
    {"isTiAndDateIndcnHr1",5,20,0x0},
    {"isTiAndDateIndcnSec1",6,5,0x0},
    {"isTiAndDateIndcnDataValid",1,6,0x0}
  }
};

/*----------Simualte missing frame variables-----------------*/



std::map<std::string, int> NetworkCluster = {

    {"ZCU_CANFD2",CH2},
    {"ZCU_CANFD1",CH3 },
    {"ChassisCANFD2",CH4},
    {"ZCUD_CAN1",CH5},
    {"PropulsionCANFD",CH6},
    {"ChassisCANFD1",CH7},
    {"ZCU_CANFD3",CH8},
    {"ZCU_CANFD4",CH9 }
    //{"ZCU_CANFD2",CH11},
    //{"ZCU_CANFD1",CH9 },
    //{"ChassisCANFD2",CH13},
    //{"ZCUD_CAN1",CH7},
    //{"PropulsionCANFD",CH5},
    //{"ChassisCANFD1",CH1},
    //{"BodyExposedCAN",CH15}
};

std::map<int,std::string> NetworkCluster_CH = {

    {CH2,"ZCU_CANFD2"},
    {CH3,"ZCU_CANFD1" },
    {CH4,"ChassisCANFD2"},
    {CH5,"ZCUD_CAN1"},
    {CH6,"PropulsionCANFD"},
    {CH7,"ChassisCANFD1"},
    {CH8,"ZCUD_CANFD3" },
    {CH9,"ZCUD_CANFD4" }
    //{CH11,"ZCU_CANFD2"},
    //{CH9,"ZCU_CANFD1" },
    //{CH13,"ChassisCANFD2"},
    //{CH7,"ZCUD_CAN1"},
    //{CH5,"PropulsionCANFD"},
    //{CH1,"ChassisCANFD1"},
    //{CH15,"BodyExposedCAN"}
};


//用于提前结束测试的类
class test_terminate
{
public:
    test_terminate() {}

    ~test_terminate() {}
};

// 用于控制线程是否继续运行的标志
std::atomic<bool> isRunning(true);

struct misMsgSignal
{
    char sigName[100];
    int sigLength;
    int startBit;
    int sigValue;
};
struct misMsg
{
    u32 msgID;
    int baseCycle;
    int RepetitionCycle;

    u32 msgCycTime;
    int msgDataLgth;
    struct misMsgSignal misMsgSignalList[20];
    int ubBit;
};
const int MSG_MAX_NUM = 1;
struct misMsgDTC
{
    char node[100];
    char cluster[100];
    char busType[100];
    u32 dtc;
    struct misMsg misMsgList[MSG_MAX_NUM];

    int testProd;
    int SUValue;
    int SDValue;
    int UnCofrmLmt;
    int CofrmLmt;
    int agdLmt;
    int usgmod;
    int fdFlg;
    int simMsgFlg;
    int simMsgImtFlg;
    int tstRsltFlg;
    int failTime;
    int passTime;
    int CCPPsn[20];
    s32 CCPValue[20];
    int CCPValueNotMet[20];
};
const int MISMSG_DTC_NUM = 16;
struct misMsgDTC misMsgDTCList[MISMSG_DTC_NUM] =
{
  {
    "POT","ZCUD_CAN1","CAN",0xD16B87,
    {
      {
        0x95,0,0,10,8,
        {
          {"TrOpenerSts",0,0,0}
        },
        4
      }
    },
    //160,9,9,18,3,255,0xD,0,0,0,1,5000,5000,
    160,9,9,18,3,5,0xD,0,0,0,1,5000,5000,
    //{1047},{0x2},{0}
    {545},{0x1},{0}
  },

        {
          "RBCM","ChassisCANFD2","CANFD",0xD11887,
          {
            {
              280,0,0,10,64,
              {
                {"BrkBoostErrIndcnReq",0,0,0}
              },
              85
            }
          },
          160,9,9,18,3,255,0xD,1,0,0,1,5000,5000,
          {1011},{0x1},{1}
        },
        {
          "PSCM","ChassisCANFD1","CAN",0xD15187,
          {
            {
              246,0,0,10,8,
              {
                {"UBoostReqBySteerFrnt",0,0,0}
              },
              57
            }
          },
          160,9,9,18,3,255,0x0,0,0,0,1,5000,5000,
          {1052},{0x2},{0}
        },
        {
          "SAS","ChassisCANFD1","CAN",0xD11B87,
          {
            {
              64,0,0,10,8,
              {
                {"SteerWhlSnsr",0,0,0}
              },
              7
            }
          },
          160,9,9,18,3,255,0xD,0,0,0,1,5000,5000,
          {1075},{0x2},{0}
        },
        {
          "SCL","ChassisCANFD1","CAN",0xD11C87,
          {
            {
              608,0,0,10,8,
              {
                {"SteerLockSts",0,0,0}
              },
              14
            }
          },
          160,9,9,18,3,255,0xD,0,0,0,1,5000,5000,
          {1076},{0x2},{0}
        },
        {
          "SUM","ChassisCANFD2","CAN",0xD11D87,
          {
            {
              257,0,0,10,64,
              {
                {"SumActv",0,0,0}
              },
              19
            }
          },
          160,9,9,18,3,255,0xD,0,0,0,1,5000,5000,
          {1080},{0x2},{0}
        },
        {
          "HCML","BodyExposedCAN","CAN",0xD13087,
          {
            {
              124,0,0,10,8,
              {
                {"HdlampLeInpSts1",0,0,0}
              },
              48
            }
          },
          160,9,9,18,3,255,0xD,0,0,0,1,5000,5000,
          {1038},{0x2},{0}
        },
        {
          "HCMR","BodyExposedCAN","CAN",0xD13187,
          {
            {
              127,0,0,10,8,
              {
                {"HdlampRiInpSts1",0,0,0}
              },
              48
            }
          },
          160,9,9,18,3,255,0xD,0,0,0,1,5000,5000,
          {1039},{0x2},{0}
        },
        {
          "SMB","ZCUD_CAN1","CAN",0xD15787,
          {
            {
              587,0,0,10,8,
              {
                {"RowSecLeInclPosAngle",0,0,0}
              },
              13
            }
          },
          160,9,9,18,3,255,0xD,0,0,0,1,5000,5000,
          {1172},{0x2},{0}
        },
        {
          "SRS","ZCUD_CAN1","CAN",0xD10E87,
          {
            {
              33,0,0,10,8,
              {
                {"CrashSts2",0,0,0}
              },
              30
            }
          },
          160,9,9,18,3,255,0xD,0,0,0,1,5000,5000,
          {1045},{0x2},{0}
        },
        {
          "SMBL","ZCUD_CAN1","CAN",0xD17687,
          {
            {
              587,0,0,10,8,
              {
                {"RowSecLeInclPosAngle",0,0,0}
              },
              13
            }
          },
          160,9,9,18,3,255,0xD,0,0,0,1,5000,5000,
          {1231},{0x2},{0}
        },
        {
          "SMBR","ZCUD_CAN1","CAN",0xD17787,
          {
            {
              586,0,0,10,8,
              {
                {"RowSecRiInclPosAngle",0,0,0}
              },
              13
            }
          },
          160,9,9,18,3,255,0xD,0,0,0,1,5000,5000,
          {1232},{0x2},{0}
        },
    //{
    //  "POT","ZCUD_CAN1","CAN",0xD16B87,
    //  {
    //    {
    //      149,0,0,10,8,
    //      {
    //        {"TrOpenerSts",0,0,0}
    //      },
    //      4
    //    }
    //  },
    //  160,9,9,18,3,255,0xD,0,0,0,1,5000,5000,
    //  {1047},{0x2},{0}
    //},
    {
      "RML","ZCUD_CAN1","CAN",0xD11687,
      {
        {
          339,0,0,10,8,
          {
            {"StsForRtrctrRvsbLe",0,0,0}
          },
          8
        }
      },
      160,9,9,18,3,255,0xD,0,0,0,1,5000,5000,
      {1063},{0x2},{0}
    },
    {
      "RMR","ZCUD_CAN1","CAN",0xD11787,
      {
        {
          373,0,0,10,8,
          {
            {"StsForRtrctrRvsbRi",0,0,0}
          },
          8
        }
      },
      160,9,9,18,3,255,0xD,0,0,0,1,5000,5000,
      {1064},{0x2},{0}
    },
    {
      "TRM","ZCUD_CAN1","CAN",0xD16C87,
      {
        {
          208,0,0,10,4,
          {
            {"TrlrPrsnt",0,0,0}
          },
          20
        }
      },
      160,9,9,18,3,255,0xD,0,0,0,1,5000,5000,
      {1083},{0x2},{0}
    },
    {
      "CMD","ZCUD_CAN1","CAN",0xD19187,
      {
        {
          265,0,0,10,8,
          {
            {"SeatMassgRunngDrvr2",0,0,0}
          },
          1
        }
      },
      160,9,9,18,3,255,0xD,0,0,0,1,5000,5000,
      {239},{0x2},{0}
    }
};


// 信号完整路径结构体（用于跨接口传递）
typedef struct {
    // 必须索引（供API调用）
    s32 FDBIndex;         // 数据库索引
    s32 FECUIndex;        // ECU索引
    s32 FFrameIndex;      // 报文索引
    s32 FSignalIndex;     // 信号索引
    u8  FIsTx;            // 方向（1=Tx, 0=Rx）
    u16 FFrameIndentifier;

    // 可读名称（供日志和调试）
    char FDBName[64];     // 数据库名称
    char FECUName[64];    // ECU名称（DUTName）
    char FMsgName[64];    // 报文名称
    char FSignalName[64]; // 信号名称

    // 通道信息
    u32 FChannelMask;     // 支持通道掩码（用于优先级判断）
    u32 FFoundChannel;    // 实际找到的通道号
} TSignalPath;

// 全局路径变量（供其他接口调用）
static TSignalPath g_VehSpdLgt_path = { 0 };
static TSignalPath g_EngSt1WdSts_path = { 0 };
static TSignalPath g_IDcDcActLoSide_path = { 0 };
static TSignalPath g_VehMtnSt_path = { 0 };
static TSignalPath g_VehModMngtGlbSafe1_path = { 0 };
static TSignalPath g_VehUbat_path = { 0 };
static TSignalPath g_CarTiGlb_path = { 0 };
static TSignalPath g_CarConfig1_path = { 0 };
static TSignalPath g_CarConfig2_path = { 0 };
static TSignalPath g_CarConfig3_path = { 0 };

static int g_VehSpdLgt_found = 0;  // 找到标志
static int g_EngSt1WdSts_found = 0;  // 找到标志
static int g_IDcDcActLoSide_found = 0;  // 找到标志
static int g_VehMtnSt_found = 0;  // 找到标志
static int g_VehModMngtGlbSafe1_found = 0;  // 找到标志
static int g_VehUbat_found = 0;  // 找到标志
static int g_CarTiGlb_found = 0;  // 找到标志
static int g_CarConfig1_found = 0;  // 找到标志
static int g_CarConfig2_found = 0;  // 找到标志
static int g_CarConfig3_found = 0;  // 找到标志

// 通道优先级配置（按顺序查找，0=最高优先级）
static const u32 VehSpd_CHANNEL_PRIORITY[] = {
    NetworkCluster["ChassisCANFD2"],
    NetworkCluster["ZCU_CANFD1"]
};
static const int VehSpd_CHANNEL_PRIORITY_COUNT = sizeof(VehSpd_CHANNEL_PRIORITY) / sizeof(VehSpd_CHANNEL_PRIORITY[0]);

// 通道优先级配置（按顺序查找，0=最高优先级）
static const u32 Engsts_CHANNEL_PRIORITY[] = {
    NetworkCluster["PropulsionCANFD"],  // 优先
    NetworkCluster["ZCU_CANFD1"] // 次之
};
static const int Engsts_CHANNEL_PRIORITY_COUNT = sizeof(Engsts_CHANNEL_PRIORITY) / sizeof(Engsts_CHANNEL_PRIORITY[0]);

static const u32 VehMtn_CHANNEL_PRIORITY[] = {
    NetworkCluster["ZCU_CANFD1"],
    NetworkCluster["ZCUD_CAN1"]
};
static const int VehMtn_CHANNEL_PRIORITY_COUNT = sizeof(VehMtn_CHANNEL_PRIORITY) / sizeof(VehMtn_CHANNEL_PRIORITY[0]);


static const u32 CarTiGlb_CHANNEL_PRIORITY[] = {
    NetworkCluster["ZCU_CANFD1"]
};
static const int CarTiGlb_CHANNEL_PRIORITY_COUNT = sizeof(CarTiGlb_CHANNEL_PRIORITY) / sizeof(CarTiGlb_CHANNEL_PRIORITY[0]);

static const u32 CarConfig_CHANNEL_PRIORITY[] = {
    NetworkCluster["ZCU_CANFD1"],
    NetworkCluster["ChassisCANFD2"]
};
static const int CarConfig_CHANNEL_PRIORITY_COUNT = sizeof(CarConfig_CHANNEL_PRIORITY) / sizeof(CarConfig_CHANNEL_PRIORITY[0]);


//支持的DTC
struct AvailableDTC_New
{
    u64 DTC;
    u8 DTC_Byte[3];
};

class TextFileManager {
public:
    // 1. 创建并打开文件 (返回文件流对象，避免频繁打开关闭)
    bool openFile(std::ofstream& outFile, const std::string& filename) {
        outFile.open(filename, std::ios::out | std::ios::trunc); // trunc 确保每次是覆盖创建
        return outFile.is_open();
    }

    // 2. 将字符串写入一行
    void writeLine(std::ofstream& outFile, const std::string& hexStr) {
        if (outFile.is_open()) {
            outFile << hexStr << "\n";
        }
    }
    void write(std::ofstream& outFile, const std::string& hexStr) {
        if (outFile.is_open()) {
            outFile << hexStr;
        }
    }

    // 3. 删除文件
    void deleteFile(const std::string& filename) {
        if (std::remove(filename.c_str()) != 0) {
            std::perror("删除文件失败");
        }
        else {
            //std::cout << "文件已成功删除" << std::endl;
            printf("文件已成功删除");
        }
    }
};
#endif // ! TEST_COMMOON